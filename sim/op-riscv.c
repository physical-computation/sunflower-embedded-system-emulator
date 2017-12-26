/*
	Copyright (c) 1999-2017, Zhengyang Gu (author)
 
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

uint32_t sign_extend(uint32_t data, uint8_t n)
{
	uint32_t sign_mask = 1 << (n-1);
	uint32_t valid_bits;
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

void riscv_add(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) + reg_read_riscv(E, S, rs2)));

	return;
}

void riscv_sub(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) - reg_read_riscv(E, S, rs2)));

	return;
}

void riscv_slt(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	
	if ((int32_t) reg_read_riscv(E, S, rs1) < (int32_t) reg_read_riscv(E, S, rs2))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	return;
}

void riscv_sltu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	
	if (reg_read_riscv(E, S, rs1) < reg_read_riscv(E, S, rs2))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	return;
}

void riscv_and(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) & reg_read_riscv(E, S, rs2)));

	return;
}

void riscv_or(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) | reg_read_riscv(E, S, rs2)));

	return;
}

void riscv_xor(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) ^ reg_read_riscv(E, S, rs2)));

	return;
}

void riscv_sll(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = reg_read_riscv(E, S, rs2) & mask;
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) << shift));

	return;
}

void riscv_srl(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = reg_read_riscv(E, S, rs2) & mask;
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) >> shift));

	return;
}

void riscv_sra(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = reg_read_riscv(E, S, rs2) & mask;
	uint32_t data = ((signed int) reg_read_riscv(E, S, rs1)) >> shift;
	reg_set_riscv(E, S, rd, data);

	return;
}

void riscv_addi(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) + (int32_t) sign_extend(imm0, 12)));

	return;
}

void riscv_slti(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) < (int32_t) sign_extend(imm0, 12))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	return;
}

void riscv_sltiu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	if (reg_read_riscv(E, S, rs1) < sign_extend(imm0, 12))
	{
		reg_set_riscv(E, S, rd, 1);
	}
	else
	{
		reg_set_riscv(E, S, rd, 0);
	}

	return;
}

void riscv_andi(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) & sign_extend(imm0, 12)));

	return;
}

void riscv_ori(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) | sign_extend(imm0, 12)));

	return;
}

void riscv_xori(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) ^ sign_extend(imm0, 12)));

	return;
}

void riscv_slli(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = imm0 & mask; 

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) << shift));

	return;
}

void riscv_srli(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = imm0 & mask; 

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) >> shift));

	return;
}

void riscv_srai(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = imm0 & mask; 
	uint32_t data = ((signed int) reg_read_riscv(E, S, rs1)) >> shift;
	reg_set_riscv(E, S, rd, data);

	return;
}

void riscv_lui(Engine *E, State *S, uint8_t rd, uint32_t imm0)
{
	reg_set_riscv(E, S, rd, (imm0<<12));

	return;
}

void riscv_auipc(Engine *E, State *S, uint8_t rd, uint32_t imm0)
{
	reg_set_riscv(E, S, rd, imm0 + S->PC);

	return;
}

void riscv_jal(Engine *E, State *S, uint8_t rd, uint16_t imm1, uint8_t imm11, uint8_t imm12, uint8_t imm20)
{
	int32_t offset = sign_extend((imm1 << 1) + (imm11 << 11) + (imm12 << 12) + (imm20 << 20), 21);

	reg_set_riscv(E, S, rd, S->PC);
	S->PC += offset - 4;

	return;
}

void riscv_jalr(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	int32_t offset = (int32_t) sign_extend(imm0, 12) + (int32_t) reg_read_riscv(E, S, rs1);
	
	uint32_t mask = -2;

	reg_set_riscv(E, S, rd, S->PC);
	S->PC = (S->PC + offset - 4) & mask;

	return;
}

void riscv_beq(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if (reg_read_riscv(E, S, rs1) == reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	return;
}

void riscv_bne(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{

	if (reg_read_riscv(E, S, rs1) != reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	return;
}

void riscv_blt(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) < (int32_t) reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	return;
}

void riscv_bltu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if (reg_read_riscv(E, S, rs1) < reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	return;
}

void riscv_bge(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if ((int32_t) reg_read_riscv(E, S, rs1) >= (int32_t) reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	return;
}

void riscv_bgeu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5,\
			   uint8_t imm11, uint8_t imm12)
{
	if (reg_read_riscv(E, S, rs1) >= reg_read_riscv(E, S, rs2))
	{
		int32_t offset = sign_extend((imm1 << 1) + (imm5 << 5) + (imm11 << 11) \
									 + (imm12 << 12), 13);
		S->PC += offset - 4;
	}

	return;
}

void riscv_lw(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	reg_set_riscv(E, S, rd, superHreadlong(E, S, addr));

	return;
}

void riscv_lh(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	reg_set_riscv(E, S, rd, sign_extend(superHreadword(E, S, addr), 16));

	return;
}

void riscv_lhu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	reg_set_riscv(E, S, rd, (uint32_t) superHreadword(E, S, addr));

	return;
}

void riscv_lb(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	uint8_t data_b = superHreadword(E, S, addr) & 0xff;

	reg_set_riscv(E, S, rd, sign_extend(data_b, 8));

	return;
}

void riscv_lbu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0, 12);

	uint8_t data_b = superHreadword(E, S, addr) & 0xff;

	reg_set_riscv(E, S, rd, (uint32_t) data_b);

	return;
}

void riscv_sw(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	superHwritelong(E, S,addr, reg_read_riscv(E,S, rs2));
}

void riscv_sh(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	superHwriteword(E, S,addr, reg_read_riscv(E,S, rs2) & 0xffff);
}

void riscv_sb(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t addr = reg_read_riscv(E,S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	superHwritebyte(E, S,addr, reg_read_riscv(E,S, rs2) & 0xff);
}

void 	riscv_fence(Engine *E, State *S) {}
void 	riscv_fence_i(Engine *E, State *S) {}
void 	riscv_cor(Engine *E, State *S) {}
void 	riscv_ecall(Engine *E, State *S) {}
void 	riscv_ebreak(Engine *E, State *S) {}
void 	riscv_csrrw(Engine *E, State *S) {}
void 	riscv_csrrs(Engine *E, State *S) {}
void 	riscv_csrrc(Engine *E, State *S) {}
void 	riscv_csrrwi(Engine *E, State *S) {}
void 	riscv_csrrsi(Engine *E, State *S) {}
void 	riscv_csrrci(Engine *E, State *S) {}