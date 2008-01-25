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
