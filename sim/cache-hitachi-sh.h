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

typedef struct
{
	/*	   See HW man section 5		*/
	int		valid;
	ulong 		tag;

	/*					*/
	/*	We only use data field when	*/
	/*	using CacheBlock in TLB model.	*/
	/*	In modeling cache, we get data	*/
	/*	right from memory when there's	*/
	/*	a hit.				*/
	/*					*/
	ulong		data;

	/*	This should not really be here.	*/
	ushort		timestamp;
} CacheBlock;


typedef struct
{
	int 		blocksize;
	int		assoc;
	int		size;
	int		nblocks;
	int		nsets;
	int		offsetbits;
	int		indexbits;
	int		tagbits;
	CacheBlock 	*blocks;

	uvlong		readmiss;
	uvlong		readhit;
	uvlong		writemiss;
	uvlong		writehit;
} Cache;

