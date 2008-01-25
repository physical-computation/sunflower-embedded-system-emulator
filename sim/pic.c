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

/*									*/
/*	Programmable Interrupt Controller. Provides queued intrs.	*/
/*									*/

void
pic_intr_enqueue(Engine *E, State *S, InterruptQ *q, int type, int value, int misc)
{
	Interrupt	*tmp;


	tmp = (Interrupt *)mcalloc(E, 1, sizeof(Interrupt),
		"Interrupt structure in pic_intr_enqueue");

	if (tmp == NULL)
	{
		mexit(E, "Could not allocate memory for (Interrupt *)tmp", -1);
		return;
	}

	tmp->type = type;
	tmp->value = value;
	tmp->misc = misc;

	/*	intrqhd and intrqtl are pre-allocated when node is created.	*/
	if (q->nqintrs == 0)
	{
		tmp->next = q->tl;
		tmp->prev = q->hd;
		q->tl->prev = tmp;
		q->hd->next = tmp;
	}
	else
	{
		tmp->next = q->tl;
		tmp->prev = q->tl->prev;
		q->tl->prev->next = tmp;
		q->tl->prev = tmp;
	}
	q->nqintrs++;


	return;
}

void *
pic_intr_dequeue(Engine *E, State *S, InterruptQ *q)
{
	Interrupt	*ptr;


	if (q->nqintrs == 0)
	{
		return NULL;
	}

	ptr = q->hd->next;
	q->hd->next = ptr->next;
	ptr->next->prev = q->hd;
	q->nqintrs--;


	return ptr;
}

void
pic_intr_clear(Engine *E, State *S, InterruptQ *q, int type, int clear_all)
{
	int		i;
	Interrupt	*p;


	/*						*/
	/*	Ugh, this is ugly. All the mallocs.	*/
	/*	Rewrite to reorder list w/o doing all	*/
	/*	these malloc/frees			*/
	/*						*/
	while ((p = (Interrupt *) pic_intr_dequeue(E, S, q)) != NULL)
	{
		if (p->type != type)
		{
			pic_intr_enqueue(E, S, q, p->type, p->value, p->misc);
		}
		else
		{
			mfree(E, p, "Interrupt *p");
			
			if (!clear_all)
			{
				break;
			}
		}
	}

	/*	Now, need to reorder the queue		*/
	for (i = 0; i < q->nqintrs; i++)
	{
		p = (Interrupt *) pic_intr_dequeue(E, S, q);
		pic_intr_enqueue(E, S, q, p->type, p->value, p->misc);
		mfree(E, p, "Interrupt *p");
	}

	return;
}
