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
	MSP430_DEFLT_MEMSIZE			= 256,
	MSP430_MEMBASE				= 0x0200,
	MSP430_FLASHBASE			= 0xFFFF - 8192,
	MSP430_DEFAULT_MEMREAD_LATENCY		= 1,
	MSP430_DEFAULT_MEMWRITE_LATENCY		= 1,
	MSP430_NPINS				= 256,
};

typedef struct
{
	ushort		mab16;
	ushort		mdb16;

	uchar		mab8;
	uchar		mdb8;

	/*					*/
	/*	When we use Numa facilities to	*/
	/*	translate addr, we count those	*/
	/*	separately.			*/
	/*					*/
	ulong		paddr_bus;
} MSP430Buses;

typedef struct MSP430State MSP430State;
struct MSP430State
{
	ushort 		R		[16];
	ushort		periph16	[(MSP430_RAM_BEGIN - MSP430_PERIPH16_BEGIN)/2];
	uchar		periph8		[MSP430_PERIPH16_BEGIN - MSP430_PERIPH8_BEGIN];
	uchar		sfr		[MSP430_PERIPH8_BEGIN - MSP430_SFR_BEGIN];		
	double		pins		[MSP430_NPINS];

	ushort		me1;
	ushort		me2;
	ushort		ie1;
	ushort		ie2;
	ushort		ifg1;
	ushort		ifg2;

	MSP430Pipe	P;
	MSP430Buses	*B;

	double		vdd;
};

/*		Entries in the Decode Cache		*/
typedef struct
{
	MSP430Pipestage	dc_p;
} MSP430DCEntry;

#define	MSP430_DEFAULT_VDD			(3.3)
#define	MSP430_DEFAULT_CYCLETIME		((double)1/8000000)
