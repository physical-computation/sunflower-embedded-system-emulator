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

#include "config.h"

#ifndef LIBSF
#	include <stdio.h>
#	include <setjmp.h>
#	include <pthread.h>
#	include <sys/stat.h>
#	include <utime.h>
#	include <stdint.h>
#endif

#ifndef LIB9
#	include "sf-types.h"
#endif

/*
	we are yet to make the switch to using picoseconds for time
	#define	Picosec	unsigned long long
*/
#define	Picosec	double


#include <setjmp.h>
#include "merror.h"
#include "bit.h"
#include "listutils.h"
#include "parserlib.h"
#include "mmalloc.h"
#include "batt.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "dev7708.h"
#include "devsim7708.h"
#include "regs-hitachi-sh.h"
#include "regs-ti-msp430.h"
#include "regs-riscv.h"
#include "memory-hierarchy.h"
#include "decode-hitachi-sh.h"
#include "decode-ti-msp430.h"
#include "decode-riscv.h"
#include "power.h"
#include "pipeline-hitachi-sh.h"
#include "pipeline-ti-msp430.h"
#include "pipeline-riscv.h"
#include "pau.h"
#include "fault.h"
#include "network-hitachi-sh.h"
#include "pic.h"
#include "interrupts-ti-msp430.h"
#include "taint.h"
#include "uncertain_upe.h"
#include "uncertain-histogram.h"
#include "machine-hitachi-sh.h"
#include "machine-riscv.h"
#include "dev430x1xx.h"
#include "machine-ti-msp430.h"
#include "main.h"
#include "mfns.h"
#include "randgen.h"
