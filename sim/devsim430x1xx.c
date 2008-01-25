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

tuck void
msp430writebyte(Engine *E, State *S, ushort addr, uchar data)
{
	int		latency = 0;
	int		i, j, id;
	ulong		offset, destbase, destoffset, paddr = addr;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->msp430->B->mab16, addr);
		S->msp430->B->mab16 = addr;
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
		addr, S->Nstack, 0, S->Nstack->count);

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
				addr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(addr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
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
		if (X->regions[i]->valuetrace &&
			(X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
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
				tmp = (ulong *)mrealloc(X->regions[i]->values,
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
			offset		= addr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					addr, id);
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
				S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
				S->msp430->B->mdb16 = data;
			}

			S->stallaction(S, paddr, MEM_WRITE_STALL, latency);

			return;
		}
	}

	/*								*/
	/*	Note that we've purposefully allowed RAM and FLASH	*/
	/*	to overlap, so inram and inflash are not mutually	*/
	/*	exclusive.						*/
	/*								*/
	if ((paddr < S->MEMBASE) || (paddr >= S->MEMEND))
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		S->devwritebyte(S, addr, data);

		return;
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
		S->msp430->B->mdb16 = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
		S->msp430->B->paddr_bus = paddr;
	}

	S->MEM[paddr - S->MEMBASE] = data;
	S->stallaction(S, paddr, MEM_WRITE_STALL, latency);


	return;
}

 
tuck void 	
msp430writeword(Engine *E, State *S, ushort addr, ushort data)
{
	int		latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase, paddr = addr;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->msp430->B->mab16, addr);
		S->msp430->B->mab16 = addr;
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
		addr, S->Nstack, 0, S->Nstack->count);

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
				addr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(addr, S->N, 0, S->N->count);
		if (i >= 0)
		{
			X = S->N;
		}
	}

	if (i != -1)
	{
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
		if (X->regions[i]->valuetrace &&
			(X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
		{
			/*								*/
			/*	For value tracing, if access is bigger than the		*/
			/*	underlying data, mask off excess.			*/
			/*								*/
			ulong	mask = ~0UL;
			int	size = X->regions[i]->endaddr - X->regions[i]->startaddr;
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
				tmp = (ulong *)mrealloc(X->regions[i]->values,
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
			offset		= addr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					addr, id);
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
				S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
				S->msp430->B->mdb16 = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
				S->msp430->B->paddr_bus = paddr;
			}

			S->stallaction(S, paddr, MEM_WRITE_STALL, latency);

			return;
		}
	}


	/*								*/
	/*	Note that we've purposefully allowed RAM and FLASH	*/
	/*	to overlap, so inram and inflash are not mutually	*/
	/*	exclusive.						*/
	/*								*/
	if ((paddr < S->MEMBASE) || (paddr >= S->MEMEND-1))
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		S->devwriteword(S, addr, data);

		return;
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
		S->msp430->B->mdb16 = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
		S->msp430->B->paddr_bus = paddr;
	}

	S->MEM[paddr - S->MEMBASE] = (uchar)((data>>8)&0xFF);
	S->MEM[paddr+1 - S->MEMBASE] = (uchar)data&0xFF;
	S->stallaction(S, paddr, MEM_WRITE_STALL, latency);

	return;
}

/*									*/
/*	Reads data of each length from address Addr. An address error 	*/
/*	will occur if word data is read from an address other than 2n	*/
/*	or if longword data is read from an address other than 4n	*/
/*									*/
tuck uchar 	
msp430readbyte(Engine *E, State *S, ushort addr)
{
	int		latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase, paddr = addr;
	uchar		data = 0;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->msp430->B->mab16, addr);
		S->msp430->B->mab16 = addr;
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
		addr, S->Nstack, 0, S->Nstack->count);

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
				addr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(addr, S->N, 0, S->N->count);
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
			offset		= addr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					addr, id);
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
			if (X->regions[i]->valuetrace &&
				(X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
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
					tmp = (ulong *)mrealloc(X->regions[i]->values,
						X->regions[i]->nvalues*sizeof(ulong), 
						"realloc C->regions[i]->values in cache.c");
					if (tmp == NULL)
					{
						mprint(E, NULL, siminfo,
							"Resizing X->regions[i]->values to %d entries failed\n",
							X->regions[i]->nvalues);
						sfatal(E, S,
							"realloc failed for X->regions[i]->values in cache.c");
					}
					X->regions[i]->values = tmp;
				}
			}

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
				S->msp430->B->mdb16 = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
				S->msp430->B->paddr_bus = paddr;
			}

			S->stallaction(S, paddr, MEM_READ_STALL, latency);

			return data;
		}
	}


	if ((paddr < S->MEMBASE) || (paddr >= S->MEMEND))
	{
		latency = S->mem_r_latency;
		data = S->MEM[paddr - S->MEMBASE];
	}
	else
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return S->devreadbyte(S, addr);
	}
	
	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
		S->msp430->B->mdb16 = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
		S->msp430->B->paddr_bus = paddr;
	}
	
	S->stallaction(S, paddr, MEM_READ_STALL, latency);


	return data;
}

tuck ushort
msp430readword(Engine *E, State *S, ushort addr)
{
	int		latency = 0;
	int		i, id, j;
	ulong		offset, destoffset = 0, destbase, paddr = addr;
	ushort		data = 0;
	State		*D;
	Numa		*X = NULL;


	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	Virtual Address Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->mab16, addr);
		S->msp430->B->mab16 = addr;
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
		addr, S->Nstack, 0, S->Nstack->count);

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
				addr, S->Nstack, 0, S->Nstack->count);
		j--;
	}

	if (i >= 0)
	{
		X = S->Nstack;
	}
	else
	{
		i = m_find_numa(addr, S->N, 0, S->N->count);
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
			offset		= addr - S->MEMBASE;
			destoffset	= offset + X->regions[i]->map_offset;

			if (destoffset >= D->MEMSIZE)
			{
				mprint(E, S, nodeinfo,
					"\nMap: Access @ addr 0x" UHLONGFMT ", dst node ID %d\n",
					addr, id);
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
			if (X->regions[i]->valuetrace &&
				(X->regions[i]->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
			{
				/*								*/
				/*	For value tracing, if access is bigger than the		*/
				/*	underlying data, mask off excess.			*/
				/*								*/
				ulong	mask = ~0UL;
				int	size = X->regions[i]->endaddr - X->regions[i]->startaddr;
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
					tmp = (ulong *)mrealloc(X->regions[i]->values,
						X->regions[i]->nvalues*sizeof(ulong), 
						"realloc C->regions[i]->values in cache.c");
					if (tmp == NULL)
					{
						mprint(E, NULL, siminfo,
							"Resizing X->regions[i]->values to %d entries failed\n",
							X->regions[i]->nvalues);
						sfatal(E, S,
							"realloc failed for X->regions[i]->values in cache.c");
					}
					X->regions[i]->values = tmp;
				}
			}

			if (SF_BITFLIP_ANALYSIS)
			{
				/*	        Data Bus	*/
				S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
				S->msp430->B->mdb16 = data;

				/*	Physical Address Bus	*/
				S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
				S->msp430->B->paddr_bus = paddr;
			}

			S->stallaction(S, paddr, MEM_READ_STALL, latency);

			return data;
		}
	}


	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 1)) /* -1 for second byte */
	{
		latency = S->mem_r_latency;
		data = (ushort)(S->MEM[paddr - S->MEMBASE]<<8)|S->MEM[paddr+1 - S->MEMBASE];
	}
	else
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return S->devreadword(S, addr);
	}

	/*		Model # bits flipping due to this mem access	*/
	if (SF_BITFLIP_ANALYSIS)
	{
		/*	        Data Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->mdb16, data);
		S->msp430->B->mdb16 = data;

		/*	Physical Address Bus	*/
		S->Cycletrans += bit_flips_32(S->msp430->B->paddr_bus, paddr);
		S->msp430->B->paddr_bus = paddr;
	}

	S->stallaction(S, paddr, MEM_READ_STALL, latency);

	return data;
}
