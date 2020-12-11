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
 * kbcore.h -- Structures for maintain the main models.
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
 * 11-Feb-2000	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Removed svqpp stuff.  It doesn't work too well anyway.
 * 
 * 06-Dec-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added kb_t.svqpp_t and related handling.
 * 
 * 30-Apr-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created.
 */


#ifndef _S3_KBCORE_H_
#define _S3_KBCORE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "libutil.h"
#include "feat.h"
#include "cont_mgau.h"
#include "subvq.h"
#include "mdef.h"
#include "dict.h"
#include "dict2pid.h"
#include "fillpen.h"
#include "lm.h"
#include "wid.h"
#include "tmat.h"


typedef struct {
    feat_t *fcb;
    mdef_t *mdef;
    dict_t *dict;
    dict2pid_t *dict2pid;
    lm_t *lm;
    fillpen_t *fillpen;
    s3lmwid_t *dict2lmwid;
    mgau_model_t *mgau;
    subvq_t *svq;
    tmat_t *tmat;
} kbcore_t;


/*
 * Initialize one or more of all the major models:  pronunciation dictionary, acoustic models,
 * language models.  Several arguments are optional (e.g., pointers may be NULL); however, they
 * may not all be independently so.  The logbase argument is required (i.e. must be valid).
 * A number of consistency verifications are carried out.  It's recommended that a reasonable
 * default be provided for any real or integer argument, even if it's unused.
 * Return value: (obvious) pointer to structure encapsulating the individual data bases.
 * NOTE: If any model fails to initialize, the call fails with a FATAL ERROR.
 */
kbcore_t *kbcore_init (float64 logbase,		/* Must be specified */
		       char *feattype,
		       char *cmn,
		       char *varnorm,
		       char *agc,
		       char *mdeffile,
		       char *dictfile,
		       char *fdictfile,
		       char *compsep,		/* Must be valid if dictfile specified */
		       char *lmfile,
		       char *fillpenfile,
		       float64 silprob,		/* Must be valid if lmfile/fillpenfile is
						   specified */
		       float64 fillprob,	/* Must be valid if lmfile/fillpenfile is
						   specified */
		       float64 langwt,		/* Must be valid if lmfile/fillpenfile is
						   specified. */
		       float64 inspen,		/* Must be valid if lmfile/fillpenfile is
						   specified. */
		       float64 uw,		/* Must be valid if lmfile/fillpenfile is
						   specified. */
		       char *meanfile,		/* Acoustic model... */
		       char *varfile,		/* Must be specified if meanfile specified */
		       float64 varfloor,	/* Must be valid if varfile specified */
		       char *mixwfile,		/* Must be specified if meanfile specified */
		       float64 mixwfloor,	/* Must be valid if mixwfile specified */
		       char *subvqfile,		/* Subvector quantized acoustic model
						   (quantized mean/var values), optional */
		       char *tmatfile,
		       float64 tmatfloor);	/* Must be valid if tmatfile specified */

  void kbcore_free (kbcore_t *kbcore);


/* Access macros; not meant for arbitrary use */
#define kbcore_fcb(k)		((k)->fcb)
#define kbcore_mdef(k)		((k)->mdef)
#define kbcore_dict(k)		((k)->dict)
#define kbcore_dict2pid(k)	((k)->dict2pid)
#define kbcore_lm(k)		((k)->lm)
#define kbcore_fillpen(k)	((k)->fillpen)
#define kbcore_dict2lmwid(k,w)	((k)->dict2lmwid[w])
#define kbcore_mgau(k)		((k)->mgau)
#define kbcore_svq(k)		((k)->svq)
#define kbcore_tmat(k)		((k)->tmat)

#ifdef __cplusplus
}
#endif

#endif
