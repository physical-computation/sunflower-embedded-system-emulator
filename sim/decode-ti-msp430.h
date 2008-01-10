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
