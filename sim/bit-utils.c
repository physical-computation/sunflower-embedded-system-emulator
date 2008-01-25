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
#include <stdio.h>
#include "sf.h"

void
mbitprint(Engine *E, State *S, short nbits, unsigned long num)
{
        int	i = 0;

        for (i = (nbits-1); i >= 0; i--)
        {
		mprint(E, S, nodeinfo, "%c", ((num >> i)&1) + '0');

		if (!(i%4))
		{
			mprint(E, S, nodeinfo, "%c", ' ');
		}
        }

        return;
}

int
bit_flips_32(ulong w1, ulong w2)
{
	ulong tmp;

	tmp = w1^w2;
	tmp = (tmp&0x55555555) + ((tmp&0xaaaaaaaa) >> 1);
	tmp = (tmp&0x33333333) + ((tmp&0xcccccccc) >> 2);
	tmp = (tmp&0x0f0f0f0f) + ((tmp&0xf0f0f0f0) >> 4);
	tmp = (tmp&0x00ff00ff) + ((tmp&0xff00ff00) >> 8);
	tmp = (tmp&0x0000ffff) + ((tmp&0xffff0000) >> 16);

	return (int)tmp;
}

int
bit_flips_n(int n, ulong w1, ulong w2)
{
	int i, ret_val = 0;

	/*  This routine is _much_ slower than above, but more general	*/
	for (i = 0; i < n; i++)
	{
		ret_val += ((w1>>i) & 1)^((w2>>i) & 1);
	}

	return ret_val;
}
