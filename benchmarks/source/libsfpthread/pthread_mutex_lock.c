#include <stdio.h>
#include "spthr_all.h"

int
pthread_mutex_lock(pthread_mutex_t *mutex)
{
	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}

	if (DEBUG)
	{
		print("in pthread_mutex_lock, mutex addr = %x...", (ulong) mutex);
	}

	if (!spthr_maptbl_isvalid((Maddr)mutex))
	{
		return SPTHR_EINVAL;
	}

	while (!spthr_tas(&mutex->lock))
	{
		//spthr_sleep(&mutex->sleepq, nil);
		/* lock is open; retry */

		spthr_dosleep();
	}
	
	/* Have lock */
	mutex->locker = spthr_myid();

	if (DEBUG)
	{
		print("done; mutex locked by thread %d.\n",
			(int)mutex->locker);
	}

	return 0;
}
