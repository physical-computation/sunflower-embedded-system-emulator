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

static tuck void	do_tandem(Engine *, State *S,  ulong *data, int n, Regvt **match);
static tuck void	do_valuetrace(Engine *E, State *S, ulong *data, int n);


	/*									*/
	/*	Register Read/Write routines: we only access reg contents	*/
	/*	through these, except in the XXX_BANK() routines.		*/
	/*									*/


tuck ulong
reg_read(Engine *E, State *S, int n)
{
	ulong	data;

	if ((n <= 7) && (S->superH->SR.RB == 1) && (S->superH->SR.MD == 1))
	{
		data = S->superH->R_BANK[n];
	}
	else
	{
		data = S->superH->R[n];
	}

	/*							*/
	/*   We model reg file as single ported, and keep last	*/
	/*   value seen at port in S->reg_file_port. On each R/W,*/
	/*   we count # transitions at port.			*/
	/*							*/
	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->regfile_port, data);
		S->regfile_port = data;
	}

	if (SF_VALUETRACE_ANALYSIS)
	{
		do_valuetrace(E, S, &data, n);
	}

	return data;
}


tuck void
reg_set(Engine *E, State *S, int n, ulong data)
{
	if (SF_VALUETRACE_ANALYSIS)
	{
		do_valuetrace(E, S, &data, n);
	}

	/*								*/
	/*	We model reg file as single ported, and keep last	*/
	/*	value seen at port in S->reg_file_port. On each R/W,	*/
	/*	we count # transitions at port.				*/
	/*								*/
	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->regfile_port, data);
		S->regfile_port = data;
	}

	if ((n <= 7) && (S->superH->SR.RB == 1) && (S->superH->SR.MD == 1))
	{
		/*								*/
		/*	Additionally, for reg writes, bits in reg file		*/
		/*	for specific register may flip.				*/
		/*								*/
		if (SF_BITFLIP_ANALYSIS)
		{
			S->Cycletrans += bit_flips_32(S->superH->R_BANK[n], data);
		}

		S->superH->R_BANK[n] = data;
	}
	else
	{
		/*								*/
		/*	Additionally, for reg writes, bits in reg file		*/
		/*	for specific register may flip.				*/
		/*								*/
		if (SF_BITFLIP_ANALYSIS)
		{
			S->Cycletrans += bit_flips_32(S->superH->R[n], data);
		}

		S->superH->R[n] = data;
	}

	return;
}

tuck void
do_valuetrace(Engine *E, State *S, ulong *data, int n)
{
	Regvt	R, *key = &R, **match;

	key->regnum = n;
	key->pcstart = S->PCSTACK[S->pcstackheight];

	match = (Regvt**) bsearch(&key, S->RT->regvts, S->RT->count, 
			sizeof(Regvt*), m_sort_regtracers_comp);

	/*										*/
	/*	As a heuristic to filter out uses of an registers by the compiler,	*/
	/*	for purposes other than the variable associated with that register	*/
	/*	in the STABS debugging info, if the underlying variable being		*/
	/*	traced is not a pointer, we ignore values that look like code		*/
	/*	or data addresses.							*/
	/*										*/
	if (match != NULL && ((*match)->ispointer || (*data < S->MEMBASE) || (*data > S->MEMEND)))
	{
		/*								*/
		/*	For value tracing, if access is bigger than the		*/
		/*	underlying data, mask off excess.			*/
		/*								*/
		ulong	mask = ~0UL;
		if ((*match)->size < 4)
		{
			mask = (1 << ((*match)->size << 3)) - 1;
		}

		(*match)->values[(*match)->validx] = *data & mask;

		if (SF_FT_TANDEM)
		{
			do_tandem(E, S, data, n, match);
		}

		if ((*match)->validx < MAX_REGTRACER_VALUETRACE)
		{
			(*match)->validx++;
		}

		if ((*match)->validx == (*match)->nvalues)
		{
			ulong	*tmp;

			(*match)->nvalues *= 2;
			tmp = (ulong *)mrealloc(E, (*match)->values,
				(*match)->nvalues*sizeof(ulong),
				"realloc (*match)->values in regaccess.c");

			if (tmp == NULL)
			{
				mprint(E, NULL, siminfo,
					"Resizing (*match)->values to %d entries failed\n",
					(*match)->nvalues);
				sfatal(E, S, "realloc failed for (*match)->values in regaccess.c");
			}
			(*match)->values = tmp;
		}
	}
	
	return;
}

tuck void
do_tandem(Engine *E, State *S,  ulong *data, int n, Regvt **match)
{
	/*						*/
	/*	For tandem analysis, node 1 incurs	*/
	/*	upsets, and node 0 is the control	*/
	/*	(clean) state.	Furthermore, the ff.	*/
	/*	code assumes node 0 is updated first,	*/
	/*	so when we are about to read a faulty	*/
	/*	state, we can verify (oracle case).	*/
	/*						*/
	if (S->NODE_ID == 1)
	{
		ulong	devn;

		devn = (*data - E->sp[0]->superH->R[n]);
		if (devn != 0)
		{
			(*match)->devns[(*match)->validx] = devn;

			/*	Oracle fixes all devs in vars	*/
			*data = E->sp[0]->superH->R[n];
		}
	}

	return;
}
