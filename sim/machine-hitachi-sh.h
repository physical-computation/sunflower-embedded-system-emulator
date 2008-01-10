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
enum
{
	/*   	Max num of chars in a deassembled instruction.		*/
	INSTRBUFSIZE = 128,
		
	SUPERH_MEMBASE			= 0x08000000,
	SUPERH_FLASHBASE		= 0x00000000,
	SUPERH_BYTES_PER_POINTER 	= 4,
	SUPERH_MEMADDRBITS		= 32,

	/*   	Max num of chars in an opcode name eg ADD = 3		*/
	MAXOPCHARS = 16,		
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
	double		TIMER_LASTACTIVATE;

	/*		Delay b/n timer interrupts	*/
	double		TIMER_INTR_DELAY;

	ulong 		R[16];
	ulong		R_BANK[8];
	ulong 		MACH;
	ulong		MACL;
	SuperHSREG	SR, SSR;
	ulong		PR;
	ulong		SPC;
	ulong 		GBR;
	ulong		VBR;

	double		VDD;

	/*	The default low operating voltage. 	*/
	double		LOWVDD;			

	/*		Saved VDD. see pau.c		*/
	double		SVDD;			

	SuperHPipe	P;
	int		opncycles[OP_MAX];

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
