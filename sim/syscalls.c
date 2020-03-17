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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "sf.h"
#include "syscalls.h"
#include "mextern.h"

/*
 *	This actually does need to be after include of sf.h, since we get the
 *	SF_EMBEDDED define from there (via conf.h).
 */
#if (SF_EMBEDDED == 0)
#	include <sys/times.h>
#endif



static ulong	sys_write(Engine *E, State *, int, char *, int);
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
static ulong	sys_fstat(State *S, int fd, struct stat *st);

ulong
sim_syscall(Engine *E, State *S, ulong type, ulong arg1, ulong arg2, ulong arg3)
{
	switch (type)
	{
		case SYS_exit:
		{
#if (SF_EMBEDDED == 0)
			struct tms 	t;
#endif
			/*
			 *	TODO: should just call m_off and get rid of most of this crud
			 */
			E->verbose = 1;
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_exit\n");
			}

#if (SF_EMBEDDED == 0)
			times(&t);
#endif
			S->ufinish = musercputimeusecs();
			S->finishclk = S->ICLK;
			mprint(E, S, nodeinfo, "\n\nNODE %d exiting...\n", S->NODE_ID);
			mprint(E, S, nodeinfo, "User Time elapsed = %.6f seconds.\n",\
				(float)(S->ufinish - S->ustart)/1E6);

			if (E->quantum == 1)
			{
				mprint(E, S, nodeinfo, "Simulated CPU Time elapsed = %.6E seconds.\n",\
					S->TIME);
				mprint(E, S, nodeinfo, "Simulated Clock Cycles = " UVLONGFMT "\n", S->finishclk - S->startclk);
				mprint(E, NULL, siminfo, "Cycles Spent Waiting = " UVLONGFMT " (%.2f%%)\n",
					S->num_cycles_waiting, 100*((float)(S->num_cycles_waiting))/(float)(S->finishclk - S->startclk));

			}

			if (S->ufinish - S->ustart > 0)
			{
				mprint(E, S, nodeinfo,
					"Instruction Simulation Rate = %.2f Cycles/Second.\n",\
					(((float)(S->finishclk - S->startclk))/(((float)\
					(S->ufinish - S->ustart))/1E6)));
			}
			if (SF_POWER_ANALYSIS)
			{
				mprint(E, S, nodeinfo,
					"Estimated CPU-only Energy = %1.6E\n", S->energyinfo.CPUEtot);
			}

			mprint(E, S, nodeinfo, "\n\n");
			S->runnable = 0;
			E->on = 0;
			//mexit(E, "pip: exiting on Sys_exit", 0);

			break;
		}

		case SYS_fork:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_fork: NOT IMPLEMENTED!!!\n");
			}

			break;
		}

		case SYS_read:
		{
			if (SF_DEBUG)
			{
			     mprint(E, S, nodeinfo, "SYSCALL: SYS_read fd=0x" UHLONGFMT " ptr=0x" UHLONGFMT " len=0x" UHLONGFMT "\n",\
			     arg1, arg2, arg3);
			}
			return sys_read(S, (int)arg1, (char *)arg2, (int)arg3);
			break;
		}

		case SYS_write:
		{
			if (SF_DEBUG)
			{
			    mprint(E, S, nodeinfo, "SYSCALL: SYS_write fd=0x" UHLONGFMT " ptr=0x" UHLONGFMT " len=0x" UHLONGFMT "\n",\
				arg1, arg2, arg3);
			}
			return sys_write(E, S, (int)arg1, (char *)arg2, (int)arg3);
			break;
		}

		case SYS_open:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_open path=%s (" UHLONGFMT ") flags=" UHLONGFMT "\n",\
				&S->MEM[(ulong)arg1 - S->MEMBASE], arg1, arg2);
			}
			return sys_open(S, (const char *)arg1, (int)arg2);
			break;
		}

		case SYS_close:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_close fd=0x" UHLONGFMT " \n", arg1);
			}
			return sys_close(S, (int)arg1);
			break;
		}

		case SYS_wait4:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_wait4\n");
			}
			break;
		}

		case SYS_creat:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_creat path=" UHLONGFMT " mode=" UHLONGFMT "\n",\
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
				mprint(E, S, nodeinfo, "SYSCALL: SYS_link\n");
			}
			break;
		}

		case SYS_unlink:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_unlink\n");
			}
			break;
		}

		case SYS_execv:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_execv\n");
			}
			break;
		}

		case SYS_chdir:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_chdir\n");
			}
			break;
		}

		case SYS_mknod:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_mknod\n");
			}
			break;
		}

		case SYS_chmod:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_chmod path=0x" UHLONGFMT " mode=0x" UHLONGFMT "\n",\
				arg1, arg2);
			}
			return sys_chmod(S, (const char *)arg1, (short)arg2);
			break;
		}

		case SYS_chown:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo,
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
				mprint(E, S, nodeinfo,
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
				mprint(E, S, nodeinfo, "SYSCALL: SYS_getpid\n");
			}
			break;
		}

		case SYS_isatty:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_isatty\n");
			}
			break;
		}

		case SYS_fstat:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_fstat\n");
			}

			return sys_fstat(S, (int) arg1, (struct stat *) arg2);

			break;
		}

		case SYS_time:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_time\n");
			}
			break;
		}

		case SYS_ARG:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_ARG\n");
			}
			break;
		}

		case SYS_stat:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_stat path=0x" UHLONGFMT " st=0x" UHLONGFMT "\n",\
				arg1, arg2);
			}
			return sys_stat(S, (const char *)arg1, (struct stat *)arg2);
			break;
		}

		case SYS_pipe:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_pipe fd=0x" UHLONGFMT "\n", arg1);
			}
			return sys_pipe(S, (int *)arg1);
			break;
		}

		case SYS_execve:
		{
			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_execve\n");
			}
			break;
		}

		case SYS_utime:
		{
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_utime path=0x" UHLONGFMT " times=0x" UHLONGFMT "\n",\
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
				mprint(E, S, nodeinfo, "SYSCALL: SYS_wait\n");
			}
			break;
		}

		case SYS_brk:
		{
			ulong returnValue;

			/*	Not Implemented		*/
			if (SF_DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_brk: arg1: 0x%X, arg2: 0x%X, arg3: 0x%X, MemSize: 0x%X End: 0x%X Return: 0x%X\n", arg1, arg2, arg3, S->MEMSIZE, S->MEM_DATA_SEGMENT_END, returnValue);
			}

			if (arg1 == 0x0)
			{
				returnValue = S->MEM_DATA_SEGMENT_END;
			}
			else
			{
				returnValue = arg1;
			}

			return returnValue;

			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Node [%d] : Unknown SYSCALL [%ld]!!!\n",\
				 S->NODE_ID, type);
			//sfatal(S, "Unknown syscall. Exiting.");

			break;
		}
	}

	return -1;
}

ulong
riscv_sim_syscall(Engine *E, State *S, ulong type, ulong arg1, ulong arg2, ulong arg3)
{
	ulong superH_type;

	switch (type) {
		case RISCV_SYS_exit:
		{
			superH_type = SYS_exit;
			break;
		}

		case RISCV_SYS_read:
		{
			superH_type = SYS_read;
			break;
		}
		case RISCV_SYS_write:
		{
			superH_type = SYS_write;
			break;
		}
		case RISCV_SYS_open:
		{
			superH_type = SYS_open;
			break;
		}
		case RISCV_SYS_close:
		{
			superH_type = SYS_close;
			break;
		}
		case RISCV_SYS_link:
		{
			superH_type = SYS_link;
			break;
		}
		case RISCV_SYS_unlink:
		{
			superH_type = SYS_unlink;
			break;
		}
		case RISCV_SYS_chdir:
		{
			superH_type = SYS_chdir;
			break;
		}
		case RISCV_SYS_lseek:
		{
			superH_type = SYS_lseek;
			break;
		}
		case RISCV_SYS_getpid:
		{
			superH_type = SYS_getpid;
			break;
		}
		case RISCV_SYS_fstat:
		{
			superH_type = SYS_fstat;
			break;
		}
		case RISCV_SYS_time:
		{
			superH_type = SYS_time;
			break;
		}

		case RISCV_SYS_stat:
		{
			superH_type = SYS_stat;
			break;
		}

		case RISCV_SYS_pipe:
		{
			superH_type = SYS_pipe;
			break;
		}
		case RISCV_SYS_execve:
		{
			superH_type = SYS_execve;
			break;
		}
		case RISCV_SYS_brk:
		{
			superH_type = SYS_brk;
			break;
		}
		default:
		{
			superH_type = -1;
			break;
		}
	}

	if (superH_type == -1) 
	{
		mprint(E, S, nodeinfo, "Node [%d] : Unknown SYSCALL [%ld]!!!\n",\
				S->NODE_ID, type);
	} 
	else 
	{
		return sim_syscall(E, S, superH_type, arg1, arg2, arg3);
	}

	return -1;
}

ulong
sys_write(Engine *E, State *S, int fd, char *ptr, int len)
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
		mprint(E, S, nodestdout, "%.*s", len, &S->MEM[(ulong)ptr - S->MEMBASE]);
		return 0;
	}
	if (fd == 2)
	{
		mprint(E, S, nodestderr, "%.*s", len, &S->MEM[(ulong)ptr - S->MEMBASE]);
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
	return open((char*)&S->MEM[(ulong)path - S->MEMBASE], flags);
}

ulong
sys_close(State *S, int fd)
{
	/*	For now, just pass it on						*/
	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.							*/
	
	/*	Avoid closing fds 0,1,2 which are needed by sf	*/

	if (fd <= 2)
	{
		return 0;
	}
	else
	{
		return close(fd);
	}
}

ulong
sys_creat(State *S, const char *path, int mode)
{
	/*	For now, just pass it on	*/
	return creat((char*)&S->MEM[(ulong)path - S->MEMBASE], mode|S_IRWXU);
}

ulong
sys_chmod(State *S, const char *path, short mode)
{
	/*	For now, just pass it on	*/
#if (SF_EMBEDDED == 0)
	return chmod((char *)&S->MEM[(ulong)path - S->MEMBASE], mode);
#else
	return 0;
#endif
}

ulong
sys_chown(State *S, const char *path, short owner, short group)
{
	/*	For now, just pass it on	*/
#if (SF_EMBEDDED == 0)
	return chown((char *)&S->MEM[(ulong)path - S->MEMBASE], owner, group);
#else
	return 0;
#endif
}

ulong
sys_lseek(State *S, int fd, int offset, int whence)
{
	/*		For now, just pass it on		*/
	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.				*/

	return lseek(fd, offset, whence);
}

ulong
sys_stat(State *S, const char *path, struct stat *st)
{
	/*	For now, just pass it on	*/
	return stat((char *)&S->MEM[(ulong)path - S->MEMBASE], (struct stat *)&S->MEM[(ulong)st - S->MEMBASE]);
}

ulong
sys_fstat(State *S, int fd, struct stat *st)
{
	/*	For now, just pass it on	*/
	/*	fd should be correct because sf opened it	*/
	return fstat(fd, (struct stat *)&S->MEM[(ulong)st - S->MEMBASE]);
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
#if (SF_EMBEDDED == 0)
	return utime((char *)&S->MEM[(ulong)path - S->MEMBASE],\
		(const struct utimbuf *)&S->MEM[(ulong)times - S->MEMBASE]);
#else
	return 0;
#endif
}
