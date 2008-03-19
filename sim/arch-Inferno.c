/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
	All rights reserved.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written 
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <time.h>
#include <stdlib.h>
#include "lib9.h"
#include "mathi.h"
#include "kernel.h"
#include "fcall.h"
#include "sf.h"
#include "mextern.h"

#ifdef linux
#	include <sys/types.h>
#	include <sys/mman.h>
#	include <sched.h>
#endif

extern void	pexit(char*, int);
extern vlong	osusectime(void);
extern int	osmillisleep(ulong);
extern int	devsfspawnscheduler(Engine *);
extern int	devsfkillscheduler(int);

tuck void	checkh2o(int maxbufsz, int *h2o, char *circbuf, char *buf);


QLock		statelock;

void
mstatelock(void)
{
	qlock(&statelock);
}

void
mstateunlock(void)
{
	qunlock(&statelock);
}


void
marchinit(void)
{
	/*								*/
	/*	On Linux, we want to:					*/
	/*		(a)	Force realtime scheduling so our	*/
	/*			throttling delays don't just make	*/
	/*			kernel context switch us out		*/
	/*								*/
	/*		(b)	Attempt to lock all pages in memory,	*/
	/*			to for less jitter in the above. We	*/
	/*			don't complain/care if attempt fails.	*/
	/*	Adapted from						*/
	/*	www-106.ibm.com/developerworks/library/l-osmig3.html	*/
	/*								*/
#ifdef linux
	struct sched_param	sp;
	struct timespec		ts, rem;
	
	print("Devsf: Linux-specific marchinit...\n");
	sp.sched_priority = sched_get_priority_max(SCHED_RR);
	sched_setscheduler(getpid(), SCHED_RR, &sp);

	if (mlockall(MCL_FUTURE|MCL_CURRENT) < 0)
	{
		print("Devsf: Pages not locked in memory.\n");
	}
	else
	{
		print("Devsf: Pages locked in memory.\n");
	}
#endif

	//setpri(PriHi);

	/*								*/
	/*	By default, the FP control word we inherit treats	*/
	/*	all FP exceptions besides INEX as fatal. We however	*/
	/*	don't want to get killed for UNFL and OVFL (and ZDIV	*/
	/*	though that should prolly kill us). See Eric Grosse's	*/
	/*	"Real Inferno" paper for more information.		*/
	/*								*/
	/*
		TODO: decide whether or not we want this.  Need to fix
		the cause of FP exceptions on Linux.  I think it is
		caused by batt code, since it is triggered towards end
		of simulation
	*/

/*
	FPcontrol(0, INEX);
	FPcontrol(0, OVFL);
	FPcontrol(0, UNFL);
	FPcontrol(0, ZDIV);
*/
	FPcontrol(1, INEX);
	FPcontrol(1, OVFL);
	FPcontrol(1, UNFL);
	FPcontrol(1, ZDIV);

	// deprecated: cleanup the use of VERBOSE and delete the ff:
	/*								*/
	/*	When running as an Inferno device driver, we want	*/
	/*	all mprints to go to the circular buffers only, and	*/
	/*	not to stderr also.					*/
	/*								*/
	//SIM_VERBOSE = 0;
}

int
mfsize(int fd)
{
/*
	The kseek approach is much simpler, but I don't see
	what's buggered with this:
	
	char	buf[STATFIXLEN*4];
	Dir	d;

	kfstat(fd, &buf[0], STATFIXLEN*4);
	convM2D(&buf[0], STATFIXLEN*4, &d, nil);
	return d.length;
*/
	int	off, length;

	off = kseek(fd, 0, 1);
	length = kseek(fd, 0, 2);
	kseek(fd, off, 0);

	return length;
}

char *
mfgets(char *buf, int len, int fd)
{
	int 	n, i = 0;
	char	ch;
	
	if (len <= 0)
	{
		return NULL;
	}

	do
	{
		n = mread(fd, &ch, 1);
		if (n == 0)
		{
			if (i == 0)
				return NULL;
			else
				break;
		}

		buf[i++] = ch;
	} while ((i < len) && (ch != '\n'));
	buf[i] = '\0';

	
	return buf;
}

void
mprintfd(int fd, char* buf)
{
	if (kwrite(fd, buf, strlen(buf)) < 0)
	{
		fprint(2, "write of [%s] to fd [%d] failed in mprintfd (devsf)\n", buf, fd);
	}

	return;
}

int
mcreate(char *path, int omode)
{
	int	mode = 0;
	int	rw = omode & (~M_OTRUNCATE);
	int	fd;

	if (rw == M_OREAD)
		mode = OREAD;
	
	if (rw == M_OWRITE)
		mode = OWRITE;

	if (rw == (M_OREAD|M_OWRITE))
		mode = ORDWR;

	if (omode & M_OTRUNCATE)
		mode |= OTRUNC;


	fd = kcreate(path, mode, 0600);

	return fd;
}

/*
	TODO: there are many problems with our open/create management under
	inferno. Under slax linux liveCD, we cannot write to sunflower.out
	(we keep getting fd=0).
*/

int
mopen(char *path, int omode)
{
	int	mode = 0;
	int	rw = omode & (~M_OTRUNCATE);


	if (rw == M_OREAD)
		mode = OREAD;
	if (rw == M_OWRITE)
		mode = OWRITE;
	if (rw == (M_OREAD|M_OWRITE))
		mode = ORDWR;
	if (mode & M_OTRUNCATE)
		mode |= OTRUNC;
	

	return kopen(path, mode);
}

int
mread(int fd, char* buf, int len)
{
	return kread(fd, buf, len);
}

int
mclose(int fd)
{
	return kclose(fd);
}

int
mwrite(int fd, char* buf, int len)
{
	return kwrite(fd, buf, len);
}

int
mspawnscheduler(Engine *E)
{	
	E->sched_pid = devsfspawnscheduler(E);

	return E->sched_pid;
}

int
mkillscheduler(Engine *E)
{
	if (E->nodetach || !E->on)
	{
		return 0;
	}
		
	/*	Scheduler will exit cleanly when E->on is cleared	*/
	E->on = 0;

	return 0;
}

int
mchdir(char *path)
{
	return kchdir(path);
}

char*
mgetpwd(void)
{
	int fd = kopen(".", OREAD);
	return kfd2path(fd);
}

#ifdef EMU
ulong
musercputimeusecs(void)
{
	struct rusage 	r;

	getrusage(RUSAGE_SELF, &r);

	return (ulong)(r.ru_utime.tv_sec*1E6 + r.ru_utime.tv_usec);
}


ulong
mcputimeusecs(void)
{
	struct rusage 	r;

	getrusage(RUSAGE_SELF, &r);

	return (ulong)(r.ru_utime.tv_sec*1E6 + r.ru_utime.tv_usec +
		       r.ru_stime.tv_sec*1E6 + r.ru_stime.tv_usec);
}

ulong
mwallclockusecs(void)
{
	struct timeval 	t;
	gettimeofday(&t, NULL);

	return t.tv_usec;
}
#else
ulong
musercputimeusecs(void)
{
	return osusectime();
}

ulong
mcputimeusecs(void)
{
	return osusectime();
}

ulong
mwallclockusecs(void)
{
	return osusectime();
}
#endif


void
mexit(Engine *E, char *str, int status)
{
	extern void	cleanexit(int);

	print("\n\n\tExiting: %s\n", str);
	print("\tWriting all node information to sunflower.out\n\n");
	m_dumpall(E, E->logfilename, M_OWRITE, "Exit", "");

//	pexit(str, 0);
//	cleanexit(0);
	exit(0);
}

ulong
mtimeusecs(void)
{
	return osusectime();
}


/*
void*
mmalloc(int size, char *ID)
{
	return malloc(size);
}

void*
mcalloc(int nelem, int size, char *ID)
{
	return calloc(nelem, size);
}

void*
mrealloc(void *oldptr, int size, char *ID)
{
	return realloc(oldptr, size);
}

void
mfree(void *ptr)
{
	free(ptr);
	
	return;
}

void
mmblocksdisplay(void)
{
	return;
}
*/

tuck void
checkh2o(int maxbufsz, int *h2o, char *circbuf, char *buf)
{
	int	ndelete, newidx, n = strlen(buf) + 1;


	/*	Make sure NUL terminated string can fit in buffer	*/
	if (n > maxbufsz)
	{
		n = maxbufsz;
	}
	
	ndelete = *h2o - (maxbufsz - n);
	if (ndelete < 0)
	{
		ndelete = 0;
	}

	/*	Move old stuff in buffer backwards, discarding some	*/
	memmove(&circbuf[0], &circbuf[ndelete], *h2o - ndelete);

	newidx = max(0, *h2o - ndelete - 1);

	/*	Put new stuff in, overwriting terminating NUL in old	*/
	memmove(&circbuf[newidx], buf, n);

	*h2o = newidx + n;
	circbuf[*h2o] = '\0';
	//fprint(2, "h2o= [%d], circbuf = [%s]\n", *h2o, circbuf);

	return;
}

void
mlog(Engine *E, State *S, char *fmt, ...)
{
	char	*buf;
	va_list	arg;

	if (!SF_SIMLOG)
	{
		return;
	}

	/*	Should never happen, but just in case code changes elsewhere:	*/
	if (S->logfd < 0)
	{
		return;
	}

	buf = mcalloc(E, 1, MAX_MIO_BUFSZ, "(char *)buf in arch-Inferno.c");
	if (buf == NULL)
	{
		mexit(E, "Could not allocate memory for (char *)buf in arch-Inferno.c", -1);
	}

	va_start(arg, fmt);
	vseprint(buf, buf+MAX_MIO_BUFSZ, fmt, arg);
	va_end(arg);

	kwrite(S->logfd, buf, strlen(buf));
	mfree(E, buf, "(char *)buf in arch-Inferno.c");


	return;
}

int
msnprint(char *dst, int size, char *fmt, ...)
{
	va_list		arg;
	int		n;

	va_start(arg, fmt);
	n = vsnprint(dst, size, fmt, arg);
	va_end(arg);

	return n;
}

void
mprint(Engine *E, State *S, int out, char *fmt, ...)
{
	char		*buf;
	va_list		arg;


	if (!E->verbose && S != NULL)
	{
		return;
	}

	buf = mcalloc(E, 1, MAX_MIO_BUFSZ, "(char *)buf in arch-Inferno.c");
	if (buf == NULL)
	{
		mexit(E, "Could not allocate memory for (char *)buf in arch-Inferno.c", -1);
	}

	va_start(arg, fmt);
	vseprint(buf, buf+MAX_MIO_BUFSZ, fmt, arg);
	va_end(arg);

	/*	If no state structure is given, send to global output	*/
	if (S == NULL)
	{
		if (out != siminfo)
		{
			fprint(2, "%s\n", Ebadsfout);
			mfree(E, buf, "(char *)buf in arch-Inferno.c");

			return;
		}

		checkh2o(MAX_SIM_INFO_BUFSZ, &E->infoh2o, E->infobuf, buf);
		mfree(E, buf, "(char *)buf in arch-Inferno.c");

		return;
	}

	if (out == nodestdout)
	{
		checkh2o(MAX_NODESTDOUT_BUFSZ, &S->stdout_h2o, S->stdout_buf, buf);
	}
	else if (out == nodestderr)
	{
		checkh2o(MAX_NODESTDERR_BUFSZ, &S->stderr_h2o, S->stderr_buf, buf);
	}
	else if (out == nodeinfo)
	{
		checkh2o(MAX_NODEINFO_BUFSZ, &S->nodeinfo_h2o, S->nodeinfo_buf, buf);
	}
	else
	{
		fprint(2, "%s\n", Ebadsfout);
	}

	/*								*/
	/*	Wake up any proc sleeping on data available: See 	*/
	/*	Qoutputrdy in emu/port/devsf.c			*/
	/*								*/
//	wakeup();
	mfree(E, buf, "(char *)buf in arch-Inferno.c");

	return;
}

void
mnsleep(ulong nsecs)
{
#ifdef	_WIN32_WINNT
	osmillisleep(nsecs/1000000);
#else
	/*								*/
	/*	Inferno 3.0 doesn't provide us with enough granularity.	*/
	/*	This should still be fairly portable since nanosleep	*/
	/*	is POSIX.1b ... yeah, right.				*/
	/*								*/
	struct timespec rqtp;
	
	rqtp.tv_sec = nsecs/1000000000;
	rqtp.tv_nsec = nsecs % 1000000000;

	nanosleep(&rqtp, nil);
#endif
}

void
merror(Engine *E, char *fmt, ...)
{
	char	*buf;
	va_list	arg;


	buf = mcalloc(E, 1, MAX_MIO_BUFSZ, "(char *)buf in arch-Inferno.c");
	if (buf == NULL)
	{
		mexit(E, "Could not allocate memory for (char *)buf in arch-Inferno.c", -1);
	}

	va_start(arg, fmt);
	vseprint(buf, buf+MAX_MIO_BUFSZ, fmt, arg);
	va_end(arg);
 
	mprint(E, NULL, siminfo, "Error: %s\n", buf);
	mfree(E, buf, "(char *)buf in arch-Inferno.c");

	return;
}
