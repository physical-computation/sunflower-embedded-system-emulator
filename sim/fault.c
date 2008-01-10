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
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sf.h"
#include "mextern.h"

static uvlong	pfunexp(void *ptr, char *type, uvlong modulo);
static uvlong	pfunrnd(void *ptr, char *type, uvlong modulo);


void
fault_feed()
{
	int			i, j;
	State			*S = NULL;
	Netsegment		*N;


	/*				*/
	/*	All device nodes:	*/
	/*				*/
	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		S = SIM_STATE_PTRS[i];

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
				mprint(S, nodeinfo,
					"\n\nNode %d recovered from random fault.\n\n",
					S->NODE_ID);
				S->runnable = 1;

				/*	Reset to address at which code was loaded	*/
				S->PC = S->STARTPC;

				/*	Can clear this now	*/
				S->got_correlated_failure = 0;
			}
		}

		if ((S->runnable) && (S->pfun(S, "state", FAULT_PROB_MODULO) 
			< (uvlong)(S->fail_prob*FAULT_PROB_MODULO)))
		{
			mprint(S, nodeinfo,
				"\n\nRandom Fault at node %d (S->fail_prob [%E]).\n",
				S->NODE_ID, S->fail_prob);

			S->nfaults++;

			/*							*/
			/*	Force fail_clocks to be always >= 1, partly	*/
			/*	that make sense and also because it simplifies	*/
			/*	the handling above.				*/
			/*							*/	
			S->fail_clocks_left = S->pfun(S, "stateduration",
				S->failure_duration_max) + 1;
			mprint(S, nodeinfo, "Set S->fail_clocks_left to [" UVLONGFMT "]\n\n",
				S->fail_clocks_left);
			S->runnable = 0;
		}
	}

	/*					*/
	/*	All Network segments:		*/
	/*					*/
	for (i = 0; i < SIM_NUM_ACTIVE_NETSEGS; i++)
	{
		N = &SIM_NET_SEGMENTS[SIM_ACTIVE_NETSEGS[i]];

		if (N->fail_prob  == 0)
		{
			continue;
		}

		if (!N->valid && N->fail_clocks_left)
		{
			N->fail_clocks_left--;
			
			if (!N->fail_clocks_left)
			{
				mprint(S, nodeinfo,
					"\n\nNetseg %d recovered from random fault.\n\n",
					N->NETSEG_ID);
				N->valid = 1;
			}
		}

		if ((N->valid) && (N->pfun(N, "network", FAULT_PROB_MODULO) <
			(uvlong)(N->fail_prob*FAULT_PROB_MODULO)))
		{
			mprint(S, nodeinfo,
				"\n\nRandom Fault at network segment %d.\n\n",
				N->NETSEG_ID);

			/*							*/
			/*	Force fail_clocks to be always >= 1, partly 	*/
			/*	that make sense and also because it simplifies	*/
			/*	the handling above.				*/
			/*							*/
			N->fail_clocks_left = N->pfun(N, "networkduration",
				N->failure_duration_max) + 1;
			N->valid = 0;

			/*							*/
			/*	Correlation with node failure. Generate a 	*/
			/*	rand(), and if it is less than correl, kill 	*/
			/*	the node too.					*/
			/*							*/
			for (j = 0; j < N->num_attached; j++)
			{
				if (N->pfun(N, "network", FAULT_PROB_MODULO) <
					(uvlong)(N->correl_coeffs[N->node_ids[j]]*
					FAULT_PROB_MODULO))
				{
					S = SIM_STATE_PTRS[N->node_ids[j]];

					mprint(S, nodeinfo,
					   "\n\nCorrelated Random Fault at node %d.\n\n",
						S->NODE_ID);

					/*						*/
					/*	Force fail_clocks to be always >= 1, 	*/
					/*	partly that make sense and also b'cos	*/
					/*	it simplifies the handling above	*/
					/*						*/
					S->fail_clocks_left = S->pfun(S, "stateduration",
						S->failure_duration_max) + 1;
					mprint(S, nodeinfo,
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

uvlong
pfunexp(void *ptr, char *type, uvlong modulo)
{
	uvlong	tmp;


	/*
		Deprecated:
			replaced with sensible implementation in development version
			of simulator
	*/
	tmp = mrandom() % (modulo + 1);	
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

uvlong
pfunrnd(void *ptr, char *type, uvlong modulo)
{
	return (mrandom() % (modulo + 1));
}


void
fault_setnodepfun(State *S, char *alg)
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
		merror("Only \"exp\" and \"urnd\" allowed.");
	}

	return;	
}

void
fault_setnetsegpfun(Netsegment *tptr, char *alg)
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
		merror("Only \"exp\" and \"urnd\" allowed.");
	}

	return;	
}

