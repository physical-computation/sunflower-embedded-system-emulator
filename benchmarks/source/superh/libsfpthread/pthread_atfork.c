#include "spthr_all.h"

int
pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
	return 0;
}
