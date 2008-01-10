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
#elif SF_B_ENDIAN
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
