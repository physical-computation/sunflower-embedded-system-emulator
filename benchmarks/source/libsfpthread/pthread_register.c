#include "spthr_all.h"

void
pthread_register(Maddr base, int size, char *tag, int homenode)
{
	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}
	
	if (DEBUG)
	{
		print("pthread_register...\n");
	}

	spthr_maptbl_add(base, size, tag, SPTHR_MAP_PUBLIC, homenode);

	return;
}
