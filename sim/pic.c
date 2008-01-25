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
