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


//Read from memory 2 bytes
void
big_endian_read_2(uchar * source, ushort * target)
{
	*target = (ushort)((source[1])|(source[0]<<8));
	
	return;
}

void
little_endian_read_2(uchar * source, ushort * target)
{
	*target = (ushort)((source[0])|(source[1]<<8));
	
	return;
}

void
read_2(State * S, uchar * source, ushort * target)
{
	if (S->endian == Little)
	{
		little_endian_read_2(source, target);
	}
	else
	{
		big_endian_read_2(source, target);
	}
}

//Read from memory 4 bytes
void
big_endian_read_4(uchar * source, ulong * target)
{
	*target = (ulong)(source[3])|(source[2]<<8)|(source[1]<<16)|(source[0]<<24);

	return;
}

void
little_endian_read_4(uchar * source, ulong * target)
{
	*target = (ulong)(source[0])|(source[1]<<8)|(source[2]<<16)|(source[3]<<24);

	return;
}

void
read_4(State * S, uchar * source, ulong * target)
{
	if (S->endian == Little)
	{
		little_endian_read_4(source, target);
	}
	else
	{
		big_endian_read_4(source, target);
	}
}


//Write to memory 2 bytes
void
big_endian_write_2(ushort source, uchar * target)
{
	target[1] = (uchar)source&0xFF;
	target[0] = (uchar)((source>>8)&0xFF);
	
	return;
}

void
little_endian_write_2(ushort source, uchar * target)
{
	target[0] = (uchar)source&0xFF;
	target[1] = (uchar)((source>>8)&0xFF);
	
	return;
}

void
write_2(State *S, ushort source, uchar * target)
{
	if (S->endian == Little)
	{
		little_endian_write_2(source, target);
	}
	else
	{
		big_endian_write_2(source, target);
	}
}


//Write to memory 4 bytes
void
big_endian_write_4(ulong source, uchar * target)
{
	target[3] = (uchar)(source&0xFF);
	target[2] = (uchar)((source>>8)&0xFF);
	target[1] = (uchar)((source>>16)&0xFF);
	target[0] = (uchar)((source>>24)&0xFF);
	
	return;
}

void
little_endian_write_4(ulong source, uchar * target)
{
	target[0] = (uchar)(source&0xFF);
	target[1] = (uchar)((source>>8)&0xFF);
	target[2] = (uchar)((source>>16)&0xFF);
	target[3] = (uchar)((source>>24)&0xFF);

	return;
}

void
write_4(State * S, ulong source, uchar * target)
{
	if (S->endian == Little)
	{
		little_endian_write_4(source, target);
	}
	else
	{
		big_endian_write_4(source, target);
	}
}


int
superHcache_init(Engine *E, State *S, int size, int blocksize, int assoc)
{	
	if (S->superH->C != NULL)
	{
		mfree(E, S->superH->C, "(Cache *)S->superH->C");
		if (S->superH->C->blocks != NULL)
		{
			mfree(E, S->superH->C->blocks, "(Cache *)S->superH->C");
		}
	}

	S->superH->C = (Cache *)mcalloc(E, 1, sizeof(Cache), "(Cache *)S->superH->C");
	if (S->superH->C == NULL)
	{
		merror(E, "mcalloc failed for (Cache *)C in cache_init()");
		return -1;
	}

	if ((blocksize > size)||(size <= 0)||(assoc <= 0)||\
		(blocksize <= 0)||(size%blocksize != 0)||\
		((size/blocksize)%assoc != 0))
	{
		merror(E, "cacheinit() failed: Invalid Cache parameters");
		return -1;
	}

	S->superH->C->blocksize = blocksize;
	S->superH->C->assoc = assoc;
	S->superH->C->size = size;
	S->superH->C->nblocks = size/blocksize;

	S->superH->C->blocks = (CacheBlock *)mcalloc(E, S->superH->C->nblocks, sizeof(CacheBlock),
			"(CacheBlock *)S->superH->C->blocks");
	if (S->superH->C->blocks == NULL)
	{
		merror(E, "mcalloc failed for C->blocks in cache_init()");
		return -1;
	}

	S->superH->C->nsets = S->superH->C->nblocks/S->superH->C->assoc;
	S->superH->C->offsetbits = (int)ceil(log10(S->superH->C->blocksize)/log10(2));
	S->superH->C->indexbits = (int)ceil(log10(S->superH->C->nsets)/log10(2));
	S->superH->C->tagbits = SUPERH_MEMADDRBITS - (S->superH->C->offsetbits+S->superH->C->indexbits);
	S->superH->cache_activated = 1;

	mprint(E, S, nodeinfo, "\nCache Parameters:\n");
	mprint(E, S, nodeinfo, "\t\tSize: %d KBytes\n", S->superH->C->size/1024);
	mprint(E, S, nodeinfo, "\t\tBlock Size: %d Bytes\n", S->superH->C->blocksize);
	mprint(E, S, nodeinfo, "\t\tNumber of Blocks: %d\n", S->superH->C->nblocks);
	mprint(E, S, nodeinfo, "\t\tAssociativity: %d-way set associative\n", S->superH->C->assoc);
	mprint(E, S, nodeinfo, "\t\tNumber of Sets: %d\n", S->superH->C->nsets);
	mprint(E, S, nodeinfo, "\t\toffset:%d bits, index:%d bits, tag:%d bits\n\n",\
			 S->superH->C->offsetbits, S->superH->C->indexbits, S->superH->C->tagbits);

	return 0;
}

void
superHcache_deactivate(Engine *E, State *S)
{
	mprint(E, S, nodeinfo, "Cache deactivated\n");
	S->superH->cache_activated = 0;
}

tuck void
superHwritebyte(Engine *E, State *S, ulong vaddr, ulong xdata)
{
	int		inram, latency = 0;
	int		i, j, id;
	ulong		offset, destbase, destoffset;
	TransAddr	trans;
	ulong		paddr;
	uchar		data = xdata & 0xFF;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->vaddr_bus, vaddr);
		S->superH->B->vaddr_bus = vaddr;
	}

	/*								*/
	/*	   	Check numa/mapped regions (RAM only)		*/
	/*	Final effect is sort of like addressing a remote 	*/
	/*	mem device via a chip select, minus the chip select.	*/
	/*	Also equivalent to an MMU w/ regard to local accesses.	*/
	/*								*/
	/*	BUG/TODO: When we first enter the function and are 	*/
	/*	executing prologue, we don't want the tracking to be 	*/
	/*	triggered...						*/
	/*								*/
	i = m_find_numastack(S->PCSTACK[S->pcstackheight], S->superH->R[14],
		vaddr, S->Nstack, 0, S->Nstack->count);

	/*								*/
	/*	If address doesn't match a registered address using	*/
	/*	the current frame pointer, it might be that we are	*/
	/*	accessing an address on one of our parent's stacks.	*/
	/*	Unwind the stack (side-effect-free) and try to match.	*/
	/*								*/
	j = S->fpstackheight - 1;
	while (i < 0 && j >= 0)
	{
		i = m_find_numastack(S->PCSTACK[j], S->FPSTACK[j],
				vaddr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(vaddr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
		//fprintf(stderr, "!");
		X->regions[i]->nwrites++;

		id = X->regions[i]->map_id;
		if (id >= E->nnodes || id < 0) 
		{
			mprint(E, S, nodeinfo, "\nMap: invalid dst node, ID = %d\n", id);
			sfatal(E, S, "Bad NUMA destination map ID!");
		}

		/*										*/
		/*	As a heuristic to filter out uses of an address by the compiler,	*/
		/*	for purposes other than the variable associated with that address	*/
		/*	in the STABS debugging info, if the underlying variable being		*/
		/*	traced is not a pointer, we ignore values that look like code		*/
		/*	or data addresses.							*/
		/*										*/
		if (X->regions[i]->valuetrace && (X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
		{
			X->regions[i]->values[X->regions[i]->validx] = data;
			if (X->regions[i]->validx < MAX_NUMAREGION_VALUETRACE)
			{
				X->regions[i]->validx++;
			}

			if (X->regions[i]->validx == X->regions[i]->nvalues)
			{
				ulong	*tmp;

				X->regions[i]->nvalues *= 2;
				tmp = (ulong *)mrealloc(E, X->regions[i]->values,
					X->regions[i]->nvalues*sizeof(ulong), "realloc C->regions[i]->values in cache.c");
				if (tmp == NULL)
				{
					mprint(E, NULL, siminfo,
						"Resizing X->regions[i]->values to %d entries failed\n",
						X->regions[i]->nvalues);
					sfatal(E, S, "realloc failed for X->regions[i]->values in cache.c");
				}
				X->regions[i]->values = tmp;
			}
		}

		if (!X->regions[i]->private || (id == S->NODE_ID))
		{
			if (id == S->NODE_ID)
			{
				latency = X->regions[i]->local_write_latency;
			}
			else
			{
				latency = X->regions[i]->remote_write_latency;
			}


			D		= E->sp[id];
			destbase	= D->MEMBASE;
			offset		= vaddr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					vaddr, id);
				mprint(E, S, nodeinfo,
					"membase 0x" UHLONGFMT ", memsize 0x" UHLONGFMT 
					", destoffset 0x" UHLONGFMT "\n",
					D->MEMBASE, D->MEMSIZE, destoffset);
				sfatal(E, S, "Bad NUMA destination address access!");
			}

			D->MEM[destoffset] = data;
			paddr = D->MEMBASE + destoffset;

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
				S->superH->B->data_bus = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
				S->superH->B->paddr_bus = paddr;
			}

			S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);

			return;
		}
	}

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
superHwriteword(Engine *E, State *S, ulong vaddr, ulong xdata)
{
	int		inram, latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase;
	TransAddr	trans;
	ulong		paddr;
	ushort		data = xdata & 0xFFFF;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->vaddr_bus, vaddr);
		S->superH->B->vaddr_bus = vaddr;
	}

	/*								*/
	/*	   	Check numa/mapped regions (RAM only)		*/
	/*	Final effect is sort of like addressing a remote 	*/
	/*	mem device via a chip select, minus the chip select.	*/
	/*	Also equivalent to an MMU w/ regard to local accesses.	*/
	/*								*/
	/*	BUG/TODO: When we first enter the function and are 	*/
	/*	executing prologue, we don't want the tracking to be 	*/
	/*	triggered...						*/
	/*								*/
	i = m_find_numastack(S->PCSTACK[S->pcstackheight], S->superH->R[14],
		vaddr, S->Nstack, 0, S->Nstack->count);

	/*								*/
	/*	If address doesn't match a registered address using	*/
	/*	the current frame pointer, it might be that we are	*/
	/*	accessing an address on one of our parent's stacks.	*/
	/*	Unwind the stack (side-effect-free) and try to match.	*/
	/*								*/
	j = S->fpstackheight - 1;
	while (i < 0 && j >= 0)
	{
		i = m_find_numastack(S->PCSTACK[j], S->FPSTACK[j],
				vaddr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(vaddr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
		//fprintf(stderr, "!");
		X->regions[i]->nwrites++;

		id = X->regions[i]->map_id;
		if (id >= E->nnodes || id < 0) 
		{
			mprint(E, S, nodeinfo, "\nMap: invalid dst node, ID = %d\n", id);
			sfatal(E, S, "Bad NUMA destination map ID!");
		}

		/*										*/
		/*	As a heuristic to filter out uses of an address by the compiler,	*/
		/*	for purposes other than the variable associated with that address	*/
		/*	in the STABS debugging info, if the underlying variable being		*/
		/*	traced is not a pointer, we ignore values that look like code		*/
		/*	or data addresses.							*/
		/*										*/
		if (X->regions[i]->valuetrace && (X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
		{
			/*								*/
			/*	For value tracing, if access is bigger than the		*/
			/*	underlying data, mask off excess.			*/
			/*								*/
			ulong	mask = ~0UL;
			ulong	size = X->regions[i]->endaddr - X->regions[i]->startaddr;
			if (size < 4)
			{
				mask = (1 << (size << 3)) - 1;
			}

			X->regions[i]->values[X->regions[i]->validx] = data & mask;
			if (X->regions[i]->validx < MAX_NUMAREGION_VALUETRACE)
			{
				X->regions[i]->validx++;
			}

			if (X->regions[i]->validx == X->regions[i]->nvalues)
			{
				ulong	*tmp;

				X->regions[i]->nvalues *= 2;
				tmp = (ulong *)mrealloc(E, X->regions[i]->values,
					X->regions[i]->nvalues*sizeof(ulong), "realloc C->regions[i]->values in cache.c");
				if (tmp == NULL)
				{
					mprint(E, NULL, siminfo,
						"Resizing X->regions[i]->values to %d entries failed\n",
						X->regions[i]->nvalues);
					sfatal(E, S, "realloc failed for X->regions[i]->values in cache.c");
				}
				X->regions[i]->values = tmp;
			}
		}

		if (!X->regions[i]->private || (id == S->NODE_ID))
		{
			if (id == S->NODE_ID)
			{
				latency = X->regions[i]->local_write_latency;
			}
			else
			{
				latency = X->regions[i]->remote_write_latency;
			}

			D		= E->sp[id];
			destbase	= D->MEMBASE;
			offset		= vaddr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					vaddr, id);
				mprint(E, S, nodeinfo,
					"membase 0x" UHLONGFMT ", memsize 0x" UHLONGFMT 
					", destoffset 0x" UHLONGFMT "\n",
					D->MEMBASE, D->MEMSIZE, destoffset);
				sfatal(E, S, "Bad NUMA destination address access!");
			}

			D->MEM[destoffset] = (uchar)((data>>8)&0xFF);
			D->MEM[destoffset+1] = (uchar)data&0xFF;
			paddr = D->MEMBASE + destoffset;

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
				S->superH->B->data_bus = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
				S->superH->B->paddr_bus = paddr;
			}

			S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);

			return;
		}
	}

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
superHwritelong(Engine *E, State *S, ulong vaddr, ulong data)
{
	int		inram, latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase;
	TransAddr	trans;
	ulong		paddr;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->vaddr_bus, vaddr);
		S->superH->B->vaddr_bus = vaddr;
	}

	/*								*/
	/*	   	Check numa/mapped regions (RAM only)		*/
	/*	Final effect is sort of like addressing a remote 	*/
	/*	mem device via a chip select, minus the chip select.	*/
	/*	Also equivalent to an MMU w/ regard to local accesses.	*/
	/*								*/
	/*	BUG/TODO: When we first enter the function and are 	*/
	/*	executing prologue, we don't want the tracking to be 	*/
	/*	triggered...						*/
	/*								*/
	i = m_find_numastack(S->PCSTACK[S->pcstackheight], S->superH->R[14],
		vaddr, S->Nstack, 0, S->Nstack->count);

	/*								*/
	/*	If address doesn't match a registered address using	*/
	/*	the current frame pointer, it might be that we are	*/
	/*	accessing an address on one of our parent's stacks.	*/
	/*	Unwind the stack (side-effect-free) and try to match.	*/
	/*								*/
	j = S->fpstackheight - 1;
	while (i < 0 && j >= 0)
	{
		i = m_find_numastack(S->PCSTACK[j], S->FPSTACK[j],
				vaddr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(vaddr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
		//fprintf(stderr, "!");
		X->regions[i]->nwrites++;

		id = X->regions[i]->map_id;
		if (id >= E->nnodes || id < 0) 
		{
			mprint(E, S, nodeinfo, "\nMap: invalid dst node, ID = %d\n", id);
			sfatal(E, S, "Bad NUMA destination map ID!");
		}

		/*										*/
		/*	As a heuristic to filter out uses of an address by the compiler,	*/
		/*	for purposes other than the variable associated with that address	*/
		/*	in the STABS debugging info, if the underlying variable being		*/
		/*	traced is not a pointer, we ignore values that look like code		*/
		/*	or data addresses.							*/
		/*										*/
		if (X->regions[i]->valuetrace && (X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
		{
			/*								*/
			/*	For value tracing, if access is bigger than the		*/
			/*	underlying data, mask off excess.			*/
			/*								*/
			ulong	mask = ~0UL;
			ulong	size = X->regions[i]->endaddr - X->regions[i]->startaddr;
			if (size < 4)
			{
				mask = (1 << (size << 3)) - 1;
			}

			X->regions[i]->values[X->regions[i]->validx] = data & mask;
			if (X->regions[i]->validx < MAX_NUMAREGION_VALUETRACE)
			{
				X->regions[i]->validx++;
			}

			if (X->regions[i]->validx == X->regions[i]->nvalues)
			{
				ulong	*tmp;

				X->regions[i]->nvalues *= 2;
				tmp = (ulong *)mrealloc(E, X->regions[i]->values,
					X->regions[i]->nvalues*sizeof(ulong), "realloc C->regions[i]->values in cache.c");
				if (tmp == NULL)
				{
					mprint(E, NULL, siminfo,
						"Resizing X->regions[i]->values to %d entries failed\n",
						X->regions[i]->nvalues);
					sfatal(E, S, "realloc failed for X->regions[i]->values in cache.c");
				}
				X->regions[i]->values = tmp;
			}
		}

		if (!X->regions[i]->private || (id == S->NODE_ID))
		{
			if (id == S->NODE_ID)
			{
				latency = X->regions[i]->local_write_latency;
			}
			else
			{
				latency = X->regions[i]->remote_write_latency;
			}

			D		= E->sp[id];
			destbase	= D->MEMBASE;
			offset		= vaddr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					vaddr, id);
				mprint(E, S, nodeinfo,
					"membase 0x" UHLONGFMT ", memsize 0x" UHLONGFMT 
					", destoffset 0x" UHLONGFMT "\n",
					D->MEMBASE, D->MEMSIZE, destoffset);
				sfatal(E, S, "Bad NUMA destination address access!");
			}

			D->MEM[destoffset] =(uchar)((data>>24)&0xFF);
			D->MEM[destoffset+1] =(uchar)((data>>16)&0xFF);
			D->MEM[destoffset+2] =(uchar)((data>>8)&0xFF);
			D->MEM[destoffset+3] =(uchar)data&0xFF;
			paddr = D->MEMBASE + destoffset;

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
				S->superH->B->data_bus = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
				S->superH->B->paddr_bus = paddr;
			}

			S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);

			return;
		}
	}

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

/*									*/
/*	Reads data of each length from address Addr. An address error 	*/
/*	will occur if word data is read from an address other than 2n	*/
/*	or if longword data is read from an address other than 4n	*/
/*									*/
tuck uchar 	
superHreadbyte(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase;
	TransAddr	trans;
	ulong		paddr;
	uchar		data = 0;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->vaddr_bus, vaddr);
		S->superH->B->vaddr_bus = vaddr;
	}

	/*								*/
	/*	   	Check numa/mapped regions (RAM only)		*/
	/*	Final effect is sort of like addressing a remote 	*/
	/*	mem device via a chip select, minus the chip select.	*/
	/*	Also equivalent to an MMU w/ regard to local accesses.	*/
	/*								*/
	/*	BUG/TODO: When we first enter the function and are 	*/
	/*	executing prologue, we don't want the tracking to be 	*/
	/*	triggered...						*/
	/*								*/
	i = m_find_numastack(S->PCSTACK[S->pcstackheight], S->superH->R[14],
		vaddr, S->Nstack, 0, S->Nstack->count);

	/*								*/
	/*	If address doesn't match a registered address using	*/
	/*	the current frame pointer, it might be that we are	*/
	/*	accessing an address on one of our parent's stacks.	*/
	/*	Unwind the stack (side-effect-free) and try to match.	*/
	/*								*/
	j = S->fpstackheight - 1;
	while (i < 0 && j >= 0)
	{
		i = m_find_numastack(S->PCSTACK[j], S->FPSTACK[j],
				vaddr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(vaddr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
		X->regions[i]->nreads++;

		id = X->regions[i]->map_id;
		if (id >= E->nnodes || id < 0) 
		{
			mprint(E, S, nodeinfo, "\nMap: invalid dst node, ID = %d\n", id);
			sfatal(E, S, "Bad NUMA destination map ID!");
		}

		if (!X->regions[i]->private || (id == S->NODE_ID))
		{
			if (id == S->NODE_ID)
			{
				latency = X->regions[i]->local_read_latency;
			}
			else
			{
				latency = X->regions[i]->remote_read_latency;
			}

			D		= E->sp[id];
			destbase	= D->MEMBASE;
			offset		= vaddr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					vaddr, id);
				mprint(E, S, nodeinfo,
					"membase 0x" UHLONGFMT ", memsize 0x" UHLONGFMT 
					", destoffset 0x" UHLONGFMT "\n",
					D->MEMBASE, D->MEMSIZE, destoffset);
				sfatal(E, S, "Bad NUMA destination address access!");
			}

			data = D->MEM[destoffset];
			paddr = D->MEMBASE + destoffset;

			/*										*/
			/*	As a heuristic to filter out uses of an address by the compiler,	*/
			/*	for purposes other than the variable associated with that address	*/
			/*	in the STABS debugging info, if the underlying variable being		*/
			/*	traced is not a pointer, we ignore values that look like code		*/
			/*	or data addresses.							*/
			/*										*/
			if (X->regions[i]->valuetrace && (X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
			{
				X->regions[i]->values[X->regions[i]->validx] = data;
				if (X->regions[i]->validx < MAX_NUMAREGION_VALUETRACE)
				{
					X->regions[i]->validx++;
				}

				if (X->regions[i]->validx == X->regions[i]->nvalues)
				{
					ulong	*tmp;

					X->regions[i]->nvalues *= 2;
					tmp = (ulong *)mrealloc(E, X->regions[i]->values,
						X->regions[i]->nvalues*sizeof(ulong), 
						"realloc C->regions[i]->values in cache.c");
					if (tmp == NULL)
					{
						mprint(E, NULL, siminfo,
							"Resizing X->regions[i]->values to %d entries failed\n",
							X->regions[i]->nvalues);
						sfatal(E, S, "realloc failed for X->regions[i]->values in cache.c");
					}
					X->regions[i]->values = tmp;
				}
			}

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
				S->superH->B->data_bus = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
				S->superH->B->paddr_bus = paddr;
			}

			S->stallaction(E, S, paddr, MEM_READ_STALL, latency);

			return data;
		}
	}

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
superHreadword(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase;
	TransAddr	trans;
	ulong		paddr;
	ushort		data = 0;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->vaddr_bus, vaddr);
		S->superH->B->vaddr_bus = vaddr;
	}

	/*								*/
	/*	   	Check numa/mapped regions (RAM only)		*/
	/*	Final effect is sort of like addressing a remote 	*/
	/*	mem device via a chip select, minus the chip select.	*/
	/*	Also equivalent to an MMU w/ regard to local accesses.	*/
	/*								*/
	/*	BUG/TODO: When we first enter the function and are 	*/
	/*	executing prologue, we don't want the tracking to be 	*/
	/*	triggered...						*/
	/*								*/
	i = m_find_numastack(S->PCSTACK[S->pcstackheight], S->superH->R[14],
		vaddr, S->Nstack, 0, S->Nstack->count);

	/*								*/
	/*	If address doesn't match a registered address using	*/
	/*	the current frame pointer, it might be that we are	*/
	/*	accessing an address on one of our parent's stacks.	*/
	/*	Unwind the stack (side-effect-free) and try to match.	*/
	/*								*/
	j = S->fpstackheight - 1;
	while (i < 0 && j >= 0)
	{
		i = m_find_numastack(S->PCSTACK[j], S->FPSTACK[j],
				vaddr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(vaddr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
		X->regions[i]->nreads++;

		id = X->regions[i]->map_id;
		if (id >= E->nnodes || id < 0) 
		{
			mprint(E, S, nodeinfo, "\nMap: invalid dst node, ID = %d\n", id);
			sfatal(E, S, "Bad NUMA destination map ID!");
		}

		if (!X->regions[i]->private || (id == S->NODE_ID))
		{
			if (id == S->NODE_ID)
			{
				latency = X->regions[i]->local_read_latency;
			}
			else
			{
				latency = X->regions[i]->remote_read_latency;
			}

			D		= E->sp[id];
			destbase	= D->MEMBASE;
			offset		= vaddr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					vaddr, id);
				mprint(E, S, nodeinfo,
					"membase 0x" UHLONGFMT ", memsize 0x" UHLONGFMT 
					", destoffset 0x" UHLONGFMT "\n",
					D->MEMBASE, D->MEMSIZE, destoffset);
				sfatal(E, S, "Bad NUMA destination address access!");
			}

			data = (ushort)(D->MEM[destoffset]<<8)|D->MEM[destoffset + 1];
			paddr = D->MEMBASE + destoffset;

			/*										*/
			/*	As a heuristic to filter out uses of an address by the compiler,	*/
			/*	for purposes other than the variable associated with that address	*/
			/*	in the STABS debugging info, if the underlying variable being		*/
			/*	traced is not a pointer, we ignore values that look like code		*/
			/*	or data addresses.							*/
			/*										*/
			if (X->regions[i]->valuetrace && (X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
			{
				/*								*/
				/*	For value tracing, if access is bigger than the		*/
				/*	underlying data, mask off excess.			*/
				/*								*/
				ulong	mask = ~0UL;
				ulong	size = X->regions[i]->endaddr - X->regions[i]->startaddr;
				if (size < 4)
				{
					mask = (1 << (size << 3)) - 1;
				}

				X->regions[i]->values[X->regions[i]->validx] = data & mask;
				if (X->regions[i]->validx < MAX_NUMAREGION_VALUETRACE)
				{
					X->regions[i]->validx++;
				}

				if (X->regions[i]->validx == X->regions[i]->nvalues)
				{
					ulong	*tmp;

					X->regions[i]->nvalues *= 2;
					tmp = (ulong *)mrealloc(E, X->regions[i]->values,
						X->regions[i]->nvalues*sizeof(ulong), 
						"realloc C->regions[i]->values in cache.c");
					if (tmp == NULL)
					{
						mprint(E, NULL, siminfo,
							"Resizing X->regions[i]->values to %d entries failed\n",
							X->regions[i]->nvalues);
						sfatal(E, S, "realloc failed for X->regions[i]->values in cache.c");
					}
					X->regions[i]->values = tmp;
				}
			}

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
				S->superH->B->data_bus = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
				S->superH->B->paddr_bus = paddr;
			}

			S->stallaction(E, S, paddr, MEM_READ_STALL, latency);

			return data;
		}
	}

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
superHreadlong(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase;
	TransAddr	trans;
	ulong		paddr;
	ulong		data = 0;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->superH->B->vaddr_bus, vaddr);
		S->superH->B->vaddr_bus = vaddr;
	}

	/*								*/
	/*	   	Check numa/mapped regions (RAM only)		*/
	/*	Final effect is sort of like addressing a remote 	*/
	/*	mem device via a chip select, minus the chip select.	*/
	/*	Also equivalent to an MMU w/ regard to local accesses.	*/
	/*								*/
	/*	BUG/TODO: When we first enter the function and are 	*/
	/*	executing prologue, we don't want the tracking to be 	*/
	/*	triggered...						*/
	/*								*/
	i = m_find_numastack(S->PCSTACK[S->pcstackheight], S->superH->R[14],
		vaddr, S->Nstack, 0, S->Nstack->count);

	/*								*/
	/*	If address doesn't match a registered address using	*/
	/*	the current frame pointer, it might be that we are	*/
	/*	accessing an address on one of our parent's stacks.	*/
	/*	Unwind the stack (side-effect-free) and try to match.	*/
	/*								*/
	j = S->fpstackheight - 1;
	while (i < 0 && j >= 0)
	{
		i = m_find_numastack(S->PCSTACK[j], S->FPSTACK[j],
				vaddr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(vaddr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
mprint(E,NULL,siminfo,"map_offset: " UHLONGFMT "\n",X->regions[i]->map_offset);
		X->regions[i]->nreads++;

		id = X->regions[i]->map_id;
		if (id >= E->nnodes || id < 0) 
		{
			mprint(E, S, nodeinfo, "\nMap: invalid dst node, ID = %d\n", id);
			sfatal(E, S, "Bad NUMA destination map ID!");
		}

		if (!X->regions[i]->private || (id == S->NODE_ID))
		{
			if (id == S->NODE_ID)
			{
				latency = X->regions[i]->local_read_latency;
			}
			else
			{
				latency = X->regions[i]->remote_read_latency;
			}

			D		= E->sp[id];
			destbase	= D->MEMBASE;
			offset		= vaddr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					vaddr, id);
				mprint(E, S, nodeinfo,
					"membase 0x" UHLONGFMT ", memsize 0x" UHLONGFMT 
					", destoffset 0x" UHLONGFMT "\n",
					D->MEMBASE, D->MEMSIZE, destoffset);
				sfatal(E, S, "Bad NUMA destination address access!");
			}

			data = (ulong)(D->MEM[destoffset]<<24)	|
				(D->MEM[destoffset+1]<<16)	|
				(D->MEM[destoffset+2]<<8)	|
				D->MEM[destoffset+3];
			paddr = D->MEMBASE + destoffset;

			/*										*/
			/*	As a heuristic to filter out uses of an address by the compiler,	*/
			/*	for purposes other than the variable associated with that address	*/
			/*	in the STABS debugging info, if the underlying variable being		*/
			/*	traced is not a pointer, we ignore values that look like code		*/
			/*	or data addresses.							*/
			/*										*/
			if (X->regions[i]->valuetrace && (X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
			{
				/*								*/
				/*	For value tracing, if access is bigger than the		*/
				/*	underlying data, mask off excess.			*/
				/*								*/
				ulong	mask = ~0UL;
				ulong	size = X->regions[i]->endaddr - X->regions[i]->startaddr;
				if (size < 4)
				{
					mask = (1 << (size << 3)) - 1;
				}

				X->regions[i]->values[X->regions[i]->validx] = data & mask;
				if (X->regions[i]->validx < MAX_NUMAREGION_VALUETRACE)
				{
					X->regions[i]->validx++;
				}

				if (X->regions[i]->validx == X->regions[i]->nvalues)
				{
					ulong	*tmp;

					X->regions[i]->nvalues *= 2;
					tmp = (ulong *)mrealloc(E, X->regions[i]->values,
						X->regions[i]->nvalues*sizeof(ulong),
						"realloc C->regions[i]->values in cache.c");
					if (tmp == NULL)
					{
						mprint(E, NULL, siminfo,
							"Resizing X->regions[i]->values to %d entries failed\n",
							X->regions[i]->nvalues);
						sfatal(E, S, "realloc failed for X->regions[i]->values in cache.c");
					}
					X->regions[i]->values = tmp;
				}
			}

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->data_bus, data);
				S->superH->B->data_bus = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->superH->B->paddr_bus, paddr);
				S->superH->B->paddr_bus = paddr;
			}

			S->stallaction(E, S, paddr, MEM_READ_STALL, latency);

			return data;
		}
	}

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

void
superHcache_printstats(Engine *E, State *S)
{
	uvlong	tot;

	if (!S->superH->cache_activated)
	{
		mprint(E, S, nodeinfo, "Cache is not activated\n");
		return;
	}

	tot = S->superH->C->readhit+S->superH->C->readmiss+S->superH->C->writehit+S->superH->C->writemiss;
	if (tot > 0)
	{
		mprint(E, S, nodeinfo, "\nMemory accesses : " UVLONGFMT "\n", tot);

		mprint(E, S, nodeinfo, "Read Hits 	: " UVLONGFMT " (%.2f%%)\n",
			S->superH->C->readhit,
			100*(float)S->superH->C->readhit/(float)tot);
		mprint(E, S, nodeinfo, "Read Misses 	: " UVLONGFMT " (%.2f%%)\n",
			S->superH->C->readmiss,
			100*(float)S->superH->C->readmiss/(float)tot);
		mprint(E, S, nodeinfo, "Write Hits 	: " UVLONGFMT " (%.2f%%)\n",
			S->superH->C->writehit,
			100*(float)S->superH->C->writehit/(float)tot);
		mprint(E, S, nodeinfo, "Write Misses 	: " UVLONGFMT " (%.2f%%)\n\n",
			S->superH->C->writemiss,
			100*(float)S->superH->C->writemiss/(float)tot);
	}
		
	return;
}
