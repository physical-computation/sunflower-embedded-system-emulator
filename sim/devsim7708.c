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
#include <math.h>
#include "sf.h"
#include "mextern.h"

ulong
devportreadlong(Engine *E, State *S, ulong addr)
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
			mprint(E, S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_RDCNT register)\n",
				addr);
			mprint(E, S, nodeinfo, "which = %d\n", which);

			sfatal(E, S, "Attempt to access NUMAREGION_RDCNT register "
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
			mprint(E, S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_WRCNT register)\n",
				addr);
			mprint(E, S, nodeinfo, "which = %d\n", which);

			sfatal(E, S, "Attempt to access NUMAREGION_WRCNT register "
				"outside number of NUMAREGIONs");
		}

		return S->N->regions[which]->nwrites;
	}
	else if (addr == SUPERH_NUMAREGION_COUNT)
	{
		return S->N->count;
	}
	else if (addr == SUPERH_BATTPERCENT)
	{
		return (int)ceil(100*((Batt*)S->BATT)->battery_remaining / ((Batt*)S->BATT)->battery_capacity);
	}
	else if ((addr >= SUPERH_SENSREAD_BEGIN) && (addr < SUPERH_SENSREAD_END))
	{
		int	which;
		ulong	tmp;

		//TODO: we should be doing it this way for NIC regs too, why aren't we ??
		which = ((addr - SUPERH_SENSREAD_BEGIN) >> 4) & 0xFFF;
		if (which >= MAX_NODE_SENSORS)
		{
			sfatal(E, S, "Attempt to access sensor register outside number of sensors");
			return 0;
		}

//fprintf(stderr, "sensor being read has value %f\n", S->sensors[which].reading);

/*
	BUG: this currently works fine for host=macos, clientcpu=superH,
		but will not work when there is andiannes mismatch.
		what we should really do, is 
		(1) autodetect endiannes during compile and get rid
		of the SF_X_endian stuff or clean it up at least

		(2) maintain a var in sim of what host endian is,
			in the gloabl M structure

		(3) when endian diff, marshall the data into
		tmp sinstead of the simple memmove, along lines of

	       	((uchar *)&tmp)[3] = ((uchar *)&reading)[0];
        	((uchar *)&tmp)[2] = ((uchar *)&reading)[1];
        	((uchar *)&tmp)[1] = ((uchar *)&reading)[2];
        	((uchar *)&tmp)[0] = ((uchar *)&reading)[3];

		etc
*/
		memmove(&tmp, &S->sensors[which].reading, sizeof(ulong));
		return tmp;
	}

	return S->devreadlong(E, S, addr);
}

ushort
devportreadword(Engine *E, State *S, ulong addr)
{
	mprint(E, S, nodeinfo, "Word access (read) at address 0x" UHLONGFMT "\n", addr);
	sfatal(E, S, "Address not in main mem, and not in I/O space either !");

	return 0;
}

uchar
devportreadbyte(Engine *E, State *S, ulong addr)
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
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_DST register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = S->superH->NIC_IFCS[whichifc].IFC_DST[offset];
	}
	else if ((addr >= SUPERH_NIC_OUI) && (addr < (SUPERH_NIC_OUI+SUPERH_NIC_REG_SPACING)))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_OUI register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = S->superH->NIC_IFCS[whichifc].IFC_OUI[offset];
	}
	else if ((addr >= SUPERH_NIC_NCSENSE) && (addr < SUPERH_NIC_NCSENSE+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCSENSE register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)(S->superH->NIC_IFCS[whichifc].IFC_CNTR_CSENSE_ERR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_CSENSE) && (addr < SUPERH_NIC_CSENSE+SUPERH_NIC_REG_SPACING))
	{
		int		i, whichifc = (addr >> 4) & 0xFFF;
		Netsegment	*curseg;


		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCSENSE register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}
		
		curseg = &E->netsegs[S->superH->NIC_IFCS[whichifc].segno];
		data = 0;

		for (i = 0; i < curseg->cur_queue_width; i++)
		{
			//this is too ideal: we should return 1 only if the snr 
			//if (curseg->segbufs[i].bits_left > 0)
			//{
			//	data = 1;
			//}

			if (check_snr(E, curseg, ((State *)curseg->segbufs[i].src_node), S) > curseg->minsnr)
			{
				data = 1;
			}
		}
	}
	else if ((addr >= SUPERH_NIC_NCOLLS) && (addr < SUPERH_NIC_NCOLLS+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCOLLS register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)(S->superH->NIC_IFCS[whichifc].IFC_CNTR_COLLS_ERR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_MAXFSZ) && (addr < SUPERH_NIC_MAXFSZ+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_FSZ register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].frame_bits/8 >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_RXFSZ) && (addr < SUPERH_NIC_RXFSZ+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;
		Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_FSZ register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		/*								*/
		/*	If theres nothing in the localbuf, then the next	*/
		/*	framesize is the one at oldestidx, since rx_localbuf	*/
		/*	and rx_localbuf_framesize don't get set until you	*/
		/*	start reading from them.				*/
		/*								*/
		if (ifcptr->rx_localbuf_h2o == 0)
		{
//fprintf(stderr, "+++SUPERH_NIC_RXFSZ = %d\n", ifcptr->rx_fifo_framesizes[ifcptr->rx_fifo_oldestidx]);
			data = (uchar)(ifcptr->rx_fifo_framesizes[ifcptr->rx_fifo_oldestidx] >> (offset*8))&0xFF;
		}
		else
		{
//fprintf(stderr, "---SUPERH_NIC_RXFSZ = %d\n", ifcptr->rx_localbuf_framesize);
			data = (uchar)(ifcptr->rx_localbuf_framesize >> (offset*8))&0xFF;
		}
	}

	else if ((addr >= SUPERH_NIC_BRR) && (addr < SUPERH_NIC_BRR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_BRR register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		/*	NIC_BRR contains network speed in Kb/s		*/
		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].IFC_BRR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_NMR) && (addr < SUPERH_NIC_NMR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "You should not be reading NIC_NMR !?");
	}
	else if ((addr >= SUPERH_NIC_NCR) && (addr < SUPERH_NIC_NCR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NCR register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].IFC_NCR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_TDR) && (addr < SUPERH_NIC_TDR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "You should not be reading NIC_TDR !?");
	}
	else if ((addr >= SUPERH_NIC_NSR) && (addr < SUPERH_NIC_NSR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;
		int	offset = addr & 0xF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_NSR register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = (uchar)((int)S->superH->NIC_IFCS[whichifc].IFC_NSR >> (offset*8))&0xFF;
	}
	else if ((addr >= SUPERH_NIC_RDR) && (addr < SUPERH_NIC_RDR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			mprint(E, S, nodeinfo,
				"Byte access to address [0x" UHLONGFMT "] (NIC_RDR register)\n",
				addr);
			mprint(E, S, nodeinfo, "whichifc = %d\n", whichifc);

			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
		}

		data = nic_rx_dequeue(E, S, whichifc);
//fprintf(stderr, "RDR returning [%d]\n", data);
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
		rnd = (ulong)mrandom(E);
			
		data = (uchar)(rnd >> (offset*8))&0xFF;
	}
	else if (addr == SUPERH_SIMCMD_DATA)
	{
		sfatal(E, S, "You should not be reading SIMCMD_DATA !?");
	}
	else if (addr == SUPERH_SIMCMD_CTL)
	{
		sfatal(E, S, "You should not be reading SIMCMD_CTL !?");
	}
	else if ((addr >= SUPERH_XLOC_BEGIN) && (addr < SUPERH_XLOC_END))
	{
		int	offset;
		uvlong	tmp;

		memmove(&tmp, &S->xloc, sizeof(uvlong));
		offset = addr - SUPERH_XLOC_BEGIN;
		data = (uchar)(tmp >> (offset*8))&0xFF;
//fprintf(stderr, "S->xloc = [%f], offset=[%d], XLOC returning [%d]\n", S->xloc, offset, data);
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
	else if (addr == SUPERH_DEVLOGPRINT)
	{
		sfatal(E, S, "You should not be reading DEVLOGPRINT !?");
	}
	else
	{
		return S->devreadbyte(E, S, addr);
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
devportwritelong(Engine *E, State *S, ulong addr, ulong data)
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
			mprint(E, S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_RDCNT register)\n",
				addr);
			mprint(E, S, nodeinfo, "which = %d\n", which);

			sfatal(E, S, "Attempt to access NUMAREGION_RDCNT register "
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
			mprint(E, S, nodeinfo,
				"Longword access to address [0x" UHLONGFMT 
				"] (NUMAREGION_WRCNT register)\n",
				addr);
			mprint(E, S, nodeinfo, "which = %d\n", which);

			sfatal(E, S, "Attempt to access NUMAREGION_WRCNT register "
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

	S->devwritelong(E, S, addr, data);

	return;
}

void
devportwriteword(Engine *E, State *S, ulong addr, ushort data)
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
	S->devwriteword(E, S, addr, data);

	return;
}

void
devportwritebyte(Engine *E, State *S, ulong addr, uchar data)
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
			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
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
			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

		S->superH->NIC_IFCS[whichifc].IFC_DST[offset] = data;
	}
	else if ((addr >= SUPERH_NIC_NMR) && (addr < SUPERH_NIC_NMR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "Use of NIC_NMR deprecated. Update your application!!");
	}
	else if ((addr >= SUPERH_NIC_BRR) && (addr < SUPERH_NIC_BRR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "Write to NIC_BRR ignored. What are you trying to do ?!");
	}
	else if ((addr >= SUPERH_NIC_NCR) && (addr < SUPERH_NIC_NCR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

/*
		if (!(data & (1 << 0)))
		{
			mprint(E, S, nodeinfo,
				"Dequeing and clearing all interrupts on IFC [%d]\n",
				whichifc);
			sfatal(E, S, "Are you sure thats what you wanted to do ?");

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
		}
*/

		switch (data)
		{
			case NIC_CMD_POWERUP:
			{
//fprintf(stderr, "## powering UP interface for node %d\n", S->NODE_ID);

				S->superH->NIC_IFCS[whichifc].IFC_STATE = NIC_STATE_LISTEN;

				break;
			}

			case NIC_CMD_POWERDN:
			{
//fprintf(stderr, "## powering DOWN interface for node %d\n", S->NODE_ID);

				S->superH->NIC_IFCS[whichifc].IFC_STATE = NIC_STATE_IDLE;

				break;
			}
	
			case NIC_CMD_TRANSMIT:
			{
				Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];

//fprintf(stderr, "## triggering TRANSMIT interface for node %d\n", S->NODE_ID);

				ifcptr->tx_fifo_framesizes[ifcptr->tx_fifo_curidx] = ifcptr->tx_fifo_h2o;
				fifo_enqueue(E, S, TX_FIFO, whichifc);
				ifcptr->tx_fifo_h2o = 0;

				break;
			}

			default:
			{
				sfatal(E, S, "Invalid command in NIC_NCR");
			}
		}

		S->superH->NIC_IFCS[whichifc].IFC_NCR = data & 0xff;
	}
	else if ((addr >= SUPERH_NIC_TDR) && (addr < SUPERH_NIC_TDR+SUPERH_NIC_REG_SPACING))
	{
		int	whichifc = (addr >> 4) & 0xFFF;

		if (whichifc >= NIC_MAX_IFCS)
		{
			sfatal(E, S, "Attempt to access IFC register outside number of IFCs");
			return;
		}

		nic_tx_enqueue(E, S, data, whichifc);
	}
	else if ((addr >= SUPERH_NIC_NSR) && (addr < SUPERH_NIC_NSR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "You should not write into NSR");
	}
	else if ((addr >= SUPERH_NIC_RDR) && (addr < SUPERH_NIC_RDR+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "You should not write into RDR");
	}
	else if ((addr >= SUPERH_NIC_MAXFSZ) && (addr < SUPERH_NIC_MAXFSZ+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "You should not write into NIC_MAXFSZ");
	}
	else if ((addr >= SUPERH_NIC_RXFSZ) && (addr < SUPERH_NIC_RXFSZ+SUPERH_NIC_REG_SPACING))
	{
		sfatal(E, S, "You should not write into NIC_RXFSZ");
	}
	else if ((addr >= SUPERH_LOGMARK_BEGIN) &&
		(addr < SUPERH_LOGMARK_END))
	{
		char	logtag[MAX_NAMELEN];


		/*							*/
		/*	Single byte memory access is used to trigger	*/
		/*	logging.  If read, we identify which log tag	*/
		/*	The logs go into the global simulation logfile	*/
		/*							*/
		int	which = addr - SUPERH_LOGMARK_BEGIN;

		msnprint(&logtag[0], MAX_NAMELEN, "NODE%d_LOGMARK_TAG_%d", S->NODE_ID, which);
		m_dumpnode(E, S->NODE_ID, E->logfilename, M_OWRITE, logtag, "");
	}
	else if ((addr >= SUPERH_NETTRACEMARK_BEGIN) &&
		(addr < SUPERH_NETTRACEMARK_END))
	{
		int		i, j;
		char		logtag[MAX_NAMELEN];
		Ifc		*ifcptr;
		Netsegment	*Seg;


		/*							*/
		/*	Single byte memory access is used to trigger	*/
		/*	logging.  Note that there is a whole bank of	*/
		/*	log trigger addresses per IFC.			*/
		/*		If read, we identify which log tag	*/
		/*	The log is replicated into ALL current net	*/
		/*	trace log files.				*/
		/*							*/
		int	which = addr - SUPERH_NETTRACEMARK_BEGIN;

		msnprint(&logtag[0], MAX_NAMELEN, "NODE%d_NETTRACEMARK_TAG_%d", S->NODE_ID, which);

		for (i = 0; i < S->superH->NIC_NUM_IFCS; i++)
		{
			ifcptr = &S->superH->NIC_IFCS[i];
			Seg = &E->netsegs[ifcptr->segno];

			for (j = 0; j < Seg->num_seg2files; j++)
			{
				m_dumpnode(E, S->NODE_ID, Seg->seg2filenames[j], M_OWRITE, logtag, "--");
			}
		}
	}
	else if ((addr >= SUPERH_RAND_BEGIN) && (addr < SUPERH_RAND_END))
	{
		sfatal(E, S, "You should not write into RAND");
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
			State		*tmpstate = E->cp;
			extern	int	sf_superh_parse(void);
			extern	int	sf_riscv_parse(void);

			S->cmdbuf[S->cmdbuf_nbytes++] = '\n';
			S->cmdbuf[S->cmdbuf_nbytes++] = '\0';
			munchinput(E, S->cmdbuf);
			S->cmdbuf_nbytes = 0;

			/*							*/
			/*	Everything in sf.y is relative to the		*/
			/*	current value of E->cp, so we have to 		*/
			/*	do a little dance, for the people, and their 	*/
			/*	cats, i guess, but then again, who cares ? 	*/
			/*							*/
			E->cp = S;
			yyengine = E;
			if (yyengine->cp->machinetype == MACHINE_SUPERH)
			{
				sf_superh_parse();
			}
			else if (yyengine->cp->machinetype == MACHINE_RISCV)
			{
				sf_riscv_parse();
			}
			E->cp = tmpstate;
		}
	}
	else if (addr == SUPERH_SIMCMD_CTL)
	{
		State		*tmpstate = E->cp;
		extern	int	sf_superh_parse(void);
		extern	int	sf_riscv_parse(void);


		/*								*/
		/*	Safe because nbytes only incr'd when SIMCMD_DATA is	*/
		/*	written to, and we do necessary checks there.		*/
		/*								*/
		S->cmdbuf[S->cmdbuf_nbytes++] = '\n';
		S->cmdbuf[S->cmdbuf_nbytes++] = '\0';
		munchinput(E, S->cmdbuf);
		S->cmdbuf_nbytes = 0;

		/*							*/
		/*	Everything in sf.y is relative to the		*/
		/*	current value of E->cp, so we have to 		*/
		/*	do a little dance, for the people, and their 	*/
		/*	cats, i guess, but then again, who cares ? 	*/
		/*							*/
		E->cp = S;
		yyengine = E;
		if (yyengine->cp->machinetype == MACHINE_SUPERH)
			{
				sf_superh_parse();
			}
			else if (yyengine->cp->machinetype == MACHINE_RISCV)
			{
				sf_riscv_parse();
			}
		E->cp = tmpstate;
	}
	else if ((addr >= SUPERH_ORBIT_BEGIN) && (addr < SUPERH_ORBIT_END))
	{
		sfatal(E, S, "You should not write into ORBIT data register");
	}
	else if ((addr >= SUPERH_VELOCITY_BEGIN) && (addr < SUPERH_VELOCITY_END))
	{
		sfatal(E, S, "You should not write into VELOCITY data register");
	}
	else if ((addr >= SUPERH_XLOC_BEGIN) && (addr < SUPERH_XLOC_END))
	{
		sfatal(E, S, "You should not write into XLOC data register");
	}
	else if ((addr >= SUPERH_YLOC_BEGIN) && (addr < SUPERH_YLOC_END))
	{
		sfatal(E, S, "You should not write into YLOC data register");
	}
	else if ((addr >= SUPERH_ZLOC_BEGIN) && (addr < SUPERH_ZLOC_END))
	{
		sfatal(E, S, "You should not write into ZLOC data register");
	}
	else if ((addr >= SUPERH_SENSWRITE_BEGIN) && (addr < SUPERH_SENSWRITE_END))
	{
/*
		int	which, offset;
		uvlong	tmp;

		which = ((addr - SUPERH_SENSWRITE_BEGIN) >> 4) & 0xFFF;
		if (which >= MAX_NODE_SENSORS)
		{
			sfatal(E, S, "Attempt to access sensor register outside number of sensors");
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
			mlog(E, S, "%c", data);
		}
	}
	else
	{
		S->devwritebyte(E, S, addr, data);
	}

	return;
}
