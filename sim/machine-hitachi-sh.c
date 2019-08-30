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
#include <math.h>
#include <float.h>
#include "sf.h"
#include "mextern.h"
#include "ilpa.h"


static tuck void	drain_pipeline(Engine *E, State *S);
static tuck int		interruptible(State *S);


tuck int
interruptible(State *S)
{
	//TODO: this is not quite right, since we should be 
	//	able to take intrs even if bus is busy
	if (S->superH->B->pbuslock && (S->superH->B->pbuslocker != S->NODE_ID))
	{
		return 0;
	}

	switch (S->superH->P.ID.op)
	{
		case SUPERH_OP_BF:
		case SUPERH_OP_BT:
		case SUPERH_OP_BRA:
		case SUPERH_OP_BSR:
		case SUPERH_OP_JMP:
		case SUPERH_OP_JSR:
		case SUPERH_OP_RTS:
		case SUPERH_OP_RTE:
		case SUPERH_OP_TRAPA:
		case SUPERH_OP_BFS:
		case SUPERH_OP_BTS:
		case SUPERH_OP_BRAF:
		case SUPERH_OP_BSRF:
		{
			return 0;
		}
	}

	switch (S->superH->P.IF.op)
	{
		case SUPERH_OP_BF:
		case SUPERH_OP_BT:
		case SUPERH_OP_BRA:
		case SUPERH_OP_BSR:
		case SUPERH_OP_JMP:
		case SUPERH_OP_JSR:
		case SUPERH_OP_RTS:
		case SUPERH_OP_RTE:
		case SUPERH_OP_TRAPA:
		case SUPERH_OP_BFS:
		case SUPERH_OP_BTS:
		case SUPERH_OP_BRAF:
		case SUPERH_OP_BSRF:
		{
			return 0;
		}
	}

	return 1;
}

tuck void
drain_pipeline(Engine *E, State *S)
{
	/*							*/
	/*	Note: this will continue trying to drain	*/
	/*	pipeline even if simulator is off, since it	*/
	/*	is called from (possibly) spawned scheduler	*/
	/*	thread. TODO: we can't halt this now because	*/
	/*	we'll lose state of how we got here: Once we	*/
	/*	implement per-CPU setjmp state-saving for 	*/
	/*	scheduler, that should be easier.		*/
	/*							*/
	while (	(S->superH->P.IF.instr != 0x9) ||
		(S->superH->P.ID.instr != 0x9) ||
		(S->superH->P.EX.instr != 0x9) ||
		(S->superH->P.MA.instr != 0x9) ||
		(S->superH->P.WB.instr != 0x9))
	{
		/*	Call superHstep, with flag set to drain pipe	*/
		superHstep(E, S, 1);
	}

	return;
}

void
superHsettimerintrdelay(Engine *E, State *S, int delay)
{
	S->superH->TIMER_INTR_DELAY = delay * 1E-6;
}

tuck int
superHcheck_nic_intr(Engine *E, State *S)
{
	return superH_check_nic_intr_macro(S);
}

tuck int
superHcheck_batt_intr(Engine *E, State *S)
{
	return ((((S->BATT) &&
			((Batt *)S->BATT)->battery_remaining <=
			((Batt *)S->BATT)->battery_capacity*S->battery_alert_frac) ||
			(S->ENABLE_TOO_MANY_FAULTS && (S->nfaults >= S->faultthreshold))) &&
		(S->superH->SR.BL == 0) &&
		(S->superH->SR.IMASK < BATT_LOW_FIXED_INTRLEVEL) &&
		!((int)S->ICLK % 60));
}

void
superHdumpregs(Engine *E, State *S)
{
	int i;

	for (i = 0; i < 16; i++)
	{
		mprint(E, S, nodeinfo, "R%-2d\t\t", i);
		mbitprint(E, S, 32, S->superH->R[i]);
		mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->R[i]);
	}

	return;
}

void
superHdumpsysregs(Engine *E, State *S)
{
	int i;
	long tmp;
	
	for (i = 0; i < 8; i++)
	{
		mprint(E, S, nodeinfo, "%-7s%d\t", "R_BANK_", i);
		mbitprint(E, S, 32, S->superH->R_BANK[i]);
		mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->R_BANK[i]);
	}

	memmove(&tmp, &S->superH->SR, sizeof(tmp));
	mprint(E, S, nodeinfo, "%-8s\t", "SR");
	mbitprint(E, S, 32, tmp);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", tmp);

	memmove(&tmp, &S->superH->SSR, sizeof(tmp));
	mprint(E, S, nodeinfo, "%-8s\t", "SSR");
	mbitprint(E, S, 32, tmp);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", tmp);

	mprint(E, S, nodeinfo, "%-8s\t", "GBR");
	mbitprint(E, S, 32, S->superH->GBR);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->GBR);

	mprint(E, S, nodeinfo, "%-8s\t", "MACH");
	mbitprint(E, S, 32, S->superH->MACH);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->MACH);

	mprint(E, S, nodeinfo, "%-8s\t", "MACL");
	mbitprint(E, S, 32, S->superH->MACL);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->MACL);

	mprint(E, S, nodeinfo, "%-8s\t", "PR");
	mbitprint(E, S, 32, S->superH->PR);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->PR);

	mprint(E, S, nodeinfo, "%-8s\t", "VBR");
	mbitprint(E, S, 32, S->superH->VBR);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->VBR);

	mprint(E, S, nodeinfo, "%-8s\t", "PC");
	mbitprint(E, S, 32, S->PC);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->PC);

	mprint(E, S, nodeinfo, "%-8s\t", "SPC");
	mbitprint(E, S, 32, S->superH->SPC);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->SPC);

	mprint(E, S, nodeinfo, "%-8s\t", "TTB");
	mbitprint(E, S, 32, S->superH->TTB);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->TTB);

	mprint(E, S, nodeinfo, "%-8s\t", "TEA");
	mbitprint(E, S, 32, S->superH->TEA);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->TEA);

	mprint(E, S, nodeinfo, "%-8s\t", "MMUCR");
	mbitprint(E, S, 32, S->superH->MMUCR);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->MMUCR);

	mprint(E, S, nodeinfo, "%-8s\t", "PTEH");
	mbitprint(E, S, 32, S->superH->PTEH);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->PTEH);

	mprint(E, S, nodeinfo, "%-8s\t", "PTEL");
	mbitprint(E, S, 32, S->superH->PTEL);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->PTEL);

	mprint(E, S, nodeinfo, "%-8s\t", "TRA");
	mbitprint(E, S, 32, S->superH->TRA);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->TRA);

	mprint(E, S, nodeinfo, "%-8s\t", "EXPEVT");
	mbitprint(E, S, 32, S->superH->EXPEVT);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->EXPEVT);

	mprint(E, S, nodeinfo, "%-8s\t", "INTEVT");
	mbitprint(E, S, 32, S->superH->INTEVT);
	mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->superH->INTEVT);

	mprint(E, S, nodeinfo, "SLEEP = [%s]\n", (S->sleep == 1 ? "YES" : "NO"));
	
	return;
}

void
superHfatalaction(Engine *E, State *S)
{
	superHdumptlb(E, S);
	mprint(E, S, nodeinfo, "FATAL (node %d): P.EX=[%s]\n",\
			S->NODE_ID, superH_opstrs[S->superH->P.EX.op]);

	return;
}

tuck void
superHstallaction(Engine *E, State *S, ulong addr, int type, int latency)
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
		S->superH->P.fetch_stall_cycles += latency;
	}
	else
	{
		S->superH->P.EX.cycles += latency;
	}

	/*								*/
	/*	TODO: This will have to change when we implement	*/
	/*	setjmp idea for simulating memory stalls		*/
	/*								*/
	//So that accesses to main mem that are not mapped objects do not lock bus
	//if (latency == 0)
	//{
	//	return;
	//}

	S->superH->B->pbuslock = 1;
	S->superH->B->pbuslocker = S->NODE_ID;
	S->superH->B->pbuslock_type = type;
	S->superH->B->pbuslock_addr = addr;

	return;
}


tuck int
superHtake_timer_intr(Engine *E, State *S)
{
	if (!interruptible(S))
	{
		return -1;
	}

	/*									*/
	/*	Timer interrupts should be in terms of absolute time, not	*/
	/*	clock cycles, so that even w/ volt/freq scaling, we will	*/
	/*	still get timer interrupts at the same time spacing		*/
	/*									*/
	if ((S->superH->ENABLE_CLK_INTR) && (S->ICLK) &&
		(S->superH->SR.BL == 0) &&
		(S->superH->SR.IMASK < TIMER_FIXED_INTRLEVEL)
	  )
	{
		if (S->step == superHfaststep)
		{
			S->superH->SPC = S->PC;
		}
		else
		{
			drain_pipeline(E, S);

			/*						*/
			/*	Must do this after drain because 	*/	
			/*	executed instruction during drain 	*/
			/*	might set PC (e.g. a BRA)		*/
			/*						*/
			S->superH->SPC = S->PC;
		}

		S->superH->SSR = S->superH->SR;
		S->superH->SR.BL = 1;
		S->superH->SR.MD = 1;
		S->superH->SR.RB = 1;
		S->superH->INTEVT = TMU0_TUNI0_EXCP_CODE;
		S->PC = S->superH->VBR + 0x600;
		S->sleep = 0;
	}

	return 0;
}

tuck void
superHtake_exception(Engine *E, State *S)
{
	enum		{ABORTED_AND_RETRIED, ABORTED, COMPLETED, INVALID_HANDLING};
	Interrupt	*intr;
	int		handling = INVALID_HANDLING;


	intr = (Interrupt *)pic_intr_dequeue(E, S, S->superH->excpQ);
	if (intr == NULL)
	{
		sfatal(E, S,
		"We supposedly had an exception, but nothing was queued!");
	}

	S->superH->SSR = S->superH->SR;
	S->superH->SR.BL = 1;
	S->superH->SR.MD = 1;
	S->superH->SR.RB = 1;
	S->sleep = 0;

	switch (intr->type)
	{
		case H_UDI_RESET_EXCP:
		{
			S->PC = RESET_VECTOR_ADDR;
			S->superH->EXPEVT = H_UDI_RESET_EXCP_CODE;
			handling = ABORTED;
			break;
		}
		case POWER_ON_RESET_EXCP:
		{
			S->PC = RESET_VECTOR_ADDR;
			S->superH->EXPEVT = POWER_ON_RESET_EXCP_CODE;
			handling = ABORTED;
			break;
		}
		case MANUAL_RESET_EXCP:
		{
			S->PC = RESET_VECTOR_ADDR;
			S->superH->EXPEVT = MANUAL_RESET_EXCP_CODE;
			handling = ABORTED;
			break;
		}
		case TLB_LOAD_MISS_EXCP:
		{
			S->PC = S->superH->VBR + TLB_MISS_OFFSET;
			S->superH->EXPEVT = TLB_LOAD_MISS_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TLB_STORE_MISS_EXCP:
		{
			S->PC = S->superH->VBR + TLB_MISS_OFFSET;
			S->superH->EXPEVT = TLB_STORE_MISS_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TLB_LOAD_INVALID_EXCP:
		{
			S->PC = S->superH->VBR + TLB_INVALID_OFFSET;
			S->superH->EXPEVT = TLB_LOAD_INVALID_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TLB_STORE_INVALID_EXCP:
		{
			S->PC = S->superH->VBR + TLB_INVALID_OFFSET;
			S->superH->EXPEVT = TLB_STORE_INVALID_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TLB_INIT_PAGEWRITE_EXCP:
		{
			S->PC = S->superH->VBR + TLB_INIT_PAGEWRITE_OFFSET;
			S->superH->EXPEVT = TLB_INIT_PAGEWRITE_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TLB_LOAD_PROTECT_EXCP:
		{
			S->PC = S->superH->VBR + TLB_PROTECT_OFFSET;
			S->superH->EXPEVT = TLB_LOAD_PROTECT_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TLB_STORE_PROTECT_EXCP:
		{
			S->PC = S->superH->VBR + TLB_PROTECT_OFFSET;
			S->superH->EXPEVT = TLB_STORE_PROTECT_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case CPU_LOAD_ADDRERR_EXCP:
		{
			/*	Doesn't make a difference; Self documenting.	*/
			if (intr->misc == MEM_ACCESS_IFETCH)
			{
				S->PC = S->superH->VBR + CPU_INSTR_ADDRERR_OFFSET;
			}
			else
			{
				S->PC = S->superH->VBR + CPU_ADDRERR_OFFSET;
			}

			S->superH->EXPEVT = CPU_LOAD_ADDRERR_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case CPU_STORE_ADDRERR_EXCP:
		{
			S->PC = S->superH->VBR + CPU_ADDRERR_OFFSET;
			S->superH->EXPEVT = CPU_STORE_ADDRERR_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case TRAPA_EXCP_CODE:
		{
			S->PC = S->superH->VBR + TRAPA_OFFSET;
			S->superH->EXPEVT = TRAPA_EXCP_CODE;
			handling = COMPLETED;
			break;
		}
		case ILLEGAL_INSTR_EXCP:
		{
			S->PC = S->superH->VBR + ILLEGAL_INSTR_OFFSET;
			S->superH->EXPEVT = ILLEGAL_INSTR_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case ILLEGAL_SLOT_INSTR_EXCP:
		{
			S->PC = S->superH->VBR + ILLEGAL_SLOT_INSTR_OFFSET;
			S->superH->EXPEVT = ILLEGAL_SLOT_INSTR_EXCP_CODE;
			handling = ABORTED_AND_RETRIED;
			break;
		}
		case USER_BKPOINT_TRAP_EXCP:
		{
			S->PC = S->superH->VBR + USER_BKPOINT_OFFSET;
			S->superH->EXPEVT = USER_BKPOINT_TRAP_EXCP_CODE;
			handling = COMPLETED;
			break;
		}
		case DMA_ADDRERR_EXCP:
		{
			S->PC = S->superH->VBR + DMA_ADDRERR_OFFSET;
			S->superH->EXPEVT = DMA_ADDRERR_EXCP_CODE;
			handling = COMPLETED;
			break;
		}

		default:
		{
			sfatal(E, S, "Unknown/invalid exception code");
		}
	}

	if (handling == ABORTED)
	{
		/*	Current instruction is aborted		*/
		superHflushpipe(S);
	}
	else if (handling == ABORTED_AND_RETRIED)
	{
		superHflushpipe(S);
		S->superH->SPC = intr->value;
	}
	else if (handling == COMPLETED)
	{
		/*	Current instruction is completed	*/
		if (S->step == superHfaststep)
		{
			S->superH->SPC = S->PC;
		}
		else
		{
			drain_pipeline(E, S);
			S->superH->SPC = S->PC;
		}
	}

	mfree(E, intr, "Interrupt *interrupt in machine-hitachi-sh.c");

	return;

}

tuck int
superHtake_nic_intr(Engine *E, State *S)
{
	Interrupt	*interrupt;

	if (!interruptible(S))
	{
		return -1;
	}

	/* 								*/
	/*	(PC is incremented at end of step() in pipeline.c)	*/
	/*	We need to re-exec instruction which is currently in	*/
	/*	ID when we RTE, so save that kids PC! (then do a 	*/
	/*	IFIDflush()). If we are faststeping, then S->PC is 	*/
	/*	the next instr that we would place into EX and exec.	*/
	/*								*/
	/*	i.e., at this point, we are yet to execute instruction	*/
	/*	@PC (fastep/step below) so rather than setting SPC 	*/
	/*	to PC+2, we set it to PC, so that RTE executes the	*/
	/*	instr after then one after we caught the interrupt.	*/
	/*								*/
	if (S->step == superHfaststep)
	{
		S->superH->SPC = S->PC;
	}
	else
	{
		drain_pipeline(E, S);

		/*						*/
		/*	Must do this after drain because 	*/	
		/*	executed instruction during drain 	*/
		/*	might set PC (e.g. a BRA)		*/
		/*						*/
		S->superH->SPC = S->PC;
	}

	S->superH->SSR = S->superH->SR;
	S->superH->SR.BL = 1;
	S->superH->SR.MD = 1;
	S->superH->SR.RB = 1;

	S->PC = S->superH->VBR + 0x600;
	S->sleep = 0;

	interrupt = (Interrupt *)pic_intr_dequeue(E, S, S->superH->nicintrQ);
	if (interrupt == NULL)
	{
		sfatal(E, S,
		"We supposedly had an interrupt, but nothing was queued!");
	}

	/*								*/
	/*	The value field, which in the case of NIC, specifies 	*/
	/*	which  interface the interrupt was generated by. The	*/
	/*	apps. interpret exception codes offset from base as	*/
	/*	the interface number (well, you know what i mean...)	*/
	/*								*/
	switch (interrupt->type)
	{
		case NIC_RXOK_INTR:
		{
			S->superH->INTEVT = (NIC_RX_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_TXOK_INTR:
		{
			S->superH->INTEVT = (NIC_TX_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_ADDRERR_INTR:
		{
			S->superH->INTEVT = (NIC_ADDR_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_FRAMEERR_INTR:
		{
			S->superH->INTEVT = (NIC_FRAME_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_COLLSERR_INTR:
		{
			S->superH->INTEVT = (NIC_COLLS_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_CSENSEERR_INTR:
		{
			S->superH->INTEVT = (NIC_CSENSE_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_RXOVRRUNERR_INTR:
		{
			S->superH->INTEVT = (NIC_RXOVRRUN_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_RXUNDRRUNERR_INTR:
		{
			S->superH->INTEVT = (NIC_RXUNDRRUN_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_TXOVRRUNERR_INTR:
		{
			S->superH->INTEVT = (NIC_TXOVRRUN_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_TXUNDRRUNERR_INTR:
		{
			S->superH->INTEVT = (NIC_TXUNDRRUN_EXCP_CODE + interrupt->value);
			break;
		}

		case NIC_CSUMERR_INTR:
		{
			S->superH->INTEVT = (NIC_CSUM_EXCP_CODE + interrupt->value);
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Received interrupt type [%d]\n",
				interrupt->type);

			sfatal(E, S, "Unknown interrupt type!");
		}
	}
	mfree(E, interrupt, "Interrupt *interrupt in machine-hitachi-sh.c");

	return 0;
}

tuck int
superHtake_batt_intr(Engine *E, State *S)
{
	if (!interruptible(S))
	{
		return -1;
	}

	if (S->step == superHfaststep)
	{
		S->superH->SPC = S->PC;
	}
	else
	{
		drain_pipeline(E, S);
		
		/*						*/
		/*	Must do this after drain because 	*/	
		/*	executed instruction during drain 	*/
		/*	might set PC (e.g. a BRA)		*/
		/*						*/
		S->superH->SPC = S->PC;
	}

	S->superH->SSR = S->superH->SR;
	S->superH->SR.BL = 1;
	S->superH->SR.MD = 1;
	S->superH->SR.RB = 1;
	S->superH->INTEVT = BATT_LOW_EXCP_CODE;
	S->PC = S->superH->VBR + 0x600;

	return 0;
}

void
superHresetcpu(Engine *E, State *S)
{
	int	i;


	superHflushpipe(S);


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


	memset(&S->superH->P, 0, sizeof(SuperHPipe));
	memset(&S->energyinfo, 0, sizeof(EnergyInfo));
	memset(&S->superH->R, 0, sizeof(ulong)*16);
	memset(&S->superH->R_BANK, 0, sizeof(ulong)*8);
	memset(&S->superH->SR, 0, sizeof(SuperHSREG));
	memset(&S->superH->SSR, 0, sizeof(SuperHSREG));
	memset(S->MEM, 0, S->MEMSIZE);
	memset(S->superH->B, 0, sizeof(SuperHBuses));

	/*								*/
	/*	The only the ratio of size:blocksize and assoc are	*/
	/*	significant when Cache struct is used for modeling TLB	*/
	/*								*/
	superHtlb_init(E, S, 128, 1, 4);

	S->superH->GBR = 0;
	S->superH->VBR = SUPERH_MEMBASE;
	S->superH->MACH = 0;
	S->superH->MACL = 0;
	S->PC = SUPERH_MEMBASE;
	S->superH->PR = 0;
	S->superH->SPC = 0;
	S->pcstackheight = 0;
	S->fpstackheight = 0;


	S->TIME = E->globaltimepsec;
	S->superH->TIMER_LASTACTIVATE = 0.0;
	S->superH->TIMER_INTR_DELAY = 1E-3;
	S->dyncnt = 0;
	S->nfetched = 0;
	S->CLK = 0;
	S->ICLK = 0;
	S->cmdbuf_nbytes = 0;

	S->CYCLETIME = SUPERH_ORIG_CYCLE;
	S->VDD = SUPERH_ORIG_VDD;
	S->SVDD = 0.0;
	S->LOWVDD = S->VDD/2.0;
	

/*
	TODO:	set these using the power_scale routines. power_scale* should update these
		(there, not here). This also takes care of the setvdd/setfreq via sf.y
		adjustments needed.

	S->mem_r_latency
	S->mem_w_latency
	S->flash_r_latency
	S->flash_w_latency
*/

	S->voltscale_alpha = 2.0;
//BUG?
	S->voltscale_K = SUPERH_ORIG_VDD * SUPERH_ORIG_CYCLE;
	S->voltscale_Vt = 0.0;

	S->Cycletrans = 0;
	S->superH->mem_access_type = 0;

	S->superH->PTEL = 0;
	S->superH->PTEH = 0;
	S->superH->TTB = 0;
	S->superH->TEA = 0;
	S->superH->MMUCR = 0;

	S->superH->TRA = 0;
	S->superH->EXPEVT = 0;
	S->superH->INTEVT = 0;

	S->superH->ICR = 0;
	S->superH->ICRA = 0;
	S->superH->ICRB = 0;

	S->runnable = 0;
	S->sleep = 0;
	S->ustart = 0;
	S->ufinish = 0;
	S->startclk = 0;
	S->finishclk = 0;
	
	S->step = superHstep;
	S->pipelined = 1;
	S->pipeshow = 0;

	S->superH->txok_intrenable_flag		= 0;
	S->superH->rxok_intrenable_flag		= 1;
	S->superH->addrerr_intrenable_flag	= 1;
	S->superH->frameerr_intrenable_flag	= 1;
	S->superH->collserr_intrenable_flag	= 1;
	S->superH->csenseerr_intrenable_flag	= 1;
	S->superH->rxovrrunerr_intrenable_flag	= 1;
	S->superH->txovrrunerr_intrenable_flag	= 1;
	S->superH->rxundrrunerr_intrenable_flag	= 1;
	S->superH->txundrrunerr_intrenable_flag	= 1;
	S->superH->csumerr_intrenable_flag	= 1;

	fault_setnodepfun(E, S, "urnd");

	if (SF_PAU_DEFINED)
	{
		pau_init(E, S, S->superH->npau);
		mprint(E, S, nodeinfo,
			"Done with pauinit, for %d PAU entries...\n",
			S->superH->npau);
	}

	for (i = SUPERH_OP_ADD; i <= SUPERH_OP_XTRCT; i++)
	{
		double reading = (R0000[i].reading1 + R0000[i].reading2)/2;

		/*							*/
		/*	Scaled current, I2 = (I1*V2*t1)/(V1*t2);	*/
		/*							*/
		S->scaledcurrents[i] =
			((reading*S->VDD*SUPERH_ORIG_CYCLE)/(SUPERH_READINGS_VDD*S->CYCLETIME))*1E-3;

		S->superH->opncycles[i] = R0000[i].ncycles;
	}

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
superHnewstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename)
{
	int	i;
	State 	*S;
	char 	*logfilename;


	S = (State *)mcalloc(E, 1, sizeof(State), "(State *)S");
	if (S == NULL)
	{
		mexit(E, "Failed to allocate memory for State *S.", -1);
	}

	S->superH = (SuperHState *)mcalloc(E, 1, sizeof(SuperHState), "S->superH");
	if (S->superH == NULL)
	{
		mexit(E, "Failed to allocate memory for S->superH.", -1);
	}

	S->MEM = (uchar *)mcalloc(E, 1, DEFLT_MEMSIZE, "(uchar *)S->MEM");
	if (S->MEM == NULL)
	{
		mexit(E, "Failed to allocate memory for S->MEM.", -1);
	}

	/*
	*	Initialise array of ShadowMem objects:
	*/

	S->TAINTMEM = (ShadowMem *)mcalloc(E, 1, S->TAINTMEMSIZE, "(ShadowMem *)S->TAINTMEM"); 

	if (S->TAINTMEM == NULL)
	{
		mexit(E, "Failed to allocate memory for S->TAINTMEM.", -1);
	}


	S->superH->B = (SuperHBuses *)mcalloc(E, 1, sizeof(SuperHBuses), "(SuperHBuses *)S->superH->B");
	if (S->superH->B == NULL)
	{
		mexit(E, "Failed to allocate memory for S->superH->B.", -1);
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
			"logfilename in machine-hitachi-sh.c"); 
		if (logfilename == NULL)
		{
                	mexit(E, "Failed to allocate memory for logfilename.", -1);
        	}

		msnprint(logfilename, MAX_NAMELEN, "simlog.node%d", E->nnodes);

		S->logfd = mcreate(logfilename, M_OWRITE|M_OTRUNCATE);
		mfree(E, logfilename, "char * logfilename in machine-hitachi-sh.c");

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

	S->endian = Big;
	S->dumpregs = superHdumpregs;
	S->dumpsysregs = superHdumpsysregs;
	S->fatalaction = superHfatalaction;
	S->stallaction = superHstallaction;
	S->settimerintrdelay = superHsettimerintrdelay;

	S->take_nic_intr = superHtake_nic_intr;
	S->take_timer_intr = superHtake_timer_intr;
	S->take_batt_intr = superHtake_batt_intr;
	S->check_batt_intr = superHcheck_batt_intr;
	S->check_nic_intr = superHcheck_nic_intr;

	S->cache_init = superHcache_init;
	S->resetcpu = superHresetcpu;
	S->step = superHstep;
	S->cyclestep = superHstep;
	S->faststep = superHfaststep;
	S->dumppipe = superHdumppipe;
	S->flushpipe = superHflushpipe;

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

	S->writebyte = superHwritebyte;

	S->xloc = xloc;
	S->yloc = yloc;
	S->zloc = zloc;

	if (trajfilename != NULL)
	{
		S->trajfilename = (char *)mcalloc(E, 1, strlen(trajfilename)+1, "S->trajfilename in "SF_FILE_MACRO);
		if (S->trajfilename == nil)
		{
			mexit(E, "mcalloc failed for S->trajfilename in "SF_FILE_MACRO, -1);
		}
		strcpy(S->trajfilename, trajfilename);
	}

	S->NODE_ID = E->baseid + E->nnodes;


	/*	Must know correct number of nodes in resetcpu()		*/
	E->nnodes++;
	S->resetcpu(E, S);

	S->superH->nicintrQ = (InterruptQ *)mcalloc(E, 1, sizeof(InterruptQ),
		"InterruptQ *nicintrQ in superHnewstate()");
	if (S->superH->nicintrQ == NULL)
	{
		mexit(E, "Failed to allocate memory for InterruptQ *nicintrQ in superHnewstate().", -1);
	}

	S->superH->nicintrQ->hd = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt *S->superH->nicintrQ->hd in superHnewstate()");
	S->superH->nicintrQ->tl = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt *S->superH->nicintrQ->tl in superHnewstate()");
	if (S->superH->nicintrQ->hd == NULL || S->superH->nicintrQ->tl == NULL)
	{
		mexit(E, "Failed to allocate memory for S->superH->nicintrQ->hd | S->superH->nicintrQ->tl.", -1);
	}

	S->superH->excpQ = (InterruptQ *)mcalloc(E, 1, sizeof(InterruptQ),
		"InterruptQ *excpQ in superHnewstate()");
	if (S->superH->excpQ == NULL)
	{
		mexit(E, "Failed to allocate memory for InterruptQ *excpQ in superHnewstate().", -1);
	}

	S->superH->excpQ->hd = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt *S->superH->excpQ->hd in superHnewstate()");
	S->superH->excpQ->tl = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt *S->superH->excpQ->tl in superHnewstate()");
	if (S->superH->excpQ->hd == NULL || S->superH->excpQ->tl == NULL)
	{
		mexit(E, "Failed to allocate memory for S->superH->excpQ->hd | S->superH->excpQ->tl.", -1);
	}


	return S;
}

void
superHsplit(Engine *E, State *S, ulong startpc, ulong stackptr, ulong argaddr, char *idstr)
{
	int		i;
	Numaregion	*tmp;


	/*								*/
	/*	Split the current CPU into two, sharing the same mem	*/
	/*								*/
	State	*N = superHnewstate(E, S->xloc, S->yloc, S->zloc, S->trajfilename);

	mprint(E, NULL, siminfo,
		"Splitting node %d to new node %d:\n\t\tstartpc @ 0x" UHLONGFMT 
		", stack @ 0x" UHLONGFMT ", arg @ 0x" UHLONGFMT "\n\n",
		S->NODE_ID, N->NODE_ID, startpc, stackptr, argaddr);

	mfree(E, N->MEM, "N->MEM in superHsplit");
	mfree(E, N->superH->B, "N->superH->B in superHsplit");

	N->MEM = S->MEM;
	N->superH->B = S->superH->B;

	/*								*/
	/*	Make a copy of the entire Numaregion queue, and reset 	*/
	/*	all the counters on the copy to 0. This enables us to	*/
	/*	inherit all created numaregions, but still maintain 	*/
	/*	individual counters.					*/
	/*								*/
	for (i = 0; i < S->N->count; i++)
	{
		tmp = (Numaregion *) mcalloc(E, 1, sizeof(Numaregion),
			"N->N->regions entry in machine-hitachi-sh.c");
		if (tmp == NULL)
		{
			merror(E, "mcalloc failed for N->N->regions entry in machine-hitachi-sh.c");
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
			"N->Nstack->regions entry in machine-hitachi-sh.c");
		if (tmp == NULL)
		{
			merror(E, "mcalloc failed for N->Nstack->regions entry in machine-hitachi-sh.c");
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

	N->superH->R[15] = stackptr;
	N->superH->R[4] = argaddr;

	/*								*/
	/*	    Inherit the machine status (e.g., priv) too		*/
	/*	TODO: could memcpy the whole State *, but I haven't	*/
	/*	yet finalized how much heterogeneity in spawned 	*/
	/*	processors will exist					*/
	/*								*/
	N->superH->SR = S->superH->SR;
	N->superH->ENABLE_CLK_INTR = S->superH->ENABLE_CLK_INTR;
	N->superH->TIMER_INTR_DELAY = S->superH->TIMER_INTR_DELAY;

	strncpy(N->idstr, idstr, MAX_NAMELEN);
	N->PC = startpc;
	N->runnable = 1;
	E->cp = N;

	return;
}
