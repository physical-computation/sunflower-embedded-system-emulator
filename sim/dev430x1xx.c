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

#include "sf.h"


#define	sp8(w, v)	(S->msp430->periph8[(w) - MSP430_PERIPH8_BEGIN] = (v))
#define	sp16(w, v)	(S->msp430->periph16[((w) - MSP430_PERIPH8_BEGIN) >> 1] = (v))


static void	reset_basicclock(State *S, int type);
static void	reset_flashmemctrlr(State *S, int type);
static void	reset_svsctrlr(State *S, int type);
static void	reset_dmactrlr(State *S, int type);
static void	reset_io(State *S, int type);
static void	reset_wdt(State *S, int type);
static void	reset_timera(State *S, int type);
static void	reset_timerb(State *S, int type);
static void	reset_usart(State *S, int type);
static void	reset_i2c(State *S, int type);
static void	reset_comparator(State *S, int type);
static void	reset_adc12(State *S, int type);
static void	reset_adc10(State *S, int type);
static void	reset_dac12(State *S, int type);




tuck void
reset_basicclock(State *S, int type)
{
	/*	See Section 4.3 of slau049e.pdf		*/
	if (type == MSP430_RESET_PUC)
	{
		sp8(MSP430_PERIPH8_DCOCTL, 0x0060);
		sp8(MSP430_PERIPH8_BCSCTL1, 0x0084);
		S->msp430->ie1 	= 0x0000;
		S->msp430->ifg1	= 0x0000;
	}
	else if (type == MSP430_RESET_POR)
	{
		sp8(MSP430_PERIPH8_BCSCTL2, 0x0000);
	}

	return;
}

tuck void
reset_flashmemctrlr(State *S, int type)
{
	/*	See Section 5.4 of slau049e.pdf		*/
	if (type == MSP430_RESET_PUC)
	{
		sp16(MSP430_PERIPH16_FCTL1, 0x9600);
		sp16(MSP430_PERIPH16_FCTL2, 0x9642);
		sp16(MSP430_PERIPH16_FCTL3, 0x9618);
		S->msp430->ie1 = 0x0000;
	}

	return;
}
	
tuck void
reset_svsctrlr(State *S, int type)
{
	/*	See Figure 6-1 of slau049e.pdf		*/
	if (type == MSP430_RESET_BOR) /*  Brown-out  */
	{
		sp8(MSP430_PERIPH8_SVSCTL, 0x0000);
	}

	return;
}

tuck void
reset_dmactrlr(State *S, int type)
{
	/*	See Section 8.3 of slau049e.pdf		*/
	if (type == MSP430_RESET_POR)
	{
		/*	Only these (of the 14 DMA regs) get reset on POR	*/
		sp16(MSP430_PERIPH16_DMA2CTL, 0x0000);
		sp16(MSP430_PERIPH16_DMA1CTL, 0x0000);
		sp16(MSP430_PERIPH16_DMA0CTL, 0x0000);
		sp16(MSP430_PERIPH16_DMACTL1, 0x0000);
		sp16(MSP430_PERIPH16_DMACTL0, 0x0000);
	}

	return;
}
	
tuck void
reset_io(State *S, int type)
{
	/*	See Section 9.3 of slau049e.pdf		*/
	if (type == MSP430_RESET_PUC)
	{
		sp8(MSP430_PERIPH8_P1DIR, 0x0000);
		sp8(MSP430_PERIPH8_P1IE, 0x0000);
		sp8(MSP430_PERIPH8_P1IFG, 0x0000);
		sp8(MSP430_PERIPH8_P1SEL, 0x0000);

		sp8(MSP430_PERIPH8_P2DIR, 0x0000);
		sp8(MSP430_PERIPH8_P2IE, 0x0000);
		sp8(MSP430_PERIPH8_P2IFG, 0x0000);
		sp8(MSP430_PERIPH8_P2SEL, 0x0000);

		sp8(MSP430_PERIPH8_P3DIR, 0x0000);
		sp8(MSP430_PERIPH8_P3SEL, 0x0000);

		sp8(MSP430_PERIPH8_P4DIR, 0x0000);
		sp8(MSP430_PERIPH8_P4SEL, 0x0000);

		sp8(MSP430_PERIPH8_P5DIR, 0x0000);
		sp8(MSP430_PERIPH8_P5SEL, 0x0000);

		sp8(MSP430_PERIPH8_P6DIR, 0x0000);
		sp8(MSP430_PERIPH8_P6SEL, 0x0000);
	}

	return;
}

tuck void
reset_wdt(State *S, int type)
{
	/*	See Section 10.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_PUC)
	{
		sp16(MSP430_PERIPH16_WDTCTL, 0x6900);
		S->msp430->ie1	= 0x0000;
		S->msp430->ifg1	= 0x0000;
	}
	else if (type == MSP430_RESET_POR)
	{
		/*	Clear WDTIFG (bit 0) in IFG1	*/
		S->msp430->ifg1 &= ~(1 << 0);
	}

	return;
}

tuck void
reset_timera(State *S, int type)
{
	/*	See Section 11.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_POR)
	{
		sp16(MSP430_PERIPH16_TACTL, 0x0000);
		sp16(MSP430_PERIPH16_TAR, 0x0000);
		sp16(MSP430_PERIPH16_TACCTL0, 0x0000);
		sp16(MSP430_PERIPH16_TACCR0,	0x0000);
		sp16(MSP430_PERIPH16_TACCTL1, 0x0000);
		sp16(MSP430_PERIPH16_TACCR1,	0x0000);
		sp16(MSP430_PERIPH16_TACCTL2, 0x0000);
		sp16(MSP430_PERIPH16_TACCR2,	0x0000);
		sp16(MSP430_PERIPH16_TAIV, 0x0000);
	}

	return;
}

tuck void
reset_timerb(State *S, int type)
{
	/*	See Section 12.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_POR)
	{
		sp16(MSP430_PERIPH16_TBCTL, 0x0000);
		sp16(MSP430_PERIPH16_TBR, 0x0000);
		sp16(MSP430_PERIPH16_TBCCTL0, 0x0000);
		sp16(MSP430_PERIPH16_TBCCCR0, 0x0000);
		sp16(MSP430_PERIPH16_TBCCTL1, 0x0000);
		sp16(MSP430_PERIPH16_TBCCR1,	0x0000);
		sp16(MSP430_PERIPH16_TBCCTL2, 0x0000);
		sp16(MSP430_PERIPH16_TBCCR2,	0x0000);
		sp16(MSP430_PERIPH16_TBCCTL3, 0x0000);
		sp16(MSP430_PERIPH16_TBCCR3,	0x0000);
		sp16(MSP430_PERIPH16_TBCCTL4, 0x0000);
		sp16(MSP430_PERIPH16_TBCCR4,	0x0000);
		sp16(MSP430_PERIPH16_TBCCTL5, 0x0000);
		sp16(MSP430_PERIPH16_TBCCR5,	0x0000);
		sp16(MSP430_PERIPH16_TBCCTL6, 0x0000);
		sp16(MSP430_PERIPH16_TBCCR6,	0x0000);
		sp16(MSP430_PERIPH16_TBIV, 0x0000);
	}

	return;
}

tuck void
reset_usart(State *S, int type)
{
	/*	See Section 13.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_PUC)
	{
		sp8(MSP430_PERIPH8_U0CTL, 0x0001);
		sp8(MSP430_PERIPH8_U0TCTL, 0x0001);
		sp8(MSP430_PERIPH8_U0RCTL, 0x0000);

		sp8(MSP430_PERIPH8_U1CTL, 0x0001);
		sp8(MSP430_PERIPH8_U1TCTL, 0x0001);
		sp8(MSP430_PERIPH8_U1RCTL, 0x0000);

		S->msp430->me1	= 0x0000;
		S->msp430->ie1	= 0x0000;
		S->msp430->ifg1	= 0x0082;

		S->msp430->me2	= 0x0000;
		S->msp430->ie2	= 0x0000;
		S->msp430->ifg2	= 0x0020;
	}

	return;
}

tuck void
reset_i2c(State *S, int type)
{
	/*	See Section 15.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_PUC)
	{
		sp8(MSP430_PERIPH8_I2CIE, 0x0000);
		sp8(MSP430_PERIPH8_I2CIFG, 0x0000);
		sp8(MSP430_PERIPH8_I2CNDAT, 0x0000);
		sp8(MSP430_PERIPH8_I2CTCTL, 0x0000);
		sp8(MSP430_PERIPH8_I2CDCTL, 0x0000);
		sp8(MSP430_PERIPH8_I2CPSC, 0x0000);
		sp8(MSP430_PERIPH8_I2CSCLH, 0x0000);
		sp8(MSP430_PERIPH8_I2CSCLL, 0x0000);
		sp8(MSP430_PERIPH8_I2CDRWB, 0x0000);

		sp16(MSP430_PERIPH16_I2COA, 0x0000);
		sp16(MSP430_PERIPH16_I2CSA, 0x0000);
		sp16(MSP430_PERIPH16_I2CIV, 0x0000);
	}

	return;
}


tuck void
reset_comparator(State *S, int type)
{
	/*	See Section 16.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_POR)
	{
		sp8(MSP430_PERIPH8_CACTL1, 0x0000);
		sp8(MSP430_PERIPH8_CACTL2, 0x0000);
		sp8(MSP430_PERIPH8_CAPD, 0x0000);
	}

	return;
}

tuck void
reset_adc12(State *S, int type)
{
	/*	See Section 17.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_POR)
	{
		sp16(MSP430_PERIPH16_ADC12CTL0, 0x0000);
		sp16(MSP430_PERIPH16_ADC12CTL1, 0x0000);
		sp16(MSP430_PERIPH16_ADC12IFG, 0x0000);
		sp16(MSP430_PERIPH16_ADC12IE, 0x0000);
		sp16(MSP430_PERIPH16_ADC12IV, 0x0000);

		sp8(MSP430_PERIPH8_ADC12MCTL0, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL1, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL2, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL3, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL4, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL5, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL6, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL7, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL8, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL9, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL10, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL11, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL12, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL13, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL14, 0x0000);
		sp8(MSP430_PERIPH8_ADC12MCTL15, 0x0000);
	}

	return;
}

tuck void
reset_adc10(State *S, int type)
{
	/*	See Section 18.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_POR)
	{
		sp8(MSP430_PERIPH8_ADC10AE, 0x0000);
		sp16(MSP430_PERIPH16_ADC10CTL0, 0x0000);
		sp16(MSP430_PERIPH16_ADC10CTL1, 0x0000);
		sp8(MSP430_PERIPH8_ADC10DTC0, 0x0000);
		sp8(MSP430_PERIPH8_ADC10DTC1, 0x0000);
		sp16(MSP430_PERIPH16_ADC10SA, 0x0200);
	}

	return;
}

tuck void
reset_dac12(State *S, int type)
{
	/*	See Section 19.3 of slau049e.pdf	*/
	if (type == MSP430_RESET_POR)
	{
		sp16(MSP430_PERIPH16_DAC12_0CTL, 0x0000);
		sp16(MSP430_PERIPH16_DAC12_0DAT, 0x0000);
		sp16(MSP430_PERIPH16_DAC12_1CTL, 0x0000);
		sp16(MSP430_PERIPH16_DAC12_1DAT, 0x0000);
	}

	return;
}

void
dev430x1xxreset(State *S, int type)
{
	reset_basicclock(S, type);
	reset_flashmemctrlr(S, type);
	reset_svsctrlr(S, type);
	reset_dmactrlr(S, type);
	reset_io(S, type);
	reset_wdt(S, type);
	reset_timera(S, type);
	reset_timerb(S, type);
	reset_usart(S, type);
	reset_i2c(S, type);
	reset_comparator(S, type);
	reset_adc12(S, type);
	reset_adc10(S, type);
	reset_dac12(S, type);

	return;
}

uchar
dev430x1xxreadbyte(Engine *E, State *S, ushort addr)
{
	/*								*/
	/*	If this is a byte read to peripheral module space,	*/
	/*	and it is to the 16-bit periphs, then a read from 	*/
	/*	an odd address returns 0, and a read from even	 	*/
	/*	address returns lower byte.				*/
	/*								*/

	if ((addr >= MSP430_PERIPH8_BEGIN) && (addr <= MSP430_PERIPH8_END))
	{
		return S->msp430->periph8[addr - MSP430_PERIPH8_BEGIN];
	}
	else if ((addr >= MSP430_PERIPH16_BEGIN) && (addr <= MSP430_PERIPH16_END))
	{
		if (addr & B0001)
		{
			return 0;
		}

		return (uchar)S->msp430->periph16[(addr - MSP430_PERIPH16_BEGIN) >> 1];
	}
	else if ((addr >= MSP430_SFR_BEGIN) && (addr <= MSP430_SFR_END))
	{
		return S->msp430->sfr[addr - MSP430_SFR_BEGIN];
	}
	sfatal(E, S, "Address not in device registers");

	return -1;
}

void
dev430x1xxwritebyte(Engine *E, State *S, ushort addr, uchar data)
{
	if ((addr >= MSP430_PERIPH8_BEGIN) && (addr <= MSP430_PERIPH8_END))
	{
		S->msp430->periph8[addr - MSP430_PERIPH8_BEGIN] = data;
	}
	else if ((addr >= MSP430_PERIPH16_BEGIN) && (addr <= MSP430_PERIPH16_END))
	{
		/*	See slau049e.pdf, 1.4.3		*/
		if (addr & B0001)
		{
			S->msp430->periph16[(addr - MSP430_PERIPH16_BEGIN) >> 1] = 0;
		}
		else
		{
			S->msp430->periph16[(addr - MSP430_PERIPH16_BEGIN) >> 1] = data;
		}
	}
	else if ((addr >= MSP430_SFR_BEGIN) && (addr <= MSP430_SFR_END))
	{
		S->msp430->sfr[addr - MSP430_SFR_BEGIN] = data;
	}
	else
	{
		sfatal(E, S, "Address not in device registers");
	}

	return;
}

ushort
dev430x1xxreadword(Engine *E, State *S, ushort addr)
{
	/*								*/
	/*	If this is a read from the 8 bit peripheral space,	*/
	/*	high byte officially returns unpredictable data (not	*/
	/*	the next register). All word accesses must be to	*/
	/*	even addresses. Architecture does not define behavior 	*/
	/*	when word access happens to odd address (non-aligned 	*/
	/*	word access).						*/
	/*								*/

	if ((addr >= MSP430_PERIPH8_BEGIN) && (addr <= MSP430_PERIPH8_END))
	{
		return (ushort)S->msp430->periph8[addr - MSP430_PERIPH8_BEGIN];
	}
	else if ((addr >= MSP430_PERIPH16_BEGIN) && (addr <= MSP430_PERIPH16_END))
	{
		if (addr & B0001)
		{
			return 0;
		}

		return S->msp430->periph16[(addr - MSP430_PERIPH16_BEGIN) >> 1];
	}
	else if ((addr >= MSP430_SFR_BEGIN) && (addr <= MSP430_SFR_END))
	{
		return (ushort)S->msp430->sfr[addr - MSP430_SFR_BEGIN];
	}
	sfatal(E, S, "Address not in device registers");

	return -1;
}

void
dev430x1xxwriteword(Engine *E, State *S, ushort addr, ushort data)
{
	/*									*/
	/*	If this is a word write to 8-bit peripheral module region,	*/
	/*	only low byte is written into the appropriate 8-bit register.	*/
	/*	high byte is ignored. All word accesses must be to even 	*/
	/*	addresses. Architecture does not define behavior when word 	*/
	/*	access happens to odd address (non-aligned word access).	*/
	/*									*/

	if ((addr >= MSP430_PERIPH8_BEGIN) && (addr <= MSP430_PERIPH8_END))
	{
		S->msp430->periph8[addr - MSP430_PERIPH8_BEGIN] = data & 0xFF;
	}
	else if ((addr >= MSP430_PERIPH16_BEGIN) && (addr <= MSP430_PERIPH16_END))
	{
		int	NMIIE = msp430_SFR_IE1_get_NMIIE(S->msp430->sfr[MSP430_SFR_IE1]);

		if (addr & B0001)
		{
			sfatal(E, S, "Word write to odd address (Architecture undefined). ");
			return;
		}


		/*								*/
		/*	See 2.2.1, last Note on page: If we are in NMI mode	*/
		/*	and NMIES is being changed, an NMI might be generated	*/
		/*								*/
		if (	NMIIE && (addr == MSP430_PERIPH16_WDTCTL) &&
			(msp430_WDTCTL_get_NMIES(data)) )
		{
			dev430x1xxNMIinterrupt(S, MSP430_INTR_NMI);

			return;
		}


		/*	See 10.3 of slau049e.pdf	*/
		if (addr == MSP430_PERIPH16_WDTCTL && ((data >> 8) != 0x5A))
		{
			dev430x1xxreset(S, MSP430_RESET_PUC);
		}

		S->msp430->periph16[(addr - MSP430_PERIPH16_BEGIN) >> 1] = data;
	}
	else if ((addr >= MSP430_SFR_BEGIN) && (addr <= MSP430_SFR_END))
	{
		S->msp430->sfr[addr - MSP430_SFR_BEGIN] = data & 0xFF;
	}
	else
	{
		sfatal(E, S, "Address not in device registers");
	}

	return;
}

void
dev430x1xxsetpin(State *S, int pin, double voltage)
{
	// check whether to raise an interrupt for this pin based on IO config
	//sdfsdf


	// If this is RST/NMI pin, check the configured edge (in WDTNMIES) 
	// raise approp rst/nmi (call dev430x1xxNMIinterrupt(), check for NMIIE is done there)
	//sdfsdfsd

	
	S->msp430->pins[pin] = voltage;


	return;
}

double
dev430x1xxgetpin(State *S, int pin)
{
	return S->msp430->pins[pin];
}

void
dev430x1xxPORreset(State *S)
{
	/*								*/
	/*	Power On Reset (POR). Occurs if pin /RST, NMI 		*/
	/*	is configured in reset mode.				*/
	/*								*/

	
	/*		Reset all peripherals 		*/
	dev430x1xxreset(S, MSP430_RESET_POR);


	/*	Configure /RST, NMI in reset mode	*/
	/*	implicit in periph reset	*/


	/*	Configure all IO pins as input		*/
	/*	implicit in periph reset	*/
	
	
	/*			Power up WDT		*/
	/*	implicit in periph reset	*/
	

	/*			Reset SR		*/
	S->msp430->R[MSP430_SR] = 0x0000;


	/*		Set PC = 0xFFFE			*/
	S->msp430->R[MSP430_PC] = 0xFFFE;


	return;
}

void
dev430x1xxPUCreset(State *S)
{
	/*		Reset all peripherals 		*/
	dev430x1xxreset(S, MSP430_RESET_PUC);

	return;
}

void
dev430x1xxNMIinterrupt(State *S, int type)
{
	int NMIIE = msp430_SFR_IE1_get_NMIIE(S->msp430->sfr[MSP430_SFR_IE1]);
	int ACCVIE = msp430_SFR_IE1_get_ACCVIE(S->msp430->sfr[MSP430_SFR_IE1]);
	int OFIE = msp430_SFR_IE1_get_OFIE(S->msp430->sfr[MSP430_SFR_IE1]);

	/*	See 2.2.1, page 2.9	*/
	//BUG: these should be moved to the caller
	if (type == MSP430_INTR_NMI)
	{
		/*	Set NMIIFG	*/
		S->msp430->sfr[MSP430_SFR_IFG1] |= (1 << 4);
	}
	else if (type == MSP430_INTR_OSCF)
	{
		/*	Set OSFIFG	*/
		S->msp430->sfr[MSP430_SFR_IFG1] = (1 << 1);
	}
	else if (type == MSP430_INTR_ACCV)
	{
		/*						*/
		/*	Note: ACCVIFG is in FCTL3 register, 	*/
		/*	not IFG1 of SFR like the other two.	*/
		/*						*/	
		/*	Set ACCVIFG	*/
		sp16(MSP430_PERIPH16_FCTL3, (1 << 2));
	}

	/*						*/
	/*	NMI is a multi-source intr that is	*/
	/*	disambiguated by SW checking the IFG	*/
	/*	bits.					*/
	/*	Any NMI resets all NMI enable bits	*/
	/*						*/
	if (	(type == MSP430_INTR_NMI && NMIIE) ||
		(type == MSP430_INTR_ACCV && ACCVIE) ||
		(type == MSP430_INTR_OSCF && OFIE)
		)
	{
		/*	Clear NMIIE, ACCVIE and OFIE, but not WDTIE	*/
		S->msp430->sfr[MSP430_SFR_IE1] = 0x01;

		S->msp430->R[MSP430_PC] = 0xFFFC;
	}


	return;
}

void
dev430x1xxinterrupt(State *S, int priority)
{
	int	GIE = msp430_sreg_get_GIE(S->msp430->R[MSP430_SR]);
	if (!GIE)
	{
		return;
	}


	/*	See section 2.2.3	*/


	/*									*/
	/*	Check the 'Rack' and complete the currently executing instr.	*/
	/*									*/
	//BUG: enable pipelining in the future


	/*									*/
	/*	Push the PC, which is the next instr after completed one,	*/
	/*	onto the stack.	See Figure 2-7.					*/
	/*									*/
	S->msp430->R[MSP430_SP] += 2;
	S->MEM[S->msp430->R[MSP430_SP]] = S->msp430->R[MSP430_PC];


	/*	Push SR on stack.	*/
	S->msp430->R[MSP430_SP] += 2;
	S->MEM[S->msp430->R[MSP430_SP]] = S->msp430->R[MSP430_SR];


	/*									*/
	/*	Look at interrupt list. Find the one with highest priority.	*/
	/*									*/
	//???


	/*									*/
	/*	If single source, clear flags, else leave them set and let 	*/
	/*	SW reset them.							*/
	/*									*/
	//???


	/*									*/
	/*	Clear SR except for SCG0. Since GIE is cleared, intrs are	*/
	/*	disabled.							*/
	/*									*/
	S->msp430->R[MSP430_SR] &= 1 << 6;


	/*									*/
	/*	Load interrupt vector into PC. Vector depends on priority.	*/
	/*	(See Table 2-1 on page 2.13 of slau049e.pdf)			*/
	/*									*/
	S->msp430->R[MSP430_PC] = 0xFFE0 + (priority << 1);


	return;
}








/*	Appropriate byte/word Access functions for each peripheral */

void
dev430x1xxflashaccessviolation(State *S)
{
/*
	flash access error

	set the ACCVIFG flag

	if ACCVIE, generate an NMI (complete current instr, plus 6 cycle latency)
*/
}

void
dev430x1xxflashreadbyte(State *S)
{
}


void
dev430x1xxflashwritebyte(State *S)
{
}

void
dev430x1xxoscfault(State *S)
{
/*
	oscillator fault

	set the OFIFG flag

	if OFIE, generate an NMI (complete current instr, plus 6 cycle latency)
*/
}
