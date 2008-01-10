/*										*/
/*		Copyright (C) 1999-2006 Phillip Stanley-Marbell.		*/
/*										*/
/*	You may redistribute and/or copy the source and binary			*/
/*	versions of this software provided:					*/
/*										*/
/*	1.	The source retains the copyright notice above, 			*/
/*		this list of conditions, and the following 			*/
/*		disclaimer.							*/
/*										*/
/*	2.	Redistributed binaries must reproduce the above			*/
/*		copyright notice, this list of conditions, and			*/
/*		the following disclaimer.					*/
/*										*/
/*	3.	The source and binaries are used for non-commercial 		*/
/*		purposes only.							*/
/*										*/
/*	4.	Permission is granted to all others, to redistribute		*/
/*		or make copies of the source or binaries, either		*/
/*		as it was received or with modifications, provided		*/
/*		the above three conditions are enforced and 			*/
/*		preserved, and that permission is granted for 			*/
/*		further redistribution, under the conditions of this		*/
/*		document.							*/
/*										*/
/*	The source is provided "as is", and no warranty is either		*/
/*	implied or explicitly granted. The authors will not be liable 		*/
/*	for any damages arising in any way out of the use or misuse 		*/
/*	of this software, even if advised of the possibility of such 		*/
/*	damage.									*/
/*										*/
/*	Contact: phillip Stanley-Marbell <pstanley@ece.cmu.edu>			*/
/*										*/	
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "sf.h"
#include "syscalls.h"
#include "mextern.h"

static ulong	sys_write(State *, int, char *, int);
static ulong	sys_read(State *, int, void *, int);
static ulong	sys_open(State *, const char *, int);
static ulong	sys_close(State *, int);
static ulong	sys_creat(State *, const char *, int);
static ulong	sys_chmod(State *S, const char *, short);
static ulong	sys_chown(State *S, const char *, short, short);
static ulong	sys_lseek(State *S, int, int, int);
static ulong	sys_stat(State *S, const char *, struct stat *);
static ulong	sys_pipe(State *S, int *);
static ulong	sys_utime(State *S, const char *, const struct utimbuf *);

ulong
sim_syscall(State *S, ulong type, ulong arg1, ulong arg2, ulong arg3)
{
	switch (type)
	{
		case SYS_exit:
		{
			struct tms 	t;

		//TODO: should just call m_off and get rid of most of this crud
			SIM_VERBOSE = 1;
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_exit\n");
			}

			times(&t);
			S->ufinish = musercputimeusecs();
			S->finishclk = S->ICLK;
			mprint(S, nodeinfo, "\n\nNODE %d exiting...\n", S->NODE_ID);
			mprint(S, nodeinfo, "User Time elapsed = %.6f seconds.\n",\
				(float)(S->ufinish - S->ustart)/1E6);

			if (SIM_QUANTUM == 1)
			{
				mprint(S, nodeinfo, "Simulated CPU Time elapsed = %.6E seconds.\n",\
					S->TIME);
				mprint(S, nodeinfo, "Simulated Clock Cycles = " UVLONGFMT "\n", S->finishclk - S->startclk);
			}

			if (S->ufinish - S->ustart > 0)
			{
				mprint(S, nodeinfo,
					"Instruction Simulation Rate = %.2f Cycles/Second.\n",\
					(((float)(S->finishclk - S->startclk))/(((float)\
					(S->ufinish - S->ustart))/1E6)));
			}
			if (SF_POWER_ANALYSIS)
			{
				mprint(S, nodeinfo,
					"Estimated CPU-only Energy = %1.6E\n", S->E.CPUEtot);
			}
				
			mprint(S, nodeinfo, "\n\n");
			S->runnable = 0;
			SIM_ON = 0;
mexit("pip: exiting on Sys_exit", 0);

			break;
		}

		case SYS_fork:
		{
			/*	Not Implemented		*/
			//if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_fork: NOT IMPLEMENTED!!!\n");
			}

			break;
		}

		case SYS_read:
		{
			if (SF_DEBUG)
			{
			     mprint(S, nodeinfo, "SYSCALL: SYS_read fd=0x" UHLONGFMT " ptr=0x" UHLONGFMT " len=0x" UHLONGFMT "\n",\
			     arg1, arg2, arg3);
			}
			return sys_read(S, (int)arg1, (char *)arg2, (int)arg3);
			break;
		}

		case SYS_write:
		{
			if (SF_DEBUG)
			{
			    mprint(S, nodeinfo, "SYSCALL: SYS_write fd=0x" UHLONGFMT " ptr=0x" UHLONGFMT " len=0x" UHLONGFMT "\n",\
				arg1, arg2, arg3);
			}
			return sys_write(S, (int)arg1, (char *)arg2, (int)arg3);
			break;
		}

		case SYS_open:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_open path=%s (" UHLONGFMT ") flags=" UHLONGFMT "\n",\
				&S->MEM[(ulong)arg1 - S->MEMBASE], arg1, arg2);
			}
			return sys_open(S, (const char *)arg1, (int)arg2); 
			break;
		}

		case SYS_close:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_close fd=0x" UHLONGFMT " \n", arg1);
			}
			return sys_close(S, (int)arg1);
			break;
		}

		case SYS_wait4:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_wait4\n");
			}
			break;
		}

		case SYS_creat:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_creat path=" UHLONGFMT " mode=" UHLONGFMT "\n",\
				arg1, arg2);
			}
			return sys_creat(S, (const char *)arg1, (int)arg2);
			break;
		}

		case SYS_link:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_link\n");
			}
			break;
		}

		case SYS_unlink:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_unlink\n");
			}
			break;
		}

		case SYS_execv:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_execv\n");
			}
			break;
		}

		case SYS_chdir:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_chdir\n");
			}
			break;
		}

		case SYS_mknod:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_mknod\n");
			}
			break;
		}

		case SYS_chmod:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_chmod path=0x" UHLONGFMT " mode=0x" UHLONGFMT "\n",\
				arg1, arg2);
			}
			return sys_chmod(S, (const char *)arg1, (short)arg2);
			break;
		}

		case SYS_chown:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo,
				"SYSCALL: SYS_chown path=0x" UHLONGFMT " owner=0x" UHLONGFMT " grp=0x" UHLONGFMT "\n",\
				arg1, arg2, arg3);
			}
			return sys_chown(S, (const char *)arg1, (short)arg2, (short)arg3);
			break;
		}

		case SYS_lseek:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo,
				"SYSCALL: SYS_lseek file=0x" UHLONGFMT " ptr=0x" UHLONGFMT " dir=0x" UHLONGFMT "\n",\
				arg1, arg2, arg3);
			}
			return sys_lseek(S, (int)arg1, (int)arg2, (int)arg3);
			break;
		}

		case SYS_getpid:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_getpid\n");
			}
			break;
		}

		case SYS_isatty:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_isatty\n");
			}
			break;
		}

		case SYS_fstat:
		{
			/*	Handled in newlib's syscalls.c		*/
			mprint(S, nodeinfo, "SYSCALL: SYS_fstat\n");
			break;
		}

		case SYS_time:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_time\n");
			}
			break;
		}

		case SYS_ARG:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_ARG\n");
			}
			break;
		}

		case SYS_stat:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_stat path=0x" UHLONGFMT " st=0x" UHLONGFMT "\n",\
				arg1, arg2);
			}
			return sys_stat(S, (const char *)arg1, (struct stat *)arg2);
			break;
		}

		case SYS_pipe:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_pipe fd=0x" UHLONGFMT "\n", arg1);
			}
			return sys_pipe(S, (int *)arg1);
			break;
		}

		case SYS_execve:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_execve\n");
			}
			break;
		}

		case SYS_utime:
		{
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_utime path=0x" UHLONGFMT " times=0x" UHLONGFMT "\n",\
				arg1, arg2);
			}
			return sys_utime(S, (const char *)arg1, (const struct utimbuf *)arg2);
			break;
		}

		case SYS_wait:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(S, nodeinfo, "SYSCALL: SYS_wait\n");
			}
			break;
		}

		default:
		{
			mprint(S, nodeinfo, "Node [%d] : Unknown SYSCALL [%ld]!!!\n",\
				 S->NODE_ID, type);
			//sfatal(S, "Unknown syscall. Exiting.");

			break;
		}
	}

	return -1;
} 

ulong
sys_write(State *S, int fd, char *ptr, int len)
{
	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.				*/

	/*							*/
	/*	Capture stdin and stdout to per-node buffers.	*/
	/*	Since the (sub)string in the node's memory	*/
	/*	is not necessarily a complete NUL terminated	*/
	/*	string, we use the %.*s verb to only print as 	*/
	/*	many chars as the write()'s 'len'.		*/
	/*							*/
	/*	1. This unfortunately will not catch dup(2)'s.	*/
	/*	2. This wouldnt work if strings were sequences	*/
	/*	of Unicode that are handled in UTF-8 encoding	*/
	/*							*/
	if (fd == 1)
	{
		mprint(S, nodestdout, "%.*s", len, &S->MEM[(ulong)ptr - S->MEMBASE]);
		return 0;
	}
	if (fd == 2)
	{
		mprint(S, nodestderr, "%.*s", len, &S->MEM[(ulong)ptr - S->MEMBASE]);
		return 0;
	}

	return write(fd, &S->MEM[(ulong)ptr - S->MEMBASE], len);
}

ulong
sys_read(State *S, int fd, void *buf, int len)
{
	/*	For now, just pass it on	*/
	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.				*/

	return read(fd, &S->MEM[(ulong)buf - S->MEMBASE], len);
}

ulong
sys_open(State *S, const char *path, int flags)
{
	/*	For now, just pass it on	*/
	return open(&S->MEM[(ulong)path - S->MEMBASE], flags);
}

ulong
sys_close(State *S, int fd)
{
	/*	For now, just pass it on	*/
	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.				*/

	return close(fd);
}

ulong
sys_creat(State *S, const char *path, int mode)
{
	/*	For now, just pass it on	*/
	return creat(&S->MEM[(ulong)path - S->MEMBASE], mode|S_IRWXU);
}

ulong
sys_chmod(State *S, const char *path, short mode)
{
	/*	For now, just pass it on	*/
	return chmod(&S->MEM[(ulong)path - S->MEMBASE], mode);
}

ulong
sys_chown(State *S, const char *path, short owner, short group)
{
	/*	For now, just pass it on	*/
	return chown(&S->MEM[(ulong)path - S->MEMBASE], owner, group);
}

ulong
sys_lseek(State *S, int fd, int offset, int whence)
{
	/*	For now, just pass it on	*/
	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.				*/

	return lseek(fd, offset, whence);
}

ulong
sys_stat(State *S, const char *path, struct stat *st)
{
/*	BUG: detected 01/23:
	since the host is writing directly into simulated
	machine's memory, the endianness of the host
	will affect the layout of the structure, e.g.
	the uid field will be read instead of the st_size
	field (as was the case in the manifestation)
*/
	/*	For now, just pass it on	*/
	return stat(&S->MEM[(ulong)path - S->MEMBASE], (struct stat *)&S->MEM[(ulong)st - S->MEMBASE]);
}

ulong
sys_pipe(State *S, int *fd)
{
	/*	For now, just pass it on	*/
	return pipe((int *)&S->MEM[(ulong)fd - S->MEMBASE]);
}

ulong
sys_utime(State *S, const char *path, const struct utimbuf *times)
{
	/*	For now, just pass it on	*/
	return utime(&S->MEM[(ulong)path - S->MEMBASE],\
		(const struct utimbuf *)&S->MEM[(ulong)times - S->MEMBASE]);
}
