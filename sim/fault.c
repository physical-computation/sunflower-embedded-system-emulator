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
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sf.h"
#include "mextern.h"

static uvlong	pfunexp(void *, void *ptr, char *type, uvlong modulo);
static uvlong	pfunrnd(void *, void *ptr, char *type, uvlong modulo);
static tuck void	induceSEE(State *S);


void
fault_feed(Engine *E)
{
	int			i, j;
	State			*S = NULL;
	Netsegment		*N;


	/*				*/
	/*	All device nodes:	*/
	/*				*/
	for (i = 0; i < E->nnodes; i++)
	{
		S = E->sp[i];

		if (S->SEEmodeling != NULL)
		{
			induceSEE(S);
		}

		/*							*/
		/*	We could have suffered a correlated fault, 	*/
		/*	and must still be woken up.			*/
		/*							*/
		if ((S->fail_prob == 0) && (!S->got_correlated_failure))
		{
			continue;
		}

		if (!S->runnable && S->fail_clocks_left)
		{
			S->fail_clocks_left--;
			
			if (!S->fail_clocks_left)
			{
				mprint(E, S, nodeinfo,
					"\n\nNode %d recovered from random fault.\n\n",
					S->NODE_ID);
				S->runnable = 1;

				/*	Reset to address at which code was loaded	*/
				S->PC = S->STARTPC;

				/*	Can clear this now	*/
				S->got_correlated_failure = 0;
			}
		}

		if ((S->runnable) && (S->pfun(E, S, "state", FAULT_PROB_MODULO) 
			< (uvlong)(S->fail_prob*FAULT_PROB_MODULO)))
		{
			mprint(E, S, nodeinfo,
				"\n\nRandom Fault at node %d (S->fail_prob [%E]).\n",
				S->NODE_ID, S->fail_prob);

			S->nfaults++;

			/*							*/
			/*	Force fail_clocks to be always >= 1, partly	*/
			/*	that make sense and also because it simplifies	*/
			/*	the handling above.				*/
			/*							*/	
			S->fail_clocks_left = S->pfun(E, S, "stateduration",
				S->failure_duration_max) + 1;
			mprint(E, S, nodeinfo, "Set S->fail_clocks_left to [" UVLONGFMT "]\n\n",
				S->fail_clocks_left);
			S->runnable = 0;
		}
	}

	/*					*/
	/*	All Network segments:		*/
	/*					*/
	for (i = 0; i < E->nactivensegs; i++)
	{
		N = &E->netsegs[E->activensegs[i]];

		if (N->fail_prob  == 0)
		{
			continue;
		}

		if (!N->valid && N->fail_clocks_left)
		{
			N->fail_clocks_left--;
			
			if (!N->fail_clocks_left)
			{
				mprint(E, S, nodeinfo,
					"\n\nNetseg %d recovered from random fault.\n\n",
					N->NETSEG_ID);
				N->valid = 1;
			}
		}

		if ((N->valid) && (N->pfun(E, N, "network", FAULT_PROB_MODULO) <
			(uvlong)(N->fail_prob*FAULT_PROB_MODULO)))
		{
			mprint(E, S, nodeinfo,
				"\n\nRandom Fault at network segment %d.\n\n",
				N->NETSEG_ID);

			/*							*/
			/*	Force fail_clocks to be always >= 1, partly 	*/
			/*	that make sense and also because it simplifies	*/
			/*	the handling above.				*/
			/*							*/
			N->fail_clocks_left = N->pfun(E, N, "networkduration",
				N->failure_duration_max) + 1;
			N->valid = 0;

			/*							*/
			/*	Correlation with node failure. Generate a 	*/
			/*	rand(), and if it is less than correl, kill 	*/
			/*	the node too.					*/
			/*							*/
			for (j = 0; j < N->num_attached; j++)
			{
				if (N->pfun(E, N, "network", FAULT_PROB_MODULO) <
					(uvlong)(N->correl_coeffs[N->node_ids[j]]*
					FAULT_PROB_MODULO))
				{
					S = E->sp[N->node_ids[j]];

					mprint(E, S, nodeinfo,
					   "\n\nCorrelated Random Fault at node %d.\n\n",
						S->NODE_ID);

					/*						*/
					/*	Force fail_clocks to be always >= 1, 	*/
					/*	partly that make sense and also b'cos	*/
					/*	it simplifies the handling above	*/
					/*						*/
					S->fail_clocks_left = S->pfun(E, S, "stateduration",
						S->failure_duration_max) + 1;
					mprint(E, S, nodeinfo,
						"Set S->fail_clocks_left to [" UVLONGFMT "]\n\n",
						S->fail_clocks_left);
					S->runnable = 0;
					S->got_correlated_failure = 1;
				}
			}
		}
	}


	return;
}

static uvlong
pfunexp(void *e, void *ptr, char *type, uvlong modulo)
{
	uvlong	tmp;
	Engine	*E = (Engine *)e;

	/*
		Deprecated:
			replaced with sensible implementation in development version
			of simulator
	*/
	tmp = mrandom(E) % (modulo + 1);	
	if (!strcmp(type, "state") && 
		(tmp < (ulong)((State *)ptr)->fail_prob*FAULT_PROB_MODULO))
	{
		if (((State *)ptr)->fail_prob < 1)
			((State *)ptr)->fail_prob *= M_E/2;
	}
	else if (!strcmp(type, "network") &&
		(tmp < (ulong)((Netsegment *)ptr)->fail_prob*FAULT_PROB_MODULO))
	{
		if (((Netsegment *)ptr)->fail_prob < 1)
			((Netsegment *)ptr)->fail_prob *= M_E/2;
	}

	return tmp;
}

static uvlong
pfunrnd(void *e, void *ptr, char *type, uvlong modulo)
{
	Engine	*E = (Engine *)e;

	return (mrandom(E) % (modulo + 1));
}


void
fault_setnodepfun(Engine *E, State *S, char *alg)
{
	if (!strcmp(alg, "exp"))
	{
		S->pfun = pfunexp;
	}
	else if (!strcmp(alg, "urnd"))
	{
		S->pfun = pfunrnd;
	}
	else
	{
		merror(E, "Only \"exp\" and \"urnd\" allowed.");
	}

	return;	
}

void
fault_setnetsegpfun(Engine *E, Netsegment *tptr, char *alg)
{
	if (!strcmp(alg, "exp"))
	{
		tptr->pfun = pfunexp;
	}
	else if (!strcmp(alg, "urnd"))
	{
		tptr->pfun = pfunrnd;
	}
	else
	{
		merror(E, "Only \"exp\" and \"urnd\" allowed.");
	}

	return;	
}

static tuck void
induceSEE(State *S)
{
	int		which;
	SEEstruct	*p;


	/*	Get a random location in machine state		*/
	which = S->SEEmodeling->loc_pfun(S,
				0,					/*	min	*/
				S->SEEmodeling->logical_bits - 1,	/*	max	*/
				S->SEEmodeling->loc_pfun_p1,		/* Dist params:	*/
				S->SEEmodeling->loc_pfun_p2,
				S->SEEmodeling->loc_pfun_p3,
				S->SEEmodeling->loc_pfun_p4);

	p = S->SEEmodeling->hd;
	while (p != NULL)
	{
		if (	(which >= p->logical_offset) &&
			(which < (p->logical_offset + p->logical_bits)))
		{
			uchar	b;
			ulong	mask;
			int	offset;

			/*	Get a random bit state (0/1)	*/
			b = S->SEEmodeling->bit_pfun(S,
				0,					/*	min	*/
				1,					/*	max	*/
				S->SEEmodeling->bit_pfun_p1,		/* Dist params:	*/
				S->SEEmodeling->bit_pfun_p2,
				S->SEEmodeling->bit_pfun_p3,
				S->SEEmodeling->bit_pfun_p4);


			/*	Integer division result is OK for us here	*/
			offset = ((which - p->logical_offset)*p->actual_bits)/p->logical_bits;


			/*							*/
			/*	If dealing with sub-structure, offset is 	*/
			/*	from a non-zero offset from structure begin	*/
			/*							*/
			offset += p->bit_offset;

			if (b == 0)
			{
				mask = ~(1UL << offset);
				*(p->hw) &= mask;
			}
			else
			{
				mask = 1UL << offset;
				*(p->hw) |= mask;
			}

			break;
		}

		p = p->next;
	}

	return;
}

/*										*/
/*	The true size of the structure is actual_bits, and the logical		*/
/*	'weight' assigned to the structure is logical_bits;  Two 		*/
/*	structures with the same actual size but with differing logical		*/
/*	sizes will see the one with larger logical size incurring more		*/
/*	SEEs.  Sub-parts of a structure can be treated differently,		*/
/*	via the bit_offset.  E.g., to treat upper and lower halves		*/
/*	of a pipeline latch differently, register it with system		*/
/*	twice, at half the full structure size, with different offsets,		*/
/*	and with same actual size but different logical size.			*/
/*										*/
void
m_hwSEEreg(Engine *E, State *S, void *hw, int actual_bits, int logical_bits, int bit_offset)
{
	SEEstruct	*s;
	

	s = (SEEstruct *) mcalloc(E, 1, sizeof(SEEstruct), "fault.c:m_hwSEEreg/SEEstruct *s");
	if (s == NULL)
	{
		sfatal(E, NULL, "Mcalloc failed");
		return;
	}

	s->hw			= (uchar *)hw;
	s->actual_bits		= actual_bits;
	s->logical_bits		= logical_bits;
	s->bit_offset		= bit_offset;

	if (S->SEEmodeling->hd == NULL || S->SEEmodeling->tl == NULL)
	{
		S->SEEmodeling->tl = S->SEEmodeling->hd = s;
		S->SEEmodeling->logical_bits = 0;
		S->SEEmodeling->actual_bits = 0;
	}
	else
	{
		S->SEEmodeling->tl->next = s;
		S->SEEmodeling->tl = s;
	}
	s->logical_offset = S->SEEmodeling->logical_bits;


	S->SEEmodeling->logical_bits	+= logical_bits;
	S->SEEmodeling->actual_bits	+= actual_bits;


	return;
}
