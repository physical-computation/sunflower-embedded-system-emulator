#include <stdio.h>
#include "spthr_all.h"

int
pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}

	if (DEBUG)
	{
		print("pthread_cond_wait...\n");
	}

	if (!spthr_maptbl_isvalid((Maddr)cond)
		|| !spthr_maptbl_isvalid((Maddr)mutex))
	{
		return SPTHR_EINVAL;
	}

	/*								*/
	/*	This unlock and sleep should be atomic, otherwise	*/
	/*	we unlock, then the other thread thinks its OK to	*/
	/*	signal, but it signals before we have enqueued in 	*/
	/*	sleep!							*/
	/*								*/
	spthr_sleep(&cond->sleepq, mutex);

	return 0;
}
