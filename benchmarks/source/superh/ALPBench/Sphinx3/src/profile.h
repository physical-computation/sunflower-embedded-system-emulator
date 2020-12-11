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
 * profile.h -- For timing and event counting.
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
 * 11-Mar-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added ptmr_init().
 * 
 * 19-Jun-97	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created from earlier Sphinx-3 version.
 */


#ifndef _LIBUTIL_PROFILE_H_
#define _LIBUTIL_PROFILE_H_


#include "prim_type.h"


/*
 * Generic event counter for profiling.  User is responsible for allocating an array
 * of the desired number.  There should be a sentinel with name = NULL.
 */
typedef struct {
    const char *name;		/* Counter print name; NULL terminates array of counters.
				   Used by pctr_print_all */
    int32 count;		/* Counter value */
} pctr_t;


void pctr_reset (pctr_t *ctr);
void pctr_reset_all (pctr_t *ctr);
void pctr_print_all (FILE *fp, pctr_t *ctr);


/*
 * Generic timer structures and functions for coarse-grained performance measurements
 * using standard system calls.
 */
typedef struct {
    const char *name;		/* Timer print name; NULL terminates an array of timers.
				   Used by ptmr_print_all */
    float64 t_cpu;		/* CPU time accumulated since most recent reset op */
    float64 t_elapsed;		/* Elapsed time accumulated since most recent reset */
    float64 t_tot_cpu;		/* Total CPU time since creation */
    float64 t_tot_elapsed;	/* Total elapsed time since creation */
    float64 start_cpu;		/* ---- FOR INTERNAL USE ONLY ---- */
    float64 start_elapsed;	/* ---- FOR INTERNAL USE ONLY ---- */
} ptmr_t;


/* Start timing using tmr */
void ptmr_start (ptmr_t *tmr);

/* Stop timing and accumulate tmr->{t_cpu, t_elapsed, t_tot_cpu, t_tot_elapsed} */
void ptmr_stop (ptmr_t *tmr);

/* Reset tmr->{t_cpu, t_elapsed} to 0.0 */
void ptmr_reset (ptmr_t *tmr);

/* Reset tmr->{t_cpu, t_elapsed, t_tot_cpu, t_tot_elapsed} to 0.0 */
void ptmr_init (ptmr_t *tmr);


/*
 * Reset t_cpu, t_elapsed of all timer modules in array tmr[] to 0.0.
 * The array should be terminated with a sentinel with .name = NULL.
 */
void ptmr_reset_all (ptmr_t *tmr);

/*
 * Print t_cpu for all timer modules in tmr[], normalized by norm (i.e., t_cpu/norm).
 * The array should be terminated with a sentinel with .name = NULL.
 */
void ptmr_print_all (FILE *fp, ptmr_t *tmr, float64 norm);


/*
 * Return the processor clock speed (in MHz); only available on some machines (Alphas).
 * The dummy argument can be any integer value.
 */
int32 host_pclk (int32 dummy);


/*
 * Check the native byte-ordering of the machine by writing a magic number to a
 * temporary file and reading it back.
 * Return value: 0 if BIG-ENDIAN, 1 if LITTLE-ENDIAN, -1 if error.
 */
int32 host_endian ( void );


#endif
