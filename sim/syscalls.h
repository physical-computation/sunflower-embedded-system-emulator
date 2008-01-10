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
};
