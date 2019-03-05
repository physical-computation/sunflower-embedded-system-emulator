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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sf.h"



void		allocaccounting(Engine *, Maddr addr, char *ID);
void		reallocaccounting(Engine *, Maddr addr, void *oldaddr, char *ID);


tuck void
allocaccounting(Engine *E, Maddr addr, char *ID)
{
	int	i;


	if (addr == 0)
	{
		mprint(E, NULL, siminfo, "malloc/calloc failed for \"%s\"\n", ID);
		mmblocksdisplay(E);

		mexit(E, "", -1);
	}

	if (E->nmemblocks == MMALLOC_MAXALLOCBLOCKS)
	{
		mprint(E, NULL, siminfo, "Ran out of memblocks. Exiting...\n");
		mmblocksdisplay(E);

		mexit(E, "", -1);
	}

	for (i = 0; i < E->nmemblocks; i++)
	{
		if (!strncmp(E->memblocks[i].ID, ID, MMALLOC_MAXIDSTRLEN))
		{
			int	nentries = E->memblocks[i].allocs - E->memblocks[i].frees;

			if (nentries >= MMALLOC_MAXALLOCS)
			{
				mprint(E, NULL, siminfo,
					"Ran out of memblock[].addrs[]. Exiting...\n");
				mmblocksdisplay(E);
				mexit(E, "", -1);
			}

			E->memblocks[i].addrs[nentries] = addr;
			E->memblocks[i].allocs++;
				
			return;
		}
	}

	strncpy(E->memblocks[i].ID, ID, MMALLOC_MAXIDSTRLEN);

	if (!E->memblocks[i].mallocd)
	{
		E->memblocks[i].addrs = (uvlong *)calloc(MMALLOC_MAXALLOCS, sizeof(Maddr));
		if (E->memblocks[i].addrs == NULL)
		{
			mprint(E, NULL, siminfo,
				"Could not allocate memory for (Maddr *)memblocks[i].addrs\n");
			mexit(E, "", -1);
		}
		E->memblocks[i].mallocd = 1;
	}

	E->memblocks[i].addrs[0] = addr;
	E->memblocks[i].allocs = 1;
	E->memblocks[i].frees = 0;
	E->memblocks[i].reallocs = 0;
	E->memblocks[i].valid = 1;

	E->nmemblocks++;


	return;
}

tuck void
reallocaccounting(Engine *E, Maddr addr, void *oldaddr, char *ID)
{
	int	i;


	if (addr == 0)
	{
		mprint(E, NULL, siminfo, "realloc failed for \"%s\"\n", ID);
		mmblocksdisplay(E);

		mexit(E, "", -1);
	}

	for (i = 0; i < E->nmemblocks; i++)
	{
		int	nentries = E->memblocks[i].allocs - E->memblocks[i].frees;

		if (!strncmp(E->memblocks[i].ID, ID, MMALLOC_MAXIDSTRLEN))
		{
			E->memblocks[i].addrs[nentries] = addr;
			E->memblocks[i].reallocs++;
				
			return;
		}
	}

	if (!E->memblocks[i].valid)
	{
		mprint(E, NULL, siminfo,
			"Attempt to realloc an unalloc'd chunk, ID = [%s]\n", ID);
		mmblocksdisplay(E);

		mexit(E, "", -1);
	}

	return;
}

void*
mmalloc(Engine *E, int size, char *ID)
{
	Maddr addr = (Maddr)malloc(size);
	if (SF_DEBUG)
	{
		allocaccounting(E, addr, ID);
	}

	return (void *)addr;
}

void*
mcalloc(Engine *E, int nelem, int size, char *ID)
{
	Maddr addr = (Maddr)calloc(nelem, size);
	if (SF_DEBUG)
	{
		allocaccounting(E, addr, ID);
	}

	return (void *)addr;
}

void*
mrealloc(Engine *E, void *oldptr, int size, char *ID)
{
	Maddr addr = (Maddr)realloc(oldptr, size);
	if (SF_DEBUG)
	{
		reallocaccounting(E, addr, oldptr, ID);
	}

	return (void *)addr;
}

void
mfree(Engine *E, void *ptr, char *ID)
{
	int	i, j;


	if (!SF_DEBUG)
	{
		free(ptr);
		return;
	}

	for (i = 0; i < E->nmemblocks; i++)
	{
		int	nentries = E->memblocks[i].allocs - E->memblocks[i].frees;

		for (j = 0; j < nentries; j++)
		{
			if (E->memblocks[i].addrs[j] == (Maddr)ptr)
			{
				if (nentries == 1)
				{
					memmove(&E->memblocks[i], &E->memblocks[E->nmemblocks - 1],
						sizeof(MMemblock));
					E->memblocks[E->nmemblocks - 1].valid = 0;
					E->nmemblocks--;
				}
				else
				{
					E->memblocks[i].addrs[j] = E->memblocks[i].addrs[nentries - 1];
					E->memblocks[i].addrs[nentries - 1] = 0;
					E->memblocks[i].frees++;
				}
				free(ptr);

				return;
			}
		}
	}

	if (SF_CHATTY)
	{
		mprint(E, NULL, siminfo, 
			"WARNING!: You tried to free an unallocated block, ID = [%s].\n", ID);
	}
	
	return;
}

void
mmblocksdisplay(Engine *E)
{
	int	i;

	mprint(E, NULL, siminfo, "\nM-MALLOC Statistics:\n\n");
	for (i = 0; i < MMALLOC_MAXALLOCBLOCKS; i++)
	{
		if (!E->memblocks[i].valid)
		{
			break;
		}

		mprint(E, NULL, siminfo, "Block \"%-64s\": %d a%-1s %d f%-1s %d r%-1s\n",
			E->memblocks[i].ID, E->memblocks[i].allocs,
			(E->memblocks[i].allocs == 1 ? "," : "s,"),

			E->memblocks[i].frees,
			(E->memblocks[i].frees == 1 ? "" : "s"),

			E->memblocks[i].reallocs,
			(E->memblocks[i].reallocs == 1 ? "" : "s"));
	}
	mprint(E, NULL, siminfo, "\n");

	return;
}
