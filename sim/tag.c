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
#include <string.h>
#include "sf.h"
#include "mextern.h"

void
tag_settag(State *S, int whichtag, char *tag, char *whohas, int confidence, ulong ttl)
{
	Tag     *tmptag;


	if (whichtag >= TAG_NTAGS)
	{
		merror("Tag out of range");
		return;
	}

	tmptag = &S->TAGMEM[whichtag];
	strncpy(tmptag->string, tag, TAG_STRING_LEN);
	strncpy(tmptag->who_has, whohas, NIC_ADDR_LEN);
	tmptag->confidence = confidence;
	tmptag->ttl = ttl;
			
	return;
}

void
tag_showtags(State *S)
{
	int 	i;
	Tag	tmptag;


	for (i = 0; i < TAG_NTAGS; i++)
	{
		tmptag = S->TAGMEM[i];
		if (tmptag.ttl > 0)
		{
			mprint(S, nodeinfo, "TAGMEM[%d].string = \"%s\"\n",\
				i, tmptag.string);
			mprint(S, nodeinfo, "TAGMEM[%d].who_has = \"%s\"\n",\
				i, tmptag.who_has);
			mprint(S, nodeinfo, "TAGMEM[%d].confidence = %d\n",\
					i, tmptag.confidence);
			mprint(S, nodeinfo, "TAGMEM[%d].ttl = %lu\n",\
				i, tmptag.ttl);
		}
	}
			
	return;
}
