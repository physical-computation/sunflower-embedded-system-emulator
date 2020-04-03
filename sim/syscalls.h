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

/*									*/
/*	  Based on newlib-1.9.0/newlib/libc/sys/sh/sys/syscall.h	*/
/*									*/
enum
{
	SYS_exit	= 1,
	SYS_fork	= 2,

	SYS_read	= 3,
	SYS_write	= 4,
	SYS_open	= 5,
	SYS_close	= 6,
	SYS_wait4	= 7,
	SYS_creat	= 8,
	SYS_link	= 9,
	SYS_unlink	= 10,
	SYS_execv	= 11,
	SYS_chdir	= 12,
	SYS_mknod	= 14,
	SYS_chmod	= 15,
	SYS_chown	= 16,
	SYS_lseek	= 19,
	SYS_getpid	= 20,
	SYS_isatty	= 21,
	SYS_fstat	= 22,
	SYS_time	= 23,

	SYS_ARG		= 24,
	SYS_stat	= 38,

	SYS_pipe	= 42,
	SYS_execve	= 59,

	SYS_utime	= 201,
	SYS_wait	= 202,

	SYS_brk		= 214, /* Not sure this is correct for Hitachi */
};

/*
 * Based on newlib-2.5.0.20170922/libgloss/riscv/machine/syscall.h
 *
 * Only syscalls that are defined in that header *and* that sunflower
 * is able to handle are listed here.
 */
enum
{
	RISCV_SYS_exit	= 93,

	RISCV_SYS_read	= 63,
	RISCV_SYS_write	= 64,
	RISCV_SYS_open	= 1024,
	RISCV_SYS_close	= 57,
	RISCV_SYS_link	= 1025,
	RISCV_SYS_unlink	= 1026,
	RISCV_SYS_chdir	= 49,
	RISCV_SYS_lseek	= 62,
	RISCV_SYS_getpid	= 172,
	RISCV_SYS_fstat	= 80,
	RISCV_SYS_time	= 1062,

	RISCV_SYS_stat	= 1038,

	RISCV_SYS_pipe	= 42,
	RISCV_SYS_execve	= 59,

	RISCV_SYS_brk	= 214,
};
