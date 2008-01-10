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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"
#include "mextern.h"

ulong
devportreadlong(State *S, ulong addr)
{
	if (addr == SUPERH_THREADID)
	{
		return S->NODE_ID;
	}
	else if ((addr >= SUPERH_NUMAREGION_RDCNT_BEGIN) &&
		(addr < SUPERH_NUMAREGION_RDCNT_END))
	{
		/*							*/
		/*	CNT register addresses are longword-aligned,	*/
		/*	so lower 2 bits of address are irrelevant, 	*/
		/*	(should always be zero).			*/
		/*							*/
		int	which = ((addr - SUPERH_NUMAREGION_RDCNT_BEGIN) >> 2) & 0x3FFF;

		if (which >= S->N->count)
		{
			mprint(S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_RDCNT register)\n",
				addr);
			mprint(S, nodeinfo, "which = %d\n", which);

			sfatal(S, "Attempt to access NUMAREGION_RDCNT register "
				"outside number of NUMAREGIONs");
		}

		return S->N->regions[which]->nreads;
	}
	else if ((addr >= SUPERH_NUMAREGION_WRCNT_BEGIN) &&
		(addr < SUPERH_NUMAREGION_WRCNT_END))
	{
		/*							*/
		/*	CNT register addresses are longword-aligned,	*/
		/*	so lower 2 bits of address are irrelevant, 	*/
		/*	(should always be zero).			*/
		/*							*/
		int	which = ((addr - SUPERH_NUMAREGION_WRCNT_BEGIN) >> 2) & 0x3FFF;

		if (which >= S->N->count)
		{
			mprint(S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_WRCNT register)\n",
				addr);
			mprint(S, nodeinfo, "which = %d\n", which);

			sfatal(S, "Attempt to access NUMAREGION_WRCNT register "
				"outside number of NUMAREGIONs");
		}

		return S->N->regions[which]->nwrites;
	}
	else if (addr == SUPERH_NUMAREGION_COUNT)
	{
		return S->N->count;
	}

	return S->devreadlong(S, addr);
}

ushort
devportreadword(State *S, ulong addr)
{
	mprint(S, nodeinfo, "Word access (read) at address 0x" UHLONGFMT "\n", addr);
	sfatal(S, "Address not in main mem, and not in I/O space either !");

	return 0;
}

uchar
devportreadbyte(State *S, ulong addr)
{
	uchar	data = 0;


	if ((addr >= SUPERH_USECS_BEGIN) && (addr < SUPERH_USECS_END))
	{
		int	offset;

		offset = addr - SUPERH_USECS_BEGIN;
		data = (uchar)((long)(S->TIME*1E6) >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_DST) && (addr < (SUPERH_NIC_DST+SUPERH_NIC_REG_SPACING)))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_DST register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = S->superH->NIC_IFCS[whichifc].IFC_DST[offset];
	}
	else if ((addr >= SUPERH_NIC_OUI) && (addr < (SUPERH_NIC_OUI+SUPERH_NIC_REG_SPACING)))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_OUI register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = S->superH->NIC_IFCS[whichifc].IFC_OUI[offset];
	}
	else if ((addr >= SUPERH_NIC_NCSENSE) && (addr < SUPERH_NIC_NCSENSE+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCSENSE register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)(S->superH->NIC_IFCS[whichifc].IFC_CNTR_CSENSE_ERR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_NCOLLS) && (addr < SUPERH_NIC_NCOLLS+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCOLLS register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)(S->superH->NIC_IFCS[whichifc].IFC_CNTR_COLLS_ERR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_FSZ) && (addr < SUPERH_NIC_FSZ+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_FSZ register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].frame_bits/8 >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_BRR) && (addr < SUPERH_NIC_BRR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_BRR register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		/*	NIC_BRR contains network speed in Kb/s		*/
		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].IFC_BRR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_NMR) && (addr < SUPERH_NIC_NMR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "You should not be reading NIC_NMR !?");
	}
	else if ((addr >= SUPERH_NIC_NCR) && (addr < SUPERH_NIC_NCR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCR register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].IFC_NCR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_TDR) && (addr < SUPERH_NIC_TDR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "You should not be reading NIC_TDR !?");
	}
	else if ((addr >= SUPERH_NIC_NSR) && (addr < SUPERH_NIC_NSR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NSR register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].IFC_NSR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_RDR) && (addr < SUPERH_NIC_RDR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_RDR register)\n",
				addr);
			mprint(S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(S, "Attempt to access IFC register outside number of IFCs");
		}

		data = nic_rx_dequeue(S, whichifc);
	}
	else if ((addr >= SUPERH_RAND_BEGIN) && (addr < SUPERH_RAND_END))
	{
		/*							*/
		/*	We actually craft the 32-bit rand out of 4 	*/
		/*	successive rand bytes.				*/
		/*							*/
		ulong	rnd;
		int	offset;


		offset = addr - SUPERH_RAND_BEGIN;
		rnd = (ulong)mrandom();
			
		data = (uchar)(rnd >> (offset*8))&0xFF;
	}
	else if (addr == SUPERH_SIMCMD_DATA)
	{
		sfatal(S, "You should not be reading SIMCMD_DATA !?");
	}
	else if (addr == SUPERH_SIMCMD_CTL)
	{
		sfatal(S, "You should not be reading SIMCMD_CTL !?");
	}
	else if ((addr >= SUPERH_ORBIT_BEGIN) && (addr < SUPERH_ORBIT_END))
	{
		int	offset;

		offset = addr - SUPERH_ORBIT_BEGIN;
		data = (uchar)(S->orbit >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_VELOCITY_BEGIN) && (addr < SUPERH_VELOCITY_END))
	{
		int	offset;
		uvlong	tmp;

		memmove(&tmp, &S->velocity, sizeof(uvlong));
		offset = addr - SUPERH_VELOCITY_BEGIN;
		data = (uchar)(tmp >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_XLOC_BEGIN) && (addr < SUPERH_XLOC_END))
	{
		int	offset;
		uvlong	tmp;

		memmove(&tmp, &S->xloc, sizeof(uvlong));
		offset = addr - SUPERH_XLOC_BEGIN;
		data = (uchar)(tmp >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_YLOC_BEGIN) && (addr < SUPERH_YLOC_END))
	{
		int	offset;
		uvlong	tmp;

		memmove(&tmp, &S->yloc, sizeof(uvlong));
		offset = addr - SUPERH_YLOC_BEGIN;
		data = (uchar)(tmp >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_ZLOC_BEGIN) && (addr < SUPERH_ZLOC_END))
	{
		int	offset;
		uvlong	tmp;

		memmove(&tmp, &S->zloc, sizeof(uvlong));
		offset = addr - SUPERH_ZLOC_BEGIN;
		data = (uchar)(tmp >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_SENSREAD_BEGIN) && (addr < SUPERH_SENSREAD_END))
	{
		int	which, offset;
		uvlong	tmp;

		//TODO: we should be doing it this way for NIC regs too, why aren't we ??
		which = ((addr - SUPERH_SENSREAD_BEGIN) >> 4) & 0xFFF;
		if (which >= MAX_NODE_SENSORS)
		{
			sfatal(S, "Attempt to access sensor register outside number of sensors");
			return 0;
		}

		memmove(&tmp, &S->sensors[which].reading, sizeof(uvlong));
		offset = (addr - SUPERH_SENSREAD_BEGIN) & 0xF;
		data = (uchar)(tmp >> (offset*8))&0xFF;
	}
	else if (addr == SUPERH_DEVLOGPRINT)
	{
		sfatal(S, "You should not be reading DEVLOGPRINT !?");
	}
	else
	{
		return S->devreadbyte(S, addr);
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
devportwritelong(State *S, ulong addr, ulong data)
{
	
	if ((addr >= SUPERH_NUMAREGION_RDCNT_BEGIN) &&
		(addr < SUPERH_NUMAREGION_RDCNT_END))
	{
		/*							*/
		/*	CNT register addresses are longword-aligned,	*/
		/*	so lower 2 bits of address are irrelevant, 	*/
		/*	(should always be zero).			*/
		/*							*/
		int	which = ((addr - SUPERH_NUMAREGION_RDCNT_BEGIN) >> 2) & 0x3FFF;

		if (which >= S->N->count)
		{
			mprint(S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_RDCNT register)\n",
				addr);
			mprint(S, nodeinfo, "which = %d\n", which);

			sfatal(S, "Attempt to access NUMAREGION_RDCNT register "
				"outside number of NUMAREGIONs");
		}

		S->N->regions[which]->nreads = data;

		return;
	}
	
	if ((addr >= SUPERH_NUMAREGION_WRCNT_BEGIN) &&
		(addr < SUPERH_NUMAREGION_WRCNT_END))
	{
		/*							*/
		/*	CNT register addresses are longword-aligned,	*/
		/*	so lower 2 bits of address are irrelevant, 	*/
		/*	(should always be zero).			*/
		/*							*/
		int	which = ((addr - SUPERH_NUMAREGION_WRCNT_BEGIN) >> 2) & 0x3FFF;

		if (which >= S->N->count)
		{
			mprint(S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_WRCNT register)\n",
				addr);
			mprint(S, nodeinfo, "which = %d\n", which);

			sfatal(S, "Attempt to access NUMAREGION_WRCNT register "
				"outside number of NUMAREGIONs");
		}

		S->N->regions[which]->nwrites = data;

		return;
	}

	/*									*/
	/*	Long writes are not actually to peripherals, but to various	*/
	/*	memmory mapped system control registers. Bitflip analysis	*/
	/*	therefore accounts for them on the regular phy addr bus.	*/
	/*									*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*		Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, addr);
		S->superH->B->paddr_bus = addr;
	}

	S->devwritelong(S, addr, data);

	return;
}

void
devportwriteword(State *S, ulong addr, ushort data)
{
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Peripheral Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->perdata_bus, data);
		S->superH->B->perdata_bus = data;

		/*	Peripheral Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->peraddr_bus, addr);
		S->superH->B->peraddr_bus = addr;
	}
	S->devwriteword(S, addr, data);

	return;
}

void
devportwritebyte(State *S, ulong addr, uchar data)
{
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Peripheral Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->perdata_bus, data);
		S->superH->B->perdata_bus = data;

		/*	Peripheral Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->peraddr_bus, addr);
		S->superH->B->peraddr_bus = addr;
	}

	if ((addr >= SUPERH_NIC_OUI) && (addr < (SUPERH_NIC_OUI+SUPERH_NIC_REG_SPACING)))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			sfatal(S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

		S->superH->NIC_IFCS[whichifc].IFC_OUI[offset] = data;
	}
	else if ((addr >= SUPERH_NIC_DST) && (addr < (SUPERH_NIC_DST+SUPERH_NIC_REG_SPACING)))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			sfatal(S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

		S->superH->NIC_IFCS[whichifc].IFC_DST[offset] = data;
	}
	else if ((addr >= SUPERH_NIC_NMR) && (addr < SUPERH_NIC_NMR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "Use of NIC_NMR deprecated. Update your application!!");
	}
	else if ((addr >= SUPERH_NIC_BRR) && (addr < SUPERH_NIC_BRR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "Write to NIC_BRR ignored. What are you trying to do ?!");
	}
	else if ((addr >= SUPERH_NIC_NCR) && (addr < SUPERH_NIC_NCR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			sfatal(S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

		if (!(data & (1 << 0)))
		{
			mprint(S, nodeinfo,
				"Dequeing and clearing all interrupts on IFC [%d]\n",
				whichifc);
			sfatal(S, "Are you sure thats what you wanted to do ?");
/*
			pic_intr_clear(S, NIC_RXOK_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_TXOK_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_ADDRERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_FRAMEERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_COLLSERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_CSENSEERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_RXOVRRUNERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_RXUNDRRUNERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_TXOVRRUNERR_INTR, whichifc, 1);
			pic_intr_clear(S, NIC_TXUNDRRUNERR_INTR, whichifc, 1);
*/
		}

		S->superH->NIC_IFCS[whichifc].IFC_NCR = data & 0xff;
	}
	else if ((addr >= SUPERH_NIC_TDR) && (addr < SUPERH_NIC_TDR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			sfatal(S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

		nic_tx_enqueue(S, data, whichifc);
	}
	else if ((addr >= SUPERH_NIC_NSR) && (addr < SUPERH_NIC_NSR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "You should not write into NSR");
	}
	else if ((addr >= SUPERH_NIC_RDR) && (addr < SUPERH_NIC_RDR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "You should not write into RDR");
	}
	else if ((addr >= SUPERH_NIC_FSZ) && (addr < SUPERH_NIC_FSZ+SUPERH_NIC_REG_SPACING))
	{
		sfatal(S, "You should not write into NIC_FSZ");
	}
	else if ((addr >= SUPERH_RAND_BEGIN) && (addr < SUPERH_RAND_END))
	{
		sfatal(S, "You should not write into RAND");
	}
	else if (addr == SUPERH_SIMCMD_DATA)
	{
		/*							*/
		/*	Commands that are too long are truncated	*/
		/*	and immediately executed			*/
		/*							*/
		S->cmdbuf[S->cmdbuf_nbytes++] = data;
		if (S->cmdbuf_nbytes == MAX_CMD_LEN - 2)
		{
			State		*tmpstate = CUR_STATE;
			extern	int	yyparse(void);

			S->cmdbuf[S->cmdbuf_nbytes++] = '\n';
			S->cmdbuf[S->cmdbuf_nbytes++] = '\0';
			munchinput(S->cmdbuf);
			S->cmdbuf_nbytes = 0;

			/*							*/
			/*	Everything in sunflower.y is relative to the	*/
			/*	current value of CUR_STATE, so we have to 	*/
			/*	do a little dance, for the people, and their 	*/
			/*	cats, i guess, but then again, who cares ? 	*/
			/*							*/
			CUR_STATE = S;
			yyparse();
			CUR_STATE = tmpstate;
		}
	}
	else if (addr == SUPERH_SIMCMD_CTL)
	{
		State		*tmpstate = CUR_STATE;
		extern	int	yyparse(void);


		/*								*/
		/*	Safe because nbytes only incr'd when SIMCMD_DATA is	*/
		/*	written to, and we do necessary checks there.		*/
		/*								*/
		S->cmdbuf[S->cmdbuf_nbytes++] = '\n';
		S->cmdbuf[S->cmdbuf_nbytes++] = '\0';
		munchinput(S->cmdbuf);
		S->cmdbuf_nbytes = 0;

		/*							*/
		/*	Everything in sunflower.y is relative to the	*/
		/*	current value of CUR_STATE, so we have to 	*/
		/*	do a little dance, for the people, and their 	*/
		/*	cats, i guess, but then again, who cares ? 	*/
		/*							*/
		CUR_STATE = S;
		yyparse();
		CUR_STATE = tmpstate;
	}
	else if ((addr >= SUPERH_ORBIT_BEGIN) && (addr < SUPERH_ORBIT_END))
	{
		sfatal(S, "You should not write into ORBIT data register");
	}
	else if ((addr >= SUPERH_VELOCITY_BEGIN) && (addr < SUPERH_VELOCITY_END))
	{
		sfatal(S, "You should not write into VELOCITY data register");
	}
	else if ((addr >= SUPERH_XLOC_BEGIN) && (addr < SUPERH_XLOC_END))
	{
		sfatal(S, "You should not write into XLOC data register");
	}
	else if ((addr >= SUPERH_YLOC_BEGIN) && (addr < SUPERH_YLOC_END))
	{
		sfatal(S, "You should not write into YLOC data register");
	}
	else if ((addr >= SUPERH_ZLOC_BEGIN) && (addr < SUPERH_ZLOC_END))
	{
		sfatal(S, "You should not write into ZLOC data register");
	}
	else if ((addr >= SUPERH_SENSWRITE_BEGIN) && (addr < SUPERH_SENSWRITE_END))
	{
/*
		int	which, offset;
		uvlong	tmp;

		which = ((addr - SUPERH_SENSWRITE_BEGIN) >> 4) & 0xFFF;
		if (which >= MAX_NODE_SENSORS)
		{
			sfatal(S, "Attempt to access sensor register outside number of sensors");
		}

		//	TODO:
		//((uchar *)&S->actuators[which].reading+offset) = data;

		return;
*/
	}
	else if (addr == SUPERH_DEVLOGPRINT)
	{
		if (SF_SIMLOG)
		{
			mlog(S, "%c", data);
		}
	}
	else
	{
		S->devwritebyte(S, addr, data);
	}

	return;
}
