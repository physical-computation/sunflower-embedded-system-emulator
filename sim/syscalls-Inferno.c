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

#include "lib9.h"
#include "kernel.h"
#include "sf.h"
#include "syscalls.h"
#include "mextern.h"


static ulong	sys_write(Engine *, State *, int, char *, int);
static ulong	sys_read(State *, int, void *, int);
static ulong	sys_open(Engine *, State *, const char *, int);
static ulong	sys_close(State *, int);
static ulong	sys_creat(State *, const char *, int);
static ulong	sys_chmod(State *S, const char *, short);
static ulong	sys_chown(State *S, const char *, short, short);
static ulong	sys_lseek(State *S, int, int, int);
//static ulong	sys_stat(State *S, const char *, struct stat *);
static ulong	sys_pipe(State *S, int *);
//static ulong	sys_utime(State *S, const char *, const struct utimbuf *);

int	DEBUG = 0;

ulong
sim_syscall(Engine *E, State *S, ulong type, ulong arg1, ulong arg2, ulong arg3)
{
	switch (type)
	{
		case SYS_exit:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_exit\n");
			}

			S->ufinish = musercputimeusecs();
			S->finishclk = S->ICLK;
			mprint(E, S, nodeinfo, "Simulated Clock Cycles = " UVLONGFMT ".\n",
				S->finishclk - S->startclk);
			mprint(E, NULL, siminfo, "Cycles Spent Waiting = " UVLONGFMT " (%.2f%)\n",
				S->num_cycles_waiting, 100*((float)(S->num_cycles_waiting))/(float)(S->finishclk - S->startclk));
			mprint(E, S, nodeinfo, "User Time elapsed = %.6f seconds.\n",
				((float)(S->ufinish - S->ustart))/1E6);
			mprint(E, S, nodeinfo,
				"Instruction Simulation Rate = %.6f K Cycles/Second.\n",
				(((float)((ulong)S->finishclk - (ulong)S->startclk))/(((float)
				(S->ufinish - S->ustart))/1E6))/1E3);
			if (SF_POWER_ANALYSIS)
			{
				mprint(E, S, nodeinfo,
					"Estimated CPU-only Energy = %.6LE\n", S->energyinfo.CPUEtot);
			}
				
			mprint(E, S, nodeinfo, "\n\n");
			S->runnable = 0;
			E->on = 0;

			break;
		}

		case SYS_fork:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_fork\n");
			}

			break;
		}

		case SYS_read:
		{
			if (DEBUG)
			{
			     mprint(E, S, nodeinfo, "SYSCALL: SYS_read fd=0x" UHLONGFMT " ptr=0x" UHLONGFMT " len=0x" UHLONGFMT "\n",
			     arg1, arg2, arg3);
			}
			return sys_read(S, (int)arg1, (char *)arg2, (int)arg3);

			break;
		}

		case SYS_write:
		{
			if (DEBUG)
			{
			    mprint(E, S, nodeinfo, "SYSCALL: SYS_write fd=0x" UHLONGFMT " ptr=0x" UHLONGFMT " len=0x" UHLONGFMT "\n",
				arg1, arg2, arg3);
			}
			return sys_write(E, S, (int)arg1, (char *)arg2, (int)arg3);

			break;
		}

		case SYS_open:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_open path=0x" UHLONGFMT " flags=0x" UHLONGFMT "\n",
				arg1, arg2);
			}
			return sys_open(E, S, (const char *)arg1, (int)arg2);

			break;
		}

		case SYS_close:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_close fd=0x" UHLONGFMT " \n", arg1);
			}
			return sys_close(S, (int)arg1);

			break;
		}

		case SYS_wait4:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_wait4\n");
			}

			break;
		}

		case SYS_creat:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_creat path=0x" UHLONGFMT " mode=0x" UHLONGFMT "\n",
				arg1, arg2);
			}
			return sys_creat(S, (const char *)arg1, (int)arg2);

			break;
		}

		case SYS_link:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_link\n");
			}

			break;
		}

		case SYS_unlink:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_unlink\n");
			}

			break;
		}

		case SYS_execv:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_execv\n");
			}

			break;
		}

		case SYS_chdir:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_chdir\n");
			}

			break;
		}

		case SYS_mknod:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_mknod\n");
			}

			break;
		}

		case SYS_chmod:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_chmod path=0x" UHLONGFMT " mode=0x" UHLONGFMT "\n",
				arg1, arg2);
			}
			return sys_chmod(S, (const char *)arg1, (short)arg2);

			break;
		}

		case SYS_chown:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo,
				"SYSCALL: SYS_chown path=0x" UHLONGFMT " owner=0x" UHLONGFMT " grp=0x" UHLONGFMT "\n",
				arg1, arg2, arg3);
			}
			return sys_chown(S, (const char *)arg1, (short)arg2, (short)arg3);

			break;
		}

		case SYS_lseek:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo,
				"SYSCALL: SYS_lseek file=0x" UHLONGFMT " ptr=0x" UHLONGFMT " dir=0x" UHLONGFMT "\n",
				arg1, arg2, arg3);
			}
			return sys_lseek(S, (int)arg1, (int)arg2, (int)arg3);

			break;
		}

		case SYS_getpid:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_getpid\n");
			}
			break;
		}

		case SYS_isatty:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_isatty\n");
			}

			break;
		}

		case SYS_fstat:
		{
			/*	Handled in newlib's syscalls.c		*/
			mprint(E, S, nodeinfo, "SYSCALL: SYS_fstat\n");

			break;
		}

		case SYS_time:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_time\n");
			}

			break;
		}

		case SYS_ARG:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_ARG\n");
			}

			break;
		}

		case SYS_stat:
		{
			if (DEBUG)
			{
				/*mprint(E, S, nodeinfo, "SYSCALL: SYS_stat path=0x" UHLONGFMT " st=0x" UHLONGFMT "\n",
				arg1, arg2);*/
			}
			//return sys_stat(S, (const char *)arg1, (struct stat *)arg2);

			break;
		}

		case SYS_pipe:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_pipe fd=0x" UHLONGFMT "\n", arg1);
			}
			return sys_pipe(S, (int *)arg1);

			break;
		}

		case SYS_execve:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_execve\n");
			}
			break;
		}

		case SYS_utime:
		{
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_utime path=0x" UHLONGFMT " times=0x" UHLONGFMT "\n",
				arg1, arg2);
			}
			//return sys_utime(S, (const char *)arg1, (const struct utimbuf *)arg2);

			break;
		}

		case SYS_wait:
		{
			/*	Not Implemented		*/
			if (DEBUG)
			{
				mprint(E, S, nodeinfo, "SYSCALL: SYS_wait\n");
			}

			break;
		}

		default:
		{
			mprint(E, S, nodeinfo, "Unknown SYSCALL [%ld]!!!\n", type);

			break;
		}
	}

	return -1;
} 

ulong
sys_write(Engine *E, State *S, int fd, char *ptr, int len)
{
	/*	This unfortunately will not catch dup(2)'s	*/

	/*	For sys_read, sys_write, sys_close, sys_lseek,	*/
	/*	we should maintain a table of fd's we've opened	*/
	/*	and return -1 if sim app is trying to access an	*/
	/*	fd we haven't opened.				*/
	if (fd == 1)
	{
		mprint(E, S, nodestdout, "%s", &S->MEM[(ulong)ptr - S->MEMBASE]);
		return 0;
	}
	else if (fd == 2)
	{
		mprint(E, S, nodestderr, "%s", &S->MEM[(ulong)ptr - S->MEMBASE]);
		return 0;
	}

	return kwrite(fd, &S->MEM[(ulong)ptr - S->MEMBASE], len);
}

ulong
sys_read(State *S, int fd, void *buf, int len)
{
	/*	For now, just pass it on	*/
	return kread(fd, &S->MEM[(ulong)buf - S->MEMBASE], len);
}

ulong
sys_open(Engine *E, State *S, const char *path, int flags)
{
	/*	not complete, and a major hack... we dont even use supplied flags!	*/

	/*	If O_CREATE bit is set, call kcreate	*/
	if (flags & 0x0200)
	{
		mprint(E, S, nodeinfo, "O_CREATE bit is set, calling kcreate.\n");
		if (kcreate((char *)&S->MEM[(ulong)path - S->MEMBASE], ORDWR, 0600) < 0)
		{
			mprint(E, S, nodeinfo, "kcreate failed.\n");
		
			return -1;
		}
		return kopen((char *)&S->MEM[(ulong)path - S->MEMBASE], ORDWR);

	}

	return kopen((char *)&S->MEM[(ulong)path - S->MEMBASE], ORDWR);
}

ulong
sys_close(State *S, int fd)
{
	/*	For now, just pass it on	*/
	return kclose(fd);
}

ulong
sys_creat(State *S, const char *path, int mode)
{
	/*	For now, just pass it on	*/
	return kcreate((char *)&S->MEM[(ulong)path - S->MEMBASE], mode, 0600);
}

ulong
sys_chmod(State *S, const char *path, short mode)
{
	/*	For now, just pass it on	*/
	//return chmod(&S->MEM[(ulong)path - S->MEMBASE], mode);
	return -1;
}

ulong
sys_chown(State *S, const char *path, short owner, short group)
{
	/*	For now, just pass it on	*/
	//return chown(&S->MEM[(ulong)path - S->MEMBASE], owner, group);
	return -1;
}

ulong
sys_lseek(State *S, int fd, int offset, int whence)
{
	/*	For now, just pass it on	*/
	return kseek(fd, offset, whence);
}
/*
ulong
sys_stat(State *S, const char *path, struct stat *st)
{
	//	For now, just pass it on	
	//return stat(&S->MEM[(ulong)path - S->MEMBASE], (struct stat *)&S->MEM[(ulong)st - S->MEMBASE]);
	return -1;
}
*/

ulong
sys_pipe(State *S, int *fd)
{
	/*	For now, just pass it on	*/
	//return pipe((int *)&S->MEM[(ulong)fd - S->MEMBASE]);
	return -1;
}
/*
ulong
sys_utime(State *S, const char *path, const struct utimbuf *times)
{
	//	For now, just pass it on	
	return utime(&S->MEM[(ulong)path - S->MEMBASE],
		(const struct utimbuf *)&S->MEM[(ulong)times - S->MEMBASE]);
}
*/
