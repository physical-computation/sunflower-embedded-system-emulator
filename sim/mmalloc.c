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
#include "mmalloc.h"


MMemblock	memblocks[MMALLOC_MAXALLOCBLOCKS];
int		nmemblocks = 0;

void		allocaccounting(Maddr addr, char *ID);
void		reallocaccounting(Maddr addr, void *oldaddr, char *ID);


tuck void
allocaccounting(Maddr addr, char *ID)
{
	int	i;


	if (addr == 0)
	{
		mprint(NULL, siminfo, "malloc/calloc failed for \"%s\"\n", ID);
		mmblocksdisplay();

		mexit("", -1);
	}

	if (nmemblocks == MMALLOC_MAXALLOCBLOCKS)
	{
		mprint(NULL, siminfo, "Ran out of memblocks. Exiting...\n");
		mmblocksdisplay();

		mexit("", -1);
	}

	for (i = 0; i < nmemblocks; i++)
	{
		if (!strncmp(memblocks[i].ID, ID, MMALLOC_MAXIDSTRLEN))
		{
			int	nentries = memblocks[i].allocs - memblocks[i].frees;

			if (nentries >= MMALLOC_MAXALLOCS)
			{
				mprint(NULL, siminfo,
					"Ran out of memblock[].addrs[]. Exiting...\n");
				mmblocksdisplay();
				mexit("", -1);
			}

			memblocks[i].addrs[nentries] = addr;
			memblocks[i].allocs++;
				
			return;
		}
	}

	strncpy(memblocks[i].ID, ID, MMALLOC_MAXIDSTRLEN);

	if (!memblocks[i].mallocd)
	{
		memblocks[i].addrs = (uvlong *)calloc(MMALLOC_MAXALLOCS, sizeof(Maddr));
		if (memblocks[i].addrs == NULL)
		{
			mprint(NULL, siminfo,
				"Could not allocate memory for (Maddr *)memblocks[i].addrs\n");
			mexit("", -1);
		}
		memblocks[i].mallocd = 1;
	}

	memblocks[i].addrs[0] = addr;
	memblocks[i].allocs = 1;
	memblocks[i].frees = 0;
	memblocks[i].reallocs = 0;
	memblocks[i].valid = 1;

	nmemblocks++;


	return;
}

tuck void
reallocaccounting(Maddr addr, void *oldaddr, char *ID)
{
	int	i;


	if (addr == 0)
	{
		mprint(NULL, siminfo, "realloc failed for \"%s\"\n", ID);
		mmblocksdisplay();

		mexit("", -1);
	}

	for (i = 0; i < nmemblocks; i++)
	{
		int	nentries = memblocks[i].allocs - memblocks[i].frees;

		if (!strncmp(memblocks[i].ID, ID, MMALLOC_MAXIDSTRLEN))
		{
			memblocks[i].addrs[nentries] = addr;
			memblocks[i].reallocs++;
				
			return;
		}
	}

	if (!memblocks[i].valid)
	{
		mprint(NULL, siminfo,
			"Attempt to realloc an unalloc'd chunk, ID = [%s]\n", ID);
		mmblocksdisplay();

		mexit("", -1);
	}

	return;
}

void*
mmalloc(int size, char *ID)
{
	Maddr addr = (Maddr)malloc(size);
	if (SF_DEBUG)
	{
		allocaccounting(addr, ID);
	}

	return (void *)addr;
}

void*
mcalloc(int nelem, int size, char *ID)
{
	Maddr addr = (Maddr)calloc(nelem, size);
	if (SF_DEBUG)
	{
		allocaccounting(addr, ID);
	}

	return (void *)addr;
}

void*
mrealloc(void *oldptr, int size, char *ID)
{
	Maddr addr = (Maddr)realloc(oldptr, size);
	if (SF_DEBUG)
	{
		reallocaccounting(addr, oldptr, ID);
	}

	return (void *)addr;
}

void
mfree(void *ptr, char *ID)
{
	int	i, j;


	if (!SF_DEBUG)
	{
		free(ptr);
		return;
	}

	for (i = 0; i < nmemblocks; i++)
	{
		int	nentries = memblocks[i].allocs - memblocks[i].frees;

		for (j = 0; j < nentries; j++)
		{
			if (memblocks[i].addrs[j] == (Maddr)ptr)
			{
				if (nentries == 1)
				{
					memmove(&memblocks[i], &memblocks[nmemblocks - 1],
						sizeof(MMemblock));
					memblocks[nmemblocks - 1].valid = 0;
					nmemblocks--;
				}
				else
				{
					memblocks[i].addrs[j] = memblocks[i].addrs[nentries - 1];
					memblocks[i].addrs[nentries - 1] = 0;
					memblocks[i].frees++;
				}
				free(ptr);

				return;
			}
		}
	}

	if (SF_CHATTY)
	{
		mprint(NULL, siminfo, 
			"WARNING!: You tried to free an unallocated block, ID = [%s].\n", ID);
	}
	
	return;
}

void
mmblocksdisplay(void)
{
	int	i;

	mprint(NULL, siminfo, "\nM-MALLOC Statistics:\n\n");
	for (i = 0; i < MMALLOC_MAXALLOCBLOCKS; i++)
	{
		if (!memblocks[i].valid)
		{
			break;
		}

		mprint(NULL, siminfo, "Block \"%-64s\": %d a%-1s %d f%-1s %d r%-1s\n",
			memblocks[i].ID, memblocks[i].allocs,
			(memblocks[i].allocs == 1 ? "," : "s,"),

			memblocks[i].frees,
			(memblocks[i].frees == 1 ? "" : "s"),

			memblocks[i].reallocs,
			(memblocks[i].reallocs == 1 ? "" : "s"));
	}
	mprint(NULL, siminfo, "\n");

	return;
}
