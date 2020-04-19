#include <stdio.h>
#include "spthr_all.h"

int
pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}

	if (DEBUG)
	{
		print("pthread_cond_init...\n");
	}

	spthr_maptbl_add((Maddr)cond, sizeof(pthread_cond_t),
		"pthread_cond_t *cond", SPTHR_MAP_PUBLIC, -1);

	return 0;
}
