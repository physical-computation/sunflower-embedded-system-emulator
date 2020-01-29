/*
	Copyright (c)	2017-2018, Zhengyang Gu (author)
			2018-2019, Harry Sarson (author)
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
#include <math.h>
#include "sf.h"
#include "mextern.h"

static void
print_integer_register_abi(Engine *E, State *S, ulong reg_index)
{
	/*
	 *	See https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#integer-register-convention-
	 */
	const char * special_names[] = {
		"zero",
		"ra",
		"sp",
		"gp",
		"tp",
	};
	if (reg_index < 5)
	{
		mprint(E, S, nodeinfo, "%-4s", special_names[reg_index]);
	}
	else if (reg_index < 8)
	{
		mprint(E, S, nodeinfo, "t%-3u", reg_index - 5);
	}
	else if (reg_index < 10)
	{
		mprint(E, S, nodeinfo, "s%-3u", reg_index - 8);
	}
	else if (reg_index < 18)
	{
		mprint(E, S, nodeinfo, "a%-3u", reg_index - 10);
	}
	else if (reg_index < 28)
	{
		mprint(E, S, nodeinfo, "s%-3u", reg_index - 18 + 2);
	}
	else if (reg_index < 32)
	{
		mprint(E, S, nodeinfo, "t%-3u", reg_index - 28 + 3);
	}
	else
	{
		mexit(E, "Cannot get ABI name for invalid index.", -1);
	}
}

static void
print_fp_register_abi(Engine *E, State *S, ulong reg_index)
{
	/*
	 *	See https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#floating-point-register-convention-
	 */
	if (reg_index < 8)
	{
		mprint(E, S, nodeinfo, "ft%-2u", reg_index);
	}
	else if (reg_index < 10)
	{
		mprint(E, S, nodeinfo, "fs%-2u", reg_index - 8);
	}
	else if (reg_index < 18)
	{
		mprint(E, S, nodeinfo, "fa%-2u", reg_index - 10);
	}
	else if (reg_index < 28)
	{
		mprint(E, S, nodeinfo, "fs%-2u", reg_index - 18 + 2);
	}
	else if (reg_index < 32)
	{
		mprint(E, S, nodeinfo, "ft%-2u", reg_index - 28 + 8);
	}
	else
	{
		mexit(E, "Cannot get ABI name for invalid index.", -1);
	}
}

tuck void
riscvstallaction(Engine *E, State *S, ulong addr, int type, int latency)
{
	/*	PAU may change VDD	*/
	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, type, addr);
	}

	/*								*/
	/*	Stall fetch unit on next access or instr in EX		*/
	/*	the stall actually occurs when in MA, since we've	*/
	/*	completed the EX wait before we get executed.		*/
	/*								*/
	if (S->superH->mem_access_type == MEM_ACCESS_IFETCH)
	{
		/*	I don't know why Philip used fetch_stall_cycles, and not		*/
		/*	IF.cycles (he doesn't remember either) but I'll leave it as it is...	*/
		S->riscv->P.fetch_stall_cycles += latency;
	}
	else
	{
		S->riscv->P.EX.cycles += latency;
	}

	/*								*/
	/*	TODO: This will have to change when we implement	*/
	/*	setjmp idea for simulating memory stalls		*/
	/*								*/

	//superH equivalent has buslocking management here

	return;
}

void
riscvdumpregs(Engine *E, State *S)
{
	int	i;
	char	fp_value[128];
	char *	f_width;

	for (i = 0; i < 32; i++)
	{
		mprint(E, S, nodeinfo, "x%-2d\t", i);
		print_integer_register_abi(E, S, i);
		mprint(E, S, nodeinfo, "\t", i);
		mbitprint(E, S, 32, S->riscv->R[i]);
		mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->riscv->R[i]);
	}

	mprint(E, S, nodeinfo, "\n");

	for (i = 0; i < 32; i++)
	{
		mprint(E, S, nodeinfo, "f%-2d\t", i);
		print_fp_register_abi(E, S, i);
		mprint(E, S, nodeinfo, "\t", i);
		uint64_t float_bits = S->riscv->fR[i];
		if((float_bits >> 32) == 0xFFFFFFFF) //NaN boxed
		{
			rv32f_rep val;
			val.bit_value = (uint32_t)float_bits;
			snprintf(fp_value, sizeof(fp_value), "%#.8g", val.float_value);
			if (S->riscv->uncertain == NULL || !isnan(S->riscv->uncertain->registers.variances[i]))
			{
				size_t start_offset = strlen(fp_value);
				snprintf(
					fp_value + start_offset,
					sizeof(fp_value) - start_offset,
					" +- %#-.5g", sqrtf(S->riscv->uncertain->registers.variances[i])
				);
			}
			f_width = "single";
		}
		else
		{
			rv32d_rep val;
			val.bit64_value = (uint64_t)float_bits;
			snprintf(fp_value, sizeof(fp_value), "%.8g", val.double_value);
			f_width = "double";
		}
		mprint(E, S, nodeinfo, "%-23s (%s)          [0x%016llx]\n", fp_value, f_width, S->riscv->fR[i]);
	}


	return;
}

void
riscvdumpsysregs(Engine *E, State *S)
{
	/*
	 *	RISC-V does not have system registers. We dump the uncertain regs.
	 *	Could also dump some CSR information.
	 */
	uncertain_print_system(S->riscv->uncertain, stdout);

}

void
riscvfatalaction(Engine *E, State *S)
{
	mprint(E, S, nodeinfo, "FATAL (node %d): P.EX=[%s]\n",\
			S->NODE_ID, riscv_opstrs[S->riscv->P.EX.op]);

	return;
}




/*
 *						Histograms
 */

/*
 *	Print histogram
 */
void
riscvdumphist(Engine *E, State *S, int histogram_id)
{
	mprint(E, S, nodeinfo, "Printing information for register %u\n", histogram_id);
	mprint(E, S, nodeinfo, "bin | val \n");
	mprint(E, S, nodeinfo, "----+-----\n");

	for (int i = 0; i < kUncertainAluHistogramBins; i++)
	{
		mprint(E, S, nodeinfo, "%03u | %-3u\n", i, S->riscv->histograms[histogram_id].bins[i]);
	}

	return;
}

/*
 *	Print histogram with extra stats and ASCII graphics
 */
void
riscvdumphistpretty(Engine *E, State *S, int histogram_id)
{
	mprint(E, S, nodeinfo, "Printing information for register %u\n", histogram_id);
	Histogram_PrettyPrint(E, S, &S->riscv->histograms[histogram_id]);

	return;
}

/*
 *	Load histogram with bin values randomly filled
 */
void
riscvldhistrandom(Engine *E, State *S, int histogram_id)
{
	Histogram_LDRandom(E, S, &S->riscv->histograms[histogram_id]);

	return;
}

/*
 *	Add two histograms
 */
void
riscvaddhist(Engine *E, State *S, int histogram_id0, int histogram_id1, int histogram_id_dest)
{
	Histogram_AddDist(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			&S->riscv->histograms[histogram_id1],
			&S->riscv->histograms[histogram_id_dest]
			);

	return;
}

/*
 *	Scalar multiply a histogram
 */
void
riscvscalarmultiply(Engine *E, State *S, int histogram_id0, int scalar)
{
	Histogram_ScalarMultiply(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			scalar
			);

	return;
}

/*
 *	Subtract two histograms
 */
void
riscvsubhist(Engine *E, State *S, int histogram_id0, int histogram_id1, int histogram_id_dest){
	Histogram_SubDist(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			&S->riscv->histograms[histogram_id1],
			&S->riscv->histograms[histogram_id_dest]
			);

	return;
}


/*
 *	Combine (bin-wise add) histograms
 */
void
riscvcombhist(Engine *E, State *S, int histogram_id0, int histogram_id1, int histogram_id_dest)
{
	Histogram_CombDist(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			&S->riscv->histograms[histogram_id1],
			&S->riscv->histograms[histogram_id_dest]
			);

	return;
}

/*
 *	Lower bound of histogram
 */
void
riscvlowerboundhist(Engine *E, State *S, int histogram_id0, int output_reg)
{
	int result = Histogram_LowerBound(
			E,
			S,
			&S->riscv->histograms[histogram_id0]
			);

	S->riscv->R[output_reg] = result;

	return;
}

/*
 *	Upper bound of histogram
 */
void
riscvupperboundhist(Engine *E, State *S, int histogram_id0, int output_reg)
{
	int result = Histogram_UpperBound(
			E,
			S,
			&S->riscv->histograms[histogram_id0]
			);

	S->riscv->R[output_reg] = result;

	return;
}

/*
 *	Left shift
 */
void
riscvdistlshifthist(Engine *E, State *S, int histogram_id0, int Rs2, int histogram_id_dest)
{
	Histogram_DistLShift(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			Rs2,
			&S->riscv->histograms[histogram_id_dest]
			);

	return;
}


/*
 *	Right shift
 */
void
riscvdistrshifthist(Engine *E, State *S, int histogram_id0, int Rs2, int histogram_id_dest)
{
	Histogram_DistRShift(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			Rs2,
			&S->riscv->histograms[histogram_id_dest]
			);

	return;
}

/*
 *	Expected value
 */
void
riscvexpectedvaluehist(Engine *E, State *S, int histogram_id0, int output_reg)
{
	int result = Histogram_ExpectedValue(
			E,
			S,
			&S->riscv->histograms[histogram_id0]
			);

	S->riscv->R[output_reg] = result;

	return;
}

/*
 *	DistLess returns the probability Pr(X < Rs2)
 */
void
riscvdistlesshist(Engine *E, State *S, int histogram_id0, int Rs2, int output_reg){
	int result = Histogram_DistLess(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			Rs2
			);

	S->riscv->R[output_reg] = result;

	return;
}


/*
 *	DistGrt returns the probability Pr(X >= Rs2)
 */
void
riscvdistgrthist(Engine *E, State *S, int histogram_id0, int Rs2, int output_reg){
	int result = Histogram_DistGrt(
			E,
			S,
			&S->riscv->histograms[histogram_id0],
			Rs2
			);

	S->riscv->R[output_reg] = result;

	return;
}

static UncertainState *
uncertainnewstate(Engine *E, char *ID)
{
	int i;
	UncertainState *S = (UncertainState *)mcalloc(E, 1, sizeof(UncertainState), ID);

	if (S == NULL)
	{
		mexit(E, "Failed to allocate memory uncertain state.", -1);
	}

	for (i = 0; i < 32; ++i) {
		uncertain_inst_sv(S, i, nan(""));
	}

	return S;
}

void
riscVresetcpu(Engine *E, State *S)
{
	int	i;


	riscvflushpipe(S);


	S->MEMSIZE = DEFLT_MEMSIZE;
	S->MEMBASE = SUPERH_MEMBASE;
	S->MEMEND = S->MEMBASE + S->MEMSIZE;
	
	S->mem_r_latency = DEFAULT_MEMREAD_LATENCY;
	S->mem_w_latency = DEFAULT_MEMWRITE_LATENCY;

	/*
	*	Initialisation of shadow memory:
	*/

	S->TAINTMEMSIZE = DEFLT_MEMSIZE*sizeof(ShadowMem);
	S->TAINTMEMBASE = SUPERH_MEMBASE;
	S->TAINTMEMEND = S->TAINTMEMBASE + S->TAINTMEMSIZE;


	// memset(&S->superH->P, 0, sizeof(SuperHPipe));
	memset(&S->energyinfo, 0, sizeof(EnergyInfo));
	// memset(&S->superH->R, 0, sizeof(ulong)*16);
	// memset(&S->superH->R_BANK, 0, sizeof(ulong)*8);
	// memset(&S->superH->SR, 0, sizeof(SuperHSREG));
	// memset(&S->superH->SSR, 0, sizeof(SuperHSREG));
	memset(S->MEM, 0, S->MEMSIZE);
	// memset(S->superH->B, 0, sizeof(SuperHBuses));

	/*								*/
	/*	The only the ratio of size:blocksize and assoc are	*/
	/*	significant when Cache struct is used for modeling TLB	*/
	/*								*/
	// superHtlb_init(E, S, 128, 1, 4);

	// S->superH->GBR = 0;
	// S->superH->VBR = SUPERH_MEMBASE;
	// S->superH->MACH = 0;
	// S->superH->MACL = 0;
	S->PC = SUPERH_MEMBASE;
	// S->superH->PR = 0;
	// S->superH->SPC = 0;
	S->pcstackheight = 0;
	S->fpstackheight = 0;


	S->TIME = E->globaltimepsec;
	// S->superH->TIMER_LASTACTIVATE = 0.0;
	// S->superH->TIMER_INTR_DELAY = 1E-3;
	S->dyncnt = 0;
	S->nfetched = 0;
	S->CLK = 0;
	S->ICLK = 0;
	S->cmdbuf_nbytes = 0;

	S->CYCLETIME = SUPERH_ORIG_CYCLE;
	S->VDD = SUPERH_ORIG_VDD;
	S->SVDD = 0.0;
	S->LOWVDD = S->VDD/2.0;

	S->voltscale_alpha = 2.0;
	//BUG?
	S->voltscale_K = SUPERH_ORIG_VDD * SUPERH_ORIG_CYCLE;
	S->voltscale_Vt = 0.0;

	S->Cycletrans = 0;
	// S->superH->mem_access_type = 0;

	// S->superH->PTEL = 0;
	// S->superH->PTEH = 0;
	// S->superH->TTB = 0;
	// S->superH->TEA = 0;
	// S->superH->MMUCR = 0;

	// S->superH->TRA = 0;
	// S->superH->EXPEVT = 0;
	// S->superH->INTEVT = 0;

	// S->superH->ICR = 0;
	// S->superH->ICRA = 0;
	// S->superH->ICRB = 0;

	S->runnable = 0;
	S->sleep = 0;
	S->ustart = 0;
	S->ufinish = 0;
	S->startclk = 0;
	S->finishclk = 0;
	
	//S->step = superHstep;
	S->pipelined = 1;
	S->pipeshow = 0;

	fault_setnodepfun(E, S, "urnd");

	// if (SF_PAU_DEFINED)
	// {
	// 	pau_init(E, S, S->superH->npau);
	// 	mprint(E, S, nodeinfo,
	// 		"Done with pauinit, for %d PAU entries...\n",
	// 		S->superH->npau);
	// }

	// for (i = SUPERH_OP_ADD; i <= SUPERH_OP_XTRCT; i++)
	// {
	// 	double reading = (R0000[i].reading1 + R0000[i].reading2)/2;

	// 	/*							*/
	// 	/*	Scaled current, I2 = (I1*V2*t1)/(V1*t2);	*/
	// 	/*							*/
	// 	S->scaledcurrents[i] =
	// 		((reading*S->VDD*SUPERH_ORIG_CYCLE)/(SUPERH_READINGS_VDD*S->CYCLETIME))*1E-3;

	// 	S->superH->opncycles[i] = R0000[i].ncycles;
	// }

	/*	Since we've reset VDD, need to update this	*/
	E->mincycpsec = PICOSEC_MAX;
	E->maxcycpsec = 0;
	for (i = 0; i < E->nnodes; i++)
	{
		E->mincycpsec = min(E->mincycpsec, E->sp[i]->CYCLETIME);
		E->maxcycpsec = max(E->maxcycpsec, E->sp[i]->CYCLETIME);
	}

	return;
}

State *
riscvnewstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename)
{
	/* State *	S = superHnewstate(E, xloc, yloc, zloc, trajfilename);
	 * Override S->writebyte = to point to riscVwritebyte 
	 * Temporary fix until superHnewstate is removed 
	 */
	State 	*S;

	S = (State *)mcalloc(E, 1, sizeof(State), "(State *)S");
	if (S == NULL)
	{
		mexit(E, "Failed to allocate memory for State *S.", -1);
	}

	S->riscv = (RiscvState *) mcalloc(E, 1, sizeof(RiscvState), "S->riscv");
	if (S->riscv == NULL)
	{
		mexit(E, "Failed to allocate memory for S->riscv.", -1);
	}

	S->riscv->uncertain = uncertainnewstate(E, "S->riscv->uncertain");

	S->MEM = (uchar *)mcalloc(E, 1, DEFLT_MEMSIZE, "(uchar *)S->MEM");
	if (S->MEM == NULL)
	{
		mexit(E, "Failed to allocate memory for S->MEM.", -1);
	}

	if (SF_TAINTANALYSIS)
	{
		S->TAINTMEM = (ShadowMem *)mcalloc(E, 1, S->TAINTMEMSIZE, "(ShadowMem *)S->TAINTMEM"); 
		if (S->TAINTMEM == NULL)
		{
			mexit(E, "Failed to allocate memory for S->TAINTMEM.", -1);
		}
	}

	E->cp = S;
	E->sp[E->nnodes] = S;
	mprint(E, NULL, siminfo, "New node created with node ID %d\n", E->nnodes);

	/*	Update the min cycle time	*/
	E->mincycpsec = PICOSEC_MAX;
	E->maxcycpsec = 0;
	for (int i = 0; i < E->nnodes; i++)
	{
		E->mincycpsec = min(E->mincycpsec, E->sp[i]->CYCLETIME);
		E->maxcycpsec = max(E->maxcycpsec, E->sp[i]->CYCLETIME);
	}

	/* FIXME superH related functions need to be cleaned up. */
	S->settimerintrdelay = superHsettimerintrdelay;

	S->take_nic_intr = superHtake_nic_intr;
	S->take_timer_intr = superHtake_timer_intr;
	S->take_batt_intr = superHtake_batt_intr;
	S->check_batt_intr = superHcheck_batt_intr;
	S->check_nic_intr = superHcheck_nic_intr;

	S->cache_init = superHcache_init;
	S->resetcpu = riscVresetcpu;
	S->cyclestep = superHstep;

	/*	Most of the device registers are SH7708 specific	*/
	S->devreadbyte = dev7708readbyte;
	S->devreadword = dev7708readword;
	S->devreadlong = dev7708readlong;
	S->devwritebyte = dev7708writebyte;
	S->devwriteword = dev7708writeword;
	S->devwritelong = dev7708writelong;
	S->split = superHsplit;
	S->vmtranslate = superHvmtranslate;
	S->dumptlb = superHdumptlb;
	S->cache_deactivate = superHcache_deactivate;
	S->cache_printstats = superHcache_printstats;

	S->xloc = xloc;
	S->yloc = yloc;
	S->zloc = zloc;

	if (trajfilename != NULL)
	{
		mexit(E, "S->trajfilename is not supported in SF Risc-V", -1);
	}

	S->NODE_ID = E->baseid + E->nnodes;

	/*	Must know correct number of nodes in resetcpu()		*/
	E->nnodes++;
	S->resetcpu(E, S);

	S->writebyte = riscVwritebyte;

	S->dumpregs = riscvdumpregs;
	S->dumpsysregs = riscvdumpsysregs;
	S->dumppipe = riscvdumppipe;
	S->flushpipe = riscvflushpipe;
	
	/*
	 *	Histogram-specific menu items
	 */
	S->dumphist = riscvdumphist;
	S->dumphistpretty = riscvdumphistpretty;
	S->ldhistrandom = riscvldhistrandom;
	S->addhist = riscvaddhist;

	S->fatalaction = riscvfatalaction;
	S->endian = Little;
	S->machinetype = MACHINE_RISCV;
	S->dumpdistribution = riscvdumpdistribution;
	S->stallaction = riscvstallaction;

	S->step = riscvstep;
	S->faststep = riscvfaststep;

	return S;
}
