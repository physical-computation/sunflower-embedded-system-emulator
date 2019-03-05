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
/*		Definitions for registers of the SH			*/
/*									*/
/*	In usermode, R0_BANK0 - R7_BANK0 and R8-R15, SR, GBR, MACH, 	*/
/*	MACL, PR and PC are the available registers. In previledged 	*/
/*	mode, either R0_BANK0 - R7_BANK0 (R0_BANK1 - R7_BANK1) 		*/
/* 	if SR.RB equals 0 (1) and R8-R15, SR, GBR, MACH, MACL, PR, VBR	*/
/*	PC, SPC are the available regs, and R0_BANK0 - R7_BANK0		*/ 
/*	(R0_BANK0 - R7_BANK0) are available through LDC/STC instrs.	*/
/*									*/
/*	Implementation : If SR.RB = 0, then we use R[0 - 16]. This is	*/
/*	always true in user mode. In supervisor mode, if SR.RB = 0,	*/
/*	we use R[0 - 16] and access R_BANK[0 - 7] through LDC/STC	*/
/*	instructions. If SR.RB = 1 in supervisor mode, our register 	*/
/*	set is R_BANK[0 - 7] and R[8 - 16], and R[0 - 7] are accessed 	*/
/*	through LDC/STC instructions. To make things simple, all 	*/
/*	modifications of registers should be done through the 		*/
/*	reg_read() and reg_set() routines, which will do the 		*/
/*	appropriate checking to see what the current SR.RB is etc. The	*/
/*	exception is in the implementation of the LDCR_BANK etc instrs	*/
/*	where we explicitly manipulate the register file.		*/
/*									*/

enum
{
	/*	gpr + banked + system	   */
	SUPERH_NREGS = 16+8+15,
};

#ifdef SF_L_ENDIAN
typedef struct
{
	/* !!!!!! ARCHITECTURE DEPENDENT !!!! 	*/
	/* 	Lowest mem addr (LSB on lil-n) 	*/
	unsigned	T:1;
	unsigned	S:1;
	unsigned   	:2; 	/* pad */
	unsigned	IMASK:4;
	unsigned	Q:1;
	unsigned	M:1;
	unsigned	:18;	/* pad */
	unsigned	BL:1;
	unsigned	RB:1;
	unsigned	MD:1;
	unsigned	:1;	/* pad */
	/* 	Highest mem addr (MSB on lil-n) */
} SuperHSREG;
#else
#ifdef SF_B_ENDIAN
typedef struct
{
	/* 	Lowest mem addr (MSB on big-n) 	*/
	unsigned	:1;	/* pad */
	unsigned	MD:1;
	unsigned	RB:1;
	unsigned	BL:1;
	unsigned	:18;	/* pad */
	unsigned	M:1;
	unsigned	Q:1;
	unsigned	IMASK:4;
	unsigned   	:2; 	/* pad */
	unsigned	S:1;
	unsigned	T:1;
	/* 	Highest mem addr (LSB on big-n) */
} SuperHSREG;
#else
#error "You must define your host machines endianness"
#error "in config.$(OSTYPE)."
#error "use -DSF_L_ENDIAN for little-endian machines"
#error "and -DSF_B_ENDIAN for big-endian machines"
#error "Files containing endian-dependent code are"
#error "endian.h and regs.h"
#endif
#endif

enum
{
	S_R0,
	S_R1,
	S_R2,
	S_R3,
	S_R4,
	S_R5,
	S_R6,
	S_R7,
	S_RBANK0,
	S_RBANK1,
	S_RBANK2,
	S_RBANK3,
	S_RBANK4,
	S_RBANK5,
	S_RBANK6,
	S_RBANK7,
	S_R8,
	S_R9,
	S_R10,
	S_R11,
	S_R12,
	S_R13,
	S_R14,
	S_R15,
	S_GBR,
	S_VBR,
	S_MACH,
	S_MACL,
	S_PC,
	S_PR,
	S_TRA,
	S_SPC,
	S_SR,
	S_SSR,
};
