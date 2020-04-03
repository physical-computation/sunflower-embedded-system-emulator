/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
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

/*	decode-riscv.h and opstr-riscv.h are dependent on this file	*/
int riscv_instr_latencies[][5] =\
{
	[RISCV_OP_LUI]		{1,	1,	1,	1,	1},//WB stage is always 1 cycle
	[RISCV_OP_AUIPC]	{1,	1,	1,	1,	1},
	[RISCV_OP_JAL]		{1,	1,	1,	1,	1},
	[RISCV_OP_JALR]		{1,	1,	1,	1,	1},
	[RISCV_OP_BEQ]		{1,	1,	1,	1,	1},
	[RISCV_OP_BNE]		{1,	1,	1,	1,	1},
	[RISCV_OP_BLT]		{1,	1,	1,	1,	1},
	[RISCV_OP_BGE]		{1,	1,	1,	1,	1},
	[RISCV_OP_BLTU]		{1,	1,	1,	1,	1},
	[RISCV_OP_BGEU]		{1,	1,	1,	1,	1},
	[RISCV_OP_LB]		{1,	1,	1,	1,	1},
	[RISCV_OP_LH]		{1,	1,	1,	1,	1},
	[RISCV_OP_LW]		{1,	1,	1,	1,	1},
	[RISCV_OP_LBU]		{1,	1,	1,	1,	1},
	[RISCV_OP_LHU]		{1,	1,	1,	1,	1},
	[RISCV_OP_SB]		{1,	1,	1,	1,	1},
	[RISCV_OP_SH]		{1,	1,	1,	1,	1},
	[RISCV_OP_SW]		{1,	1,	1,	1,	1},
	[RISCV_OP_ADDI]		{1,	1,	1,	1,	1},
	[RISCV_OP_SLTI]		{1,	1,	1,	1,	1},
	[RISCV_OP_SLTIU]	{1,	1,	1,	1,	1},
	[RISCV_OP_XORI]		{1,	1,	1,	1,	1},
	[RISCV_OP_ORI]		{1,	1,	1,	1,	1},
	[RISCV_OP_ANDI]		{1,	1,	1,	1,	1},
	[RISCV_OP_SLLI]		{1,	1,	1,	1,	1},
	[RISCV_OP_SRLI]		{1,	1,	1,	1,	1},
	[RISCV_OP_SRAI]		{1,	1,	1,	1,	1},
	[RISCV_OP_ADD]		{1,	1,	1,	1,	1},
	[RISCV_OP_SUB]		{1,	1,	1,	1,	1},
	[RISCV_OP_SLL]		{1,	1,	1,	1,	1},
	[RISCV_OP_SLT]		{1,	1,	1,	1,	1},
	[RISCV_OP_SLTU]		{1,	1,	1,	1,	1},
	[RISCV_OP_XOR]		{1,	1,	1,	1,	1},
	[RISCV_OP_SRL]		{1,	1,	1,	1,	1},
	[RISCV_OP_SRA]		{1,	1,	1,	1,	1},
	[RISCV_OP_OR]		{1,	1,	1,	1,	1},
	[RISCV_OP_AND]		{1,	1,	1,	1,	1},
	[RISCV_OP_FENCE]	{1,	1,	1,	1,	1},
	[RISCV_OP_FENCE_I]	{1,	1,	1,	1,	1},
	[RISCV_OP_ECALL]	{1,	1,	1,	1,	1},
	[RISCV_OP_EBREAK]	{1,	1,	1,	1,	1},
	[RISCV_OP_CSRRW]	{1,	1,	1,	1,	1},
	[RISCV_OP_CSRRS]	{1,	1,	1,	1,	1},
	[RISCV_OP_CSRRC]	{1,	1,	1,	1,	1},
	[RISCV_OP_CSRRWI]	{1,	1,	1,	1,	1},
	[RISCV_OP_CSRRSI]	{1,	1,	1,	1,	1},
	[RISCV_OP_CSRRCI]	{1,	1,	1,	1,	1},
	[RISCV_OP_MUL]		{1,	1,	1,	1,	1},
	[RISCV_OP_MULH]		{1,	1,	1,	1,	1},
	[RISCV_OP_MULHSU]	{1,	1,	1,	1,	1},
	[RISCV_OP_MULHU]	{1,	1,	1,	1,	1},
	[RISCV_OP_DIV]		{1,	1,	1,	1,	1},
	[RISCV_OP_DIVU]		{1,	1,	1,	1,	1},
	[RISCV_OP_REM]		{1,	1,	1,	1,	1},
	[RISCV_OP_REMU]		{1,	1,	1,	1,	1},
	/*[RISCV_OP_MAX]	square brackets are necessary	*/
	/* RV32F */
	[RV32F_OP_FLW]		{1,	1,	1,	1,	1},
	[RV32F_OP_FSW]		{1,	1,	1,	1,	1},
	[RV32F_OP_FMADDS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FMSUBS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FNMSUBS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FNMADDS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FADDS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FSUBS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FMULS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FDIVS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FSQRTS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FSGNJS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FSGNJNS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FSGNJXS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FMINS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FMAXS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FCVTWS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FCVTWUS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FMVXW]	{1,	1,	1,	1,	1},
	[RV32F_OP_FEQS]		{1,	1,	1,	1,	1},
	[RV32F_OP_FLTS]		{1,	1,	1,	1,	1},
	[RV32F_OP_FLES]		{1,	1,	1,	1,	1},
	[RV32F_OP_FCLASSS]	{1,	1,	1,	1,	1},
	[RV32F_OP_FCVTSW]	{1,	1,	1,	1,	1},
	[RV32F_OP_FCVTSWU]	{1,	1,	1,	1,	1},
	[RV32F_OP_FMVWX]	{1,	1,	1,	1,	1},
	/*[RV32F_OP_FMAX]	square brackets are necessary	*/
	/* RV32D */
	[RV32D_OP_FLD]		{1,	1,	1,	1,	1},
	[RV32D_OP_FSD]		{1,	1,	1,	1,	1},
	[RV32D_OP_FMADDD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FMSUBD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FNMSUBD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FNMADDD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FADDD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FSUBD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FMULD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FDIVD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FSQRTD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FSGNJD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FSGNJND]	{1,	1,	1,	1,	1},
	[RV32D_OP_FSGNJXD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FMIND]	{1,	1,	1,	1,	1},
	[RV32D_OP_FMAXD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FCVTSD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FCVTDS]	{1,	1,	1,	1,	1},
	[RV32D_OP_FEQD]		{1,	1,	1,	1,	1},
	[RV32D_OP_FLTD]		{1,	1,	1,	1,	1},
	[RV32D_OP_FLED]		{1,	1,	1,	1,	1},
	[RV32D_OP_FCLASSD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FCVTWD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FCVTWUD]	{1,	1,	1,	1,	1},
	[RV32D_OP_FCVTDW]	{1,	1,	1,	1,	1},
	[RV32D_OP_FCVTDWU]	{1,	1,	1,	1,	1},
	/*[RV32D_OP_DMAX]	square brackets are necessary	*/

	[RV32UN_OP_UNUPGS]	{1,	1,	1,	1,	1},
	[RV32UN_OP_UNGCOVS]	{1,	1,	1,	1,	1},
	[RV32UN_OP_UNSVARS]	{1,	1,	1,	1,	1},
	[RV32UN_OP_UNCLVARS]	{1,	1,	1,	1,	1},
	[RV32UN_OP_UNCPVARS]	{1,	1,	1,	1,	1},
	[RV32UN_OP_UNPART1]	{1,	1,	1,	1,	1},

	/*[RV32UN_OP_UNMAX]	square brackets are necessary	*/
};
