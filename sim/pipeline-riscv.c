/*
	Copyright (c)	2017-2018, Zhengyang Gu (author)
			2019, Samuel Wong (author)
 
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
#include "mextern.h"



char*
formatbininstr(uint32_t n, int halfbytespacing)
{
	int i;

	int length = 32 + (5 + halfbytespacing * 2) + 1;
	static char binaryNum[32 + 7 + 1];
	binaryNum[length-1] = '\0';

	for (i = length - 2; i >= 0; i--)
	{
		if (	(halfbytespacing && (i == 4 || i == 9 || i == 14 || i == 19 || i == 24 || i == 29 || i == 34)) ||
			(!halfbytespacing && (i == 7 || i == 13 || i == 19 || i == 23 || i == 29))
		)
		{
			binaryNum[i] = ' ';
		}
		else
		{
			binaryNum[i] = (char)((n % 2) + 48);
			n = n / 2;
		}
	}
	return binaryNum;
}

/*
Assumed pipeline implementation:
	Arithmetic instrs are forwarded:
		The output EX of a prev instr can be fed
		directly into EX of the next instr. This means
		dependent arithmetic instrs do not stall.
	LOAD instr are forwarded:
		The output MA of a prev instr can be fed
		directly into EX of the next instr. This means
		dependent instrs cause only 1 stall.
	BRANCH instr are tested in an earlier stage:
		The new PC calculation and register comparison is done by the
		end of the ID stage. This reduces cost of incorrect branches.
	BRANCH instr always take the wrong path:
		(Reduced cost branch) always causes 1 flush.
	JUMP locations are calculated earlier:
		JAL executes by end of ID stage, so causes 1 flush.
		JALR executes by end of EX stage. so causes 2 flushes.
	BRANCH instr always take the wrong path:
		(Reduced cost branch) always causes 1 flush.

Hazards:
	1 stall:
		LOAD instrs that write to a reg-required-by-next-instr after reading from mem:
			...EX of next instr needs reg data from MA of LOAD instrs.
		BRANCH instrs test for (in)equality in ID, dependent on previous instr:
			...ID of BRANCH instr needs reg data from EX of previous instr.
	2 stalls:
		LOAD instr followed by dependent BRANCH instr:
			...ID of BRANCH instr needs reg data from MA of LOAD instrs.
	1 flush:
		JAL instr calculates the PC of next instr using arithmetic in ID.
			...IF of next instr needs PC from ID of JAL instr.
		BRANCH instr guessed incorrectly, so need to flush:
			...IF of next instr needs PC from ID of BRANCH instr.
	2 flushes:
		JALR instr calculates the PC of next instr by using register.
			...IF of next instr needs reg data from EX of JALR instr.	
*/

int
riscvbranches(int op)
{
	switch(op)
	{
		case RISCV_OP_BEQ:
		case RISCV_OP_BNE:
		case RISCV_OP_BLT:
		case RISCV_OP_BGE:
		case RISCV_OP_BLTU:
		case RISCV_OP_BGEU:
		{
			return 1;
		}
	}
	return 0;
}

int
riscvchangespc(int op)
{
	/*	JALR instr only.	*/
	/*	JAL and BRANCH instr not included as they do nothing in EX	*/
	if (op == RISCV_OP_JALR/* || op == RISCV_OP_JAL || riscvbranches(op)*/)
	{
		return 1;
	}

	return 0;
}

int
riscvloads(int op)
{
	switch (op)
	{
		case RISCV_OP_LB:
		case RISCV_OP_LH:
		case RISCV_OP_LW:
		case RISCV_OP_LBU:
		case RISCV_OP_LHU:

		case RV32F_OP_FLW:
		case RV32D_OP_FLD:
		{
			return 1;
		}
	}

	return 0;
}

int
riscvreadsreg(int op)
{
	switch(op)
	{
		case RISCV_OP_JALR:
		case RISCV_OP_LB:
		case RISCV_OP_LH:
		case RISCV_OP_LW:
		case RISCV_OP_LBU:
		case RISCV_OP_LHU:
		case RISCV_OP_ADDI:
		case RISCV_OP_SLTI:
		case RISCV_OP_SLTIU:
		case RISCV_OP_XORI:
		case RISCV_OP_ORI:
		case RISCV_OP_ANDI:
		case RISCV_OP_SLLI:
		case RISCV_OP_SRLI:
		case RISCV_OP_SRAI:
		{
			return 1;
		}
		case RISCV_OP_BEQ:
		case RISCV_OP_BNE:
		case RISCV_OP_BLT:
		case RISCV_OP_BGE:
		case RISCV_OP_BLTU:
		case RISCV_OP_BGEU:
		case RISCV_OP_SB:
		case RISCV_OP_SH:
		case RISCV_OP_SW:
		case RISCV_OP_ADD:
		case RISCV_OP_SUB:
		case RISCV_OP_SLL:
		case RISCV_OP_SLT:
		case RISCV_OP_SLTU:
		case RISCV_OP_XOR:
		case RISCV_OP_SRL:
		case RISCV_OP_SRA:
		case RISCV_OP_OR:
		case RISCV_OP_AND:
		{
			return 2;
		}
	}
	return 0;
}

int
riscvsetsreg(int op)
{
	switch(op)
	{
		//	LUI and AUIPC: Info is there by end of ID so no need to stall for next instr
		//	JAL and JALR cause their own stall(s) anyway
		//	LOADs have their own stall tests
		case RISCV_OP_ADDI:
		case RISCV_OP_SLTI:
		case RISCV_OP_SLTIU:
		case RISCV_OP_XORI:
		case RISCV_OP_ORI:
		case RISCV_OP_ANDI:
		case RISCV_OP_SLLI:
		case RISCV_OP_SRLI:
		case RISCV_OP_SRAI:
		case RISCV_OP_ADD:
		case RISCV_OP_SUB:
		case RISCV_OP_SLL:
		case RISCV_OP_SLT:
		case RISCV_OP_SLTU:
		case RISCV_OP_XOR:
		case RISCV_OP_SRL:
		case RISCV_OP_SRA:
		case RISCV_OP_OR:
		case RISCV_OP_AND:
		{
			return 1;
		}
	}
	return 0;
}

int
riscvnumstalls(RiscvPipestage IDstage, RiscvPipestage IFstage)
{
	uint8_t IDrd	= (IDstage.instr&maskExtractBits7to11) >> 7;
	uint8_t IFrs1	= (IFstage.instr&maskExtractBits15to19) >> 15;
	uint8_t IFrs2	= (IFstage.instr&maskExtractBits20to24) >> 20;

	if (riscvloads(IDstage.op))
	{
		if (riscvbranches(IFstage.op))
		{
			if (IFrs1 == IDrd || IFrs2 == IDrd)
			{
				return 2;
			}
		}
		else if (riscvreadsreg(IFstage.op) == 1)
		{
			if (IFrs1 == IDrd)
			{
				return 1;
			}
		}
		else if (riscvreadsreg(IFstage.op) == 2)
		{
			if (IFrs1 == IDrd || IFrs2 == IDrd)
			{
				return 1;
			}
		}
	}
	if (riscvsetsreg(IDstage.op))
	{
		if (riscvbranches(IFstage.op))
		{
			if (IFrs1 == IDrd || IFrs2 == IDrd)
			{
				return 1;
			}
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
		/*	need to check for exceptions/interrupts here	*/

		tmpPC = S->PC;
		tmpinstr = riscVreadlong(E, S, S->PC);

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
	//S->superH->SR.MD = 1;

	saved_globaltime = E->globaltimepsec;
	for (i = 0; (i < E->quantum) && E->on && S->runnable; i++)
	{
		/*	superH multiprocessor equivalent has bus locking managment inserted here.	*/

		if (!drain_pipeline)
		{
			if (!eventready(E->globaltimepsec, S->TIME, S->CYCLETIME))
			{
				E->globaltimepsec = max(E->globaltimepsec, S->TIME) + S->CYCLETIME;
				continue;
			}

			/*	need to check for exceptions/interrupts here	*/

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
			S->riscv->P.MA.cycles--;
			if (S->riscv->P.MA.cycles != 0)
			{
				S->num_cycles_waiting++;
			}

			/*							*/
			/*	For mem stall, energy cost assigned is NOP	*/
			/*							*/

			if (SF_POWER_ANALYSIS)
			{
				update_energy(SUPERH_OP_NOP, 0, 0);/*	Power data is only available for superH	*/
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
			S->riscv->P.WB.cycles = S->riscv->P.WB.instr_latencies[WB];

			S->riscv->P.MA.valid = 0;
			S->riscv->P.WB.valid = 1;
		}


		/*										*/
		/* 	 EX cycles--. If 0, exec, mark EX stage empty and move it to MA		*/
		/*										*/
		if ((S->riscv->P.EX.valid) && (S->riscv->P.EX.cycles > 0))
		{
			S->riscv->P.EX.cycles--;
			if (S->riscv->P.EX.cycles != 0)
			{
				S->num_cycles_waiting++;
			}

			if (SF_POWER_ANALYSIS)
			{
				update_energy(S->riscv->P.EX.op, 0, 0);
				exec_energy_updated = 1;
			}

		}

		if (S->riscv->P.EX.valid && (S->riscv->P.EX.fptr == NULL))
		{
			mprint(E, S, nodeinfo, "PC=0x" UHLONGFMT "\n",
				S->riscv->P.EX.fetchedpc);
			mprint(E, S, nodeinfo, "S->riscv->P.EX.instr = [0x%x]",
				S->riscv->P.EX.instr);
			sfatal(E, S, "Illegal instruction.");
		}

		if (	(S->riscv->P.EX.valid)
			&& (S->riscv->P.EX.cycles == 0)
			&& !(S->riscv->P.MA.valid)
		)
		{
			/*	Rewind PC so that instrs that use PC have	*/
			/*	the correct PC. Will bring PC back after.	*/
			S->PC = S->riscv->P.EX.fetchedpc + 4;

			/*	Flushes next 2 instructions if jumping as they must be wrong	*/
			if (S->riscv->P.EX.op == RISCV_OP_JALR)
			{
				riscvIFIDflush(S);
			}

			switch (S->riscv->P.EX.format)
			{
				case INSTR_R:
				{
					uint32_t tmp = (uint32_t) S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S,
								(tmp&maskExtractBits15to19) >> 15,
								(tmp&maskExtractBits20to24) >> 20,
								(tmp&maskExtractBits7to11) >> 7);
					S->dyncnt++;

					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

					break;
				}

				case INSTR_I:
				{
					uint32_t tmp = (uint32_t) S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S,
								(tmp&maskExtractBits15to19) >> 15,
								(tmp&maskExtractBits7to11) >> 7,
								(tmp&maskExtractBits20to31) >> 20);
					S->dyncnt++;

					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

					break;
				}

				case INSTR_S:
				{
					uint32_t tmp = (uint32_t) S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S,
								(tmp&maskExtractBits15to19) >> 15,
								(tmp&maskExtractBits20to24) >> 20,
								(tmp&maskExtractBits7to11) >> 7,
								(tmp&maskExtractBits25to31) >> 25);
					S->dyncnt++;

					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

					break;
				}

				case INSTR_B:
				{
					/*	BRANCH executes in the ID stage. So do
						nothing, because it's already done in ID stage
					uint32_t tmp = (uint32_t) S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S,
								(tmp&maskExtractBits15to19) >> 15,
								(tmp&maskExtractBits20to24) >> 20,
								(tmp&maskExtractBits8to11) >> 8,
								(tmp&maskExtractBits25to30) >> 25,
								(tmp&maskExtractBit7) >> 7,
								(tmp&maskExtractBit31) >> 31);
					S->dyncnt++;				

					The implementation of the pipeline also affects the taint
					propagation statistics gathered in op-riscv (to find all
					instances hereof, use the find function to search for
					"instruction_taintDistribution"), be aware of this if
					changing the pipeline implementation.

					If branch instructions were included in the EX stage of the
					pipeline then the instruction below should be included:

					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

					*/
					break;
				}

				case INSTR_U:
				{
					uint32_t tmp = (uint32_t) S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S,
								(tmp&maskExtractBits7to11) >> 7,
								(tmp&maskExtractBits12to31) >> 12);
					S->dyncnt++;

					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

					break;
				}

				case INSTR_J:
				{
					/*	There is only one instruction of J-type, which is JAL,
						which does early PC calculation	in the ID stage.
						So do nothing, because it's already done in ID stage
					uint32_t tmp = S->riscv->P.EX.instr;
					(*(S->riscv->P.EX.fptr))(E, S,
								(tmp&maskExtractBits7to11) >> 7,
								(tmp&maskExtractBits21to30) >> 21,
								(tmp&maskExtractBit20) >> 20,
								(tmp&maskExtractBits12to19) >> 12,
								(tmp&maskExtractBit31) >> 31);
					S->dyncnt++;				*/
					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;
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
					(*(S->riscv->P.EX.fptr))(E, S);	/*	riscv_nop??	*/
					S->dyncnt++;

					S->riscv->instruction_distribution[S->riscv->P.EX.op]++;

					break;
				}

				default:
				{
					sfatal(E, S, "Unknown Instruction Type !!");
					break;
				}
			}

			/*	Set PC back to current, unless it has been changed	*/
			if (!riscvchangespc(S->riscv->P.EX.op))
			{
				S->PC = tmpPC;
			}

			/*		Count # bits flipping in MA		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.EX.instr,
							S->riscv->P.MA.instr);
			}

			memmove(&S->riscv->P.MA, &S->riscv->P.EX, sizeof(RiscvPipestage));
			S->riscv->P.MA.cycles = S->riscv->P.MA.instr_latencies[MA];

			S->riscv->P.EX.valid = 0;
			S->riscv->P.MA.valid = 1;
		}


		/*										*/
		/* 	 ID cycles--. If 0, mark ID stage empty and move it to EX		*/
		/*										*/
		if ((S->riscv->P.ID.valid) && (S->riscv->P.ID.cycles > 0))
		{
			S->riscv->P.ID.cycles--;
			if (S->riscv->P.ID.cycles != 0)
			{
				S->num_cycles_waiting++;
			}

			if (SF_POWER_ANALYSIS)
			{
				update_energy(S->riscv->P.ID.op, 0, 0);
				exec_energy_updated = 1;
			}

		}

		/*	First : If fetch unit is stalled, dec its counter			*/
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
			&& (S->riscv->P.ID.cycles == 0)
			&& (!S->riscv->P.EX.valid)
		)
		{
			/*	check if hazards need to stall next instuction (currently in IF)	*/
			S->riscv->P.IF.cycles += riscvnumstalls(S->riscv->P.ID, S->riscv->P.IF);

			/*	Executes early JUMP/BRANCH. Assumes next instr is always wrong.	*/
			if (S->riscv->P.ID.op == RISCV_OP_JAL || riscvbranches(S->riscv->P.ID.op))
			{
				S->PC = S->riscv->P.ID.fetchedpc + 4;/*	set PC back to when it was at JAL/BRANCH instr	*/

				if (S->riscv->P.ID.op == RISCV_OP_JAL)
				{
				uint32_t tmp = S->riscv->P.ID.instr;
				(*(S->riscv->P.ID.fptr))(E, S,
							(tmp&maskExtractBits7to11) >> 7,
							(tmp&maskExtractBits21to30) >> 21,
							(tmp&maskExtractBit20) >> 20,
							(tmp&maskExtractBits12to19) >> 12,
							(tmp&maskExtractBit31) >> 31);
				S->riscv->instruction_distribution[S->riscv->P.ID.op]++;
				}
				else
				{
				uint32_t tmp = (uint32_t) S->riscv->P.ID.instr;
				(*(S->riscv->P.ID.fptr))(E, S,
							(tmp&maskExtractBits15to19) >> 15,
							(tmp&maskExtractBits20to24) >> 20,
							(tmp&maskExtractBits8to11) >> 8,
							(tmp&maskExtractBits25to30) >> 25,
							(tmp&maskExtractBit7) >> 7,
							(tmp&maskExtractBit31) >> 31);
				S->riscv->instruction_distribution[S->riscv->P.ID.op]++;
				}
				S->dyncnt++;
				riscvIFflush(S);
			}

			/*		Count # bits flipping in EX		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.ID.instr,
							S->riscv->P.EX.instr);
			}

			memmove(&S->riscv->P.EX, &S->riscv->P.ID, sizeof(RiscvPipestage));
			S->riscv->P.EX.cycles = S->riscv->P.EX.instr_latencies[EX];

			S->riscv->P.ID.valid = 0;
			S->riscv->P.EX.valid = 1;
		}


		/*										*/
		/* 	 IF cycles--. If 0, exec, mark IF stage empty and move it to ID		*/
		/*										*/
		if ((S->riscv->P.IF.valid) && (S->riscv->P.IF.cycles > 0))
		{
			S->riscv->P.IF.cycles--;
			if (S->riscv->P.IF.cycles != 0)
			{
				S->num_cycles_waiting++;
			}

			if (SF_POWER_ANALYSIS)
			{
				update_energy(S->riscv->P.IF.op, 0, 0);
				exec_energy_updated = 1;
			}
		}

		/*									*/
		/* 	    Move instr in IF stage to ID stage if ID stage is empty	*/
		/*									*/
		if (	!(S->riscv->P.ID.valid)
			&& (S->riscv->P.IF.valid)
			&& (S->riscv->P.IF.cycles == 0)
			&& (S->riscv->P.fetch_stall_cycles == 0))
		{
			/*		Count # bits flipping in ID		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.ID.instr,
						S->riscv->P.IF.instr);
			}

			memmove(&S->riscv->P.ID, &S->riscv->P.IF, sizeof(RiscvPipestage));
			S->riscv->P.ID.cycles = S->riscv->P.ID.instr_latencies[ID];

			S->riscv->P.IF.valid = 0;
			S->riscv->P.ID.valid = 1;
		}

		/*									*/
		/* 	  Put instr in IF stage if it is empty, and increment PC	*/
		/*									*/
		if (	!(S->riscv->P.IF.valid)
		)
		{
			uint32_t	instrlong;

			/*						*/
			/*	Get inst from mem hierarchy or fetch	*/
			/*	NOPs (used for draining pipeline).	*/
			/*						*/
			if (drain_pipeline)
			{
				instrlong = 51;/*	should be 0000000 00000 00000 000 00000 0010011 for ADD x0,x0,x0	*/
			}
			else
			{
				S->riscv->mem_access_type = MEM_ACCESS_IFETCH;
				instrlong = riscVreadlong(E, S, S->PC);
				S->nfetched++;
				S->riscv->mem_access_type = MEM_ACCESS_NIL;
			}

			/*	Count # bits flipping in IF		*/
			if (SF_BITFLIP_ANALYSIS)
			{
				S->Cycletrans += bit_flips_32(S->riscv->P.IF.instr, instrlong);
			}

			S->riscv->P.IF.instr = instrlong;
			S->riscv->P.IF.valid = 1;

			riscvdecode(E, S->riscv->P.IF.instr, &S->riscv->P.IF);
			S->riscv->P.IF.cycles = S->riscv->P.IF.instr_latencies[IF];

			if (!drain_pipeline)
			{
				S->riscv->P.IF.fetchedpc = S->PC;
				S->PC += 4;
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
	mprint(E, S, nodeinfo, "\nnode ID=%d, PC=0x" UHLONGFMT ", ICLK=" UVLONGFMT ". \n",
			S->NODE_ID, S->PC, S->ICLK);
	if (S->riscv->P.WB.valid)
	{
		mprint(E, S, nodeinfo, "WB: [%s],\t[%s], pc: [0x%x], \n",
					riscv_opstrs[S->riscv->P.WB.op],
					formatbininstr(S->riscv->P.WB.instr,0),
					S->riscv->P.WB.fetchedpc);
	}
	else
	{
		mprint(E, S, nodeinfo, "WB: []\n");
	}

	if (S->riscv->P.MA.valid)
	{
		mprint(E, S, nodeinfo, "MA: [%s],\t[%s], pc: [0x%x] \n",
					riscv_opstrs[S->riscv->P.MA.op],
					formatbininstr(S->riscv->P.MA.instr,0),
					S->riscv->P.MA.fetchedpc);
	}
	else
	{
		mprint(E, S, nodeinfo, "MA: []\n");
	}

	if (S->riscv->P.EX.valid)
	{
		mprint(E, S, nodeinfo, "EX: [%s],\t[%s], pc: [0x%x], \n",
					riscv_opstrs[S->riscv->P.EX.op],
					formatbininstr(S->riscv->P.EX.instr,0),
					S->riscv->P.EX.fetchedpc);
	}
	else
	{
		mprint(E, S, nodeinfo, "EX: []\n");
	}

	if (S->riscv->P.ID.valid)
	{
		mprint(E, S, nodeinfo, "ID: [---],\t[%s], pc: [0x%x], \n",
					formatbininstr(S->riscv->P.ID.instr,0),
					S->riscv->P.ID.fetchedpc);
	}
	else
	{
		mprint(E, S, nodeinfo, "ID: []\n");
	}

	if (S->riscv->P.IF.valid)
	{
		mprint(E, S, nodeinfo, "IF: [---],\t[%s], pc: [0x%x], \n\n",
					formatbininstr(S->riscv->P.IF.instr,0),
					S->riscv->P.IF.fetchedpc);
	}
	else
	{
		mprint(E, S, nodeinfo, "IF: []\n\n");
	}

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

void
riscvflushpipe(State *S)
{
	/*								*/
	/*	Flush pipeline, count # bits we clear in pipe regs	*/
	/*								*/
	S->riscv->P.IF.cycles = 0;
	S->riscv->P.IF.valid = 0;


	S->riscv->P.ID.cycles = 0;
	S->riscv->P.ID.valid = 0;


	S->riscv->P.EX.cycles = 0;
	S->riscv->P.EX.valid = 0;


	S->riscv->P.MA.cycles = 0;
	S->riscv->P.MA.valid = 0;


	S->riscv->P.WB.cycles = 0;
	S->riscv->P.WB.valid = 0;


	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->riscv->P.IF.instr, 0);
		S->Cycletrans += bit_flips_32(S->riscv->P.ID.instr, 0);
		S->Cycletrans += bit_flips_32(S->riscv->P.EX.instr, 0);
		S->Cycletrans += bit_flips_32(S->riscv->P.MA.instr, 0);
		S->Cycletrans += bit_flips_32(S->riscv->P.WB.instr, 0);
	}

	return;
}

void
riscvIFflush(State *S)
{
	S->riscv->P.IF.cycles = 0;
	S->riscv->P.IF.valid = 0;

	S->num_cycles_waiting++;

	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->riscv->P.IF.instr, 0);
	}

	return;
}

void
riscvIFIDflush(State *S)
{
	S->riscv->P.IF.cycles = 0;
	S->riscv->P.IF.valid = 0;


	S->riscv->P.ID.cycles = 0;
	S->riscv->P.ID.valid = 0;

	S->num_cycles_waiting += 2;

	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->riscv->P.IF.instr, 0);
		S->Cycletrans += bit_flips_32(S->riscv->P.ID.instr, 0);
	}

	return;
}

