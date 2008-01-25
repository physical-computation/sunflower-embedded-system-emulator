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

#include <math.h>
#include "mmu-hitachi-sh.h"
#include "sf.h"
#include "mextern.h"

static void	superHtlbexception(Engine *, State *S, int code, ulong vaddr);
static void	prcheckpriv(Engine *, State *S, int wflag, ulong data, ulong vaddr, TransAddr *tr);
static void	prcheckuser(Engine *, State *S, int wflag, ulong data, ulong vaddr, TransAddr *tr);


int
superHtlb_init(Engine *E, State *S, int size, int blocksize, int assoc)
{
	if (S->superH->TLB != NULL)
	{
		mfree(E, S->superH->TLB, "(Cache *)S->superH->TLB");
	}

	S->superH->TLB = (Cache *)mcalloc(E, 1, sizeof(Cache),
				"(Cache *)S->superH->TLB");
	if (S->superH->TLB == NULL)
	{
		merror(E, "mcalloc failed for (Cache *)S->superH->TLB in cache_init()");
		return -1;
	}

	if ((blocksize > size)||(size <= 0)||(assoc <= 0)||\
		(blocksize <= 0)||(size%blocksize != 0)||\
		((size/blocksize)%assoc != 0))
	{
		merror(E, "superHtlb_init() failed: Invalid TLB parameters");
		return -1;
	}

	S->superH->TLB->blocksize = blocksize;
	S->superH->TLB->assoc = assoc;
	S->superH->TLB->size = size;
	S->superH->TLB->nblocks = size/blocksize;

	S->superH->TLB->blocks = (CacheBlock *)mcalloc(E, S->superH->TLB->nblocks,
			sizeof(CacheBlock), "(CacheBlock *)S->superH->TLB->blocks");
	if (S->superH->TLB->blocks == NULL)
	{
		merror(E, "mcalloc failed for S->superH->TLB->blocks in superHtlb_init()");
		return -1;
	}

	S->superH->TLB->nsets = S->superH->TLB->nblocks/S->superH->TLB->assoc;
	S->superH->TLB->offsetbits = (int)ceil(log10(S->superH->TLB->blocksize)/log10(2));
	S->superH->TLB->indexbits = (int)ceil(log10(S->superH->TLB->nsets)/log10(2));
	S->superH->TLB->tagbits = SUPERH_MEMADDRBITS -
				(S->superH->TLB->offsetbits+S->superH->TLB->indexbits);

/*
	mprint(E, S, nodeinfo, "TLB Parameters:\n");
	mprint(E, S, nodeinfo, "\t\tNumber of entries: %d\n",
		S->superH->TLB->nblocks);
	mprint(E, S, nodeinfo, "\t\tAssociativity: %d-way set associative\n",
		S->superH->TLB->assoc);
	mprint(E, S, nodeinfo, "\t\tNumber of Sets: %d\n",
		S->superH->TLB->nsets);
*/

	return 0;
}


void
superHvmtranslate(Engine *E, State *S, int op, TransAddr *tr)
{
	int		area = MMU_AREA_INVALID, i, start, end, hflag;
	ulong		vaddr = tr->vaddr, index;
	ulong		vaddr31_17, vaddr11_10, pteh_asid, mmucr_sv;



	/*									*/
	/*	First, for MMU_AREA_P4 (priv, ctrl space), no translation	*/
	/*									*/
	if ((vaddr >= MMU_AREA_P4_START) && (vaddr <= MMU_AREA_P4_END))
	{
		tr->error = 0;
		tr->paddr = vaddr;
		tr->cacheable = 0;
//fprintf(stderr, "MMU_AREA_P4. Address [0x" UHLONGFMT "] translated to [0x" UHLONGFMT "]...\n", tr->vaddr, tr->paddr);

		return;
	}

	/*								*/
	/*	If MMU is disabled, virtual addr is used directly	*/
	/*	as phy addr by ignoring top 3 bits of virtual addr	*/
	/*	See rej09b0146_sh7706.pdf, page 55, para 4.		*/
	/*								*/
	if (mmucr_field_at(S->superH->MMUCR) == 0)
	{
		tr->error = 0;
		tr->paddr = vaddr & ~(B0111 << 29);
		tr->cacheable = 1;

		return;
	}

//%fprintf(stderr, "MMU enabled. Translating address [0x" UHLONGFMT "]...\n", vaddr);


	if (S->superH->SR.MD == 0)
	{
		if ((vaddr >=  MMU_AREA_U0_START) && (vaddr < MMU_AREA_U0_END))
		{
			area = MMU_AREA_U0;
		}
		else
		{
			if (op == MEM_READ_BYTE)
			{
				superHtlbexception(E, S, CPU_LOAD_ADDRERR_EXCP, vaddr);
			}
			else
			{
				superHtlbexception(E, S, CPU_STORE_ADDRERR_EXCP, vaddr);
			}
			tr->error = 1;
mprint(E, NULL, siminfo, "Load/store address error for address [0x" UHLONGFMT "]...\n", vaddr);
sfatal(E, S, "stopping on tlb exception...\n");

			return;
		}
	}
	else
	{
		if ((vaddr >= MMU_AREA_P0_START) && (vaddr < MMU_AREA_P1_START))
		{
			area = MMU_AREA_P0;
		}
		else if ((vaddr >= MMU_AREA_P1_START) && (vaddr < MMU_AREA_P2_START))
		{
			area = MMU_AREA_P1;
		}
		else if ((vaddr >= MMU_AREA_P2_START) && (vaddr < MMU_AREA_P3_START))
		{
			area = MMU_AREA_P2;
		}
		else if ((vaddr >= MMU_AREA_P3_START) && (vaddr < MMU_AREA_P4_START))
		{
			area = MMU_AREA_P3;
		}
	}

	
	/*									*/
	/*	For MMU_AREA_P1 (recall: priv), translation is simply		*/
	/*	setting the virtual address MSB to 0				*/
	/*									*/
	if (area == MMU_AREA_P1)
	{
		tr->error = 0;
		tr->paddr = vaddr & ~(1 << 31);
		tr->cacheable = 1;
//fprintf(stderr, "MMU_AREA_P1. Address translated to [0x" UHLONGFMT "]...\n", tr->paddr);

		return;
	}

	/*									*/
	/*	For MMU_AREA_P2 (recall: priv), translation is simply		*/
	/*	setting the virtual address 3 MSBs to 0				*/
	/*									*/
	if (area == MMU_AREA_P2)
	{
		tr->error = 0;
		tr->paddr = vaddr & ~(B0111 << 29);
		tr->cacheable = 0;
//fprintf(stderr, "MMU_AREA_P2. Address translated to [0x" UHLONGFMT "]...\n", tr->paddr);

		return;
	}



	/*									*/
	/*	Address is not in P1 or P2, so it must be translated.		*/
	/*	Do a TLB lookup.						*/
	/*									*/

	/*									*/
	/*	Extract index. Scheme used depends on MMUCR "ix" field.		*/
	/*	See rej09b0146_sh7706.pdf, Figure 3.6.				*/
	/*									*/
	if (mmucr_field_ix(S->superH->MMUCR) == 0)
	{
		index = (vaddr >> 12) & B00011111;
	}
	else
	{
		index = ((vaddr >> 12) & B00011111) ^ (pteh_field_asid(S->superH->PTEH));
	}

	vaddr31_17 = vaddr_field_vpn31_17(vaddr);
	vaddr11_10 = vaddr_field_vpn11_10(vaddr);
	pteh_asid = pteh_field_asid(S->superH->PTEH);
	mmucr_sv = mmucr_field_sv(S->superH->MMUCR);


	start = index * S->superH->TLB->assoc;
	end = (index + 1) * S->superH->TLB->assoc;
	hflag = 0;


	/*
		TODO: check whether this obeys Figure 3.10
	*/
	for (i = start; i < end; i++)
	{
		ulong data		= S->superH->TLB->blocks[i].data;
		ulong tag		= S->superH->TLB->blocks[i].tag;
		ulong tlb_asid		= tlbaddrarray_datafield_asid(tag);
		ulong tlb_vpn31_17	= tlbaddrarray_datafield_vpn31_17(tag);
		ulong tlb_vpn11_10	= tlbaddrarray_datafield_vpn11_10(tag);
		ulong tlb_sz		= tlbdataarray_datafield_sz(data);
		ulong tlb_sh		= tlbdataarray_datafield_sh(data);


		/*								*/
		/*	Search through Set for a match on VPN.  The match	*/
		/*	depends on page size ('sz' field of TLB entry) and	*/
		/*	single/multiple virtual memory choice ('sh' field of 	*/
		/*	TLB entry). See flowchart, Figure 3.7 of rej09b0146	*/
		/*								*/
		if (tlb_sh || (S->superH->SR.MD && mmucr_sv))
		{
			if (tlb_sz)
			{/* 4k pages */
				if (vaddr31_17 == tlb_vpn31_17)
				{
					hflag = 1;
					break;
				}
			}
			else
			{/* 1k pages */
				if ((vaddr31_17 == tlb_vpn31_17) &&
					(vaddr11_10 == tlb_vpn11_10))
				{
					hflag = 1;
					break;
				}
			}
		}
		else
		{
			if (tlb_sz)
			{/* 4k pages */
				if ((vaddr31_17 == tlb_vpn31_17) &&
					(pteh_asid == tlb_asid))
				{
					hflag = 1;
					break;
				}
			}
			else
			{/* 1k pages */
				if ((vaddr31_17 == tlb_vpn31_17) &&
					(vaddr11_10 == tlb_vpn11_10) &&
					(pteh_asid == tlb_asid))
				{
					hflag = 1;
					break;
				}
			}
		}
	}

	if (hflag) /* Address Hit */
	{
		ulong	data = S->superH->TLB->blocks[i].data;
		/*	TODO: these two vars should always be coherent. Ensure this	*/
		ulong	tlb_v = tlbaddrarray_datafield_v(data) | tlbdataarray_datafield_v(data);
		int	wflag = 0;


		if ((op==MEM_WRITE_BYTE)||(op==MEM_WRITE_WORD)||(op==MEM_WRITE_LONG))
		{
			wflag = 1;
		}

		/*	If hit but entry not valid, TLB invalid exception	*/
		if (!tlb_v)
		{
			if (wflag)
			{
				superHtlbexception(E, S, TLB_STORE_INVALID_EXCP, vaddr);
			}
			else
			{
				superHtlbexception(E, S, TLB_LOAD_INVALID_EXCP, vaddr);
			}
			tr->error = 1;
mprint(E, NULL, siminfo, "TLB invalid exception for address [0x" UHLONGFMT "]...\n", vaddr);
sfatal(E, S, "stopping on tlb exception...\n");

			return;
		}

		if (S->superH->SR.MD)
		{
			prcheckpriv(E, S, wflag, data, vaddr, tr);
		}
		else
		{
			prcheckuser(E, S, wflag, data, vaddr, tr);
		}
	}
	else
	{
		if ((op==MEM_READ_BYTE)||(op==MEM_READ_WORD)||(op==MEM_READ_LONG))
		{
			superHtlbexception(E, S, TLB_LOAD_MISS_EXCP, vaddr);
		}
		else
		{
			superHtlbexception(E, S, TLB_STORE_MISS_EXCP, vaddr);
		}
		tr->error = 1;
mprint(E, NULL, siminfo, "MMU_AREA_P0/P3. TLB load/store miss exception for address [0x" UHLONGFMT "]...\n", vaddr);
sfatal(E, S, "stopping on tlb exception...\n");

		return;
	}

	mexit(E, "End of superHvmtranslate reached. Some condition unhandled", -1);
}

void
prcheckuser(Engine *E, State *S, int wflag, ulong data, ulong vaddr, TransAddr *tr)
{
	int	tlb_pr = tlbdataarray_datafield_pr(data);
	ulong	tlb_sz = tlbdataarray_datafield_sz(data);
	ulong	tlb_d = tlbdataarray_datafield_d(data);


	if ((tlb_pr == B0000) || (tlb_pr == B0001))
	{
		if (wflag)
		{
			superHtlbexception(E, S, TLB_STORE_PROTECT_EXCP, vaddr);
		}
		else
		{
			superHtlbexception(E, S, TLB_LOAD_PROTECT_EXCP, vaddr);
		}
		tr->error = 1;
mprint(E, NULL, siminfo, "TLB protect exception...\n");

		return;
	}

	if ((tlb_pr == B0010) && wflag)
	{
		superHtlbexception(E, S, TLB_STORE_PROTECT_EXCP, vaddr);
		tr->error = 1;
mprint(E, NULL, siminfo, "TLB protect exception...\n");

		return;
	}

	if ((tlb_pr == B0011) && wflag && !tlb_d)
	{
		superHtlbexception(E, S, TLB_INIT_PAGEWRITE_EXCP, vaddr);
		tr->error = 1;
mprint(E, NULL, siminfo, "TLB init page write exception...\n");

		return;
	}

	/*	See Figure 3.4		*/
	if (tlb_sz)
	{/* 4K pages */
		/*							*/
		/*	Lower 2 bits of PPN (bits 11 and 10 when 	*/
		/*	looking at PPN as the most significant bits	*/
		/*	of an address, or bits 1 and 0 when looking	*/
		/*	at 22 bit PPN on its own) are not used in 	*/
		/*	4K page case. See Figure 3.4.			*/
		/*							*/
		tr->paddr = ((tlbdataarray_datafield_ppn(data) & ~B0011) << 22) |
				(vaddr & ((1<<12)-1));
	}
	else
	{/* 1K pages */
		tr->paddr = (tlbdataarray_datafield_ppn(data) << 22) |
			(vaddr & ((1<<10)-1));
	}

	tr->error = 0;
	tr->cacheable = tlbdataarray_datafield_c(data);

//fprintf(stderr, "MMU_AREA_P0/P3. Address translated to [0x" UHLONGFMT "]...\n", tr->paddr);

	return;
}

void
prcheckpriv(Engine *E, State *S, int wflag, ulong data, ulong vaddr, TransAddr *tr)
{
	int	tlb_pr = tlbdataarray_datafield_pr(data);
	ulong	tlb_sz = tlbdataarray_datafield_sz(data);
	ulong	tlb_d = tlbdataarray_datafield_d(data);


	if (((tlb_pr == B0000) || (tlb_pr == B0010)) && wflag)
	{
		superHtlbexception(E, S, TLB_STORE_PROTECT_EXCP, vaddr);
		tr->error = 1;
mprint(E, NULL, siminfo, "TLB store protect exception...\n");

		return;
	}


	if (((tlb_pr == B0001) || (tlb_pr == B0011)) && wflag && !tlb_d)
	{
		superHtlbexception(E, S, TLB_INIT_PAGEWRITE_EXCP, vaddr);
		tr->error = 1;
mprint(E, NULL, siminfo, "TLB init page write exception...\n");

		return;
	}


	/*	See Figure 3.4		*/
	if (tlb_sz)
	{/* 4K pages */
		/*							*/
		/*	Lower 2 bits of PPN (bits 11 and 10 when 	*/
		/*	looking at PPN as the most significant bits	*/
		/*	of an address, or bits 1 and 0 when looking	*/
		/*	at 22 bit PPN on its own) are not used in 	*/
		/*	4K page case. See Figure 3.4.			*/
		/*							*/
		tr->paddr = ((tlbdataarray_datafield_ppn(data) & ~B0011) << 22) |
			(vaddr & ((1<<12)-1));
	}
	else
	{/* 1K pages */
		tr->paddr = (tlbdataarray_datafield_ppn(data) << 22) |
			(vaddr & ((1<<10)-1));
	}

	tr->error = 0;
	tr->cacheable = tlbdataarray_datafield_c(data);
//fprintf(stderr, "MMU_AREA_P0/P3. Address translated to [0x" UHLONGFMT "]...\n", tr->paddr);

	return;
}

void
superHtlbexception(Engine *E, State *S, int type, ulong vaddr)
{
	/*								*/
	/*	All TLB exceptions lead to same sequence of events,	*/
	/*	except for TLB miss exceptions which handle updating	*/
	/*	of MMUCR RC field slightly differently.			*/
	/*	See Sec. 3.5 of rej09b0146_sh7706.pdf for (1 -- 10)	*/
	/*								*/


	/*	(1) BUG?	*/
	S->superH->PTEH &= B11111111;
	S->superH->PTEH |= vaddr & (~B0000001111111111);

	/*	(2)	*/
	S->superH->TEA = vaddr;

	/*								*/
	/*	(3 -- 8) Don't need to do these since we queue excp	*/
	/*	with fetchedpc, and when we take exception we'll	*/
	/*	copy fetchedpc into SPC, and set the EXPEVT.		*/
	/*								*/


	/*								*/
	/*	(9) Update MMUCR's RC field. BUG: Fix this soon, its 	*/
	/*	easy to do.						*/
	/*								*/
	if ((type == TLB_LOAD_MISS_EXCP) || (type == TLB_STORE_MISS_EXCP))
	{
	}
	else
	{
	}

	/*	(10)	*/
	if (S->superH->mem_access_type == MEM_ACCESS_IFETCH)
	{
		pic_intr_enqueue(E, S, S->superH->excpQ, type,
			S->PC, S->superH->mem_access_type);
	}
	else
	{
		pic_intr_enqueue(E, S, S->superH->excpQ, type,
			S->superH->P.EX.fetchedpc, S->superH->mem_access_type);
	}
//fprintf(stderr, "S->excpQ->nqintrs = %d\n", S->excpQ->nqintrs);
//superHdumpregs(S);
//superHdumpsysregs(S);
//S->pipeshow = 1;

	return;
}

ulong
superHtlb_addrarray_read(State *S, ulong addr)
{
	int	index, way;


	/*	See Figure 3.13 in  rej09b0146_sh7706.pdf	*/
	index = tlbaddrarray_addrfield_index(addr);
	if (mmucr_field_ix(S->superH->MMUCR) == 1)
	{
		index ^= pteh_field_asid(S->superH->PTEH);
	}

	way = tlbaddrarray_addrfield_way(addr);

	return S->superH->TLB->blocks[index*S->superH->TLB->assoc + way].tag;
}

void
superHtlb_addrarray_write(State *S, ulong addr, ulong data)
{
	int	index, way;


	/*	See Figure 3.13 in  rej09b0146_sh7706.pdf	*/
	index = tlbaddrarray_addrfield_index(addr);
	if (mmucr_field_ix(S->superH->MMUCR) == 1)
	{
		index ^= pteh_field_asid(S->superH->PTEH);
	}

	way = tlbaddrarray_addrfield_way(addr);
	S->superH->TLB->blocks[index*S->superH->TLB->assoc + way].tag = data;

	return;
}

ulong
superHtlb_dataarray_read(State *S, ulong addr)
{
	int	index, way;


	/*	See Figure 3.13 in  rej09b0146_sh7706.pdf	*/
	index = tlbdataarray_addrfield_index(addr);
	if (mmucr_field_ix(S->superH->MMUCR) == 1)
	{
		index ^= pteh_field_asid(S->superH->PTEH);
	}

	way = tlbdataarray_addrfield_way(addr);

	return S->superH->TLB->blocks[index*S->superH->TLB->assoc + way].tag;
}

void
superHtlb_dataarray_write(State *S, ulong addr, ulong data)
{
	int	index, way;


	/*	See Figure 3.13 in  rej09b0146_sh7706.pdf	*/
	index = tlbdataarray_addrfield_index(addr);
	if (mmucr_field_ix(S->superH->MMUCR) == 1)
	{
		index ^= pteh_field_asid(S->superH->PTEH);
	}

	way = tlbdataarray_addrfield_way(addr);
	S->superH->TLB->blocks[index*S->superH->TLB->assoc + way].tag = data;

	return;
}

void
superHdumptlb(Engine *E, State *S)
{
	int	i;

			
	mprint(E, S, nodeinfo, "\n");
	for (i = 0; i < S->superH->TLB->nblocks; i++)
	{
		ulong data		= S->superH->TLB->blocks[i].data;
		ulong tag		= S->superH->TLB->blocks[i].tag;

		if (!tlbaddrarray_datafield_v(tag))
		{
			continue;
		}

		mprint(E, S, nodeinfo, "VPN:0x" UHLONGFMT ",0x" UHLONGFMT " ",
			tlbaddrarray_datafield_vpn31_17(tag),
			tlbaddrarray_datafield_vpn11_10(tag));
		mprint(E, S, nodeinfo, "V:0x" UHLONGFMT " ", tlbaddrarray_datafield_v(tag));
		mprint(E, S, nodeinfo, "ASID:0x" UHLONGFMT "\t", tlbaddrarray_datafield_asid(tag));

		mprint(E, S, nodeinfo, "PPN:0x" UHLONGFMT " ", tlbdataarray_datafield_ppn(data));
		mprint(E, S, nodeinfo, "V:0x" UHLONGFMT " ", tlbdataarray_datafield_v(data));
		mprint(E, S, nodeinfo, "PR:0x" UHLONGFMT " ", tlbdataarray_datafield_pr(data));
		mprint(E, S, nodeinfo, "SZ:0x" UHLONGFMT " ", tlbdataarray_datafield_sz(data));
		mprint(E, S, nodeinfo, "C:0x" UHLONGFMT " ", tlbdataarray_datafield_c(data));
		mprint(E, S, nodeinfo, "D:0x" UHLONGFMT " ", tlbdataarray_datafield_d(data));
		mprint(E, S, nodeinfo, "SH:0x" UHLONGFMT "\n", tlbdataarray_datafield_sh(data));

		if (!((i+1) % S->superH->TLB->assoc))
		{
			mprint(E, S, nodeinfo, "\n");
		}
	}
	mprint(E, S, nodeinfo, "\n");

	return;
}

void
superHtlbflush(Engine *E, State *S)
{
	/*
		TODO: we currently don't do an actual flush
	*/

	mprint(E, NULL, siminfo, "\nSunflower debug: in superHtlbflush\n\n");

	return;
}
