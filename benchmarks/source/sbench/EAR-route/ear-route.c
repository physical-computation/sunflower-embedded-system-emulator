//#include <stdio.h> //TODO: delete this when code is cleaned up
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "e-types.h"
#include "errors.h"
#include "sf-includes.h"
#include "ear-route.h"



static int		netint2int(uchar *);
static int		netbyte2int(uchar *);
static ulong		uniform(ulong min, ulong max);
static uint		Cnjni(EARState *S, uchar *src, uint cost);
static uint		meanCnjni(EARState *S);
static int		sendtomostprobable(EARState *S, uchar *final_dst, uchar *orig_src, uint cost,
				uchar *data, int len, int nxthdr, int hopcount, int phase);
static int		sendtonearest(EARState *S, uchar *final_dst, uchar *orig_src, uint cost,
				uchar *data, int len, int nxthdr, int hopcount, int phase);
static EARLocation *	addr2location(EARState *S, uchar *addr);
static int		distance(EARLocation *a, EARLocation *b);
static int		addreq(uchar *a, uchar *b);
static int		ntablelookup(EARState *S, uchar *addr);
static int		addtontable(EARState *S, uchar *addr,
				int src_loc_x, int src_loc_y, int src_loc_z);
static void		refresh_randtable(EARState *S);
static int		ftablelookup(EARState *S, uchar *addr);
static int		addtoftable(EARState *S, uchar *src, uint cost);
static int		xmit(EARState *S, uchar *next_addr, uchar *final_addr,
				uchar *orig_src, uint cost, uchar *data,
				int len, int nxthdr, int hopcount, int phase);

static void		lock(ulong *l);
static void		unlock(ulong *l);

extern void		fatal(char *);
extern void		splhi(void);
extern void		spldone(ulong);
extern ulong		getpsr(void);

/*	Global buffer used to craft header in xmit()		*/
static EARHeader	xmit_hdr;


extern int		mac_xmit(uchar *ear_dst_addr,
				uchar *ear_hdr, int hdrlen,
				uchar *data, int data_len);
extern uchar		MAC_BCAST_ADDR[];


//	Oracle locations (since EAR assumes we can determine locs)
EARLocation	oracle_location;


//BUG: 	a problem here (and also in our S-MAC implementation), is that
//	we do not lock the data structures, even though they are accessed
//	asynchronously by intr hdlr, while main code might also be accessing structures


/*									*/
/*	If we used spin locks for the locking routines, we could	*/
/*	easily deadlock, e.g., when intr hdlr prempts app, but ends	*/
/*	up waiting for lock held by app (which won't get run till	*/
/*	hdlr is done.  This is ugly, but is the way things are since	*/
/*	we don't have a real OS.					*/
/*									*/


//TODO:	we don't use whichifc, and it really shouldnt be exposed here at the
//	routing layer.  Get rid of it from all interfaces.

static void
lock(ulong *l)
{
	*l = getpsr();
	splhi();
}

static void
unlock(ulong *l)
{
	spldone(*l);
}


static int
netbyte2int(uchar *data)
{
	return	(int) data[0];
}

static int
netint2int(uchar *data)
{
	return (int)((data[0] << 0) |
		(data[1] << 8) |
		(data[2] << 16) |
		(data[3] << 24));
}

static ulong
uniform(ulong min, ulong max)
{
	return min + (devrand_getrand() % ((max-min)+1));
}

uint
Cnjni(EARState *S, uchar *src, uint cost)
{
//TODO: reading of new battfrac register hardcoded here. need to move it info a devbatt.c
	double	battfrac = ((double)*((ulong *)0xefeeffc5))/100.0;

	return cost + (int)ceil(pow(S->eij, S->alpha)*pow(battfrac, S->beta));
}

uint
meanCnjni(EARState *S)
{
	int	i;
	uint	mean = 0;

	for (i = 0; i < S->numftable; i++)
	{
		mean += (S->ftable[i].prob_num * S->ftable[i].cost)
				/ S->ftable[i].prob_denom;
	}

	return mean;
}

int
sendtomostprobable(EARState *S, uchar *final_dst, uchar *orig_src, uint cost, uchar *data,
	int len, int nxthdr, int hopcount, int phase)
{
	int	idx;

	if (S->numftable == 0)
	{
		/*								*/
		/*	Not explicitly stated in paper, but this is logical	*/
		/*	thought he choice of cost here is not well motiv.	*/
		/*								*/
		return sendtonearest(S, final_dst, orig_src, cost, data,
				len, nxthdr, hopcount, phase);
	}

	idx = S->randtable[uniform(0, EAR_RANDTABLE_SIZE-1)];
	xmit(S, S->ftable[idx].addr, final_dst, orig_src, cost, data, len,
		nxthdr, hopcount, phase);

	return 0;
}

int
sendtonearest(EARState *S, uchar *final_dst, uchar *orig_src, uint cost, uchar *data,
	int len, int nxthdr, int hopcount, int phase)
{
	int		i, mindist;
	uchar		*minaddr;
	EARLocation	*l;


	l = addr2location(S, final_dst);
	if (l == NULL)
	{
//@printf("\t\t\tCould not determine location of node [%s], using broadcast...\n", final_dst);
		xmit(S, &MAC_BCAST_ADDR[0], final_dst, orig_src, cost, data, len, nxthdr, hopcount, phase);

		return -1;
	}

	mindist = distance(&S->location, l);
	minaddr = &S->addr[0];

	for (i = 0; i < S->numntable; i++)
	{
		int	dist;

//@printf("neighbor=[%s], distance to [%s]=[%d], mindist=[%d]\n", S->ntable[i].addr, final_dst, distance(&S->ntable[i].location, l), mindist);
		dist = distance(&S->ntable[i].location, l);
		if (dist <= mindist)
		{
			minaddr = &S->ntable[i].addr[0];
			mindist = dist;
		}
	}

	if (addreq(minaddr, S->addr))
	{
//@printf("No nearest neighbor found for addr=[%s] in sendtonearest(), using broadcast...\n", final_dst);
		minaddr = &MAC_BCAST_ADDR[0];
	}

	xmit(S, minaddr, final_dst, orig_src, cost, data, len, nxthdr, hopcount, phase);
	

	return 0;
}

EARLocation *
addr2location(EARState *S, uchar *addr)
{
	EARLocation	*l;
	int		i;

	for (i = 0; i < S->numntable; i++)
	{
		if (addreq(addr, S->ntable[i].addr))
		{
			return &S->ntable[i].location;
		}
	}




//	Oracle Node Location for default DAM topology: only really works for the ~5x5 network
if (EAR_ORACLE_LOCATIONS)
{
	int		id;
	char		tmp;
	char		*ep = &tmp;
	char		cleanaddr[EAR_ADDR_LEN+1];


	cleanaddr[0] = addr[0];
	cleanaddr[1] = addr[1];
	cleanaddr[2] = '\0';
	id = strtol(&cleanaddr[0], &ep, 0);
	if (*ep != '\0')
	{
//@@		printf("Invalid addr period supplied to addr2location().\n");
		return NULL;
	}

	if (id == 25)
	{
		oracle_location.x = 45;
		oracle_location.y = 45;
		oracle_location.z = 0;
	}
	else
	{
		oracle_location.x = 10*(id/5);
		oracle_location.y = 10*(id % 5);
		oracle_location.z = 0;
	}

//@	printf("\tReturning oracle location for node [%s] = [%d][%d][%d]\n",
//@			addr, oracle_location.x, oracle_location.y, oracle_location.z);
	
	return &oracle_location;
}




	return NULL;
}

int
distance(EARLocation *a, EARLocation *b)
{
	int d = (int)ceil(sqrt(	(a->x-b->x)*(a->x-b->x) +
			(a->y-b->y)*(a->y-b->y) +
			(a->z-b->z)*(a->z-b->z)));

//printf("returning distance = %d\n", d);

	return d;
}

int
addreq(uchar *a, uchar *b)
{
        if ((a[0] != b[0]) || (a[1] != b[1]))
        {
                return 0;
        }

        return 1;
}

int
ntablelookup(EARState *S, uchar *addr)
{
	int	i;

	for (i = 0; i < S->numntable; i++)
	{
		if (addreq(addr, S->ntable[i].addr))
		{
			return i;
		}
	}

	return -1;
}

int
addtontable(EARState *S, uchar *addr, int src_loc_x, int src_loc_y, int src_loc_z)
{
	int	inc, status = 0, idx;

	idx = ntablelookup(S, addr);
	if (idx >= 0)
	{
		return idx;
	}

	/*								*/
	/*	The EAR paper doesn't specify what to do in this case	*/
	/*	If table is full, we pick a random entry to replace:	*/
	/*								*/
	if (S->numntable == EAR_MAX_NEIGHBORS)
	{
		idx = uniform(0, EAR_MAX_NEIGHBORS-1);
		status = -1;
		inc = 0;
	}
	else
	{
		idx = S->numntable;
		inc = 1;
	}

	S->ntable[idx].location.x = src_loc_x;
	S->ntable[idx].location.y = src_loc_y;
	S->ntable[idx].location.z = src_loc_z;

	strncpy(S->ntable[idx].addr, addr, EAR_ADDR_LEN);
	S->numntable += inc;

	return status;
}

void
refresh_randtable(EARState *S)
{
	int	r, i, j, idx = 0;

//@@printf("nmftable=%d, ftable[%d].num=%d, ftable[%d].denom=%d\n",
//@@	S->numftable, 0, S->ftable[0].prob_num, 0, S->ftable[0].prob_denom);

//@@printf("\tRefreshing randtable: [ ");
	for (i = 0; i < S->numftable; i++)
	{
//@@printf("ftable[%d].num=%d, ftable[%d].denom=%d\n",
//@@	i, S->ftable[i].prob_num, i, S->ftable[i].prob_denom);

		r = (S->ftable[i].prob_num*EAR_RANDTABLE_SIZE)/S->ftable[i].prob_denom;
		for (j = 0; j < r && idx < EAR_RANDTABLE_SIZE; j++)
		{
			S->randtable[idx++] = i;
//@@printf("%d ", i);
		}
	}
//@@printf("]\n");


	return;
}

int
ftablelookup(EARState *S, uchar *addr)
{
	int	i;

	for (i = 0; i < S->numftable; i++)
	{
		if (addreq(addr, S->ftable[i].addr))
		{
			return i;
		}
	}

	return -1;
}

int
addtoftable(EARState *S, uchar *src, uint cost)
{
	int	status = -1, i, idx, inc, min_i, max_i;
	uint	mincost, maxcost, newcost;
	float	sumcost = 0.0;


	// BUG: our implementation currently does not handle deletion
	// for ntable or ftable entries (though, the point at which
	// such deletions should happen is not clear from the paper)

//printf("in addtoftable(), S->numftable = %d\n", S->numftable);



	/*								*/
	/*	Note that cost depends on current battery level		*/
	/*	so even if the addr is in table, need to update.	*/
	/*								*/
	newcost = Cnjni(S, src, cost);

//@@printf("newcost (%s --> %s) = %u\n", S->addr, src, newcost);

	if (S->numftable == 0)
	{
		strncpy(S->ftable[S->numftable].addr, src, EAR_ADDR_LEN);
		S->ftable[S->numftable].cost 		= newcost;
		S->ftable[S->numftable].prob_num 	= 1;
		S->ftable[S->numftable].prob_denom	= 1;
		S->numftable++;
		refresh_randtable(S);

		return 0;
	}

	maxcost = 0;
	mincost = ~0U;
	for (i = 0; i < S->numftable; i++)
	{
		if (maxcost < S->ftable[i].cost)
		{
			maxcost = S->ftable[i].cost;
			max_i = i;
		}

		if (mincost > S->ftable[i].cost)
		{
			mincost = S->ftable[i].cost;
			min_i = i;
		}

		sumcost += 1.0/(float)S->ftable[i].cost;
	}

//@@printf("sumcost = %f, mincost = %u, maxcost = %u\n", sumcost, mincost, maxcost);
//@@printf("\n");
	
	if (newcost <= S->ftable_alpha*mincost) 
	{
		/*								*/
		/*	Not explicitly stated in paper, but makes sense ---	*/
		/*	if table is full, replace entry with max cost:		*/
		/*								*/
		idx = ftablelookup(S, src);
		if (idx >= 0)
		{
			sumcost	-= 1.0/(float)S->ftable[idx].cost;

			/*	Depends on batt level; need to update	*/
			S->ftable[idx].cost = newcost;
			status = 0;
			inc = 0;
		}
		else if (S->numftable == EAR_MAX_FTABLE)
		{
			idx = max_i;
			sumcost	-= 1.0/(float)maxcost;
			inc = 0;
		}
		else
		{
			idx = S->numftable;
			inc = 1;
		}

		strncpy(S->ftable[idx].addr, src, EAR_ADDR_LEN);

		S->ftable[idx].cost	= newcost;
		sumcost 		+= 1.0/(float)S->ftable[idx].cost;
		S->numftable 		+= inc;

		for (i = 0; i < S->numftable; i++)
		{
			S->ftable[i].prob_num 	= 100/S->ftable[i].cost;
			S->ftable[i].prob_denom	= 100*sumcost;
		}
		refresh_randtable(S);


		return 0;
	}

	return status;
}

EARSrcPayloadTuple *
ear_recv(EARState *S, uchar *payload, int len)
{
	EARHeader		*h;
	EARSrcPayloadTuple 	*r;


	/*								*/
	/*	This routine is called with the MAC-layer payload,	*/
	/*	and extracts and returns the transport-layer load	*/
	/*	if the current node is the final destination, or	*/
	/*	initiates further actions if we are an intermediate	*/
	/*	hop (and returns NULL).	  The application should	*/
	/*	do something along the lines of:			*/
	/*		if ((x = ear_rcv(...)) != NULL) 		*/
	/*			... do something with x.payload ...	*/
	/*			send response to payload:		*/
	/*			ear_response(S, x.src, data)		*/
	/*								*/
	

	if (payload[0] != PROTO_EAR)
	{
		return NULL;
	}

	r = (EARSrcPayloadTuple*)malloc(sizeof(EARSrcPayloadTuple));
	if (r == NULL)
	{
		fatal(Emalloc);
	}


	/*	Cookie cutter decode	*/
	h = (EARHeader *)&payload[0];



//@printf("h->orig_src=[%s], h->src=[%s], h->hopcount=[%d]\n\n\n", h->orig_src, h->src, h->hopcount);

printf("h->len = [%d], h->nxthdr=[%d], h->cost=[%d], h->final_dst=[%s], h->orig_src=[%s], h->src=[%s], h->src_loc_x=[%d], h->src_loc_y=[%d], h->src_loc_z=[%d]\n\n\n", h->len, h->nxthdr, h->cost, h->final_dst, h->orig_src, h->src, h->src_loc_x, h->src_loc_y, h->src_loc_z);




	/*	Avoid loops	*/
	if (addreq(S->addr, h->src) || addreq(S->addr, h->orig_src))
	{
//@printf("S->addr ([%s]) == h->orig_src ([%s]): skipping out...\n", S->addr, h->orig_src);
		return NULL;
	}


	/*								*/
	/*	NOTE: this is an idealization: we currently embed	*/
	/*	senders location in the EAR packets, though the		*/
	/*	MAC layer should determine these.  Either way, we	*/
	/*	will still only record neighbor locations in the	*/
	/*	neighbor-list table upon receipt of a communication	*/
	/*								*/
	if (addtontable(S, h->src, h->src_loc_x, h->src_loc_y, h->src_loc_z) < 0)
	{
		//print("Too many neighbors : could not add node [%c%c]\n",
		//	h->src[0],h->src[1]);
	}


	/*								*/
	/*	Implied by paper: don't add to ftable if not a setup-	*/
	/*	phase packet.						*/
	/*								*/
	if (h->phase == EAR_PHASE_SETUP)
	{
		if (addtoftable(S, h->src, h->cost) < 0)
		{
			//print("Too many forwarding table entries : could not add node [%c%c]\n",
			//	h->src[0],h->src[1]);
		}
	}



	if (!addreq(S->addr, h->final_dst))
	{
		//@printf("\tReceived packet for which we are not final destination:\n"
		//@	"\toriginal src = [%s], final dst = [%s], intermediate src = [%s]\n",
		//@	h->orig_src, h->final_dst, h->src);


		if (h->hopcount < S->maxhops)
		{
			if (h->phase == EAR_PHASE_SETUP)
			{
//@@print("\n\tforwarding EAR_PHASE_SETUP packet to nearest neighbor...\n\n");
				sendtonearest(S, h->final_dst, h->orig_src, meanCnjni(S),
					&payload[sizeof(EARHeader)], h->len, h->nxthdr,
					h->hopcount+1, h->phase);
			}
			else
			{				
//@@print("\n\tforwarding EAR_PHASE_COMMS packet to most probable neighbor...\n\n");
				sendtomostprobable(S, h->final_dst, h->orig_src, meanCnjni(S),
					&payload[sizeof(EARHeader)], h->len, h->nxthdr,
					h->hopcount+1, h->phase);
			}
		}
		else
		{
		//@	printf("Packet has [%d] hops... dropping\n", h->hopcount);
		}

		return NULL;
	}

//@print("\n\n\n\tWe are final dest... returning Tuple...\n\n");
	
	r->payload = &payload[sizeof(EARHeader)];
	r->nxthdr = h->nxthdr;
	strncpy(r->src, h->orig_src, EAR_ADDR_LEN);


	return r;
}

int
ear_response(EARState *S, uchar *final_dst, uchar *data, int len, int nxthdr)
{
	/*								*/
	/*	This initiates the 'Data Communication Phase' of	*/
	/*	EAR, and is activated in response to receiving a	*/
	/*	packet (via ear_recv()) for which we are the final	*/
	/*	destination.						*/
	/*								*/
	return sendtomostprobable(S, final_dst, S->addr, 0 /* cost */, data, len, nxthdr, 0, EAR_PHASE_COMMS);
}

int
ear_request(EARState *S, uchar *src, uchar *data, int len, int nxthdr)
{
	EARLocation	*l;
	int		i, nearerneighbors = 0;

//@@printf("Node issuing ear_request() has %d items in ntable:\n", S->numntable);
//@@for (i = 0; i < S->numntable; i++) printf("\tS->ntable[%d] = %s\n", i, S->ntable[i].addr);

	/*								*/
	/*	This initiates 'Setup Phase' of EAR, with the dest	*/
	/*	initiating the seeding of a path to the source.		*/
	/*								*/

	/*								*/
	/*	Currently implemented an oracle in addr2locations	*/
	/*	since EAR assumes initiator of request knows location	*/
	/*	of dst, thus addr2locations won't return NULL.  In	*/
	/*	reality, in the abscence of the oracle, it would have	*/
	/*	no choice but to broadcast				*/
	/*								*/
	l = addr2location(S, src);

	/*	If ntable is as yet empty no choice but to bcast	*/
	if (l == NULL || S->numntable == 0)
	{
//@@printf("Could not determine location of node [%s]... using broadcast\n", src);
		xmit(S, &MAC_BCAST_ADDR[0], src, S->addr, 0 /* cost */,
				data, len, nxthdr, 0, EAR_PHASE_SETUP);
		return 0;
	}


//TODO: why not use sendtonearest() here ?
	/*	"Flood" all neighbors nearer "src" than we are		*/
	for (i = 0; i < S->numntable; i++)
	{
		if (	distance(&S->location, l) >
			distance(&S->ntable[i].location, l))
		{
			xmit(S, S->ntable[i].addr, src, S->addr, 0 /* cost */,
				data, len, nxthdr, 0, EAR_PHASE_SETUP);
			nearerneighbors++;
		}
	}

	return nearerneighbors;
}

int
xmit(EARState *S, uchar *next_addr, uchar *final_addr, uchar *orig_src, uint cost,
	uchar *data, int len, int nxthdr, int hopcount, int phase)
{
	int	hdrlen;

	/*							*/
	/*	We use a single global EARHeader structure to	*/
	/*	handle packing, rather than mallocing each time */
	/*							*/
	xmit_hdr.proto		= PROTO_EAR;
	xmit_hdr.phase		= phase;
	xmit_hdr.hopcount	= hopcount;
	xmit_hdr.len		= len;
	xmit_hdr.nxthdr		= nxthdr;
	xmit_hdr.cost		= cost;

	strncpy(xmit_hdr.final_dst, final_addr, EAR_ADDR_LEN);
	strncpy(xmit_hdr.orig_src, orig_src, EAR_ADDR_LEN);
	strncpy(xmit_hdr.src, S->addr, EAR_ADDR_LEN);

	xmit_hdr.src_loc_x	= S->location.x;
	xmit_hdr.src_loc_y	= S->location.y;
	xmit_hdr.src_loc_z	= S->location.z;

	hdrlen = (int)sizeof(EARHeader);

	/*	Call the MAC layer's transmit routine		*/
	return mac_xmit(next_addr, (uchar *)&xmit_hdr, hdrlen, data, len);
}

EARState *
ear_init(uchar *addr, int which_ifc, int alpha, int beta, int ftable_alpha, int eij, int maxhops,
	int x, int y, int z)
{
	EARState	*S;

	S = (EARState*) malloc(sizeof(EARState));
	if (S == NULL)
	{
		fatal(Emalloc);
	}

	S->addr[0]	= addr[0];
	S->addr[1]	= addr[1];
	S->which_ifc	= which_ifc;
	S->location.x	= x;
	S->location.y	= y;
	S->location.z	= z;
	S->alpha	= alpha;
	S->beta		= beta;
	S->eij		= eij;
	S->ftable_alpha	= ftable_alpha;
	S->maxhops	= maxhops;

	S->numntable	= 0;
	S->numftable	= 0;
	S->lock		= 0;

	/*								*/
	/*	EAR protocol (unrealistically) relies on a priori 	*/
	/*	knowledge of neighbors					*/
	/*								*/
	/*	NOTE!!: this is only valid for the DAM 5x5 topology	*/
	/*								*/
	if (EAR_ORACLE_NEIGHBORS)
	{
		int		i, id;
		char		tmp;
		char		*ep = &tmp;
		char		cleanaddr[EAR_ADDR_LEN+1];
		char		neighbor[EAR_ADDR_LEN+1];

		cleanaddr[0] = S->addr[0];
		cleanaddr[1] = S->addr[1];
		cleanaddr[2] = '\0';
		id = strtol(&cleanaddr[0], &ep, 0);
		if (*ep != '\0')
		{
			printf("Could not initialize ORACLE neighbors...\n");

			return S;
		}

		if (id == 25)
		{
			sprintf(neighbor, "%d", 24);
			addtontable(S, neighbor, S->location.x-5, S->location.y-5, 0);
printf("done ear_init(). %d items in ntable:\n", S->numntable);
for (i = 0; i < S->numntable; i++) printf("\tS->ntable[%d] = %s\n", i, S->ntable[i].addr);

			return S;
		}
printf("in ear_init(), id=[%d]\n\n\n", id);

		sprintf(neighbor, "%d", id-1);
		if (id-1 >= 0) addtontable(S, neighbor, S->location.x, S->location.y-10, 0);
		sprintf(neighbor, "%d", id+1);
		if (id+1 <= 25) addtontable(S, neighbor, S->location.x, S->location.y+10, 0);
		sprintf(neighbor, "%d", id-5);
		if (id-5 >= 0) addtontable(S, neighbor, S->location.x-10, S->location.y, 0);
		sprintf(neighbor, "%d", id+5);
		if (id+5 <= 25) addtontable(S, neighbor, S->location.x+10, S->location.y, 0);
		sprintf(neighbor, "%d", id-6);
		if (id-6 >= 0) addtontable(S, neighbor, S->location.x-10, S->location.y-10, 0);
		sprintf(neighbor, "%d", id+6);
		if (id+6 <= 25) addtontable(S, neighbor, S->location.x+10, S->location.y+10, 0);
		sprintf(neighbor, "%d", id-4);
		if (id-4 >= 0) addtontable(S, neighbor, S->location.x-10, S->location.y+10, 0);
		sprintf(neighbor, "%d", id+4);
		if (id+4 <= 25) addtontable(S, neighbor, S->location.x+10, S->location.y-10, 0);
printf("done ear_init(). %d items in ntable:\n", S->numntable);
for (i = 0; i < S->numntable; i++) printf("\tS->ntable[%d] = %s\n", i, S->ntable[i].addr);
	}


	return S;
}
