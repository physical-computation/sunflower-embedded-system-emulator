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
#include <string.h>
#include <float.h>
#include "sf.h"
#include "instr-hitachi-sh.h"
#include "endian-hitachi-sh.h"
#include "mextern.h"

int
superHtouchesmem(int op)
{
	switch (op)
	{
		case OP_ANDM:
		case OP_LDCMSR:
		case OP_LDCMGBR:
		case OP_LDCMVBR:
		case OP_LDCMSSR:
		case OP_LDCMSPC:
		case OP_LDCMRBANK:
		case OP_LDSMMACH:
		case OP_LDSMMACL:
		case OP_LDSMPR:
		case OP_MACL:
		case OP_MACW:
		case OP_MOVBL:
		case OP_MOVWL:
		case OP_MOVLL:
		case OP_MOVBP:
		case OP_MOVWP:
		case OP_MOVLP:
		case OP_MOVBL0:
		case OP_MOVWL0:
		case OP_MOVLL0:
		case OP_MOVWI:
		case OP_MOVLI:
		case OP_MOVBLG:
		case OP_MOVWLG:
		case OP_MOVLLG:
		case OP_MOVBL4:
		case OP_MOVWL4:
		case OP_MOVLL4:
		case OP_ORM:
		case OP_TAS:
		case OP_TSTM:
		case OP_XORM:

		case OP_MOVBS:
		case OP_MOVWS:
		case OP_MOVLS:
		case OP_MOVBM:
		case OP_MOVWM:
		case OP_MOVLM:
		case OP_MOVBS0:
		case OP_MOVWS0:
		case OP_MOVLS0:
		case OP_MOVBSG:
		case OP_MOVWSG:
		case OP_MOVLSG:
		case OP_MOVBS4:
		case OP_MOVWS4:
		case OP_MOVLS4:
		case OP_STCMSR:
		case OP_STCMGBR:
		case OP_STCMVBR:
		case OP_STCMSSR:
		case OP_STCMSPC:
		case OP_STCMRBANK:
		case OP_STSMMACH:
		case OP_STSMMACL:
		case OP_STSMPR:
		{
			return 1;
		}
	}	

	return 0;
}

int
superHEXtouchesmem(State *S)
{
	if (!S->superH->P.EX.valid)
	{
		return 0;
	}

	if (superHtouchesmem(S->superH->P.EX.op))
	{
		return 1;
	}

	switch (S->superH->P.EX.op)
	{
		case OP_BFS:
		case OP_BRA:
		case OP_BRAF:
		case OP_BSR:
		case OP_BSRF:
		case OP_BTS:
		case OP_JMP:
		case OP_JSR:
		case OP_RTE:
		case OP_RTS:
			if (superHtouchesmem(S->superH->P.ID.op))
			{
				return 1;
			}
	}

	return 0;
}

/*										*/
/*	faststep() does not emulate pipeline, doesn't perform any of the	*/
/*	safety checks performed by step(). 					*/
/*										*/
int
superHfaststep(Engine *E, State *S, int drain_pipeline)
{
	int		i, tmpinstr;
	ulong		tmpPC;
	Picosec		saved_globaltime;


	USED(drain_pipeline);

	saved_globaltime = E->globaltimepsec;
	for (i = 0; (i < E->quantum) && E->on && S->runnable; i++)
	{
		if (!eventready(E->globaltimepsec, S->TIME, S->CYCLETIME))
		{
			E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;
			continue;
		}

		if (superH_check_excp_macro(S))
		{
			superHtake_exception(E, S);
		}
		else if (SF_NETWORK && superH_check_nic_intr_macro(S))
		{
			S->take_nic_intr(E, S);
		}
		else if (eventready(E->globaltimepsec, S->superH->TIMER_LASTACTIVATE, S->superH->TIMER_INTR_DELAY))
		{
			/*								*/
			/*	Taking interrupt might fail if not interruptible, 	*/
			/*	so do not set LASTACTIVATE til it actually succeeds	*/
			/*								*/
			if (S->take_timer_intr(E, S) == 0)
			{
				S->superH->TIMER_LASTACTIVATE = E->globaltimepsec;
			}
		}

		if (S->sleep)
		{
			update_energy(OP_SLEEP, 0, 0);
			S->ICLK++;
			S->TIME += S->CYCLETIME;
			E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;

			continue;
		}

		tmpPC = S->PC;
		tmpinstr = superHreadword(E, S, S->PC);
		S->superH->P.EX.fptr 	= E->superHDC[tmpinstr].dc_p.fptr;
		S->superH->P.EX.format 	= E->superHDC[tmpinstr].dc_p.format;
		S->superH->P.EX.instr 	= tmpinstr;

		if (SF_POWER_ANALYSIS)
		{
			update_energy(E->superHDC[tmpinstr].dc_p.op, 0, 0);
		}

		S->superH->P.EX.fetchedpc = S->PC;
		S->PC += 2;
		S->CLK++;
		S->ICLK++;
		S->dyncnt++;
		S->TIME += S->CYCLETIME;

		switch (S->superH->P.EX.format)
		{
			case INSTR_0:
			{
				/*							*/
				/* 	Instruction may be delayed branch, so fill ID 	*/
				/*	for Delay_SLot()				*/
				/*							*/
				S->superH->P.ID.instr = superHreadword(E, S, S->PC);
				S->superH->P.ID.fetchedpc = S->PC;

				(*(S->superH->P.EX.fptr))(E, S);
				break;
			}

			case INSTR_N:
			{
				instr_n *tmp;

				/*							*/
				/* 	Instruction may be delayed branch, so fill ID 	*/
				/*	for Delay_SLot()				*/
				/*							*/
				S->superH->P.ID.instr = superHreadword(E, S, S->PC);
				S->superH->P.ID.fetchedpc = S->PC;

				tmp = (instr_n *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->dst);
				break;
			}

			case INSTR_M:
			{
				instr_m *tmp = (instr_m *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->src);
				break;
			}

			case INSTR_MBANK:
			{
				instr_mbank *tmp = (instr_mbank *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->reg, tmp->src);
				break;
			}

			case INSTR_NBANK:
			{
				instr_nbank *tmp = (instr_nbank *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->reg, tmp->dst);
				break;
			}

			case INSTR_NM:
			{
				instr_nm *tmp = (instr_nm *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->src, tmp->dst);
				break;
			}

			case INSTR_MD:
			{
				instr_md *tmp = (instr_md *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->src, tmp->disp);
				break;
			}

			case INSTR_NMD:
			{
				instr_nmd *tmp = (instr_nmd *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->src, tmp->disp, tmp->dst);
				break;
			}

			case INSTR_D8:
			{
				instr_d8 *tmp;

				/*							*/
				/* 	Instruction may be delayed branch, so fill ID 	*/
				/*	for Delay_SLot()				*/
				/*							*/
				S->superH->P.ID.instr = superHreadword(E, S, S->PC);
				S->superH->P.ID.fetchedpc = S->PC;

				tmp = (instr_d8 *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->disp);
				break;
			}

			case INSTR_D12:
			{
				instr_d12 *tmp;

				/*							*/
				/* 	Instruction may be delayed branch, so fill ID 	*/
				/*	for Delay_SLot()				*/
				/*							*/
				S->superH->P.ID.instr = superHreadword(E, S, S->PC);
				S->superH->P.ID.fetchedpc = S->PC;

				tmp = (instr_d12 *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->disp);
				break;
			}

			case INSTR_ND4:
			{
				instr_nd4 *tmp = (instr_nd4 *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->disp, tmp->dst);
				break;
			}

			case INSTR_ND8:
			{
				instr_nd8 *tmp = (instr_nd8 *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->disp, tmp->dst);
				break;
			}

			case INSTR_I:
			{
				instr_i *tmp = (instr_i *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->imm);
				break;
			}

			case INSTR_NI:
			{
				instr_ni *tmp = (instr_ni *)&S->superH->P.EX.instr;
				(*(S->superH->P.EX.fptr))(E, S, tmp->imm, tmp->dst);
				break;
			}

			default:
			{
				sfatal(E, S, "Unknown Instruction Type !!");
				break;
			}
		}

		if (SF_PAU_DEFINED && S->superH->PAUs != NULL)
		{
			pau_clk(E, S);
		}

		if (SF_BITFLIP_ANALYSIS)
		{
			S->Cycletrans += bit_flips_32(tmpPC, S->PC);	
			S->energyinfo.ntrans = S->energyinfo.ntrans + S->Cycletrans;
			S->Cycletrans = 0;
		}

		E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;
	}
	E->globaltimepsec = saved_globaltime;
	S->last_stepclks = i;

	return i;
}


int
superHstep(Engine *E, State *S, int drain_pipeline)
{
	int		i, exec_energy_updated = 0, stall_energy_updated = 0;
	ulong		tmpPC;
	Picosec		saved_globaltime;


	saved_globaltime = E->globaltimepsec;
	for (i = 0; (i < E->quantum) && E->on && S->runnable; i++)
	{
		/*								*/
		/*	TODO: This will have to change when we implement	*/
		/*	setjmp idea for simulating memory stalls		*/
		/*								*/
		if (	S->superH->B->pbuslock
			&& (S->superH->B->pbuslocker == S->NODE_ID)
			&& (S->superH->P.fetch_stall_cycles == 0)
			&& ((S->superH->P.MA.cycles == 0) || (S->superH->P.MA.valid == 0))
		)
		{
			S->superH->B->pbuslock = 0;
			S->superH->B->pbuslocker = -1;
		}

	    	if (!drain_pipeline)
	    	{
			if (!eventready(E->globaltimepsec, S->TIME, S->CYCLETIME))
			{
				E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;

				continue;
			}

			if ((S->superH->SR.BL == 1) && (S->superH->excpQ->nqintrs > 0))
			{
				sfatal(E, S, "Exceptions are blocked and we got an exception."
					"We don't handle this case correctly for synchronous exceptions!!!\n");
			}

			if (superH_check_excp_macro(S))
			{
				superHtake_exception(E, S);
			}
			else if (SF_NETWORK && superH_check_nic_intr_macro(S))
			{
				S->take_nic_intr(E, S);
			}
			else if (eventready(E->globaltimepsec, S->superH->TIMER_LASTACTIVATE,
					S->superH->TIMER_INTR_DELAY))
			{
				if (S->take_timer_intr(E, S) == 0)
				{
					S->superH->TIMER_LASTACTIVATE = E->globaltimepsec;
				}
			}

			if (S->sleep)
			{
				update_energy(OP_SLEEP, 0, 0);
				S->ICLK++;
				S->TIME += S->CYCLETIME;
				E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;

				continue;
			}
		}
	
		tmpPC = S->PC;

		/*								*/
		/* 	 		Clear WB stage				*/
	 	/*								*/
		S->superH->P.WB.valid = 0;
	

		/*								*/
		/*   MA cycles--. If 0, move instr in MA to WB if WB is empty	*/
		/*								*/
		if ((S->superH->P.MA.valid) && (S->superH->P.MA.cycles > 0))
		{
			S->superH->P.MA.cycles -= 1;
	
			/*							*/
			/*	For mem stall, energy cost assigned is NOP	*/
			/*							*/
			if (SF_POWER_ANALYSIS)
			{
				update_energy(OP_NOP, 0, 0);
				stall_energy_updated = 1;
			}
		}
	
		if ((S->superH->P.MA.valid) && (S->superH->P.MA.cycles == 0)
			&& (!S->superH->P.WB.valid))
		{
			/*		Count # bits flipping in WB		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->superH->P.MA.instr,
							S->superH->P.WB.instr);
			}
	
			memmove(&S->superH->P.WB, &S->superH->P.MA, sizeof(SuperHPipestage));
			S->superH->P.MA.valid = 0;
			S->superH->P.WB.valid = 1;
		}
	
	
		/*										*/
		/* 	 EX cycles--. If 0, exec, mark EX stage empty and move it to MA		*/
		/*										*/
		if ((S->superH->P.EX.valid) && (S->superH->P.EX.cycles > 0))
		{
			S->superH->P.EX.cycles -= 1;
	
			if (SF_POWER_ANALYSIS)
			{
				update_energy(S->superH->P.EX.op, 0, 0);
				exec_energy_updated = 1;
			}
		}
	
		if (S->superH->P.EX.valid && (S->superH->P.EX.fptr == NULL))
		{
			mprint(E, S, nodeinfo, "PC=0x" UHLONGFMT "\n",
				S->superH->P.EX.fetchedpc);
			mprint(E, S, nodeinfo, "S->superH->P.EX.instr = [0x%x]",
				S->superH->P.EX.instr);
			sfatal(E, S, "Illegal instruction.");
		}
	
		/*								*/
		/* 	Execution "completes" only if next stage is empty.	*/
		/*	Since we currently do mem acceses in EX, only go	*/
		/*	ahead if bus is not locked.				*/
		/*								*/
		/*	TODO: only instructions which would cause an access	*/
		/*	to main memory (non-cacheable / miss in cache) should	*/
		/*	wait on bus lock. This can be fixed cleanly, if we	*/
		/*	make actual memory access happen at the end of the	*/
		/*	stall, in the MA stage (like it should). To do this,	*/
		/*	would have to (1) mark pipestage as "memaccess-pending"	*/
		/*	or "re-exec-in-MA". But then how to deal with instrs	*/
		/*	for which the mem access is not the last step of 	*/
		/*	execution, e.g. TAS ?  The logically cleanest solution	*/
		/*	seems to use setjmp to save context as soon as a mem	*/
		/*	access happens, and then setting the CPU's stall cycles	*/
		/*	when stall cycles diminish to 0, do a longjmp to cont	*/
		/*	the simulation of that node...				*/
		/*								*/
		if (	(S->superH->P.EX.valid)
			&& (S->superH->P.EX.cycles == 0)
			&& !(S->superH->P.MA.valid)
			&& (!S->superH->B->pbuslock ||
				S->superH->B->pbuslocker == S->NODE_ID ||
				!superHEXtouchesmem(S)))
		{
			switch (S->superH->P.EX.format)
			{
				case INSTR_0:
				{
					(*(S->superH->P.EX.fptr))(E, S);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_N:
				{
					instr_n *tmp = (instr_n *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_M:
				{
					instr_m *tmp = (instr_m *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->src);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_MBANK:
				{
					instr_mbank *tmp = (instr_mbank *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->reg, tmp->src);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_NBANK:
				{
					instr_nbank *tmp = (instr_nbank *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->reg, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_NM:
				{
					instr_nm *tmp = (instr_nm *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->src, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_MD:
				{
					instr_md *tmp = (instr_md *)&S->superH->P.EX.instr;
	
						
					/*					*/
					/*   At this point, disp is relative	*/
					/*					*/
					(*(S->superH->P.EX.fptr))(E, S, tmp->src, tmp->disp);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_NMD:
				{
					instr_nmd *tmp = (instr_nmd *)&S->superH->P.EX.instr;
	
					/*					*/
					/*    At this point, disp is relative	*/
					/*					*/
					(*(S->superH->P.EX.fptr))(E, S, tmp->src, tmp->disp, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_D8:
				{
					instr_d8 *tmp = (instr_d8 *)&S->superH->P.EX.instr;
	
					/*					*/
					/*    At this point, disp is relative	*/
					/*					*/
					(*(S->superH->P.EX.fptr))(E, S, tmp->disp);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_D12:
				{
					instr_d12 *tmp = (instr_d12 *)&S->superH->P.EX.instr;
	
					/*					*/
					/*    At this point, disp is relative	*/
					/*					*/
					(*(S->superH->P.EX.fptr))(E, S, tmp->disp);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_ND4:
				{
					instr_nd4 *tmp = (instr_nd4 *)&S->superH->P.EX.instr;
	
					/*					*/
					/*    At this point, disp is relative	*/
					/*					*/
					(*(S->superH->P.EX.fptr))(E, S, tmp->disp, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_ND8:
				{
					instr_nd8 *tmp = (instr_nd8 *)&S->superH->P.EX.instr;

					/*					*/
					/*    At this point, disp is relative	*/
					/*					*/
					(*(S->superH->P.EX.fptr))(E, S, tmp->disp, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
	
				case INSTR_I:
				{
					instr_i *tmp = (instr_i *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->imm);
					S->dyncnt++;
	
					break;
				}
	
				case INSTR_NI:
				{
					instr_ni *tmp = (instr_ni *)&S->superH->P.EX.instr;
	
					(*(S->superH->P.EX.fptr))(E, S, tmp->imm, tmp->dst);
					S->dyncnt++;
	
					break;
				}
	
				default:
				{
					sfatal(E, S, "Unknown Instruction Type !!");
					break;
				}
			}
	
			/*		Count # bits flipping in MA		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->superH->P.EX.instr,
							S->superH->P.MA.instr);
			}
	
			memmove(&S->superH->P.MA, &S->superH->P.EX, sizeof(SuperHPipestage));
			S->superH->P.EX.valid = 0;
			S->superH->P.MA.valid = 1;
		}
	
		/*	     First : If fetch unit is stalled, dec its counter		*/
		if (S->superH->P.fetch_stall_cycles > 0)
		{
			/*								*/
			/*	Fetch Unit is stalled. Decrement time for it to wait.	*/
			/*	If we have not accounted for energy cost of stall 	*/
			/*	above (i.e. no stalled instr in MA), then cost of this	*/
			/*	cycle is calculated as cost of a NOP.			*/
			/*								*/
			S->superH->P.fetch_stall_cycles--;
	
			if (SF_POWER_ANALYSIS)
			{
				if (!stall_energy_updated && !exec_energy_updated)
				{
					update_energy(OP_NOP, 0, 0);
				}
			}
		}
	
		/*									*/
		/* 	move instr in ID stage to EX stage if EX stage is empty.	*/
		/*									*/
		if (	(S->superH->P.ID.valid)
			&& (S->superH->P.fetch_stall_cycles == 0)
			&& (!S->superH->P.EX.valid)
			&& (!S->superH->B->pbuslock ||
				S->superH->B->pbuslocker == S->NODE_ID ||
				!superHEXtouchesmem(S)))
		{
			/*		Count # bits flipping in EX		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->superH->P.ID.instr,
							S->superH->P.EX.instr);
			}
	
			/*	We use Decode Cache Rather than call decode()	*/
			S->superH->P.ID.fptr 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.fptr;
			S->superH->P.ID.op 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.op;
			S->superH->P.ID.format 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.format;
			S->superH->P.ID.cycles 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.cycles;
			memmove(&S->superH->P.EX, &S->superH->P.ID, sizeof(SuperHPipestage));
	
			S->superH->P.ID.valid = 0;
			S->superH->P.EX.valid = 1;
		}
	
	
		/*									*/
		/* 	    Move instr in IF stage to ID stage if ID stage is empty	*/
		/*									*/
		if (	!(S->superH->P.ID.valid)
			&& (S->superH->P.IF.valid)
			&& (S->superH->P.fetch_stall_cycles == 0))
		{
			/*		Count # bits flipping in ID		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->superH->P.ID.instr,
						S->superH->P.IF.instr);
			}
	
			memmove(&S->superH->P.ID, &S->superH->P.IF, sizeof(SuperHPipestage));
			S->superH->P.IF.valid = 0;
			S->superH->P.ID.valid = 1;
		}
	
		/*									*/
		/* 	  Put instr in IF stage if it is empty, and increment PC	*/
		/*	Check against bus lock is for the enclosing lonmgword address	*/
		/*									*/
		if (	!(S->superH->P.IF.valid)
			&& (!S->superH->B->pbuslock ||
				S->superH->B->pbuslocker == S->NODE_ID ||
				(S->PC & ~B0011) != S->superH->B->pbuslock_addr))
		{
			ushort	instrword;


			/*						*/
			/*	Get inst from mem hierarchy or fetch	*/
			/*	NOPs (used for draining pipeline).	*/
			/*						*/
			if (drain_pipeline)
			{
				instrword = 0x0009;
			}
			else
			{
				S->superH->mem_access_type = MEM_ACCESS_IFETCH;
				instrword = superHreadword(E, S, S->PC);
				S->superH->mem_access_type = MEM_ACCESS_NIL;
			}

			/*   Count # bits flipping in IF		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->superH->P.IF.instr, instrword);
			}
	
			S->superH->P.IF.instr = instrword;
			S->superH->P.IF.valid = 1;

			/*						*/
			/*	We also set this here (early) to	*/
			/*	enable intr/excp handling, since	*/
			/*	there, we do not drain the pipeline	*/
			/*	if instr in IF is of type which uses	*/
			/*	delay slot.				*/
			/*						*/	
			S->superH->P.IF.op = E->superHDC[(int)(instrword)].dc_p.op;

			if (!drain_pipeline)
			{
				S->superH->P.IF.fetchedpc = S->PC;
				S->PC += 2;
			}
		}
	
		S->CLK++;
		S->ICLK++;
		S->TIME += S->CYCLETIME;
	
		if (S->pipeshow)
		{
			superHdumppipe(E, S);
		}

		if (SF_PAU_DEFINED && S->superH->PAUs != NULL)
		{
			pau_clk(E, S);
		}

		if (SF_BITFLIP_ANALYSIS)
		{
			S->Cycletrans += bit_flips_32(tmpPC, S->PC);	
			S->energyinfo.ntrans = S->energyinfo.ntrans + S->Cycletrans;
			S->Cycletrans = 0;
		}

		E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;
	}
	E->globaltimepsec = saved_globaltime;


	return i;
}

void
superHpipeflush(State *S)
{
	/*								*/
	/*	Flush pipeline, count # bits we clear in pipe regs	*/
	/*								*/
	S->superH->P.IF.cycles = 0;
	S->superH->P.IF.valid = 0;

	S->superH->P.ID.cycles = 0;
	S->superH->P.ID.valid = 0;

	S->superH->P.EX.cycles = 0;
	S->superH->P.EX.valid = 0;

	S->superH->P.MA.cycles = 0;
	S->superH->P.MA.valid = 0;

	S->superH->P.WB.cycles = 0;
	S->superH->P.WB.valid = 0;

	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->superH->P.IF.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.ID.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.EX.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.MA.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.WB.instr, 0);
	}

	return;
}

void
superHifidflush(State *S)
{
	S->superH->P.IF.cycles = 0;
	S->superH->P.IF.valid = 0;

	S->superH->P.ID.cycles = 0;
	S->superH->P.ID.valid = 0;

	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->superH->P.IF.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.ID.instr, 0);
	}

	return;
}

void
superHdumppipe(Engine *E, State *S)
{
	mprint(E, S, nodeinfo, "\nnode ID=%d, PC=0x" UHLONGFMT ", ICLK=" UVLONGFMT ", sleep?=%d\n",
		S->NODE_ID, S->PC, S->ICLK, S->sleep);

	mprint(E, S, nodeinfo, "buslock=%d, buslocker=%d, EX touches mem = %d\n",
		S->superH->B->pbuslock, S->superH->B->pbuslocker, superHEXtouchesmem(S));

	if (S->superH->P.WB.valid)
	{
		mprint(E, S, nodeinfo, "WB: [%s],%d\n",
			opstrs[S->superH->P.WB.op], S->superH->P.WB.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "WB: []\n");
	}

	if (S->superH->P.MA.valid)
	{
		mprint(E, S, nodeinfo, "MA: [%s],%d\n",
			opstrs[S->superH->P.MA.op], S->superH->P.MA.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "MA: []\n");
	}

	if (S->superH->P.EX.valid)
	{
		mprint(E, S, nodeinfo, "EX: [%s],%d\n",
			opstrs[S->superH->P.EX.op], S->superH->P.EX.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "EX: []\n");
	}

	if (S->superH->P.ID.valid)
	{
		mprint(E, S, nodeinfo, "ID: [0x%x],%d\n",
			S->superH->P.ID.instr, S->superH->P.ID.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "ID: []\n");
	}

	if (S->superH->P.IF.valid)
	{
		mprint(E, S, nodeinfo, "IF: [0x%x],%d\n\n",
			S->superH->P.IF.instr, S->superH->P.IF.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "IF: []\n\n");
	}

	return;
}
