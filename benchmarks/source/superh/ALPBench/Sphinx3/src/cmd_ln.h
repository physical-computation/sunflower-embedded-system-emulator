/*
 * 
 * This file is part of the ALPBench Benchmark Suite Version 1.0
 * 
 * Copyright (c) 2005 The Board of Trustees of the University of Illinois
 * 
 * All rights reserved.
 * 
 * ALPBench is a derivative of several codes, and restricted by licenses
 * for those codes, as indicated in the source files and the ALPBench
 * license at http://www.cs.uiuc.edu/alp/alpbench/alpbench-license.html
 * 
 * The multithreading and SSE2 modifications for SpeechRec, FaceRec,
 * MPEGenc, and MPEGdec were done by Man-Lap (Alex) Li and Ruchira
 * Sasanka as part of the ALP research project at the University of
 * Illinois at Urbana-Champaign (http://www.cs.uiuc.edu/alp/), directed
 * by Prof. Sarita V. Adve, Dr. Yen-Kuang Chen, and Dr. Eric Debes.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal with the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimers.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimers in the documentation and/or other materials provided
 *       with the distribution.
 * 
 *     * Neither the names of Professor Sarita Adve's research group, the
 *       University of Illinois at Urbana-Champaign, nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this Software without specific prior written permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
 * SOFTWARE.
 * 
 */


/* ====================================================================
 * Copyright (c) 1999-2001 Carnegie Mellon University.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*
 * cmd_ln.h -- Command line argument parsing.
 *
 * **********************************************
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1999 Carnegie Mellon University.
 * ALL RIGHTS RESERVED.
 * **********************************************
 * 
 * HISTORY
 * 
 * 15-Jul-1997	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added required arguments types.
 * 
 * 07-Dec-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created, based on Eric's implementation.  Basically, combined several
 *		functions into one, eliminated validation, and simplified the interface.
 */


#ifndef _LIBUTIL_CMD_LN_H_
#define _LIBUTIL_CMD_LN_H_


#include "prim_type.h"

#define ARG_REQUIRED	1

/* Arguments of these types are OPTIONAL */
#define ARG_INT32	2
#define ARG_FLOAT32	4
#define ARG_FLOAT64	6
#define ARG_STRING	8

/* Arguments of these types are REQUIRED */
#define REQARG_INT32	(ARG_INT32 | ARG_REQUIRED)
#define REQARG_FLOAT32	(ARG_FLOAT32 | ARG_REQUIRED)
#define REQARG_FLOAT64	(ARG_FLOAT64 | ARG_REQUIRED)
#define REQARG_STRING	(ARG_STRING | ARG_REQUIRED)
typedef int32 argtype_t;


typedef struct {
    char *name;		/* Name of the command line switch (case-insensitive) */
    argtype_t type;
    char *deflt;	/* Default value (as a printed string) or NULL if none */
    char *doc;		/* Documentation/description string */
} arg_t;


/*
 * Parse the given list of arguments (name-value pairs) according to the given definitions.
 * Argument values can be retrieved in future using cmd_ln_access().  argv[0] is assumed to be
 * the program name and skipped.  Any unknown argument name causes a fatal error.  The routine
 * also prints the prevailing argument values (to stderr) after parsing.
 * Return value: 0 if successful, -1 if error.
 */
int32 cmd_ln_parse (arg_t *defn,	/* In: Array of argument name definitions */
		    int32 argc,		/* In: #Actual arguments */
		    char *argv[]);	/* In: Actual arguments */


/*
 * Return a pointer to the previously parsed value for the given argument name.
 * The pointer should be cast to the appropriate type before use:
 * e.g., *((float32 *) cmd_ln_access ("-eps") to get the float32 argument named "-eps".
 * And, some convenient wrappers around this function.
 */
const void *cmd_ln_access (char *name);	/* In: Argument name whose value is sought */
#define cmd_ln_str(name)	((char *)cmd_ln_access(name))
#define cmd_ln_int32(name)	(*((int32 *)cmd_ln_access(name)))
#define cmd_ln_float32(name)	(*((float32 *)cmd_ln_access(name)))
#define cmd_ln_float64(name)	(*((float64 *)cmd_ln_access(name)))


/*
 * Print a help message listing the valid argument names, and the associated
 * attributes as given in defn.
 */
void  cmd_ln_print_help (FILE *fp,	/* In: File to which to print */
			 arg_t *defn);	/* In: Array of argument name definitions */

/* RAH, 4.17.01, call this to free memory allocated during cmd_ln_parse() */
void cmd_ln_free ();


#endif
