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
#include "config.h"

#ifndef LIBSF
#	include <stdio.h>
#	include <setjmp.h>
#	include <pthread.h>
#	include <sys/stat.h>
#	include <utime.h>
#endif

#ifndef LIB9
#	include "e-types.h"
#endif

#include <setjmp.h>
#include "merror.h"
#include "bit.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "dev7708.h"
#include "devsim7708.h"
#include "regs-hitachi-sh.h"
#include "regs-ti-msp430.h"
#include "cache-hitachi-sh.h"
#include "decode-hitachi-sh.h"
#include "decode-ti-msp430.h"
#include "power.h"
#include "pipeline-hitachi-sh.h"
#include "pipeline-ti-msp430.h"
#include "pau.h"
#include "fault.h"
#include "network-hitachi-sh.h"
#include "pic.h"
#include "interrupts-ti-msp430.h"
#include "machine-hitachi-sh.h"
#include "dev430x1xx.h"
#include "machine-ti-msp430.h"
#include "main.h"
#include "batt.h"
#include "mfns.h"
#include "parserlib.h"
