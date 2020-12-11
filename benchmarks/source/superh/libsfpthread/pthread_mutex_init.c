#include <stdio.h>
#include "spthr_all.h"

int
pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}
	
	if (DEBUG)
	{
		print("pthread_mutex_init...\n");
	}

	mutex->lock = 0;
	spthr_maptbl_add((Maddr)mutex, sizeof(pthread_mutex_t),
		"pthread_mutex_t *mutex", SPTHR_MAP_PUBLIC, -1);

	return 0;
}
