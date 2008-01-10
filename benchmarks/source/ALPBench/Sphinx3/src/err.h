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
 * err.h -- Package for checking and catching common errors, printing out
 *		errors nicely, etc.
 *
 * **********************************************
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1999 Carnegie Mellon University.
 * ALL RIGHTS RESERVED.
 * **********************************************
 *
 * 6/01/95  Paul Placeway  CMU speech group
 */


#ifndef _LIBUTIL_ERR_H_
#define _LIBUTIL_ERR_H_

/* 01.18.01 RAH, allow for C++ compiles */
#ifdef __cplusplus
extern "C" {
#endif



#include <stdarg.h>
#include <errno.h>

void _E__pr_header( char const *file, long line, char const *msg );
void _E__pr_info_header( char const *file, long line, char const *tag );
void _E__pr_warn( char const *fmt, ... );
void _E__pr_info( char const *fmt, ... );
void _E__die_error( char const *fmt, ... );
void _E__abort_error( char const *fmt, ... );
void _E__sys_error( char const *fmt, ... );
void _E__fatal_sys_error( char const *fmt, ... );

/* These three all abort */

/* core dump after error message */
#ifndef E_ABORT
#define E_ABORT  _E__pr_header(__FILE__, __LINE__, "ERROR"),_E__abort_error
#endif

/* exit with non-zero status after error message */
#define E_FATAL  _E__pr_header(__FILE__, __LINE__, "FATAL_ERROR"),_E__die_error

/* Print error text; Call perror(""); exit(errno); */
#define E_FATAL_SYSTEM	_E__pr_header(__FILE__, __LINE__, "SYSTEM_ERROR"),_E__fatal_sys_error

/* Print error text; Call perror(""); */
#define E_WARN_SYSTEM	_E__pr_header(__FILE__, __LINE__, "SYSTEM_ERROR"),_E__sys_error

/* Print error text; Call perror(""); */
#define E_ERROR_SYSTEM	_E__pr_header(__FILE__, __LINE__, "SYSTEM_ERROR"),_E__sys_error


/* Print logging information, warnings, or error messages; all to stderr */
#define E_INFO	  _E__pr_info_header(__FILE__, __LINE__, "INFO"),_E__pr_info

#define E_WARN	  _E__pr_header(__FILE__, __LINE__, "WARNING"),_E__pr_warn

#define E_ERROR	  _E__pr_header(__FILE__, __LINE__, "ERROR"),_E__pr_warn


#ifdef __cplusplus
}
#endif


#endif /* !_ERR_H */


