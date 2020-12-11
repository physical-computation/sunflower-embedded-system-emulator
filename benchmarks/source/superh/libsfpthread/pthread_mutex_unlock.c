#include <stdio.h>
#include "spthr_all.h"

int
pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}

	if (DEBUG)
	{
		print("pthread_mutex_unlock() (mutex->locker=%d, mutex addr=%x)...\n",
			(int)mutex->locker, (ulong)mutex);
	}

	if (!spthr_maptbl_isvalid((Maddr)mutex))
	{
		return SPTHR_EINVAL;
	}

	if (mutex->locker != spthr_myid())
	{
		return SPTHR_EPERM;
	}

	mutex->lock = 0;

	/*	Wakeup a sleeper, if any.	*/
	spthr_wakeup(&mutex->sleepq);


	return 0;
}
