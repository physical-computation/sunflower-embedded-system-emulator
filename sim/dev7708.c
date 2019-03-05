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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"
#include "mextern.h"
#include "mmu-hitachi-sh.h"

/*
	TODO: see section 23 of rej manual for valid access sizes of all memory mapped registers
*/

ulong
dev7708readlong(Engine *E, State *S, ulong addr)
{
	ulong	data = 0;


	/*									*/
	/*	Long reads are not actually to peripherals, but to various	*/
	/*	memmory mapped system control registers. Bitflip analysis	*/
	/*	therefore accounts for them on the regular phy addr bus.	*/
	/*									*/
	if ((addr >= TLB_DATAARRAY_START) && (addr <= TLB_DATAARRAY_END))
	{
		if (S->superH->SR.MD == 1)
		{
			//fprintf(stderr, "tlb data array read\n");
			data =  superHtlb_dataarray_read(S, addr);
		}
		else
		{
			/*							*/
			/*	BUG:						*/
			/*	Non-priv access should really be generating	*/
			/*	some form of exception.				*/
			/*							*/
			sfatal(E, S, "Illegal read from TLB data array in user mode");
		}
	}
	else if ((addr >= TLB_ADDRARRAY_START) && (addr <= TLB_ADDRARRAY_END))
	{
		//fprintf(stderr, "tlb addr array read\n");

		if (S->superH->SR.MD == 1)
		{
			data = superHtlb_addrarray_read(S, addr);
		}
		else
		{
			/*							*/
			/*	BUG:						*/
			/*	Non-priv access should really be generating	*/
			/*	some form of exception.				*/
			/*							*/
			sfatal(E, S, "Illegal read from TLB address array in user mode");
		}
	}

	else switch (addr)
	{
		case EXCP_INTEVT:
		{
			data = S->superH->INTEVT;
			break;
		}

		case EXCP_EXPEVT:
		{
			data = S->superH->EXPEVT;
			break;
		}

		case EXCP_TRA:
		{
			data = S->superH->TRA;
			break;
		}

		case MMU_PTEH:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal read from PTEH in user mode");
			}

			data = S->superH->PTEH;
			break;
		}

		case MMU_PTEL:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal read from PTEL in user mode");
			}

			data = S->superH->PTEL;
			break;
		}

		case MMU_TTB:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal read from TTB in user mode");
			}

			data = S->superH->TTB;
			break;
		}

		case MMU_TEA:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal read from TEA in user mode");
			}

			data = S->superH->TEA;
			break;
		}

		case MMU_MMUCR:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal read from MMUCR in user mode");
			}
			//fprintf(stderr, "reading MMUCR\n");

			data = S->superH->MMUCR;
			break;
		}

		case CACHE_CCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from CACHE_CCR...\n");
			break;
		}

		case TIMER_TCOR0:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCOR0...\n");
			break;
		}
		case TIMER_TCNT0:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCNT0...\n");
			break;
		}
		case TIMER_TCOR1:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCOR1...\n");
			break;
		}
		case TIMER_TCNT1:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCNT1...\n");
			break;
		}
		case TIMER_TCOR2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCOR2...\n");
			break;
		}
		case TIMER_TCNT2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCNT2...\n");
			break;
		}
		case TIMER_TCPR2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCPR2...\n");
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Double Word access (read) at address 0x" UHLONGFMT "\n", addr);
			sfatal(E, S, "Invalid double word access.");
		}
	}

	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Peripheral Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->perdata_bus, data);
		S->superH->B->perdata_bus = data;

		/*	Peripheral Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->peraddr_bus, addr);
		S->superH->B->peraddr_bus = addr;
	}

	return data;
}

ushort
dev7708readword(Engine *E, State *S, ulong addr)
{
	ushort	data = 0;

	switch(addr)
	{
		case TIMER_TCR0:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCR0...\n");
			break;
		}
		case TIMER_TCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCR1...\n");
			break;
		}
		case TIMER_TCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TCR2...\n");
			break;
		}

		case ICLR_ICR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from ICLR_ICR...\n");
			break;
		}
		case ICLR_IPRA:
		{
			mprint(E, S, nodeinfo, "Ignoring read from ICLR_IPRA...\n");
			break;
		}
		case ICLR_IPRB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from ICLR_IPRB...\n");
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Word access (read) at address 0x" UHLONGFMT "\n", addr);
			sfatal(E, S, "Address not in main mem, and not in I/O space either !");
		}
	}

	return data;
}

uchar
dev7708readbyte(Engine *E, State *S, ulong addr)
{
	uchar	data = 0;


	if ((addr >= CACHE_SRAM_START) && (addr < CACHE_SRAM_END))
	{
		mprint(E, S, nodeinfo, "Ignoring read from CACHE_SRAM...\n");
	}

	else switch (addr)
	{

		case UBC_BARA:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BARA...\n");
			break;
		}

		case UBC_BASRA:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BASRA...\n");
			break;
		}

		case UBC_BAMRA:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BAMRA...\n");
			break;
		}

		case UBC_BBRA:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BBRA...\n");
			break;
		}

		case UBC_BARB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BARB...\n");
			break;
		}

		case UBC_BAMRB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BAMRB...\n");
			break;
		}

		case UBC_BASRB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BASRB...\n");
			break;
		}

		case UBC_BBRB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BBRB...\n");
			break;
		}

		case UBC_BDRB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BDRB...\n");
			break;
		}

		case UBC_BDMRB:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BDMRB...\n");
			break;
		}

		case UBC_BRCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from UBC_BRCR...\n");
			break;
		}

		case POWER_STBCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from POWER_STBCR...\n");
			break;
		}

		case OCOSC_FRQCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from OCOSC_FRQCR...\n");
			break;
		}

		case WDOG_WTCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from WDOG_WTCNT...\n");
			break;
		}

		case WDOG_WTCSR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from WDOG_WTCSR...\n");
			break;
		}

		case BSC_BCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_BCR1...\n");
			break;
		}

		case BSC_BCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_BCR2...\n");
			break;
		}

		case BSC_WCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_WCR1...\n");
			break;
		}

		case BSC_WCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_WCR2...\n");
			break;
		}

		case BSC_MCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_MCR...\n");
			break;
		}

		case BSC_DCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_DCR...\n");
			break;
		}

		case BSC_PCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_PCR...\n");
			break;
		}

		case BSC_RTCSR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_RTCSR...\n");
			break;
		}

		case BSC_RTCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_RTCNT...\n");
			break;
		}

		case BSC_RTCCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_RTCCR...\n");
			break;
		}

		case BSC_RFCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_RFCR...\n");
			break;
		}

		case BSC_SDMR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from BSC_SDMR...\n");
			break;
		}

		case TIMER_TOCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TOCR...\n");
			break;
		}

		case TIMER_TSTR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from TIMER_TSTR...\n");
			break;
		}




		case RTCLK_RSECCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_R64CNT...\n");
			break;
		}

		case RTCLK_R64CNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_R64CNT...\n");
			break;
		}

		case RTCLK_RMINCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RMINCNT...\n");
			break;
		}

		case RTCLK_RHRCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RHRCNT...\n");
			break;
		}

		case RTCLK_RWKCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RWKCNT...\n");
			break;
		}

		case RTCLK_RDAYCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RDAYCNT...\n");
			break;
		}

		case RTCLK_RMONCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RMONCNT...\n");
			break;
		}

		case RTCLK_RYRCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RYRCNT...\n");
			break;
		}

		case RTCLK_RSECAR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RSECAR...\n");
			break;
		}

		case RTCLK_RMINAR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RMINAR...\n");
			break;
		}

		case RTCLK_RHRAR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RHRAR...\n");
			break;
		}

		case RTCLK_RWKAR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RWKAR...\n");
			break;
		}

		case RTCLK_RDAYAR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RDAYAR...\n");
			break;
		}

		case RTCLK_RMONAR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RMONAR...\n");
			break;
		}

		case RTCLK_RCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RCR1...\n");
			break;
		}

		case RTCLK_RCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring read from RTCLK_RCR2...\n");
			break;
		}

		case SCC_SCSMR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SCC_SCSMR...\n");
			break;
		}

		case SCC_SCBRR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SCC_SCBRR...\n");
			break;
		}

		case SCC_SCSCR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SCC_SCSCR...\n");
			break;
		}

		case SCC_SCTDR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SCC_SCTDR...\n");
			break;
		}

		case SCC_SCSSR:
		{
			/*	Hack, just to accept char for TX	*/
			//mprint(E, S, nodeinfo, "Returning 0xFF for read from SCC_SCSSR...\n");
			//to always make write succeed, hack is return 0xFF;
			return 0xFF;
		}

		case SCC_SCRDR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SCC_SCRDR...\n");
			break;
		}

		case SCC_SCSPTR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SCC_SCSPTR...\n");
			break;
		}

		case SMARTCARD_SCSCMR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from SMARTCARD_SCSMR...\n");
			break;
		}

		case GPIO_PCTR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from GPIO_PCTR...\n");
			break;
		}

		case GPIO_PDTR:
		{
			mprint(E, S, nodeinfo, "Ignoring read from GPIO_PDTR...\n");
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Byte access (read) at address 0x" UHLONGFMT "\n", addr);
			sfatal(E, S, "Invalid byte access.");
		}
	}

	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Peripheral Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->perdata_bus, data);
		S->superH->B->perdata_bus = data;

		/*	Peripheral Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->peraddr_bus, addr);
		S->superH->B->peraddr_bus = addr;
	}

	return data;
}


void
dev7708writelong(Engine *E, State *S, ulong addr, ulong data)
{
	/*									*/
	/*	Long reads are not actually to peripherals, but to various	*/
	/*	memmory mapped system control registers. Bitflip analysis	*/
	/*	therefore accounts for them on the regular phy addr bus.	*/
	/*									*/
	if ((addr >= TLB_DATAARRAY_START) && (addr <= TLB_DATAARRAY_END))
	{
		//fprintf(stderr, "tlb data array write\n");

		if (S->superH->SR.MD == 1)
		{
			superHtlb_dataarray_write(S, addr, data);
		}
		else
		{
			/*							*/
			/*	BUG:						*/
			/*	Non-priv access should really be generating	*/
			/*	some form of exception.				*/
			/*							*/
			sfatal(E, S, "Illegal write to TLB data array in user mode");
		}
	}
	else if ((addr >= TLB_ADDRARRAY_START) && (addr <= TLB_ADDRARRAY_END))
	{
		//fprintf(stderr, "tlb addr array write\n");

		if (S->superH->SR.MD == 1)
		{
			superHtlb_addrarray_write(S, addr, data);
		}
		else
		{
			/*							*/
			/*	BUG:						*/
			/*	Non-priv access should really be generating	*/
			/*	some form of exception.				*/
			/*							*/
			sfatal(E, S, "Illegal write to TLB address array in user mode");
		}
	}
	else switch (addr)
	{
		case MMU_PTEH:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal write to MMU PTEH in user mode");
			}

			S->superH->PTEH = data;
			break;
		}

		case MMU_PTEL:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal write to MMU PTEL in user mode");
			}

			S->superH->PTEL = data;
			break;
		}

		case MMU_TTB:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal write to MMU TTB in user mode");
			}

			S->superH->TTB = data;
			break;
		}

		case MMU_TEA:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal write to MMU TEA in user mode");
			}

			S->superH->TEA = data;
			break;
		}

		case MMU_MMUCR:
		{
			if (S->superH->SR.MD != 1)
			{
				/*							*/
				/*	BUG:						*/
				/*	Non-priv access should really be generating	*/
				/*	some form of exception.				*/
				/*							*/
				sfatal(E, S, "Illegal write to MMU MMUCR in user mode");
			}


			if (mmucr_field_tf(data))
			{
				superHtlbflush(E, S);
			}

			/*	clear the TF bit	*/
			data &= ~(1<<2);
			S->superH->MMUCR = data;

			break;
		}

		case CACHE_CCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to CACHE_CCR...\n");
			break;
		}

		case TIMER_TCOR0:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCNT0:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCOR1:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCNT1:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCOR2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCNT2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCPR2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo,
				"Long word access (write) at address 0x" UHLONGFMT "\n", addr);
			sfatal(E, S, "Invalid long word access.");
		}
	}

	return;
}

void
dev7708writeword(Engine *E, State *S, ulong addr, ushort data)
{
	switch(addr)
	{
		case TIMER_TCR0:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}
		case TIMER_TCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}

		case ICLR_ICR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to ICLR...\n");
			break;
		}
		case ICLR_IPRA:
		{
			mprint(E, S, nodeinfo, "Ignoring write to ICLR...\n");
			break;
		}
		case ICLR_IPRB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to ICLR...\n");
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Word access (write) at address 0x" UHLONGFMT "\n", addr);
			sfatal(E, S, "Invalid word access.");
		}
	}

	return;
}

void
dev7708writebyte(Engine *E, State *S, ulong addr, uchar data)
{
	if ((addr >= CACHE_SRAM_START) && (addr < CACHE_SRAM_END))
	{
		mprint(E, S, nodeinfo, "Ignoring write to CACHE_SRAM...\n");
	}
	else switch (addr)
	{
		case EXCP_TRA:
		{
			sfatal(E, S, "Write to EXCP_TRA not permitted");
			break;
		}

		case EXCP_EXPEVT:
		{
			sfatal(E, S, "Write to EXCP_EXPEVT not permitted");
			break;
		}

		case EXCP_INTEVT:
		{
			sfatal(E, S, "Write to EXCP_INTEVT not permitted");
			break;
		}


		case UBC_BARA:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BASRA:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BAMRA:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BBRA:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BARB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BAMRB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BASRB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BBRB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BDRB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BDMRB:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case UBC_BRCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to UBC...\n");
			break;
		}

		case POWER_STBCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to POWER_STBCR...\n");
			break;
		}

		case OCOSC_FRQCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to OCOSC_FRQCR...\n");
			break;
		}

		case WDOG_WTCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to WDOG...\n");
			break;
		}

		case WDOG_WTCSR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to WDOG...\n");
			break;
		}

		case BSC_BCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_BCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_WCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_WCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_MCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_DCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_PCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_RTCSR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_RTCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_RTCCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_RFCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case BSC_SDMR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to BSC...\n");
			break;
		}

		case TIMER_TOCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER...\n");
			break;
		}

		case TIMER_TSTR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to TIMER_TSTR...\n");
			break;
		}




		case RTCLK_R64CNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RSECCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RMINCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RHRCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RWKCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RDAYCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RMONCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RYRCNT:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RSECAR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RMINAR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RHRAR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RWKAR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RDAYAR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RMONAR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RCR1:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case RTCLK_RCR2:
		{
			mprint(E, S, nodeinfo, "Ignoring write to RTCLK...\n");
			break;
		}

		case SCC_SCSMR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to SCC_SCSMR...\n");
			break;
		}

		case SCC_SCBRR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to SCC_SCBRR...\n");
			break;
		}

		case SCC_SCSCR:
		{
			mprint(E, S, nodeinfo, "Ignoring write to SCC_SCSCR...\n");
			break;
		}

		case SCC_SCTDR:
		{
			mprint(E, S, nodestdout, "%c", (char)data);

			break;
		}

		case SCC_SCSSR:
		{
			break;
		}

		case SCC_SCRDR:
		{
			break;
		}

		case SCC_SCSPTR:
		{
			break;
		}

		case SMARTCARD_SCSCMR:
		{
			break;
		}

		case GPIO_PCTR:
		{
			break;
		}

		case GPIO_PDTR:
		{
			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, 
				"Byte access (write) at address 0x" UHLONGFMT "\n", addr);
			sfatal(E, S, "Invalid byte access.");
		}
	}

	return;
}
