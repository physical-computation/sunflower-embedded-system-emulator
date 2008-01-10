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






	/*								*/
	/*	!!!!!! NOTE:  This implementation is out of sync 	*/
	/*	with changes made over the last few years to the	*/
	/*	simulator.						*/
	/*								*/








/*									*/
/*			The Power Adaptation Unit			*/
/*									*/
/*	This is a small structure in the CPU that monitors program	*/
/*	behavior and adjusts the CPU operating frequency and voltage	*/
/*	to minimize average power, hopefully without degrading 		*/
/*	performance.							*/
/*									*/
typedef struct
{
	int	valid;
	int	transient;
	int	active;
	int	init;
	int	device;

	ulong	trigger_pc;
	ulong	stride;

	int	Q;

	/*	dynamic instr cnt for period of stride		*/
	ulong	instrs;
	ulong	dyntmp;

	/*							*/
	/*	Only nclks needs to be uvlong. Others are 	*/
	/*	limited to PAU_STRIDE_MAX			*/
	/*							*/
	uvlong	nclks;
} PAUentry;

enum
{
	PAU_INIT_ADDR_MASK	= B0000001111111111,	/*	Lower 10 bits of address for indexing PAU	*/
	PAU_INIT_NENTRIES	= PAU_INIT_ADDR_MASK+1,	/*	Mask is (2^N)-1, and we are 2^N entries		*/

	PAU_Q_LOW_H2O	= 1,
	PAU_Q_HIGH_H2O	= 2,
	PAU_STRIDE_MAX	= 1 << 24,
	PAU_RFG_CYCLES	= 1024,
	PAU_STRIDE_MIN	= (2)*(1)*(PAU_RFG_CYCLES),
};

enum
{
	PAU_LOG_STEP = (1 << (sizeof(uchar)))/2,
};
