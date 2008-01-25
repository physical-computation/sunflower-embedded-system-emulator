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
#ifdef SF_L_ENDIAN
#	include "little-endian-hitachi-sh.h"
#	define SF_ENDONE
#endif

#ifdef SF_B_ENDIAN
#	include "big-endian-hitachi-sh.h"
#	define SF_ENDONE
#endif

#ifndef SF_ENDONE
#	error "You must define your host machine's endianness"
#	error "in config.$(OSTYPE)."
#	error "use -DSF_L_ENDIAN for little-endian machines"
#	error "and -DSF_B_ENDIAN for big-endian machines"
#	error "Files containing endian-dependent code are"
#	error "endian.h and regs.h"
#endif
