/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef	SUNOS
#	include <strings.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <float.h>
#include <signal.h>
#include "sf.h"
#include "mversion.h"
#include "instr-hitachi-sh.h"
#include "endian-hitachi-sh.h"
#include "help.h"
#include "opstr-hitachi-sh.h"
#include "opstr-riscv.h"
#include "latencies-hitachi-sh.h"
#include "latencies-riscv.h"



Engine*		engines[MAX_NUM_ENGINES];
int		nengines;

Engine		*yyengine;
extern int	sf_superh_parse(void);
extern int	sf_riscv_parse(void);

static void	spinbaton(Engine *, int);
static void	do_numaregion(Engine *, State *, char *, ulong, ulong, long, long, long, long, int, ulong, int, int, int, ulong, int, int);
static void	updaterandsched(Engine *);
static void	bpts_feed(Engine *);
static void	readnodetrajectory(Engine *, State *, char*, int, int);

Engine *
m_lookupengine(uvlong engineid)
{
	int	i;

	for (i = 0; i < nengines; i++)
	{
		if (engines[i]->randseed == engineid)
		{
			return engines[i];
		}
	}

	return nil;
}

Engine *
m_allocengine(uvlong seed)
{
	Engine	*tmp;


	tmp = (Engine *) calloc(1, sizeof(Engine));
	if (tmp == nil)
	{
		fprintf(stderr, "calloc() failed for Engine *tmp in %s, exiting...", SF_FILE_MACRO);
		exit(-1);
	}

	tmp->throttlensec	= 0;
	tmp->throttlewin	= 10000;
	tmp->quantum		= 1;
	tmp->baseid		= 0;
	tmp->cn			= 0;
	tmp->scanning		= 0;
	tmp->nnodes		= 0;
	tmp->on			= 0;
	tmp->ignoredeaths	= 0;
	tmp->infoh2o		= 0;
	tmp->globaltimepsec	= 0;
	tmp->mincycpsec		= PICOSEC_MAX;
	tmp->maxcycpsec		= 0;
	tmp->fperiodpsec	= 100E-6; //100000;	/* 100E-6 seconds */
	tmp->flastpsec		= 0;
	tmp->nnetsegs		= 0;
	tmp->nactivensegs	= 0;
	tmp->nicsimbytes	= 0;
	tmp->netioh2o		= 0;
	tmp->netperiodpsec	= PICOSEC_MAX;		/* set by netsched*/
	tmp->netlastpsec	= 0;
	tmp->nsigsrcs		= 0;
	tmp->phylastpsec	= 0;
	tmp->phyperiodpsec	= 100E-6; //100000;	/* 100E-6 seconds */
	tmp->curbatt		= 0;
	tmp->nactivebatts	= 0;
	tmp->ndepletedbatts	= 0;
	tmp->nbatts		= 0;
	tmp->verbose		= 1;
	tmp->battlastpsec	= 0;
	tmp->battperiodpsec	= 1E-3; //1000000;	/* 1E-3	seconds */
	tmp->dumplastpsec	= 0;
	tmp->dumpperiodpsec	= 1E-3; //1000000;	/* 1E-3	seconds */
	tmp->nmemblocks		= 0;


	/*	We do exactly what Matsumoto-Nishimura paper does	*/
	tmp->randgen_mti	= RANDGEN_NN+1;


	tmp->logfilename = (char *)calloc(1, strlen(SF_LOGFILENAME)+1);
	if (tmp->logfilename == nil)
	{
		fprintf(stderr, "calloc failed for tmp->logfilename in %s, exiting...", SF_FILE_MACRO);
		exit(-1);
	}
	strcpy(tmp->logfilename, SF_LOGFILENAME);


	tmp->randseed = mrandominit(tmp, seed);

	/*	Initialize random number generators	*/
	mprint(tmp, NULL, siminfo,
			"Initialized random number generator with seed %d...\n",
			tmp->randseed);

	engines[nengines++] = tmp;


	return tmp;
}

tuck void
spinbaton(Engine *E, int pos)
{
	char		*batons[] = {"*", "X", "-", "\\", "|", "/"};

	if (pos == 0)
	{
		mprint(E, NULL, siminfo, "[*]");
	}
	else
	{
		mprint(E, NULL, siminfo, "\b\b%s]", batons[pos % 4]);
	}
}

#ifndef LIBSF
int
main(int nargs, char *args[])
{
	Engine		*E;
	char 		*buf = NULL;
	int		argn;


	nengines = 0;
	E = m_allocengine(-1);
	if (E == nil)
	{
		fprintf(stderr, "Could not allocate a simulation engine. Exiting...\n\n");
		exit(-1);
	}

	E->verbose = 1;
	marchinit();
	m_version(E);
	m_newnode(E, "superH", 0, 0, 0, nil, 0, 0.0);	/*	default processor	*/


	buf = (char *) mmalloc(E, sizeof(char)*(MAX_BUFLEN+1),
			"(char *)buf for command interface in main.c");
	if (buf == NULL)
	{
		mexit(E, "Malloc failed in main.c for \"buf\"", -1);
	}

	argn = 1;
	while (argn < nargs)
	{
		loadcmds(E, args[argn++]);
	}

	/*	In the non-LIBSF version, we use fprintf to write to console	*/
	fprintf(stderr, "\n");
	fprintf(stderr, "[ID=%d of %d][PC=0x" UHLONGFMT "][%.1EV, %.1EMHz] ",
		E->cp->NODE_ID, E->nnodes, (unsigned long)E->cp->PC,
		E->cp->VDD, (1/E->cp->CYCLETIME)/1E6);

	while (1)
	{
		fgets(buf, MAX_BUFLEN, stdin);
		if (strlen(buf) > 0)
		{
			mstatelock();
			munchinput(E, buf);
			yyengine = E;
			if (yyengine->cp->machinetype == MACHINE_SUPERH)
			{
				sf_superh_parse();
			}
			else if (yyengine->cp->machinetype == MACHINE_RISCV)
			{
				sf_riscv_parse();
			}
			fprintf(stderr, "[ID=%d of %d][PC=0x" UHLONGFMT "][%.1EV, %.1EMHz] ",
				E->cp->NODE_ID, E->nnodes, (unsigned long)E->cp->PC,
				E->cp->VDD, (1/E->cp->CYCLETIME)/1E6);

			/*
			 *	Needed on some host embedded platforms, doesn't hurt in general.
			 */
			fflush(stderr);

			mstateunlock();

			buf[0] = '\0';
		}
	}

	return 0;
}
#endif


tuck void
sched_step(Engine *E)
{
	State		*S;
	int		i;
	Picosec		net_secsleft, fault_secsleft, physics_secsleft, batt_secsleft, traj_secsleft;
	Picosec		min_secsleft;
	double		max_cputime = 0.0;
	ulong		throttle_tripctr = 0;


	/*
	 *	TODO: make the locking finer grained, possibly also splitting out into reader and writer locks
	 */
	mstatelock();
	S = E->sp[E->cn];
	min_secsleft = PICOSEC_MAX;

	if (SF_NETWORK)
	{
		if  (eventready(E->globaltimepsec, E->netlastpsec, E->netperiodpsec))
		{
			network_clock(E);
			E->netlastpsec = E->globaltimepsec;
		}

		net_secsleft = E->netperiodpsec -
					(E->globaltimepsec - E->netlastpsec);
		min_secsleft = min(min_secsleft, net_secsleft);
	}

	if (SF_FAULT)
	{
		if (eventready(E->globaltimepsec, E->flastpsec, E->fperiodpsec))
		{
			fault_feed(E);
			E->flastpsec = E->globaltimepsec;
		}

		fault_secsleft = E->fperiodpsec -
					(E->globaltimepsec - E->flastpsec);
		min_secsleft = min(min_secsleft, fault_secsleft);
	}

	if (SF_PHYSICS)
	{
		if (eventready(E->globaltimepsec, E->phylastpsec, E->phyperiodpsec))
		{
			physics_feed(E);
			E->phylastpsec = E->globaltimepsec;
		}

		physics_secsleft = E->phyperiodpsec -
					(E->globaltimepsec - E->phylastpsec);
		min_secsleft = min(min_secsleft, physics_secsleft);
	}

	if (SF_TRAJECTORIES)
	{
		if (eventready(E->globaltimepsec, E->trajlastpsec, E->trajperiodpsec))
		{
			traj_feed(E);
			E->trajlastpsec = E->globaltimepsec;
		}

		traj_secsleft = E->trajperiodpsec -
					(E->globaltimepsec - E->trajlastpsec);
		min_secsleft = min(min_secsleft, traj_secsleft);
	}

	if (SF_BPTS)
	{
		bpts_feed(E);
	}


	/*							*/
	/*	Battery and DC-DC converter. Must sample often	*/
	/*	enough to catch short bursts of current draw.	*/
	/*							*/
	/*	We are using the current from the previous	*/
	/*	cycle (or E->quantum # of cycles), which will	*/
	/*	get reset before the node is simulated again	*/
	/*							*/
	if (SF_BATT)
	{
		/*	Battery feed with arg > 0 will force it to
			use the supplied value and disregard values
			gotten from battery model for current
		*/
		if (eventready(E->globaltimepsec, E->battlastpsec, E->battperiodpsec))
		{
			battery_feed(E, -1);

			if (SF_BATTLOG)
			{
				battery_dumpall(E, E->sp[0]);
			}

			E->battlastpsec = E->globaltimepsec;
		}

		batt_secsleft = E->battperiodpsec -
					(E->globaltimepsec - E->battlastpsec);
		min_secsleft = min(min_secsleft, batt_secsleft);
	}

	/*									*/
	/*	Auto quantum is not a perfect solution: E.g., in a case		*/
	/*	where two nodes attempt to transmit on the same link in		*/
	/*	a given auto quantum window, say 400 cycles. If, in the		*/
	/*	cycle-by-cycle simulation, first node trnsmits on link in	*/
	/*	cycle 0 (i.e., seg_enqueue() at cycle 0), and second node	*/
	/*	attempts to transmit at cycle 200, then the later node will	*/
	/*	incur a collision and will retry at 200+frame delay. In the	*/
	/*	case of auto quantum, the later node will incur the collision	*/
	/*	at cycle 0 (rather than 200), and its retry will happen at	*/
	/*	time 0 + frame delay. This error is not trivial, and may	*/
	/*	be amplified significantly by, e.g., use of non-deterministic	*/
	/*	collision avoidance simulation configurations.			*/
	/*									*/
	if (SF_AUTO_QUANTUM)
	{
		/*								*/
		/*	NOTE: quantum based on MAX_CYCLE to be conservative	*/
		/*								*/
		/*	Example: If CYCLETIME is 250ns, and the time of the	*/
		/*	next event is 1000ns, then min_secsleft/CYCLETIME == 4	*/
		/*	However, because we keep the clock one cycle ahead,	*/
		/*	the network_clock() etc get triggered at 1250ns, if	*/
		/*	doing autoquantum, and at 1000ns if not (with an ICLK	*/
		/*	of 3 and max_cputime of 750 after simulating nodes,	*/
		/*	then 250ns is added at the end of the loop and the	*/
		/*	network_clock() etc get triggered at 1000ns. There	*/
		/*	is a special case of when the calculated QUANTUM is	*/
		/*	less than one.	In that case, we make QUANTUM 1,	*/
		/*	which is the same behavior as running without AUTO_Q	*/
		/*								*/
		E->quantum = (int)(min_secsleft / E->maxcycpsec) - 1;
		E->quantum = max(E->quantum, 1);
	}

	if (E->schedtype == SchedRandom)
	{
		updaterandsched(E);
	}

	for (i = 0; i < E->nnodes; i++)
	{
		E->cn = i;
		if (E->schedtype == SchedRandom)
		{
			E->cn = E->randsched[i];
		}

		S = E->sp[E->cn];
		S->energyinfo.current_draw = 0;

		if (!S->runnable)
		{
			continue;
		}

		if (SF_BATT && S->ENABLE_BATT_LOW_INTR && S->check_batt_intr(E, S))
		{
			S->take_batt_intr(E, S);
			S->sleep = 0;
		}

		if (SF_BATT && (S->BATT) && ((Batt *)S->BATT)->dead)
		{
			S->runnable = 0;
			m_dumpall(E, "sunflower.out.tmp", M_OWRITE|M_OTRUNCATE, "Battery Dead", "");

			continue;
		}


		S->step(E, S, 0);


		if (SF_DUMPPWR
			&& eventready(E->globaltimepsec, E->dumplastpsec,
				E->dumpperiodpsec)
			&& S->runnable)
		{
			mlog(E, S, "%E %E",
				E->globaltimepsec, S->energyinfo.current_draw * S->VDD);
			E->dumplastpsec = E->globaltimepsec;
		}

		max_cputime = max(max_cputime, S->TIME);
	}

	/*									*/
	E->globaltimepsec = max(E->globaltimepsec, max_cputime) + E->mincycpsec;

	/*	Throttling is only approximate if doing autoquantum:	*/
	if (E->throttlensec > 0)
	{
		throttle_tripctr += E->quantum;

		if (throttle_tripctr >= E->throttlewin)
		{
			mnsleep(E->throttlensec*E->throttlewin);
			throttle_tripctr = 0;
		}
	}
	mstateunlock();
}

static void
updaterandsched(Engine *E)
{
	int	i, idx, genscheds = 0;

	for (i = 0; i < E->nnodes; i++)
	{
		E->randsched[i] = -1;
	}

	while (genscheds < E->nnodes)
	{
		idx = mrandom(E) % E->nnodes;
		if (E->randsched[idx] == -1)
		{
			E->randsched[idx] = genscheds++;
		}
	}

	/*
	fprintf(stderr, "sched:\t");
	for (i = 0; i < E->nnodes; i++)
	{
		fprintf(stderr, "%d ", E->randsched[i]);
	}
	fprintf(stderr, "\n");
	*/
}

void
scheduler(Engine *E)
{
#if (SF_EMBEDDED == 0)
	int	jmpval;
#endif

	/*								*/
	/*	Scheduler is spawned as a new proc; This does any	*/
	/*	architecture specific actions, like FPcontrol in	*/
	/*	Inferno or change the priority of the scheduler proc	*/
	/*								*/
	marchinit();

#if (SF_EMBEDDED == 0)
	if (!(jmpval = setjmp(E->jmpbuf)))
	{
		/*	Returning from initial call	*/
	}
	else
	{
		/*	Returning from longjmp()	*/
		/*	jmpval == node that barfed.	*/
	}
#endif

	while (E->on)
	{
		sched_step(E);
	}

	return;
}

void
load_srec(Engine *E, State *S, char *filename)
{
	char	*filebuf, buf[MAX_SREC_LINELEN];
	int	filesize, fpos, fd, i, n, nrecs = 0, batonpos = 0;
	ulong	rec_addr;
	int	pcset, rec_type, rec_length;
	char	*line;


	if ((fd = mopen(filename, M_OREAD)) < 0)
	{
		mprint(E, S, nodeinfo,
			"Open of \"%s\" failed...\n\n", filename);
		return;
	}

	if ((filesize = mfsize(fd)) < 0)
	{
		mprint(E, S, nodeinfo,
			"Determining size of \"%s\" failed...\n\n", filename);
		return;
	}

	filebuf = (char *)mcalloc(E, filesize, sizeof(char), "mcalloc for filebuf in main.c");
	if (filebuf == NULL)
	{
		mprint(E, S, nodeinfo,
			"Could not allocate memory for filebuf in main.c");
		return;
	}

	if ((n = mread(fd, filebuf, filesize)) != filesize)
	{
		mprint(E, S, nodeinfo,
			"Expected [%d] bytes in [%s], read [%d]",
			filesize, filename, n);
		return;
	}

	pcset = 0;
	fpos = 0;
	for (;;)
	{
		i = 0;


		if (fpos == filesize)
		{
			goto done;
		}

		while (i < MAX_SREC_LINELEN && fpos < filesize && filebuf[fpos] != '\n')
		{
			buf[i++] = filebuf[fpos++];
		}
		fpos++;

		if (i == MAX_SREC_LINELEN)
		{
			merror(E, "Absurdly long SREC line !");
			return;
		}

		buf[i] = '\0';


		line = &buf[0];
		rec_type = line[1] - '0';
		switch (rec_type)
		{
			case 0:
			{
				/*	Optional starting record. Skip	*/
				break;
			}

			case 1:
			{
				/*	Data record with 16bit address	*/
				break;
			}

			case 2:
			{
				/*	Data record with 24bit address	*/
				break;
			}

			/*								*/
			/*	TODO:	We do not verify checksum on SREC records	*/
			/*								*/
			case 3:
			{
				/*	Data record with 32bit addr	*/
				int	i;
				char	*tptr, tmp[8+1];

				memmove(&tmp[0], &line[2], 2);
				tmp[2] = '\0';
				rec_length = strtoul(&tmp[0], nil, 16);

				memmove(&tmp[0], &line[4], 8);
				tmp[8] = '\0';
				rec_addr = strtoul(&tmp[0], nil, 16);

				if (!pcset)
				{
					S->PC = rec_addr;
					S->STARTPC = S->PC;
					mprint(E, S, nodeinfo,
						"Loading S-RECORD to memory at address 0x" UHLONGFMT "\n",
						rec_addr);
					pcset = 1;
				}

#ifndef LIBSF
				spinbaton(E, batonpos);
				batonpos += !(nrecs++ % 256);
#else
				USED(nrecs);
				USED(batonpos);
				/*
				if (!(rec_addr % 128))
				{
					mprint(E, NULL, siminfo, ".");
				}
				*/
#endif

				/*	rec_length includes length of addr and chksum	*/
				rec_length -= 5;

				tptr = &line[12];
				for (i = 0; i < rec_length; i++)
				{
					memmove(&tmp[0], tptr, 2);
					tmp[2] = '\0';
					tptr += 2;

					/*						*/
					/*	Use the Write_Byte() routines so that	*/
					/*	addresses get translated.		*/
					/*						*/
					S->writebyte(E, S, rec_addr+i, strtoul(&tmp[0], nil, 16));
				}

				break;
			}

			case 4:
			{
				/*	Symbol record (LSI extension)	*/
				break;
			}

			case 5:
			{
				/*	# records in preceding block	*/
				break;
			}

			case 6:
			{
				/*		Unused			*/
				break;
			}

			case 7:
			{
				/*	End record for S3 records	*/
				break;
			}

			case 8:
			{
				/*	End record for S2 records	*/
				break;
			}

			case 9:
			{
				/*	End record for S1 records	*/
				break;
			}

			default:
			{
				mprint(E, S, nodeinfo, "Seen unknown SRECORD type.\n");
				mprint(E, S, nodeinfo, "Aborting SRECL.\n");
				goto done;
			}
		}
	}
done:
#ifdef LIBSF
	mprint(E, NULL, siminfo, "\nDone loading [%s].\n", filename);
#else
	mprint(E, NULL, siminfo, "\b\bM] Done.\n");
#endif
	mclose(fd);


	return;
}

void
load_mapfile(Engine *E, State *S, char *filename)
{
	FILE	*fp;
	char	line[MAX_SREC_LINELEN], *p;
	int		nlines = 0;
	//unsigned long dataSegmentEnd=0x0;
	char 	dataSegmentStr[9];


	if ((fp = fopen(filename, "r")) == NULL)
	{
		mprint(E, S, nodeinfo,
			"Open of \"%s\" failed...\n\n", filename);
		return;
	}
	else
	{
		mprint(E, S, nodeinfo,
			"Loading \"%s\" map file...\n\n", filename);
	}
	
	/*														*/
	/*		Skip all lines until the DATA_SEGMENT_END		*/
	/*														*/
	for (; strstr(line, "DATA_SEGMENT_END") == NULL ; nlines++)
	{
		fgets(line, sizeof(line), fp);
	}

	p = strtok(line, " \t");

	/*	18 is for 64bit addresses and 10 for 32bit addresses	*/
	if (strlen(p) == 18)
	{
		strcpy(dataSegmentStr, &p[10]);
	}
	else if (strlen(p) == 10)
	{
		strcpy(dataSegmentStr, &p[2]);
	}
	else
	{
		printf("Unknown memory length %d in mmap file.\n", strlen(p));
		return;
	}
	
	S->MEM_DATA_SEGMENT_END = strtol(dataSegmentStr, NULL, 16);

	mprint(E, S, nodeinfo, "DATA_SEGMENT_END: 0x%X\n", S->MEM_DATA_SEGMENT_END);

	//printf("Data segment str: %s. Data segment end: 0x%X\n", dataSegmentStr, dataSegmentEnd);

	fclose(fp);
}

void
m_setnode(Engine *E, int which)
{
	if (which >= E->nnodes)
	{
		merror(E, "Cannot set current node to [%d]: ID out of range.", which);
		return;
	}

	E->cp = E->sp[which];

	return;
}

void
loadcmds(Engine *E, char *filename)
{
	char	*buf;
	int	fd;


	buf = (char *) mcalloc(E, MAX_BUFLEN+1, sizeof(char),
			"char *buf in loadcmds(), main.c");
	if (buf == NULL)
	{
		mprint(E, NULL, siminfo, "Could not allocate memory for char *buf in loadcmds()");
		mexit(E, "See above messages.", -1);
	}

	fd = mopen(filename, M_OREAD);
	if (fd < 0)
	{
		mprint(E, NULL, siminfo, "Cannot open \"%s\" for loading...\n\n", filename);
		return;
	}

	/*									*/
	/*	NOTE:	Do not lock simulator state; our callers will.		*/
	/*									*/
	mprint(E, NULL, siminfo, "Loading %s...\n", filename);
	clearistream(E);
	while (mfgets(buf, MAX_BUFLEN, fd) != NULL)
	{
		munchinput(E, buf);
	}

	/*
	 *	NOTE: scan_labels_and_globalvars does a sf_superh_parse(), so need yyengine set before
	 */
	yyengine = E;
	//streamchk(E);
	scan_labels_and_globalvars(E);
	//streamchk(E);
	if (yyengine->cp->machinetype == MACHINE_SUPERH)
	{
		sf_superh_parse();
	}
	else if (yyengine->cp->machinetype == MACHINE_RISCV)
	{
		sf_riscv_parse();
	}
	mclose(fd);


	return;
}

void
savemem(Engine *E, State *S, ulong start_addr, ulong end_addr, char *filename)
{
	int i, outfd;

	if ((start_addr < S->MEMBASE) || (end_addr >= S->MEMEND))
	{
		mprint(E, S, nodeinfo, "Memory address out of range in SAVE command");
	}

	if ((outfd = mcreate(filename,  M_OWRITE|M_OTRUNCATE)) < 0)
	{
		mprint(E, S, nodeinfo,
			"Could not open output file for writing in SAVE command");

		return;
	}

	for (i = start_addr; i <= end_addr; i++)
	{
		mwrite(outfd, (char *)&S->MEM[i-S->MEMBASE], 1);
	}
	mclose(outfd);

	return;
}

void
sfatal(Engine *E, State *S, char *msg)
{
#if (SF_EMBEDDED == 0)
	int	do_jmp = E->on;
#endif


	E->verbose = 1;
	mprint(E, S, nodeinfo, "Sunflower FATAL (node %d) : <%s>\n",\
			S->NODE_ID, msg);

	S->fatalaction(E, S);
	S->dumpregs(E, S);
	S->dumpsysregs(E, S);

	S->runnable = 0;
	mprint(E, NULL, siminfo, "Stopping execution on node %d and pausing simulation...\n\n",
		S->NODE_ID);

	if (!E->ignoredeaths)
	{
		mstateunlock();
		mkillscheduler(E);
	}

	/*								*/
	/*	Don't longjmp when, e.g., loading a file into mem, in	*/
	/*	which case we'd have entered sfatal w/ E->on false.	*/
	/*								*/
#if (SF_EMBEDDED == 0)
	if (do_jmp)
	{
		longjmp(E->jmpbuf, S->NODE_ID);
	}
#else
	mexit(E, "Exiting simulation on node failure (host platform does not support setjmp/longjmp)...", 0);
#endif
}

void
go(Engine *E, State *S, int gocycles)
{
	while ((gocycles--) > 0)
	{
		S->step(E, S, 0);
	}

	return;
}

void
cont(Engine *E, State *S, ulong until_pc)
{
	/*	NOTE != as opposed to < etc.	*/
	while (S->PC != until_pc)
	{
		S->step(E, S, 0);
	}

	return;
}

void
help(Engine *E)
{
	int i;

	for (i = 0; i < nHelpstrs; i++)
	{
		mprint(E, NULL, siminfo, "%-24s", Helpstrs[i].cmd);

		if (((i != 0) && !((i+1) % 3)))
		{
			mprint(E, NULL, siminfo, "\n\t");
		}
	}

	if (i % 3)
	{
		mprint(E, NULL, siminfo, "\n");
	}

	mprint(E, NULL, siminfo, "There are %d commands and %d aliases:\n",
			uniqHelpstrs, nHelpstrs-uniqHelpstrs);
	mprint(E, NULL, siminfo,
		"Type \"man <command>\" for help on a particular command.\n");
	mprint(E, NULL, siminfo,
		"You can also type \"man <cmd>*\" to get a list of commands matching \"cmd\".\n");

	return;
}

void
man(Engine *E, char *cmd)
{
	int	tmplen, i, j, n = 0;


	/*	Wildcard	*/
	if (cmd[strlen(cmd) - 1] == '*')
	{
		cmd[strlen(cmd) - 1] = '\0';
		mprint(E, NULL, siminfo, "Commands and aliases beginning with \"%s\":\n\n\t", cmd);

		for (i = 0; i < nHelpstrs; i++)
		{
			if (!strncmp(cmd, Helpstrs[i].cmd, strlen(cmd)))
			{
				mprint(E, NULL, siminfo, "%-24s", Helpstrs[i].cmd);

				if (((n != 0) && !((n+1) % 3)))
				{
					mprint(E, NULL, siminfo, "\n\t");
				}
				n++;
			}
		}

		if (n % 3)
		{
			mprint(E, NULL, siminfo, "\n");
		}
		mprint(E, NULL, siminfo, "\nType \"man <command>\" for help on a particular command.\n");

		return;
	}

	for (i = 0; i < nHelpstrs; i++)
	{
		if (!strcmp(cmd, Helpstrs[i].cmd))
		{
			mprint(E, NULL, siminfo, "\n\tCommand:\t%s\n\n", Helpstrs[i].cmd);

			/*								*/
			/*	Format to 60 charracters wide, accounting for leading	*/
			/*	16 chars of 'Description' etc on first line		*/
			/*								*/
			tmplen = strlen(Helpstrs[i].intro);
			j = 0;
			while (j < tmplen)
			{
				mprint(E, NULL, siminfo, "%.*s",
						(j == 0 ? 60 : 60-16),
						&Helpstrs[i].intro[j]);

				j = min(tmplen, j+(j == 0 ? 60 : 60-16));

				if (j < tmplen) mprint(E, NULL, siminfo, "\\\n\t\t\t");
			}

			mprint(E, NULL, siminfo, "\n\n");

			tmplen = strlen(Helpstrs[i].args);
			j = 0;
			while (j < tmplen)
			{
				mprint(E, NULL, siminfo, "%.*s",
						(j == 0 ? 60 : 60-16),
						&Helpstrs[i].args[j]);
				j = min(tmplen, j+(j == 0 ? 60 : 60-16));

				if (j < tmplen) mprint(E, NULL, siminfo, "\\\n\t\t\t");
			}

			mprint(E, NULL, siminfo, "\n\n");

			return;
		}
	}
	help(E);

	return;
}

/*	Based on BSD strsep	*/
char *
mstrsep(char **stringptr, const char *delim)
{
	char		*s;
	const char	*spanp;
	int		c, sc;
	char		*tok;

	if ((s = *stringptr) == NULL)
	{
		return NULL;
	}

	for (tok = s;;)
	{
		c = *s++;
		spanp = delim;
		do
		{
			if ((sc = *spanp++) == c)
			{
				if (c == 0)
				{
					s = NULL;
				}
				else
				{
					s[-1] = 0;
				}
				*stringptr = s;

				return (tok);
			}
		} while (sc != 0);
	}

	return NULL;
}

void
m_dumpmem(Engine *E, State *S, long start_addr, long end_addr)
{
	ulong i;

	if ((start_addr >= S->MEMBASE) && (end_addr < S->MEMEND))
	{
		for (i = start_addr; i <= end_addr; i++)
		{
			mprint(E, S, nodeinfo, "MEM[0x" UHLONGFMT "] = [0x" UH2XFMT "]\n",
				i, S->MEM[i-S->MEMBASE]);
		}
	}
	else
	{
		mprint(E, NULL, siminfo, "Memory address out of range in DUMPMEM command\n");
	}

	return;
}

void
m_version(Engine *E)
{
	mprint(E, NULL, siminfo,
		"\nSunflower %s\n", MVERSION);
	mprint(E, NULL, siminfo,
		"Authored, 1999-2018: Phillip Stanley-Marbell <phillip.stanleymarbell@gmail.com>. 20018-onwards: See CONTRIBUTORS.TXT.\n");
	mprint(E, NULL, siminfo,
		"This software is provided with ");
	mprint(E, NULL, siminfo,
		"ABSOLUTELY NO WARRANTY. Read LICENSE.txt\n\n");

	return;
}

void
m_newnode(Engine *E, char *type, double x, double y, double z, char *trajfilename, int looptrajectory, int trajectoryrate)
{
	State	*S = NULL;


	if (E->nnodes >= MAX_SIMNODES)
	{
		mprint(E, NULL, siminfo, "Node creation limit reached. Sorry.");

		return;
	}

	/*  newnode xloc yloc zloc orbit velocity  */
	if ((strlen(type) == 0) || !strncmp(type, "superH", strlen("superH")))
	{
		/*		Prime the decode caches		*/
		for (int i = 0; i < (sizeof(E->superHDC)/sizeof(SuperHDCEntry)); i++)
		{
			superHdecode(E, (ushort)(i&0xFFFF), &E->superHDC[i].dc_p);
		}

		S = superHnewstate(E, x, y, z, trajfilename);
	}
	else if (!strncmp(type, "riscv", strlen("riscv")))
	{
		S = riscvnewstate(E, x, y, z, trajfilename);
	}
	else
	{
		merror(E, "Machine type \"%s\" not supported.", type);
	}

	if (S == NULL)
	{
		mprint(E, NULL, siminfo, "Node creation failed...");

		return;
	}

	if ((trajfilename!= NULL) && (strlen(trajfilename) > 0))
	{
		readnodetrajectory(E, S, trajfilename, looptrajectory, trajectoryrate);
	}

	return;
}

static void
readnodetrajectory(Engine *E, State *S, char*trajfilename, int looptrajectory, int trajectoryrate)
{
	enum		{MAX_LINELEN = 1024};
	char		c, buf[MAX_LINELEN], *ep = &c;
	double		val;
	int		tmpfd, linesread = 0;


	S->trajfilename = mcalloc(E, strlen(trajfilename)+1, sizeof(char), SF_FILE_MACRO);
	if (S->trajfilename == NULL)
	{
		merror(E, "Could not allocate memory for S->trajfilename.");
		return;
	}
	strcpy(S->trajfilename, trajfilename);


	tmpfd = mopen(S->trajfilename, M_OREAD);
	linesread = 0;
	while ((tmpfd  >= 0) && (mfgets(buf, MAX_LINELEN, tmpfd) != NULL))
	{
		if ((strlen(buf) > 0) && buf[strlen(buf)-1] == '\n')
		{
			buf[strlen(buf)-1] = '\0';
		}

		val = strtod(buf, &ep);
		if ((linesread == 0) && (*ep != '\0'))
		{
			merror(E, "Invalid \"# of records\" field (\"%s\") in trajectory file.", buf);
			continue;
		}

		if (linesread == 0)
		{
			S->path.xloc = mcalloc(E, (int)val, sizeof(double), SF_FILE_MACRO);
			if (S->path.xloc == NULL)
			{
				merror(E, "Could not allocate memory for S->path.xloc.");
				return;
			}

			S->path.yloc = mcalloc(E, (int)val, sizeof(double), SF_FILE_MACRO);
			if (S->path.yloc == NULL)
			{
				merror(E, "Could not allocate memory for S->path.yloc.");
				return;
			}

			S->path.zloc = mcalloc(E, (int)val, sizeof(double), SF_FILE_MACRO);
			if (S->path.zloc == NULL)
			{
				merror(E, "Could not allocate memory for S->path.zloc.");
				return;
			}

			S->path.rho = mcalloc(E, (int)val, sizeof(double), SF_FILE_MACRO);
			if (S->path.rho == NULL)
			{
				merror(E, "Could not allocate memory for S->path.rho.");
				return;
			}

			S->path.theta = mcalloc(E, (int)val, sizeof(double), SF_FILE_MACRO);
			if (S->path.theta == NULL)
			{
				merror(E, "Could not allocate memory for S->path.theta.");
				return;
			}

			S->path.phi = mcalloc(E, (int)val, sizeof(double), SF_FILE_MACRO);
			if (S->path.phi == NULL)
			{
				merror(E, "Could not allocate memory for S->path.phi.");
				return;
			}

			mprint(E, NULL, siminfo,
				"[%d] records in trajectory file\n", (int)val);

		}
		else
		{
			int	i = 0;
			char	*p;


			for ((p = strtok(buf, " \n\t")); p; (p = strtok(NULL, " \n\t")), i++)
			{
				switch (i)
				{
					case 0:
					{
						S->path.xloc[S->path.nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror(E, "Invalid xloc in trajectory file.");
						}
//fprintf(stderr, "Read S->path.xloc[%d] = [%f], S->path.xloc = [%x]\n", S->path.nlocations, S->path.xloc[S->path.nlocations], S->path.xloc);
						break;
					}

					case 1:
					{
						S->path.yloc[S->path.nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror(E, "Invalid yloc in trajectory file.");
						}

						break;
					}

					case 2:
					{
						S->path.zloc[S->path.nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror(E, "Invalid zloc in trajectory file.");
						}

						break;
					}

					case 3:
					{
						S->path.rho[S->path.nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror(E, "Invalid rho in trajectory file.");
						}

						break;
					}

					case 4:
					{
						S->path.theta[S->path.nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror(E, "Invalid theta in trajectory file.");
						}

						break;
					}

					case 5:
					{
						S->path.phi[S->path.nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror(E, "Invalid phi in trajectory file.");
						}

						break;
					}

					default:
					{
						merror(E, "Extra field in trajectory file.");
					}

				}
			}
			S->path.nlocations++;

		}
		linesread++;
	}
	S->path.trajectory_rate = trajectoryrate;
	S->path.looptrajectory = looptrajectory;

	return;
}

void
traj_feed(Engine *E)
{
	int	i;
	State	*S;


	/*	Similar to what we do in physics_feed: we interpolate		*/
	for (i = 0; i < E->nnodes; i++)
	{
		S = E->sp[i];
		if (S->path.nlocations > 1)
		{
			double	idx_low, idx_high, lskew, hskew;
			double	idx_real = E->globaltimepsec * S->path.trajectory_rate;

			if (idx_real >= (double) S->path.nlocations)
			{
				if (S->path.looptrajectory)
				{
					idx_real = fmod(idx_real, (double) S->path.nlocations);
				}
				else
				{
					idx_real = (double) (S->path.nlocations - 1);
				}
			}
			idx_low = floor(idx_real);
			idx_high = ceil(idx_real);

			lskew = (idx_high - idx_real) / (idx_high - idx_low);
			hskew = (idx_real - idx_low) / (idx_high - idx_low);

			S->xloc	= lskew*S->path.xloc[(int)idx_low]	+ hskew*S->path.xloc[(int)idx_high];
			S->yloc = lskew*S->path.yloc[(int)idx_low]	+ hskew*S->path.yloc[(int)idx_high];
			S->zloc = lskew*S->path.zloc[(int)idx_low]	+ hskew*S->path.zloc[(int)idx_high];

			S->rho	= lskew*S->path.rho[(int)idx_low]	+ hskew*S->path.rho[(int)idx_high];
			S->theta= lskew*S->path.theta[(int)idx_low]	+ hskew*S->path.theta[(int)idx_high];
			S->phi	= lskew*S->path.phi[(int)idx_low]	+ hskew*S->path.phi[(int)idx_high];

//fprintf(stderr, "In traj_feed, idx_real=%f, S->path.nlocations=%d, S->path.looptrajectory=%d, E->globaltimepsec=[%f], S->path.trajectory_rate=[%d], idx_low=%f, idx_high=%f, lskew=%f, hskew=%f,  S->path.xloc[%d]=[%f], S->xloc = [%f]\n", idx_real, S->path.nlocations, S->path.looptrajectory, E->globaltimepsec, S->path.trajectory_rate, idx_low, idx_high, lskew, hskew, (int)idx_low, S->path.xloc[(int)idx_low], S->xloc);

		}
	}

	return;
}

void
m_powertotal(Engine *E)
{
	int	i;
	double	total_power=0.0, total_energy=0.0;


	for (i = 0; i < E->nnodes; i++)
	{
		total_power += E->sp[i]->energyinfo.CPUEtot;
		total_energy += E->sp[i]->energyinfo.CPUEtot/
			(E->sp[i]->ICLK * E->sp[i]->CYCLETIME);
	}

	mprint(E, NULL, siminfo, "Average power accross all nodes = %E\n",
		total_power/i);
	mprint(E, NULL, siminfo, "Total energy accross all nodes = %E\n",
		total_energy);

	return;
}

int
m_find_numa(ulong vaddr, Numa *N, int start, int count)
{
	int		x, y;
	Numaregion	*a;
	Numaregion	*b;

	if (start < 0 || count <= 0 || (start+count) > N->count)
	{
		return -1;
	}

	a = N->regions[start];
	b = N->regions[start+count-1];

	if (a == NULL || b == NULL)
	{
		return -1;
	}

	//printf("find_numa(): start=%d, count=%d\n", start, count);

	if (count == 1)
	{
		if ((vaddr >= a->startaddr) && (vaddr < a->endaddr))
		{
			return start;
		}

		return -1;
	}

	if ((vaddr > b->startaddr) || (vaddr < a->startaddr))
	{
		return -1;
	}

	x = m_find_numa(vaddr, N, start, count/2);
	y = m_find_numa(vaddr, N, start+count/2, count/2);

	return max(x,y);
}

int
m_find_numastack(ulong curfn, ulong curframe, ulong vaddr, Numa *N, int start, int count)
{
	int		x, y;
	Numaregion	*a = N->regions[start];
	Numaregion	*b = N->regions[start+count-1];

	if (start < 0 || count <= 0 || (start+count) > N->count)
	{
		return -1;
	}

	a = N->regions[start];
	b = N->regions[start+count-1];

	if (a == NULL || b == NULL)
	{
		return -1;
	}

	if (count == 1)
	{
		ulong		begin = curframe+a->frameoffset;
		ulong		end = curframe + a->frameoffset + (a->endaddr - a->startaddr);

		if (	(curfn == a->pcstart) &&
			((vaddr >= begin)&&(vaddr < end)))
		{
			return start;
		}

		return -1;
	}

	if (	(curfn > b->pcstart) || (curfn < a->pcstart) ||
		((curfn == a->pcstart) && (vaddr < curframe+a->frameoffset)) ||
		((curfn == b->pcstart) && (vaddr > curframe+b->frameoffset)))
	{
		return -1;
	}

	x = m_find_numastack(curfn, curframe, vaddr, N, start, count/2);
	y = m_find_numastack(curfn, curframe, vaddr, N, start+count/2, count/2);

	return max(x,y);
}

int
m_sort_numaregions_comp(const void *x, const void *y)
{
	Numaregion *a = *((Numaregion **)x);
	Numaregion *b = *((Numaregion **)y);

	if (	(a->startaddr > b->startaddr) ||
		((a->startaddr == b->startaddr) && (a->pcstart > b->pcstart)) ||
		((a->startaddr == b->startaddr) && (a->pcstart == b->pcstart) && (a->frameoffset > b->frameoffset)))
	{
		return 1;
	}

	if (	(a->startaddr == b->startaddr) &&
		(a->endaddr == b->endaddr) &&
		(a->pcstart == b->pcstart) &&
		(a->frameoffset == b->frameoffset))
	{
		return 0;
	}

	return -1;
}

int
m_sort_regtracers_comp(const void *x, const void *y)
{
	Regvt *a = *((Regvt **)x);
	Regvt *b = *((Regvt **)y);

	if (	(a->pcstart > b->pcstart) ||
		((a->pcstart == b->pcstart) && (a->regnum > b->regnum)))
	{
		return 1;
	}

	if (	(a->pcstart == b->pcstart) &&
		(a->regnum == b->regnum))
	{
		return 0;
	}

	return -1;
}

void
m_regtracer(Engine *E, State *S, char *name, ulong pcstart, int regnum, int size, int ispointer)
{
	int		i;
	Regvt		*tmp;

	//TODO: replace with hash fn access
	for (i = 0; i < S->RT->count; i++)
	{
		if ((regnum == S->RT->regvts[i]->regnum) &&
			(pcstart == S->RT->regvts[i]->pcstart) &&
			!strncmp(S->RT->regvts[i]->name, name, MAX_REGTRACER_NAMELEN))
		{
			return;
		}
	}

	if (S->RT->count == MAX_REG_TRACERS)
	{
		mprint(E, S, nodeinfo,
			"Maximum number of register tracers reached");
		return;
	}


	tmp = (Regvt *) mcalloc(E, 1, sizeof(Regvt), "S->RT->regvts entry in main.c");
	if (tmp == NULL)
	{
		sfatal(E, S, "mcalloc failed for S->RT->regvts entry in main.c");
	}

	tmp->values = (ulong *) mcalloc(E, BASE_REGTRACER_VALUETRACE, sizeof(ulong), "S->RT->regvts->values entry in main.c");
	if (tmp->values == NULL)
	{
		sfatal(E, S, "mcalloc failed for S->RT->regvts->values entry in main.c");
	}
	tmp->nvalues = BASE_REGTRACER_VALUETRACE;

	tmp->devns = (ulong *) mcalloc(E, BASE_REGTRACER_DEVNTRACE, sizeof(ulong), "S->RT->regvts->devns entry in main.c");
	if (tmp->devns == NULL)
	{
		sfatal(E, S, "mcalloc failed for S->RT->regvts->devns entry in main.c");
	}
	tmp->ndevns = BASE_REGTRACER_DEVNTRACE;

	tmp->pcstart = pcstart;
	tmp->regnum = regnum;
	tmp->size = size;
	tmp->ispointer = ispointer;

	/*
	 *	Copy only MAX_REGTRACER_NAMELEN-1 bytes to ensure that name is always NULL-terminated.
	 */
	strncpy(tmp->name, name, MAX_REGTRACER_NAMELEN-1);

	S->RT->regvts[S->RT->count] = tmp;
	S->RT->count++;

	qsort(S->RT->regvts, S->RT->count, sizeof(Regvt *), m_sort_regtracers_comp);

	return;
}

void
do_numaregion(Engine *E, State *S, char *name, ulong start, ulong end, long xlrlat,
	long xlwlat, long xrrlat, long xrwlat, int mapid, ulong offset, int private,
	int valuetrace, int onstack, ulong pcstart, int frameoffset, int ispointer)
{
	int		i;
	Numaregion	*tmp;
	Numa		*X;
	long		lwlat = xlwlat, lrlat = xlrlat;
	long		rwlat = xrwlat, rrlat = xrrlat;


	if (onstack)
	{
		X = S->Nstack;
	}
	else
	{
		X = S->N;
	}

	if (lrlat < 0)
	{
		lrlat = S->mem_r_latency;
	}
	if (lwlat < 0)
	{
		lwlat = S->mem_w_latency;
	}
	if (rrlat < 0)
	{
		rrlat = S->mem_r_latency;
	}
	if (rwlat < 0)
	{
		rwlat = S->mem_w_latency;
	}

	/*								*/
	/*	If and identical address range exists, reuse it with	*/
	/*	the supplied params.					*/
	/*								*/
	//TODO: replace with hash function access
	for (i = 0; i < X->count; i++)
	{
		if ((start == X->regions[i]->startaddr) &&
			(end == X->regions[i]->endaddr) &&
			(mapid == X->regions[i]->map_id) &&
			(offset == X->regions[i]->map_offset) &&
			!strncmp(X->regions[i]->name, name, MAX_NUMAREGION_NAMELEN))
		{
			tmp = X->regions[i];
			tmp->local_read_latency = lrlat;
			tmp->local_write_latency = lwlat;
			tmp->remote_read_latency = rrlat;
			tmp->remote_write_latency = rwlat;

			return;
		}
	}

	if (X->count == MAX_NUMA_REGIONS)
	{
		mprint(E, S, nodeinfo,
			"Maximum number of NUMA regions reached");
		return;
	}

	tmp = (Numaregion *) mcalloc(E, 1, sizeof(Numaregion), "X->regions entry in main.c");
	if (tmp == NULL)
	{
		sfatal(E, S, "mcalloc failed for X->regions entry in main.c");
	}

	tmp->values = (ulong *) mcalloc(E, BASE_NUMAREGION_VALUETRACE, sizeof(ulong), "X->regions->values entry in main.c");
	if (tmp->values == NULL)
	{
		sfatal(E, S, "mcalloc failed for X->regions->values entry in main.c");
	}
	tmp->nvalues = BASE_NUMAREGION_VALUETRACE;

	tmp->startaddr = start;
	tmp->endaddr = end;
	tmp->local_read_latency = lrlat;
	tmp->local_write_latency = lwlat;
	tmp->remote_read_latency = rrlat;
	tmp->remote_write_latency = rwlat;
	tmp->nreads = 0;
	tmp->nwrites = 0;
	tmp->map_id = mapid;
	tmp->map_offset = offset;
	tmp->private = private;
	tmp->valuetrace = valuetrace;
	tmp->validx = 0;

	tmp->onstack = onstack;
	tmp->pcstart = pcstart;
	tmp->frameoffset = frameoffset;
	tmp->ispointer = ispointer;

	strncpy(tmp->name, name, MAX_NUMAREGION_NAMELEN);

	X->regions[X->count] = tmp;
	X->count++;

	qsort(X->regions, X->count, sizeof(Numaregion *), m_sort_numaregions_comp);


	return;
}

void
m_numaregion(Engine *E, char *name, ulong start, ulong end, long xlrlat,
	long xlwlat, long xrrlat, long xrwlat, int mapid, ulong offset, int private)
{
	int	i;

	/*								*/
	/*	Numa regions get installed on all nodes. The mapid	*/
	/*	and private flag determine whether it is public/priv	*/
	/*	and who the owner/home node is for private regions.	*/
	/*								*/
	for (i = 0; i < E->nnodes; i++)
	{
		State	*S = E->sp[i];

		do_numaregion(E, S, name, start, end, xlrlat, xlwlat, xrrlat,
			xrwlat, mapid, offset, private, 0, 0, 0, 0, 0);
	}

	return;
}

void
m_regtracerstats(Engine *E, State *S)
{
	int	i, j;

	for (i = 0; i < MAX_REG_TRACERS; i++)
	{
		if (S->RT->regvts[i] == NULL)
		{
			continue;
		}

		mprint(E, NULL, siminfo, "\n%-20s %s\n", "Name:", S->RT->regvts[i]->name);
		mprint(E, NULL, siminfo, "%-20s 0x" UHLONGFMT "\n", "PCstart:", S->RT->regvts[i]->pcstart);
		mprint(E, NULL, siminfo, "%-20s 0x" UHLONGFMT "\n", "Register:", S->RT->regvts[i]->regnum);
		mprint(E, NULL, siminfo, "Value History: ");

		for (j = 0; j < S->RT->regvts[i]->validx; j++)
		{
			mprint(E, NULL, siminfo, ULONGFMT " ", S->RT->regvts[i]->values[j]);
		}
		mprint(E, NULL, siminfo, "\n");
	}

	return;
}

void
m_numastats(Engine *E, State *S)
{
	int	i;

	for (i = 0; i < MAX_NUMA_REGIONS; i++)
	{
		if (S->N->regions[i] == NULL)
		{
			continue;
		}

		mprint(E, NULL, siminfo,
			"\n%-20s %s\n", "Name:", S->N->regions[i]->name);

		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Start address:", S->N->regions[i]->startaddr);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "End address:", S->N->regions[i]->endaddr);

		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Local Read latency:",
			S->N->regions[i]->local_read_latency);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Local Write latency:",
			S->N->regions[i]->local_write_latency);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Remote Read latency:",
			S->N->regions[i]->remote_read_latency);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Remote Write latency:",
			S->N->regions[i]->remote_write_latency);

		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Map ID:", S->N->regions[i]->map_id);
		mprint(E, NULL, siminfo,
			"%-20s " UHLONGFMT "\n", "Private:", S->N->regions[i]->private);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Map offset:", S->N->regions[i]->map_offset);

		mprint(E, NULL, siminfo,
			"%-20s %d\n", "Read accesses:", S->N->regions[i]->nreads);
		mprint(E, NULL, siminfo,
			"%-20s %d\n\n", "Write accesses:", S->N->regions[i]->nwrites);
	}

	return;
}

void
m_numastatsall(Engine *E)
{
	int	i;

	for (i = 0; i < E->nnodes; i++)
	{
		m_numastats(E, E->sp[i]);
	}

	return;
}

void
m_numasetmapid(Engine *E, int whichmap, int cpuid)
{
	int	i;

	for (i = 0; i < E->nnodes; i++)
	{
		if (whichmap >= E->sp[i]->N->count)
		{
			sfatal(E, E->sp[i],
				"Access to numa region out of range of valid regions");
		}

		if (cpuid >= E->nnodes)
		{
			sfatal(E, E->sp[i],
				"Invalid cpuid argument to numasetmapid");
		}

		E->sp[i]->N->regions[whichmap]->map_id = cpuid;
	}

	/*	Lock bus for time prop to size of mapping	*/
	E->sp[cpuid]->superH->B->pbuslock = 1;
	E->sp[cpuid]->superH->B->pbuslocker = cpuid;

	//TODO: this is not the cleanest way to do this:
	E->sp[cpuid]->sleep = 0;
	E->sp[cpuid]->superH->P.MA.valid = 1;
	//Any local table as good as the other for size info. We use info from node0
	E->sp[cpuid]->superH->P.MA.cycles =
		E->sp[0]->N->regions[whichmap]->endaddr -
		E->sp[0]->N->regions[whichmap]->startaddr;

	return;
}

void
m_addvaluetrace(Engine *E, State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer)
{
	do_numaregion(E, S, tag, addr, addr+size, -1, -1, -1, -1, S->NODE_ID, 0, 0, 1, onstack, pcstart, frameoffset, ispointer);
}

void
m_delvaluetrace(Engine *E, State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer)
{
	int	i, j;

	for (i = 0; i < S->Nstack->count; i++)
	{
		if (S->Nstack->regions[i] != NULL &&
			S->Nstack->regions[i]->valuetrace &&
			!strcmp(S->Nstack->regions[i]->name, tag) &&
			S->Nstack->regions[i]->startaddr == addr &&
			S->Nstack->regions[i]->endaddr == addr+size &&
			S->Nstack->regions[i]->onstack == onstack &&
			S->Nstack->regions[i]->pcstart == pcstart &&
			S->Nstack->regions[i]->frameoffset == frameoffset &&
			S->Nstack->regions[i]->ispointer == ispointer)
		{
			/*								*/
			/*	Dump this value trace to disk before deleting it	*/
			/*								*/
			mlog(E, S, "\n%-20s %s\n", "Name:", S->Nstack->regions[i]->name);
			mlog(E, S, "%-20s 0x" UHLONGFMT "\n", "pcstart:",
				S->Nstack->regions[i]->pcstart);
			mlog(E, S, "%-20s 0x" UHLONGFMT "\n", "frame offset:",
				S->Nstack->regions[i]->frameoffset);
			mlog(E, S, "%-20s 0x" UHLONGFMT "\n", "size:",
				S->Nstack->regions[i]->endaddr-S->Nstack->regions[i]->startaddr);
			mlog(E, S, "%-20s %d\n", "Read accesses:", S->Nstack->regions[i]->nreads);
			mlog(E, S, "%-20s %d\n\n", "Write accesses:", S->Nstack->regions[i]->nwrites);
			mlog(E, S, "Value History: ");
			for (j = 0; j < S->Nstack->regions[i]->validx; j++)
			{
				mlog(E, S, ULONGFMT " ", S->Nstack->regions[i]->values[j]);
			}
			mlog(E, S, "\n");

			mfree(E, S->Nstack->regions[i], "S->Nstack->regions in m_delvaluetrace");
			S->Nstack->count--;
			S->Nstack->regions[i] = S->Nstack->regions[S->Nstack->count];
			S->Nstack->regions[S->Nstack->count] = NULL;

			break;
		}
	}

	for (i = 0; i < S->N->count; i++)
	{
		if (S->N->regions[i] != NULL &&
			S->N->regions[i]->valuetrace &&
			!strcmp(S->N->regions[i]->name, tag) &&
			S->N->regions[i]->startaddr == addr &&
			S->N->regions[i]->endaddr == addr+size &&
			S->N->regions[i]->onstack == onstack &&
			S->N->regions[i]->pcstart == pcstart &&
			S->N->regions[i]->frameoffset == frameoffset &&
			S->N->regions[i]->ispointer == ispointer)
		{
			/*								*/
			/*	Dump this value trace to disk before deleting it	*/
			/*								*/
			mlog(E, S, "\n%-20s %s\n", "Name:", S->N->regions[i]->name);
			mlog(E, S, "%-20s 0x" UHLONGFMT "\n", "Start address:",
				S->N->regions[i]->startaddr);
			mlog(E, S, "%-20s 0x" UHLONGFMT "\n", "End address:",
				S->N->regions[i]->endaddr);
			mlog(E, S, "%-20s %d\n", "Read accesses:", S->N->regions[i]->nreads);
			mlog(E, S, "%-20s %d\n\n", "Write accesses:", S->N->regions[i]->nwrites);
			mlog(E, S, "Value History: ");
			for (j = 0; j < S->N->regions[i]->validx; j++)
			{
				mlog(E, S, ULONGFMT " ", S->N->regions[i]->values[j]);
			}
			mlog(E, S, "\n");

			mfree(E, S->N->regions[i], "S->N->regions in m_delvaluetrace");
			S->N->count--;
			S->N->regions[i] = S->N->regions[S->N->count];
			S->N->regions[S->N->count] = NULL;

			break;
		}
	}

	return;
}

void
m_valuestats(Engine *E, State *S)
{
	int	i, j;

	for (i = 0; i < S->Nstack->count; i++)
	{
		if (S->Nstack->regions[i] == NULL || !S->Nstack->regions[i]->valuetrace)
		{
			continue;
		}

		mprint(E, NULL, siminfo,
			"\n%-20s %s\n", "Name:", S->Nstack->regions[i]->name);

		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "PCstart:", S->Nstack->regions[i]->pcstart);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Frame offset:", S->Nstack->regions[i]->frameoffset);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Size:", S->Nstack->regions[i]->endaddr-S->Nstack->regions[i]->startaddr);

		mprint(E, NULL, siminfo,
			"%-20s %d\n", "Read accesses:", S->Nstack->regions[i]->nreads);
		mprint(E, NULL, siminfo,
			"%-20s %d\n\n", "Write accesses:", S->Nstack->regions[i]->nwrites);

		mprint(E, NULL, siminfo, "Value History: ");

		for (j = 0; j < S->Nstack->regions[i]->validx; j++)
		{
			mprint(E, NULL, siminfo, ULONGFMT " ", S->Nstack->regions[i]->values[j]);
		}
		mprint(E, NULL, siminfo, "\n");
	}

	for (i = 0; i < S->N->count; i++)
	{
		if (S->N->regions[i] == NULL || !S->N->regions[i]->valuetrace)
		{
			continue;
		}

		mprint(E, NULL, siminfo,
			"\n%-20s %s\n", "Name:", S->N->regions[i]->name);


		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Start address:", S->N->regions[i]->startaddr);
		mprint(E, NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "End address:", S->N->regions[i]->endaddr);

		mprint(E, NULL, siminfo,
			"%-20s %d\n", "Read accesses:", S->N->regions[i]->nreads);
		mprint(E, NULL, siminfo,
			"%-20s %d\n\n", "Write accesses:", S->N->regions[i]->nwrites);

		mprint(E, NULL, siminfo, "Value History: ");

		for (j = 0; j < S->N->regions[i]->validx; j++)
		{
			mprint(E, NULL, siminfo, ULONGFMT " ", S->N->regions[i]->values[j]);
		}
		mprint(E, NULL, siminfo, "\n");
	}

	m_regtracerstats(E, S);

	return;
}

void
m_parseobjdump(Engine *E, State *S, char *filename)
{
/*
	Diasm	d;
typedef struct
{
	ulong	addr;
	uchar	*bytes;
	char	*asmstr;

	char	*fname;
	int	lineno;
} Disasm;
*/
	/*								*/
	/*	Example line format:					*/
	/*								*/
	/*	8c094362:	81 46		mov.w	r0,@(12,r4)	*/
	/*								*/
/*
	while ()
	{
		try to match either tab or whitespace instead of just tab

		scanf("%x:\t%s\t%s", d.addr, d.bytes, d.asmstr);
	}
*/
}

void
m_renumbernodes(Engine *E)
{
	int i;

	for (i = 0; i < E->nnodes; i++)
	{
		E->sp[i]->NODE_ID = i + E->baseid;
	}

	return;
}

void
m_sizemem(Engine *E, State *S, int size)
{
	uchar *tmp;
	ShadowMem *tainttmp;

	/*
	*	Memory reallocation:
	*/

	if (S->MEM == NULL)
	{
		S->MEM = (uchar *)mcalloc(E, 1, size, "(uchar *)S->MEM");
		if (S->MEM == NULL)
		{
			mexit(E, "Could not allocate mem for S->MEM in main.c", -1);
		}

		return;
	}

	tmp = (uchar *)mrealloc(E, S->MEM, size, "(uchar *)S->MEM");
	if (tmp == NULL)
	{
		mprint(E, S, nodeinfo,
			"SIZEMEM failed: could not allocate memory for %d bytes.\n", size);
	}
	else
	{
		S->MEM = tmp;
		S->MEMSIZE = size;
		S->MEMEND = S->MEMBASE+S->MEMSIZE;
		mprint(E, S, nodeinfo,
			"Set memory size to %d Kilobytes\n", S->MEMSIZE/1024);
	}

	if (S->riscv != NULL && S->riscv->uncertain != NULL)
	{
		uncertain_sizemem(E, S, size);
	}

	/*
	*	Shadow/taintmemory reallocation:
	*/
	if (S->TAINTMEM == NULL)
	{
		S->TAINTMEM = (ShadowMem *)mcalloc(E, 1, sizeof(ShadowMem)*size, "(ShadowMem *)S->TAINTMEM");
		if (S->MEM == NULL)
		{
			mexit(E, "Could not allocate mem for S->TAINTMEM in main.c", -1);
		}

		return;
	}

	tainttmp = (ShadowMem *)mrealloc(E, S->TAINTMEM, sizeof(ShadowMem)*size, "(ShadowMem *)S->TAINTMEM");
	if (tainttmp == NULL)
	{
		mprint(E, S, nodeinfo,
			"SIZEMEM Shadow failed: could not allocate memory for %d bytes.\n", (size*sizeof(ShadowMem)));
	}
	else
	{
		S->TAINTMEM = tainttmp;
		S->TAINTMEMSIZE = size*sizeof(ShadowMem);
		S->TAINTMEMEND = S->TAINTMEMBASE+S->TAINTMEMSIZE;
		mprint(E, S, nodeinfo,
			"Set shadow memory size to %d Kilobytes\n", (S->TAINTMEMSIZE)/1024);
	}

	return;
}

void
m_run(Engine *E, State *S, char *args)
{
	/*							*/
	/*	We tokenize the args directly into the mem	*/
	/*	space of the simulated machine. The entire	*/
	/*	arg string ($2) is first placed into the	*/
	/*	simulated machine's mem at location:		*/
	/*		ARGVOFFSET + strlen($2)			*/
	/*	below top of memory, so it does not get nuked	*/
	/*	before a main() is called, assuming 64K is	*/
	/*	enough stack space during init, which I think	*/
	/*	more than suffices. The argv goes below it.	*/
	/*							*/
	int	argc, ARGVOFFSET = 16, argstrlen, align;
	char	**tptr, **argv, *simstr;
	ulong	argvptroffset;


	argstrlen = strlen(args)+1;
	if ((ARGVOFFSET + argstrlen) > S->MEMSIZE)
	{
		mprint(E, NULL, siminfo, "Memory is too small, ARGV offset outside memory!\n");
		mprint(E, NULL, siminfo, "Sunflower Exiting (see above errors)...\n");
		mexit(E, "See above messages.", -1);
	}

	/*							*/
	/*	Copy arg string into simulated machine's mem.	*/
	/*							*/
	simstr = (char*)&S->MEM[S->MEMSIZE - (ARGVOFFSET+argstrlen)];
	strcpy(simstr, args);

	/*	This is where pointers to the parsed argv go	*/
	argvptroffset = ARGVOFFSET + argstrlen + MAX_RUN_ARGS*SUPERH_BYTES_PER_POINTER;

	/*							*/
	/*	Force alignment on 8 byte boundary. Primarily	*/
	/*	for (@%$*!!!) Solaris, can't hurt elsewhere.	*/
	/*							*/
	align = (ulong)(&S->MEM[S->MEMSIZE - argvptroffset]) & B0111;
	align = (align == 0 ? 0 : 8 - align);

	/*							*/
	/*	Set location of argv in sim machine's mem	*/
	/*							*/
	argv = (char **)&S->MEM[S->MEMSIZE - argvptroffset + align];


	/*							*/
	/*	Break args[] into argv[], and count argc.	*/
	/*	argc goes into R4, the arg strings go on	*/
	/*	stack (top of mem, as per mem.h), and a		*/
	/*	pointer to this vector goes in R5. This		*/
	/*	is all as per the GCC Hitachi SH ABI.		*/
	/*	We model SH as big-E, so do any necessary	*/
	/*	byte reordering.				*/
	/*							*/
	argc = 0;
	for (tptr = argv; (*tptr = mstrsep(&simstr, " \t")) != NULL; )
	{
		ulong	tmp_123, tmp_321;


		if (**tptr == '\0')
		{
			continue;
		}

		tmp_123 = (ulong)argv[argc] - (ulong)&S->MEM[0] +
				S->MEMBASE;

		/*		Reverse byte order	*/
		tmp_321 =
			(((tmp_123 & 0xFF000000) >> 24) |
			((tmp_123 & 0x00FF0000) >> 8) |
			((tmp_123 & 0x0000FF00) << 8) |
			((tmp_123 & 0x000000FF) << 24));

#ifdef	SF_L_ENDIAN
			argv[argc] = (char *)tmp_321;
#else
			argv[argc] = (char *)tmp_123;
#endif

		argc++;
		if (++tptr >= &argv[MAX_RUN_ARGS])
		{
			break;
		}
	}

	if (S->machinetype == MACHINE_SUPERH)
	{
		S->superH->R[4] = argc;
		S->superH->R[5] = S->MEMEND - argvptroffset + align;

		mprint(E, S, nodeinfo, "args = [%s], argc = %d\n",
			args, argc);
		mprint(E, S, nodeinfo, "R4 = [0x" UH8LONGFMT "], R5 = [0x" UH8LONGFMT "]\n",
			S->superH->R[4], S->superH->R[5]);

		S->runnable = 1;
		mprint(E, S, nodeinfo, "Running...\n\n");
	}
	else if (S->machinetype == MACHINE_RISCV)
	{
		S->riscv->R[2] = S->MEMEND - argvptroffset + align;

		mprint(E, S, nodeinfo, "args = [%s], argc = %d\n",
			args, argc);
		mprint(E, S, nodeinfo, "R2 = [0x%08lx]\n",
			S->riscv->R[2]);

		S->runnable = 1;
		mprint(E, S, nodeinfo, "Running...\n\n");
	}
	else
	{
		merror(E, "This machine does not know how to \"run\".");
	}

	return;
}

void
m_on(Engine *E, State *S)
{
	int	i;
	ulong	now;


	if (E->on)
	{
		merror(E, "Already on!.");
		return;
	}

	now = musercputimeusecs();
	for (i = 0; i < E->nnodes; i++)
	{
		E->sp[i]->startclk = E->sp[i]->ICLK;
		E->sp[i]->ustart = now;

		E->sp[i]->trip_ustart = now;
		E->sp[i]->trip_startclk = E->sp[i]->ICLK;
	}

	E->on = 1;
	if (E->nodetach)
	{
		scheduler(E);
	}
	else
	{
		if (mspawnscheduler(E) < 0)
		{
			sfatal(E, S, "Could not create thread in ON call");
		}
	}

	return;
}

void
m_off(Engine *E, State *S)
{
	if (!E->on)
	{
		mprint(E, NULL, siminfo, "Simulator already off/paused...\n\n");
		return;
	}

	if (mkillscheduler(E))
	{
		mprint(E, NULL, siminfo, "Warning: could not kill scheduler thread...\n\n");
		return;
	}

	S->ufinish = musercputimeusecs();
	S->finishclk = S->ICLK;

	mprint(E, NULL, siminfo, "Simulator Paused.\n\n");
	mprint(E, NULL, siminfo, "User Time elapsed = %.6f seconds.\n",
			((float)(S->ufinish - S->ustart)/1E6));

	if (E->quantum == 1)
	{
		mprint(E, NULL, siminfo, "Simulated CPU Time elapsed = %.6E seconds.\n",
			S->TIME);
		mprint(E, NULL, siminfo, "Simulated Clock Cycles = " UVLONGFMT "\n",
			S->finishclk - S->startclk);
		mprint(E, NULL, siminfo, "Cycles Spent Waiting = " UVLONGFMT " (%.2f%%)\n",
			S->num_cycles_waiting, 100*((float)(S->num_cycles_waiting))/(float)(S->finishclk - S->startclk));

	}

	mprint(E, NULL, siminfo,
		"Instruction Simulation Rate on node %d = %.2f Cycles/Second.\n\n",
		S->NODE_ID, (((float)(S->finishclk - S->startclk))/(((float)
		(S->ufinish - S->ustart))/1E6)));

	E->on = 0;

	return;
}

void
m_sharebus(Engine *E, State *S, int donorid)
{
	State	*R;

	if (donorid > E->nnodes || donorid < 0)
	{
		mprint(E, NULL, siminfo,
			"Invalid Bus Donor (id %d)\n", donorid);
		return;
	}

	R = E->sp[donorid];
	if (R == NULL)
	{
		mprint(E, NULL, siminfo,
			"Bus Donor (id %d) has NULL state struct\n", donorid);
		return;
	}

	mfree(E, S->superH->B, "S->superH->B in m_sharebus");
	S->superH->B = R->superH->B;

	return;
}

void
m_dumpnode(Engine *E, int i, char *filename, int mode, char *tag, char *pre)
{
	int	j, k;
	State	tmp, *S;
	int	txok = 0, rxok = 0, addrerr = 0, frmerr = 0,
		collserr = 0, csenseerr = 0, rxovrnerr = 0,
		rxundrrunerr = 0, txovrnerr = 0, txundrrunerr = 0,
		maxoccupancy = 0;
	State	*X = E->sp[i];


	S = &tmp;
	S->ufinish = musercputimeusecs();

	S->logfd = mcreate(filename, mode);
	if (S->logfd <= 2)
	{
		mprint(E, NULL, siminfo,
			"Could not open [%s] for writing, create returned fd \"%d\"\n",
			filename, S->logfd);
		return;
	}

	mlog(E, S, "\n\n%sTag %s{\n", pre, tag);

	/*  Abbreviate to reduce size of dist logs */
	if (1)
	{
		mlog(E, S, "%sNode%d\t\tUser Time elapsed = %.6f seconds.\n",
			pre, X->NODE_ID,
			((float)(S->ufinish - X->ustart)/1E6));

		if ((S->ufinish - X->ustart) > 0)
		{
			mlog(E, S,
				"%sNode%d\t\tInstruction Simulation Rate = %.2f Cycles/Second.\n",
				pre, X->NODE_ID,
				(((float)(X->finishclk - X->startclk))/(((float)
				(S->ufinish - X->ustart))/1E6)));
		}
		else
		{
			mlog(E, S,
				"%sNode%d\t\tInstruction Simulation Rate = %.2f Cycles/Second.\n",
				pre, X->NODE_ID, 0.0);
		}

		mlog(E, S, "%sNode%d\t\t\"machinetype\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->machinetype);
		mlog(E, S, "%sNode%d\t\t\"Cycletrans\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->Cycletrans);
		mlog(E, S, "%sNode%d\t\t\"CYCLETIME\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->CYCLETIME);
		mlog(E, S, "%sNode%d\t\tNTRANS\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->energyinfo.ntrans);
		mlog(E, S, "%sNode%d\t\tCPU-only ETOT\t=\t%.6E Joules\n",
			pre, X->NODE_ID,
			X->energyinfo.CPUEtot);

		if (X->ICLK == 0 || X->CYCLETIME == 0)
		{
			mlog(E, S, "%sNode%d\t\tCPU-only AVG POWER\t=\t%.6E Watts\n",
				pre, X->NODE_ID, 0.0);
		}
		else
		{
			mlog(E, S, "%sNode%d\t\tCPU-only AVG POWER\t=\t%.6E Watts\n",
				pre, X->NODE_ID,
				X->energyinfo.CPUEtot/
				(X->ICLK*X->CYCLETIME));
		}
	}

		mlog(E, S, "%sNode%d\t\t\"ICLK\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->ICLK);
		mlog(E, S, "%sNode%d\t\t\"CLK\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->CLK);
		mlog(E, S, "%sNode%d\t\t\"TIME\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->TIME);
		mlog(E, S, "%sNode%d\t\t\"dyncnt\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->dyncnt);

	/* Abbreviate to reduce size of dist logs */
	if (0)
	{
		mlog(E, S, "%sNode%d\t\t\"VDD\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->VDD);
		mlog(E, S, "%sNode%d\t\t\"LOWVDD\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->LOWVDD);
		mlog(E, S, "%sNode%d\t\t\"SVDD\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->SVDD);
		mlog(E, S, "%sNode%d\t\t\"fail_prob\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->fail_prob);
		mlog(E, S, "%sNode%d\t\t\"failure_duration_max\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->failure_duration_max);
		mlog(E, S, "%sNode%d\t\t\"fail_clocks_left\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->fail_clocks_left);
		mlog(E, S, "%sNode%d\t\t\"got_correlated_failure\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->got_correlated_failure);
		mlog(E, S, "%sNode%d\t\t\"nfaults\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->nfaults);
		mlog(E, S, "%sNode%d\t\t\"faultthreshold\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->faultthreshold);
		mlog(E, S, "%sNode%d\t\t\"ENABLE_TOO_MANY_FAULTS\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->ENABLE_TOO_MANY_FAULTS);
		mlog(E, S, "%sNode%d\t\t\"clock_modulus\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->clock_modulus);
		mlog(E, S, "%sNode%d\t\t\"ENABLE_BATT_LOW_INTR\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->ENABLE_BATT_LOW_INTR);
		mlog(E, S, "%sNode%d\t\t\"battery_alert_frac\"\t=\t%E\n",
			pre, X->NODE_ID,
			X->battery_alert_frac);
		mlog(E, S, "%sNode%d\t\t\"rxok_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->rxok_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"txok_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->txok_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"addrerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->addrerr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"frameerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->frameerr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"collserr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->collserr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"csenseerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->csenseerr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"rxovrrunerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->rxovrrunerr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"txovrrunerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->txovrrunerr_intrenable_flag);
		mlog(E, S,"%sNode%d\t\t\"rxundrrunerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->rxundrrunerr_intrenable_flag);
		mlog(E, S,"%sNode%d\t\t\"txundrrunerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->txundrrunerr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"csumerr_intrenable_flag\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->csumerr_intrenable_flag);
		mlog(E, S, "%sNode%d\t\t\"nicintrQ->nqintrs\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->nicintrQ->nqintrs);
		mlog(E, S, "%sNode%d\t\t\"NIC_NUM_IFCS\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->NIC_NUM_IFCS);
		mlog(E, S, "%sNode%d\t\t\"NIC_INTR\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->NIC_INTR);
		mlog(E, S, "%sNode%d\t\t\"trace\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->trace);
		mlog(E, S, "%sNode%d\t\t\"runnable\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->runnable);
		mlog(E, S, "%sNode%d\t\t\"sleep\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->sleep);
		mlog(E, S, "%sNode%d\t\t\"ustart\"\t=\t" ULONGFMT "\n",
			pre, X->NODE_ID,
			X->ustart);
		mlog(E, S, "%sNode%d\t\t\"ufinish\"\t=\t" ULONGFMT "\n",
			pre, X->NODE_ID,
			S->ufinish);
		mlog(E, S, "%sNode%d\t\t\"startclk\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->startclk);
		mlog(E, S, "%sNode%d\t\t\"finishclk\"\t=\t" UVLONGFMT "\n",
			pre, X->NODE_ID,
			X->finishclk);
		mlog(E, S, "%sNode%d\t\t\"ENABLE_CLK_INTR\"\t=\t%d\n",
			pre, X->NODE_ID,
			X->superH->ENABLE_CLK_INTR);
		mlog(E, S, "%sNode%d\t\t\"xloc\"\t=\t%f\n",
			pre, X->NODE_ID,
			X->xloc);
		mlog(E, S, "%sNode%d\t\t\"yloc\"\t=\t%f\n",
			pre, X->NODE_ID,
			X->yloc);
		mlog(E, S, "%sNode%d\t\t\"zloc\"\t=\t%f\n",
			pre, X->NODE_ID,
			X->zloc);

		if (X->ICLK > 0)
		{
			mlog(E, S, "%sNode%d\t\tRatio of active/sleep cycles\t=\t%.6f\n",
				pre, X->NODE_ID,
				(float)X->CLK/(float)X->ICLK);
		}
		else
		{
			mlog(E, S, "%sNode%d\t\tRatio of active/sleep cycles\t=\t%.6f\n",
				pre, X->NODE_ID, 0.0);
		}

		for (j = 0; j < X->superH->NIC_NUM_IFCS; j++)
		{
			Ifc *ifcptr = &X->superH->NIC_IFCS[j];

			if (ifcptr->tx_fifo_maxoccupancy > maxoccupancy)
			{
				maxoccupancy = ifcptr->tx_fifo_maxoccupancy;
			}

			if (ifcptr->rx_fifo_maxoccupancy > maxoccupancy)
			{
				maxoccupancy = ifcptr->rx_fifo_maxoccupancy;
			}

			txok += ifcptr->IFC_CNTR_TXOK;
			rxok += ifcptr->IFC_CNTR_RXOK;
			addrerr += ifcptr->IFC_CNTR_ADDR_ERR;
			frmerr += ifcptr->IFC_CNTR_FRAME_ERR;
			collserr += ifcptr->IFC_CNTR_COLLS_ERR;
			csenseerr += ifcptr->IFC_CNTR_CSENSE_ERR;
			rxovrnerr += ifcptr->IFC_CNTR_RXOVRRUN_ERR;
			rxundrrunerr += ifcptr->IFC_CNTR_RXUNDRRUN_ERR;
			txovrnerr += ifcptr->IFC_CNTR_TXOVRRUN_ERR;
			txundrrunerr += ifcptr->IFC_CNTR_TXUNDRRUN_ERR;
		}

		mlog(E, S, "%sNode%d\t\tMax occupancy (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, maxoccupancy);
		mlog(E, S, "%sNode%d\t\tMax txok (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, txok);
		mlog(E, S, "%sNode%d\t\tMax rxok (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, rxok);
		mlog(E, S, "%sNode%d\t\tMax addrerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, addrerr);
		mlog(E, S, "%sNode%d\t\tMax frmerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, frmerr);
		mlog(E, S, "%sNode%d\t\tMax collserr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, collserr);
		mlog(E, S, "%sNode%d\t\tMax csenseerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, csenseerr);
		mlog(E, S, "%sNode%d\t\tMax rxovrnerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, rxovrnerr);
		mlog(E, S, "%sNode%d\t\tMax rxundrrunerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, rxundrrunerr);
		mlog(E, S, "%sNode%d\t\tMax txovrnerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, txovrnerr);
		mlog(E, S, "%sNode%d\t\tMax txundrrunerr (all TX/RX FIFOs)\t=\t%d\n",
			pre, X->NODE_ID, txundrrunerr);
		mlog(E, S, "%s\n", pre);

		for (j = 0; j < X->Nstack->count; j++)
		{
			mlog(E, S, "%s\n%-20s %s\n", "Name:",
				pre, X->Nstack->regions[j]->name);

			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "pcstart:",
				pre, X->Nstack->regions[j]->pcstart);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "frame offset:",
				pre, X->Nstack->regions[j]->frameoffset);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "size:",
				pre, X->Nstack->regions[j]->endaddr - X->Nstack->regions[j]->startaddr);

			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Local Read latency:",
				pre, X->Nstack->regions[j]->local_read_latency);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Local Write latency:",
				pre, X->Nstack->regions[j]->local_write_latency);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Remote Read latency:",
				pre, X->Nstack->regions[j]->remote_read_latency);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Remote Write latency:",
				pre, X->Nstack->regions[j]->remote_write_latency);

			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Map ID:",
				pre, X->Nstack->regions[j]->map_id);
			mlog(E, S, "%s%-20s " UHLONGFMT "\n", "Private:",
				pre, X->Nstack->regions[j]->private);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Map offset:",
				pre, X->Nstack->regions[j]->map_offset);

			mlog(E, S, "%s%-20s %d\n", "Read accesses:",
				pre, X->Nstack->regions[j]->nreads);
			mlog(E, S, "%s%-20s %d\n\n", "Write accesses:",
				pre, X->Nstack->regions[j]->nwrites);

			mlog(E, S, "%sValue History: ", pre);
			for (k = 0; k < X->Nstack->regions[j]->validx; k++)
			{
				mlog(E, S, "%s" ULONGFMT " ", pre, X->Nstack->regions[j]->values[k]);
			}
			mlog(E, S, "%s\n", pre);
		}

		for (j = 0; j < X->N->count; j++)
		{
			mlog(E, S, "%s\n%-20s %s\n", "Name:",
				pre, X->N->regions[j]->name);

			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Start address:",
				pre, X->N->regions[j]->startaddr);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "End address:",
				pre, X->N->regions[j]->endaddr);

			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Local Read latency:",
				pre, X->N->regions[j]->local_read_latency);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Local Write latency:",
				pre, X->N->regions[j]->local_write_latency);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Remote Read latency:",
				pre, X->N->regions[j]->remote_read_latency);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Remote Write latency:",
				pre, X->N->regions[j]->remote_write_latency);

			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Map ID:",
				pre, X->N->regions[j]->map_id);
			mlog(E, S, "%s%-20s " UHLONGFMT "\n", "Private:",
				pre, X->N->regions[j]->private);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", "Map offset:",
				pre, X->N->regions[j]->map_offset);

			mlog(E, S, "%s%-20s %d\n", "Read accesses:",
				pre, X->N->regions[j]->nreads);
			mlog(E, S, "%s%-20s %d\n\n", "Write accesses:",
				pre, X->N->regions[j]->nwrites);

			mlog(E, S, "%sValue History: ", pre);
			for (k = 0; k < X->N->regions[j]->validx; k++)
			{
				mlog(E, S, "%s" ULONGFMT " ", pre, X->N->regions[j]->values[k]);
			}
			mlog(E, S, "%s\n", pre);
		}

		for (j = 0; j < X->RT->count; j++)
		{
			mlog(E, S, "%s\n%-20s %s\n", pre, "Name:",
				X->RT->regvts[j]->name);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n", pre, "PCstart:",
				X->RT->regvts[j]->pcstart);
			mlog(E, S, "%s%-20s 0x" UHLONGFMT "\n",pre,  "Register:",
				X->RT->regvts[j]->regnum);

			mlog(E, S, "%sValue History: ", pre);
			for (k = 0; k < X->RT->regvts[j]->validx; k++)
			{
				mlog(E, S, "%s" ULONGFMT " ", pre, X->RT->regvts[j]->values[k]);
			}
			mlog(E, S, "%s\n", pre);
		}
	}

	mlog(E, S, "%s} Tag %s.\n", pre, tag);
	mclose(S->logfd);

	return;
}

void
m_dumpall(Engine *E, char *filename, int mode, char *tag, char *pre)
{
	int	i;
	State	tmp, *S;


	S = &tmp;
	S->logfd = mcreate(filename, mode);
	if (S->logfd <= 2)
	{
		mprint(E, NULL, siminfo,
			"Could not open [%s] for writing, create returned fd \"%d\"\n",
			filename, S->logfd);
		return;
	}

	mlog(E, S, "\n\n%sTag %s{\n", pre, tag);
	mlog(E, S, "%sRandom seed used = [%ld]\n", pre, E->randseed);

	for (i = 0; i < E->nnodes; i++)
	{
		/*	Do not truncate inner writes to log	*/
		m_dumpnode(E, i, filename, M_OWRITE, tag, pre);
	}

	for (i = 0; i < E->nbatts; i++)
	{
		mlog(E, S, "%s\tBATT%d\t\tNumber of nodes attached\t=\t%d\n",
				pre, E->batts[i].ID,
				E->batts[i].num_attached);
		mlog(E, S, "%s\tBATT%d\t\tEstimated Ideal Capacity\t=\t%E (Joules)\n",
				pre, E->batts[i].ID,
				E->batts[i].battery_capacity);
		mlog(E, S, "%s\tBATT%d\t\tEstimated Ideal remaining (Joules)\t=\t%E\n",
				pre, E->batts[i].ID,
				E->batts[i].battery_remaining);
		mlog(E, S, "%s\tBATT%d\t\tMaximum Sampled Current Load (mA)\t=\t%E\n",
				pre, E->batts[i].ID,
				E->batts[i].maxIload*1000);
		mlog(E, S, "%s\tBATT%d\t\tavgIload             %E\n",
				pre, E->batts[i].ID,
				E->batts[i].avgIload);
		mlog(E, S, "%s\tBATT%d\t\tnsamplesIload        %E\n",
				pre, E->batts[i].ID,
				E->batts[i].nsamplesIload);
		mlog(E, S, "%s\tE->battperiodpsec="UVLONGFMT"\n", pre, E->battperiodpsec);
		mlog(E, S, "%s\n", pre);
	}
	mlog(E, S, "%s} Tag %s.\n", pre, tag);
	mclose(S->logfd);

	return;
}

void
m_pcbacktrace(Engine *E, State *S)
{
	int	i;

	mprint(E, S, nodeinfo, "\n");
	for (i = 0; i < S->pcstackheight; i++)
	{
		mprint(E, S, nodeinfo, "%4d:\tPC = [0x" UH8LONGFMT "], Frame Pointer (R14) = [0x" UH8LONGFMT "]\n",
			i, S->PCSTACK[i], S->FPSTACK[i]);
	}
	mprint(E, S, nodeinfo, "\n");

	return;
}

int
m_decodehwstructname(char *name)
{
	/*									*/
	/*	We just need a canonical ordering of all hardware structure	*/
	/*	names, OK if we mix all architectures in here.  This function	*/
	/*	is used in SEE modeling.  The simulator variables representing	*/
	/*	HW structures may also be in the rvar modeling, and the		*/
	/*	ordering of sim vars is decoded by m_decodesimrvarname()	*/
	/*									*/
	return -1;
}

int
m_decodesimrvarname(char *name)
{
	/*									*/
	/*	A canonical ordering of all simulator variables that may	*/
	/*	be treated as random variables.					*/
	/*									*/
	return -1;
}

int
getbptidx(Engine *E)
{
	int	i;

	for (i = 0; i < MAX_BREAKPOINTS; i++)
	{
		if (!E->bpts[i].valid)
			return i;
	}

	return -1;
}

void
m_setbptglobaltime(Engine *E, Picosec t)
{
	int	idx;

	if ((idx = getbptidx(E)) < 0)
	{
		mprint(E, NULL, siminfo, "Maximum number of breakpoints reached...");
		return;
	}

	E->bpts[idx].type	= BPT_GLOBALTIME;
	E->bpts[idx].globaltime	= t;
	E->bpts[idx].valid	= 1;

	E->validbpts[E->nvalidbpts++] = idx;
}

void
m_setbptcycles(Engine *E, State *S, uvlong n)
{
	int	idx;

	if ((idx = getbptidx(E)) < 0)
	{
		mprint(E, NULL, siminfo, "Maximum number of breakpoints reached...");
		return;
	}

	E->bpts[idx].type		= BPT_CYCLES;
	E->bpts[idx].cyclesbpt.nodeid	= S->NODE_ID;
	E->bpts[idx].cyclesbpt.cycles	= n;
	E->bpts[idx].valid		= 1;

	E->validbpts[E->nvalidbpts++] = idx;
}

void
m_setbptinstrs(Engine *E, State *S, uvlong n)
{
	int	idx;

	if ((idx = getbptidx(E)) < 0)
	{
		mprint(E, NULL, siminfo, "Maximum number of breakpoints reached...");
		return;
	}

	E->bpts[idx].type		= BPT_INSTRS;
	E->bpts[idx].instrsbpt.nodeid	= S->NODE_ID;
	E->bpts[idx].instrsbpt.dyncnt	= n;
	E->bpts[idx].valid		= 1;

	E->validbpts[E->nvalidbpts++] = idx;
}

void
m_setbptsensorreading(Engine *E, State *S, int whichsensor, double val)
{
	int	idx;

	if ((idx = getbptidx(E)) < 0)
	{
		mprint(E, NULL, siminfo, "Maximum number of breakpoints reached...");
		return;
	}

	if (whichsensor >= MAX_NODE_SENSORS)
	{
		mprint(E, NULL, siminfo, "Invalid node sensor index...");
		return;
	}

	E->bpts[idx].type			= BPT_SENSORREADING;
	E->bpts[idx].sensorreadingbpt.nodeid	= S->NODE_ID;
	E->bpts[idx].sensorreadingbpt.sensor	= whichsensor;
	E->bpts[idx].sensorreadingbpt.value	= val;
	E->bpts[idx].valid			= 1;

	E->validbpts[E->nvalidbpts++] = idx;
}

void
printbpt(Engine *E, Breakpoint *b)
{
	switch (b->type)
	{
		case BPT_GLOBALTIME:
		{
			mprint(E, NULL, siminfo, "Global time breakpoint\t@\t" DBLFMT " seconds\n", b->globaltime);
			break;
		}

		case BPT_CYCLES:
		{
			State	*tmp = E->sp[b->cyclesbpt.nodeid];

			mprint(E, tmp, nodeinfo, "Node %d cycles breakpoint\t@\t" UVLONGFMT " cycles\n",
				b->cyclesbpt.nodeid, b->cyclesbpt.cycles);
			break;
		}

		case BPT_INSTRS:
		{
			State	*tmp = E->sp[b->cyclesbpt.nodeid];

			mprint(E, tmp, nodeinfo, "Node %d instrs. breakpoint\t@\t" UVLONGFMT " instrs.\n",
				b->instrsbpt.nodeid, b->instrsbpt.dyncnt);
			break;
		}

		case BPT_SENSORREADING:
		{
			State	*tmp = E->sp[b->cyclesbpt.nodeid];

			mprint(E, tmp, nodeinfo, "Node %d sensor %d  value breakpoint\t@\tvalue=" DBLFMT "\n",
					b->sensorreadingbpt.nodeid, b->sensorreadingbpt.sensor, b->sensorreadingbpt.value);
			break;
		}

		default:
			merror(E, "Sanity check failed on a registered breakpoint...");
	}
}

void
m_bptls(Engine *E)
{
	int	i;

	mprint(E, NULL, siminfo, "\n");
	for (i = 0; i < E->nvalidbpts; i++)
	{
		mprint(E, NULL, siminfo, "\t%d\t", E->validbpts[i]);
		printbpt(E, &E->bpts[E->validbpts[i]]);
	}
	mprint(E, NULL, siminfo, "\n");

	return;
}

void
m_bptdel(Engine *E, int which)
{
	int	i;

	if ((which >= MAX_BREAKPOINTS) || (which < 0) || !E->bpts[which].valid)
	{
		mprint(E, NULL, siminfo, "Invalid breakpoints specified...");
		return;
	}

	E->bpts[which].valid = 0;

	for (i = 0; i < E->nvalidbpts; i++)
	{
		if (E->validbpts[i] == which)
		{
			E->validbpts[i] = E->validbpts[E->nvalidbpts];
			E->nvalidbpts--;

			break;
		}
	}

	return;
}

void
m_setloc(Engine *E, State *S, double x, double y, double z)
{
	S->xloc = x;
	S->yloc = y;
	S->zloc = z;

	m_locstats(E, S);

	return;
}

void
m_locstats(Engine *E, State *S)
{
	mprint(E, NULL, siminfo, "Location  = [%E][%E][%E]\n", S->xloc, S->yloc, S->zloc);
}

tuck void
bpts_feed(Engine *E)
{
	int		i;
	Breakpoint	*b;
	State		*tmp;


	for (i = 0; i < E->nvalidbpts; i++)
	{
		b = &E->bpts[E->validbpts[i]];
		switch (b->type)
		{
			case BPT_GLOBALTIME:
			{
				if (E->globaltimepsec >= b->globaltime)
				{
					mprint(E, NULL, siminfo, "\n\nGlobal breakpoint hit:\n\t%d\t", E->validbpts[i]);
					printbpt(E, &E->bpts[E->validbpts[i]]);
					E->on = 0;
				}

				break;
			}

			case BPT_CYCLES:
			{
				tmp = E->sp[b->cyclesbpt.nodeid];

				if (tmp->runnable && tmp->ICLK >= b->cyclesbpt.cycles)
				{
					mprint(E, tmp, nodeinfo, "\n\nNode %d breakpoint hit:\n\t%d\t", b->cyclesbpt.nodeid, E->validbpts[i]);
					printbpt(E, &E->bpts[E->validbpts[i]]);
					E->on = 0;
				}

				break;
			}

			case BPT_INSTRS:
			{
				tmp = E->sp[b->instrsbpt.nodeid];

				if (tmp->runnable && tmp->dyncnt >= b->instrsbpt.dyncnt)
				{
					mprint(E, tmp, nodeinfo, "\n\nNode %d breakpoint hit:\n\t%d\t", b->instrsbpt.nodeid, E->validbpts[i]);
					printbpt(E, &E->bpts[E->validbpts[i]]);
					E->on = 0;
				}

				break;
			}

			case BPT_SENSORREADING:
			{
				tmp = E->sp[b->instrsbpt.nodeid];

				if (tmp->sensors[b->sensorreadingbpt.sensor].reading >= b->sensorreadingbpt.value)
				{
					mprint(E, tmp, nodeinfo, "\n\nNode %d breakpoint hit:\n\t%d\t", b->instrsbpt.nodeid, E->validbpts[i]);
					printbpt(E, &E->bpts[E->validbpts[i]]);
					E->on = 0;
				}

				break;
			}

			default:
				merror(E, "Sanity check failed on a registered breakpoint...");
		}
	}
};
