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

/*									*/
/*	Implementation of the operations performed by the instrs.	*/
/*									*/
/*	References : SH7700 Series software programming manual, 	*/
/*	h1301.pdf, from www.halsp.hitachi.com.				*/
/*									*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sf.h"
#include "mmu-hitachi-sh.h"
#include "instr-hitachi-sh.h"
#include "endian-hitachi-sh.h"
#include "mextern.h"

static tuck void	delayslot(Engine *E, State *S, ulong dPC);

/*
*		TODO:
*			system registers are currently read
*	 		manually. Put them all in a sysreg_read,
*			and add bitflip analysis for that
*		OR (better)
*			integrate reading system registers, 
*			using S_R0 ... S_GBR etc to identify
*	TODO: We are not correctly taking care of preventing excepting instrs
*	from dirtying machine state: when trans.error is set, we get returned
*	0 on reads, which in some cases gets put into regs. Also, e.g., load w/ predecrement
*	leaves the machine state change if the load was excepting. Identify all potentially
*	excepting instructions and handle them correctly!
*/

/*									*/
/*			Remember, we are BIG ENDIAN			*/
/*									*/

/*		reg_set and reg_read, to get inlined			*/
#include "regaccess-hitachi-sh.c"

/*	We do this so that the superHreadword etc. can get inlined.		*/
#include "memory-hierarchy.c"


tuck void	
delayslot(Engine *E, State *S, ulong dPC)
{
	USED(dPC);

	/*								*/
	/* 	At this point in simulation, instruction in ID has 	*/
	/*	not yet been decoded. Decode it (not harmful to do so)	*/
	/*								*/
	S->superH->P.ID.fptr 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.fptr;
	S->superH->P.ID.op 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.op;
	S->superH->P.ID.format 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.format;
	S->superH->P.ID.cycles 	= E->superHDC[(int)(S->superH->P.ID.instr)].dc_p.cycles;

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
			sfatal(E, S, "Illegal slot instruction !\n");
			break;
		}

		/*								*/
		/*	See pages 107 and 115 of the programming manual		*/
		/*	for an explanation of the following:			*/
		/*								*/
		case SUPERH_OP_MOVI:
		case SUPERH_OP_MOVWI:
		case SUPERH_OP_MOVLI:
		case SUPERH_OP_MOVA:
		{
			/*							*/
			/*	When we call (*(S->superH->P.ID.fptr)), it will use	*/
			/*	P.EX.fetchedpc, and add 4 to calculate disp.	*/
			/*	For these instructions, in delay slot, we want	*/
			/*	to use PC of branch + 2, so we decrement the	*/
			/*	P.EX.fetchedpc by 2 before calling (*(fptr))	*/
			/*	then increment it by 2 after returning, so 	*/
			/*	the branch calculation is done correctly:	*/
			/*							*/

			S->superH->P.EX.fetchedpc -= 2;
			switch (S->superH->P.ID.format)
			{
				case INSTR_D8:
				{
					instr_d8 *tmp = (instr_d8 *)&S->superH->P.ID.instr;
					(*(S->superH->P.ID.fptr))(E, S, tmp->disp);

					break;
				}

				case INSTR_ND8:
				{
					instr_nd8 *tmp = (instr_nd8 *)&S->superH->P.ID.instr;
					(*(S->superH->P.ID.fptr))(E, S, tmp->disp, tmp->dst);
				
					break;
				}

				case INSTR_NI:
				{
					instr_ni *tmp = (instr_ni *)&S->superH->P.ID.instr;
					(*(S->superH->P.ID.fptr))(E, S, tmp->imm, tmp->dst);
				
					break;
				}

				default:
				{
					sfatal(E, S, "unknown instr fmt in delay slot !");
					break;
				}
			}
			S->superH->P.EX.fetchedpc += 2;

			break;
		}
	}


	/*	If not an illegal slot instruction, execute it		*/
	switch (S->superH->P.ID.format)
	{
		case INSTR_0:
		{
			(*(S->superH->P.ID.fptr))(E, S);
					
			break;
		}

		case INSTR_N:
		{
			instr_n *tmp = (instr_n *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->dst);
		
			break;
		}

		case INSTR_M:
		{
			instr_m *tmp = (instr_m *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->src);
		
			break;
		}

		case INSTR_MBANK:
		{
			instr_mbank *tmp = (instr_mbank *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->reg, tmp->src);
		
			break;
		}

		case INSTR_NBANK:
		{
			instr_nbank *tmp = (instr_nbank *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->reg, tmp->dst);

			break;
		}

		case INSTR_NM:
		{
			instr_nm *tmp = (instr_nm *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->src, tmp->dst);

			break;
		}

		case INSTR_MD:
		{
			instr_md *tmp = (instr_md *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->src, tmp->disp);

			break;
		}

		case INSTR_NMD:
		{
			instr_nmd *tmp = (instr_nmd *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->src, tmp->disp, tmp->dst);

			break;
		}

		case INSTR_D8:
		{
			instr_d8 *tmp = (instr_d8 *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->disp);

			break;
		}

		case INSTR_D12:
		{
			instr_d12 *tmp = (instr_d12 *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->disp);

			break;
		}

		case INSTR_ND4:
		{
			instr_nd4 *tmp = (instr_nd4 *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->disp, tmp->dst);

			break;
		}

		case INSTR_ND8:
		{
			instr_nd8 *tmp = (instr_nd8 *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->disp, tmp->dst);

			break;
		}


		case INSTR_I:
		{
			instr_i *tmp = (instr_i *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->imm);

			break;
		}

		case INSTR_NI:
		{
			instr_ni *tmp = (instr_ni *)&S->superH->P.ID.instr;
			(*(S->superH->P.ID.fptr))(E, S, tmp->imm, tmp->dst);

			break;
		}

		default:
		{
			sfatal(E, S, "unknown instruction format in delay slot !");
			break;
		}
	}

	return;
}

	
/*									*/
/*   ADD (Add Binary): Arithmetic Instruction				*/
/*									*/
/*   Format 		Abstract	Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   ADD Rm,Rn 		Rm+Rn -> Rn 	0011nnnnmmmm1100   1 	  -	*/
/*   ADD #imm,Rn 	Rn+#imm -> Rn 	0111nnnniiiiiiii   1 	  -	*/
/*									*/
/*   Description: Adds general register Rn data to Rm data, and		*/
/*   stores the result in Rn. 8-bit immediate data can be added 	*/
/*   instead of Rm data. Since the 8-bit immediate data is sign-	*/
/*   extended to 32 bits, this instruction can add and subtract 	*/
/*   immediate data.							*/
/*									*/
void
superH_add(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n)+reg_read(E, S, m));

	return;
}

void
superH_addi(Engine *E, State *S, long i, ulong n)
{
	if ((i&0x80)==0)
	{
		reg_set(E, S, n, reg_read(E, S, n)+(0x000000FF&(long)i));
	}
	else 
	{
		reg_set(E, S, n, reg_read(E, S, n)+(0xFFFFFF00 | (long)i));
	}

	return;
}


/*									*/
/*   ADDC (Add with Carry): Arithmetic Instruction			*/
/*									*/
/*   Format 	 Abstract 		Code 		 Cycle  T Bit	*/
/*   ---------------------------------------------------------------	*/
/*   ADDC Rm,Rn  Rn+Rm+T->Rn, carry->T  0011nnnnmmmm1110   1    Carry	*/
/*									*/
/*   Description: Adds general register Rm data and the T bit to 	*/
/*   Rn data, and stores the result in Rn. The T bit changes 		*/
/*   according to the result. This instruction can add data that 	*/
/*   has more than 32 bits.						*/
/*									*/
void
superH_addc(Engine *E, State *S, ulong m, ulong n)
{
	ulong tmp0, tmp1;

	tmp1 = reg_read(E, S, n)+reg_read(E, S, m);
	tmp0 = reg_read(E, S, n);
	reg_set(E, S, n, tmp1 + S->superH->SR.T);

	if (tmp0 > tmp1)
	{
		S->superH->SR.T = 1;
	}
	else 
	{
		S->superH->SR.T = 0;
	}
	if (tmp1 > reg_read(E, S, n))
	{
		S->superH->SR.T = 1;
	}

	return;
}


/*									*/
/*   ADDV (Add with V Flag Overflow Check): Arithmetic Instruction	*/
/*									*/
/*   Format 	 Abstract 		Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   ADDV Rm,Rn  Rn+Rm -> Rn, ovflw->T 	0011nnnnmmmm1111   1 	ovflw	*/
/*									*/
/*   Description: Adds general register Rn data to Rm data, and stores 	*/
/*   the result in Rn. If an overflow occurs, the T bit is set to 1.	*/
/*									*/
void
superH_addv(Engine *E, State *S, ulong m, ulong n)
{
	long dest, src, ans;

	if ((long)reg_read(E, S, n) >= 0)
	{
		dest = 0;
	}
	else
	{
		dest = 1;
	}

	if ((long)reg_read(E, S, m) >= 0)
	{
		src = 0;
	}
	else
	{
		src = 1;
	}

	src += dest;
	reg_set(E, S, n, reg_read(E, S, n)+reg_read(E, S, m));

	if ((long)reg_read(E, S, n) >= 0)
	{
		ans = 0;
	}
	else
	{
		ans = 1;
	}

	ans += dest;

	if (src == 0 || src == 2) 
	{
		if (ans == 1)
		{
			S->superH->SR.T = 1;
		}
		else
		{
			S->superH->SR.T = 0;
		}
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/
/*   AND (AND Logical): Logic Operation Instruction			*/
/*									*/
/*   Format 	          Abstract 	 Code 	     	   Cycle T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   AND   Rm,Rn          Rn&Rm -> Rn    0010nnnnmmmm1001    1 	  -	*/
/*   AND   #imm,R0        R0&imm -> R0   11001001iiiiiiii    1 	  -	*/
/*   AND.B #imm,@(R0,GBR) 						*/
/*	                  (R0+GBR)&imm->(R0 + GBR)			*/
/*					 11001101iiiiiiii    3 	   -	*/
/*									*/
/*   Description: Logically ANDs the contents of general registers Rn 	*/
/*   and Rm, and stores the result in Rn. The contents of general 	*/
/*   register R0 can be ANDed with zero-extended 8-bit immediate data.	*/
/*   8-bit memory data pointed to by GBR relative addressing can be 	*/
/*   ANDed with 8-bit immediate data. Note: After AND #imm, R0 is 	*/
/*   executed and the upper 24 bits of R0 are always cleared to 0.	*/
/*									*/
void
superH_and(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n)&reg_read(E, S, m));

	return;
}

void
superH_andi(Engine *E, State *S, long i)
{
	reg_set(E, S, 0, reg_read(E, S, 0)&(0x000000FF&(long)i));

	return;
}

void
superH_andm(Engine *E, State *S, long i)
{
	long temp;

	temp = (long)superHreadbyte(E, S, S->superH->GBR+reg_read(E, S, 0));
	temp &= (0x000000FF & (long)i);
	superHwritebyte(E, S, S->superH->GBR+reg_read(E, S, 0),temp);

	return;
}


/*									*/
/*   BF (Branch if False): Branch Instruction				*/
/*									*/
/*   Format 	Abstract 	       Code	 	  Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   BF label 	T==0 ? disp+PC -> PC;  10001011dddddddd     3 	   -	*/
/*		T==1 ? nop				    1	   -	*/
/*									*/
/*   Description: Reads T bit, and conditionally branches. If T = 1, 	*/
/*   BF executes the next instruction. If T = 0, it branches. The 	*/
/*   branch destination is an address specified by PC + displacement. 	*/
/*   The PC points to the starting address of the second instruction 	*/
/*   after the branch instruction. The 8-bit displacement is sign-	*/
/*   extended and doubled. Consequently, the relative interval from 	*/
/*   the branch destination is -256 to +254 bytes. If the displacement 	*/
/*   is too short to reach the branch destination, use BF with the BRA 	*/
/*   instruction or the like. Note: When branching, three cycles; when 	*/
/*   not branching, one cycle.						*/
/*									*/
void
superH_bf(Engine *E, State *S, long d)
{
	long disp;

	if ((d&0x80) == 0)
	{
		disp = (0x000000FF & (long)d);
	}
	else
	{
		disp = (0xFFFFFF00 | (long)d);
	}
	if (S->superH->SR.T == 0)
	{
		/*								*/
		/* 	(a) P.EX.fetchedpc : addr in mem of curr instr, since	*/
		/* 	PC has incremented twice since this instruction was 	*/
		/*	fetched. (b) We double disp, b'cos disp's are in 	*/
		/*	# of instrs, but PC increments by 2 b/n instructions,	*/
		/*	since PC is byte addr and instructions are 16 bit. 	*/
		/*	(c) Finally, +4, according to manual, PC of instr two	*/
		/*	instrs away is used to calculate target addr:		*/
		/*			BF					*/
		/*			NOP					*/
		/*			NOP <- this PC is used to calc targ	*/
		/*								*/
		S->PC = (S->superH->P.EX.fetchedpc+4)+(disp<<1);		
		superHIFIDflush(S);
		S->superH->P.EX.cycles += 2;
	}
	else
	{
		/*	Branch Not Taken	*/
	}

	return;
}


/*									*/
/*   BF/S (Branch if False with Delay Slot): Branch Instruction		*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 	      Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   BF label 	T==0 ?, disp+PC->PC;  10001111dddddddd	  2	  -	*/
/*		T==1 ?, nop				  1	  -	*/
/*									*/
/*   Description: Reads the T bit, and if T = 1, BF executes the next 	*/
/*   instruction. If T = 0, it branches after executing the next 	*/
/*   instruction. The branch destination is an address specified by	*/
/*   PC + displacement. The PC points to the starting address of the 	*/
/*   second instruction after the branch instruction. The 8-bit 	*/
/*   displacement is sign-extended and doubled. Consequently, the 	*/
/*   relative interval from the branch destination is -256 to +254 	*/
/*   bytes. If the displacement is too short to reach the branch 	*/
/*   destination, use BF with the BRA instruction or the like.		*/
/*   Note: The BF/S instruction is a conditional delayed branch 	*/
/*   instruction:							*/
/*   1. Taken case: The instruction immediately following is executed 	*/
/*   before the branch. Between the time this instruction and the 	*/
/*   instruction immediately following are executed, no interrupts are	*/
/*   accepted. When the instruction immediately following is a branch 	*/
/*   instruction, it is recognized as an illegal slot instruction.	*/
/*   2. Not taken case: This instruction operates as a nop instruction. */
/*   Between the time this instruction and the instruction immediately 	*/
/*   following are executed, interrupts are accepted. When the 		*/
/*   instruction immediately following is a branch instruction, it is 	*/
/*   not recognized as an illegal slot instruction.			*/
/*									*/
void
superH_bfs(Engine *E, State *S, long d)
{
	long disp;
	ulong temp;

	/*	B'cos PC has been inc. 2x since we were fetched		*/
	temp = (S->superH->P.EX.fetchedpc);

	if ((d&0x80) == 0)
	{
		disp = (0x000000FF & (long)d);
	}
	else
	{
		disp = (0xFFFFFF00 | (long)d);
	}
	if (S->superH->SR.T == 0) 
	{
		/* 	Taken : flush, exec delay slot instr, takes 2 cycles	*/
		/*								*/
		/* 	(a) P.EX.fetchedpc : addr in mem of curr instr, since	*/
		/* 	PC has incremented twice since this instruction was 	*/
		/*	fetched. (b) We double disp, b'cos disp's are in 	*/
		/*	# of instrs, but PC increments by 2 b/n instructions,	*/
		/*	since PC is byte addr and instructions are 16 bit. 	*/
		/*	(c) Finally, +4, according to manual, PC of instr two	*/
		/*	instrs away is used to calculate target addr:		*/
		/*			BF					*/
		/*			NOP					*/
		/*			NOP <- this PC is used to calc targ	*/
		/*								*/
		S->PC = (S->superH->P.EX.fetchedpc+4)+(disp<<1);
		
		/* 	Exec instr in delay slot before flushing IF/ID		*/
		delayslot(E, S, temp+2);
		superHIFIDflush(S);
	}
	else
	{
		/*	Not taken	*/
	}

	return;
}


/*									*/
/*   BRA (Branch): Branch Instruction					*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 	    Code 		Cycle 	T Bit 	*/
/*   ----------------------------------------------------------------	*/
/*   BRA label 	disp + PC -> PC     1010dddddddddddd 	  2 	  -	*/
/*									*/
/*   Description: Branches unconditionally after executing the 		*/
/*   instruction following this BRA instruction. The branch destination */
/*   is an address specified by PC + displacement. The PC points to the	*/
/*   starting address of the second instruction after this BRA 		*/
/*   instruction. The 12-bit displacement is sign-extended and doubled. */
/*   Consequently, the relative interval from the branch destination is */
/*   -4096 to +4094 bytes. If the displacement is too short to reach 	*/
/*   the branch destination, this instruction must be changed to the 	*/
/*   JMP instruction. Here, a MOV instruction must be used to transfer 	*/
/*   the destination address to a register. Note: Since this is a 	*/
/*   delayed branch instruction, the instruction after BRA is executed	*/
/*   before branching. No interrupts are accepted between this 		*/
/*   instruction and the next instruction. If the next instruction is 	*/
/*   a branch instruction, it is acknowledged as an illegal slot 	*/
/*   instruction.							*/
/*									*/
void
superH_bra(Engine *E, State *S, long d)
{
	ulong temp;
	long disp;

	if ((d&0x800)==0)
	{
		disp=(0x00000FFF & d);
	}
	else
	{
		disp=(0xFFFFF000 | d);
	}

	/*	B'cos PC has  been inc. 2x since we were fetched		*/
	temp = (S->superH->P.EX.fetchedpc);

	/*								*/
	/* 	(a) P.EX.fetchedpc : addr in mem of curr instr, since	*/
	/* 	PC has incremented twice since this instruction was 	*/
	/*	fetched. (b) We double disp, b'cos disp's are in 	*/
	/*	# of instrs, but PC increments by 2 b/n instructions,	*/
	/*	since PC is byte addr and instructions are 16 bit. 	*/
	/*	(c) Finally, +4, according to manual, PC of instr two	*/
	/*	instrs away is used to calculate target addr:		*/
	/*		BF						*/
	/*		NOP						*/
	/*		NOP <- this PC is used to calc targ		*/
	/*								*/
	S->PC = (S->superH->P.EX.fetchedpc+4)+(disp<<1);

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, temp+2);
	superHIFIDflush(S);


	return;
}


/*									*/
/*   BRAF (Branch Far): Branch Instruction				*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 	    Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   BRAF Rn 	Rn + PC -> PC 	    0000nnnn00100011 	  2 	  -	*/
/*									*/
/*   Description: Branches unconditionally. The branch destination is 	*/
/*   PC + the 32-bit contents of the general register Rn. PC is the 	*/
/*   start address of the second instruction after this instruction.	*/
/*   Note: Since this is a delayed branch instruction, the instruction 	*/
/*   after BRAF is executed before branching. No interrupts and address	*/
/*   errors are accepted between this instruction and the next 		*/
/*   instruction. If the next instruction is a branch instruction, it 	*/
/*   is acknowledged as an illegal slot instruction.			*/
/*									*/
void
superH_braf(Engine *E, State *S, ulong n)
{
	ulong temp;

	/*	B'cos PC has been inc. 2x since we were fetched		*/
	temp = (S->superH->P.EX.fetchedpc);

	/*	PC used in calculation is PC of 2nd instr after us.	*/
	S->PC += 4 + reg_read(E, S, n);

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, temp+2);
	superHIFIDflush(S);

		
	return;
}


/*									*/
/*   BSR (Branch to Subroutine): Branch Instruction			*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 		Code 		  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   BSR label 	PC->PR, disp+PC -> PC 	1011dddddddddddd    2 	   -	*/
/*									*/
/*   Description: Branches to the subroutine procedure at a specified 	*/
/*   address after executing the instruction following this BSR 	*/
/*   instruction. The PC value is stored in the PR, and the program	*/
/*   branches to an address specified by PC + displacement. The PC 	*/
/*   points to the starting address of the second instruction after 	*/
/*   this BSR instruction. The 12-bit displacement is sign-extended	*/
/*   and doubled. Consequently, the relative interval from the branch 	*/
/*   destination is -4096 to +4094 bytes. If the displacement is too 	*/
/*   short to reach the branch destination, the JSR instruction must 	*/
/*   be used instead. With JSR, the destination address must be 	*/
/*   transferred to a register by using the MOV instruction. This BSR 	*/
/*   instruction and the RTS instruction are used for a subroutine 	*/
/*   procedure call. Note: Since this is a delayed branch instruction, 	*/
/*   the instruction after BSR is executed before branching. No 	*/
/*   interrupts are accepted between this instruction and the next 	*/
/*   instruction. If the next instruction is a branch instruction, it 	*/
/*   is acknowledged as an illegal slot instruction.			*/
/* 									*/
void
superH_bsr(Engine *E, State *S, long d)
{
	long disp;

	if ((d&0x800) == 0)
	{
		disp = (0x00000FFF & d);
	}
	else
	{
		disp = (0xFFFFF000 | d);
	}

	/*	B'cos PC has been inc. 2x since we were fetched		*/
	S->superH->PR = (S->superH->P.EX.fetchedpc);

	S->PC = (S->superH->P.EX.fetchedpc+4)+(disp<<1);

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, S->superH->PR+2);
	superHIFIDflush(S);
	
	/*								*/
	/*		For simulators debugging facilities		*/
	/*								*/
	if (S->pcstackheight >= MAX_PCSTACK_HEIGHT)
	{
		sfatal(E, S, "Internal simulator error: PC stack overflow");
	}

	/*								*/
	/*	Current index of PCSTACK is the current function's	*/
	/*	start PC, whereas current index for frame pointer	*/
	/*	stack is not set until we're leaving function, b'cos	*/
	/*	we can't tell for sure where the frame pointer of	*/
	/*	functionw e're about to enter is going to be, since	*/
	/*	it depends on compiler generated function prologue.	*/
	/*								*/
	S->FPSTACK[S->fpstackheight++] = S->superH->R[14];
	S->PCSTACK[++S->pcstackheight] = S->PC;

	//fprintf(stdout, "NODE%d, BSR from " ULONGFMT " to " ULONGFMT ", S->CLK " UVLONGFMT "\n", S->NODE_ID, S->superH->PR&0xFFFFFFFF, S->PC&0xFFFFFFFF, S->ICLK&0xFFFFFFFF);

	return;
}


/*									*/
/*   BSRF (Branch to Subroutine Far): Branch Instruction		*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 		Code 		  Cycle	 T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   BSRF Rn 	PC->PR, Rn+PC -> PC 	0000nnnn00000011    2 	   -	*/
/*									*/
/*   Description: Branches to the subroutine procedure at a specified 	*/
/*   address after executing the instruction following this BSRF 	*/
/*   instruction. The PC value is stored in the PR. The branch		*/
/*   destination is PC + the 32-bit contents of the general register 	*/
/*   Rn. PC is the start address of the second instruction after this 	*/
/*   instruction. Used as a subroutine call in combination with RTS.	*/
/*   Note: Since this is a delayed branch instruction, the instruction 	*/
/*   after BSR is executed before branching. No interrupts are accepted */
/*   between this instruction and the next instruction. If the next	*/
/*   instruction is a branch instruction, it is acknowledged as an 	*/
/*   illegal slot instruction.						*/
/* 									*/
void
superH_bsrf(Engine *E, State *S, ulong n)
{
	/*	B'cos PC has been inc. 2x since we were fetched		*/
	S->superH->PR = S->superH->P.EX.fetchedpc;

	/*	PC used to calc dst addr is PC of 2nd instr after us	*/
	S->PC += 4 + reg_read(E, S, n);

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, S->superH->PR+2);
	superHIFIDflush(S);

	/*								*/
	/*		For simulators debugging facilities		*/
	/*								*/
	if (S->pcstackheight >= MAX_PCSTACK_HEIGHT)
	{
		sfatal(E, S, "Internal simulator error: PC stack overflow");
	}

	/*								*/
	/*	Current index of PCSTACK is the current function's	*/
	/*	start PC, whereas current index for frame pointer	*/
	/*	stack is not set until we're leaving function, b'cos	*/
	/*	we can't tell for sure where the frame pointer of	*/
	/*	functionw e're about to enter is going to be, since	*/
	/*	it depends on compiler generated function prologue.	*/
	/*								*/
	S->FPSTACK[S->fpstackheight++] = S->superH->R[14];
	S->PCSTACK[++S->pcstackheight] = S->PC;

	//fprintf(stdout, "NODE%d BSRF from " ULONGFMT " to " ULONGFMT ", S->CLK " UVLONGFMT "\n", S->NODE_ID, S->superH->PR&0xFFFFFFFF, S->PC&0xFFFFFFFF, S->ICLK&0xFFFFFFFF);

	return;
}


/*									*/
/*   BT (Branch if True): Branch Instruction				*/
/*									*/
/*   Format 	Abstract 		Code 		  Cycle   T Bit	*/
/*   ------------------------------------------------------------------	*/   			
/*   BT label 	T==1 ?, disp+PC -> PC;	10001001dddddddd   3	    -	*/
/*		T==0 ?, nop				   1		*/
/*									*/
/*   Description: Reads the T bit, and conditionally branches. If T = 1,*/
/*   BT branches. If T = 0, BT executes the next instruction. The 	*/
/*   branch destination is an address specified by PC + displacement. 	*/
/*   The PC points to the starting address of the second instruction 	*/
/*   after the branch instruction. The 8-bit displacement is sign-	*/
/*   extended and doubled. Consequently, the relative interval from 	*/
/*   the branch destination is -256 to +254 bytes. If the displacement 	*/
/*   is too short to reach the branch destination, use BT with the BRA 	*/
/*   instruction or the like. Note: When branching, requires three 	*/
/*   cycles; when not branching, one cycle.				*/
/* 									*/
void
superH_bt(Engine *E, State *S, long d)
{
	long disp;

	if ((d&0x80) == 0)
	{
		disp = (0x000000FF & (long)d);
	}
	else
	{
		disp = (0xFFFFFF00 | (long)d);
	}
	if (S->superH->SR.T == 1)
	{
		/*	Taken : flush IF and ID		*/
		S->PC = (S->superH->P.EX.fetchedpc+4)+(disp<<1);
		superHIFIDflush(S);
	}
	else
	{
		/*	Not taken	*/
	}

	return;
}


/*									*/
/*   BT/S (Branch if True with Delay Slot): Branch Instruction		*/
/*									*/
/*   Format 	Abstract 		Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   BT/S label T==1 ?, disp+PC -> PC;	10001101dddddddd   2	  -	*/
/*   		T==0 ?, nop				   1	  -	*/
/*   Description: Reads the T bit, and if T = 1, BT/S branches after 	*/
/*   the following instruction executes. If T = 0, BT/S executes the 	*/
/*   next instruction. The branch destination is an address specified	*/
/*   by PC + displacement. The PC points to the starting address of 	*/
/*   the second instruction after the branch instruction. The 8-bit 	*/
/*   displacement is sign-extended and doubled. Consequently, the 	*/
/*   relative interval from the branch destination is -256 to +254 	*/
/*   bytes. If the displacement is too short to reach the branch 	*/
/*   destination, use BT/S with the BRA instruction or the like. Note: 	*/
/*   The BF/S instruction is a conditional delayed branch instruction:	*/
/*   1. Taken case: The instruction immediately following is executed 	*/
/*   before the branch. Between the time this instruction and the 	*/
/*   instruction immediately following are executed, no interrupts	*/
/*   are accepted. When the instruction immediately following is a 	*/
/*   branch instruction, it is recognized as an illegal slot 		*/
/*   instruction. 2. Not taken case: This instruction operates as a 	*/
/*   nop instruction. Between the time this instruction and the 	*/
/*   instruction immediately following are executed, interrupts are 	*/
/*   accepted. When the instruction immediately following is a branch 	*/
/*   instruction, it is not recognized as an illegal slot instruction.	*/
/*									*/
void
superH_bts(Engine *E, State *S, long d)
{
	long disp;
	ulong temp;

	/*	B'cos PC has been inc. 2x since we were fetched		*/
	temp = S->superH->P.EX.fetchedpc;

	if ((d&0x80)==0)
	{
		disp = (0x000000FF & (long)d);
	}
	else
	{
		disp = (0xFFFFFF00 | (long)d);
	}
	if (S->superH->SR.T == 1) 
	{
		/*	Taken : flush, execute delay slot instr		*/
		S->PC = (S->superH->P.EX.fetchedpc+4)+(disp<<1);

		/* 	Exec instr in delay slot before flushing IF/ID		*/
		delayslot(E, S, temp+2);
		superHIFIDflush(S);
	}
	else
	{
		/*	Not taken	*/
	}

	return;
}


/*									*/
/*   CLRMAC (Clear MAC Register): System Control Instruction		*/
/*									*/
/*   Format 	Abstract 	    Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   CLRMAC 	0 -> MACH, MACL     0000000000101000 	  1 	  -	*/
/*									*/
/*   Description: Clears the MACH and MACL registers.			*/
/*									*/
void
superH_clrmac(Engine *E, State *S)
{
	S->superH->MACH = 0;
	S->superH->MACL = 0;

	return;
}


/*									*/
/*   CLRS (Clear S Bit): System Control Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   CLRS 	0 -> S 		0000000001001000 	  1 	  -	*/
/*									*/
/*   Description: Clears the S bit.					*/
/*									*/
void
superH_clrs(Engine *E, State *S)
{
	S->superH->SR.S = 0;

	return;
}


/*									*/
/*   CLRT (Clear T Bit): System Control Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   CLRT 	0 -> T 		0000000000001000 	  1       0	*/
/*									*/
/*   Description: Clears the T bit.					*/
/*									*/
void
superH_clrt(Engine *E, State *S)
{
	S->superH->SR.T = 0;

	return;
}


/*									*/
/*   CMP/cond (Compare Conditionally): Arithmetic Instruction		*/
/*									*/
/*   Format 	    Abstract 		 Code 		   Cycle T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   CMP/EQ Rm,Rn   when Rn == Rm, 1->T  0011nnnnmmmm0000    1   result	*/
/*   CMP/GE Rm,Rn   sgd && Rn>=Rm, 1->T	 0011nnnnmmmm0011    1 	 result	*/
/*   CMP/GT Rm,Rn   sgd && Rn>Rm, 1->T	 0011nnnnmmmm0111    1 	 result	*/
/*   CMP/HI Rm,Rn   usgd && Rn>Rm, 1->T  0011nnnnmmmm0110    1 	 result	*/
/*   CMP/HS Rm,Rn   usgd && Rn>=Rm, 1->T 0011nnnnmmmm0010    1 	 result	*/
/*   CMP/PL Rn 	    Rn>0 ?, 1->T 	 0100nnnn00010101    1 	 result	*/
/*   CMP/PZ Rn 	    Rn>=0 ?, 1->T 	 0100nnnn00010001    1 	 result	*/
/*   CMP/STR Rm,Rn  When a byte in Rn	 0010nnnnmmmm1100    1 	 result */
/*		    equals a byte in Rm, 				*/
/*		    1->T						*/
/*   CMP/EQ #imm,R0 R0==imm, 1->T 	 10001000iiiiiiii    1 	 result */
/*									*/
/*   Description: Compares general register Rn data with Rm data, and 	*/
/*   sets the T bit to 1 if a specified condition (cond) is satisfied. 	*/
/*   The T bit is cleared to 0 if the condition is not satisfied, and 	*/
/*   the Rn data does not change. The nine conditions in table 6.1 can 	*/
/*   be specified. Conditions PZ and PL are the results of comparisons 	*/
/*   between Rn and 0. Sign-extended 8-bit immediate data can also be	*/
/*   compared with R0 by using condition EQ. Here, R0 data does not 	*/
/*   change. Table 6.1 shows the mnemonics for the conditions.		*/
/* 									*/
/*   LEGEND ;)- sgd && = "when signed,  and" etc.			*/
/*									*/
void
superH_cmpeq(Engine *E, State *S, ulong m, ulong n)
{
	if (reg_read(E, S, n) == reg_read(E, S, m))
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}

void
superH_cmpge(Engine *E, State *S, ulong m, ulong n)
{
	if ((long)reg_read(E, S, n) >= (long)reg_read(E, S, m))
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}

void
superH_cmpgt(Engine *E, State *S, ulong m, ulong n)
{
	if ((long)reg_read(E, S, n) > (long)reg_read(E, S, m))
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}

void
superH_cmphi(Engine *E, State *S, ulong m, ulong n)
{
	if ((ulong)reg_read(E, S, n) > (ulong)reg_read(E, S, m))
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


void
superH_cmphs(Engine *E, State *S, ulong m, ulong n)
{
	if ((ulong)reg_read(E, S, n) >= (ulong)reg_read(E, S, m))
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}

void
superH_cmppl(Engine *E, State *S, ulong n)
{
	if ((long)reg_read(E, S, n) > 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


void
superH_cmppz(Engine *E, State *S, ulong n)
{
	if ((long)reg_read(E, S, n) >= 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


void
superH_cmpstr(Engine *E, State *S, ulong m, ulong n)
{
	ulong temp;
	long HH,HL,LH,LL;

	temp = reg_read(E, S, n) ^ reg_read(E, S, m);
	HH = (temp&0xFF000000) >> 12;
	HL = (temp&0x00FF0000) >> 8;
	LH = (temp&0x0000FF00) >> 4; 
	LL = temp&0x000000FF;
	HH = HH&&HL&&LH&&LL;

	if (HH == 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


void
superH_cmpim(Engine *E, State *S, long i)
{
	long imm;

	if ((i & 0x80) == 0)
	{
		imm = (0x000000FF & i);
	}
	else
	{
		imm = (0xFFFFFF00 | i);
	}
	if (reg_read(E, S, 0) == imm)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/
/*   DIV0S (Divide Step 0 as Signed): Arithmetic Instruction		*/
/* 									*/
/*   Format 		Abstract 	Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   DIV0S Rm,Rn 	MSB of Rn->Q, 	0010nnnnmmmm0111   1 	result	*/
/*			MSB of Rm->M, 					*/
/*			M^Q->T						*/
/*									*/
/*   Description: DIV0S is an initialization instruction for signed 	*/
/*   division. It finds the quotient by repeatedly dividing in 		*/
/*   combination with the DIV1 or another instruction that divides for 	*/
/*   each bit after this instruction. See the description given with 	*/
/*   DIV1 for more information.						*/
/* 									*/
void
superH_div0s(Engine *E, State *S, ulong m, ulong n)
{
	if ((reg_read(E, S, n)&0x80000000) == 0)
	{
		S->superH->SR.Q = 0;
	}
	else
	{
		S->superH->SR.Q = 1;
	}
	if ((reg_read(E, S, m)&0x80000000) == 0)
	{
		S->superH->SR.M = 0;
	}
	else
	{
		S->superH->SR.M = 1;
	}

	S->superH->SR.T = !(S->superH->SR.M == S->superH->SR.Q);


	return;
}


/*									*/
/*   DIV0U (Divide Step 0 as Unsigned): Arithmetic Instruction		*/
/*									*/
/*   Format 	Abstract 	Code		 	Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   DIV0U 	0 -> M/Q/T 	0000000000011001   	  1 	  0	*/
/*									*/
/*   Description: DIV0U is an initialization instruction for unsigned 	*/
/*   division. It finds the quotient by repeatedly dividing in 		*/
/*   combination with the DIV1 or another instruction that divides for 	*/
/*   each bit after this instruction. See the description given with 	*/
/*   DIV1 for more information.						*/
/* 									*/
void
superH_div0u(Engine *E, State *S)
{
	S->superH->SR.M = S->superH->SR.Q = S->superH->SR.T = 0;

	return;
}


/*									*/
/*   DIV1 (Divide Step 1): Arithmetic Instruction			*/
/*									*/
/*   Format	 Abstract 	      Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   DIV1 Rm,Rn  1 step div (Rn+Rm)   0011nnnnmmmm0100    1 	result	*/
/* 									*/
/*   Description: Uses single-step division to divide one bit of the 	*/
/*   32-bit data in general register Rn (dividend) by Rm data (divisor).*/
/*   It finds a quotient through repetition either independently or 	*/
/*   used in combination with other instructions. During this 		*/
/*   repetition, do not rewrite the specified register or the M, Q, 	*/
/*   and T bits. In one-step division, the dividend is shifted one bit	*/
/*   left, the divisor is subtracted and the quotient bit reflected in	*/
/*   the Q bit according to the status (positive or negative). Zero 	*/
/*   division, overflow detection, and remainder operation are not 	*/
/*   supported. Check for zero division and overflow division before 	*/
/*   dividing. Find the remainder by first finding the sum of the 	*/
/*   divisor and the quotient obtained and then subtracting it from 	*/
/*   the dividend. That is, first initialize with DIV0S or DIV0U. 	*/
/*   Repeat DIV1 for each bit of the divisor to obtain the quotient. 	*/
/*   When the quotient requires 17 or more bits, place ROTCL before 	*/
/*   DIV1. For the division sequence, see the following examples.	*/
/* 									*/
void
superH_div1(Engine *E, State *S, ulong m, ulong n)
{
	ulong tmp0;
	uchar old_q,tmp1;

	old_q = S->superH->SR.Q;
	S->superH->SR.Q = (uchar)((0x80000000 & reg_read(E, S, n))!=0);
	reg_set(E, S, n, reg_read(E, S, n) << 1);
	reg_set(E, S, n, reg_read(E, S, n)|(long)S->superH->SR.T);

	switch (old_q)
	{
		case 0:
			switch(S->superH->SR.M)
			{
				case 0:
					tmp0 = reg_read(E, S, n);
					reg_set(E, S, n, reg_read(E, S, n)-reg_read(E, S, m));
					tmp1 = (reg_read(E, S, n) > tmp0);
					switch(S->superH->SR.Q)
					{
						case 0:
							S->superH->SR.Q = tmp1;
							break;

						case 1:
							S->superH->SR.Q = (uchar)(tmp1 == 0);
							break;
					}
					break;

				case 1:
					tmp0 = reg_read(E, S, n);
					reg_set(E, S, n, reg_read(E, S, n)+reg_read(E, S, m));
					tmp1 = (reg_read(E, S, n) < tmp0);
					switch(S->superH->SR.Q)
					{
						case 0:
							S->superH->SR.Q = (uchar)(tmp1 == 0);
							break;
						case 1:
							S->superH->SR.Q = tmp1;
							break;
					}
					break;
			}
			break;

		case 1:
			switch(S->superH->SR.M)
			{
				case 0:
					tmp0 = reg_read(E, S, n);
					reg_set(E, S, n, reg_read(E, S, n)+reg_read(E, S, m));
					tmp1 = (reg_read(E, S, n) < tmp0);
					switch(S->superH->SR.Q)
					{
						case 0:
							S->superH->SR.Q=tmp1;
							break;
						case 1:
							S->superH->SR.Q = (uchar)(tmp1 == 0);
							break;
					}
					break;

				case 1:
					tmp0 = reg_read(E, S, n);
					reg_set(E, S, n, reg_read(E, S, n)-reg_read(E, S, m));
					tmp1 = (reg_read(E, S, n) > tmp0);
					switch(S->superH->SR.Q)
					{
						case 0:
							S->superH->SR.Q = (uchar)(tmp1 == 0);
							break;
						case 1:
							S->superH->SR.Q = tmp1;
							break;
					}
					break;
			}
			break;
	}

	S->superH->SR.T = (S->superH->SR.Q == S->superH->SR.M);

	return;
}


/*									*/
/*   DMULS.L (Double-Length Multiply as Signed): Arithmetic Instruction	*/
/*									*/
/*   Format 		Abstract 	Code 	  	  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   DMULS.L Rm,Rn 	with sign, 	0011nnnnmmmm1101   2-5	   -	*/
/*			Rn x Rm->MACH,MACL				*/
/* 									*/
/*   Description: Performs 32-bit multiplication of the contents of 	*/
/*   general registers Rn and Rm, and stores the 64-bit results in the 	*/
/*   MACL and MACH register. The operation is a signed arithmetic 	*/
/*   operation.								*/
/* 									*/
void
superH_dmuls(Engine *E, State *S, ulong m, ulong n)
{
	ulong RnL,RnH,RmL,RmH,Res0,Res1,Res2;
	ulong temp0,temp1,temp2,temp3;
	long tempm,tempn,fnLmL;

	tempn = (long)reg_read(E, S, n);
	tempm = (long)reg_read(E, S, m);

	if (tempn < 0)
	{
		tempn = 0-tempn;
	}
	if (tempm < 0)
	{
		tempm = 0-tempm;
	}
	if ((long)(reg_read(E, S, n)^reg_read(E, S, m)) < 0)
	{
		fnLmL = -1;
	}
	else
	{
		fnLmL = 0;
	}

	temp1 = (ulong)tempn;
	temp2 = (ulong)tempm;
	RnL = temp1 & 0x0000FFFF;
	RnH = (temp1 >> 16) & 0x0000FFFF;
	RmL = temp2 & 0x0000FFFF;
	RmH = (temp2 >> 16) & 0x0000FFFF;
	temp0 = RmL*RnL;
	temp1 = RmH*RnL;
	temp2 = RmL*RnH;
	temp3 = RmH*RnH;
	Res2 = 0;
	Res1 = temp1+temp2;

	if (Res1 < temp1)
	{
		Res2+=0x00010000;
	}

	temp1 = (Res1<<16)&0xFFFF0000;
	Res0 = temp0+temp1;

	if (Res0 < temp0)
	{
		Res2++;
	}

	Res2 = Res2 + ((Res1 >> 16) & 0x0000FFFF) + temp3;

	if (fnLmL < 0)
	{
		Res2 = ~Res2;
		if (Res0 == 0)
		{
			Res2++;
		}
		else
		{
			Res0=(~Res0)+1;
		}
	}

	S->superH->MACH = Res2;
	S->superH->MACL = Res0;

	return;
}


/*									*/
/*   DMULU.L (Double-Length Multiply as Unsigned): Arithmetic Instr.	*/
/*									*/
/*   Format 	    Abstract 		Code 		  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   DMULU.L Rm,Rn  Without sign, 	0011nnnnmmmm0101  2 - 5	   -	*/
/*		    Rn x Rm -> MACH,MACL				*/
/*									*/
/*   Description: Performs 32-bit multiplication of the contents of 	*/
/*   general registers Rn and Rm, and stores the 64-bit results in the 	*/
/*   MACL and MACH register. The operation is an unsigned arithmetic	*/
/*   operation.								*/
/* 									*/
void
superH_dmulu(Engine *E, State *S, ulong m, ulong n)
{
	ulong RnL, RnH, RmL, RmH, Res0, Res1, Res2;
	ulong temp0, temp1, temp2, temp3;

	RnL = reg_read(E, S, n) & 0x0000FFFF;
	RnH = (reg_read(E, S, n) >> 16) & 0x0000FFFF;
	RmL = reg_read(E, S, m) & 0x0000FFFF;
	RmH = (reg_read(E, S, m) >> 16) & 0x0000FFFF;
	temp0 = RmL*RnL;
	temp1 = RmH*RnL;
	temp2 = RmL*RnH;
	temp3 = RmH*RnH;
	Res2 = 0;
	Res1 = temp1+temp2;

	if (Res1 < temp1)
	{
		Res2 += 0x00010000;
	}

	temp1 = (Res1<<16)&0xFFFF0000;
	Res0 = temp0+temp1;

	if (Res0 < temp0)
	{
		Res2++;
	}

	Res2 = Res2+((Res1>>16)&0x0000FFFF)+temp3;
	S->superH->MACH = Res2;
	S->superH->MACL = Res0;

	return;
}



/*									*/
/*   DT (Decrement and Test): Arithmetic Instruction			*/
/*									*/
/*   Format 	Abstract 		Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   DT Rn 	Rn - 1 -> Rn;	0100nnnn00010000	1	result	*/
/*		Rn==0 ?, 1 -> T,					*/
/*		Rn!=0 ?, 0 -> T						*/
/*									*/
/*   Description: Decrements the contents of general register Rn by 1 	*/
/*   and compares the results to 0 (zero). When the result is 0, the T 	*/
/*   bit is set to 1. When the result isnt zero, the T bit is set to 0. */
/*									*/
void
superH_dt(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n)-1);

	if (reg_read(E, S, n) == 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/
/*   EXTS (Extend as Signed): Arithmetic Instruction			*/
/*									*/
/*   Format 	    Abstract	      Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   EXTS.B Rm,Rn   sig-ex Rm -> Rn   0110nnnnmmmm1110     1 	  -	*/
/*   EXTS.W Rm,Rn   sig-ex Rm -> Rn   0110nnnnmmmm1111 	   1 	  -	*/
/*									*/
/*   Description: Sign-extends general register Rm data, and stores 	*/
/*   the result in Rn. If byte length is specified, the bit 7 value of	*/
/*   Rm is copied into bits 8 to 31 of Rn. If word length is specified	*/
/*   , the bit 15 value of Rm is copied into bits 16 to 31 of Rn.	*/
/*									*/
void
superH_extsb(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, m));

	if ((reg_read(E, S, m) & 0x00000080) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n)&0x000000FF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n)|0xFFFFFF00);
	}

	return;
}

void
superH_extsw(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, m));

	if ((reg_read(E, S, m)&0x00008000) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n)&0x0000FFFF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n)|0xFFFF0000);
	}

	return;
}


/*									*/
/*   EXTU (Extend as Unsigned): Arithmetic Instruction			*/
/*									*/
/*   Format 	    Abstract 	     Code 		 Cycle  T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   EXTU.B Rm,Rn   z-ex Rm -> Rn    0110nnnnmmmm1100 	   1 	  -	*/
/*   EXTU.W Rm,Rn   z-ex Rm -> Rn    0110nnnnmmmm1101 	   1 	  - 	*/
/*									*/
/*   Description: Zero-extends general register Rm data, and stores 	*/
/*   the result in Rn. If byte length is specified, 0s are written in	*/
/*   bits 8 to 31 of Rn. If word length is specified, 0s are written 	*/
/*   in bits 16 to 31 of Rn.						*/
/* 									*/
void
superH_extub(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, m));
	reg_set(E, S, n, reg_read(E, S, n)&0x000000FF);

	return;
}

void
superH_extuw(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, m));
	reg_set(E, S, n, reg_read(E, S, n)&0x0000FFFF);

	return;
}


/*									*/	
/*   JMP (Jump): Branch Instruction					*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 		Abstract 	Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   JMP @Rn 		Rn -> PC 	0100nnnn00101011   2 	  -	*/
/*									*/
/*   Description: Branches unconditionally after executing the 		*/
/*   instruction following this JMP instruction. The branch destination	*/
/*   is an address specified by the 32-bit data in general register Rn.	*/
/*   Note: Since this is a delayed branch instruction, the instruction 	*/
/*   after JMP is executed before branching. No interrupts are accepted	*/
/*   between this instruction and the next instruction. If the next	*/
/*   instruction is a branch instruction, it is acknowledged as an 	*/
/*   illegal slot instruction.						*/
/* 									*/
void
superH_jmp(Engine *E, State *S, ulong n)
{
	ulong temp;

	/*	B'cos PC has been inc'd 2x since we were fetched	*/
	temp = S->superH->P.EX.fetchedpc;

	/*	XXX/BUG ? The spec does PC = reg_read(E, S, n)+4, but that makes no sense	*/
	S->PC = reg_read(E, S, n);

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, temp+2);
	superHIFIDflush(S);

	return;
}


/*									*/
/*   JSR (Jump to Subroutine): Branch Instruction			*/
/*   Class: Delayed branch instruction					*/
/* 									*/
/*   Format 	Abstract 	    Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*  JSR @Rn 	PC->PR, Rn->PC 	    0100nnnn00001011 	  2 	  -	*/
/*									*/
/*   Description: Branches to the subroutine procedure at a specified	*/
/*   address after executing the instruction following this JSR 	*/
/*   instruction. The PC value is stored in the PR. The jump 		*/
/*   destination is an address specified by the 32-bit data in general	*/
/*   register Rn. The PC points to the starting address of the second	*/
/*   instruction after JSR. The JSR instruction and RTS instruction are */
/*   used for subroutine procedure calls. Note: Since this is a delayed */
/*   branch instruction, the instruction after JSR is executed before	*/
/*   branching. No interrupts are accepted between this instruction 	*/
/*   and the next instruction. If the next instruction is a branch 	*/
/*   instruction, it is acknowledged as an illegal slot instruction.	*/
/* 									*/
void
superH_jsr(Engine *E, State *S, ulong n)
{
	/*	B'cos PC has been inc'd 2x since we were fetched	*/
	S->superH->PR = S->superH->P.EX.fetchedpc;

	/*	XXX/BUG ? The spec does PC = reg_read(E, S, n)+4, but that makes no sense	*/
	S->PC = reg_read(E, S, n);

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, S->superH->PR+2);
	superHIFIDflush(S);

	/*								*/
	/*		For simulators debugging facilities		*/
	/*								*/
	if (S->pcstackheight >= MAX_PCSTACK_HEIGHT)
	{
		sfatal(E, S, "Internal simulator error: PC stack overflow");
	}

	/*								*/
	/*	Current index of PCSTACK is the current function's	*/
	/*	start PC, whereas current index for frame pointer	*/
	/*	stack is not set until we're leaving function, b'cos	*/
	/*	we can't tell for sure where the frame pointer of	*/
	/*	functionw e're about to enter is going to be, since	*/
	/*	it depends on compiler generated function prologue.	*/
	/*								*/
	S->FPSTACK[S->fpstackheight++] = S->superH->R[14];
	S->PCSTACK[++S->pcstackheight] = S->PC;

	//fprintf(stdout, "NODE%d JSR from " ULONGFMT " to " ULONGFMT ", S->CLK " UVLONGFMT "\n", S->NODE_ID, S->superH->PR&0xFFFFFFFF, S->PC&0xFFFFFFFF, S->ICLK&0xFFFFFFFF);

	return;
}


/*									*/
/*   LDC (Load to Control Register): System Control Instruction		*/
/*   (Privileged Only)							*/
/*									*/
/*   Format 	     Abstract 		 Code 		   Cycle T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   LDC Rm,SR 	     Rm -> SR 		 0100mmmm00001110    5 	  LSB	*/
/*   LDC Rm,GBR      Rm -> GBR 		 0100mmmm00011110    1 	   -	*/
/*   LDC Rm,VBR      Rm -> VBR 		 0100mmmm00101110    1 	   -	*/
/*   LDC Rm,SSR      Rm -> SSR 		 0100mmmm00111110    1 	   -	*/
/*   LDC Rm,SPC      Rm -> SPC 		 0100mmmm01001110    1 	   -	*/
/*   LDC Rm,R0_BANK  Rm -> R0_BANK 	 0100mmmm10001110    1 	   -	*/
/*   LDC Rm,R1_BANK  Rm -> R1_BANK 	 0100mmmm10011110    1 	   -	*/
/*   LDC Rm,R2_BANK  Rm -> R2_BANK 	 0100mmmm10101110    1 	   -	*/
/*   LDC Rm,R3_BANK  Rm -> R3_BANK 	 0100mmmm10111110    1 	   -	*/
/*   LDC Rm,R4_BANK  Rm -> R4_BANK 	 0100mmmm11001110    1 	   -	*/
/*   LDC Rm,R5_BANK  Rm -> R5_BANK 	 0100mmmm11011110    1 	   -	*/
/*   LDC Rm,R6_BANK  Rm -> R6_BANK 	 0100mmmm11101110    1 	   -	*/
/*   LDC Rm,R7_BANK  Rm -> R7_BANK 	 0100mmmm11111110    1 	   -	*/
/*   LDC.L @Rm+,SR   (Rm)->SR, Rm+4->Rm  0100mmmm00000111    7 	  LSB	*/
/*   LDC.L @Rm+,GBR  (Rm)->GBR, Rm+4->Rm 0100mmmm00010111    1 	   -	*/
/*   LDC.L @Rm+,VBR  (Rm)->VBR, Rm+4->Rm 0100mmmm00100111    1 	   -	*/
/*   LDC.L @Rm+,SSR  (Rm)->SSR, Rm+4->Rm 0100mmmm00110111    1 	   -	*/
/*   LDC.L @Rm+,SPC  (Rm)->SPC, Rm+4->Rm 0100mmmm01000111    1 	   -	*/
/*   LDC.L @Rm+,R0_BANK (Rm) -> R0_BANK, 0100mmmm10000111    1	   -	*/
/* 		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R1_BANK (Rm) -> R1_BANK, 0100mmmm10010111    1	   -	*/
/*  		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R2_BANK (Rm) -> R2_BANK, 0100mmmm10100111    1	   -	*/
/*		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R3_BANK (Rm) -> R3_BANK, 0100mmmm10110111    1	   -	*/
/* 		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R4_BANK (Rm) -> R4_BANK, 0100mmmm11000111    1	   -	*/
/*  		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R5_BANK (Rm) -> R5_BANK, 0100mmmm11010111    1	   -	*/
/*		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R6_BANK (Rm) -> R6_BANK, 0100mmmm11100111    1	   -	*/
/* 		      Rm + 4 -> Rm					*/
/*   LDC.L @Rm+,R7_BANK (Rm) -> R7_BANK, 0100mmmm11110111    1	   -	*/
/*		      Rm + 4 -> Rm					*/
/*									*/
/*   Description: Stores source operand into control registers SR, GBR,	*/
/*   VBR, SSR, SPC, or R0_BANK to R7_BANK. LDC and LDC.L, except for 	*/
/*   LDC GBR, Rn and LDC.L GBR, @-Rn are privileged instructions and 	*/
/*   can be used in privileged mode only. If used in user mode, they	*/
/*   cause illegal instruction exceptions. LDC GBR, Rn and LDC.L GBR,	*/
/*   @-Rn can be used in user mode. Rn_BANK is designated by the RB 	*/
/*   bit of the SR. When the RB = 1, Rn_BANK0 is accessed by LDC/LDC.L 	*/
/*   instructions. When the RB = 0, Rn_BANK1 is accessed by LDC/LDC.L 	*/
/*   instructions.							*/
/* 									*/
void
superH_ldcsr(Engine *E, State *S, ulong m)
{
	long tmp = reg_read(E, S, m) & 0x700003F3;

	memmove(&S->superH->SR, &tmp, sizeof(S->superH->SR));
	
	return;
}

void
superH_ldcgbr(Engine *E, State *S, ulong m)
{
	S->superH->GBR = reg_read(E, S, m);

	return;
}

void
superH_ldcvbr(Engine *E, State *S, ulong m)
{
	S->superH->VBR = reg_read(E, S, m);

	return;
}

void
superH_ldcssr(Engine *E, State *S, ulong m)
{
	long 	tmp = reg_read(E, S, m) & 0x700003F3;
	memmove(&S->superH->SSR, &tmp, sizeof(S->superH->SSR));

	return;
}

void
superH_ldcspc(Engine *E, State *S, ulong m)
{
	S->superH->SPC = reg_read(E, S, m);

	return;
}

void
superH_ldcr_bank(Engine *E, State *S, int reg, ulong m)
{	
	if (S->superH->SR.RB == 0)
	{
		S->superH->R_BANK[reg] = S->superH->R[m];
	}
	else
	{
		S->superH->R[reg] = S->superH->R[m];
	}

	return;
}

void
superH_ldcmsr(Engine *E, State *S, ulong m)
{
	long tmp = superHreadlong(E, S, reg_read(E, S, m)) & 0x700003F3;
	memmove(&S->superH->SR, &tmp, sizeof(S->superH->SR));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldcmgbr(Engine *E, State *S, ulong m)
{
	S->superH->GBR = superHreadlong(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldcmvbr(Engine *E, State *S, ulong m)
{
	S->superH->VBR = superHreadlong(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldcmssr(Engine *E, State *S, ulong m)
{
	long	tmp = superHreadlong(E, S, reg_read(E, S, m)) & 0x700003F3;
	memmove(&S->superH->SSR, &tmp, sizeof(S->superH->SSR));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldcmspc(Engine *E, State *S, ulong m)
{
	S->superH->SPC = superHreadlong(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldcmr_bank(Engine *E, State *S, int reg, ulong m)
{
	if (S->superH->SR.RB == 0)
	{
		S->superH->R_BANK[reg] = superHreadlong(E, S, S->superH->R[m]);
	}
	else
	{
		S->superH->R[reg] = superHreadlong(E, S, S->superH->R[m]);
	}
	S->superH->R[m] += 4;

	return;
}


/*									*/
/*   LDS (Load to System Register): System Control Instruction		*/
/*									*/
/*   Format 	     Abstract 		  Code 		    Cycle T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   LDS Rm,MACH     Rm -> MACH 	  0100mmmm00001010    1     -	*/
/*   LDS Rm,MACL     Rm -> MACL 	  0100mmmm00011010    1     -	*/
/*   LDS Rm,PR 	     Rm -> PR 		  0100mmmm00101010    1     -	*/
/*   LDS.L @Rm+,MACH (Rm)->MACH, Rm+4->Rm 0100mmmm00000110    1     -	*/
/*   LDS.L @Rm+,MACL (Rm)->MACL, Rm+4->Rm 0100mmmm00010110    1     -	*/
/*   LDS.L @Rm+,PR   (Rm)->PR, Rm+4->Rm   0100mmmm00100110    1     -	*/
/*									*/
/*   Description: Stores the source operand into the system registers 	*/
/*   MACH, MACL, or PR.							*/
/*									*/
void
superH_ldsmach(Engine *E, State *S, ulong m)
{
	S->superH->MACH = reg_read(E, S, m);
	if ((S->superH->MACH&0x00000200) == 0)
	{
		S->superH->MACH &= 0x000003FF;
	}
	else
	{
		S->superH->MACH |= 0xFFFFFC00;
	}

	return;
}

void
superH_ldsmacl(Engine *E, State *S, ulong m)
{
	S->superH->MACL = reg_read(E, S, m);

	return;
}

void
superH_ldspr(Engine *E, State *S, ulong m)
{
	S->superH->PR = reg_read(E, S, m);

	return;
}

void
superH_ldsmmach(Engine *E, State *S, ulong m)
{
	S->superH->MACH = superHreadlong(E, S, reg_read(E, S, m));

	if ((S->superH->MACH&0x00000200) == 0)
	{
		S->superH->MACH &= 0x000003FF;
	}
	else
	{
		S->superH->MACH |= 0xFFFFFC00;
	}
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldsmmacl(Engine *E, State *S, ulong m)
{
	S->superH->MACL = superHreadlong(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}

void
superH_ldsmpr(Engine *E, State *S, ulong m)
{
	S->superH->PR = superHreadlong(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	return;
}


/*									*/
/*   LDTLB (Load PTEH/PTEL to TLB): System Control Instruction		*/
/*   (Privileged Only)							*/
/*									*/
/*   Format 	Abstract 		Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   LDTLB 	PTEH/PTEL -> TLB 	0000000000111000   1 	  -	*/
/*									*/
/*   Description: Loads PTEH/PTEL registers to the translation 		*/
/*   lookaside buffer (TLB). The TLB is indexed by the virtual address 	*/
/*   held in the PTEH register. The loaded set is designated by the	*/
/*   MMUCR.RC (MMUCR is an MMU control register and RC is a two bit 	*/
/*   field for a counter). LDTLB is a privileged instruction and can 	*/
/*   be used in privileged mode only. If used in user mode, it causes 	*/
/*   an illegal instruction exception. Note: As LDTLB is for loading 	*/
/*   PTEH and PTEL to the TLB, the instruction should be issued when 	*/
/*   MMU is off (MMUCR.AT = 0) or should be placed in the P1 or P2 	*/
/*   space with MMU enabled (see section 3, MMU, of the SH7700 Series 	*/
/*   Hardware Manual). If the instruction is issued in an exception 	*/
/*   handler, it should be at least two instructions prior to an RTE 	*/
/*   instruction that terminates the handler.				*/
/* 									*/
void
superH_ldtlb(Engine *E, State *S)
{
	int	i;


	ulong vpn31_17	= pteh_field_vpn31_17(S->superH->PTEH);
	uchar vpn11_10	= pteh_field_vpn11_10(S->superH->PTEH);
	uchar asid	= pteh_field_asid(S->superH->PTEH);
	int index	= pteh_field_index(S->superH->PTEH);
	ulong ppn	= ptel_field_ppn(S->superH->PTEL);
	uchar v		= ptel_field_v(S->superH->PTEL);
	uchar pr	= ptel_field_pr(S->superH->PTEL);
	uchar sz	= ptel_field_sz(S->superH->PTEL);
	uchar c		= ptel_field_c(S->superH->PTEL);
	uchar d		= ptel_field_d(S->superH->PTEL);
	uchar sh	= ptel_field_sh(S->superH->PTEL);


	/*	See Section 3.4.3 in rej09b0146_sh7706.pdf		*/
	if (mmucr_field_ix(S->superH->MMUCR))
	{
		index ^= asid;
	}

	i = index*S->superH->TLB->assoc + mmucr_field_rc(S->superH->MMUCR);

	S->superH->TLB->blocks[i].data = tlbentry_data_pack(ppn, pr, sz, c, d, sh);
	S->superH->TLB->blocks[i].tag = tlbentry_addr_pack(vpn31_17, vpn11_10, asid, v);

//	fprintf(stderr, "\n\nsunflower: ldtlb instr\n\n");
	return;
}


/*									*/
/*   MAC.L (Multiply and Accumulate Long): Arithmetic Instruction	*/
/*									*/
/*   Format 	      Abstract  	   Code 	    Cycle T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   MAC.L @Rm+,@Rn+  Signed operation,    0000nnnnmmmm1111  2-5    -	*/
/*		      (Rn)x(Rm)+MAC -> MAC				*/
/*									*/
/*   Description: Does signed multiplication of 32-bit operands 	*/
/*   obtained using the contents of general registers Rm and Rn as 	*/
/*   addresses. The 64-bit result is added to contents of the MAC 	*/
/*   register, and the final result is stored in the MAC register. 	*/
/*   Every time an operand is read, RM and Rn are incremented by four.	*/
/*   When the S bit is cleared to 0, the 64-bit result is stored in 	*/
/*   the coupled MACH and MACL registers. When bit S is set to 1, 	*/
/*   addition to the MAC register is a saturation operation of 48 bits	*/
/*   starting from the LSB. For the saturation operation, only the 	*/
/*   lower 48 bits of the MACL register are enabled and the result is 	*/
/*   limited to between H'FFFF800000000000 (minimum) and 		*/
/*   H'00007FFFFFFFFFFF (maximum).					*/
/*									*/
void
superH_macl(Engine *E, State *S, ulong m, ulong n)
{
	ulong RnL, RnH, RmL, RmH, Res0, Res1, Res2;
	ulong temp0, temp1, temp2, temp3;
	long tempm, tempn, fnLmL;

	tempn = (long)superHreadlong(E, S, reg_read(E, S, n));
	reg_set(E, S, n, reg_read(E, S, n)+4);
	tempm = (long)superHreadlong(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+4);

	if ((long)(tempn^tempm) < 0)
	{
		fnLmL = -1;
	}
	else
	{
		fnLmL = 0;
	}
	if (tempn < 0)
	{
		tempn = 0-tempn;
	}
	if (tempm < 0)
	{
		tempm = 0-tempm;
	}

	temp1 = (ulong)tempn;
	temp2 = (ulong)tempm;
	RnL = temp1&0x0000FFFF;
	RnH = (temp1>>16)&0x0000FFFF;
	RmL = temp2&0x0000FFFF;
	RmH = (temp2>>16)&0x0000FFFF;
	temp0 = RmL*RnL;
	temp1 = RmH*RnL;
	temp2 = RmL*RnH;
	temp3 = RmH*RnH;
	Res2 = 0;
	Res1 = temp1+temp2;

	if (Res1 < temp1)
	{
		Res2+=0x00010000;
	}

	temp1 = (Res1<<16)&0xFFFF0000;
	Res0 = temp0+temp1;

	if (Res0 < temp0)
	{
		Res2++;
	}

	Res2 = Res2+((Res1>>16)&0x0000FFFF)+temp3;

	if (fnLmL < 0)
	{
		Res2 =~ Res2;
		if (Res0 == 0)
		{
			Res2++;
		}
		else
		{
			Res0 = (~Res0)+1;
		}
	}
	if(S->superH->SR.S==1)
	{
		Res0 = S->superH->MACL+Res0;
		if (S->superH->MACL > Res0)
		{
			Res2++;
		}
		Res2 += (S->superH->MACH&0x0000FFFF);

		if(((long)Res2<0) && (Res2<0xFFFF8000))
		{
			Res2 = 0x00008000;
			Res0 = 0x00000000;
		}

		if(((long)Res2>0) && (Res2>0x00007FFF))
		{
			Res2 = 0x00007FFF;
			Res0 = 0xFFFFFFFF;
		}

		S->superH->MACH = Res2;
		S->superH->MACL = Res0;
	}
	else 
	{
		Res0 = S->superH->MACL+Res0;

		if (S->superH->MACL > Res0)
		{
			Res2++;
		}

		Res2 += S->superH->MACH;
		S->superH->MACH = Res2;
		S->superH->MACL = Res0;
	}

	return;
}


/*									*/
/*   MAC (Multiply and Accumulate): Arithmetic Instruction		*/
/*									*/
/*   Format 	      Abstract 	   	 Code 		   Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   MAC.W @Rm+,@Rn+  With sign, 	 0100nnnnmmmm1111   2-5	    -	*/
/*		      (Rn)x(Rm)+MAC->MAC				*/
/*									*/
/*   Description: Multiplies 16-bit operands obtained using the 	*/
/*   contents of general registers Rm and Rn as addresses. The 32-bit 	*/
/*   result is added to contents of the MAC register, and the final 	*/
/*   result is stored in the MAC register. When the S bit is cleared 	*/
/*   to 0, the 42-bit result is stored in the coupled MACH and MACL	*/
/*   registers. Bit 9 data is copied to the upper 22 bits (bits 31 to 	*/
/*   10) of the MACH register. Rm and Rn data are incremented by 2 	*/
/*   after the operation. When the bit S is set to 1, addition to the 	*/
/*   MAC register is a saturation operation. For the saturation 	*/
/*   operation, only the MACL register is enabled and the result is 	*/
/*   limited to between H'80000000 (minimum) and H'7FFFFFFF (maximum).	*/
/*   If an overflow occurs, the LSB of the MACH register is set to 1. 	*/
/*   The result is stored in the MACL register, and the result is 	*/
/*   limited to a value between H'80000000 (minimum) for overflows in 	*/
/*   the negative direction and H'7FFFFFFF (maximum) for overflows in 	*/
/*   the positive direction. Note: The normal number of cycles for 	*/
/*   execution is 3; however, succeeding instructions can be executed 	*/
/*   in two cycles.							*/
/* 									*/
void
superH_macw(Engine *E, State *S, ulong m, ulong n)
{
	long tempm,tempn,dest,src,ans;
	ulong templ;

	tempn = (long)superHreadword(E, S, reg_read(E, S, n));
	reg_set(E, S, n, reg_read(E, S, n)+2);
	tempm = (long)superHreadword(E, S, reg_read(E, S, m));
	reg_set(E, S, m, reg_read(E, S, m)+2);
	templ = S->superH->MACL;
	tempm = ((long)(short)tempn*(long)(short)tempm);

	if ((long)S->superH->MACL >= 0)
	{
		dest = 0;
	}
	else
	{
		dest = 1;
	}
	if ((long)tempm >= 0) 
	{
		src = 0;
		tempn = 0;
	}
	else 
	{
		src = 1;
		tempn = 0xFFFFFFFF;
	}
	src += dest;
	S->superH->MACL += tempm;
	if ((long)S->superH->MACL >= 0)
	{
		ans = 0;
	}
	else
	{
		ans = 1;
	}

	ans += dest;

	if (S->superH->SR.S == 1) 
	{
		if (ans == 1) 
		{
			if (src == 0 || src == 2)
			{
				S->superH->MACH |= 0x00000001;
			}
			if (src == 0)
			{
				S->superH->MACL = 0x7FFFFFFF;
			}
			if (src == 2)
			{
				S->superH->MACL = 0x80000000;
			}
		}
	}
	else 
	{
		S->superH->MACH += tempn;

		if (templ > S->superH->MACL)
		{
			S->superH->MACH += 1;
		}
		if ((S->superH->MACH & 0x00000200) == 0)
		{
			S->superH->MACH &= 0x000003FF;
		}
		else
		{
			S->superH->MACH |= 0xFFFFFC00;
		}
	}

	return;
}


/*									*/
/*   MOV (Move Data): Data Transfer Instruction				*/
/*									*/
/*   Format 	     Abstract 	          Code 		   Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   MOV Rm,Rn 	     Rm -> Rn 	          0110nnnnmmmm0011   1 	    -	*/
/*   MOV.B Rm,@Rn    Rm -> (Rn) 	  0010nnnnmmmm0000   1 	    -	*/
/*   MOV.W Rm,@Rn    Rm -> (Rn) 	  0010nnnnmmmm0001   1 	    - 	*/
/*   MOV.L Rm,@Rn    Rm -> (Rn) 	  0010nnnnmmmm0010   1 	    -	*/
/*   MOV.B @Rm,Rn    (Rm)->sigex->Rn 	  0110nnnnmmmm0000   1 	    -	*/
/*   MOV.W @Rm,Rn    (Rm)->sigex->Rn 	  0110nnnnmmmm0001   1	    -	*/
/*   MOV.L @Rm,Rn    (Rm)->Rn 		  0110nnnnmmmm0010   1 	    -	*/
/*   MOV.B Rm,@-Rn   Rn-1->Rn,Rm->(Rn)    0010nnnnmmmm0100   1 	    -	*/
/*   MOV.W Rm,@-Rn   Rn-2->Rn,Rm->(Rn) 	  0010nnnnmmmm0101   1 	    -	*/
/*   MOV.L Rm,@-Rn   Rn-4->Rn,Rm->(Rn) 	  0010nnnnmmmm0110   1	    -	*/ 
/*   MOV.B @Rm+,Rn   (Rm) -> sigex -> Rn, 0110nnnnmmmm0100   1	    -	*/
/*		     Rm + 1 -> Rm					*/
/*   MOV.W @Rm+,Rn   (Rm) -> sigex -> Rn, 0110nnnnmmmm0101   1	    -	*/
/* 		      Rm + 2 -> Rm					*/
/*   MOV.L @Rm+,Rn   (Rm)->Rn, Rm+4->Rm   0110nnnnmmmm0110   1 	    -	*/
/*   MOV.B Rm,@(R0,Rn) Rm -> (R0 + Rn) 	  0000nnnnmmmm0100   1	    -	*/
/*   MOV.W Rm,@(R0,Rn) Rm -> (R0 + Rn) 	  0000nnnnmmmm0101   1	    -	*/
/*   MOV.L Rm,@(R0,Rn) Rm -> (R0 + Rn) 	  0000nnnnmmmm0110   1	    -	*/
/*   MOV.B @(R0,Rm),Rn (R0+Rm)->sigex->Rn 0000nnnnmmmm1100   1	    -	*/
/*   MOV.W @(R0,Rm),Rn (R0+Rm)->sigex->Rn 0000nnnnmmmm1101   1	    -	*/
/*   MOV.L @(R0,Rm),Rn (R0+Rm)->Rn 	  0000nnnnmmmm1110   1	    -	*/
/*									*/
/*   Description: Transfers the source operand to the destination. When	*/
/*   the operand is stored in memory, the transferred data can be a 	*/
/*   byte, word, or longword. Loaded data from memory is stored in a 	*/
/*   register after it is sign-extended to a longword.			*/
/* 									*/
void
superH_mov(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, m));

	return;
}

void
superH_movbs(Engine *E, State *S, ulong m, ulong n)
{
	superHwritebyte(E, S, reg_read(E, S, n), reg_read(E, S, m));

	return;
}

void
superH_movws(Engine *E, State *S, ulong m, ulong n)
{
	superHwriteword(E, S, reg_read(E, S, n), reg_read(E, S, m));

	return;
}

void
superH_movls(Engine *E, State *S, ulong m, ulong n)
{
	superHwritelong(E, S, reg_read(E, S, n), reg_read(E, S, m));

	return;
}

void
superH_movbl(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, (long)superHreadbyte(E, S, reg_read(E, S, m)));

	if ((reg_read(E, S, n) & 0x80) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n)&0x000000FF);
	}
	else
 	{
		reg_set(E, S, n, reg_read(E, S, n)|0xFFFFFF00);
	}

	return;
}

void
superH_movwl(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, (long)superHreadword(E, S, reg_read(E, S, m)));

	if ((reg_read(E, S, n) & 0x8000) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n)&0x0000FFFF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n)|0xFFFF0000);
	}

	return;
}

void
superH_movll(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, superHreadlong(E, S, reg_read(E, S, m)));

	return;
}

void
superH_movbm(Engine *E, State *S, ulong m, ulong n)
{
	superHwritebyte(E, S, reg_read(E, S, n)-1, reg_read(E, S, m));
	reg_set(E, S, n, reg_read(E, S, n)-1);

	return;
}

void
superH_movwm(Engine *E, State *S, ulong m, ulong n)
{
	superHwriteword(E, S, reg_read(E, S, n)-2, reg_read(E, S, m));
	reg_set(E, S, n, reg_read(E, S, n)-2);

	return;
}

void
superH_movlm(Engine *E, State *S, ulong m, ulong n)
{
	superHwritelong(E, S, reg_read(E, S, n)-4, reg_read(E, S, m));
	reg_set(E, S, n, reg_read(E, S, n)-4);

	return;
}

void
superH_movbp(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n,  (long)superHreadbyte(E, S, reg_read(E, S, m)));

	if ((reg_read(E, S, n) & 0x80) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x000000FF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0xFFFFFF00);
	}
	if (n != m)
	{
		reg_set(E, S, m, reg_read(E, S, m) + 1);
	}

	return;
}

void
superH_movwp(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, (long)superHreadword(E, S, reg_read(E, S, m)));

	if ((reg_read(E, S, n) & 0x8000) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x0000FFFF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0xFFFF0000);
	}
	if (n != m)
	{
		reg_set(E, S, m, reg_read(E, S, m) + 2);
	}

	return;
}

void
superH_movlp(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, superHreadlong(E, S, reg_read(E, S, m)));

	if (n != m)
	{
		reg_set(E, S, m, reg_read(E, S, m) + 4);
	}

	return;
}

void
superH_movbs0(Engine *E, State *S, ulong m, ulong n)
{
	superHwritebyte(E, S, reg_read(E, S, n)+reg_read(E, S, 0), reg_read(E, S, m));

	return;
}

void
superH_movws0(Engine *E, State *S, ulong m, ulong n)
{
	superHwriteword(E, S, reg_read(E, S, n)+reg_read(E, S, 0), reg_read(E, S, m));

	return;
}

void
superH_movls0(Engine *E, State *S, ulong m, ulong n)
{
	superHwritelong(E, S, reg_read(E, S, n)+reg_read(E, S, 0), reg_read(E, S, m));

	return;
}

void
superH_movbl0(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, (long)superHreadbyte(E, S, reg_read(E, S, m)+reg_read(E, S, 0)));

	if ((reg_read(E, S, n) & 0x80) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x000000FF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0xFFFFFF00);
	}

	return;
}

void
superH_movwl0(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, (long)superHreadword(E, S, reg_read(E, S, m)+reg_read(E, S, 0)));

	if ((reg_read(E, S, n) & 0x8000) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x0000FFFF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0xFFFF0000);
	}

	return;
}

void
superH_movll0(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, superHreadlong(E, S, reg_read(E, S, m)+reg_read(E, S, 0)));

	return;
}


/*									*/
/*   MOV (Move Immediate Data): Data Transfer Instruction		*/
/*									*/
/*   Format 	         Abstract 	 Code 	    	  Cycle T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   MOV #imm,Rn         #imm->sign	 1110nnnniiiiiiii   1	  -	*/
/*		         extension->Rn					*/
/*   MOV.W @(disp,PC),Rn (disp+PC)->sign 1001nnnndddddddd   1	  -	*/
/*			  extension->Rn					*/
/*   MOV.L @(disp,PC),Rn (disp+PC)->Rn   1101nnnndddddddd   1	  -	*/
/*									*/
/*   Description: Stores immediate data, which has been sign-extended 	*/
/*   to a longword, into general register Rn. If the data is a word or 	*/
/*   longword, table data stored in the address specified by PC + 	*/
/*   displacement is accessed. If the data is a word, the 8-bit 	*/
/*   displacement is zero-extended and doubled. Consequently, the 	*/
/*   relative interval from the table is up to PC + 510 bytes. The PC 	*/
/*   points to the starting address of the second instruction after 	*/
/*   this MOV instruction. If the data is a longword, the 8-bit 	*/
/*   displacement is zero-extended and quadrupled. Consequently, the 	*/
/*   relative interval from the table is up to PC + 1020 bytes. The PC 	*/
/*   points to the starting address of the second instruction after 	*/
/*   this MOV instruction, but the lowest two bits of the PC are 	*/
/*   corrected to B'00. Note: The end address of the program area 	*/
/*   (module) or the second address after an unconditional branch 	*/
/*   instruction are suitable for the start address of the table. If 	*/
/*   suitable table assignment is impossible (for example, if there 	*/
/*   are no unconditional branch instructions within the area specified	*/
/*   by PC + 510 bytes or PC + 1020 bytes), the BRA instruction must 	*/
/*   be used to jump past the table. When this MOV instruction is 	*/
/*   placed immediately after a delayed branch instruction, the PC	*/
/*   points to an address specified by (the starting address of the	*/
/*   branch destination) + 2.						*/
/*									*/
void
superH_movi(Engine *E, State *S, long i, ulong n)
{
	if ((i & 0x80) == 0)
	{
		reg_set(E, S, n, (0x000000FF & (long)i));
	}
	else
	{
		reg_set(E, S, n, (0xFFFFFF00 | (long)i));
	}

	return;
}

void
superH_movwi(Engine *E, State *S, long d, ulong n)
{
	long disp;

	disp = (0x000000FF & (long)d);

	/*										*/
	/*	 PC used in calculation is PC of this instr+4 (see prog. manual).	*/
	/*										*/
	reg_set(E, S, n, (long)superHreadword(E, S, (S->superH->P.EX.fetchedpc+4)+(disp<<1)));

	if ((reg_read(E, S, n) & 0x8000) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x0000FFFF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0xFFFF0000);
	}

	return;
}

void
superH_movli(Engine *E, State *S, long d, ulong n)
{
	long disp;

	/*	Displacements are 8-bit		*/
	disp = (0x000000FF & (long)d);

	/*										*/
	/*	 PC used in calculation is PC of this instr+4 (see prog. manual).	*/						
	reg_set(E, S, n, superHreadlong(E, S, ((S->superH->P.EX.fetchedpc+4)&0xFFFFFFFC)+(disp<<2)));

	return;
}


/*									*/
/*   MOV (Move Peripheral Data): Data Transfer Instruction		*/
/*									*/
/*   Format 		  Abstract 	   Code	    	    Cycle T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   MOV.B @(disp,GBR),R0 (disp+GBR)->sign 11000100dddddddd   1	    -	*/
/*			  extension->R0					*/
/*   MOV.W @(disp,GBR),R0 (disp+GBR)->sign 11000101dddddddd   1     -	*/
/*		          extension->R0					*/
/*   MOV.L @(disp,GBR),R0 (disp+GBR)->R0   11000110dddddddd   1	    -	*/
/*   MOV.B R0,@(disp,GBR) R0->(disp+GBR)   11000000dddddddd   1	    -	*/
/*   MOV.W R0,@(disp,GBR) R0->(disp+GBR)   11000001dddddddd   1	    -	*/
/*   MOV.L R0,@(disp,GBR) R0->(disp+GBR)   11000010dddddddd   1	    -	*/
/*									*/
/*   Description: Transfers the source operand to the destination. This	*/
/*   instruction is suitable for accessing data in the peripheral 	*/
/*   module area. The data can be a byte, word, or longword, but only 	*/
/*   the R0 register can be used. A peripheral module base address is	*/
/*   set to the GBR. When the peripheral module data is a byte, the 	*/
/*   only change made is to zero-extend the 8-bit displacement. 	*/
/*   Consequently, an address within +255 bytes can be specified. When 	*/
/*   the peripheral module data is a word, the 8-bit displacement is	*/
/*   zero-extended and doubled. Consequently, an address within +510 	*/
/*   bytes can be specified. When the peripheral module data is a 	*/
/*   longword, the 8-bit displacement is zero-extended and is 		*/
/*   quadrupled. Consequently, an address within +1020 bytes can be 	*/
/*   specified. If the displacement is too short to reach the memory 	*/
/*   operand, the above @(R0,Rn) mode must be used after the GBR data 	*/
/*   is transferred to a general register. When the source operand is 	*/
/*   in memory, the loaded data is stored in the register after it is 	*/
/*   sign-extended to a longword. Note: The destination register of a 	*/
/*   data load is always R0. R0 cannot be accessed by the next		*/
/*   instruction until the load instruction is finished. The 		*/
/*   instruction order shown in figure 6.1 of programming manual will 	*/
/*   give better results.						*/
/*									*/
void
superH_movblg(Engine *E, State *S, long d)
{
	long disp;
	disp = (0x000000FF & (long)d);
	reg_set(E, S, 0, (long)superHreadbyte(E, S, S->superH->GBR+disp));

	if ((reg_read(E, S, 0) & 0x80) == 0)
	{
		reg_set(E, S, 0, reg_read(E, S, 0) & 0x000000FF);
	}
	else
	{
		reg_set(E, S, 0, reg_read(E, S, 0) | 0xFFFFFF00);
	}

	return;
}

void
superH_movwlg(Engine *E, State *S, long d)
{
	long disp;

	disp = (0x000000FF & (long)d);
	reg_set(E, S, 0, (long)superHreadword(E, S, S->superH->GBR+(disp<<1)));

	if ((reg_read(E, S, 0) & 0x8000) == 0)
	{
		reg_set(E, S, 0, reg_read(E, S, 0) & 0x0000FFFF);
	}
	else
	{
		reg_set(E, S, 0, reg_read(E, S, 0) | 0xFFFF0000);
	}

	return;
}

void
superH_movllg(Engine *E, State *S, long d)
{
	long disp;

	disp = (0x000000FF & (long)d);
	reg_set(E, S, 0, superHreadlong(E, S, S->superH->GBR+(disp<<2)));

	return;
}

void
superH_movbsg(Engine *E, State *S, long d)
{
	long disp;

	disp = (0x000000FF & (long)d);
	superHwritebyte(E, S, S->superH->GBR+disp,reg_read(E, S, 0));

	return;
}

void
superH_movwsg(Engine *E, State *S, long d)
{
	long disp;

	disp = (0x000000FF & (long)d);
	superHwriteword(E, S, S->superH->GBR+(disp<<1),reg_read(E, S, 0));

	return;
}

void
superH_movlsg(Engine *E, State *S, long d)
{
	long disp;

	disp = (0x000000FF & (long)d);
	superHwritelong(E, S, S->superH->GBR+(disp<<2),reg_read(E, S, 0));

	return;
}


/*									*/
/*   MOV (Move Structure Data): Data Transfer Instruction		*/
/*									*/
/*   Format 		  Abstract 	 Code 		   Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   MOV.B R0,@(disp,Rn)  R0->(disp+Rn)   10000000nnnndddd    1    -	*/
/*   MOV.W R0,@(disp,Rn)  R0->(disp+Rn)   10000001nnnndddd    1    -	*/
/*   MOV.L Rm,@(disp,Rn)  Rm->(disp+Rn)   0001nnnnmmmmdddd    1	   -	*/
/*   MOV.B @(disp,Rm),R0  (disp+Rm)->sign 10000100mmmmdddd    1	   -	*/
/* 			  extension -> R0				*/
/*   MOV.W @(disp,Rm),R0  (disp+Rm)->sign 10000101mmmmdddd    1	   -	*/
/*			  extension -> R0				*/
/*   MOV.L @(disp,Rm),Rn  (disp+Rm)->Rn   0101nnnnmmmmdddd    1	   -	*/
/*									*/
/*   Description: Transfers the source operand to the destination. This	*/
/*   instruction is suitable for accessing data in a structure or a 	*/
/*   stack. The data can be a byte, word, or longword, but when a byte	*/
/*   or word is selected, only the R0 register can be used. When the 	*/
/*   data is a byte, the only change made is to zero-extend the 4-bit 	*/
/*   displacement. Consequently, an address within +15 bytes can be	*/
/*   specified. When the data is a word, the 4-bit displacement is 	*/
/*   zero-extended and doubled. Consequently, an address within +30 	*/
/*   bytes can be specified. When the data is a longword, the 4-bit 	*/
/*   displacement is zero-extended and quadrupled. Consequently, an 	*/
/*   address within +60 bytes can be specified. If the displacement is 	*/
/*   too short to reach the memory operand, the aforementioned @(R0,Rn) */
/*   mode must be used. When the source operand is in memory, the 	*/
/*   loaded data is stored in the register after it is sign-extended 	*/
/*   to a longword. Note: When byte or word data is loaded, the 	*/
/*   destination register is always R0. R0 cannot be accessed by the 	*/
/*   next instruction until the load instruction is finished. The 	*/
/*   instruction order in figure 6.2 of the programming manual will 	*/
/*   give better results.						*/
/* 									*/
void
superH_movbs4(Engine *E, State *S, long d, ulong n)
{
	long disp;

	disp = (0x0000000F & (long)d);
	superHwritebyte(E, S, reg_read(E, S, n)+disp, reg_read(E, S, 0));

	return;
}

void
superH_movws4(Engine *E, State *S, long d, ulong n)
{
	long disp;

	disp = (0x0000000F & (long)d);
	superHwriteword(E, S, reg_read(E, S, n)+(disp<<1), reg_read(E, S, 0));

	return;
}

void
superH_movls4(Engine *E, State *S, ulong m, long d, ulong n)
{
	long disp;

	disp = (0x0000000F & (long)d);
	superHwritelong(E, S, reg_read(E, S, n)+(disp<<2), reg_read(E, S, m));

	return;
}

void
superH_movbl4(Engine *E, State *S, ulong m, long d)
{
	long disp;

	disp = (0x0000000F & (long)d);
	reg_set(E, S, 0, superHreadbyte(E, S, reg_read(E, S, m)+disp));

	if ((reg_read(E, S, 0) & 0x80) == 0)
	{
		reg_set(E, S, 0, reg_read(E, S, 0) & 0x000000FF);
	}
	else
	{
		reg_set(E, S, 0, reg_read(E, S, 0) | 0xFFFFFF00);
	}

	return;
}

void
superH_movwl4(Engine *E, State *S, ulong m, long d)
{
	long disp;

	disp = (0x0000000F & (long)d);
	reg_set(E, S, 0, superHreadword(E, S, reg_read(E, S, m)+(disp<<1)));

	if ((reg_read(E, S, 0) & 0x8000) == 0)
	{
		reg_set(E, S, 0, reg_read(E, S, 0) & 0x0000FFFF); 
	}
	else
	{
		reg_set(E, S, 0, reg_read(E, S, 0) | 0xFFFF0000);
	}

	return;
}

void
superH_movll4(Engine *E, State *S, ulong m, long d, ulong n)
{
	long disp;

	disp = (0x0000000F & (long)d);
	reg_set(E, S, n, superHreadlong(E, S, reg_read(E, S, m)+(disp << 2)));

	return;
}


/*									*/
/*   MOVA (Move Effective Address): Data Transfer Instruction		*/
/*									*/
/*   Format 	          Abstract	Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   MOVA @(disp,PC),R0   disp+PC->R0   11000111dddddddd   1	  -	*/
/*									*/
/*   Description: Stores the effective address of the source operand 	*/
/*   into general register R0. The 8-bit displacement is zero-extended 	*/
/*   and quadrupled. Consequently, the relative interval from the	*/
/*   operand is PC + 1020 bytes. The PC points to the starting address 	*/
/*   of the second instruction after this MOVA instruction, but the 	*/
/*   lowest two bits of the PC are corrected to B'00. Note: If this 	*/
/*   instruction is placed immediately after a delayed branch 		*/
/*   instruction, the PC must point to an address specified by (the 	*/
/*   starting address of the branch destination) + 2.			*/
/*									*/
void
superH_mova(Engine *E, State *S, long d)
{
	long disp;

	disp = (0x000000FF & (long)d);

	/*	P.EX.fetchedpc B'cos PC has been inc'd 2x since we were fetched	*/
	reg_set(E, S, 0, ((S->superH->P.EX.fetchedpc+4)&0xFFFFFFFC)+(disp<<2));

	return;
}


/*									*/
/*   MOVT (Move T Bit): Data Transfer Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   MOVT Rn 	T -> Rn 	0000nnnn00101001 	  1 	  -	*/
/*									*/
/*   Description: Stores the T bit value into general register Rn. 	*/
/*   When T = 1, 1 is stored in Rn, and when T = 0, 0 is stored in Rn.	*/
/*									*/
void
superH_movt(Engine *E, State *S, ulong n)
{
	long tmp;
	memmove(&tmp, &S->superH->SR, sizeof(tmp));

	reg_set(E, S, n, (0x00000001 & tmp));

	return;
}


/*									*/
/*   MUL.L (Multiply Long): Arithmetic Instruction			*/
/*									*/
/*   Format 	   Abstract 	      Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   MUL.L Rm,Rn   Rn x Rm -> MACL    0000nnnnmmmm0111   2-5      -	*/
/*									*/
/*   Description: Performs 32-bit multiplication of the contents of 	*/
/*   general registers Rn and Rm, and stores the bottom 32 bits of the 	*/
/*   result in the MACL register. The MACH register data does not 	*/
/*   change.								*/
/*									*/
void
superH_mull(Engine *E, State *S, ulong m, ulong n)
{
	S->superH->MACL = reg_read(E, S, n)*reg_read(E, S, m);

	return;
}


/*									*/
/*   MULS.W (Multiply as Signed Word): Arithmetic Instruction		*/
/*									*/
/*   Format 	   Abstract 		  Code 		   Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   MULS.W Rm,Rn  sgnd opn, RnxRm->MACL  0010nnnnmmmm1111  1-3     -	*/
/*   MULS Rm,Rn	   sgnd opn, RnxRm->MACL  0010nnnnmmmm1111  1-3     -	*/		
/*									*/
/*   NOTE (pip) : "sgnd opn" == "Signed Operation"/ ;)			*/
/*   Description: Performs 16-bit multiplication of the contents of 	*/
/*   general registers Rn and Rm, and stores the 32-bit result in the 	*/
/*   MACL register. The operation is signed and the MACH register data	*/
/*   does not change.							*/
/*									*/
void
superH_muls(Engine *E, State *S, ulong m, ulong n)
{
	S->superH->MACL = ((long)(short)reg_read(E, S, n)*(long)(short)reg_read(E, S, m));

	return;
}


/*									*/
/*   MULU.W (Multiply as Unsigned Word): Arithmetic Instruction		*/
/*									*/
/*   Format 	   Abstract 		  Code 	           Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   MULU.W Rm,Rn  unsgnd, RnxRm -> MAC   0010nnnnmmmm1110  1-3     -	*/
/*   MULU Rm,Rn    unsgnd, RnxRm -> MAC   0010nnnnmmmm1110  1-3     -	*/
/*									*/
/*   Description: Performs 16-bit multiplication of the contents of 	*/
/*   general registers Rn and Rm, and stores the 32-bit result in the 	*/
/*   MACL register. The operation is unsigned and the MACH register	*/
/*   data does not change.						*/
/*									*/
void
superH_mulu(Engine *E, State *S, ulong m, ulong n)
{
	S->superH->MACL = ((ulong)(ushort)reg_read(E, S, n)
	*(ulong)(ushort)reg_read(E, S, m));

	return;
}


/*									*/
/*   NEG (Negate): Arithmetic Instruction				*/
/*									*/
/*   Format 	  Abstract         Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   NEG Rm,Rn    0 - Rm -> Rn 	   0110nnnnmmmm1011       1	  -	*/
/*									*/
/*   Description: Takes the two's complement of data in general 	*/
/*   register Rm, and stores the result in Rn. This effectively 	*/
/*   subtracts Rm data from 0, and stores the result in Rn.		*/
/*									*/
void
superH_neg(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, 0 - reg_read(E, S, m));

	return;
}


/*									*/
/*   NEGC (Negate with Carry): Arithmetic Instruction			*/
/*									*/
/*   Format 	 Abstract 		 Code 		   Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   NEGC Rm,Rn  0-Rm-T-> Rn, Borrow->T  0110nnnnmmmm1010    1    Borrow*/
/*									*/
/*   Description: Subtracts general register Rm data and the T bit from	*/
/*   0, and stores the result in Rn. If a borrow is generated, T bit 	*/
/*   changes accordingly. This instruction is used for inverting the 	*/
/*   sign of a value that has more than 32 bits.			*/
/*									*/
void
superH_negc(Engine *E, State *S, ulong m, ulong n)
{
	ulong temp;

	temp = 0 - reg_read(E, S, m);
	reg_set(E, S, n, temp - S->superH->SR.T);

	if (0 < temp)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}
	if (temp < reg_read(E, S, n))
	{
		S->superH->SR.T = 1;
	}

	return;
}


/*									*/
/*   NOP (No Operation): System Control Instruction			*/
/*									*/
/*   Format	Abstract 		Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   NOP 	No operation 		0000000000001001   1 	  -	*/
/*									*/
/*   Description: Increments the PC to execute the next instruction.	*/
/*									*/
void
superH_nop(Engine *E, State *S)
{
	return;
}


/*									*/
/*   NOT (NOT - Logical Complement): Logic Operation Instruction	*/
/*									*/
/*   Format 	   Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   NOT Rm,Rn 	   ~Rm -> Rn 	0110nnnnmmmm0111 	  1	  -	*/
/*									*/
/*   Description: Takes the one's complement of general register Rm 	*/
/*   data, and stores the result in Rn. This effectively inverts each 	*/
/*   bit of Rm data and stores the result in Rn.			*/
/*									*/
void
superH_not(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, ~ reg_read(E, S, m));

	return;
}


/*									*/
/*   OR (OR Logical) Logic Operation Instruction			*/
/*									*/
/*   Format 	          Abstract 	  Code 		   Cycle T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   OR Rm,Rn 	          Rn | Rm -> Rn   0010nnnnmmmm1011   1 	   -	*/
/*   OR #imm,R0           R0 | imm -> R0  11001011iiiiiiii   1 	   -	*/
/*   OR.B #imm,@(R0,GBR)  (R0+GBR) | imm  11001111iiiiiiii   3	   -	*/
/*			  -> (R0+GBR)					*/
/*									*/
/*   Description: Logically ORs the contents of general registers Rn 	*/
/*   and Rm, and stores the result in Rn. The contents of general 	*/
/*   register R0 can also be ORed with zero-extended 8-bit immediate	*/
/*   data, or 8-bit memory data accessed by using indirect indexed GBR	*/
/*   addressing can be ORed with 8-bit immediate data.			*/
/*									*/
void
superH_or(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) | reg_read(E, S, m));

	return;
}

void
superH_ori(Engine *E, State *S, long i)
{
	reg_set(E, S, 0, reg_read(E, S, 0) | (0x000000FF & (long)i));

	return;
}

void
superH_orm(Engine *E, State *S, long i)
{
	long temp;

	temp = (long)superHreadbyte(E, S, S->superH->GBR+reg_read(E, S, 0));
	temp |= (0x000000FF & (long)i);
	superHwritebyte(E, S, S->superH->GBR+reg_read(E, S, 0), temp);

	return;
}


/*									*/
/*   PREF (Prefetch Data to the Cache)					*/
/*									*/
/*   Format    Abstract 		 Code 		  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   PREF @Rn  (Rn&0xfffffff0)->Cache	 0000nnnn10000011    1	   -	*/
/*  	       (Rn&0xfffffff0+4)->Cache					*/
/*	       (Rn&0xfffffff0+8)->Cache					*/
/*	       (Rn&0xfffffff0+C)->Cache					*/
/*									*/
/*   Description: Loads data to cache on software prefetching. 16-byte	*/
/*   data containing the data pointed by Rn (Cache 1 line) is loaded 	*/
/*   to the cache. Address Rn should be on longword boundary. No 	*/
/*   address related error is detected in this instruction. In case of 	*/
/*   an error, the instruction operates as NOP. The destination is 	*/
/*   on-chip cache, therefore this instruction functions as an NOP 	*/
/*   instruction in effect, that is, it never changes registers or 	*/
/*   processor status.							*/
/*									*/
void
superH_pref(Engine *E, State *S, ulong n)
{
	return;
}

/*									*/
/*   RFG (Reconfigure)							*/
/*   Format    Abstract 		Code 		  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   RFG #imm  Reconfigure w/code imm	1111XXXXiiiiiiii    1	   -	*/
/*									*/
/*   NOTE: The XXXX bits are currently unused, and can have any value.	*/
/*									*/
void
superH_rfg(Engine *E, State *S, long i)
{
	return;
}


/*									*/
/*   ROTCL (Rotate with Carry Left): Shift Instruction			*/
/*									*/
/*   Format 	  Abstract 	   Code 	       Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   ROTCL Rn 	  T <- Rn <- T 	   0100nnnn00100100      1 	 MSB	*/
/*									*/
/*   Description: Rotates the contents of general register Rn and the 	*/
/*   T bit to the left by one bit, and stores the result in Rn. The 	*/
/*   bit that is shifted out of the operand is transferred to the T 	*/
/*   bit (see figure 6.3 of programming manual).			*/
/*									*/
void
superH_rotcl(Engine *E, State *S, ulong n)
{
	long temp;

	if ((reg_read(E, S, n) & 0x80000000) == 0)
	{
		temp = 0;
	}
	else
	{
		temp = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) << 1);

	if (S->superH->SR.T == 1)
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0x00000001);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0xFFFFFFFE);
	}
	if (temp == 1)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/
/*   ROTCR (Rotate with Carry Right): Shift Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   ROTCR Rn 	T -> Rn -> T 	0100nnnn00100101 	  1 	 LSB	*/
/*									*/
/*   Description: Rotates the contents of general register Rn and the	*/
/*   T bit to the right by one bit, and stores the result in Rn. The 	*/
/*   bit that is shifted out of the operand is transferred to the T 	*/
/*   bit (see figure 6.4 of programming manual).			*/
/*									*/
void
superH_rotcr(Engine *E, State *S, ulong n)
{
	long temp;

	if ((reg_read(E, S, n) & 0x00000001) == 0)
	{
		temp = 0;
	}
	else
	{
		temp = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) >> 1);

	if (S->superH->SR.T == 1)
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0x80000000);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x7FFFFFFF);
	}
	if (temp == 1)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/
/*   ROTL (Rotate Left): Shift Instruction				*/
/*									*/
/*   Format 	Abstract 	   Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   ROTL Rn 	T <- Rn <- MSB 	   0100nnnn00000100 	  1 	 MSB	*/
/*									*/
/*   Description: Rotates the contents of general register Rn to the 	*/
/*   left by one bit, and stores the result in Rn (figure 6.5). The 	*/
/*   bit that is shifted out of the operand is transferred to the T 	*/
/*   bit.								*/
/*									*/
void
superH_rotl(Engine *E, State *S, ulong n)
{
	if ((reg_read(E, S, n) & 0x80000000) == 0)
	{
		S->superH->SR.T = 0;
	}
	else
	{
		S->superH->SR.T = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) << 1);

	if (S->superH->SR.T == 1)
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0x00000001);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0xFFFFFFFE);
	}

	return;
}


/*									*/
/*   ROTR (Rotate Right): Shift Instruction				*/
/*									*/
/*   Format 	Abstract 	   Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   ROTR Rn 	LSB -> Rn -> T 	   0100nnnn00000101 	  1 	 LSB	*/
/*									*/
/*   Description: Rotates the contents of general register Rn to the	*/
/*   right by one bit, and stores the result in Rn (figure 6.6). The 	*/
/*   bit that is shifted out of the operand is transferred to the 	*/
/*   T bit.								*/
/*									*/			
void
superH_rotr(Engine *E, State *S, ulong n)
{
	if ((reg_read(E, S, n) & 0x00000001) == 0)
	{
		S->superH->SR.T = 0;
	}
	else
	{
		S->superH->SR.T = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) >> 1);

	if (S->superH->SR.T == 1)
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0x80000000);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x7FFFFFFF);
	}

	return;
}


/*									*/
/*   RTE (Return from Exception): System Control Instruction		*/
/*   (Privileged Only)							*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 		Code 		  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   RTE 	SSR -> SR, SPC -> PC 	0000000000101011    4 	   -	*/
/*									*/
/*   Description: Returns from an exception routine. The PC and SR 	*/
/*   values are loaded from SPC and SSR. The program continues from 	*/
/*   the address specified by the loaded PC value. RTE is a privileged 	*/
/*   instruction and can be used in privileged mode only. If used in 	*/
/*   user mode, it causes an illegal instruction exception. Note: 	*/
/*   Since this is a delayed branch instruction, the instruction 	*/
/*   after this RTE is executed before branching. No interrupts are 	*/
/*   accepted between this instruction and the next instruction. If 	*/
/*   the next instruction is a branch instruction, it is acknowledged	*/
/*   as an illegal slot instruction. The SR accessed by an instruction 	*/
/*   in the delay slot of an RTE has been restored from the SSR by the 	*/
/*   RTE.								*/
/*									*/
void
superH_rte(Engine *E, State *S)
{
	ulong temp;
	
	/*	B'cos PC has been inc'd 2x since we were fetched	*/
	temp = S->superH->P.EX.fetchedpc;

	S->PC = S->superH->SPC;
	S->superH->SR = S->superH->SSR;

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, temp+2);
	superHIFIDflush(S);

	return;
}


/*									*/
/*   RTS (Return from Subroutine): Branch Instruction			*/
/*   Class: Delayed branch instruction					*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   RTS 	PR -> PC 	0000000000001011 	  2 	  -	*/
/*									*/
/*   Description: Returns from a subroutine procedure. The PC values 	*/
/*   are restored from the PR, and the program continues from the 	*/
/*   address specified by the restored PC value. This instruction is	*/
/*   used to return to the program from a subroutine program called 	*/
/*   by a BSR or JSR instruction. Note: Since this is a delayed branch	*/
/*   instruction, the instruction after this RTS is executed before 	*/
/*   branching. No interrupts are accepted between this instruction 	*/
/*   and the next instruction. If the next instruction is a branch 	*/
/*   instruction, it is acknowledged as an illegal slot instruction.	*/
/*   An instruction restoring the PR should be prior to an RTS 		*/
/*   instruction. That restoring instruction should not be the delay	*/
/*   slot of the RTS.							*/
/*									*/
void
superH_rts(Engine *E, State *S)
{
	ulong temp;

	/*	P.EX.fetchedpc b'cos PC has been inc'd 2x since we were fetched	*/
	temp = S->superH->P.EX.fetchedpc;

	S->PC = S->superH->PR+4;

	/* 	Exec instr in delay slot before flushing IF/ID		*/
	delayslot(E, S, temp+2);
	superHIFIDflush(S);

	/*								*/
	/*		For simulators debugging facilities		*/
	/*								*/
	if (S->pcstackheight == 0)
	{
		//sfatal(E, S, "Internal simulator error: PC stack underflow");
		//debug:
		return;
	}

	//fprintf(stdout, "NODE%d, RTS from " ULONGFMT ", S->CLK " UVLONGFMT "\n", S->NODE_ID, S->PCSTACK[S->pcstackheight]&0xFFFFFFFF, S->ICLK&0xFFFFFFFF);

	S->pcstackheight--;
	S->fpstackheight--;


	return;
}


/*									*/
/*   SETS (Set S Bit): System Control Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SETS 	1 -> S 		0000000001011000 	  1 	  -	*/
/*									*/
/*   Description: Sets the S bit to 1.					*/
/*									*/
void
superH_sets(Engine *E, State *S)
{
	S->superH->SR.S = 1;

	return;
}


/*									*/
/*   SETT (Set T Bit): System Control Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SETT 	1 -> T 		0000000000011000 	  1 	  1	*/
/*									*/
/*   Description: Sets the T bit to 1.					*/
/*									*/
void
superH_sett(Engine *E, State *S)
{
	S->superH->SR.T = 1;

	return;
}


/*									*/
/*   SHAD (Shift Arithmetic Dynamically): Shift Instruction		*/
/*									*/
/*   Format 	  Abstract 		 Code 		   Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   SHAD Rm,Rn   Rn << Rm->Rn (Rm >= 0) 0100nnnnmmmm1100    2	    -	*/
/*		  Rn >> Rm->Rn (Rm < 0)					*/
/*									*/
/*   Description: Arithmetically shifts the contents of general 	*/
/*   register Rn. General register Rm indicates the shift direction 	*/
/*   and shift count (see figure 6.7 of programming manual).		*/
/*									*/
/*   * Shift direction: Rm >- 0, left					*/
/*			Rm < 0, right					*/
/*									*/
/*   * Shift count: Rm (4-0) are used; if negative, two's complement 	*/
/*   is set to Rm.							*/
/*									*/
/*   The maximum magnitude of the left shift count is 31 (0-31).	*/
/*   The maximum magnitude of the right shift count is 32 (1-32).	*/
/*									*/
void
superH_shad(Engine *E, State *S, ulong m, ulong n)
{
	long cnt, sgn;

	sgn = reg_read(E, S, m) & 0x80000000;
	cnt = reg_read(E, S, m) & 0x0000001F;

	if (sgn == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) << cnt);
	}
	else
	{
		reg_set(E, S, n, (signed long)reg_read(E, S, n) >> ((~cnt+1) & 0x1F));
	}

	return;
}


/*									*/
/*   SHAL (Shift Arithmetic Left, (Same as SHLL)): Shift Instruction	*/
/* 									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SHAL Rn 	T <- Rn <- 0 	0100nnnn00100000 	  1 	 MSB	*/
/*									*/
/*   Description: Arithmetically shifts the contents of general 	*/
/*   register Rn to the left by one bit, and stores the result in Rn. 	*/
/*   The bit that is shifted out of the operand is transferred to the 	*/
/*   T bit (see figure 6.8 of programming manaul).			*/
/*									*/
void
superH_shal(Engine *E, State *S, ulong n)
{
	if ((reg_read(E, S, n) & 0x80000000) == 0)
	{
		S->superH->SR.T = 0;
	}
	else
	{
		S->superH->SR.T = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) << 1);

	return;
}


/*									*/
/*   SHAR (Shift Arithmetic Right): Shift Instruction			*/
/*									*/
/*   Format 	Abstract 	    Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SHAR Rn 	MSB -> Rn -> T 	    0100nnnn00100001 	  1	 LSB	*/
/*									*/
/*   Description: Arithmetically shifts the contents of general 	*/
/*   register Rn to the right by one bit, and stores the result in Rn. 	*/
/*   The bit that is shifted out of the operand is transferred to the 	*/
/*   T bit (see figure 6.9 of programming manual).			*/
/*									*/
void
superH_shar(Engine *E, State *S, ulong n)
{
	long temp;

	if ((reg_read(E, S, n) & 0x00000001) == 0)
	{
		S->superH->SR.T = 0;
	}
	else
	{
		S->superH->SR.T = 1;
	}
	if ((reg_read(E, S, n) & 0x80000000) == 0)
	{
		temp = 0;
	}
	else
	{
		temp = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) >> 1);

	if (temp == 1)
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0x80000000);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x7FFFFFFF);
	}

	return;
}


/*									*/
/*   SHLD (Shift Logical Dynamically): Shift Instruction		*/
/* 									*/
/*   Format 	   Abstract 		 Code 		   Cycle  T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   SHLD Rm,Rn    Rn<<Rm -> Rn (Rm>=0)  0100nnnnmmmm1101    2	    -	*/
/*		   Rn>>Rm -> Rn (Rm<0)					*/
/*									*/
/*   Description: Arithmetically shifts the contents of general 	*/
/*   register Rn. General register Rm indicates the shift direction 	*/
/*   and shift count (figure 6.10). T bit is the last shifted bit of 	*/
/*   Rn.								*/
/*									*/
/*   * Shift direction: Rm >= 0, left					*/
/*			Rm < 0, right					*/
/*									*/
/*   * Shift count: Rm (4-0) are used; if negative, two's complement 	*/
/*   is set to Rm.							*/
/*									*/
/*   The maximum magnitude of the left shift count is 31 (0-31).	*/
/*   The maximum magnitude of the right shift count is 32 (1-32).	*/
/*									*/
void
superH_shld(Engine *E, State *S, ulong m, ulong n)
{
	long cnt, sgn;

	sgn = reg_read(E, S, m) & 0x80000000;
	cnt = reg_read(E, S, m) & 0x0000001F;

	if (sgn == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) << cnt);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) >> ((~cnt+1)&0x1F));
	}

	return;
}


/*									*/
/*   SHLL (Shift Logical Left (Same as SHAL) ): Shift Instruction	*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SHLL Rn 	T <- Rn <- 0 	0100nnnn00000000 	  1 	 MSB	*/
/*									*/
/*   Description: Logically shifts the contents of general register 	*/
/*   Rn to the left by one bit, and stores the result in Rn. The bit 	*/
/*   that is shifted out of the operand is transferred to the T bit	*/
/*   (see figure 6.11 of the programming manual).			*/
/*									*/
void
superH_shll(Engine *E, State *S, ulong n)
{
	if ((reg_read(E, S, n) & 0x80000000) == 0)
	{
		S->superH->SR.T = 0;
	}
	else
	{
		S->superH->SR.T = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) << 1);

	return;
}


/*									*/
/*   SHLLn (Shift Logical Left n Bits): Shift Instruction		*/
/*									*/
/*   Format 	Abstract 	     Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SHLL2 	Rn Rn << 2 -> Rn     0100nnnn00001000 	  1	  -	*/
/*   SHLL8 	Rn Rn << 8 -> Rn     0100nnnn00011000 	  1	  -	*/
/*   SHLL16 	Rn Rn << 16 -> Rn    0100nnnn00101000 	  1	  -	*/
/*									*/
/*   Description: Logically shifts the contents of general register 	*/
/*   Rn to the left by 2, 8, or 16 bits, and stores the result in Rn. 	*/
/*   Bits that are shifted out of the operand are not stored 		*/
/*   (see figure 6.12 of programming manual).				*/
/*									*/
void
superH_shll2(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) << 2);

	return;
}

void
superH_shll8(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) << 8);

	return;
}

void
superH_shll16(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) << 16);

	return;
}


/*									*/
/*   SHLR (Shift Logical Right): Shift Instruction			*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SHLR Rn 	0 -> Rn -> T 	0100nnnn00000001 	  1 	 LSB	*/
/*									*/
/*   Description: Logically shifts the contents of general register 	*/
/*   Rn to the right by one bit, and stores the result in Rn. The bit 	*/
/*   that is shifted out of the operand is transferred to the T bit 	*/
/*   (see figure 6.13 of programming manual).				*/
/*									*/
void
superH_shlr(Engine *E, State *S, ulong n)
{
	if ((reg_read(E, S, n) & 0x00000001) == 0)
	{
		S->superH->SR.T = 0;
	}
	else
	{
		S->superH->SR.T = 1;
	}

	reg_set(E, S, n, reg_read(E, S, n) >> 1);
	reg_set(E, S, n, reg_read(E, S, n) & 0x7FFFFFFF);

	return;
}


/*									*/
/*   SHLRn (Shift Logical Right n Bits): Shift Instruction		*/
/*									*/
/*   Format 	  Abstract 	   Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SHLR2 Rn 	  Rn>>2 -> Rn 	   0100nnnn00001001 	  1	  -	*/
/*   SHLR8 Rn 	  Rn>>8 -> Rn 	   0100nnnn00011001 	  1	  -	*/
/*   SHLR16 Rn	  Rn>>16 -> Rn 	   0100nnnn00101001 	  1	  -	*/
/*									*/
/*   Description: Logically shifts the contents of general register 	*/
/*   Rn to the right by 2, 8, or 16 bits, and stores the result in Rn. 	*/
/*   Bits that are shifted out of the operand are not stored 		*/
/*  (see figure 6.14 of programming manual).				*/
/*									*/
void
superH_shlr2(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) >> 2);
	reg_set(E, S, n, reg_read(E, S, n) & 0x3FFFFFFF);

	return;
}

void
superH_shlr8(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) >> 8);
	reg_set(E, S, n, reg_read(E, S, n) & 0x00FFFFFF);

	return;
}

void
superH_shlr16(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) >> 16);
	reg_set(E, S, n, reg_read(E, S, n) & 0x0000FFFF);

	return;
}


/*									*/
/*   SLEEP (Sleep): System Control Instruction (Privileged Only)	*/
/*									*/
/*   Format 	Abstract 	Code 			Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SLEEP 	Sleep 		0000000000011011 	  4	  -	*/
/*									*/
/*   Description: Sets the CPU into power-down mode. In power-down 	*/
/*   mode, instruction execution stops, but the CPU module status is 	*/
/*   maintained, and the CPU waits for an interrupt request. If an	*/
/*   interrupt is requested, the CPU exits the power-down mode and 	*/
/*   begins exception processing. SLEEP is a privileged instruction 	*/
/*   and can be used in privileged mode only. If used in user mode, it	*/
/*   causes an illegal instruction exception. Note: The number of 	*/
/*   cycles given is for the transition to sleep mode.			*/
/*									*/
void
superH_sleep(Engine *E, State *S)
{
	S->sleep = 1;

	return;
}


/*									*/
/*   STC (Store Control Register): System Control Instruction 		*/
/*   (Privileged Only)							*/
/*									*/
/*   Format 	         Abstract 	Code 		  Cycle	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   STC SR,Rn 	         SR -> Rn 	0000nnnn00000010    1     -	*/
/*   STC GBR,Rn          GBR -> Rn 	0000nnnn00010010    1     -	*/
/*   STC VBR,Rn          VBR -> Rn 	0000nnnn00100010    1     -	*/
/*   STC SSR,Rn          SSR -> Rn 	0000nnnn00110010    1     -	*/
/*   STC SPC,Rn          SPC -> Rn 	0000nnnn01000010    1     -	*/
/*   STC R0_BANK,Rn      R0_BANK -> Rn 	0000nnnn10000010    1     -	*/
/*   STC R1_BANK,Rn      R1_BANK -> Rn 	0000nnnn10010010    1     -	*/
/*   STC R2_BANK,Rn      R2_BANK -> Rn 	0000nnnn10100010    1     -	*/
/*   STC R3_BANK,Rn      R3_BANK -> Rn 	0000nnnn10110010    1     -	*/
/*   STC R4_BANK,Rn      R4_BANK -> Rn 	0000nnnn11000010    1     -	*/
/*   STC R5_BANK,Rn      R5_BANK -> Rn 	0000nnnn11010010    1     -	*/
/*   STC R6_BANK,Rn      R6_BANK -> Rn 	0000nnnn11100010    1     -	*/
/*   STC R7_BANK,Rn      R7_BANK -> Rn 	0000nnnn11110010    1     -	*/
/*   STC.L SR,@-Rn       Rn - 4 -> Rn,	0100nnnn00000011    1     -	*/	
/*   			 SR -> (Rn) 					*/
/*   STC.L GBR,@-Rn      Rn - 4 -> Rn,	0100nnnn00010011    1     -	*/
/*   			 GBR -> (Rn) 					*/
/*   STC.L VBR,@-Rn      Rn-4 -> Rn,	0100nnnn00100011    1     -	*/
/*   			 VBR -> (Rn) 					*/
/*   STC.L SSR,@-Rn      Rn - 4 -> Rn,	0100nnnn00110011    1     -	*/
/*   			 SSR -> (Rn) 					*/
/*   STC.L SPC,@-Rn      Rn - 4 -> Rn,	0100nnnn01000011    1     -	*/
/*   			 SPC -> (Rn) 					*/
/*   STC.L R0_BANK,@-Rn  Rn - 4 -> Rn,	0100nnnn10000011    2     -	*/
/*   			 R0_BANK->(Rn)					*/
/*   STC.L R1_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn10010011    2     -	*/
/*			 R1_BANK->(Rn)					*/
/*   STC.L R2_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn10100011    2     -	*/
/*			 R2_BANK->(Rn)					*/
/*   STC.L R3_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn10110011    2     -	*/
/*			 R3_BANK->(Rn)					*/
/*   STC.L R4_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn11000011    2     -	*/
/*			 R4_BANK->(Rn)					*/
/*   STC.L R5_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn11010011    2     -	*/
/*			 R5_BANK->(Rn)					*/
/*   STC.L R6_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn11100011    2     -	*/
/*			 R6_BANK->(Rn)					*/
/*   STC.L R7_BANK,@-Rn  Rn - 4 -> Rn, 	0100nnnn11110011    2     -	*/
/*			 R7_BANK->(Rn)					*/
/*									*/
/*   Description: Stores control registers SR, GBR, VBR, SSR, SPC, or	*/
/*   R0-R7_BANK data into a specified destination. STC and STC.L, 	*/
/*   except for STC GBR, Rn and STC.L GBR, @-Rn are privileged 		*/
/*   instructions and can be used in privileged mode only. If used in 	*/
/*   user mode, they cause illegal instruction exceptions. STC GBR, Rn 	*/
/*   and STC.L GBR, @-Rn can be used in user mode. Rn_BANK is 		*/
/*   designated by the RB bit of the SR. When the RB = 1, Rn_BANK0 is 	*/
/*   accessed by STC/STC.L instructions. When the RB = 0, Rn_BANK1 is 	*/
/*   accessed by STC/STC.L instructions.				*/
/*									*/
void
superH_stcsr(Engine *E, State *S, ulong n)
{
	ulong tmp;

	memmove(&tmp, &S->superH->SR, sizeof(tmp));
	reg_set(E, S, n, tmp);

	return;
}

void
superH_stcgbr(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, S->superH->GBR);

	return;
}

void
superH_stcvbr(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, S->superH->VBR);

	return;
}

void
superH_stcssr(Engine *E, State *S, ulong n)
{
	ulong tmp;

	memmove(&tmp, &S->superH->SSR, sizeof(tmp));
	reg_set(E, S, n, tmp);

	return;
}

void
superH_stcspc(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, S->superH->SPC);

	return;
}


/*									*/
/*    	See description at section 6.28 for reason why [!SR.RB]		*/
/*    	Our register set is R_BANK and R, where R_BANK is a 2x7		*/
/*    	array of longs, and R is a 1x7 array of longs.	 		*/
/*									*/
void
superH_stcr_bank(Engine *E, State *S, int reg, ulong n)
{
	if (S->superH->SR.RB == 0)
	{
		S->superH->R[n] = S->superH->R_BANK[reg];
	}
	else
	{
		S->superH->R[n] = S->superH->R[reg];
	}

	return;
}

void
superH_stcmsr(Engine *E, State *S, ulong n)
{
	ulong	tmp;

	memmove(&tmp, &S->superH->SR, sizeof(tmp));
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), tmp);

	return;
}

void
superH_stcmgbr(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), S->superH->GBR);

	return;
}

void
superH_stcmvbr(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), S->superH->VBR);

	return;
}

void
superH_stcmssr(Engine *E, State *S, ulong n)
{
	ulong	tmp;

	memmove(&tmp, &S->superH->SSR, sizeof(tmp));
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), tmp);

	return;
}

void
superH_stcmspc(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), S->superH->SPC);

	return;
}

void
superH_stcmr_bank(Engine *E, State *S, int reg, ulong n)
{
	S->superH->R[n] -= 4;

	if (S->superH->SR.RB == 0)
	{
		superHwritelong(E, S, S->superH->R[n], S->superH->R_BANK[reg]);
	}
	else
	{
		superHwritelong(E, S, S->superH->R[n], S->superH->R[reg]);
	}

	return;
}


/*									*/
/*   STS (Store System Register): System Control Instruction		*/
/*									*/
/*   Format 	      Abstract 		   Code 	    Cycle T Bit */
/*   ------------------------------------------------------------------	*/
/*   STS MACH,Rn      MACH -> Rn 	   0000nnnn00001010   1	    -	*/
/*   STS MACL,Rn      MACL -> Rn 	   0000nnnn00011010   1	    -	*/
/*   STS PR,Rn        PR -> Rn 		   0000nnnn00101010   1	    -	*/
/*   STS.L MACH,@-Rn  Rn-4->Rn,MACH->(Rn)  0100nnnn00000010   1	    -	*/
/*   STS.L MACL,@-Rn  Rn-4->Rn,MACL->(Rn)  0100nnnn00010010   1	    -	*/
/*   STS.L PR,@-Rn    Rn-4->Rn,PR->(Rn)    0100nnnn00100010   1	    -	*/
/*									*/
/*   Description: Stores system registers MACH, MACL and PR data into 	*/
/*   a specified destination.						*/
/*									*/
void
superH_stsmach(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, S->superH->MACH);

	if ((reg_read(E, S, n) & 0x00000200) == 0)
	{
		reg_set(E, S, n, reg_read(E, S, n) & 0x000003FF);
	}
	else
	{
		reg_set(E, S, n, reg_read(E, S, n) | 0xFFFFFC00);
	}

	return;
}

void
superH_stsmacl(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, S->superH->MACL);

	return;
}

void
superH_stspr(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, S->superH->PR);

	return;
}

void
superH_stsmmach(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	if ((S->superH->MACH&0x00000200) == 0)
	{
		superHwritelong(E, S, reg_read(E, S, n), S->superH->MACH & 0x000003FF);
	}
	else
	{
		superHwritelong(E, S, reg_read(E, S, n), S->superH->MACH | 0xFFFFFC00);
	}

	return;
}

void
superH_stsmmacl(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), S->superH->MACL);

	return;
}

void
superH_stsmpr(Engine *E, State *S, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - 4);
	superHwritelong(E, S, reg_read(E, S, n), S->superH->PR);

	return;
}


/*									*/
/*   SUB (Subtract Binary): Arithmetic Instruction			*/
/*									*/
/*   Format 	   Abstract 	     Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SUB Rm,Rn 	   Rn - Rm -> Rn     0011nnnnmmmm1000 	  1 	  -	*/
/*									*/
/*   Description: Subtracts general register Rm data from Rn data, and 	*/
/*   stores the result in Rn. To subtract immediate data, use ADD 	*/
/*   #imm,Rn.								*/
/*									*/
void
superH_sub(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) - reg_read(E, S, m));

	return;
}


/*									*/
/*   SUBC (Subtract with Carry): Arithmetic Instruction			*/
/*									*/
/*   Format 	   Abstract 		 Code 	    	   Cycle T Bit 	*/
/*   -----------------------------------------------------------------	*/
/*   SUBC Rm,Rn    Rn-Rm-T -> Rn, B->T   0011nnnnmmmm1010    1     B	*/
/*									*/
/*   B = "Borrow"							*/
/*									*/
/*   Description: Subtracts Rm data and the T bit value from general 	*/
/*   register Rn data, and stores the result in Rn. The T bit changes 	*/
/*   according to the result. This instruction is used for subtraction 	*/
/*   of data that has more than 32 bits.				*/
/*									*/
void
superH_subc(Engine *E, State *S, ulong m, ulong n)
{
	ulong tmp0, tmp1;

	tmp1 = reg_read(E, S, n) - reg_read(E, S, m);
	tmp0 = reg_read(E, S, n);
	reg_set(E, S, n, tmp1 - S->superH->SR.T);

	if (tmp0 < tmp1)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}
	if (tmp1 < reg_read(E, S, n))
	{
		S->superH->SR.T = 1;
	}

	return;
}


/*									*/
/*  SUBV (Subtract with V Flag Underflow Check): Arithmetic Instruction */
/*									*/
/*   Format	  Abstract		Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SUBV Rm,Rn   Rn-Rm->Rn, U->T 	0011nnnnmmmm1011   1	  U	*/
/*									*/
/*   Description: Subtracts Rm data from general register Rn data, and 	*/
/*   stores the result in Rn. If an underflow occurs, the T bit is set 	*/
/*   to 1.								*/
/*									*/
void
superH_subv(Engine *E, State *S, ulong m, ulong n)
{
	long dest, src, ans;

	if ((long)reg_read(E, S, n) >= 0)
	{
		dest = 0;
	}
	else
	{
		dest = 1;
	}
	if ((long)reg_read(E, S, m) >= 0)
	{
		src = 0;
	}
	else
	{
		src = 1;
	}

	src += dest;
	reg_set(E, S, n, reg_read(E, S, n) - reg_read(E, S, m));

	if ((long)reg_read(E, S, n) >= 0)
	{
		ans = 0;
	}
	else
	{
		ans = 1;
	}

	ans += dest;

	if (src == 1) 
	{
		if (ans == 1)
		{
			S->superH->SR.T = 1;
		}
		else
		{
			S->superH->SR.T = 0;
		}
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/
/*   SWAP (Swap Register Halves): Data Transfer Instruction		*/
/*									*/
/*   Format 	   Abstract 		 Code 		  Cycle T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   SWAP.B Rm,Rn  Rm -> Swap upper and  0110nnnnmmmm1000   1	  -	*/
/*		   lower halves of  					*/
/*		   lower 2 bytes -> Rn					*/
/*									*/
/*   SWAP.W Rm,Rn  Rm -> Swap upper and  0110nnnnmmmm1001   1	  -	*/
/*		   lower word -> Rn					*/
/*									*/
/*   Description: Swaps the upper and lower bytes of the general 	*/
/*   register Rm data, and stores the result in Rn. If a byte is 	*/
/*   specified, bits 0 to 7 of Rm are swapped for bits 8 to 15. 	*/
/*   The upper 16 bits of Rm are transferred to the upper 16 bits of 	*/
/*   Rn. If a word is specified, bits 0 to 15 of Rm are swapped for 	*/
/*   bits 16 to 31.							*/
/*									*/
void
superH_swapb(Engine *E, State *S, ulong m, ulong n)
{
	ulong temp0, temp1;

	temp0 = reg_read(E, S, m) & 0xffff0000;
	temp1 = (reg_read(E, S, m) & 0x000000ff)<<8;
	reg_set(E, S, n, (reg_read(E, S, m) & 0x0000ff00)>>8);
	reg_set(E, S, n, reg_read(E, S, n) | temp1 | temp0);
	
	return;
}

void
superH_swapw(Engine *E, State *S, ulong m, ulong n)
{
	ulong temp;
	temp=(reg_read(E, S, m)>>16)&0x0000FFFF;
	reg_set(E, S, n, reg_read(E, S, m) << 16);
	reg_set(E, S, n, reg_read(E, S, n) | temp);

	return;
}


/*									*/
/*   TAS (Test and Set): Logic Operation Instruction			*/
/*									*/
/*   Format 	 Abstract 		Code 		 Cycle 	T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   TAS.B @Rn   When (Rn) is 0, 1->T,  0100nnnn00011011   3   results	*/
/*		 1->MSB of (Rn) 					*/
/*									*/
/*   Description: Reads byte data from the address specified by general */
/*   register Rn, and sets the T bit to 1 if the data is 0, or clears 	*/
/*   the T bit to 0 if the data is not 0. Then, data bit 7 is set to 1, */
/*   and the data is written to the address specified by Rn. During 	*/
/*   this operation, the bus is not released. Note: The destination of 	*/
/*   the TAS instruction should be placed in a non-cacheable space when	*/
/*   the cache is enabled.						*/
/*									*/
void
superH_tas(Engine *E, State *S, ulong n)
{
	long temp;

	temp = (long)superHreadbyte(E, S, reg_read(E, S, n)); /* Bus Lock enable */
	if (temp == 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	temp |= 0x00000080;
	superHwritebyte(E, S, reg_read(E, S, n), temp); /* Bus Lock disable */

	return;
}


/*									*/
/*   TRAPA (Trap Always): System Control Instruction			*/
/*									*/
/*   Format 	   Abstract 		Code 		Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   TRAPA #imm    imm -> TRA,		11000011iiiiiiii  6	 -	*/
/* 		   PC -> SPC,						*/
/* 		   SR -> SSR,						*/
/* 		   1 -> SR.MD/BL/RB					*/
/* 		   0x160 -> EXPEVT					*/
/* 		   VBR+H'00000100 -> PC					*/
/*   Description: Starts the trap exception processing. The PC and 	*/
/*   SR values are saved in SPC and SSR. Eight-bit immediate data is 	*/
/*   stored in the TRA registers (TRA9 to TRA2). The processor goes 	*/
/*   into privileged mode (SR.MD = 1) with SR.BL = 1 and SR.RB = 1, 	*/
/*   that is, blocking exceptions and masking interrupts, and selecting	*/
/*   BANK1 registers (R0_BANK1 to R7_BANK1). Exception code 0x160 is 	*/
/*   stored in the EXPEVT register (EXPEVT11 to EXPEVT0). The program	*/
/*   branches to an address (VBR+H'00000100).				*/
/*									*/
void
superH_trapa(Engine *E, State *S, long i)
{
	long imm;


	imm = (0x000000FF & i);

	//TODO: shouldn't we be checking interrupt mask before going ahead ?

	if (imm == 34)
	{
		/*							*/
		/*	First 4 words are in R4-R7, rest on stack	*/
		/*	fortunately, we only need 1st 4 args.		*/
		/*							*/
		reg_set(E, S, 0, sim_syscall(E, S, reg_read(E, S, 4),
			reg_read(E, S, 5), reg_read(E, S, 6), reg_read(E, S, 7)));

		return;
	}

	/*								*/
	/*	NOTE: according to HW manual, imm is zero-extended	*/
	/*	and quadrupled.						*/
	/*								*/
	S->superH->TRA = imm<<2;
	S->superH->SSR = S->superH->SR;

	/*   P.EX.fetchedpc b'cos PC has in'd 2x since we were fetched		*/
	S->superH->SPC = S->superH->P.EX.fetchedpc;

	S->superH->SR.MD = 1;
	S->superH->SR.BL = 1;
	S->superH->SR.RB = 1;
	S->superH->EXPEVT = 0x00000160;

	S->PC = S->superH->VBR+0x00000100;
	superHIFIDflush(S);

	return;
}


/*									*/
/*   TST (Test Logical): Logic Operation Instruction			*/
/*									*/
/*   Format 	  Abstract 		  Code 		   Cycle T Bit	*/
/*   ------------------------------------------------------------------	*/
/*   TST Rm,Rn    Rn & Rm, when result    0010nnnnmmmm1000   1	 result	*/
/*		  is 0, 1 -> T						*/
/*									*/
/*   TST #imm,R0  R0 & imm, when result   11001000iiiiiiii   1	 result	*/
/*		  is 0, 1 -> T						*/
/*									*/
/*   TST.B #imm,  (R0 + GBR) & imm, when  11001100iiiiiiii   3   result	*/
/*   @(R0,GBR)	  result is 0, 1 -> T					*/
/*									*/
/*   Description: Logically ANDs the contents of general registers Rn 	*/
/*   and Rm, and sets the T bit to 1 if the result is 0 or clears the 	*/
/*   T bit to 0 if the result is not 0. The Rn data does not change. 	*/
/*   The contents of general register R0 can also be ANDed with 	*/
/*   zero-extended 8-bit immediate data, or the contents of 8-bit 	*/
/*   memory accessed by indirect indexed GBR addressing can be ANDed 	*/
/*   with 8-bit immediate data. The R0 and memory data do not change.	*/
/*									*/
void
superH_tst(Engine *E, State *S, ulong m, ulong n)
{
	if ((reg_read(E, S, n) & reg_read(E, S, m)) == 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}

void
superH_tsti(Engine *E, State *S, long i)
{
	long temp;

	temp = reg_read(E, S, 0) & (0x000000FF & (long)i);
	if (temp == 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}

void
superH_tstm(Engine *E, State *S, long i)
{
	long temp;

	temp = (long)superHreadbyte(E, S, S->superH->GBR+reg_read(E, S, 0));
	temp &= (0x000000FF & (long)i);

	if (temp == 0)
	{
		S->superH->SR.T = 1;
	}
	else
	{
		S->superH->SR.T = 0;
	}

	return;
}


/*									*/				
/*   XOR (Exclusive OR Logical): Logic Operation Instruction		*/
/*									*/
/*   Format 	  	   Abstract 	 Code 		  Cycle  T Bit	*/
/*   -----------------------------------------------------------------	*/
/*   XOR Rm,Rn 		   Rn^Rm->Rn 	 0010nnnnmmmm1010   1	   -	*/
/*   XOR #imm,R0 	   R0^imm->R0 	 11001010iiiiiiii   1	   -	*/
/*   XOR.B #imm,@(R0,GBR)  (R0+GBR)^imm	 11001110iiiiiiiii  3	   -	*/
/*			   -> (R0 + GBR)				*/
/*									*/
/*   Description: Exclusive ORs the contents of general registers Rn 	*/
/*   and Rm, and stores the result in Rn. The contents of general 	*/
/*   register R0 can also be exclusive ORed with zero-extended 8-bit	*/
/*   immediate data, or 8-bit memory accessed by indirect indexed GBR 	*/
/*   addressing can be exclusive ORed with 8-bit immediate data.	*/
/* 									*/
void
superH_xor(Engine *E, State *S, ulong m, ulong n)
{
	reg_set(E, S, n, reg_read(E, S, n) ^ reg_read(E, S, m));

	return;
}

void
superH_xori(Engine *E, State *S, long i)
{
	reg_set(E, S, 0, reg_read(E, S, 0) ^ (0x000000FF & (long)i));

	return;
}

void
superH_xorm(Engine *E, State *S, long i)
{
	long temp;

	temp = (long)superHreadbyte(E, S, S->superH->GBR+reg_read(E, S, 0));
	temp ^= (0x000000FF & (long)i);
	superHwritebyte(E, S, S->superH->GBR+reg_read(E, S, 0),temp);

	return;
}


/*									*/
/*   XTRCT (Extract): Data Transfer Instruction				*/
/*									*/
/*   Format 	    Abstract 		Code 		 Cycle 	T Bit	*/
/*   ----------------------------------------------------------------	*/
/*   XTRCT Rm,Rn    Rm: Center 32 bits 	0010nnnnmmmm1101   1 	  -	*/
/*		    of Rn -> Rn						*/
/*									*/
/*   Description: Extracts the middle 32 bits from the 64 bits of 	*/
/*   general registers Rm and Rn, and stores the 32 bits in Rn 		*/
/*   (see figure 6.15 of programming manual)				*/
/*									*/
void
superH_xtrct(Engine *E, State *S, ulong m, ulong n)
{
	ulong temp;

	temp = (reg_read(E, S, m) << 16) & 0xFFFF0000;
	reg_set(E, S, n, (reg_read(E, S, n)>>16)&0x0000FFFF);
	reg_set(E, S, n, reg_read(E, S, n) | temp);

	return;
}
