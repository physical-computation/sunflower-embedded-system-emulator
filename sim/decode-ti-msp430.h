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

enum
{
	MSP430_OP_ADD,
	MSP430_OP_ADDB,
	MSP430_OP_ADDC,
	MSP430_OP_ADDCB,
	MSP430_OP_AND,
	MSP430_OP_ANDB,
	MSP430_OP_BIC,
	MSP430_OP_BICB,
	MSP430_OP_BIS,
	MSP430_OP_BISB,
	MSP430_OP_BIT,
	MSP430_OP_BITB,
	MSP430_OP_CMP,
	MSP430_OP_CMPB,
	MSP430_OP_DADD,
	MSP430_OP_DADDB,
	MSP430_OP_MOV,
	MSP430_OP_MOVB,
	MSP430_OP_SUB,
	MSP430_OP_SUBB,
	MSP430_OP_SUBC,
	MSP430_OP_SUBCB,
	MSP430_OP_XOR,
	MSP430_OP_XORB,

	MSP430_OP_CALL,
	MSP430_OP_PUSH,
	MSP430_OP_PUSHB,
	MSP430_OP_RETI,
	MSP430_OP_RRA,
	MSP430_OP_RRAB,
	MSP430_OP_RRC,
	MSP430_OP_RRCB,
	MSP430_OP_SWPB,
	MSP430_OP_SXT,

	MSP430_OP_JC,
	MSP430_OP_JEQJZ,
	MSP430_OP_JGE,
	MSP430_OP_JL,
	MSP430_OP_JMP,
	MSP430_OP_JN,
	MSP430_OP_JNC,
	MSP430_OP_JNEJZ,

	MSP430_OP_MAX,
};

#define msp430_instrI_opcode(instr)	((instr) >> 12)
#define msp430_instrI_sreg(instr)	(((instr) >> 8) & B1111)
#define msp430_instrI_ad(instr)		(((instr) >> 7) & B0001)
#define msp430_instrI_bw(instr)		(((instr) >> 6) & B0001)
#define msp430_instrI_as(instr)		(((instr) >> 4) & B0011)
#define msp430_instrI_dreg(instr)	((instr) & B1111)

#define msp430_instrII_opcode(instr)	((instr) >> 7)
#define msp430_instrII_bw(instr)	(((instr) >> 6) & B0001)
#define msp430_instrII_ad(instr)	(((instr) >> 4) & B0011)
#define msp430_instrII_dsreg(instr)	((instr) & B1111)

#define msp430_instrIII_opcode(instr)	((instr) >> 13)
#define msp430_instrIII_c(instr)	(((instr) >> 10) & B0111)
#define msp430_instrIII_offset(instr)	(((instr) >> 4) & B00000001111111111)
