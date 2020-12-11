/*
	Copyright (c) 2017-2018, Zhengyang Gu (author)
			2019, Hein Alexander Mante (taint propagation)

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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sf.h"
#include "instr-riscv.h"
#include "mextern.h"

uint32_t
sign_extend(uint32_t	data, uint8_t n)
{
	uint32_t	sign_mask = 1 << (n-1);
	uint32_t	valid_bits;
	if (n == 32)
	{
		valid_bits = -1;
	}
	else
	{
		valid_bits = (1 << n) - 1;
	}
	if (data & sign_mask)
	{

		return (data | (uint32_t) (-1 - valid_bits));
	}
	else
	{
		return (data & valid_bits);
	}
}

/*
 *	Convert a 32 bit (single precision) float into a NaN 64 bit
 *	(double precision) float containing the single precision
 *	float in the lower 32 bits.
 */
uint64_t
nan_box(uint32_t	f)
{
	return ((~(uint64_t)0) << 32) | f;
}

/*
 *	Checks if a 64-bit value is NaN-boxed
 *	returns the lower 32-bits if NaN-boxed and a canonical NaN if otherwise
 */
uint32_t
is_nan_boxed(uint64_t f)
{
	if((uint32_t)(f >> 32) == 0xFFFFFFFF) return (uint32_t)f;
	else return 0x7FFFFFFF;
}

void
riscv_nop(Engine *E, State *S)/*	pseudo instruction	*/
{
	riscv_addi(E, S, 0, 0, 0);

	return;
}

int	get_uncertain_memory_index(Engine *E, State *S, uint32_t	vaddr)
{
	/*	TODO: programmatically pick a sensible value
	 *	Note that in the "finished" version uncertain memory will be
	 *	indexed starting at zero and so this will not be needed.
	 */
	const uint32_t	INST_END_ADDR = 0x8009fa4;
	if ((vaddr & 0b11) != 0)
	{
		merror(E, "Unaligned uncertain aware floating point	load/store at address 0x%08u", vaddr);
	}
	int	addr = (int)(vaddr - INST_END_ADDR) / 4;

	return addr;
}

void uncertain_check_part2_pc(Engine *E, State *S)
{
	UncertainUpdateInfo *tmp = &S->riscv->uncertain->last_op;

	if (tmp->op_fp_pc != S->riscv->P.EX.fetchedpc - 4)
	{
		merror(
			E,
			"Found second part of an uncertainty aware instruction that is not immediately proceded\n"
			"in memory by a first part.\n"
			"First part at 0x%08X and second part at 0x%08X.",
			tmp->op_fp_pc,
			S->riscv->P.EX.fetchedpc
			);
	}
}

void
riscv_add(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) + reg_read_riscv(E, S, rs2)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_sub(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) - reg_read_riscv(E, S, rs2)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_mul(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int64_t multiplicationResult=0;
	int32_t resultLowerHalf=0;
	
	/* --------- */
	multiplicationResult = reg_read_riscv(E, S, rs1) * reg_read_riscv(E, S, rs2);
	resultLowerHalf = multiplicationResult & 0xFFFFFFFF;

	if (multiplicationResult < 0)
	{
		resultLowerHalf = -resultLowerHalf;	
	}

	reg_set_riscv(E, S, rd, resultLowerHalf);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_mulh(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int64_t multiplicationResult=0;
	int32_t resultLowerHalf=0;

	multiplicationResult = reg_read_riscv(E, S, rs1) * reg_read_riscv(E, S, rs2);
	resultLowerHalf = (multiplicationResult >> 32) & 0xFFFFFFFF;

	reg_set_riscv(E, S, rd, resultLowerHalf);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_mulhu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint64_t multiplicationResult=0;
	uint32_t resultLowerHalf=0;

	multiplicationResult = reg_read_riscv(E, S, rs1) * reg_read_riscv(E, S, rs2);
	resultLowerHalf = (multiplicationResult >> 32) & 0xFFFFFFFF;

	reg_set_riscv(E, S, rd, resultLowerHalf);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_mulhsu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int64_t multiplicationResult=0;
	int32_t resultLowerHalf=0;

	multiplicationResult = reg_read_riscv(E, S, rs1) * reg_read_riscv(E, S, rs2);
	resultLowerHalf = (multiplicationResult >> 32) & 0xFFFFFFFF;

	reg_set_riscv(E, S, rd, resultLowerHalf);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_div(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int32_t dividend, divisor, quotient;

	dividend = (int32_t) reg_read_riscv(E, S, rs1);
	divisor = (int32_t) reg_read_riscv(E, S, rs2);
	
	if (divisor != 0)
	{
		quotient = dividend / divisor;
	}
	else /* Volume I: RISC-V Unprivileged ISA V20190608-Base-Ratified - page 44 */
	{
		quotient = -1;
	}
	
	reg_set_riscv(E, S, rd, quotient);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_divu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t dividend, divisor, quotient;

	dividend = reg_read_riscv(E, S, rs1);
	divisor = reg_read_riscv(E, S, rs2);
	
	if (divisor != 0)
	{
		quotient = dividend / divisor;
	}
	else /* Volume I: RISC-V Unprivileged ISA V20190608-Base-Ratified - page 44 */
	{
		quotient = 0xFFFFFFFF;
	}
	
	//fprintf(stderr, "riscv_divu: rs2 %d. rs1 %d. Rd is %d.\n", rs2, rs1, rd);
	//fprintf(stderr, "riscv_divu: dividend: %d. divisor: %d. quotient: %d.\n", dividend, divisor, quotient);

	reg_set_riscv(E, S, rd, quotient);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_rem(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int32_t dividend, divisor, remainder;

	dividend = (int32_t) reg_read_riscv(E, S, rs1);
	divisor = (int32_t) reg_read_riscv(E, S, rs2);
	
	if (divisor != 0)
	{
		remainder = dividend % divisor;
	}
	else /* Volume I: RISC-V Unprivileged ISA V20190608-Base-Ratified - page 44 */
	{
		remainder = dividend;
	}
	
	reg_set_riscv(E, S, rd, remainder);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_remu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t dividend, divisor, remainder;

	dividend = reg_read_riscv(E, S, rs1);
	divisor = 	reg_read_riscv(E, S, rs2);
	
	if (divisor != 0)
	{
		remainder = dividend % divisor;
	}
	else /* Volume I: RISC-V Unprivileged ISA V20190608-Base-Ratified - page 44 */
	{
		remainder = dividend;
	}
	
	reg_set_riscv(E, S, rd, remainder);

	if (SF_TAINTANALYSIS)
	{
		/* FIXME */
	}
}

void
riscv_slt(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) < (int32_t) reg_read_riscv(E, S, rs2))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}


	return;
}

void
riscv_sltu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	if (reg_read_riscv(E, S, rs1) < reg_read_riscv(E, S, rs2))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}
	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}


	return;
}

void
riscv_and(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) & reg_read_riscv(E, S, rs2)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_or(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) | reg_read_riscv(E, S, rs2)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_xor(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) ^ reg_read_riscv(E, S, rs2)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_sll(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int	xlen_b = 5;
	int	mask = ((1 << xlen_b) - 1);
	uint8_t	shift = reg_read_riscv(E, S, rs2) & mask;

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) << shift));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_srl(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int	xlen_b = 5;
	int	mask = ((1 << xlen_b) - 1);
	uint8_t	shift = reg_read_riscv(E, S, rs2) & mask;

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) >> shift));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_sra(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int		xlen_b = 5;
	int		mask = ((1 << xlen_b) - 1);
	uint8_t		shift = reg_read_riscv(E, S, rs2) & mask;
	uint32_t	data = ((signed int) reg_read_riscv(E, S, rs1)) >> shift;

	reg_set_riscv(E, S, rd, data);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}
	return;
}

void
riscv_addi(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	/*
	 *	Assumption: imm0 carries no taint	(assumption carries on to all other functions
	 *	with immediate values but will only be stated here)
	 */

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) + (int32_t) sign_extend(imm0, 12)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}
	return;
}

void
riscv_slti(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) < (int32_t) sign_extend(imm0, 12))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_sltiu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	if (reg_read_riscv(E, S, rs1) < sign_extend(imm0, 12))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_andi(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) & sign_extend(imm0, 12)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_ori(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) | sign_extend(imm0, 12)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_xori(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) ^ sign_extend(imm0, 12)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_slli(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	int	xlen_b = 5;
	int	mask = ((1 << xlen_b) - 1);
	uint8_t shift = imm0 & mask;

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) << shift));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_srli(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	int	xlen_b = 5;
	int	mask = ((1 << xlen_b) - 1);
	uint8_t shift = imm0 & mask;

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) >> shift));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_srai(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t	imm0)
{
	int	xlen_b = 5;
	int	mask = ((1 << xlen_b) - 1);
	uint8_t shift = imm0 & mask;
	uint32_t	data = ((signed int)reg_read_riscv(E, S, rs1)) >> shift;
	reg_set_riscv(E, S, rd, data);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_lui(Engine *E, State *S, uint8_t rd, uint32_t	imm0)
{
	reg_set_riscv(E, S, rd, (imm0<<12));

	return;
}

void
riscv_auipc(Engine *E, State *S, uint8_t rd, uint32_t	imm0)
{
	reg_set_riscv(E, S, rd, (imm0<<12) + S->PC - 4);

	if (SF_TAINTANALYSIS)
	{
		/*
		 *	Propagate taint	of PC into register[rd]:
		 */
		taintprop(E, S,	taintretreg(E,S,32),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_jal(Engine *E, State *S, uint8_t rd, uint16_t imm1, uint8_t imm11, uint8_t imm12, uint8_t imm20)
{
	int32_t offset = sign_extend((imm1 << 1) + (imm11 << 11) + (imm12 << 12) + (imm20 << 20), 21);

	reg_set_riscv(E, S, rd, S->PC);
	S->PC += offset - 4;

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,32),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);

		/*
		 *	If immediates are seen as carrying taint, more then their taint
		 *	should also be propagated to the PC (ORed with the PC's previous
		 *	taint).
		 */
	}

	return;
}

void
riscv_jalr(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	int32_t addr = (int32_t) sign_extend(imm0, 12) + (int32_t) reg_read_riscv(E, S, rs1);

	uint32_t	mask = -2;

	reg_set_riscv(E, S, rd, S->PC);
	S->PC = (addr) & mask;

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,32),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);

		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_beq(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if (reg_read_riscv(E, S, rs1) == reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}
	return;
}

void
riscv_bne(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{

	if (reg_read_riscv(E, S, rs1) != reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_blt(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) < (int32_t) reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_bltu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if (reg_read_riscv(E, S, rs1) < reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}
	return;
}

void
riscv_bge(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) >= (int32_t) reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_bgeu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if (reg_read_riscv(E, S, rs1) >= reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
				(uint64_t)32,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_lw(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	uint32_t	value = riscVreadlong(E, S, addr);

	reg_set_riscv(E, S, rd, value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,4),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_lh(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	reg_set_riscv(E, S, rd, sign_extend(riscVreadword(E, S, addr), 16));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_lhu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	reg_set_riscv(E, S, rd, (uint32_t) riscVreadword(E, S, addr));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_lb(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);
	uint8_t		data_b = riscVreadword(E, S, addr) & 0xff;

	reg_set_riscv(E, S, rd, sign_extend(data_b, 8));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,1),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_lbu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);
	uint8_t		data_b = riscVreadbyte(E, S, addr);

	reg_set_riscv(E, S, rd, (uint32_t) data_b);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,1),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
riscv_sw(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);
	uint32_t	value = reg_read_riscv(E,S, rs2);

	riscVwritelong(E, S, addr, value);

	if (SF_TAINTANALYSIS)
	{
		for (int	i = 0 ; i<4 ; i++)
		{
			taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
					(uint64_t)(addr+i),	kSunflowerTaintMemTypeMemory);
		}
	}
}

void
riscv_sh(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	riscVwriteword(E, S,addr, reg_read_riscv(E,S, rs2) & 0xffff);

	if (SF_TAINTANALYSIS)
	{
		for (int	i = 0 ; i<2 ; i++)
		{
			taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
					(uint64_t)(addr+i),	kSunflowerTaintMemTypeMemory);
		}
	}
}

void
riscv_sb(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t	addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	riscVwritebyte(E, S,addr, reg_read_riscv(E,S, rs2) & 0xff);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretreg(E,S,rs2),
					(uint64_t)addr,	kSunflowerTaintMemTypeMemory);
	}
}

void
riscv_fence(Engine *E, State *S)
{
	return;
}

void
riscv_fence_i(Engine *E, State *S)
{
	return;
}

void
riscv_cor(Engine *E, State *S)
{
	return;
}

void
riscv_ebreak(Engine *E, State *S)
{
	return;
}

void
riscv_csrrw(Engine *E, State *S)
{
	return;
}

void
riscv_csrrs(Engine *E, State *S)
{
	return;
}

void
riscv_csrrc(Engine *E, State *S)
{
	return;
}

void
riscv_csrrwi(Engine *E, State *S)
{
	return;
}

void
riscv_csrrsi(Engine *E, State *S)
{
	return;
}

void
riscv_csrrci(Engine *E, State *S)
{
	return;
}

void
riscv_ecall(Engine *E, State *S)
{
	unsigned long returnValue;

	/*
	 *	http://man7.org/linux/man-pages/man2/syscall.2.html
	 */
	uint32_t	syscall_num = reg_read_riscv(E, S, RISCV_A7);

	returnValue = riscv_sim_syscall(E, S, syscall_num,
				reg_read_riscv(E, S, RISCV_A0),
				reg_read_riscv(E, S, RISCV_A1),
				reg_read_riscv(E, S, RISCV_A2));
	
	reg_set_riscv(E, S, RISCV_A0, returnValue);
	/*
	 *	No taint propagation implemented in this function
	 */
}



/*
 *	RISC-V RV32F instructions
 */



void
rv32f_flw(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E, S, rs1) + sign_extend(imm0, 12);

	/*
	 *	Perform a normal floating point load.
	 */
	freg_set_riscv(E, S, rd, nan_box(riscVreadlong(E, S, addr)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,4),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL)
	{
		if (S->riscv->uncertain->last_op.valid)
		{
			uncertain_check_part2_pc(E, S);

			uint32_t	rs1Uncertain = ((instr_i *)&S->riscv->uncertain->last_op.insn_part1)->rs1;
			uint32_t	immUncertain = ((instr_i *)&S->riscv->uncertain->last_op.insn_part1)->imm0;

			uint32_t	uncertainAddr = reg_read_riscv(E, S, rs1Uncertain) + sign_extend(immUncertain, 12);
			int		uncertainIndex = get_uncertain_memory_index(E, S, uncertainAddr);

			/*
			 *	If the uncertainIndex is negative then we are trying to read a memory location
			 *	in the `.text` section of the binary. This is how gnu generated binaries initialise
			 *	floats. For now, just load an uncertainty of zero.
			 */
			if (uncertainIndex < 0)
				uncertain_inst_sv(S->riscv->uncertain, rd, 0);
			else
				uncertain_inst_lr(S->riscv->uncertain, rd, uncertainIndex);
			S->riscv->uncertain->last_op.valid = 0;
		}
	}

	return;
}

void
rv32f_fsw(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t	addr = reg_read_riscv(E, S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	/*
	 *	Perform a normal floating point store.
	 */
	riscVwritelong(E, S, addr, freg_read_riscv(E, S, rs2));

	if (SF_TAINTANALYSIS)
	{
		for (int	i = 0 ; i<4 ; i++)
		{
			taintprop(E, S,	taintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
					(uint64_t)(addr+i),	kSunflowerTaintMemTypeMemory);
		}
	}

	if (S->riscv->uncertain != NULL)
	{
		if (S->riscv->uncertain->last_op.valid)
		{
			uncertain_check_part2_pc(E, S);

			uint32_t	rs1Uncertain = ((instr_i *)&S->riscv->uncertain->last_op.insn_part1)->rs1;
			uint32_t	immUncertain = ((instr_i *)&S->riscv->uncertain->last_op.insn_part1)->imm0;
			uint32_t	uncertainAddr = reg_read_riscv(E, S, rs1Uncertain) + sign_extend(immUncertain, 12);
			int		uncertainIndex = get_uncertain_memory_index(E, S, uncertainAddr);

			/*
			 *	If software tries to store an uncertain value into the `.text` section
			 *	I think it is a bug in that software(?).
			 *
			 *	Regardless, the uncertain memory does not exist for negative indexes so
			 *	we cannot possibly fulfill this store. Instead, log an error.
			 */
			if (uncertainIndex < 0)
			{
				merror(E, "Cannot store uncertainty to address %X.\n", uncertainAddr);
			}
			else
			{
				uncertain_inst_sr(S->riscv->uncertain, rs2, uncertainIndex);
			}

			S->riscv->uncertain->last_op.valid = 0;
		}
	}

	return;
}

void
rv32f_fmadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep	src1, src2, src3, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));
	src3.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs3));

	result.float_value = (src1.float_value * src2.float_value) + src3.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32f_fmsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep	src1, src2, src3, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));
	src3.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs3));

	result.float_value = (src1.float_value * src2.float_value) - src3.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32f_fnmsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep	src1, src2, src3, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));
	src3.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs3));

	result.float_value = (-1.0 * src1.float_value * src2.float_value) + src3.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32f_fnmadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep	src1, src2, src3, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));
	src3.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs3));

	result.float_value = (-1.0 * src1.float_value * src2.float_value) - src3.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32f_fadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32f_rep	src1, src2, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	result.float_value = src1.float_value + src2.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);

		uncertain_inst_up2(S->riscv->uncertain, rd, rs1, rs2, 1, 1);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32f_rep	src1, src2, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	result.float_value = src1.float_value - src2.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);

		uncertain_inst_up2(S->riscv->uncertain, rd, rs1, rs2, 1, -1);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fmul_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32f_rep	src1, src2, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	result.float_value = src1.float_value * src2.float_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);
		uncertain_inst_up2(S->riscv->uncertain, rd, rs1, rs2, src2.float_value, src1.float_value);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fdiv_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32f_rep	src1, src2, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	float		divsrc2 = 1 / src2.float_value;
	float		src1div2 = src1.float_value * divsrc2;
	float		minussrc1div2div2 = -src1div2 / src2.float_value;

	result.float_value = src1div2;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);
		uncertain_inst_up2(S->riscv->uncertain, rd, rs1, rs2, divsrc2, minussrc1div2div2);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fsqrt_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32f_rep	src1, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));

	float		root = sqrtf(src1.float_value);

	result.float_value = root;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);
		uncertain_inst_up1(S->riscv->uncertain, rd, rs1, 0.5 / root);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fsgnj_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t	src1 = is_nan_boxed(freg_read_riscv(E, S, rs1));
	uint32_t	src2 = is_nan_boxed(freg_read_riscv(E, S, rs2));
	uint32_t	result = (src1 & (-1 - (1 << 31))) | (src2 & (1 << 31));

	freg_set_riscv(E, S, rd, nan_box(result));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);
		uncertain_inst_mv(S->riscv->uncertain, rd, rs1);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fsgnjn_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t	src1 = is_nan_boxed(freg_read_riscv(E, S, rs1));
	uint32_t	src2 = is_nan_boxed(freg_read_riscv(E, S, rs2));
	uint32_t	result = (src1 & (-1 - (1 << 31))) | (~src2 & (1 << 31));

	freg_set_riscv(E, S, rd, nan_box(result));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);
		uncertain_inst_mv(S->riscv->uncertain, rd, rs1);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fsgnjx_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t	src1 = is_nan_boxed(freg_read_riscv(E, S, rs1));
	uint32_t	src2 = is_nan_boxed(freg_read_riscv(E, S, rs2));
	uint32_t	result = (src1 & (-1 - (1 << 31))) | ((src1 ^ src2) & (1 << 31));

	freg_set_riscv(E, S, rd, nan_box(result));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);
		uncertain_inst_mv(S->riscv->uncertain, rd, rs1);
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fmin_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	src1, src2, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	result.float_value = fminf(src1.float_value, src2.float_value);

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);

	}

	// uint8_t min_reg = (src1.float_value <= src2.float_value) ? rs1 : rs2;
	// TODO: update for fmin?

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);

		merror(E, "We currently do not support unfmin.s. Sorry!");

		uncertain_inst_mv(S->riscv->uncertain, rd, nan(""));
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fmax_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	src1, src2, result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	result.float_value = fmaxf(src1.float_value, src2.float_value);

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);

	}

	// uint8_t max_reg = (src1.float_value >= src2.float_value) ? rs1 : rs2;
	// TODO: update for fmax?

	if (S->riscv->uncertain != NULL && S->riscv->uncertain->last_op.valid)
	{
		uncertain_check_part2_pc(E, S);

		merror(E, "We currently do not support unfmax.s. Sorry!");

		uncertain_inst_mv(S->riscv->uncertain, rd, nan(""));
		S->riscv->uncertain->last_op.valid = 0;
	}

	return;
}

void
rv32f_fcvt_w_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	src1;
	uint32_t	result;
	uint8_t		frm;
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));

	/*
	 *	Rounding modes - Reference
	 *	https://www.gnu.org/software/libc/manual/html_node/Rounding-Functions.html
	 */
	switch (rm) //TODO check rm value for rounding
	{
		case 0b000: //Round to nearest (ties to Even)
			result = (int32_t)rintf(src1.float_value);
			break;

		case 0b001: //Round towards zero
			result = (int32_t)truncf(src1.float_value);
			break;

		case 0b010:	//Round down (towards -inf)
			result = (int32_t)floorf(src1.float_value);
			break;

		case 0b011: //Round up (towards +inf)
			result = (int32_t)ceilf(src1.float_value);
			break;

		case 0b100: //Round to nearest (ties to Max Magnitude)
			result = (int32_t)roundf(src1.float_value);
			break;

		case 0b101: //Invalid. Reserved for future use
		case 0b110: //Do nothing for now
			return;

		case 0b111: //Dynamic rounding mode, read from frm
			frm = (S->riscv->fCSR & (0b111 << 5)) >> 5;
			switch (frm)
			{
				case 0b000: //Round to nearest (ties to Even)
					result = (int32_t)rintf(src1.float_value);
					break;

				case 0b001: //Round towards zero
					result = (int32_t)truncf(src1.float_value);
					break;

				case 0b010:	//Round down (towards -inf)
					result = (int32_t)floorf(src1.float_value);
					break;

				case 0b011: //Round up (towards +inf)
					result = (int32_t)ceilf(src1.float_value);
					break;

				case 0b100: //Round to nearest (ties to Max Magnitude)
					result = (int32_t)roundf(src1.float_value);
					break;

				case 0b101: //Invalid
				case 0b110: //Do nothing for now
				case 0b111:
				default:
					return;
			}
			break;

		default:
			return;
	}

	reg_set_riscv(E, S, rd, result);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_fcvt_wu_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	src1;
	uint32_t	result;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));

	uint8_t		frm;
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;

	/*
	 *	Rounding modes - Reference
	 *	https://www.gnu.org/software/libc/manual/html_node/Rounding-Functions.html
	 */
	switch (rm) //TODO check rm value for rounding
	{
		case 0b000: //Round to nearest (ties to Even)
			result = (uint32_t)rintf(src1.float_value);
			break;

		case 0b001: //Round towards zero
			result = (uint32_t)truncf(src1.float_value);
			break;

		case 0b010:	//Round down (towards -inf)
			result = (uint32_t)floorf(src1.float_value);
			break;

		case 0b011: //Round up (towards +inf)
			result = (uint32_t)ceilf(src1.float_value);
			break;

		case 0b100: //Round to nearest (ties to Max Magnitude)
			result = (uint32_t)roundf(src1.float_value);
			break;

		case 0b101: //Invalid. Reserved for future use
		case 0b110: //Do nothing for now
			return;

		case 0b111: //Dynamic rounding mode, read from frm
			frm = (S->riscv->fCSR & (0b111 << 5)) >> 5;
			switch (frm)
			{
				case 0b000: //Round to nearest (ties to Even)
					result = (uint32_t)rintf(src1.float_value);
					break;

				case 0b001: //Round towards zero
					result = (uint32_t)truncf(src1.float_value);
					break;

				case 0b010:	//Round down (towards -inf)
					result = (uint32_t)floorf(src1.float_value);
					break;

				case 0b011: //Round up (towards +inf)
					result = (uint32_t)ceilf(src1.float_value);
					break;

				case 0b100: //Round to nearest (ties to Max Magnitude)
					result = (uint32_t)roundf(src1.float_value);
					break;

				case 0b101: //Invalid
				case 0b110: //Do nothing for now
				case 0b111:
				default:
					return;
			}
			break;

		default:
			return;
	}

	reg_set_riscv(E, S, rd, result);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_fmv_x_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, is_nan_boxed(freg_read_riscv(E, S, rs1)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_feq_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	src1, src2;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	reg_set_riscv(E, S, rd, (src1.float_value == src2.float_value) ? 1 : 0);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_flt_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	src1, src2;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	reg_set_riscv(E, S, rd, (src1.float_value < src2.float_value) ? 1 : 0);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_fle_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1, src2;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	src2.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs2));

	reg_set_riscv(E, S, rd, (src1.float_value <= src2.float_value) ? 1 : 0);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_fclass_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	/*
	 *	TODO: See https://www.gnu.org/software/libc/manual/html_node/Floating-Point-Classes.html
	 */
	uint32_t	shift = 0;
	rv32f_rep	src1;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));

	int	class = fpclassify(src1.float_value);

	switch (class)
	{
		case FP_NAN:
			//BUG: not portable
			//shift = (__issignaling(src1.float_value)) ? 8 : 9;
			break;

		case FP_INFINITE:
			shift = (signbit(src1.float_value)==0) ? 7 : 0;
			break;

		case FP_ZERO:
			shift = (signbit(src1.float_value)==0) ? 4 : 3;
			break;

		case FP_SUBNORMAL:
			shift = (signbit(src1.float_value)==0) ? 5 : 2;
			break;

		case FP_NORMAL:
			shift = (signbit(src1.float_value)==0) ? 6 : 1;
			break;

		default:
			break;
	}

	reg_set_riscv(E, S, rd, (uint32_t)(1 << shift));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32f_fcvt_s_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?

	rv32f_rep	result;
	int32_t		src1 = (int32_t)reg_read_riscv(E, S, rs1);

	result.float_value = (float)src1; //cast to float

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32f_fcvt_s_wu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?

	rv32f_rep	result; //TODO test this
	uint32_t	src1 = reg_read_riscv(E, S, rs1);

	result.float_value = (float)src1; //cast to float

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32f_fmv_w_x(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	freg_set_riscv(E, S, rd, nan_box(reg_read_riscv(E, S, rs1)));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}



/*
 *	RISC-V RV32D instructions
 */



void
rv32d_fld(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t	addr = reg_read_riscv(E, S, rs1) + sign_extend(imm0, 12);
	uint64_t	data_lsw = (uint64_t)riscVreadlong(E, S, addr);
	uint64_t	data_msw = (uint64_t)riscVreadlong(E, S, (addr+4));
	uint64_t	data = (data_msw << 32) | data_lsw;

	freg_set_riscv(E, S, rd, data);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	taintretmems(E,S,addr,8),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fsd(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t	addr = reg_read_riscv(E, S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);
	uint64_t	data = freg_read_riscv(E, S, rs2);
	uint32_t	data_lsw = (uint32_t)(data & 0xffffffff);
	uint32_t	data_msw = (uint32_t)((data >> 32) & 0xffffffff);

	riscVwritelong(E, S, addr, data_lsw);
	riscVwritelong(E, S, addr+4, data_msw);

	if (SF_TAINTANALYSIS)
	{
		for (int i = 0 ; i<8 ;i++)
		{
			taintprop(E, S,	taintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
					(uint64_t)(addr+i),	kSunflowerTaintMemTypeMemory);
		}
	}

	return;
}

void
rv32d_fmadd_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32d_rep	src1, src2, src3, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);
	src3.bit64_value = freg_read_riscv(E, S, rs3);

	result.double_value = (src1.double_value * src2.double_value) + src3.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fmsub_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32d_rep src1, src2, src3, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);
	src3.bit64_value = freg_read_riscv(E, S, rs3);

	result.double_value = (src1.double_value * src2.double_value) - src3.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fnmsub_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32d_rep	src1, src2, src3, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);
	src3.bit64_value = freg_read_riscv(E, S, rs3);

	result.double_value = (-1.0 * src1.double_value * src2.double_value) + src3.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fnmadd_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32d_rep	src1, src2, src3, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);
	src3.bit64_value = freg_read_riscv(E, S, rs3);

	result.double_value = (-1.0 * src1.double_value * src2.double_value) - src3.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	(ftaintretreg(E,S,rs2) | ftaintretreg(E,S,rs3)),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fadd_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;

	rv32d_rep	src1, src2, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	result.double_value = src1.double_value + src2.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fsub_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32d_rep	src1, src2, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	result.double_value = src1.double_value - src2.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fmul_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32d_rep	src1, src2, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	result.double_value = src1.double_value * src2.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fdiv_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32d_rep	src1, src2, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	result.double_value = src1.double_value / src2.double_value;

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fsqrt_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//TODO: rs2 is unused

	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	rv32d_rep	src1, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	result.double_value = sqrt(src1.double_value);

	switch (rm) //TODO check rm value for rounding
	{
	}

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fsgnj_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint64_t	src1 = freg_read_riscv(E, S, rs1);
	uint64_t	src2 = freg_read_riscv(E, S, rs2);
	uint64_t	result = (src1 & ((uint64_t)(-1) - ((uint64_t)1 << 63))) | (src2 & ((uint64_t)1 << 63));

	freg_set_riscv(E, S, rd, result);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fsgnjn_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint64_t	src1 = freg_read_riscv(E, S, rs1);
	uint64_t	src2 = freg_read_riscv(E, S, rs2);
	uint64_t	result = (src1 & ((uint64_t)(-1) - ((uint64_t)1 << 63))) | (~src2 & ((uint64_t)1 << 63));

	freg_set_riscv(E, S, rd, result);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fsgnjx_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint64_t	src1 = freg_read_riscv(E, S, rs1);
	uint64_t	src2 = freg_read_riscv(E, S, rs2);
	uint64_t	result = (src1 & ((uint64_t)(-1) - ((uint64_t)1 << 63))) | ((src1 ^ src2) & ((uint64_t)1 << 63));

	freg_set_riscv(E, S, rd, result);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fmin_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32d_rep	src1, src2, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	result.double_value = fmin(src1.double_value, src2.double_value);

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fmax_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32d_rep	src1, src2, result;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);
	result.double_value = fmax(src1.double_value, src2.double_value);

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);

	}

	return;
}

void
rv32d_fcvt_s_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?

	//TODO: rs2 unused

	rv32f_rep	result;
	rv32d_rep	src1;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	result.float_value = (float)src1.double_value; //cast to float

	freg_set_riscv(E, S, rd, nan_box(result.bit_value));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fcvt_d_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?

	//TODO: rs2 unused

	rv32d_rep	result;
	rv32f_rep	src1;

	src1.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));
	result.double_value = (double)src1.float_value; //cast to double

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_feq_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32d_rep	src1, src2;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	reg_set_riscv(E, S, rd, (src1.double_value == src2.double_value) ? 1 : 0);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32d_flt_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32d_rep	src1, src2;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	reg_set_riscv(E, S, rd, (src1.double_value < src2.double_value) ? 1 : 0);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32d_fle_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32d_rep	src1, src2;

	src1.bit64_value = freg_read_riscv(E, S, rs1);
	src2.bit64_value = freg_read_riscv(E, S, rs2);

	reg_set_riscv(E, S, rd, (src1.double_value <= src2.double_value) ? 1 : 0);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	ftaintretreg(E,S,rs2),
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32d_fclass_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	/*
	 *	TODO: See https://www.gnu.org/software/libc/manual/html_node/Floating-Point-Classes.html
	 */
	uint32_t	shift = 0;
	rv32d_rep	src1;

	src1.bit64_value = freg_read_riscv(E, S, rs1);

	int		class = fpclassify(src1.double_value);

	switch (class)
	{
		case FP_NAN:
			//BUG: not portable
			//shift = (__issignaling(src1.double_value)) ? 8 : 9;
			break;

		case FP_INFINITE:
			shift = (signbit(src1.double_value)==0) ? 7 : 0;
			break;

		case FP_ZERO:
			shift = (signbit(src1.double_value)==0) ? 4 : 3;
			break;

		case FP_SUBNORMAL:
			shift = (signbit(src1.double_value)==0) ? 5 : 2;
			break;

		case FP_NORMAL:
			shift = (signbit(src1.double_value)==0) ? 6 : 1;
			break;

		default:
			break;
	}

	reg_set_riscv(E, S, rd, (uint32_t)(1 << shift));

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32d_fcvt_w_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//TODO: rs2 unused

	rv32d_rep	src1;

	uint8_t		frm;
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;

	src1.bit64_value = freg_read_riscv(E, S, rs1);

	/*
	 *	Rounding modes - Reference
	 *	https://www.gnu.org/software/libc/manual/html_node/Rounding-Functions.html
	 */
	switch (rm) //TODO check rm value for rounding
	{
		case 0b000: //Round to nearest (ties to Even)
			reg_set_riscv(E, S, rd, (int32_t)rint(src1.double_value));
			break;

		case 0b001: //Round towards zero
			reg_set_riscv(E, S, rd, (int32_t)trunc(src1.double_value));
			break;

		case 0b010:	//Round down (towards -inf)
			reg_set_riscv(E, S, rd, (int32_t)floor(src1.double_value));
			break;

		case 0b011: //Round up (towards +inf)
			reg_set_riscv(E, S, rd, (int32_t)ceil(src1.double_value));
			break;

		case 0b100: //Round to nearest (ties to Max Magnitude)
			reg_set_riscv(E, S, rd, (int32_t)round(src1.double_value));
			break;

		case 0b101: //Invalid. Reserved for future use
		case 0b110: //Do nothing for now
			break;

		case 0b111: //Dynamic rounding mode, read from frm
			frm = (S->riscv->fCSR & (0b111 << 5)) >> 5;
			switch (frm)
			{
				case 0b000: //Round to nearest (ties to Even)
					reg_set_riscv(E, S, rd, (int32_t)rint(src1.double_value));
					break;

				case 0b001: //Round towards zero
					reg_set_riscv(E, S, rd, (int32_t)trunc(src1.double_value));
					break;

				case 0b010:	//Round down (towards -inf)
					reg_set_riscv(E, S, rd, (int32_t)floor(src1.double_value));
					break;

				case 0b011: //Round up (towards +inf)
					reg_set_riscv(E, S, rd, (int32_t)ceil(src1.double_value));
					break;

				case 0b100: //Round to nearest (ties to Max Magnitude)
					reg_set_riscv(E, S, rd, (int32_t)round(src1.double_value));
					break;

				case 0b101: //Invalid
				case 0b110: //Do nothing for now
				case 0b111:
				default:
					break;
			}
			break;

		default:
			break;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32d_fcvt_wu_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32d_rep	src1;

	src1.bit64_value = freg_read_riscv(E, S, rs1);

	uint8_t		frm;
	uint8_t		rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;

	/*
	 *	Rounding modes - Reference
	 *	https://www.gnu.org/software/libc/manual/html_node/Rounding-Functions.html
	 */
	switch (rm) //TODO check rm value for rounding
	{
		case 0b000: //Round to nearest (ties to Even)
			reg_set_riscv(E, S, rd, (uint32_t)rint(src1.double_value));
			break;

		case 0b001: //Round towards zero
			reg_set_riscv(E, S, rd, (uint32_t)trunc(src1.double_value));
			break;

		case 0b010:	//Round down (towards -inf)
			reg_set_riscv(E, S, rd, (uint32_t)floor(src1.double_value));
			break;

		case 0b011: //Round up (towards +inf)
			reg_set_riscv(E, S, rd, (uint32_t)ceil(src1.double_value));
			break;

		case 0b100: //Round to nearest (ties to Max Magnitude)
			reg_set_riscv(E, S, rd, (uint32_t)round(src1.double_value));
			break;

		case 0b101: //Invalid. Reserved for future use
		case 0b110: //Do nothing for now
			break;

		case 0b111: //Dynamic rounding mode, read from frm
			frm = (S->riscv->fCSR & (0b111 << 5)) >> 5;
			switch (frm)
			{
				case 0b000: //Round to nearest (ties to Even)
					reg_set_riscv(E, S, rd, (uint32_t)rint(src1.double_value));
					break;

				case 0b001: //Round towards zero
					reg_set_riscv(E, S, rd, (uint32_t)trunc(src1.double_value));
					break;

				case 0b010:	//Round down (towards -inf)
					reg_set_riscv(E, S, rd, (uint32_t)floor(src1.double_value));
					break;

				case 0b011: //Round up (towards +inf)
					reg_set_riscv(E, S, rd, (uint32_t)ceil(src1.double_value));
					break;

				case 0b100: //Round to nearest (ties to Max Magnitude)
					reg_set_riscv(E, S, rd, (uint32_t)round(src1.double_value));
					break;

				case 0b101: //Invalid
				case 0b110: //Do nothing for now
				case 0b111:
				default:
					break;
			}
			break;

		default:
			break;
	}

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	ftaintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypeRegister);
	}

	return;
}

void
rv32d_fcvt_d_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?
	//TODO: rs2 unused

	rv32d_rep	result;
	int32_t		src1 = (int32_t)reg_read_riscv(E, S, rs1);

	result.double_value = (double)src1; //cast to double

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}

void
rv32d_fcvt_d_wu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?

	rv32d_rep	result; //TODO test this
	uint32_t	src1 = reg_read_riscv(E, S, rs1);

	result.double_value = (double)src1; //cast to double

	freg_set_riscv(E, S, rd, result.bit64_value);

	if (SF_TAINTANALYSIS)
	{
		taintprop(E, S,	taintretreg(E,S,rs1),	0,
				(uint64_t)rd,		kSunflowerTaintMemTypefltRegister);
	}

	return;
}



/*
 *	RISC-V RV32UN instructions
 */



void
rv32un_unupg_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uncertain_inst_up1(S->riscv->uncertain, rd, rs1, rs2);
}

void
rv32un_ungcov_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep	var;

	var.float_value = uncertain_inst_gcov(S->riscv->uncertain, rs1, rs2);

	// printf("Getting var of %g at x%u\n", var.float_value, rs1);
	freg_set_riscv(E, S, rd, nan_box(var.bit_value));
	uncertain_inst_sv(S->riscv->uncertain, rd, nan(""));

}

void
rv32un_unsvar_s(Engine *E, State *S, uint8_t rs1, uint8_t _rs2, uint8_t rd)
{
	rv32f_rep	var;

	var.bit_value = is_nan_boxed(freg_read_riscv(E, S, rs1));

	uncertain_inst_sv(S->riscv->uncertain, rd, var.float_value);
}

void
rv32un_unclvar_s(Engine *E, State *S, uint8_t _rs1, uint8_t _rs2, uint8_t rd)
{
	uncertain_inst_sv(S->riscv->uncertain, rd, 0.0);
}

void
rv32un_uncpvar_s(Engine *E, State *S, uint8_t rs1, uint8_t _rs2, uint8_t rd)
{
	uncertain_inst_mv(S->riscv->uncertain, rd, rs1);
}

void
rv32un_un_part1(Engine *E, State *S, uint8_t _rs1, uint8_t _rd, uint16_t _imm0)
{
	if (S->riscv->uncertain == NULL)
	{
		merror(E, "Simulator is not running in unceratin mode but uncertainty aware instruction encountered!");
	}
	else
	{
		UncertainUpdateInfo *tmp = &S->riscv->uncertain->last_op;

		if (tmp->valid)
		{
			merror(
				E,
				"Found two first parts of an uncertainty aware instruction\n"
				"\twithout a second part in between!\n"
				"\tInstructions fetched from 0x%08X and 0x%08X",
				tmp->op_fp_pc,
				S->riscv->P.EX.fetchedpc
				);

		}

		tmp->op_fp_pc = S->riscv->P.EX.fetchedpc;
		tmp->insn_part1 = S->riscv->P.EX.instr;
		tmp->valid = 1;
	}
}
