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
	/*   	Max num of chars in a deassembled instruction.		*/
	INSTRBUFSIZE = 128,
		
	SUPERH_MEMBASE			= 0x08000000,
	SUPERH_FLASHBASE		= 0x00000000,
	SUPERH_BYTES_PER_POINTER 	= 4,
	SUPERH_MEMADDRBITS		= 32,

	/*   	Max num of chars in an opcode name eg ADD = 3		*/
	MAX_OPCHARS = 16,
	DEFAULT_MISS_PENALTY = 100,
};

/*							*/
/*	Used to maintain last value driven on buses	*/
/*	It only makes sense for the phy addr/data	*/
/*	buses to have a lock. All other buses are	*/
/*	internal to the "CPU core", and are private	*/
/*	even when you have a multi-core config.		*/
/*	    (Leastways, thats our implementation)	*/
/*							*/
typedef struct
{
	ulong		paddr_bus;
	ulong		vaddr_bus;
	ulong		data_bus;
	ushort		peraddr_bus;
	ushort		perdata_bus;

	uchar		pbuslock;
	int		pbuslocker;
	int		pbuslock_type;
	ulong		pbuslock_addr;
} SuperHBuses;

/*		Entries in the Decode Cache		*/
typedef struct
{
	SuperHPipestage	dc_p;
} SuperHDCEntry;

typedef struct SuperHState SuperHState;
struct SuperHState
{



	int		flash_r_latency;
	int		flash_w_latency;

	uchar		*FLASH;

	int		FLASHSIZE;
	int		FLASHBASE;
	int		FLASHEND;

	Cache		*C;
	int		cache_activated;


	int		ENABLE_CLK_INTR;


	SuperHBuses	*B;


	/*			Exception queue			*/
	InterruptQ	*excpQ;

	/*								*/
	/*	Whether intrs get generated for RX/TX events/errors	*/
	/*	We use individual vars rather than bits in a register	*/
	/*	since its easier to maintain: c.f. the NIC_NSR: it's	*/
	/*	often hard in that case to remember quickly which bit	*/
	/*	does what, and is hence error prone.			*/
	/*								*/
	int		rxok_intrenable_flag;
	int		txok_intrenable_flag;
	int		addrerr_intrenable_flag;
	int		frameerr_intrenable_flag;
	int		collserr_intrenable_flag;
	int		csenseerr_intrenable_flag;
	int		rxovrrunerr_intrenable_flag;
	int		txovrrunerr_intrenable_flag;
	int		rxundrrunerr_intrenable_flag;
	int		txundrrunerr_intrenable_flag;
	int		csumerr_intrenable_flag;



	/*			Generic Network interface		*/
	Ifc		NIC_IFCS[NIC_MAX_IFCS];
	int		NIC_NUM_IFCS;
	int		NIC_INTR;

	/*			NIC interrupt queue			*/
	InterruptQ	*nicintrQ;

	/*	Time of last generated timer intr	*/
	Picosec		TIMER_LASTACTIVATE;

	/*		Delay b/n timer interrupts	*/
	Picosec		TIMER_INTR_DELAY;

	ulong 		R[16];
	ulong		R_BANK[8];
	ulong 		MACH;
	ulong		MACL;
	SuperHSREG	SR, SSR;
	ulong		PR;
	ulong		SPC;
	ulong 		GBR;
	ulong		VBR;

	SuperHPipe	P;
	int		opncycles[SUPERH_OP_MAX];

	/*		Power Adaptation Unit		*/
	PAUentry	*PAUs;
	int		npau;
	int		influenced;
	int		controlling_pau;
	ulong		pauaddrmask;

	Cache		*TLB;

	int		*PAUvalids;
	int		numpauvalids;


	/*	Whether mem access is in IF or EX	*/
	int		mem_access_type;


	/*	    Exception and trap mmap regs	*/
	ulong		TRA;
	ulong		EXPEVT;
	ulong		INTEVT;


	/*		MMU stuff			*/
	ulong		PTEH;
	ulong		PTEL;
	ulong		TTB;
	ulong		TEA;
	ulong		MMUCR;


	/*		Interrupt Controller		*/
	ulong		ICR;
	ulong		ICRA;
	ulong		ICRB;

};

#define superH_check_nic_intr_macro(S)			\
		(((S)->superH->SR.BL == 0) &&		\
		((S)->superH->nicintrQ->nqintrs > 0) &&		\
		((S)->superH->SR.IMASK < NIC_FIXED_INTRLEVEL))

#define superH_check_excp_macro(S)			\
		(((S)->superH->SR.BL == 0) &&		\
		((S)->superH->excpQ->nqintrs > 0))
