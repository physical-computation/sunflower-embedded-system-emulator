#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spthr_all.h"
#include "devmtsp.h"
#include "devrtc.h"
#include "devrand.h"
#include "spthr_error.h"
#include "spthr_staticmtable.h"


//	!!!
//	!!!
//TODO: thread creation is not sequential in general. All global
//	structures need to be locked!!!
//	Make all the globals unnamed structs with a lock field.
//	!!!
//	!!!

//TODO: we have two places where we cache the HW map table in SW:
//	the maplist (spthr_maplist[hd,tl]) and the mapstats[]
//	array in each thread state. We should unify these.

ThreadState	*spthr_threadqhd = nil;
ThreadState	*spthr_threadqtl = nil;
RegionMap	*spthr_maplisthd = nil;
RegionMap	*spthr_maplisttl = nil;
int		spthr_nthreads = 0;
char		spthr_initdone = 0;
Maddr		spthr_loweststack;
LibMode		spthr_mode = MODEFLAG;
ulong		spthr_lastmapupdate;
int		spthr_windowsize = SPTHR_MAPUPDATE_USECS;

/*								*/
/*	Array of pointers to register save stacks of all	*/
/*	threads.  This is a tradeoff in implementation		*/
/*	complexity vs. generality: The interrupt handler	*/
/*	assembler code can quickly locate its register save	*/
/*	area, but (1) we statically allocate this large array	*/
/*	(2) An artificial limit is imposed on # of threads.	*/
/*								*/
Maddr		spthr_regstackptrs[MAX_NTHREADS];

struct
{
	Maddr	addr;
	uchar	lock;
}		spthr_mallocmutex;

struct
{
	int 	max;
	int 	who;
	int	oldwho;
}		spthr_mapcounts[MAX_MAPCOUNT];


static void	hdlr_install(void);
static void	install_numaregion(ulong, ulong, char *, int, int, int, int, int, int);
static void	printq(Sleepq *q);
static int	findhome(char *name);




/*	For informational purposes, to track mapping over time:		*/
char		time_map[MAX_MAPCOUNT][8192];
int		time_mapcount = 0;


void
spthr_addglobalstomap(void)
{
	spthr_maptbl_add((Maddr)spthr_regstackptrs,
		sizeof(spthr_regstackptrs), "Maddr spthr_regstackptrs[]",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)spthr_staticmaptable,
		sizeof(spthr_staticmaptable), "StaticMap spthr_staticmaptable[]",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_mallocmutex,
		sizeof(spthr_mallocmutex), "struct spthr_mallocmutex",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_mapcounts,
		sizeof(spthr_mapcounts), "struct spthr_mapcounts",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_threadqhd,
		sizeof(spthr_threadqhd), "ThreadState *spthr_threadqhd",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_threadqtl,
		sizeof(spthr_threadqtl), "ThreadState *spthr_threadqtl",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_maplisthd,
		sizeof(spthr_maplisthd), "RegionMap *spthr_maplisthd",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_maplisttl,
		sizeof(spthr_maplisttl), "RegionMap *spthr_maplisttl",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_nthreads,
		sizeof(spthr_nthreads), "int spthr_nthreads",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_initdone,
		sizeof(spthr_initdone), "char spthr_initdone",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_loweststack,
		sizeof(spthr_loweststack), "Maddr spthr_loweststack",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_mode,
		sizeof(spthr_mode), "LibMode spthr_mode",
		SPTHR_MAP_PUBLIC, -1);
	spthr_maptbl_add((Maddr)&spthr_windowsize,
		sizeof(spthr_windowsize), "int spthr_windowsize",
		SPTHR_MAP_PUBLIC, -1);

	return;
}

ThreadState *
spthr_allocthread(Maddr startpc, Maddr argaddr)
{
	if (spthr_nthreads == MAX_NTHREADS)
	{
		return nil;
	}

	/*	Need things like maxnread in MapStat to be zeroed	*/
	ThreadState	*new = (ThreadState*)calloc(1, sizeof(ThreadState));
	if (new == nil)
	{
		return nil;
	}


	/*								*/
	/*	New thread's stack is STKSIZE bytes below the lowest	*/
	/*	thread stack in this shared address space.		*/
	/*								*/
	//more general, but linear in # threads:
	//new->stkaddr	= spthr_getloweststk() - STKSIZE;

	//less general, but constant time:
	new->stkaddr	= spthr_loweststack - STKSIZE;
	spthr_loweststack = new->stkaddr;


	new->stksize	= STKSIZE;
	new->id		= spthr_nthreads;
	new->status	= STATUS_ACTIVE;
	new->fnaddr	= startpc;
	new->fnargaddr	= argaddr;
	new->privaddr	= 0;
	new->next	= nil;


	//TODO: need a way to ensure that a thread's id field is what is
	//	read from THREADID register
	spthr_regstackptrs[new->id] = (Maddr)new->registers;

	if (DEBUG)
	{
		print("new thread id=%d, regsavestack @ %x\n",
			new->id, (Maddr)new->registers);
	}

	snprintf(new->name, NAMELEN, "ThreadState *ThreadState0x%lx.%d",
		startpc, spthr_nthreads);
	spthr_nthreads++;

	/*								*/
	/*	Threadstate *new is added to map table by thread's	*/
	/*	own trampoline.						*/
	/*								*/

	return new;
}

void
spthr_addthread(ThreadState *new)
{
	if (spthr_threadqtl == nil)
	{
		spthr_threadqtl = spthr_threadqhd = new;
		return;
	}

	spthr_threadqtl->next = new;
	spthr_threadqtl = new;

	return;
}

void
spthr_init(void)
{
	ThreadState	*t;


	hdlr_install();

	/*	Need things like maxnread in MapStat to be zeroed	*/
	t = calloc(1, sizeof(ThreadState));
	if (t == nil)
	{
		spthr_threadfatal(Emalloc);
	}

	t->id		= spthr_nthreads;
	t->status	= STATUS_ACTIVE;
	t->stksize	= STKSIZE;

	/*								*/
	/*	The main thread ends up having the amount of stack	*/
	/*	space at the time of the call to spthr_init, plus	*/
	/*	STKSIZE							*/
	/*								*/
	t->stkaddr	= spthr_getcallerstk();
	spthr_loweststack = t->stkaddr;

	t->fnaddr	= 0;
	t->fnargaddr	= 0;
	t->privaddr	= 0;
	t->next		= nil;
	spthr_addthread(t);

	snprintf(t->name, NAMELEN, "ThreadState *init0x%lx.%d",
		spthr_getcallerpc(), spthr_nthreads);
	spthr_maptbl_add((Maddr)t, sizeof(ThreadState), t->name,
		SPTHR_MAP_PRIVATE, -1);
	spthr_addglobalstomap();
	spthr_nthreads++;

	//TODO: need a way to ensure that a thread's id field is what is
	//	read from THREADID register
	spthr_regstackptrs[t->id] = (Maddr)t->registers;

	if (DEBUG)
	{
		print("done spthr_init, init thread has ID=%d, regsavestack at %x...\n",
			t->id, spthr_regstackptrs[t->id]);
	}

	spthr_tas(&spthr_initdone);
	spthr_spllo();


	return;
}

void
spthr_threadfatal(const char *error)
{
	enum	{BUFLEN = 256};
	char	buf[BUFLEN];

	snprintf(buf, BUFLEN, "sfatal \"%s\"", error);
	spthr_simcmd(buf);

	return;
}

Maddr
spthr_getloweststk(void)
{
	Maddr		stkaddr;
	ThreadState	*t;


	if (spthr_threadqhd == nil)
	{
		spthr_threadfatal(Ethreadq);
	}

	t = spthr_threadqhd;
	stkaddr = spthr_threadqhd->stkaddr;
	t = t->next;
	while (t != nil)
	{
		stkaddr = min(t->stkaddr, stkaddr);
		t = t->next;
	}

	return stkaddr;
}

void
spthr_sleep(Sleepq **q, pthread_mutex_t *mutex)
{
	Sleepq		*tmp;
	Sleepq		*s;


	s = (Sleepq *) malloc(sizeof(Sleepq));
	if (s == nil)
	{
		spthr_threadfatal(Emalloc);
	}
	s->threadid = spthr_myid();

	spthr_maptbl_add((Maddr)s, sizeof(Sleepq), "Sleepq *s",
		SPTHR_MAP_PUBLIC, -1);


if (*q != nil)
{
  printq(*q);
  spthr_threadfatal("sleeping on a non-empty queue");
}


	/*		Add to front of list		*/
	tmp = *q;
	*q = s;
	s->next = tmp;

	/*						*/
	/*	We'll be awoken by timer interrupts	*/
	/*	The rdy flag is set when someone	*/
	/*	unlocks the object on whose Sleepq	*/
	/*	we are dozing.				*/
	/*						*/
	s->rdy = 0;
	
	if (DEBUG)
	{
		print("\tsleeping on queue [0x%x], *q = [0x%x], (*q)->next = [0x%x]\n",
			(ulong)q, (ulong)(*q), (ulong)(*q)->next);
	}

	/*						*/
	/*	If a mutex was passed in, unlock it	*/
	/*	before dozing.				*/
	/*						*/
	if (mutex != nil)
	{
		pthread_mutex_unlock(mutex);
	}

	while (!s->rdy)
	{
		spthr_dosleep();
	}

	if (DEBUG)
	{
		print("\tAwoken from sleep on item at addr [0x%x]...\n", (ulong)s);
	}

	spthr_maptbl_del((Maddr)s);
	free((void *)s);


	return;
}

void
spthr_wakeup(Sleepq **q)
{
	Sleepq		*o = nil;
	Sleepq		*p;


	p = *q;

	if (DEBUG)
	{
		print("\tin spthr_wakeup(): q=[0x%x], *q=[0x%x]...\n",
			(ulong)q, (ulong)p);
		printq(p);
	}

	if (p == nil)
	{
		return;
	}

	/*						*/
	/*	Wake + del the sole item on queue:	*/
	/*	It gets free()'d in spthr_sleep()	*/
	/*						*/
	if (p->next == nil)
	{
		p->rdy = 1;

		/*				*/
		/*	NOTE: p is a copy. 	*/
		/*	Clear actual queue ptr	*/
		/*				*/
		*q = nil;

		return;
	}

printq(p);
spthr_threadfatal("debug: more than one sleeper!");

	/*						*/
	/*	Wake + del the oldest item on queue.	*/
	/*	free()'ing and removal from map table	*/
	/*	is done by sleeper, after waking up.	*/
	/*						*/
	while (p->next != nil)
	{
		o = p;
		p = p->next;
	}
	o->next = nil;
	p->rdy = 1;


	return;
}

void
spthr_maptbl_add(Maddr addr, int size, char *name, int scope, int homenode)
{
	RegionMap	*m;
	int		lrlat = -1, lwlat = -1;

			//TODO: these really shouldnt be hard coded here like this.
			//	instead, we need a more flexible numaregion in sim
			//	(along with the flexible-multi-memory) so that we rather
			//	just specify which of the device's memories we want to
			//	map this into.
// for static-all-memory-local, set both to -1
	int		rrlat = 100, rwlat = 100;

	int		myid = spthr_myid();
	int		mapid = myid;


	m = (RegionMap*)malloc(sizeof(RegionMap));
	if (m == nil)
	{
		spthr_threadfatal(Emalloc);
	}
	m->id = devmtsp_getcount();
	m->addr = addr;
	m->size = size;
	m->next = nil;

	if (spthr_maplisthd == nil)
	{
		spthr_maplisthd = spthr_maplisttl = m;
	}
	else
	{
		spthr_maplisttl->next = m;
		spthr_maplisttl = m;
	}

	if (spthr_mode == SPTHR_LIB_STATIC)
	{
		int	where;


		if (homenode < 0)
		{
			where = findhome(name);
		}
		else
		{
			where = homenode;
		}

		if (where == SPTHR_MAP_INVALID)
		{
			printf("Sfpthr Lib: No entry for \"%s\" in static map table\n", name);
			spthr_threadfatal("Bad static map table");
		}

		if (where == SPTHR_MAP_LOCAL)
		{
			mapid = myid;
		}
		else
		{
			mapid = where;
		}
	}

	if (scope == SPTHR_MAP_PRIVATE)
	{
		install_numaregion(addr, size, name, mapid,
			lrlat, lwlat, rrlat, rwlat, 1);
	}
	else if (scope == SPTHR_MAP_PUBLIC)
	{
		install_numaregion(addr, size, name, mapid,
			lrlat, lwlat, rrlat, rwlat, 0);
	}
	else
	{
		spthr_threadfatal(Ebadscope);
	}

	return;
}

void
spthr_maptbl_del(Maddr addr)
{
	RegionMap	*m = spthr_maplisthd, *p = nil;
return;

	while (m != nil)
	{
		if (m->addr == addr)
		{
			if (p == nil)
			{
				p = spthr_maplisthd;
				spthr_maplisthd = spthr_maplisttl = nil;
				free(p);

				return;
			}

			p->next = m->next;			
			free(m);

			return;
		}

		p = m;
		m = m->next;
	}

	return;
}

RegionMap*
spthr_maptbl_getmapbyid(int id)
{
	RegionMap	*m = spthr_maplisthd;

	while (m != nil)
	{
		if (m->id == id)
		{
			return m;
		}
		m = m->next;
	}

	return nil;
}

int
spthr_maptbl_isvalid(Maddr addr)
{
	RegionMap	*m = spthr_maplisthd;
return 1;

	while (m != nil)
	{
		if (m->addr == addr)
		{
			return 1;
		}
		m = m->next;
	}

	return 0;
}

int
spthr_myid(void)
{
	return *((volatile ulong *)THREADID_ADDR);
}

ThreadState *
spthr_getthreadstate(int threadid)
{
	ThreadState *p;

	p = spthr_threadqhd;
	while (p != NULL)
	{
		if (p->id == threadid)
		{
			return p;
		}
		p = p->next;
	}

	return NULL;
}


void
install_numaregion(ulong start, ulong size, char *name, int id,
	int lrlat, int lwlat, int rrlat, int rwlat, int private)
{
	enum		{buflen = 256};
	uchar		buf[buflen];

	snprintf(buf, buflen, "numaregion \"%.128s\" 0x%lx 0x%lx %d %d %d %d %d 0 %d\n",
		name, start, start+size, lrlat, lwlat, rrlat, rwlat, id, private);
	spthr_simcmd(buf);

	return;
}

void
local_mapstats(void)
{
	int 		i;
	ThreadState	*me;
	int		count = devmtsp_getcount();


	if (count >= MAX_MAPCOUNT)
	{
		spthr_threadfatal(Emaxmaps);
	}

	me = spthr_getthreadstate(spthr_myid());
	if (me == NULL)
	{
		spthr_threadfatal(Ethreadidstate);
	}

	for (i = 0; i < count; i++)
	{
		me->mapstats[i].nreads = devmtsp_clearrdcnt(i);
		me->mapstats[i].nwrites = devmtsp_clearwrcnt(i);

		me->mapstats[i].maxnreads =
			max(me->mapstats[i].maxnreads, me->mapstats[i].nreads);
		me->mapstats[i].maxnwrites =
			max(me->mapstats[i].maxnwrites, me->mapstats[i].nwrites);
	}
	me->mapstats_timestamp = devrtc_getusecs();


	return;
}

/*										*/
/*	updatewindow() and updatepred() work in tandem.  updatewindow()		*/
/*	tries to keep the update window to be the largest such that we		*/
/*	still correctly track the changes in optimal mapping at the 		*/
/*	granularity at which they happen. This is the fine grained window;	*/
/*	The granularity with which we actually change the mapping will be	*/
/*	much larger.								*/
/*										*/
/*	Given that we are tracking the changes in optimum mapping at the	*/
/*	granularity at which they occur, updatepred() tries to track the	*/
/*	distribution of duration of mapping of each map object to each CPU.	*/
/*	e.g., map object A one run of duration 0-50ms, 12 runs of duration	*/
/*	50-100ms and 19 runs of duration >100ms (we could further break this	*/
/*	up by which cpu the map-run corresponds to, but for now we dont).	*/
/*	These statistics give us a "relative frequency" model for the 		*/
/*	probability distribution of run lengths. We can then use this 		*/
/*	distribution to predict which of the mappings from the previous		*/
/*	window will ... (we thus need to track how long a given mapping		*/
/*	has been the best mapping)						*/
/*	Similarly, maintaining the distribution on number of accesses for	*/
/*	which a given mapping is the optimum lets us predict the number of	*/
/*	accesses that will be made in determining the reconf cost vs benefit	*/
/*	tradeoff. 								*/
/*	In using both of the above dists, be random pick from a bag of		*/
/*	colored balls, with the number of balls of each color prop to the	*/
/*	probability								*/
/*										*/
void
updatewindow(void)
{
}

void
updatepred(ThreadState *t, int i, int nreads, int nwrites)
{
	int	coin, naccesses, maxaccesses, minaccesses, scaledrand;
	ulong	rand = devrand_getrand();


	if (spthr_mode == SPTHR_LIB_DYNAMIC_1)
	{
		/*							*/
		/*	Alternative 1: prediction for next window	*/
		/*	is the results from the current window.		*/
		/*	This would fit well with lifetimes of maps	*/
		/*	being heavy tailed				*/
		/*							*/
		/*	old behavior continues	*/
		t->mapstats[i].prednreads = nreads;
		t->mapstats[i].prednwrites = nwrites;
	}
	else if (spthr_mode == SPTHR_LIB_DYNAMIC_2)
	{
		/*							*/
		/*	Alternative 2: assuming number of accesses	*/
		/*	during a window is either high or low, then	*/
		/*	for each prediction, we flip a coin, and if	*/
		/*	it comes up heads, we use either the high	*/
		/*	(max so far) if naccesses in previos window	*/
		/*	was close to the low (min so far) and vice	*/
		/*	versa. This mimics the coin-flipping binomial	*/
		/*	basis for exponential. 				*/
		/*							*/
		coin = rand & 1;
		if (!coin)
		{
			/*	old behavior continues		*/
			t->mapstats[i].prednreads = nreads;
			t->mapstats[i].prednwrites = nwrites;
		}
		else
		{
			/*	old behavior ends		*/
			naccesses = nreads + nwrites;
			maxaccesses = t->mapstats[i].maxnreads + t->mapstats[i].maxnwrites;
			if (naccesses > maxaccesses/2)
			{
				t->mapstats[i].prednreads = t->mapstats[i].minnreads;
				t->mapstats[i].prednwrites = t->mapstats[i].minnwrites;
			}
			else
			{
				t->mapstats[i].prednreads = t->mapstats[i].maxnreads;
				t->mapstats[i].prednwrites = t->mapstats[i].maxnwrites;
			}
		}
	}
	else if (spthr_mode == SPTHR_LIB_DYNAMIC_3)
	{
		/*							*/
		/*	Alternative 3: assuming the duration of map	*/
		/*	lifetimes/#accesses is uniform, predict a	*/
		/*	number of accesses for next period uniformly	*/
		/*	at random between the min # accesses seen and	*/
		/*	max seen to date.				*/
		/*							*/
		maxaccesses = t->mapstats[i].maxnreads + t->mapstats[i].maxnwrites;
		minaccesses = t->mapstats[i].minnreads + t->mapstats[i].minnwrites;
		scaledrand = (rand/(~0L)) * (maxaccesses - minaccesses) + minaccesses;

		t->mapstats[i].prednreads = scaledrand;
		t->mapstats[i].prednwrites = scaledrand;
	}
	else
	{
		spthr_threadfatal(Ebaddynamicmode);
	}

	return;
}


void
map_update(void)
{
	/*	BUG: these shouldnt be hardcoded like this	*/
	int		localweight = 0, remoteweight = 100;

	int		i, j, oldnaccesses, prednaccesses, hwcount;
	float		oldcost, newcost;
	ThreadState	*t;
	char		buf[128];


	hwcount = devmtsp_getcount();

	for (i = 0; i < hwcount; i++)
	{
		RegionMap	*map = spthr_maptbl_getmapbyid(i);

		if (map == nil)
		{
			/*	Not in library map table;  Skip.	*/
			continue;
		}


		spthr_mapcounts[i].oldwho = spthr_mapcounts[i].who;
		spthr_mapcounts[i].who = spthr_mapcounts[i].max = -1;

		oldcost = 0;
		t = spthr_threadqhd;
		while (t != NULL)
		{
			updatepred(t, i, t->mapstats[i].nreads, t->mapstats[i].nwrites);
			oldnaccesses =
				t->mapstats[i].nreads + t->mapstats[i].nwrites;
			prednaccesses =
				t->mapstats[i].prednreads + t->mapstats[i].prednwrites;

			if (t->id == spthr_mapcounts[i].oldwho)
			{
				oldcost += (float)oldnaccesses*(float)localweight
						/(float)spthr_windowsize;
			}
			else
			{
				oldcost += (float)oldnaccesses*(float)remoteweight
						/(float)spthr_windowsize;
			}

			if (prednaccesses > spthr_mapcounts[i].max)
			{
				spthr_mapcounts[i].max = prednaccesses;
				spthr_mapcounts[i].who = t->id;
			}
			t = t->next;
		}

		updatewindow();

		newcost = 0;
		t = spthr_threadqhd;
		while (t != NULL)
		{
			prednaccesses = t->mapstats[i].prednreads + t->mapstats[i].prednwrites;

			if (t->id == spthr_mapcounts[i].who)
			{
				newcost += (float)prednaccesses*(float)localweight
						/(float)spthr_windowsize;
			}
			else
			{
				newcost += (float)prednaccesses*(float)remoteweight
						/(float)spthr_windowsize;

				if (t->id == spthr_mapcounts[i].oldwho)
				{
					/*					*/
					/*	Overhead of DMA'ing set to	*/
					/*	size * cost of remote access	*/
					/*					*/
					newcost += (float)map->size*(float)remoteweight
						/(float)spthr_windowsize;
				}
			}
			t = t->next;
		}
	
		printf("map entry %d,\toldcost = %f, newcost = %f ", i, oldcost, newcost);
		if ((newcost < oldcost) && (spthr_mapcounts[i].who != spthr_mapcounts[i].oldwho))
		{
			printf("(size %ld, moving from %d->%d)",
					map->size, spthr_mapcounts[i].oldwho, spthr_mapcounts[i].who);

			/*	Update the mapping; Locks bus in process	*/
			snprintf(buf, sizeof(buf), "numasetmapid %d %d", i, spthr_mapcounts[i].who);
			spthr_simcmd(buf);
		}
		printf("\n");
	}




for (i = 0; i < hwcount; i++)
{
	time_map[i][time_mapcount] = spthr_mapcounts[i].who;
}
if (time_mapcount == 100) for (i = 0; i < hwcount; i++)
{
	lprint("\t\tentry%d: ", i);
	for (j = 0; j < time_mapcount; j++)
	{
		lprint("%d ", time_map[i][j]);
	}
	lprint("\n");
}
time_mapcount++;


	return;
}

void
intr_hdlr(void)
{
	ulong		now;
	ThreadState	*me;


	if (spthr_mode == SPTHR_LIB_STATIC)
	{
		return;
	}

	now = devrtc_getusecs();

	me = spthr_getthreadstate(spthr_myid());
	if (me == NULL)
	{
		spthr_threadfatal(Ethreadidstate);
	}

	/*								*/
	/*	Ideally we'd like the map_update computation to be	*/
	/*	parallelized across all CPUs.  Each CPU's intr hdlr	*/
	/*	(i.e., us) reads its specific entries in the MTSP,	*/
	/*	into its thread state structure.  			*/
	/*								*/
	/*	Need to ensure that these snapshots are synchronized	*/
	/*								*/
	/*	One of the CPUs	then scans all the thread state 	*/
	/*	structures to compute the new map table.		*/
	/*								*/
	//TODO: preliminary study (mapping vs time plot) show it may 
	//	sometimes be beneficial to have the window sizes be per-
	//	map table object
	if ((now - me->mapstats_timestamp) > spthr_windowsize)
	{
		local_mapstats();

		/*	For now, hard coded that map maintainance runs on CPU0	*/
		if (spthr_myid() == 0)
		{
			map_update();
		}

		return;
	}

	return;
}

int
findhome(char *name)
{
	int i, matchlen;

	for (i = 0; spthr_staticmaptable[i].homenode != SPTHR_MAP_END; i++)
	{
		matchlen = strlen(spthr_staticmaptable[i].name);
		if (!strncmp(name, spthr_staticmaptable[i].name, matchlen))
		{
			return spthr_staticmaptable[i].homenode;
		}
	}

	return SPTHR_MAP_INVALID;
}

void
hdlr_install(void)
{
	extern	uchar	intr_vec_stub_begin, intr_vec_stub_end;
	uchar		*dstptr;
	uchar		*srcptr;


	/*	Interrupts	*/
	dstptr = (uchar *)0x8000600;
	srcptr = &intr_vec_stub_begin;
	while (srcptr < &intr_vec_stub_end)
	{
		*dstptr++ = *srcptr++;
	}

	/*	Exceptions	*/
/*
	dstptr = (uchar *)0x8000100;
	srcptr = &excp_vec_stub_begin;
	while (srcptr < &excp_vec_stub_end)
	{
		*dstptr++ = *srcptr++;
	}
*/
	return;
}

void
printq(Sleepq *q)
{
	Sleepq		*p = q;

	while (p != nil)
	{
		printf("\tQueue item, addr = [0x%lx] rdy=%d, thread=%d...\n",
			(ulong)p, p->rdy, p->threadid);
		p = p->next;
	}
}

/*									*/
/*	These two functions must be provided so that newlib will	*/
/*	lock the memory pool during malloc(). They spin-lock to		*/
/*	provide global mutual exclusion.				*/
/*									*/	
void
__malloc_lock(void *addr)
{
/*							*/
/*	Some C libraries require recursive locks,	*/
/*	i.e., do not block on an attempt to lock 	*/
/*	an addr (supplied) to which one already owns 	*/
/*	the lock, but not newlib:			*/
/*
	if ((Maddr)addr == spthr_mallocmutex.addr)
	{
		return;
	}
*/
	while (!spthr_tas(&(spthr_mallocmutex.lock)))
	{
		spthr_dosleep();
	}
	spthr_mallocmutex.addr = (Maddr)addr;

	return;
}

void
__malloc_unlock(void)
{
	spthr_mallocmutex.lock = 0;
	spthr_mallocmutex.addr = -1;
}
