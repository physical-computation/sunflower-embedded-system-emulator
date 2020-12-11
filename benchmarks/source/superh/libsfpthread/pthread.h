/*									*/
/*			Pthread standard interface			*/
/*									*/
/*	for size_t in pthread.h:	*/
#include <stddef.h>

/*	for struct _reent:		*/
#include <reent.h>

#include "e-types.h"
#include "spthr_internals.h"

typedef int			pthread_once_t;
typedef unsigned long int	pthread_t;
typedef unsigned int		pthread_key_t;
typedef int			sigset_t;

typedef struct
{
	int 			sched_priority;
} sched_param;

typedef struct
{
	int			junk;
} timespec;

typedef struct
{
	char			attr;
} pthread_attr_t;

typedef struct
{
	char			cond;
	Sleepq			*sleepq;
} pthread_cond_t;

typedef struct
{
	char			condattr;
} pthread_condattr_t;

typedef struct
{
	char			lock;
	int			locker;
	Sleepq			*sleepq;
} pthread_mutex_t;

typedef struct
{
	char			mutexattr;
} pthread_mutexattr_t;


/*										*/
/*	The various constants used in pthreads standard calls go here.		*/
/*	For now, we only have this one below:					*/
/*										*/
#define PTHREAD_CREATE_JOINABLE      1


int	pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
int	pthread_attr_destroy(pthread_attr_t *attr);
int	pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int	pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);
int	pthread_attr_getschedparam(const pthread_attr_t *attr, sched_param *param);
int	pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int	pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
int	pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
int	pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
int	pthread_attr_init(const pthread_attr_t *attr);
int	pthread_attr_setdetachstate(const pthread_attr_t *attr, int detachstate);
int	pthread_attr_setinheritsched(const pthread_attr_t *attr, int inherit);
int	pthread_attr_setschedparam(const pthread_attr_t *attr, const sched_param *param);
int	pthread_attr_setschedpolicy(const pthread_attr_t *attr, int policy);
int	pthread_attr_setscope(const pthread_attr_t *attr, int scope);
int	pthread_attr_setstackaddr(const pthread_attr_t *attr, void *stackaddr);
int	pthread_attr_setstacksize(const pthread_attr_t *attr, size_t stacksize);
int	pthread_cancel(pthread_t thread);
void	pthread_cleanup_pop(int execute);
void	pthread_cleanup_push(void (*routine)(void), void *arg);
int	pthread_cond_broadcast(pthread_cond_t *cond);
int	pthread_cond_destroy(pthread_cond_t *cond);
int	pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int	pthread_cond_signal(pthread_cond_t *cond);
int	pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const timespec *abstime);
int	pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int	pthread_condattr_destroy(pthread_condattr_t *attr);
int	pthread_condattr_getpshared(pthread_condattr_t *attr, int *pshared);
int	pthread_condattr_init(pthread_condattr_t *attr);
int	pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
int	pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int	pthread_detach(pthread_t thread);
int	pthread_equal(pthread_t thread1, pthread_t thread2);
void	pthread_exit(void *value);
int	pthread_getschedparam(pthread_t thread, int *policy, sched_param *param);
void	pthread_getspecific(pthread_key_t key);
int	pthread_join(pthread_t thread, void **value_ptr);
int	pthread_key_create(pthread_key_t *key, void (destructor)(void *));
int	pthread_key_delete(pthread_key_t key);
int	pthread_kill(pthread_t thread, int signal);
int	pthread_mutex_destroy(pthread_mutex_t *mutex);
int	pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int	pthread_mutex_lock(pthread_mutex_t *mutex);
int	pthread_mutex_trylock(pthread_mutex_t *mutex);
int	pthread_mutex_unlock(pthread_mutex_t *mutex);
int	pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int	pthread_mutexattr_getprioceiling(pthread_mutexattr_t *attr, int *prioceiling);
int	pthread_mutexattr_getprotocol(pthread_mutexattr_t *attr, int *protocol);
int	pthread_mutexattr_getpshared(pthread_mutexattr_t *attr, int *pshared);
int	pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
int	pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
int	pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
int	pthread_once(pthread_once_t *once_block, void (*init_routine)(void));
int	pthread_self(void);
int	pthread_setcancelstate(int state, int *oldstate);
int	pthread_setcanceltype(int type, int *oldtype);
int	pthread_setschedparam(pthread_t thread, int policy, const sched_param *param);
int	pthread_setspecific(pthread_key_t key, void *value);
int	pthread_sigmask(int how, const sigset_t *set, sigset_t *oset);
void	pthread_testcancel(void);


/*									*/
/*	This is our extension to the pthread interface, useful when	*/
/*	address spaces are not shared. There must still be a single	*/
/*	location where the library all its internal structures. This	*/
/*	raises the age-old issue of how do you access this central	*/
/*	name space, and how can it be made non-central.			*/
/*									*/
/*	A thread registers an address range (base, and size) with	*/
/*	an associated tag.  Accesses to objects with the same tag	*/
/*	across different threads/CPUs will access the same object.	*/
/*	These accesses must therefore be guarded by the programmer	*/
/*	with the use of mutexs (regular pthread_mutex_t).  These	*/
/*	mutexs themselves have to be global, and hence must also 	*/
/*	have been registered. This registration must be done before	*/
/*	calling pthread_mutex_init().					*/
/*									*/
/*	A downside to this approach is that there is no compile-	*/
/*	or link-time checking.						*/
/*									*/
/*	Implementation: The actual map list is in a global shared	*/
/*	memory.	 Static approach: Profile application to get both	*/
/*	(i) total number of accesses by each thread per object and	*/
/*	time sequence of accesses to objects. The former can be used	*/
/*	to setup a single fixed mapping, and the latter an be used	*/
/*	to setup a single time-varying (but predetermined) mapping	*/
/*	Dynamic approach: every MAPUPDATE_QUANTUM milliseconds, the	*/
/*	interrupt handler calls spthr_map_update(). This goes through	*/
/*	the map list, and looks at the access counts of each thread	*/
/*	to each maplist item and (1) locks the bus and copies data if	*/
/*	mapping is changing (2) updates the mapping.			*/
/*		There are many criteria that can be used to determine	*/
/*	whether to update the mapping: (a) thread with most accesses	*/
/*	within a time window gets the object locally. Need to be able	*/
/*	to prevent against pathological cases that lead to "thrashing"	*/
/*	of objects between memories. (b) thread with the highest CPU	*/
/*	utilization get the object local, aim being to maximize the	*/
/*	throughput. This can lead to a bottleneck-inversion as we	*/
/*	saw in our preliminary study.					*/
/*									*/
/*	NOTE: Before we can make good decisions about where to make	*/
/*	objects local, it would be beneficial to understand what the	*/
/*	distribution of the duration of memory transfers. These		*/
/*	durations are a function of both the sizes of memory objects,	*/
/*	and the behavior of the cache hierarchy.			*/
/*									*/
void	pthread_register(Maddr base, int size, char *tag, int homenode);
