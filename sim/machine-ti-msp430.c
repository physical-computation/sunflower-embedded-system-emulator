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

#include <string.h>
#include "sf.h"
#include "mextern.h"

void
msp430dumpregs(Engine *E, State *S)
{
	int i;

	for (i = 0; i < 16; i++)
	{
		mprint(E, S, nodeinfo, "R%-2d\t\t", i);
		mbitprint(E, S, 32, S->msp430->R[i]);
		mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->msp430->R[i]);
	}

	return;
}

void
msp430dumpsysregs(Engine *E, State *S)
{
}

void
msp430fatalaction(Engine *E, State *S)
{
}

void
msp430stallaction(State *S, ulong addr, int type, int latency)
{
}


tuck int
msp430take_timer_intr(State *S)
{
	return 0;
}


void
msp430resetcpu(Engine *E, State *S)
{
	msp430flushpipe(S);

	S->MEMSIZE = MSP430_DEFLT_MEMSIZE;
	S->MEMBASE = MSP430_MEMBASE;
	S->MEMEND = S->MEMBASE + S->MEMSIZE;
	
	S->mem_r_latency = MSP430_DEFAULT_MEMREAD_LATENCY;
	S->mem_w_latency = MSP430_DEFAULT_MEMWRITE_LATENCY;

	memset(&S->msp430->P, 0, sizeof(MSP430Pipe));
	memset(&S->energyinfo, 0, sizeof(EnergyInfo));
	memset(&S->msp430->R[0], 0, sizeof(ulong)*16);
	memset(S->MEM, 0, S->MEMSIZE);
	memset(S->msp430->B, 0, sizeof(MSP430Buses));

	S->PC = MSP430_MEMBASE;
	S->pcstackheight = 0;
	S->fpstackheight = 0;

	S->TIME = E->globaltimepsec;
	S->dyncnt = 0;
	S->CLK = 0;
	S->ICLK = 0;
	S->cmdbuf_nbytes = 0;

	S->VDD = MSP430_DEFAULT_VDD;
	S->CYCLETIME = MSP430_DEFAULT_CYCLETIME;

	S->mem_r_latency = MSP430_DEFAULT_MEMREAD_LATENCY;
	S->mem_w_latency = MSP430_DEFAULT_MEMWRITE_LATENCY;
	S->Cycletrans = 0;

	S->runnable = 0;
	S->sleep = 0;
	S->ustart = 0;
	S->ufinish = 0;
	S->startclk = 0;
	S->finishclk = 0;
	
	S->step = msp430step;
	S->pipelined = 1;
	S->pipeshow = 0;

	fault_setnodepfun(S, "urnd");

	return;
}

State *
msp430newstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename)
{
	int	i;
	State 	*S;
	char 	*logfilename;


	S = (State *)mcalloc(E, 1, sizeof(State), "(State *)S");
	if (S == NULL)
	{
		mexit(E, "Failed to allocate memory for State *S.", -1);
	}

	S->msp430 = (MSP430State *)mcalloc(E, 1, sizeof(MSP430State), "S->msp430");
	if (S->msp430 == NULL)
	{
		mexit(E, "Failed to allocate memory for S->msp430.", -1);
	}

	S->MEM = (uchar *)mcalloc(E, 1, DEFLT_MEMSIZE, "(uchar *)S->MEM");
	if (S->MEM == NULL)
	{
		mexit(E, "Failed to allocate memory for S->MEM.", -1);
	}

	S->msp430->B = (MSP430Buses *)mcalloc(E, 1, sizeof(MSP430Buses), "(MSP430Buses *)S->msp430->B");
	if (S->msp430->B == NULL)
	{
		mexit(E, "Failed to allocate memory for S->msp430->B.", -1);
	}


	S->N = (Numa *)mcalloc(E, 1, sizeof(Numa), "(Numa *)S->N");
	if (S->N == NULL)
	{
		mexit(E, "Failed to allocate memory for S->N.", -1);
	}
	S->N->count = 0;

	/*	Actual entries are allocated when a region is installed		*/
	S->N->regions = (Numaregion **)mcalloc(E, MAX_NUMA_REGIONS,
		sizeof(Numaregion*), "(Numaregion **)S->N->regions");
	if (S->N->regions == NULL)
	{
		mexit(E, "Failed to allocate memory for S->N->regions.", -1);
	}


	S->Nstack = (Numa *)mcalloc(E, 1, sizeof(Numa), "(Numa *)S->Nstack");
	if (S->Nstack == NULL)
	{
		mexit(E, "Failed to allocate memory for S->Nstack.", -1);
	}
	S->Nstack->count = 0;

	/*	Actual entries are allocated when a region is installed		*/
	S->Nstack->regions = (Numaregion **)mcalloc(E, MAX_NUMA_REGIONS,
		sizeof(Numaregion*), "(Numaregion **)S->Nstack->regions");
	if (S->Nstack->regions == NULL)
	{
		mexit(E, "Failed to allocate memory for S->Nstack->regions.", -1);
	}


	S->RT = (Regtraces *)mcalloc(E, 1, sizeof(Regtraces), "(Regtraces *)S->RT");
	if (S->RT == NULL)
	{
		mexit(E, "Failed to allocate memory for S->RT.", -1);
	}
	S->RT->count = 0;

	/*	Actual entries are allocated when a region is installed		*/
	S->RT->regvts = (Regvt **)mcalloc(E, MAX_REG_TRACERS,
		sizeof(Regvt*), "(Regvt **)S->RT->regvts");
	if (S->RT->regvts == NULL)
	{
		mexit(E, "Failed to allocate memory for S->RT->regvts.", -1);
	}



	if (SF_SIMLOG)
	{	
		logfilename = (char *)mcalloc(E, 1, MAX_NAMELEN*sizeof(char),
			"logfilename in machine-ti-msp430.c"); 
		if (logfilename == NULL)
		{
                	mexit(E, "Failed to allocate memory for logfilename.", -1);
        	}

		msnprint(logfilename, MAX_NAMELEN, "simlog.node%d", E->nnodes);

		S->logfd = mcreate(logfilename, M_OWRITE|M_OTRUNCATE);
		mfree(E, logfilename, "char * logfilename in machine-ti-msp430.c");

		if (S->logfd < 0)
		{
			mexit(E, "Could not open logfile for writing.", -1);
		}
	}

	E->cp = S;
	E->sp[E->nnodes] = S;
	mprint(E, NULL, siminfo, "New node created with node ID %d\n", E->nnodes);

	/*	Update the min cycle time	*/
	E->mincycpsec = PICOSEC_MAX;
	E->maxcycpsec = 0;
	for (i = 0; i < E->nnodes; i++)
	{
		E->mincycpsec = min(E->mincycpsec, E->sp[i]->CYCLETIME);
		E->maxcycpsec = max(E->maxcycpsec, E->sp[i]->CYCLETIME);
	}

	S->dumpregs = msp430dumpregs;
	S->dumpsysregs = msp430dumpsysregs;
	S->fatalaction = msp430fatalaction;
	S->stallaction = msp430stallaction;

	S->take_timer_intr = msp430take_timer_intr;

	S->resetcpu = msp430resetcpu;
	S->step = msp430step;
	S->dumppipe = msp430dumppipe;
	S->flushpipe = msp430flushpipe;

	/*	Most of the device registers are SH7708 specific	*/
	S->devreadbyte = dev430x1xxreadbyte;
	S->devreadword = dev430x1xxreadword;
	S->devreadlong = NULL;
	S->devwritebyte = dev430x1xxwritebyte;
	S->devwriteword = dev430x1xxwriteword;
	S->devwritelong = NULL;
	S->split = msp430split;

	S->xloc = xloc;
	S->yloc = yloc;
	S->zloc = zloc;

	S->trajfilename = (char *)mcalloc(E, 1, strlen(trajfilename)+1, "S->trajfilename in "SF_FILE_MACRO);
	if (S->trajfilename == nil)
	{
		mexit(E, "mcalloc failed for S->trajfilename in "SF_FILE_MACRO);
	}
	strcpy(S->trajfilename, trajfilename);

	S->NODE_ID = E->baseid + E->nnodes;

	/*	Must know correct number of nodes in resetcpu()		*/
	E->nnodes++;

	S->resetcpu(S);

	S->intrQ = (InterruptQ *)mcalloc(E, 1, sizeof(InterruptQ),
		"InterruptQ *intrQ in msp430newstate()");
	if (S->intrQ == NULL)
	{
		mexit(E, "Failed to allocate memory for InterruptQ *intrQ in msp430newstate().", -1);
	}

	S->intrQ->hd = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt *S->intrQ->hd in msp430newstate()");
	S->intrQ->tl = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt *S->intrQ->tl in msp430newstate()");
	if (S->intrQ->hd == NULL || S->intrQ->tl == NULL)
	{
		mexit(E, "Failed to allocate memory for S->intrQ->hd | S->intrQ->tl.", -1);
	}


	return S;
}

void
msp430split(Engine *E, State *S, ulong startpc, ulong stackptr, ulong argaddr, char *idstr)
{
	int		i;
	Numaregion	*tmp;


	/*								*/
	/*	Split the current CPU into two, sharing the same mem	*/
	/*								*/
	State	*N = msp430newstate(E, S->xloc, S->yloc, S->zloc, S->trajfilename);


	mprint(E, NULL, siminfo,
		"Splitting node %d to new node %d:\n\t\tstartpc @ 0x" UHLONGFMT 
		", stack @ 0x" UHLONGFMT ", arg @ 0x" UHLONGFMT "\n\n",
		S->NODE_ID, N->NODE_ID, startpc, stackptr, argaddr);

	mfree(E, N->MEM, "N->MEM in msp430split");
	mfree(E, N->msp430->B, "N->msp430->B in msp430split");

	N->MEM = S->MEM;
	N->msp430->B = S->msp430->B;

	/*								*/
	/*	Make a copy of the entire Numaregion queue, and reset 	*/
	/*	all the counters on the copy to 0. This enables us to	*/
	/*	inherit all created numaregions, but still maintain 	*/
	/*	individual counters.					*/
	/*								*/
	for (i = 0; i < S->N->count; i++)
	{
		tmp = (Numaregion *) mcalloc(E, 1, sizeof(Numaregion),
			"N->N->regions entry in machine-ti-msp430.c");
		if (tmp == NULL)
		{
			merror(E, "mcalloc failed for N->N->regions entry in machine-ti-msp430.c");
			return;
		}

		N->N->regions[i] = tmp;
		memcpy(N->N->regions[i], S->N->regions[i], sizeof(Numaregion));
		
		N->N->regions[i]->nreads = 0;
		N->N->regions[i]->nwrites = 0;
	}
	N->N->count = S->N->count;

	for (i = 0; i < S->Nstack->count; i++)
	{
		tmp = (Numaregion *) mcalloc(E, 1, sizeof(Numaregion),
			"N->Nstack->regions entry in machine-ti-msp430.c");
		if (tmp == NULL)
		{
			merror(E, "mcalloc failed for N->Nstack->regions entry in machine-ti-msp430.c");
			return;
		}

		N->Nstack->regions[i] = tmp;
		memcpy(N->Nstack->regions[i], S->Nstack->regions[i], sizeof(Numaregion));
		
		N->Nstack->regions[i]->nreads = 0;
		N->Nstack->regions[i]->nwrites = 0;
	}
	N->Nstack->count = S->Nstack->count;


	N->MEMBASE = S->MEMBASE;
	N->MEMEND = S->MEMEND;
	N->MEMSIZE = S->MEMSIZE;

	N->msp430->R[MSP430_SP] = stackptr;

	/*	BUG: this should be changed to match calling convention on msp430:	*/
	N->msp430->R[4] = argaddr;

	/*								*/
	/*	    Inherit the machine status (e.g., priv) too		*/
	/*	TODO: could memcpy the whole State *, but I haven't	*/
	/*	yet finalized how much heterogeneity in spawned 	*/
	/*	processors will exist					*/
	/*								*/
	N->msp430->R[MSP430_SR] = S->msp430->R[MSP430_SR];


	strncpy(N->idstr, idstr, MAX_NAMELEN);
	N->PC = startpc;
	N->runnable = 1;
	E->cp = N;

	return;
}
