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

/*									*/
/*		From Figure 3.2 of rej09b0146_sh7706.pdf		*/
/*									*/
enum
{
	MMU_AREA_U0_START	= 0x00000000,
	MMU_AREA_U0_END		= 0x80000000,

	MMU_AREA_P0_START	= 0x00000000,
	MMU_AREA_P1_START	= 0x80000000,
	MMU_AREA_P2_START	= 0xA0000000,
	MMU_AREA_P3_START	= 0xC0000000,
	MMU_AREA_P4_START	= 0xE0000000,
	MMU_AREA_P4_END		= 0xFFFFFFFF,
};

typedef enum
{
	MMU_AREA_U0,

	MMU_AREA_P0,
	MMU_AREA_P1,
	MMU_AREA_P2,
	MMU_AREA_P3,
	MMU_AREA_P4,

	MMU_AREA_INVALID,
} MMUarea;


/*									*/
/*	From Section 3.2 of rej09b0146_sh7706.pdf. Also, Fig 3.8	*/
/*									*/
#define mmucr_field_at(r)			((r) & B0001)
#define mmucr_field_ix(r)			(((r) >> 1) & B0001)
#define mmucr_field_tf(r)			(((r) >> 2) & B0001)
#define mmucr_field_rc(r)			(((r) >> 4) & B0011)
#define mmucr_field_sv(r)			(((r) >> 8) & B0001)

#define pteh_field_vpn31_17(r)			(((r) >> 17) & ((1 << 15) - 1))
#define pteh_field_vpn11_10(r)			(((r) >> 10) & B0011)
#define pteh_field_index(r)			(((r) >> 12) & B00011111)
#define pteh_field_asid(r)			((r) & B11111111)

#define ptel_field_sh(r)			(((r) >> 1) & B0001)
#define ptel_field_d(r)				(((r) >> 2) & B0001)
#define ptel_field_c(r)				(((r) >> 3) & B0001)
#define ptel_field_sz(r)			(((r) >> 4) & B0001)
#define ptel_field_pr(r)			(((r) >> 5) & B0011)
#define ptel_field_v(r)				(((r) >> 8) & B0001)
#define ptel_field_ppn(r)			(((r) >> 10) & ((1 << 22) - 1))


/*									*/
/*		From Figure 3.4 of rej09b0146_sh7706.pdf		*/
/*									*/
#define	vaddr1k_field_vpn(a)			(((a) >> 10) & ((1 << 22) - 1))
#define	vaddr1k_field_offset(a)			((a) & B0000001111111111)
#define	vaddr4k_field_vpn(a)			(((a) >> 12) & ((1 << 20) - 1))
#define	vaddr4k_field_offset(a)			((a) & B0000111111111111)

/*
WTF was I thinking ?
#define	tlbentry_datafield_sh(e)		((e) & B0001)
#define	tlbentry_datafield_d(e)			(((e) >> 1) & B0001)
#define	tlbentry_datafield_c(e)			(((e) >> 2) & B0001)
#define	tlbentry_datafield_sz(e)		(((e) >> 3) & B0001)
#define	tlbentry_datafield_pr(e)		(((e) >> 4) & B0011)
#define	tlbentry_datafield_ppn(e)		(((e) >> 6) & ((1 << 22) - 1))

#define	tlbentry_addrfield_v(e)			((e) & B0001)
#define	tlbentry_addrfield_asid(e)		(((e) >> 1) & B11111111)
#define	tlbentry_addrfield_vpn11_10(e)		(((e) >> 9) & B0011)
#define	tlbentry_addrfield_vpn31_17(e)		(((e) >> 11) & ((1 << 15) - 1))
*/

//TODO: this cant be right. see figure 3.13
#define	tlbentry_data_pack(ppn,pr,sz,c,d,sh)	(((ppn) << 6) | ((pr) << 4) |\
						((sz) << 3) | ((c) << 2) |\
						((d) << 1) | (sh))

#define	tlbentry_addr_pack(vpn31_17,vpn11_10,asid,v)	(((vpn31_17) << 11) |\
						((vpn11_10) << 9) | ((asid) << 1) | (v))


#define	vaddr_field_vpn11_10(e)			(((e) >> 10) & B0011)
#define	vaddr_field_vpn31_17(e)			(((e) >> 17) & ((1 << 15) - 1))


/*									*/
/*	From Figure 3.13 for TLB address and data array memory 		*/
/*	mapped access. Some of the macros are very similar to		*/
/*	other previously defined macros, but using those would		*/
/*	obfuscate.							*/
/*									*/
#define	tlbaddrarray_addrfield_index(a)		(((a) >> 12) & B00011111)
#define	tlbaddrarray_addrfield_way(a)		(((a) >> 8) & B0011)
#define	tlbaddrarray_datafield_vpn31_17(d)	(((d) >> 17) & ((1 << 15) - 1))
#define	tlbaddrarray_datafield_vpn11_10(d)	(((d) >> 10) & B0011)
#define	tlbaddrarray_datafield_v(d)		(((d) >> 8) & B0001)
#define	tlbaddrarray_datafield_asid(d)		((d) & B11111111)

#define	tlbdataarray_addrfield_index(a)		(((a) >> 12) & B00011111)
#define	tlbdataarray_addrfield_way(a)		(((a) >> 8) & B0011)
#define	tlbdataarray_datafield_ppn(d)		(((d) >> 10) & ((1 << 19) - 1))
#define	tlbdataarray_datafield_v(d)		(((d) >> 8) & B0001)
#define	tlbdataarray_datafield_pr(d)		(((d) >> 5) & B0011)
#define	tlbdataarray_datafield_sz(d)		(((d) >> 4) & B0001)
#define	tlbdataarray_datafield_c(d)		(((d) >> 3) & B0001)
#define	tlbdataarray_datafield_d(d)		(((d) >> 2) & B0001)
#define	tlbdataarray_datafield_sh(d)		(((d) >> 1) & B0001)
