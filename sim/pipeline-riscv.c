/*
	Copyright (c) 2017-2018, Zhengyang Gu (author)
 
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
#include <string.h>
#include "sf.h"
#include "instr-riscv.h"
#include "opstr-riscv.h"
#include "mextern.h"

int
riscvtouchesmem(int op)
{
	switch (op)
	{
		case RISCV_OP_LB:
		case RISCV_OP_LH:
		case RISCV_OP_LW:
		case RISCV_OP_LBU:
		case RISCV_OP_LHU:
		case RISCV_OP_SB:
		case RISCV_OP_SH:
		case RISCV_OP_SW:
		{
			return 1;
		}
	}

	return 0;
}

int
riscvEXtouchesmem(State *S)
{
	if (!S->riscv->P.EX.valid)
	{
		return 0;
	}

	if (riscvtouchesmem(S->riscv->P.EX.op))
	{
		return 1;
	}

	switch (S->riscv->P.EX.op)
	{
		case RISCV_OP_LUI:
		case RISCV_OP_AUIPC:
		case RISCV_OP_JAL:
		case RISCV_OP_JALR:
		case RISCV_OP_BEQ:
		case RISCV_OP_BNE:
		case RISCV_OP_BLT:
		case RISCV_OP_BGE:
		case RISCV_OP_BLTU:
		case RISCV_OP_BGEU:
			if (superHtouchesmem(S->riscv->P.ID.op))
			{
				return 1;
			}
	}

	return 0;
}

int
riscvfaststep(Engine *E, State *S, int drain_pipeline)
{
	int		i;
	uint32_t	tmpinstr;
	uint32_t	tmpPC;
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
		/*	the superH equivalent checks for exceptions/interrupts,	*/
		/*	and whether it is in power-down (sleep) mode		*/

		tmpPC = S->PC;
		tmpinstr = superHreadlong(E, S, S->PC);

		riscvdecode(E, tmpinstr, &(S->riscv->P.EX));

		S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

		S->riscv->P.EX.fetchedpc = S->PC;
		S->PC += 4;
		S->CLK++;
		S->ICLK++;
		S->dyncnt++;
		S->TIME += S->CYCLETIME;

		switch (S->riscv->P.EX.format)
		{
			case INSTR_R:
			{
				instr_r *tmp;

				tmp = (instr_r *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rs1, tmp->rs2, tmp->rd);
				break;
			}

			case INSTR_I:
			{
				instr_i *tmp;

				tmp = (instr_i *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rs1, tmp->rd, tmp->imm0);
				break;
			}

			case INSTR_S:
			{
				instr_s *tmp;

				tmp = (instr_s *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rs1, tmp->rs2, tmp->imm0, tmp->imm5);
				break;
			}

			case INSTR_B:
			{
				instr_b *tmp;

				tmp = (instr_b *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rs1, tmp->rs2, tmp->imm1, tmp->imm5, tmp->imm11, tmp->imm12);
				break;
			}

			case INSTR_U:
			{
				instr_u *tmp;

				tmp = (instr_u *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rd, tmp->imm0);
				break;
			}

			case INSTR_J:
			{
				instr_j *tmp;

				tmp = (instr_j *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rd, tmp->imm1, tmp->imm11, tmp->imm12, tmp->imm20);
				break;
			}
			
			case INSTR_R4:
			{
				instr_r4 *tmp;

				tmp = (instr_r4 *)&S->riscv->P.EX.instr;
				(*(S->riscv->P.EX.fptr))(E, S, tmp->rs1, tmp->rs2, tmp->rs3, tmp->rm, tmp->rd);
				break;
			}

			case INSTR_N:
			{
				(*(S->riscv->P.EX.fptr))(E, S);
				break;
			}

			default:
			{
				sfatal(E, S, "Unknown Instruction Type !!");
				break;
			}
		}

		if (SF_BITFLIP_ANALYSIS)
		{
			S->Cycletrans += bit_flips_32(tmpPC, S->PC);	
			S->Cycletrans = 0;
		}
/*	Not needed for fast step?	*/
		if (S->pipeshow)
		{					
			riscvdumppipe(E, S);
		}
*/
		E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;
	}
	E->globaltimepsec = saved_globaltime;
	S->last_stepclks = i;

	return i;
}

int
riscvstep(Engine *E, State *S, int drain_pipeline)
{
	int		i, exec_energy_updated = 0, stall_energy_updated = 0;
	ulong		tmpPC;
	Picosec		saved_globaltime;


	saved_globaltime = E->globaltimepsec;
	for (i = 0; (i < E->quantum) && E->on && S->runnable; i++)
	{

		/*	superH equivalent has some sort of bus locking managment inserted here.	*/

		if (!drain_pipeline)
		{
			if (!eventready(E->globaltimepsec, S->TIME, S->CYCLETIME))
			{
				E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;
				continue;
			}

			/*	superH equivalent has interrupt and exception managment inserted here.	*/

		}

		tmpPC = S->PC;

		/*								*/
		/* 	 		Clear WB stage				*/
		/*								*/
		S->riscv->P.WB.valid = 0;


		/*								*/
		/*   MA cycles--. If 0, move instr in MA to WB if WB is empty	*/
		/*								*/
		if ((S->riscv->P.MA.valid) && (S->riscv->P.MA.cycles > 0))
		{
			S->riscv->P.MA.cycles -= 1;

			/*							*/
			/*	For mem stall, energy cost assigned is NOP	*/
			/*							*/

			if (SF_POWER_ANALYSIS)
			{
				update_energy(SUPERH_OP_NOP, 0, 0);
				stall_energy_updated = 1;
			}

		}

		if ((S->riscv->P.MA.valid) && (S->riscv->P.MA.cycles == 0)
			&& (!S->riscv->P.WB.valid))
		{
			/*		Count # bits flipping in WB		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.MA.instr,
							S->riscv->P.WB.instr);
			}

			memmove(&S->riscv->P.WB, &S->riscv->P.MA, sizeof(RiscvPipestage));
			S->riscv->P.MA.valid = 0;
			S->riscv->P.WB.valid = 1;
		}


		/*										*/
		/* 	 EX cycles--. If 0, exec, mark EX stage empty and move it to MA		*/
		/*										*/
		if ((S->riscv->P.EX.valid) && (S->riscv->P.EX.cycles > 0))
		{
			S->riscv->P.EX.cycles -= 1;
/*	Power analysis for riscv not implemented...?
			if (SF_POWER_ANALYSIS)
			{
				update_energy(S->riscv->P.EX.op, 0, 0);
				exec_energy_updated = 1;
			}
*/
		}

		if (S->riscv->P.EX.valid && (S->riscv->P.EX.fptr == NULL))
		{
			mprint(E, S, nodeinfo, "PC=0x" UHLONGFMT "\n",
				S->riscv->P.EX.fetchedpc);
			mprint(E, S, nodeinfo, "S->riscv->P.EX.instr = [0x%x]",
				S->riscv->P.EX.instr);
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
		if (	(S->riscv->P.EX.valid)
			&& (S->riscv->P.EX.cycles == 0)
			&& !(S->riscv->P.MA.valid)
			&& !superHEXtouchesmem(S)
		)
		{
			switch (S->riscv->P.EX.format)
			{
				case INSTR_N:
				{
					(*(S->riscv->P.EX.fptr))(E, S);	/*	riscv_nop?	*/
					S->dyncnt++;

					break;
				}

				case INSTR_R:
				{
					uint32_t tmp = (uint32_t *)&S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S, tmp&Bits15to19, tmp&Bits20to24, tmp&Bits7to11);
					S->dyncnt++;

					break;
				}

				case INSTR_I:
				{
					uint32_t tmp = (uint32_t *)&S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S, tmp&Bits15to19, tmp&Bits7to11,tmp&Bits20to31);
					S->dyncnt++;

					break;
				}

				case INSTR_S:
				{
					uint32_t tmp = (uint32_t *)&S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S, tmp&Bits15to19, tmp&Bits20to24, tmp&Bits7to11, tmp&Bits25to31);
					S->dyncnt++;

					break;
				}

				case INSTR_B:
				{
					uint32_t tmp = (uint32_t *)&S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S, tmp&Bits15to19, tmp&Bits20to24, tmp&Bits8to11, tmp&Bits25to30, tmp&Bit7, tmp&Bit31);
					S->dyncnt++;

					break;
				}

				case INSTR_U:
				{
					uint32_t tmp = (uint32_t *)&S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S, tmp&Bits7to11, tmp&Bits12to31);
					S->dyncnt++;

					break;
				}

				case INSTR_J:
				{
					uint32_t tmp = (uint32_t *)&S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S, tmp&Bits7to11, tmp&Bits21to30, tmp&Bit20, tmp&Bits12to19, tmp&Bit31);
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
				S->Cycletrans += bit_flips_32(S->riscv->P.EX.instr,
							S->riscv->P.MA.instr);
			}

			memmove(&S->riscv->P.MA, &S->riscv->P.EX, sizeof(RiscvPipestage));
			S->riscv->P.EX.valid = 0;
			S->riscv->P.MA.valid = 1;
		}

		/*	     First : If fetch unit is stalled, dec its counter		*/
		if (S->riscv->P.fetch_stall_cycles > 0)
		{
			/*								*/
			/*	Fetch Unit is stalled. Decrement time for it to wait.	*/
			/*	If we have not accounted for energy cost of stall 	*/
			/*	above (i.e. no stalled instr in MA), then cost of this	*/
			/*	cycle is calculated as cost of a NOP.			*/
			/*								*/
			S->riscv->P.fetch_stall_cycles--;

			if (SF_POWER_ANALYSIS)
			{
				if (!stall_energy_updated && !exec_energy_updated)
				{
					update_energy(SUPERH_OP_NOP, 0, 0);
				}
			}
		}

		/*									*/
		/* 	move instr in ID stage to EX stage if EX stage is empty.	*/
		/*									*/
		if (	(S->riscv->P.ID.valid)
			&& (S->riscv->P.fetch_stall_cycles == 0)
			&& (!S->riscv->P.EX.valid)
			&& !riscvEXtouchesmem(S)
		)
		{
			/*		Count # bits flipping in EX		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.ID.instr,
							S->riscv->P.EX.instr);
			}

			/*	We use Decode Cache Rather than call decode()	*/
			S->riscv->P.ID.fptr 	= E->riscvDC[(int)(S->riscv->P.ID.instr)].dc_p.fptr;
			S->riscv->P.ID.op 	= E->riscvDC[(int)(S->riscv->P.ID.instr)].dc_p.op;
			S->riscv->P.ID.format 	= E->riscvDC[(int)(S->riscv->P.ID.instr)].dc_p.format;
			S->riscv->P.ID.cycles 	= E->riscvDC[(int)(S->riscv->P.ID.instr)].dc_p.cycles;
			memmove(&S->riscv->P.EX, &S->riscv->P.ID, sizeof(RiscvPipestage));

			S->riscv->P.ID.valid = 0;
			S->riscv->P.EX.valid = 1;
		}

		/*									*/
		/* 	    Move instr in IF stage to ID stage if ID stage is empty	*/
		/*									*/
		if (	!(S->riscv->P.ID.valid)
			&& (S->riscv->P.IF.valid)
			&& (S->riscv->P.fetch_stall_cycles == 0))
		{
			/*		Count # bits flipping in ID		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.ID.instr,
						S->riscv->P.IF.instr);
			}

			memmove(&S->riscv->P.ID, &S->riscv->P.IF, sizeof(RiscvPipestage));
			S->riscv->P.IF.valid = 0;
			S->riscv->P.ID.valid = 1;
		}

		/*									*/
		/* 	  Put instr in IF stage if it is empty, and increment PC	*/
		/*	Check against bus lock is for the enclosing longword address	*/
		/*									*/
		if (	!(S->riscv->P.IF.valid)
		)
		{
			ushort	instrlong;

			/*						*/
			/*	Get inst from mem hierarchy or fetch	*/
			/*	NOPs (used for draining pipeline).	*/
			/*						*/
			if (drain_pipeline)
			{
				instrlong = 0x0009;/*	should be 0x00000000?	*/
			}
			else
			{
/*	Riscv Cache not implemented...
				S->riscv->mem_access_type = MEM_ACCESS_IFETCH;
				instrlong = superHreadword(E, S, S->PC);
				S->nfetched++;
				S->riscv->mem_access_type = MEM_ACCESS_NIL;
*/
				instrlong = S->MEM[S->PC - S->MEMBASE];
				S->nfetched++;
			}

			/*   Count # bits flipping in IF		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.IF.instr, instrlong);
			}

			S->riscv->P.IF.instr = instrlong;
			S->riscv->P.IF.valid = 1;

			/*						*/
			/*	We also set this here (early) to	*/
			/*	enable intr/excp handling, since	*/
			/*	there, we do not drain the pipeline	*/
			/*	if instr in IF is of type which uses	*/
			/*	delay slot.				*/
			/*						*/
			S->riscv->P.IF.op = E->riscvDC[(int)(instrlong)].dc_p.op;
			S->riscv->P.IF.op = E->riscvDC[(int)(instrlong)].dc_p.op;

			if (!drain_pipeline)
			{
				S->riscv->P.IF.fetchedpc = S->PC;
				S->PC += 2;
			}
		}

		S->CLK++;
		S->ICLK++;
		S->TIME += S->CYCLETIME;

		if (S->pipeshow)
		{
			riscvdumppipe(E, S);
		}
/*	Power Adaptation Unit not implemented...
		if (SF_PAU_DEFINED && S->riscv->PAUs != NULL)
		{
			pau_clk(E, S);
		}
*/
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
riscvdumppipe(Engine *E, State *S)
{
	mprint(E, S, nodeinfo, "\nnode ID=%d, PC=0x" UHLONGFMT ", ICLK=" UVLONGFMT ", sleep?=%d\n",
		S->NODE_ID, S->PC, S->ICLK, S->sleep);

	mprint(E, S, nodeinfo, "EX: [%s]\n", riscv_opstrs[S->riscv->P.EX.op]);

	return;
}

void
riscvdumpdistribution(Engine *E, State *S)
{
	for(int i = 0; i < RISCV_OP_MAX; i++) {
	    mprint(E, S, nodeinfo, "%-8s {%d}\n", riscv_opstrs[i], S->riscv->instruction_distribution[i]);
	}

	return;
}		
