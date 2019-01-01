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

void riscv_slli(Engine *E, State *S, uint8_t rs1, uint8_t shamt, uint8_t rd)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = shamt & mask; 

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) << shift));

	return;
}

void riscv_srli(Engine *E, State *S, uint8_t rs1, uint8_t shamt, uint8_t rd)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = shamt & mask; 

	reg_set_riscv(E, S, rd, (reg_read_riscv(E, S, rs1) >> shift));

	return;
}

void riscv_srai(Engine *E, State *S, uint8_t rs1, uint8_t shamt, uint8_t rd)
{
	int xlen_b = 5;
	int mask = ((1 << xlen_b) - 1);
	uint8_t shift = shamt & mask; 
	signed int src1 = reg_read_riscv(E, S, rs1);
	int32_t data = src1 >> shift;
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
	int32_t addr = (int32_t) sign_extend(imm0, 12) + (int32_t) reg_read_riscv(E, S, rs1);
	
	uint32_t mask = -2;

	reg_set_riscv(E, S, rd, S->PC);
	S->PC = (addr) & mask;

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
	
	uint32_t value = superHreadlong(E, S, addr);
	
	reg_set_riscv(E, S, rd, value);

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
	
	uint32_t value = reg_read_riscv(E,S, rs2);
	
	superHwritelong(E, S, addr, value);
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


/* RISC-V RV32F instructions */
void rv32f_flw(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0)
{
	uint32_t addr = reg_read_riscv(E, S, rs1) + sign_extend(imm0, 12);

	freg_set_riscv(E, S, rd, superHreadlong(E, S, addr));

	return;
}

void rv32f_fsw(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5)
{
	uint32_t addr = reg_read_riscv(E, S, rs1) + sign_extend(imm0 + (imm5 << 5), 12);

	superHwritelong(E, S, addr, freg_read_riscv(E, S, rs2));
	
	return;
}

void rv32f_fmadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep src1, src2, src3, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	src3.bit_value = freg_read_riscv(E, S, rs3);
	
	result.float_value = (src1.float_value * src2.float_value) + src3.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fmsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{	
	rv32f_rep src1, src2, src3, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	src3.bit_value = freg_read_riscv(E, S, rs3);
	
	result.float_value = (src1.float_value * src2.float_value) - src3.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fnmsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep src1, src2, src3, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	src3.bit_value = freg_read_riscv(E, S, rs3);
	
	result.float_value = (-1.0 * src1.float_value * src2.float_value) + src3.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fnmadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd)
{
	rv32f_rep src1, src2, src3, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	src3.bit_value = freg_read_riscv(E, S, rs3);
	
	result.float_value = (-1.0 * src1.float_value * src2.float_value) - src3.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	rv32f_rep src1, src2, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	result.float_value = src1.float_value + src2.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	rv32f_rep src1, src2, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	result.float_value = src1.float_value - src2.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fmul_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	rv32f_rep src1, src2, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	result.float_value = src1.float_value * src2.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fdiv_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	rv32f_rep src1, src2, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	result.float_value = src1.float_value / src2.float_value;
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_f_sqrt_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	rv32f_rep src1, result;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	
	result.float_value = (float)sqrt((double)src1.float_value);
	
	switch (rm) //TODO check rm value for rounding
	{
		
	}
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fsgnj_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t src1 = freg_read_riscv(E, S, rs1);
	uint32_t src2 = freg_read_riscv(E, S, rs2);
	
	uint32_t result = (src1 & (-1 - (1 << 31))) | (src2 & (1 << 31));
	
	freg_set_riscv(E, S, rd, result);
	
	return;
}

void rv32f_fsgnjn_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t src1 = freg_read_riscv(E, S, rs1);
	uint32_t src2 = freg_read_riscv(E, S, rs2);
	
	uint32_t result = (src1 & (-1 - (1 << 31))) | (~src2 & (1 << 31));
	
	freg_set_riscv(E, S, rd, result);
	
	return;
}

void rv32f_fsgnjx_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	uint32_t src1 = freg_read_riscv(E, S, rs1);
	uint32_t src2 = freg_read_riscv(E, S, rs2);
	
	uint32_t result = (src1 & (-1 - (1 << 31))) | ((src1 ^ src2) & (1 << 31));
	
	freg_set_riscv(E, S, rd, result);
	
	return;
}

void rv32f_fmin_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1, src2;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	freg_set_riscv(E, S, rd, (src1.float_value <= src2.float_value) ? src1.bit_value : src2.bit_value);
	
	/*if(src1.float_value <= src2.float_value) //TODO remove if unused
	{
		freg_set_riscv(E, S, rd, src1.bit_value);
	}
	else
	{
		freg_set_riscv(E, S, rd, src2.bit_value);
	}*/
	
	return;
}

void rv32f_fmax_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1, src2;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	freg_set_riscv(E, S, rd, (src1.float_value >= src2.float_value) ? src1.bit_value : src2.bit_value);
	
	/*if(src1.float_value >= src2.float_value) //TODO remove if unused
	{
		freg_set_riscv(E, S, rd, src1.bit_value);
	}
	else
	{
		freg_set_riscv(E, S, rd, src2.bit_value);
	}*/
	
	return;
}

void rv32f_fcvt_w_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1;
	
	uint8_t frm;
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	
	/*
	*Rounding modes - Reference
	*https://www.gnu.org/software/libc/manual/html_node/Rounding-Functions.html
	*/
	switch (rm) //TODO check rm value for rounding
	{
		case 0b000: //Round to nearest (ties to Even)
			reg_set_riscv(E, S, rd, (int32_t)rintf(src1.float_value));
			break;
		
		case 0b001: //Round towards zero
			reg_set_riscv(E, S, rd, (int32_t)truncf(src1.float_value));
			break;
		
		case 0b010:	//Round down (towards -inf)
			reg_set_riscv(E, S, rd, (int32_t)floorf(src1.float_value));
			break;
		
		case 0b011: //Round up (towards +inf)
			reg_set_riscv(E, S, rd, (int32_t)ceilf(src1.float_value));
			break;
		
		case 0b100: //Round to nearest (ties to Max Magnitude)
			reg_set_riscv(E, S, rd, (int32_t)roundf(src1.float_value));
			break;
		
		case 0b101: //Invalid. Reserved for future use
		case 0b110: //Do nothing for now
			break;
		
		case 0b111: //Dynamic rounding mode, read from frm
			frm = (S->riscv->fCSR & (0b111 << 5)) >> 5;
			switch (frm)
			{
				case 0b000: //Round to nearest (ties to Even)
					reg_set_riscv(E, S, rd, (int32_t)rintf(src1.float_value));
					break;
				
				case 0b001: //Round towards zero
					reg_set_riscv(E, S, rd, (int32_t)truncf(src1.float_value));
					break;
				
				case 0b010:	//Round down (towards -inf)
					reg_set_riscv(E, S, rd, (int32_t)floorf(src1.float_value));
					break;
				
				case 0b011: //Round up (towards +inf)
					reg_set_riscv(E, S, rd, (int32_t)ceilf(src1.float_value));
					break;
				
				case 0b100: //Round to nearest (ties to Max Magnitude)
					reg_set_riscv(E, S, rd, (int32_t)roundf(src1.float_value));
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
	
	return;
}

void rv32f_fcvt_wu_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	
	uint8_t frm;
	uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3;
	
	/*
	*Rounding modes - Reference
	*https://www.gnu.org/software/libc/manual/html_node/Rounding-Functions.html
	*/
	switch (rm) //TODO check rm value for rounding
	{
		case 0b000: //Round to nearest (ties to Even)
			reg_set_riscv(E, S, rd, (uint32_t)rintf(src1.float_value));
			break;
		
		case 0b001: //Round towards zero
			reg_set_riscv(E, S, rd, (uint32_t)truncf(src1.float_value));
			break;
		
		case 0b010:	//Round down (towards -inf)
			reg_set_riscv(E, S, rd, (uint32_t)floorf(src1.float_value));
			break;
		
		case 0b011: //Round up (towards +inf)
			reg_set_riscv(E, S, rd, (uint32_t)ceilf(src1.float_value));
			break;
		
		case 0b100: //Round to nearest (ties to Max Magnitude)
			reg_set_riscv(E, S, rd, (uint32_t)roundf(src1.float_value));
			break;
		
		case 0b101: //Invalid. Reserved for future use
		case 0b110: //Do nothing for now
			break;
		
		case 0b111: //Dynamic rounding mode, read from frm
			frm = (S->riscv->fCSR & (0b111 << 5)) >> 5;
			switch (frm)
			{
				case 0b000: //Round to nearest (ties to Even)
					reg_set_riscv(E, S, rd, (uint32_t)rintf(src1.float_value));
					break;
				
				case 0b001: //Round towards zero
					reg_set_riscv(E, S, rd, (uint32_t)truncf(src1.float_value));
					break;
				
				case 0b010:	//Round down (towards -inf)
					reg_set_riscv(E, S, rd, (uint32_t)floorf(src1.float_value));
					break;
				
				case 0b011: //Round up (towards +inf)
					reg_set_riscv(E, S, rd, (uint32_t)ceilf(src1.float_value));
					break;
				
				case 0b100: //Round to nearest (ties to Max Magnitude)
					reg_set_riscv(E, S, rd, (uint32_t)roundf(src1.float_value));
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
	
	return;
}

void rv32f_fmv_x_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	reg_set_riscv(E, S, rd, freg_read_riscv(E, S, rs1));
	
	return;
}

void rv32f_feq_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1, src2;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	reg_set_riscv(E, S, rd, (src1.float_value == src2.float_value) ? 1 : 0);
	
	return;
}

void rv32f_flt_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1, src2;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	reg_set_riscv(E, S, rd, (src1.float_value < src2.float_value) ? 1 : 0);
	
	return;
}

void rv32f_fle_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	rv32f_rep src1, src2;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	src2.bit_value = freg_read_riscv(E, S, rs2);
	
	reg_set_riscv(E, S, rd, (src1.float_value <= src2.float_value) ? 1 : 0);
	
	return;
}

void rv32f_fclass_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//https://www.gnu.org/software/libc/manual/html_node/Floating-Point-Classes.html
	//TODO
	uint32_t shift;
	rv32f_rep src1;
	
	src1.bit_value = freg_read_riscv(E, S, rs1);
	
	int class = fpclassify(src1.float_value);
	
	switch (class)
	{
		case FP_NAN:
			shift = (__issignaling(src1.float_value)) ? 8 : 9;
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
	
	return;
}

void rv32f_fcvt_s_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?
	
	rv32f_rep result; //TODO test this
	
	result.float_value = (float)reg_read_riscv(E, S, rs1); //cast to float
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fcvt_s_wu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
	//uint8_t rm = ((instr_r *)&S->riscv->P.EX.instr)->funct3; //TODO check why there is a rm field?
	
	rv32f_rep result; //TODO test this
	
	uint32_t src1 = reg_read_riscv(E, S, rs1);
	
	result.float_value = (float)src1; //cast to float
	
	freg_set_riscv(E, S, rd, result.bit_value);
	
	return;
}

void rv32f_fmv_w_x(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd)
{	
	freg_set_riscv(E, S, rd, reg_read_riscv(E, S, rs1));
	
	return;
}
