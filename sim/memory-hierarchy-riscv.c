/*
	Copyright (c) 2019, Vasileios Tsoutsouras (author)
 
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

tuck void
riscVwritebyte(Engine *E, State *S, ulong vaddr, ulong xdata)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	uchar		data = xdata & 0xFF;
	
	/*								*/
	/*	Translate address. If error occured, we do nothing:	*/
	/*	faulting instruction will get re-executed after the	*/
	/*	exception is handled.					*/
	/*								*/
	trans.vaddr = vaddr;
	trans.error = 0;
	S->vmtranslate(E, S, MEM_WRITE_BYTE, &trans);
	if (trans.error)
	{
		return;
	}

	paddr	= trans.paddr;
	inram	= 0;

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND))
	{
		inram = 1;
		latency = S->mem_w_latency;
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		devportwritebyte(E, S, vaddr, data);

		return;
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
		S->superH->B->paddr_bus = paddr;
	}

	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, MEM_WRITE_BYTE, paddr);
	}


	/*								  */
	/*   Currently only support write-back, LRU replacement. Real     */
	/*   SH3 does write-thru or write-back, only LRU + has WB-buffer. */
	/*								  */
	if (S->superH->cache_activated && trans.cacheable)
	{
		int i, max = 0, oldest;
		ulong tag, index;


		/*		Extract	Tag			*/
		tag = (paddr >> (S->superH->C->indexbits+S->superH->C->offsetbits));

		/*		Extract index			*/
		index = ((paddr & (0xFFFFFFFF>>S->superH->C->tagbits)) >>(S->superH->C->offsetbits));

		/*	Search through Set for a match on Tag	*/
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].tag == tag))
			{
				/*  Found block, let write proceed sans stall */
				S->superH->C->writehit++;

				/*  memmove will screw up byte order  */
				S->MEM[(paddr)-S->MEMBASE] = data;

				return;
			}
		}
		
		S->superH->C->writemiss++;

		/*	LRU Replacement. Emulates the SH3's 6-bit LRU	*/ 
		/*	field indirectly.				*/
		oldest = index*S->superH->C->assoc;
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			/*	Age all blocks in set by 1, mod 2^6	*/
			/* 	SH3 uses 6-bit LRU history 		*/
			S->superH->C->blocks[i].timestamp += 1;
			S->superH->C->blocks[i].timestamp %= 2<<6;

			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].timestamp > max))
			{
				max =  S->superH->C->blocks[i].timestamp;
				oldest = i;
			}
		}

		/*							*/
		/*	Write-miss : mem_w_latency cycle penalty to 	*/
		/*	write line contents to memory, if block valid	*/
		/*							*/
		if (S->superH->C->blocks[oldest].valid)
		{
			S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
		}

		S->superH->C->blocks[oldest].tag = tag;
		S->superH->C->blocks[oldest].valid = 1;
		S->superH->C->blocks[oldest].timestamp = 0;
	}
	
	if (inram)
	{
		S->MEM[paddr - S->MEMBASE] = data;
	}
	
	if (!S->superH->cache_activated || !trans.cacheable)
	{
		S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
	}

	return;
}

tuck void 	
riscVwriteword(Engine *E, State *S, ulong vaddr, ulong xdata)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	ushort		data = xdata & 0xFFFF;
	
	/*								*/
	/*	Translate address. If error occured, we do nothing:	*/
	/*	faulting instruction will get re-executed after the	*/
	/*	exception is handled.					*/
	/*								*/
	trans.vaddr = vaddr;
	trans.error = 0;
	S->vmtranslate(E, S, MEM_WRITE_WORD, &trans);
	if (trans.error)
	{
		return;
	}

	paddr	= trans.paddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0001)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND-1)) /* -1 for 2nd byte */
	{
		inram = 1;
		latency = S->mem_w_latency;
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		devportwriteword(E, S, vaddr, data);

		return;
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
		S->superH->B->paddr_bus = paddr;
	}

	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, MEM_WRITE_WORD, paddr);
	}

	/*								  */
	/*   Currently only support write-back, LRU replacement. Real     */
	/*   SH3 does write-thru or write-back, only LRU + has WB-buffer. */
	/*								  */
	if (S->superH->cache_activated && S->superH->C->size != 0)
	{
		int i, max = 0, oldest;
		ulong tag, index;

		/*		Extract	Tag			*/
		tag = (paddr >> (S->superH->C->indexbits+S->superH->C->offsetbits));

		/*		Extract index			*/
		index = ((paddr & (0xFFFFFFFF>>S->superH->C->tagbits))>>(S->superH->C->offsetbits));

		/*	Search through Set for a match on Tag	*/
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].tag == tag))
			{
				/*  Found block, let write proceed sans stall */
				S->superH->C->writehit++;
				/*  memmove will screw up byte order  */
				write_2(S, data, &S->MEM[paddr - S->MEMBASE]);
				
				/*S->MEM[paddr - S->MEMBASE] = (uchar)((data>>8)&0xFF);
				S->MEM[paddr+1 - S->MEMBASE] = (uchar)data&0xFF;*/

				return;
			}
		}
		
		S->superH->C->writemiss++;

		/*	LRU Replacement. Emulates the SH3's 6-bit LRU	*/ 
		/*	field indirectly.				*/
		oldest = index*S->superH->C->assoc;
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			//BUG/TODO: check below
			/*	Age all blocks in set by 1, mod 2^6	*/
			/* 	SH3 uses 6-bit LRU history 		*/
			S->superH->C->blocks[i].timestamp += 1;
			S->superH->C->blocks[i].timestamp %= 2<<6;

			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].timestamp >max))
			{
				max =  S->superH->C->blocks[i].timestamp;
				oldest = i;
			}
		}

		/*							*/
		/*	Write miss: mem_w_latency cycle penalty to 	*/
		/*	write line contents to memory, if block valid	*/
		/*							*/
		if (S->superH->C->blocks[oldest].valid)
		{
			S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
		}

		S->superH->C->blocks[oldest].tag = tag;
		S->superH->C->blocks[oldest].valid = 1;
		S->superH->C->blocks[oldest].timestamp = 0;
	}
	
	if (inram)
	{
		write_2(S, data, &S->MEM[paddr - S->MEMBASE]);
		
		/*S->MEM[paddr - S->MEMBASE] = (uchar)((data>>8)&0xFF);
		S->MEM[paddr+1 - S->MEMBASE] = (uchar)data&0xFF;*/
	}
	
	if (!S->superH->cache_activated || !trans.cacheable)
	{
		S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
	}

	return;
}

tuck void 	
riscVwritelong(Engine *E, State *S, ulong vaddr, ulong data)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	
	/*								*/
	/*	Translate address. If error occured, we do nothing:	*/
	/*	faulting instruction will get re-executed after the	*/
	/*	exception is handled.					*/
	/*								*/
	trans.vaddr = vaddr;
	trans.error = 0;
	S->vmtranslate(E, S, MEM_WRITE_LONG, &trans);
	if (trans.error)
	{
		return;
	}

	paddr	= trans.paddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0011)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 3)) /* -3 for 4th byte */
	{
		inram = 1;
		latency = S->mem_w_latency;
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		devportwritelong(E, S, vaddr, data);

		return;
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
		S->superH->B->paddr_bus = paddr;
	}

	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, MEM_WRITE_LONG, paddr);
	}

	/*								*/
	/*   Currently only support write-back, LRU replacement. Real   */
	/*   SH3 does write-thru or write-back, LRU + has WB-buffer.	*/
	/*								*/
	if (S->superH->cache_activated && S->superH->C->size != 0)
	{
		int i, max = 0, oldest;
		ulong tag, index;

		/*		Extract	Tag			*/
		tag = (paddr >> (S->superH->C->indexbits+S->superH->C->offsetbits));

		/*		Extract index			*/
		index = ((paddr & (0xFFFFFFFF>>S->superH->C->tagbits)) >> (S->superH->C->offsetbits));

		/*	Search through Set for a match on Tag	*/
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].tag == tag))
			{
				/* Found block, let write proceed sans stall */
				S->superH->C->writehit++;
				/*	memmove will screw up byte order   */
				write_4(S, data, &(S->MEM[paddr - S->MEMBASE]));
				
				/*S->MEM[paddr - S->MEMBASE] =(uchar)((data>>24)&0xFF);
				S->MEM[paddr+1 - S->MEMBASE] =(uchar)((data>>16)&0xFF);
				S->MEM[paddr+2 - S->MEMBASE] =(uchar)((data>>8)&0xFF);
				S->MEM[paddr+3 - S->MEMBASE] =(uchar)data&0xFF;*/
				
				return;
			}
		}

		S->superH->C->writemiss++;

		/*	LRU Replacement. Emulates the SH3's 6-bit LRU	*/ 
		/*	field indirectly.				*/
		oldest = index*S->superH->C->assoc;
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			/*	Age all blocks in set by 1, mod 2^6	*/
			/* 	SH3 uses 6-bit LRU history 		*/
			S->superH->C->blocks[i].timestamp += 1;
			S->superH->C->blocks[i].timestamp %= 2<<6;

			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].timestamp >max))
			{
				max =  S->superH->C->blocks[i].timestamp;
				oldest = i;
			}
		}

		/*							*/
		/*	Write miss: mem_w_latency cycle penalty to 	*/
		/*	write line contents to memory, if block valid	*/
		/*							*/
		if (S->superH->C->blocks[oldest].valid)
		{
			S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
		}

		S->superH->C->blocks[oldest].tag = tag;
		S->superH->C->blocks[oldest].valid = 1;
		S->superH->C->blocks[oldest].timestamp = 0;
	}
	
	if (inram)
	{
		write_4(S, data, &(S->MEM[paddr - S->MEMBASE]));
		
		/*S->MEM[paddr - S->MEMBASE] = (uchar)((data>>24)&0xFF);
		S->MEM[paddr+1 - S->MEMBASE] = (uchar)((data>>16)&0xFF);
		S->MEM[paddr+2 - S->MEMBASE] = (uchar)((data>>8)&0xFF);
		S->MEM[paddr+3 - S->MEMBASE] = (uchar)data&0xFF;*/
	}
	
	if (!S->superH->cache_activated || !trans.cacheable)
	{
		S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
	}

	return;
}


tuck uchar 	
riscVreadbyte(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	uchar		data = 0;
	
	/*								*/
	/*	Translate address. If error occured, we do nothing:	*/
	/*	faulting instruction will get re-executed after the	*/
	/*	exception is handled.					*/
	/*								*/
	trans.vaddr = vaddr;
	trans.error = 0;
	S->vmtranslate(E, S, MEM_READ_BYTE, &trans);
	if (trans.error)
	{
		/*							*/
		/*	This value doesnt actually reach instr, since	*/
		/*	an exception has been raised and instr will 	*/
		/*	get reexecuted					*/
		/*							*/
		return 0;
	}


	paddr	= trans.paddr;
	inram	= 0;

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND))
	{
		inram = 1;
		latency = S->mem_r_latency;
		data = S->MEM[paddr - S->MEMBASE];
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return devportreadbyte(E, S, vaddr);
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
		S->superH->B->paddr_bus = paddr;
	}

	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, MEM_READ_BYTE, paddr);
	}

	/*								*/
	/*	Currently only support write-back, LRU replacement. Real*/
	/* 	SH3 does write-thru or write-back, LRU + has WB-buffer.	*/
	/*								*/
	if (S->superH->cache_activated && S->superH->C->size != 0)
	{
		int i, max = 0, oldest;
		ulong tag, index;

		/*		Extract	Tag			*/
		tag = (paddr >> (S->superH->C->indexbits+S->superH->C->offsetbits));

		/*		Extract index			*/
		index = ((paddr & (0xFFFFFFFF>>S->superH->C->tagbits)) >> (S->superH->C->offsetbits));

		/*	Search through Set for a match on Tag	*/
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].tag == tag))
			{
				/* Found block, let write proceed sans stall*/
				S->superH->C->readhit++;
				return data;
			}
		}

		S->superH->C->readmiss++;

		/*	LRU Replacement. Emulates the SH3's 6-bit LRU	*/ 
		/*	field indirectly.				*/
		oldest = index*S->superH->C->assoc;
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			/*	Age all blocks in set by 1, mod 2^6	*/
			/* 	SH3 uses 6-bit LRU history 		*/
			S->superH->C->blocks[i].timestamp += 1;
			S->superH->C->blocks[i].timestamp %= 2<<6;

			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].timestamp > max))
			{
				max =  S->superH->C->blocks[i].timestamp;
				oldest = i;
			}
		}

		S->stallaction(E, S, paddr, MEM_READ_STALL, latency);

		S->superH->C->blocks[oldest].tag = tag;
		S->superH->C->blocks[oldest].valid = 1;
		S->superH->C->blocks[oldest].timestamp = 0;
	}
	
	if (!S->superH->cache_activated || !trans.cacheable)
	{
		S->stallaction(E, S, paddr, MEM_READ_STALL, latency);
	}

	return data;
}

tuck ushort
riscVreadword(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	ushort		data = 0;
	
	/*								*/
	/*	Translate address. If error occured, we do nothing:	*/
	/*	faulting instruction will get re-executed after the	*/
	/*	exception is handled.					*/
	/*								*/
	trans.vaddr = vaddr;
	trans.error = 0;
	S->vmtranslate(E, S, MEM_READ_WORD, &trans);
	if (trans.error)
	{
		/*							*/
		/*	This value doesnt actually reach instr, since	*/
		/*	an exception has been raised and instr will 	*/
		/*	get reexecuted					*/
		/*							*/
		return 0;
	}


	paddr	= trans.paddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0001)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 1)) /* -1 for second byte */
	{
		inram = 1;
		latency = S->mem_r_latency;
		read_2(S, &S->MEM[paddr - S->MEMBASE], &data);
		//data = (ushort)(S->MEM[paddr - S->MEMBASE]<<8)|S->MEM[paddr+1 - S->MEMBASE];
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return devportreadword(E, S, vaddr);
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
		S->superH->B->paddr_bus = paddr;
	}

	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, MEM_READ_WORD, paddr);
	}

	/*								*/
	/*	Currently only support write-back, LRU replacement. Real*/
	/* 	SH3 does write-thru or write-back, only LRU + has WB-buffer.*/
	/*								*/
	if (S->superH->cache_activated && S->superH->C->size != 0)
	{
		int i, max = 0, oldest;
		ulong tag, index;

		/*		Extract	Tag			*/
		tag = (paddr >> (S->superH->C->indexbits+S->superH->C->offsetbits));

		/*		Extract index			*/
		index = ((paddr & (0xFFFFFFFF>>S->superH->C->tagbits))>>(S->superH->C->offsetbits));

		/*	Search through Set for a match on Tag	*/
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].tag == tag))
			{
			     	/* Found block, let write proceed sans stall */
				S->superH->C->readhit++;
			     	return data;
			}
		}

		S->superH->C->readmiss++;

		/*	LRU Replacement. Emulates the SH3's 6-bit LRU	*/ 
		/*	field indirectly.				*/
		oldest = index*S->superH->C->assoc;
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			/*	Age all blocks in set by 1, mod 2^6	*/
			/* 	SH3 uses 6-bit LRU history 		*/
			S->superH->C->blocks[i].timestamp += 1;
			S->superH->C->blocks[i].timestamp %= 2<<6;

			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].timestamp >max))
			{
				max =  S->superH->C->blocks[i].timestamp;
				oldest = i;
			}
		}

		S->stallaction(E, S, paddr, MEM_READ_STALL, latency);

		S->superH->C->blocks[oldest].tag = tag;
		S->superH->C->blocks[oldest].valid = 1;
		S->superH->C->blocks[oldest].timestamp = 0;
	}
	
	if (!S->superH->cache_activated || !trans.cacheable)
	{
		S->stallaction(E, S, paddr, MEM_READ_STALL, latency);
	}

	return data;
}

tuck ulong 	
riscVreadlong(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	ulong		data = 0;

	/*								*/
	/*	Translate address. If error occured, we do nothing:	*/
	/*	faulting instruction will get re-executed after the	*/
	/*	exception is handled.					*/
	/*								*/
	trans.vaddr = vaddr;
	trans.error = 0;
	S->vmtranslate(E, S, MEM_READ_LONG, &trans);
	if (trans.error)
	{
		/*							*/
		/*	This value doesnt actually reach instr, since	*/
		/*	an exception has been raised and instr will 	*/
		/*	get reexecuted					*/
		/*							*/
		return 0;
	}


	paddr	= trans.paddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0011)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 3)) /* -3 for 4th byte */
	{
		inram = 1;
		latency = S->mem_r_latency;
		read_4(S, &(S->MEM[paddr - S->MEMBASE]), &data);
		/*data = (ulong)(S->MEM[paddr - S->MEMBASE]<<24)|\
				(S->MEM[paddr+1 - S->MEMBASE]<<16)|\
				(S->MEM[paddr+2 - S->MEMBASE]<<8)|\
				S->MEM[paddr+3 - S->MEMBASE];*/
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return devportreadlong(E, S, vaddr);
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
		S->superH->B->data_bus = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
		S->superH->B->paddr_bus = paddr;
	}

	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, MEM_READ_LONG, paddr);
	}

	/*									*/
	/*	Currently only support write-back, LRU replacement. Real	*/
	/* 	SH3 does write-thru or write-back, only LRU + has WB-buffer.	*/
	/*									*/
	if (S->superH->cache_activated && S->superH->C->size != 0)
	{
		int i, max = 0, oldest;
		ulong tag, index;

		/*		Extract	Tag			*/
		tag = (paddr >> (S->superH->C->indexbits+S->superH->C->offsetbits));

		/*		Extract index			*/
		index = ((paddr & (0xFFFFFFFF>>S->superH->C->tagbits))>>(S->superH->C->offsetbits));

		/*	Search through Set for a match on Tag	*/
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].tag == tag))
			{
			     	/* Found block, let write proceed sans stall*/
				S->superH->C->readhit++;
			     	return data;
			}
		}

		S->superH->C->readmiss++;

		/*	LRU Replacement. Emulates the SH3's 6-bit LRU	*/ 
		/*	field indirectly.				*/
		oldest = index*S->superH->C->assoc;
		for (i = (index*S->superH->C->assoc); i < (((index+1)*S->superH->C->assoc)); i++)
		{
			/*	Age all blocks in set by 1, mod 2^6	*/
			/* 	SH3 uses 6-bit LRU history 		*/
			S->superH->C->blocks[i].timestamp += 1;
			S->superH->C->blocks[i].timestamp %= 2<<6;

			if (S->superH->C->blocks[i].valid && (S->superH->C->blocks[i].timestamp >max))
			{
				max =  S->superH->C->blocks[i].timestamp;
				oldest = i;
			}
		}

		S->stallaction(E, S, paddr, MEM_READ_STALL, latency);

		S->superH->C->blocks[oldest].tag = tag;
		S->superH->C->blocks[oldest].valid = 1;
		S->superH->C->blocks[oldest].timestamp = 0;
	}
	
	if (!S->superH->cache_activated || !trans.cacheable)
	{
		S->stallaction(E, S, paddr, MEM_READ_STALL, latency);
	}

	return data;
}
