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
	MSP430_NREGS = 16,
};

enum
{
	MSP430_R0	= 0,
	MSP430_PC	= 0,

	MSP430_R1	= 1,
	MSP430_SP	= 1,

	MSP430_R2	= 2,
	MSP430_SR	= 2,
	MSP430_CG1	= 2,

	MSP430_R3	= 3,
	MSP430_CG2	= 3,

	MSP430_R4,
	MSP430_R5,
	MSP430_R6,
	MSP430_R7,
	MSP430_R8,
	MSP430_R9,
	MSP430_R10,
	MSP430_R11,
	MSP430_R12,
	MSP430_R13,
	MSP430_R14,
	MSP430_R15,
};

enum
{
	MSP430_AMODE_REG,	/*	Rn	*/
	MSP430_AMODE_IDX,	/*	X(Rn)	*/
	MSP430_AMODE_SYM,	/*	ADDR	*/
	MSP430_AMODE_ABS,	/*	&ADDR	*/
	MSP430_AMODE_IND,	/*	@Rn	*/
	MSP430_AMODE_INC,	/*	@Rn+	*/
	MSP430_AMODE_IMM,	/*	#N	*/
};

typedef struct
{
	unsigned	C:1;
	unsigned	Z:1;
	unsigned	N:1;
	unsigned	GIE:1;
	unsigned	CPU_OFF:1;
	unsigned	OSC_OFF:1;
	unsigned	SCG0:1;
	unsigned	SCG1:1;
	unsigned	V:1;
	unsigned	:7;	/*	reserved	*/
} MSP430SREG;

#define msp430_sreg_get_C(reg)			(((reg) >> 0) & B0001)
#define msp430_sreg_get_Z(reg)			(((reg) >> 1) & B0001)
#define msp430_sreg_get_N(reg)			(((reg) >> 2) & B0001)
#define msp430_sreg_get_GIE(reg)		(((reg) >> 3) & B0001)
#define msp430_sreg_get_CPU_OFF(reg)		(((reg) >> 4) & B0001)
#define msp430_sreg_get_OSC_OFF(reg)		(((reg) >> 5) & B0001)
#define msp430_sreg_get_SCG0(reg)		(((reg) >> 6) & B0001)
#define msp430_sreg_get_SCG1(reg)		(((reg) >> 7) & B0001)
#define msp430_sreg_get_V(reg)			(((reg) >> 8) & B0001)

#define msp430_sreg_setval_C(reg, val)		((reg) = (reg)|(((val)&1) << 0))
#define msp430_sreg_setval_Z(reg, val)		((reg) = (reg)|(((val)&1) << 1))
#define msp430_sreg_setval_N(reg, val)		((reg) = (reg)|(((val)&1) << 2))
#define msp430_sreg_setval_GIE(reg, val)	((reg) = (reg)|(((val)&1) << 3))
#define msp430_sreg_setval_CPU_OFF(reg, val)	((reg) = (reg)|(((val)&1) << 4))
#define msp430_sreg_setval_OSC_OFF(reg, val)	((reg) = (reg)|(((val)&1) << 5))
#define msp430_sreg_setval_SCG0(reg, val)	((reg) = (reg)|(((val)&1) << 6))
#define msp430_sreg_setval_SCG1(reg, val)	((reg) = (reg)|(((val)&1) << 7))
#define msp430_sreg_setval_V(reg, val)		((reg) = (reg)|(((val)&1) << 8))

#define msp430_sreg_set_C(reg)			((reg) = (reg)|(1 << 0))
#define msp430_sreg_set_Z(reg)			((reg) = (reg)|(1 << 1))
#define msp430_sreg_set_N(reg)			((reg) = (reg)|(1 << 2))
#define msp430_sreg_set_GIE(reg)		((reg) = (reg)|(1 << 3))
#define msp430_sreg_set_CPU_OFF(reg)		((reg) = (reg)|(1 << 4))
#define msp430_sreg_set_OSC_OFF(reg)		((reg) = (reg)|(1 << 5))
#define msp430_sreg_set_SCG0(reg)		((reg) = (reg)|(1 << 6))
#define msp430_sreg_set_SCG1(reg)		((reg) = (reg)|(1 << 7))
#define msp430_sreg_set_V(reg)			((reg) = (reg)|(1 << 8))

#define msp430_sreg_clr_C(reg)			((reg) = (reg)& ~(1 << 0))
#define msp430_sreg_clr_Z(reg)			((reg) = (reg)& ~(1 << 1))
#define msp430_sreg_clr_N(reg)			((reg) = (reg)& ~(1 << 2))
#define msp430_sreg_clr_GIE(reg)		((reg) = (reg)& ~(1 << 3))
#define msp430_sreg_clr_CPU_OFF(reg)		((reg) = (reg)& ~(1 << 4))
#define msp430_sreg_clr_OSC_OFF(reg)		((reg) = (reg)& ~(1 << 5))
#define msp430_sreg_clr_SCG0(reg)		((reg) = (reg)& ~(1 << 6))
#define msp430_sreg_clr_SCG1(reg)		((reg) = (reg)& ~(1 << 7))
#define msp430_sreg_clr_V(reg)			((reg) = (reg)& ~(1 << 8))

#define msp430_SFR_IE1_get_ACCVIE(reg)		(((reg) >> 5) & B0001)
#define msp430_SFR_IE1_get_NMIIE(reg)		(((reg) >> 4) & B0001)
#define msp430_SFR_IE1_get_OFIE(reg)		(((reg) >> 1) & B0001)
#define msp430_SFR_IE1_get_WDTIE(reg)		(((reg) >> 0) & B0001)

#define msp430_SFR_IE2_get_UTXIE0(reg)		(((reg) >> 1) & B0001)
#define msp430_SFR_IE2_get_URXIE0(reg)		(((reg) >> 0) & B0001)

#define msp430_SFR_IFG1_get_NMIIFG(reg)		(((reg) >> 4) & B0001)
#define msp430_SFR_IFG1_get_OFIFG(reg)		(((reg) >> 1) & B0001)
#define msp430_SFR_IFG1_get_WDTIFG(reg)		(((reg) >> 0) & B0001)

#define msp430_SFR_IFG2_get_UTXIFG0(reg)	(((reg) >> 1) & B0001)
#define msp430_SFR_IFG2_get_URXIFG0(reg)	(((reg) >> 0) & B0001)

#define msp430_SFR_ME2_get_UTXE0(reg)		(((reg) >> 1) & B0001)
#define msp430_SFR_ME2_get_URXE0(reg)		(((reg) >> 0) & B0001)
/*	alias of the above	*/
#define msp430_SFR_ME2_get_USPIE0(reg)		(((reg) >> 0) & B0001)


#define	msp430_WDTCTL_get_NMIES(data)		(((data) >> 6) & 0x1)




/*
msp430_FCTL3_set_ACCVIFG
msp430_SFR_IFG1_set_NMIIFG
msp430_SFR_IFG1_set_OSFIFG
msp430_WDTCTL_get_NMIES
*/
