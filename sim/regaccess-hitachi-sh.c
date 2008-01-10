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
/*									*/
/*	Register Read/Write macros: we only access reg contents		*/
/*	through these, except in the XXX_BANK() routines.		*/
/*									*/
tuck ulong
reg_read(State *S, int n)
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
		if (match != NULL && ((*match)->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
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

			(*match)->values[(*match)->validx] = data & mask;

			if ((*match)->validx < MAX_REGTRACER_VALUETRACE)
			{
				(*match)->validx++;
			}

			if ((*match)->validx == (*match)->nvalues)
			{
				ulong	*tmp;

				(*match)->nvalues *= 2;
				tmp = (ulong *)mrealloc((*match)->values,
					(*match)->nvalues*sizeof(ulong), "realloc (*match)->values in regaccess.c");
				if (tmp == NULL)
				{
					mprint(NULL, siminfo,
						"Resizing (*match)->values to %d entries failed\n",
						(*match)->nvalues);
					sfatal(S, "realloc failed for (*match)->values in regaccess.c");
				}
				(*match)->values = tmp;
			}
		}
	}

	return data;
}


tuck void
reg_set(State *S, int n, ulong data)
{
	if (SF_VALUETRACE_ANALYSIS)
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
		if (match != NULL && ((*match)->ispointer || (data < S->MEMBASE) || (data > S->MEMEND)))
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

			(*match)->values[(*match)->validx] = data & mask;

			if ((*match)->validx < MAX_REGTRACER_VALUETRACE)
			{
				(*match)->validx++;
			}

			if ((*match)->validx == (*match)->nvalues)
			{
				ulong	*tmp;

				(*match)->nvalues *= 2;
				tmp = (ulong *)mrealloc((*match)->values,
					(*match)->nvalues*sizeof(ulong), "realloc (*match)->values in regaccess.c");
				if (tmp == NULL)
				{
					mprint(NULL, siminfo,
						"Resizing (*match)->values to %d entries failed\n",
						(*match)->nvalues);
					sfatal(S, "realloc failed for (*match)->values in regaccess.c");
				}
				(*match)->values = tmp;
			}
		}
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
