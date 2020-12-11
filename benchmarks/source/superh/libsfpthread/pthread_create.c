#include <stdio.h>
#include "spthr_all.h"

static void	preamble(ThreadState *t);

int
pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	void *(*startfn)(void *), void *argptr)
{
	ThreadState	*new;
	enum		{BUFLEN = 256};
	char		buf[BUFLEN];


	if (spthr_tas(&spthr_initdone))
	{
		spthr_init();
	}

	new = spthr_allocthread((Maddr)startfn, (Maddr)argptr);
	if (new == nil)
	{
		return -1;
	}
	spthr_addthread(new);

	/*								*/
	/*	Threads w/ remote field set are on host "host%d",	*/
	/*	where %d is the thread ID.				*/
	/*								*/
	new->remote = 1;

	/*								*/
	/*	Execution starts in preamble, which remaps accesses	*/
	/*	to the C librariy's global _impure_ptr.	Note that	*/
	/*	we pass in a pointer to the thread state structure	*/
	/*	as the argument to the preamble. Note that split cpus	*/
	/*	share a single bus structure, locking it when in use.	*/
	/*								*/
	snprintf(buf, BUFLEN, "split 0x%lx 0x%lx 0x%lx host%d\n",
		(Maddr)preamble, (Maddr)new->stkaddr, (Maddr)new, new->id);
	spthr_simcmd(buf);


	return 0;
}

void
preamble(ThreadState *t)
{
	void           	(*fptr)();
	Maddr		ipa, mpa, moff;
	enum		{BUFLEN = 128};
	char		buf[BUFLEN];
	int		myid = spthr_myid();


	/*								*/
	/*	Init the Reentrancy structure. See libc/impure.c	*/
	/*								*/
	t->rs = (struct _reent) _REENT_INIT ((t->rs));

	ipa = (Maddr)_impure_ptr;
	mpa = (Maddr)&t->rs;
	moff = mpa - ipa;

	/*								*/
	/*	Redirect all accesses to global _impure_ptr to our	*/
	/*	per-thread Reentrancy structure.			*/
	/*								*/
	/*	Haven't yet setup _impure_ptr remapping, so need to	*/
	/*	explicitly provide reentrancy structure in libc calls:	*/
	/*								*/
	_snprintf_r(&t->rs, buf, BUFLEN,
		"numaregion \"_impure_ptr\" 0x%lx 0x%lx -1 -1 -1 -1 %d 0x%lx 1",
		ipa, ipa+sizeof(struct _reent), myid, moff);
	spthr_simcmd(buf);


	/*		OK now that impure_ptr has been setup:		*/
	spthr_maptbl_add((Maddr)t, sizeof(ThreadState), t->name,
		SPTHR_MAP_PRIVATE, -1);

	fptr = (void *)t->fnaddr;
	fptr((void *)t->fnargaddr);
}
