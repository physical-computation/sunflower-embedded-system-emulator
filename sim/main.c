/*										*/
/*		Copyright (C) 1999-2006 Phillip Stanley-Marbell.		*/
/*										*/
/*	You may redistribute and/or copy the source and binary			*/
/*	versions of this software provided:					*/
/*										*/
/*	1.	The source retains the copyright notice above, 			*/
/*		this list of conditions, and the following 			*/
/*		disclaimer.							*/
/*										*/
/*	2.	Redistributed binaries must reproduce the above			*/
/*		copyright notice, this list of conditions, and			*/
/*		the following disclaimer.					*/
/*										*/
/*	3.	The source and binaries are used for non-commercial 		*/
/*		purposes only.							*/
/*										*/
/*	4.	Permission is granted to all others, to redistribute		*/
/*		or make copies of the source or binaries, either		*/
/*		as it was received or with modifications, provided		*/
/*		the above three conditions are enforced and 			*/
/*		preserved, and that permission is granted for 			*/
/*		further redistribution, under the conditions of this		*/
/*		document.							*/
/*										*/
/*	The source is provided "as is", and no warranty is either		*/
/*	implied or explicitly granted. The authors will not be liable 		*/
/*	for any damages arising in any way out of the use or misuse 		*/
/*	of this software, even if advised of the possibility of such 		*/
/*	damage.									*/
/*										*/
/*	Contact: phillip Stanley-Marbell <pstanley@ece.cmu.edu>			*/
/*										*/	
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
#include "mmalloc.h"
#include "opstr-hitachi-sh.h"

Input 		istream;
Labels		labellist;

SuperHDCEntry	superHDC[1<<16];
MSP430DCEntry	msp430DC[1<<16];


/*	Do not spawn new thread on 'ON' command			*/
int		SIM_NODETACH;


/*	Misc state		*/
ulong		SIM_THROTTLE_NANOSEC = 0;
ulong		SIM_THROTTLE_WINDOW = 10000;
double		SIM_SCALE_ALPHA = 2.0;
double		SIM_SCALE_K = 1.0;
double		SIM_SCALE_VT = 0.7;
int		SIM_QUANTUM = 1;
int		SIM_BASENODEID = 0;
State		*SIM_STATE_PTRS[MAX_SIMNODES];
State 		*CUR_STATE;
int		CUR_NODE_ID = 0;
int		SCANNING = 0;
int		SIM_NUM_NODES = 0;
int		SIM_ON = 0;
int		SIM_IGNORE_NODEDEATHS = 0;
jmp_buf		SIM_JMP_BUF;
Machinetype	SIM_MACHINE_TYPE = MACHINE_SUPERH;
char            SIM_INFO_BUF[MAX_SIM_INFO_BUFSZ];
int             SIM_INFO_H2O = 0;
double		SIM_GLOBAL_TIME = 0.0;
double		SIM_MIN_CYCLETIME = DBL_MAX;
double		SIM_MAX_CYCLETIME = 0.0;
double		SIM_BATT_LEAK_CURRENT = 1E-6;


/*	Failure			*/
double		SIM_FAULT_PERIOD = 0.0;
double		SIM_FAULT_LASTACTIVATE = 0.0;

/*	Network			*/
Netsegment	SIM_NET_SEGMENTS[MAX_NETSEGMENTS];
int		SIM_NUM_NET_SEGMENTS = 0;
int		SIM_ACTIVE_NETSEGS[MAX_NETSEGMENTS];
int		SIM_NUM_ACTIVE_NETSEGS = 0;
uvlong		SIM_NIC_BYTES_SENT = 0;
char          	SIM_NETIO_BUF[MAX_NETIO_NBUFS][MAX_SEGBUF_TEXT];
int           	SIM_NETIO_H2O = 0;

/*								*/
/*	At 8 Mb/s, each byte is transferred in 1 us, thus 	*/
/*	period of < 1us is OK for simulating network speeds	*/
/*	which are < 8 Mb/s. The period is set automatically	*/
/*	in network.c, based on the min netseg speed (that's	*/
/*	why we initialize to DBL_MAX).				*/
/*								*/
double		SIM_NETWORK_PERIOD = DBL_MAX;
double		SIM_NETWORK_LASTACTIVATE = 0.0;

/*	Physical Phenomena	*/
Signalsrc	SIM_SIGSRCS[MAX_SIGNAL_SRCS];
int		SIM_NUM_SIGSRCS = 0;
double		SIM_PHYSICS_LASTACTIVATE;
double		SIM_PHYSICS_PERIOD = 100E-6;


/*	Power/Batt.		*/
Batt		SIM_BATTERIES[MAX_BATTERIES];
Batt*		SIM_ACTIVE_BATTERIES[MAX_BATTERIES];
int		CUR_BATT_ID = 0;
int		SIM_NUM_ACTIVE_BATTERIES = 0;
int		SIM_NUM_DEPLETED_BATTERIES = 0;
int		SIM_NUM_BATTERIES = 0;
int		SIM_VERBOSE = 1;
double		SIM_BATTFEED_LASTACTIVATE = 0.0;
double		SIM_BATTFEED_PERIOD = 1E-3;
double		SIM_DUMPPWR_LASTACTIVATE = 0.0;
double		SIM_DUMPPWR_PERIOD = 1E-3;

extern int	yyparse(void);
static void	spinbaton(int);
static void	do_numaregion(State *S, char *name, ulong start, 
				ulong end, long xlrlat, long xlwlat, long xrrlat,
				long xrwlat, int mapid, ulong offset, int private,
				int valuetrace, int onstack, ulong pcstart,
				int frameoffset, int ispointer);


tuck void
spinbaton(int pos)
{
	char		*batons[] = {"*", "X", "-", "\\", "|", "/"};

	if (pos == 0)
	{
		mprint(NULL, siminfo, "[*]");
	}
	else
	{
		mprint(NULL, siminfo, "\b\b%s]", batons[pos % 4]);
	}
}

#ifndef LIBSF
int
main(int nargs, char *args[])
{
	char 		*buf = NULL;
	State		*S;
	int		argn;

	
	SIM_VERBOSE = 1;
	marchinit();	
	m_version();
	superHnewstate(0, 0, 0, 0, 0);
	S = SIM_STATE_PTRS[0];

	m_init();

        buf = (char *) mmalloc(sizeof(char)*(MAX_BUFLEN+1),
			"(char *)buf for command interface in main.c");
        if (buf == NULL)
        {
                fprintf(stderr, "Malloc failed in main.c for \"buf\"");
		mexit("See above messages.", -1);
        }

	argn = 1;
	while (argn < nargs)
	{
		loadcmds(args[argn++]);
	}

	fprintf(stderr, "\n");
  	fprintf(stderr, "[ID=%d of %d][PC=0x" UHLONGFMT "][%.1EV, %.1EMHz] ",
		CUR_STATE->NODE_ID, SIM_NUM_NODES, CUR_STATE->PC,
		CUR_STATE->VDD, (1/CUR_STATE->CYCLETIME)/1E6);
	while (1)
	{
  		fgets(buf, MAX_BUFLEN, stdin);
		if (strlen(buf) > 0)
		{
			mstatelock();
			munchinput(buf);
			yyparse();
  			fprintf(stderr, "[ID=%d of %d][PC=0x" UHLONGFMT "][%.1EV, %.1EMHz] ",
				CUR_STATE->NODE_ID, SIM_NUM_NODES, CUR_STATE->PC,
				CUR_STATE->VDD, (1/CUR_STATE->CYCLETIME)/1E6);
			mstateunlock();

			buf[0] = '\0';
		}
  	}

	return 0;
}
#endif


tuck void
sched_step(void)
{
	State	*S;
	double	net_secsleft, fault_secsleft, physics_secsleft, batt_secsleft;
	double	min_secsleft;
	double	max_cputime = 0.0;
	ulong	throttle_tripctr = 0;


	mstatelock();
	S = SIM_STATE_PTRS[CUR_NODE_ID];
	min_secsleft = DBL_MAX;

	if (SF_NETWORK)
	{
		if  (eventready(SIM_GLOBAL_TIME, SIM_NETWORK_LASTACTIVATE, SIM_NETWORK_PERIOD))
		{
			network_clock();
			SIM_NETWORK_LASTACTIVATE = SIM_GLOBAL_TIME;
		}

		net_secsleft = SIM_NETWORK_PERIOD -
					(SIM_GLOBAL_TIME - SIM_NETWORK_LASTACTIVATE);
		min_secsleft = min(min_secsleft, net_secsleft);
	}

	if (SF_FAULT)
	{
		if (eventready(SIM_GLOBAL_TIME, SIM_FAULT_LASTACTIVATE, SIM_FAULT_PERIOD))
		{
			fault_feed();
			SIM_FAULT_LASTACTIVATE = SIM_GLOBAL_TIME;
		}

		fault_secsleft = SIM_FAULT_PERIOD -
					(SIM_GLOBAL_TIME - SIM_FAULT_LASTACTIVATE);
		min_secsleft = min(min_secsleft, fault_secsleft);
	}

	if (SF_PHYSICS)
	{
		if (eventready(SIM_GLOBAL_TIME, SIM_PHYSICS_LASTACTIVATE, SIM_PHYSICS_PERIOD))
		{
			physics_feed();
			SIM_PHYSICS_LASTACTIVATE = SIM_GLOBAL_TIME;
		}

		physics_secsleft = SIM_PHYSICS_PERIOD -
					(SIM_GLOBAL_TIME - SIM_PHYSICS_LASTACTIVATE);
		min_secsleft = min(min_secsleft, physics_secsleft);
	}

	/*							*/
	/*	Battery and DC-DC converter. Feed every 	*/
	/*	SIM_BATTFEED_GRANULARITY. Must sample often 	*/
	/*	enough to catch short bursts of current draw.	*/		
	/*							*/
	/*	We are using the current from the previous	*/
	/*	cycle (or SIM_QUANTUM # of cycles), which will	*/
	/*	get reset before the node is simulated again	*/
	/*							*/
	if (SF_BATT)
	{
		/*	Battery feed with arg > 0 will force it to
			use the supplied value and disregard values
			gotten from battery model for current
		*/
		if (eventready(SIM_GLOBAL_TIME, SIM_BATTFEED_LASTACTIVATE, SIM_BATTFEED_PERIOD))
		{
			battery_feed(-1);

			if (SF_BATTLOG)
			{
				battery_dumpall(SIM_STATE_PTRS[0]);
			}

			SIM_BATTFEED_LASTACTIVATE = SIM_GLOBAL_TIME;
		}

		batt_secsleft = SIM_BATTFEED_PERIOD -
					(SIM_GLOBAL_TIME - SIM_BATTFEED_LASTACTIVATE);
		min_secsleft = min(min_secsleft, batt_secsleft);
	}

	/*									*/
	/*	Auto quantum is not a perfect solution: E.g., in a case		*/
	/*	where two nodes attempt to transmit on the same link in		*/
	/*	a given auto quantum window, say 400 cycles. If, in the 	*/
	/*	cycle-by-cycle simulation, first node trnsmits on link in	*/
	/*	cycle 0 (i.e., seg_enqueue() at cycle 0), and second node	*/
	/*	attempts to transmit at cycle 200, then the later node will	*/
	/*	incur a collision and will retry at 200+frame delay. In the	*/
	/*	case of auto quantum, the later node will incur the collision	*/
	/*	at cycle 0 (rather than 200), and its retry will happen at 	*/
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
		SIM_QUANTUM = (int)(min_secsleft / SIM_MAX_CYCLETIME) - 1;
		SIM_QUANTUM = max(SIM_QUANTUM, 1);
	}

	for (CUR_NODE_ID = 0; CUR_NODE_ID < SIM_NUM_NODES; CUR_NODE_ID++)
	{
		S = SIM_STATE_PTRS[CUR_NODE_ID];
		S->E.current_draw = 0;

		if (!S->runnable)
		{
			continue;
		}

		if (SF_BATT && S->ENABLE_BATT_LOW_INTR && S->check_batt_intr(S))
		{
			S->take_batt_intr(S);
			S->sleep = 0;
		}

		if (SF_BATT && (S->BATT) && ((Batt *)S->BATT)->dead)
		{
			S->runnable = 0;
			m_dumpall("sunflower.out.tmp");

			continue;
		}


		S->step(S, 0);


		if (SF_DUMPPWR
			&& eventready(SIM_GLOBAL_TIME, SIM_DUMPPWR_LASTACTIVATE,
				SIM_DUMPPWR_PERIOD)
			&& S->runnable)
		{
			mlog(S, "%E %E",
				SIM_GLOBAL_TIME, S->E.current_draw * S->VDD);
			SIM_DUMPPWR_LASTACTIVATE = SIM_GLOBAL_TIME;
		}

		max_cputime = max(max_cputime, S->TIME);
	}

	/*									*/
	/*	Due to floating point inexact values, to ensure the check	*/
	/*	in eventready works correctly, we need to advance time by	*/
	/*	more than DBL_EPSILON. This is really just putting the		*/
	/*	carrot a bit farther ahead, for simulation to catch up to,	*/
	/*	and does not affect correctness: The SIM_GLOBAL_TIME is		*/
	/*	always one MIN_CYCLE ahead, to pull all events.	It is more	*/
	/*	or less a "next_tick".						*/
	/*									*/
	SIM_GLOBAL_TIME = max(SIM_GLOBAL_TIME, max_cputime) + SIM_MIN_CYCLETIME;

	/*	    Throttling is only approximate if doing autoquantum:	*/
	if (SIM_THROTTLE_NANOSEC > 0)
	{
		throttle_tripctr += SIM_QUANTUM;

		if (throttle_tripctr >= SIM_THROTTLE_WINDOW)
		{
			mnsleep(SIM_THROTTLE_NANOSEC*SIM_THROTTLE_WINDOW);
			throttle_tripctr = 0;
		}
	}
	mstateunlock();
}

void
scheduler(void *unused)
{
	int	jmpval;


	USED(unused);

	/*								*/
	/*	Scheduler is spawned as a new proc; This does any	*/
	/*	architecture specific actions, like FPcontrol in	*/
	/*	Inferno or change the priority of the scheduler proc	*/
	/*								*/
	marchinit();

	if (!(jmpval = setjmp(SIM_JMP_BUF)))
	{
		/*	Returning from initial call	*/
	}
	else
	{
		/*	Returning from longjmp()	*/
		/*	jmpval == node that barfed.	*/
	}

	while (SIM_ON)
	{
		sched_step();
	}

	return;
}

void
load_srec(State *S, char *filename)
{
	char	*filebuf, buf[MAX_SREC_LINELEN];
	int 	filesize, fpos, fd, i, n, nrecs = 0, batonpos = 0;
	ulong	rec_addr;
	int	pcset, rec_type, rec_length;
	char	*line;


	if ((fd = mopen(filename, M_OREAD)) < 0)
	{
		mprint(S, nodeinfo,
			"Open of \"%s\" failed...\n\n", filename);
		return;
	}

	if ((filesize = mfsize(fd)) < 0)
	{
		mprint(S, nodeinfo,
			"Determining size of \"%s\" failed...\n\n", filename);
		return;
	}

	filebuf = (char *)mcalloc(filesize, sizeof(char), "mcalloc for filebuf in main.c");
	if (filebuf == NULL)
	{
		mprint(S, nodeinfo,
			"Could not allocate memory for filebuf in main.c");
		return;
	}
	
	if ((n = mread(fd, filebuf, filesize)) != filesize)
	{
		mprint(S, nodeinfo,
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
			merror("Absurdly long SREC line !");
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
			/*	TODO:    We do not verify checksum on SREC records	*/
			/*								*/
			case 3:
			{
				/*	Data record with 32bit addr	*/
				int	i;
				char	*tptr, tmp[8];
				
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
					mprint(S, nodeinfo,
						"Loading S-RECORD to memory at address 0x" UHLONGFMT "\n",
						rec_addr);
					pcset = 1;
				}

#ifndef LIBSF
				spinbaton(batonpos);
				batonpos += !(nrecs++ % 256);
#else
				USED(nrecs);
				USED(batonpos);
				/*
				if (!(rec_addr % 128))
				{
					mprint(NULL, siminfo, ".");
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
					S->writebyte(S, rec_addr+i, strtoul(&tmp[0], nil, 16));
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
				/*		Unused 			*/	
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
				mprint(S, nodeinfo, "Seen unknown SRECORD type.\n");
				mprint(S, nodeinfo, "Aborting SRECL.\n");
				goto done;
			}	
		}
	}
done:
#ifdef LIBSF
	mprint(NULL, siminfo, "\nDone loading [%s].\n", filename);
#else
	mprint(NULL, siminfo, "\b\bM] Done.\n");
#endif
	mclose(fd);


	return;
}

void
m_setnode(int which)
{
	if (which >= SIM_NUM_NODES)
	{
		merror("Cannot set current node to [%d]: ID out of range.", which);
		return;
	}

	CUR_STATE = SIM_STATE_PTRS[which];

	return;
}

void
loadcmds(char *filename)
{
	char	*buf;
	int	fd;


	buf = (char *) mcalloc(MAX_BUFLEN+1, sizeof(char),
			"char *buf in loadcmds(), main.c");
	if (buf == NULL)
	{
		mprint(NULL, siminfo, "Could not allocate memory for char *buf in loadcmds()");
		mexit("See above messages.", -1);
	}

	fd = mopen(filename, M_OREAD);
	if (fd < 0)
	{
		mprint(NULL, siminfo, "Cannot open \"%s\" for loading...\n\n", filename);
		return;
	}

	/*									*/
	/*	NOTE:	Do not lock simulator state; our callers will.		*/
	/*									*/
	mprint(NULL, siminfo, "Loading %s...\n", filename);
	clearistream();
	while (mfgets(buf, MAX_BUFLEN, fd) != NULL)
	{
		munchinput(buf);
	}

	//streamchk();
	scan_labels_and_globalvars();
	//streamchk();
	yyparse();
	mclose(fd);


	return;
}

void
savemem(State *S, ulong start_addr, ulong end_addr, char *filename)
{
	int i, outfd;

      	if ((start_addr < S->MEMBASE) || (end_addr >= S->MEMEND))
	{
		mprint(S, nodeinfo, "Memory address out of range in SAVE command");
	}

	if ((outfd = mcreate(filename,  M_OWRITE|M_OTRUNCATE)) < 0)
        {
		mprint(S, nodeinfo,
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
sfatal(State *S, char *msg)
{
	int	do_jmp = SIM_ON;


	SIM_VERBOSE = 1;
	mprint(S, nodeinfo, "Sunflower FATAL (node %d) : <%s>\n",\
			S->NODE_ID, msg);

	S->fatalaction(S);
	S->dumpregs(S);
	S->dumpsysregs(S);
	
	S->runnable = 0;
	mprint(NULL, siminfo, "Stopping execution on node %d and pausing simulation...\n\n",
		S->NODE_ID);

//mexit("pip: exiting on Stop", 0);

	if (!SIM_IGNORE_NODEDEATHS)
	{
		mstateunlock();
		mkillscheduler();
	}

	/*								*/
	/*	Don't longjmp when, e.g., loading a file into mem, in	*/
	/*	which case we'd have entered sfatal w/ SIM_ON false.	*/
	/*								*/
	if (do_jmp)
	{
		longjmp(SIM_JMP_BUF, S->NODE_ID);
	}
}

void
go(State *S, int gocycles)
{
	while ((gocycles--) > 0)
	{
		S->step(S, 0);
	}

	return;
}

void
cont(State *S, ulong until_pc)
{
	/*	NOTE != as opposed to < etc.	*/
	while (S->PC != until_pc)
	{
		S->step(S, 0);
	}

	return;
}

void
help()
{
	int i;

	for (i = 0; i < nHelpstrs; i++)
	{
		mprint(NULL, siminfo, "%-24s", Helpstrs[i].cmd);

		if (((i != 0) && !((i+1) % 3)))
		{
			mprint(NULL, siminfo, "\n\t");
		}
	}

	if (i % 3)
	{
		mprint(NULL, siminfo, "\n");
	}

	mprint(NULL, siminfo, "There are %d commands and %d aliases:\n",
			uniqHelpstrs, nHelpstrs-uniqHelpstrs);
	mprint(NULL, siminfo,
		"Type \"man <command>\" for help on a particular command.\n");
	mprint(NULL, siminfo,
		"You can also type \"man <cmd>*\" to get a list of commands matching \"cmd\".\n");

	return;
}

void
man(char *cmd)
{
	int	i, n = 0;

	/*	Wildcard	*/
	if (cmd[strlen(cmd) - 1] == '*')
	{
		cmd[strlen(cmd) - 1] = '\0';
		mprint(NULL, siminfo, "Commands and aliases beginning with \"%s\":\n\n\t", cmd);

		for (i = 0; i < nHelpstrs; i++)
		{
			if (!strncmp(cmd, Helpstrs[i].cmd, strlen(cmd)))
			{
				mprint(NULL, siminfo, "%-24s", Helpstrs[i].cmd);

				if (((n != 0) && !((n+1) % 3)))
				{
					mprint(NULL, siminfo, "\n\t");
				}
				n++;
			}
		}
		
		if (n % 3)
		{
			mprint(NULL, siminfo, "\n");
		}
		mprint(NULL, siminfo, "\nType \"man <command>\" for help on a particular command.\n");
	
		return;
	}
	
	for (i = 0; i < nHelpstrs; i++)
	{
		if (!strcmp(cmd, Helpstrs[i].cmd))
		{
			mprint(NULL, siminfo, "\n\t");
			mprint(NULL, siminfo, "Command:\t%s", Helpstrs[i].cmd);
			mprint(NULL, siminfo, "\n");
			mprint(NULL, siminfo, "%s\n%s\n\n",\
				Helpstrs[i].intro, Helpstrs[i].args);
	
			return;
		}
	}	
	help();

	return;
}

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
                                        s = NULL;
                                else
                                        s[-1] = 0;
                                *stringptr = s;
                                return (tok);
                        }
                } while (sc != 0);
        }

	return NULL;
}

void
m_dumpmem(State *S, long start_addr, long end_addr)
{
	ulong i;

	if ((start_addr >= S->MEMBASE) && (end_addr < S->MEMEND))
	{
		for (i = start_addr; i <= end_addr; i++)
		{
			mprint(S, nodeinfo, "MEM[0x" UHLONGFMT "] = [0x%02X]\n",
				i, S->MEM[i-S->MEMBASE]);
		}
	}
	else
	{
		mprint(NULL, siminfo, "Memory address out of range in DUMPMEM command\n");
	}

	return;
}

void
m_init(void)
{
	int		i;

	/*	Initialize random number generators	*/
	mprint(NULL, siminfo,
			"Initialized random number generator with seed %d...\n",
			mrandominit()); 

	/*		Prime the decode cache		*/
	for (i = 0; i < (1 << 16); i++)
	{
		superHdecode((ushort)(i&0xFFFF), &superHDC[i].dc_p);
	}

	return;
}

void
m_version(void)
{
	mprint(NULL, siminfo,
		"\nSunflower %s\n", MVERSION);
	mprint(NULL, siminfo,
		"Authored, 1999-2005, by phillip stanley-marbell, <pstanley@ece.cmu.edu>\n");
	mprint(NULL, siminfo,
		"Public key fingerprint 0884 DE6E E1F6 A201 023C 2E9B 7F9F FD41 AB2A 4587\n");
	mprint(NULL, siminfo,
		"This software is provided with ");
	mprint(NULL, siminfo,
		"ABSOLUTELY NO WARRANTY. Read LICENSE.txt\n\n");		

	return;
}

void
m_newnode(char *type, double x, double y, double z, double orbit, double velocity)
{
	if (SIM_NUM_NODES >= MAX_SIMNODES)
	{
		mprint(NULL, siminfo, "Node creation limit reached. Sorry.");

		return;
	}
		
	/*  newnode xloc yloc zloc orbit velocity  */
	if ((strlen(type) == 0) || !strncmp(type, "superH", strlen("superH")))
	{
		superHnewstate(x, y, z, orbit, velocity);
	}
	else
	{
		merror("Machine type \"%s\" not supported.", type);
	}

	return;
}

void
m_powertotal(void)
{
	int	i;
	double	total_power=0.0, total_energy=0.0;

				
	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		total_power += SIM_STATE_PTRS[i]->E.CPUEtot;
		total_energy += SIM_STATE_PTRS[i]->E.CPUEtot/
			(SIM_STATE_PTRS[i]->ICLK * SIM_STATE_PTRS[i]->CYCLETIME);
	}

	mprint(NULL, siminfo, "Average power accross all nodes = %E\n",
		total_power/i);
	mprint(NULL, siminfo, "Total energy accross all nodes = %E\n",
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
m_regtracer(State *S, char *name, ulong pcstart, int regnum, int size, int ispointer)
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
		mprint(S, nodeinfo,
			"Maximum number of register tracers reached");
		return;
	}

	tmp = (Regvt *) mcalloc(1, sizeof(Regvt), "S->RT->regvts entry in main.c");
	if (tmp == NULL)
	{
		sfatal(S, "mcalloc failed for S->RT->regvts entry in main.c");
	}

	tmp->values = (ulong *) mcalloc(BASE_REGTRACER_VALUETRACE, sizeof(ulong), "S->RT->regvts->values entry in main.c");
	if (tmp->values == NULL)
	{
		sfatal(S, "mcalloc failed for S->RT->regvts->values entry in main.c");
	}
	tmp->nvalues = BASE_REGTRACER_VALUETRACE;

	tmp->pcstart = pcstart;
	tmp->regnum = regnum;
	tmp->size = size;
	tmp->ispointer = ispointer;
	strncpy(tmp->name, name, MAX_REGTRACER_NAMELEN);

	S->RT->regvts[S->RT->count] = tmp;
	S->RT->count++;

	qsort(S->RT->regvts, S->RT->count, sizeof(Regvt *), m_sort_regtracers_comp);

	return;
}

void
do_numaregion(State *S, char *name, ulong start, ulong end, long xlrlat,
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
		mprint(S, nodeinfo,
			"Maximum number of NUMA regions reached");
		return;
	}

	tmp = (Numaregion *) mcalloc(1, sizeof(Numaregion), "X->regions entry in main.c");
	if (tmp == NULL)
	{
		sfatal(S, "mcalloc failed for X->regions entry in main.c");
	}

	tmp->values = (ulong *) mcalloc(BASE_NUMAREGION_VALUETRACE, sizeof(ulong), "X->regions->values entry in main.c");
	if (tmp->values == NULL)
	{
		sfatal(S, "mcalloc failed for X->regions->values entry in main.c");
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
m_numaregion(char *name, ulong start, ulong end, long xlrlat,
	long xlwlat, long xrrlat, long xrwlat, int mapid, ulong offset, int private)
{
	int	i;

	/*								*/
	/*	Numa regions get installed on all nodes. The mapid	*/
	/*	and private flag determine whether it is public/priv	*/
	/*	and who the owner/home node is for private regions.	*/
	/*								*/
	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		State	*S = SIM_STATE_PTRS[i];

		do_numaregion(S, name, start, end, xlrlat, xlwlat, xrrlat,
			xrwlat, mapid, offset, private, 0, 0, 0, 0, 0);
	}

	return;
}

void
m_regtracerstats(State *S)
{
	int	i, j;

	for (i = 0; i < MAX_REG_TRACERS; i++)
	{
		if (S->RT->regvts[i] == NULL)
		{
			continue;
		}

		mprint(NULL, siminfo, "\n%-20s %s\n", "Name:", S->RT->regvts[i]->name);
		mprint(NULL, siminfo, "%-20s 0x" UHLONGFMT "\n", "PCstart:", S->RT->regvts[i]->pcstart);
		mprint(NULL, siminfo, "%-20s 0x" UHLONGFMT "\n", "Register:", S->RT->regvts[i]->regnum);
		mprint(NULL, siminfo, "Value History: ");

		for (j = 0; j < S->RT->regvts[i]->validx; j++)
		{
			mprint(NULL, siminfo, ULONGFMT " ", S->RT->regvts[i]->values[j]);
		}
		mprint(NULL, siminfo, "\n");
	}

	return;
}

void
m_numastats(State *S)
{
	int	i;

	for (i = 0; i < MAX_NUMA_REGIONS; i++)
	{
		if (S->N->regions[i] == NULL)
		{
			continue;
		}

		mprint(NULL, siminfo,
			"\n%-20s %s\n", "Name:", S->N->regions[i]->name);

		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Start address:", S->N->regions[i]->startaddr);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "End address:", S->N->regions[i]->endaddr);

		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Local Read latency:",
			S->N->regions[i]->local_read_latency);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Local Write latency:",
			S->N->regions[i]->local_write_latency);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Remote Read latency:",
			S->N->regions[i]->remote_read_latency);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Remote Write latency:",
			S->N->regions[i]->remote_write_latency);

		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Map ID:", S->N->regions[i]->map_id);
		mprint(NULL, siminfo,
			"%-20s " UHLONGFMT "\n", "Private:", S->N->regions[i]->private);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Map offset:", S->N->regions[i]->map_offset);

		mprint(NULL, siminfo,
			"%-20s %d\n", "Read accesses:", S->N->regions[i]->nreads);
		mprint(NULL, siminfo,
			"%-20s %d\n\n", "Write accesses:", S->N->regions[i]->nwrites);
	}

	return;
}

void
m_numastatsall(void)
{
	int	i;

	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		m_numastats(SIM_STATE_PTRS[i]);
	}

	return;
}

void
m_numasetmapid(int whichmap, int cpuid)
{
	int	i;

	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		if (whichmap >= SIM_STATE_PTRS[i]->N->count)
		{
			sfatal(SIM_STATE_PTRS[i],
				"Access to numa region out of range of valid regions");
		}

		if (cpuid >= SIM_NUM_NODES)
		{
			sfatal(SIM_STATE_PTRS[i],
				"Invalid cpuid argument to numasetmapid");
		}

		SIM_STATE_PTRS[i]->N->regions[whichmap]->map_id = cpuid;
	}

	/*	Lock bus for time prop to size of mapping	*/
	SIM_STATE_PTRS[cpuid]->superH->B->pbuslock = 1;
	SIM_STATE_PTRS[cpuid]->superH->B->pbuslocker = cpuid;

	//TODO: this is not the cleanest way to do this:
	SIM_STATE_PTRS[cpuid]->sleep = 0;
	SIM_STATE_PTRS[cpuid]->superH->P.MA.valid = 1;
	//Any local table as good as the other for size info. We use info from node0
	SIM_STATE_PTRS[cpuid]->superH->P.MA.cycles = 
		SIM_STATE_PTRS[0]->N->regions[whichmap]->endaddr -
		SIM_STATE_PTRS[0]->N->regions[whichmap]->startaddr;

	return;
}

void
m_addvaluetrace(State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer)
{
	do_numaregion(S, tag, addr, addr+size, -1, -1, -1, -1, S->NODE_ID, 0, 0, 1, onstack, pcstart, frameoffset, ispointer);
}

void
m_delvaluetrace(State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer)
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
			mlog(S, "\n%-20s %s\n", "Name:", S->Nstack->regions[i]->name);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "pcstart:", 
				S->Nstack->regions[i]->pcstart);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "frame offset:", 
				S->Nstack->regions[i]->frameoffset);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "size:", 
				S->Nstack->regions[i]->endaddr-S->Nstack->regions[i]->startaddr);
			mlog(S, "%-20s %d\n", "Read accesses:", S->Nstack->regions[i]->nreads);
			mlog(S, "%-20s %d\n\n", "Write accesses:", S->Nstack->regions[i]->nwrites);
			mlog(S, "Value History: ");
			for (j = 0; j < S->Nstack->regions[i]->validx; j++)
			{
				mlog(S, ULONGFMT " ", S->Nstack->regions[i]->values[j]);
			}
			mlog(S, "\n");

			mfree(S->Nstack->regions[i], "S->Nstack->regions in m_delvaluetrace");
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
			mlog(S, "\n%-20s %s\n", "Name:", S->N->regions[i]->name);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Start address:", 
				S->N->regions[i]->startaddr);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "End address:", 
				S->N->regions[i]->endaddr);
			mlog(S, "%-20s %d\n", "Read accesses:", S->N->regions[i]->nreads);
			mlog(S, "%-20s %d\n\n", "Write accesses:", S->N->regions[i]->nwrites);
			mlog(S, "Value History: ");
			for (j = 0; j < S->N->regions[i]->validx; j++)
			{
				mlog(S, ULONGFMT " ", S->N->regions[i]->values[j]);
			}
			mlog(S, "\n");

			mfree(S->N->regions[i], "S->N->regions in m_delvaluetrace");
			S->N->count--;
			S->N->regions[i] = S->N->regions[S->N->count];
			S->N->regions[S->N->count] = NULL;

			break;	
		}
	}

	return;
}

void
m_valuestats(State *S)
{
	int	i, j;

	for (i = 0; i < S->Nstack->count; i++)
	{
		if (S->Nstack->regions[i] == NULL || !S->Nstack->regions[i]->valuetrace)
		{
			continue;
		}

		mprint(NULL, siminfo,
			"\n%-20s %s\n", "Name:", S->Nstack->regions[i]->name);

		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "PCstart:", S->Nstack->regions[i]->pcstart);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Frame offset:", S->Nstack->regions[i]->frameoffset);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Size:", S->Nstack->regions[i]->endaddr-S->Nstack->regions[i]->startaddr);

		mprint(NULL, siminfo,
			"%-20s %d\n", "Read accesses:", S->Nstack->regions[i]->nreads);
		mprint(NULL, siminfo,
			"%-20s %d\n\n", "Write accesses:", S->Nstack->regions[i]->nwrites);

		mprint(NULL, siminfo, "Value History: ");

		for (j = 0; j < S->Nstack->regions[i]->validx; j++)
		{
			mprint(NULL, siminfo, ULONGFMT " ", S->Nstack->regions[i]->values[j]);
		}
		mprint(NULL, siminfo, "\n");
	}

	for (i = 0; i < S->N->count; i++)
	{
		if (S->N->regions[i] == NULL || !S->N->regions[i]->valuetrace)
		{
			continue;
		}

		mprint(NULL, siminfo,
			"\n%-20s %s\n", "Name:", S->N->regions[i]->name);


		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "Start address:", S->N->regions[i]->startaddr);
		mprint(NULL, siminfo,
			"%-20s 0x" UHLONGFMT "\n", "End address:", S->N->regions[i]->endaddr);

		mprint(NULL, siminfo,
			"%-20s %d\n", "Read accesses:", S->N->regions[i]->nreads);
		mprint(NULL, siminfo,
			"%-20s %d\n\n", "Write accesses:", S->N->regions[i]->nwrites);

		mprint(NULL, siminfo, "Value History: ");

		for (j = 0; j < S->N->regions[i]->validx; j++)
		{
			mprint(NULL, siminfo, ULONGFMT " ", S->N->regions[i]->values[j]);
		}
		mprint(NULL, siminfo, "\n");
	}

	m_regtracerstats(S);

	return;
}

void
m_parseobjdump(State *S, char *filename)
{
}

void
m_renumbernodes(void)
{
	int i;

	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		SIM_STATE_PTRS[i]->NODE_ID = i + SIM_BASENODEID;
	}

	return;
}

void
m_sizemem(State *S, int size)
{
	uchar *tmp;

	if (S->MEM == NULL)
	{
		S->MEM = (uchar *)mcalloc(1, size, "(uchar *)S->MEM");
		if (S->MEM == NULL)
		{
			mexit("Could not allocate mem for S->MEM in main.c", -1);
		}

		return;
	}

	tmp = (uchar *)mrealloc(S->MEM, size, "(uchar *)S->MEM");
	if (tmp == NULL)
	{
		mprint(S, nodeinfo,
			"SIZEMEM failed : could not allocate memory for %d bytes.\n", size);
		S->MEMSIZE = 0;
	}
	else
	{
		S->MEM = tmp;
		S->MEMSIZE = size;
		S->MEMEND = S->MEMBASE+S->MEMSIZE;
		mprint(S, nodeinfo,
			"Set memory size to %d Kilobytes\n", S->MEMSIZE/1024);
	}

	return;
}

void
m_run(State *S, char *args)
{
	/*							*/
	/*	We tokenize the args directly into the mem	*/
	/*	space of the simulated machine. The entire	*/
	/*	arg string ($2) is first placed into the 	*/
	/*	simulated machine's mem at location:		*/
	/*		ARGVOFFSET + strlen($2)			*/
	/*	below top of memory, so it does not get nuked	*/
	/*	before a main() is called, assuming 64K is 	*/
	/*	enough stack space during init, which I think	*/
	/*	more than suffices. The argv goes below it.	*/
	/*							*/
	int	argc, ARGVOFFSET = 65536, argstrlen, align;
	char	**tptr, **argv, *simstr;
	ulong	argvptroffset;
				

	argstrlen = strlen(args)+1;
	if ((ARGVOFFSET + argstrlen) > S->MEMSIZE)
	{
		mprint(NULL, siminfo, "Memory is too small, ARGV offset outside memory!\n");
		mprint(NULL, siminfo, "Sunflower Exiting (see above errors)...\n");
		mexit("See above messages.", -1);
	}

	/*							*/
	/*	Copy arg string into simulated machine's mem.	*/
	/*							*/
	simstr = &S->MEM[S->MEMSIZE - (ARGVOFFSET+argstrlen)];
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
	/*	stack (top of mem, as per mem.h), and a 	*/
	/*	pointer to this vector goes in R5. This 	*/
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

		/*		Reverse byte order 	*/
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

		mprint(S, nodeinfo, "args = [%s], argc = %d\n",
			args, argc);
		mprint(S, nodeinfo, "R4 = [0x%08lx], R5 = [0x%08lx]\n",
			S->superH->R[4], S->superH->R[5]);

		S->runnable = 1;
		mprint(S, nodeinfo, "Running...\n\n");
	}
	else
	{
		merror("This machine does not know how to \"run\".");
	}

	return;	
}

void
m_on(State *S)
{
	int	i;
	ulong	now;


	if (SIM_ON)
	{
		merror("Already ON!.");
		return;
	}

	now = musercputimeusecs();
	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		SIM_STATE_PTRS[i]->startclk = SIM_STATE_PTRS[i]->ICLK;
		SIM_STATE_PTRS[i]->ustart = now;

		SIM_STATE_PTRS[i]->trip_ustart = now;
		SIM_STATE_PTRS[i]->trip_startclk = SIM_STATE_PTRS[i]->ICLK;
	}

	SIM_ON = 1;
	if (SIM_NODETACH)
	{
		scheduler(nil);
	}
	else
	{
		if (mspawnscheduler() < 0)
		{
			sfatal(S, "Could not create thread in ON call");
		}
	}

	return;
}

void
m_off(State *S)
{
	if (SIM_ON && mkillscheduler())
	{
		mprint(NULL, siminfo, "Simulator already off/paused...\n\n");
		return;
	}

	S->ufinish = musercputimeusecs();
	S->finishclk = S->ICLK;

	mprint(NULL, siminfo, "Simulator Paused.\n\n");
	mprint(NULL, siminfo, "User Time elapsed = %.6f seconds.\n",
			((float)(S->ufinish - S->ustart)/1E6));
		
	if (SIM_QUANTUM == 1)
	{
		mprint(NULL, siminfo, "Simulated CPU Time elapsed = %.6E seconds.\n",
			S->TIME);
		mprint(NULL, siminfo, "Simulated Clock Cycles = " UVLONGFMT "\n",
			S->finishclk - S->startclk);
	}

	mprint(NULL, siminfo,
		"Instruction Simulation Rate on node %d = %.2f Cycles/Second.\n\n",
		S->NODE_ID, (((float)(S->finishclk - S->startclk))/(((float)
		(S->ufinish - S->ustart))/1E6)));

	SIM_ON = 0;


	return;
}

void
m_sharebus(State *S, int donorid)
{
	State	*R;

	if (donorid > SIM_NUM_NODES || donorid < 0)
	{
		mprint(NULL, siminfo,
			"Invalid Bus Donor (id %d)\n", donorid);
		return;
	}

	R = SIM_STATE_PTRS[donorid];
	if (R == NULL)
	{
		mprint(NULL, siminfo,
			"Bus Donor (id %d) has NULL state struct\n", donorid);
		return;
	}

	mfree(S->superH->B, "S->superH->B in m_sharebus");
	S->superH->B = R->superH->B;

	return;
}

void
m_dumpall(char *filename)
{
	int	i, j, k;
	State	tmp, *S;

				
	S = &tmp;
	S->ufinish = musercputimeusecs();

	S->logfd = mcreate(filename, M_OWRITE|M_OTRUNCATE);
	if (S->logfd <= 2)
	{
		mprint(NULL, siminfo,
			"Could not open [%s] for writing, create returned fd \"%d\"\n",
			filename, S->logfd);
		return;
	}

	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		int	txok = 0, rxok = 0, addrerr = 0, frmerr = 0,
			collserr = 0, csenseerr = 0, rxovrnerr = 0,
			rxundrrunerr = 0, txovrnerr = 0, txundrrunerr = 0,
			maxoccupancy = 0;
		State	*X = SIM_STATE_PTRS[i];

		mlog(S, "Node%d\t\tUser Time elapsed = %.6f seconds.\n",
			X->NODE_ID,
			((float)(S->ufinish - X->ustart)/1E6));

		if ((S->ufinish - X->ustart) > 0)
		{
			mlog(S,
				"Node%d\t\tInstruction Simulation Rate = %.2f Cycles/Second.\n",
				X->NODE_ID,
				(((float)(X->finishclk - X->startclk))/(((float)
				(S->ufinish - X->ustart))/1E6)));
		}
		else
		{
			mlog(S,
				"Node%d\t\tInstruction Simulation Rate = %.2f Cycles/Second.\n",
				X->NODE_ID, 0.0);
		}

		mlog(S, "Node%d\t\t\"machinetype\"\t=\t%d\n",
			X->NODE_ID,
			X->machinetype);
		mlog(S, "Node%d\t\t\"Cycletrans\"\t=\t%d\n",
			X->NODE_ID,
			X->Cycletrans);
		mlog(S, "Node%d\t\t\"CYCLETIME\"\t=\t%E\n",
			X->NODE_ID,
			X->CYCLETIME);
		mlog(S, "Node%d\t\tNTRANS\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->E.ntrans);
		mlog(S, "Node%d\t\tCPU-only ETOT\t=\t%.6E Joules\n",
			X->NODE_ID,
			X->E.CPUEtot);

		if (X->ICLK == 0 || X->CYCLETIME == 0)
		{
			mlog(S, "Node%d\t\tCPU-only AVG POWER\t=\t%.6E Watts\n",
				X->NODE_ID, 0.0);
		}
		else
		{
			mlog(S, "Node%d\t\tCPU-only AVG POWER\t=\t%.6E Watts\n",
				X->NODE_ID,
				X->E.CPUEtot/
				(X->ICLK*X->CYCLETIME));
		}

		mlog(S, "Node%d\t\t\"ICLK\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->ICLK);
		mlog(S, "Node%d\t\t\"CLK\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->CLK);
		mlog(S, "Node%d\t\t\"TIME\"\t=\t%E\n",
			X->NODE_ID,
			X->TIME);
		mlog(S, "Node%d\t\t\"dyncnt\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->dyncnt);
		mlog(S, "Node%d\t\t\"VDD\"\t=\t%E\n",
			X->NODE_ID,
			X->VDD);
		mlog(S, "Node%d\t\t\"LOWVDD\"\t=\t%E\n",
			X->NODE_ID,
			X->LOWVDD);
		mlog(S, "Node%d\t\t\"SVDD\"\t=\t%E\n",
			X->NODE_ID,
			X->SVDD);
		mlog(S, "Node%d\t\t\"fail_prob\"\t=\t%E\n",
			X->NODE_ID,
			X->fail_prob);
		mlog(S, "Node%d\t\t\"failure_duration_max\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->failure_duration_max);
		mlog(S, "Node%d\t\t\"fail_clocks_left\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->fail_clocks_left);
		mlog(S, "Node%d\t\t\"got_correlated_failure\"\t=\t%d\n",
			X->NODE_ID,
			X->got_correlated_failure);
		mlog(S, "Node%d\t\t\"nfaults\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->nfaults);
		mlog(S, "Node%d\t\t\"faultthreshold\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->faultthreshold);
		mlog(S, "Node%d\t\t\"ENABLE_TOO_MANY_FAULTS\"\t=\t%d\n",
			X->NODE_ID,
			X->ENABLE_TOO_MANY_FAULTS);
		mlog(S, "Node%d\t\t\"clock_modulus\"\t=\t%d\n",
			X->NODE_ID,
			X->clock_modulus);
		mlog(S, "Node%d\t\t\"ENABLE_BATT_LOW_INTR\"\t=\t%d\n",
			X->NODE_ID,
			X->ENABLE_BATT_LOW_INTR);
		mlog(S, "Node%d\t\t\"battery_alert_frac\"\t=\t%E\n",
			X->NODE_ID,
			X->battery_alert_frac);
		mlog(S, "Node%d\t\t\"rxok_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->rxok_intrenable_flag);
		mlog(S, "Node%d\t\t\"txok_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->txok_intrenable_flag);
		mlog(S, "Node%d\t\t\"addrerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->addrerr_intrenable_flag);
		mlog(S, "Node%d\t\t\"frameerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->frameerr_intrenable_flag);
		mlog(S, "Node%d\t\t\"collserr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->collserr_intrenable_flag);
		mlog(S, "Node%d\t\t\"csenseerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->csenseerr_intrenable_flag);
		mlog(S, "Node%d\t\t\"rxovrrunerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->rxovrrunerr_intrenable_flag);
		mlog(S, "Node%d\t\t\"txovrrunerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->txovrrunerr_intrenable_flag);
		mlog(S,"Node%d\t\t\"rxundrrunerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->rxundrrunerr_intrenable_flag);
		mlog(S,"Node%d\t\t\"txundrrunerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->txundrrunerr_intrenable_flag);
		mlog(S, "Node%d\t\t\"csumerr_intrenable_flag\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->csumerr_intrenable_flag);
		mlog(S, "Node%d\t\t\"nicintrQ->nqintrs\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->nicintrQ->nqintrs);
		mlog(S, "Node%d\t\t\"NIC_NUM_IFCS\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->NIC_NUM_IFCS);
		mlog(S, "Node%d\t\t\"NIC_INTR\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->NIC_INTR);
		mlog(S, "Node%d\t\t\"trace\"\t=\t%d\n",
			X->NODE_ID,
			X->trace);
		mlog(S, "Node%d\t\t\"runnable\"\t=\t%d\n",
			X->NODE_ID,
			X->runnable);
		mlog(S, "Node%d\t\t\"sleep\"\t=\t%d\n",
			X->NODE_ID,
			X->sleep);
		mlog(S, "Node%d\t\t\"ustart\"\t=\t" ULONGFMT "\n",
			X->NODE_ID,
			X->ustart);
		mlog(S, "Node%d\t\t\"ufinish\"\t=\t" ULONGFMT "\n",
			X->NODE_ID,
			S->ufinish);
		mlog(S, "Node%d\t\t\"startclk\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->startclk);
		mlog(S, "Node%d\t\t\"finishclk\"\t=\t" UVLONGFMT "\n",
			X->NODE_ID,
			X->finishclk);
		mlog(S, "Node%d\t\t\"ENABLE_CLK_INTR\"\t=\t%d\n",
			X->NODE_ID,
			X->superH->ENABLE_CLK_INTR);
		mlog(S, "Node%d\t\t\"orbit\"\t=\t%d\n",
			X->NODE_ID,
			X->orbit);
		mlog(S, "Node%d\t\t\"velocity\"\t=\t%d\n",
			X->NODE_ID,
			X->velocity);
		mlog(S, "Node%d\t\t\"xloc\"\t=\t%f\n",
			X->NODE_ID,
			X->xloc);
		mlog(S, "Node%d\t\t\"yloc\"\t=\t%f\n",
			X->NODE_ID,
			X->yloc);
		mlog(S, "Node%d\t\t\"zloc\"\t=\t%f\n",
			X->NODE_ID,
			X->zloc);

		if (X->ICLK > 0)
		{
			mlog(S, "Node%d\t\tRatio of active/sleep cycles\t=\t%.6f\n",
				X->NODE_ID,
				(float)X->CLK/(float)X->ICLK);
		}
		else
		{
			mlog(S, "Node%d\t\tRatio of active/sleep cycles\t=\t%.6f\n",
				X->NODE_ID, 0.0);
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

		mlog(S, "Node%d\t\tMax occupancy (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, maxoccupancy);
		mlog(S, "Node%d\t\tMax txok (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, txok);
		mlog(S, "Node%d\t\tMax rxok (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, rxok);
		mlog(S, "Node%d\t\tMax addrerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, addrerr);
		mlog(S, "Node%d\t\tMax frmerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, frmerr);
		mlog(S, "Node%d\t\tMax collserr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, collserr);
		mlog(S, "Node%d\t\tMax csenseerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, csenseerr);
		mlog(S, "Node%d\t\tMax rxovrnerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, rxovrnerr);
		mlog(S, "Node%d\t\tMax rxundrrunerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, rxundrrunerr);
		mlog(S, "Node%d\t\tMax txovrnerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, txovrnerr);
		mlog(S, "Node%d\t\tMax txundrrunerr (all TX/RX FIFOs)\t=\t%d\n",
			X->NODE_ID, txundrrunerr);
		mlog(S, "\n\n");

		for (j = 0; j < X->Nstack->count; j++)
		{
			mlog(S, "\n%-20s %s\n", "Name:",
				X->Nstack->regions[j]->name);

			mlog(S, "%-20s 0x" UHLONGFMT "\n", "pcstart:", 
				X->Nstack->regions[j]->pcstart);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "frame offset:", 
				X->Nstack->regions[j]->frameoffset);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "size:", 
				X->Nstack->regions[j]->endaddr - X->Nstack->regions[j]->startaddr);

			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Local Read latency:",
				X->Nstack->regions[j]->local_read_latency);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Local Write latency:",
				X->Nstack->regions[j]->local_write_latency);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Remote Read latency:",
				X->Nstack->regions[j]->remote_read_latency);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Remote Write latency:",
				X->Nstack->regions[j]->remote_write_latency);

			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Map ID:",
				X->Nstack->regions[j]->map_id);
			mlog(S, "%-20s " UHLONGFMT "\n", "Private:",
				X->Nstack->regions[j]->private);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Map offset:",
				X->Nstack->regions[j]->map_offset);

			mlog(S, "%-20s %d\n", "Read accesses:",
				X->Nstack->regions[j]->nreads);
			mlog(S, "%-20s %d\n\n", "Write accesses:",
				X->Nstack->regions[j]->nwrites);

			mlog(S, "Value History: ");
			for (k = 0; k < X->Nstack->regions[j]->validx; k++)
			{
				mlog(S, ULONGFMT " ", X->Nstack->regions[j]->values[k]);
			}
			mlog(S, "\n");
		}

		for (j = 0; j < X->N->count; j++)
		{
			mlog(S, "\n%-20s %s\n", "Name:",
				X->N->regions[j]->name);

			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Start address:",
				X->N->regions[j]->startaddr);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "End address:",
				X->N->regions[j]->endaddr);

			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Local Read latency:",
				X->N->regions[j]->local_read_latency);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Local Write latency:",
				X->N->regions[j]->local_write_latency);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Remote Read latency:",
				X->N->regions[j]->remote_read_latency);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Remote Write latency:",
				X->N->regions[j]->remote_write_latency);

			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Map ID:",
				X->N->regions[j]->map_id);
			mlog(S, "%-20s " UHLONGFMT "\n", "Private:",
				X->N->regions[j]->private);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Map offset:",
				X->N->regions[j]->map_offset);

			mlog(S, "%-20s %d\n", "Read accesses:",
				X->N->regions[j]->nreads);
			mlog(S, "%-20s %d\n\n", "Write accesses:",
				X->N->regions[j]->nwrites);

			mlog(S, "Value History: ");
			for (k = 0; k < X->N->regions[j]->validx; k++)
			{
				mlog(S, ULONGFMT " ", X->N->regions[j]->values[k]);
			}
			mlog(S, "\n");
		}

		for (j = 0; j < X->RT->count; j++)
		{
			mlog(S, "\n%-20s %s\n", "Name:",
				X->RT->regvts[j]->name);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "PCstart:",
				X->RT->regvts[j]->pcstart);
			mlog(S, "%-20s 0x" UHLONGFMT "\n", "Register:",
				X->RT->regvts[j]->regnum);

			mlog(S, "Value History: ");
			for (k = 0; k < X->RT->regvts[j]->validx; k++)
			{
				mlog(S, ULONGFMT " ", X->RT->regvts[j]->values[k]);
			}
			mlog(S, "\n");
		}
	}

	for (i = 0; i < SIM_NUM_BATTERIES; i++)
	{
		mlog(S, "\tBATT%d\t\tNumber of nodes attached\t=\t%d\n",
				SIM_BATTERIES[i].ID,
				SIM_BATTERIES[i].num_attached);
		mlog(S, "\tBATT%d\t\tEstimated Ideal Capacity\t=\t%E (Joules)\n",
				SIM_BATTERIES[i].ID,
				SIM_BATTERIES[i].battery_capacity);
		mlog(S, "\tBATT%d\t\tEstimated Ideal remaining (Joules)\t=\t%E\n",
				SIM_BATTERIES[i].ID,
				SIM_BATTERIES[i].battery_remaining);
		mlog(S, "\tBATT%d\t\tMaximum Sampled Current Load (mA)\t=\t%E\n",
				SIM_BATTERIES[i].ID,
				SIM_BATTERIES[i].maxIload*1000);
                mlog(S, "\tBATT%d\t\tavgIload             %E\n", 
				SIM_BATTERIES[i].ID,
                                SIM_BATTERIES[i].avgIload);
                mlog(S, "\tBATT%d\t\tnsamplesIload        %E\n", 
				SIM_BATTERIES[i].ID,
                                SIM_BATTERIES[i].nsamplesIload);
                mlog(S, "\tSIM_BATTFEED_PERIOD=%E\n", SIM_BATTFEED_PERIOD); 
		mlog(S, "\n\n");
	}

	return;
}
