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

enum
{
	/*
	 *	RISCV Instruction formats (types)
	 *	These determine the meaning of the individual bits of the 32b instructions
	 *	-> Fig. 2.3, pg. 12 in https://content.riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf
	 *	-> or https://en.wikipedia.org/wiki/RISC-V#ISA_base_and_extensions for a nicer figure
	 */
	INSTR_R, // Register-register
	INSTR_I, // register-Immediate
	INSTR_S, // Store
	INSTR_B, // Branch
	INSTR_U, // Upper-immediate
	INSTR_J, // Jump

	/*
	 *	Additional R4 type introduced in RV32F extension
	 *	These are fused-* (e.g. fused multiply add) instruction formats
	 */
	INSTR_R4,

	/*
	 *	We introduce this new type to accommodate for any invalid instruction,
	 *	including 0. These instructions will be executed as nop
	 */
	INSTR_N
};

/*	These structs are currently used in fp operations in op-riscv.c.
	They are not used in sf-riscv.y with the RISC-V inline assembler
	implementation as we chose to stop using bit fields.			*/

/*
 *	Variable naming convention:
 *
 *	 bitfield_name:length
 *
 *	where bitfield_name is one of
 *
 *	 rs{1,2} and rd: source, destination registers
 *
 *	 funct{3,7} and opcode: instruction specifiers
 *
 *	 immN: immediate value, with N being the starting bit
 *	 	(NB this means our naming convention differs from the spec, we give start:length
 *	 	    rather than start:end as in the spec)
 */
typedef struct
{
	unsigned	opcode:7;
	unsigned	rd:5;
	unsigned	funct3:3;
	unsigned	rs1:5;
	unsigned	rs2:5;
	unsigned	funct7:7;
} instr_r;

typedef struct
{
	unsigned	opcode:7;
	unsigned	rd:5;
	unsigned	funct3:3;
	unsigned	rs1:5;
	unsigned	imm0:12;
} instr_i;

typedef struct
{
	unsigned	opcode:7;
	unsigned	imm0:5;
	unsigned	funct3:3;
	unsigned	rs1:5;
	unsigned	rs2:5;
	unsigned	imm5:7;
} instr_s;

typedef struct
{
	unsigned	opcode:7;
	unsigned	imm11:1;
	unsigned	imm1:4;
	unsigned	funct3:3;
	unsigned	rs1:5;
	unsigned	rs2:5;
	unsigned	imm5:6;
	unsigned	imm12:1;
} instr_b;

typedef struct
{
	unsigned	opcode:7;
	unsigned	rd:5;
	unsigned	imm0:20;
} instr_u;

typedef struct
{
	unsigned	opcode:7;
	unsigned	rd:5;
	unsigned	imm12:8;
	unsigned	imm11:1;
	unsigned	imm1:10;
	unsigned	imm20:1;
} instr_j;

typedef struct
{
	unsigned	opcode:7;
	unsigned	misc_lo:5;
	unsigned	funct3:3;
	unsigned	misc_md:5;
	unsigned	b20:1;
	unsigned	misc_hi:4;
	unsigned	funct7:7;
   
} instr_riscv_decode;

typedef struct
{
	unsigned	opcode:7;
	unsigned	rd:5;
	unsigned	rm:3;
	unsigned	rs1:5;
	unsigned	rs2:5;
	unsigned	unused:2;
	unsigned	rs3:5;
} instr_r4;
