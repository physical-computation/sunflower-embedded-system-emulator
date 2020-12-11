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
 * glist.h -- Module for maintaining a generic, linear linked-list structure.
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
 * 09-Mar-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added glist_chkdup_*().
 * 
 * 13-Feb-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created from earlier version.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "glist.h"
#include "ckd_alloc.h"


glist_t glist_add_ptr (glist_t g, void *ptr)
{
    gnode_t *gn;
    
    gn = (gnode_t *) mymalloc (sizeof(gnode_t));
    gn->data.ptr = ptr;
    gn->next = g;
    return ((glist_t) gn);	/* Return the new head of the list */
}


glist_t glist_add_int32 (glist_t g, int32 val)
{
    gnode_t *gn;
    
    gn = (gnode_t *) mymalloc (sizeof(gnode_t));
    gn->data.int32 = val;
    gn->next = g;
    return ((glist_t) gn);	/* Return the new head of the list */
}


glist_t glist_add_uint32 (glist_t g, uint32 val)
{
    gnode_t *gn;
    
    gn = (gnode_t *) mymalloc (sizeof(gnode_t));
    gn->data.uint32 = val;
    gn->next = g;
    return ((glist_t) gn);	/* Return the new head of the list */
}


glist_t glist_add_float32 (glist_t g, float32 val)
{
    gnode_t *gn;
    
    gn = (gnode_t *) mymalloc (sizeof(gnode_t));
    gn->data.float32 = val;
    gn->next = g;
    return ((glist_t) gn);	/* Return the new head of the list */
}


glist_t glist_add_float64 (glist_t g, float64 val)
{
    gnode_t *gn;
    
    gn = (gnode_t *) mymalloc (sizeof(gnode_t));
    gn->data.float64 = val;
    gn->next = g;
    return ((glist_t) gn);	/* Return the new head of the list */
}


int32 glist_chkdup_ptr (glist_t g, void *val)
{
    gnode_t *gn;

    for (gn = g; gn; gn = gnode_next(gn))
	if (gnode_ptr(gn) == val)
	    return 1;
    
    return 0;
}


int32 glist_chkdup_int32 (glist_t g, int32 val)
{
    gnode_t *gn;

    for (gn = g; gn; gn = gnode_next(gn))
	if (gnode_int32(gn) == val)
	    return 1;
    
    return 0;
}


int32 glist_chkdup_uint32 (glist_t g, uint32 val)
{
    gnode_t *gn;

    for (gn = g; gn; gn = gnode_next(gn))
	if (gnode_uint32(gn) == val)
	    return 1;
    
    return 0;
}


int32 glist_chkdup_float32 (glist_t g, float32 val)
{
    gnode_t *gn;

    for (gn = g; gn; gn = gnode_next(gn))
	if (gnode_float32(gn) == val)
	    return 1;
    
    return 0;
}


int32 glist_chkdup_float64 (glist_t g, float64 val)
{
    gnode_t *gn;

    for (gn = g; gn; gn = gnode_next(gn))
	if (gnode_float64(gn) == val)
	    return 1;
    
    return 0;
}


void glist_apply_ptr (glist_t g, void (*func)(void *))
{
    gnode_t *gn;
    
    for (gn = g; gn; gn = gn->next)
	(*func)(gn->data.ptr);
}


void glist_apply_int32 (glist_t g, void (*func)(int32))
{
    gnode_t *gn;
    
    for (gn = g; gn; gn = gn->next)
	(*func)(gn->data.int32);
}


void glist_apply_uint32 (glist_t g, void (*func)(uint32))
{
    gnode_t *gn;
    
    for (gn = g; gn; gn = gn->next)
	(*func)(gn->data.uint32);
}


void glist_apply_float32 (glist_t g, void (*func)(float32))
{
    gnode_t *gn;
    
    for (gn = g; gn; gn = gn->next)
	(*func)(gn->data.float32);
}


void glist_apply_float64 (glist_t g, void (*func)(float64))
{
    gnode_t *gn;
    
    for (gn = g; gn; gn = gn->next)
	(*func)(gn->data.float64);
}


void glist_free (glist_t g)
{
    gnode_t *gn;
    
    while (g) {
	gn = g;
	g = gn->next;
	myfree((char *)gn, sizeof(gnode_t));
    }
}


void glist_myfree (glist_t g, int32 datasize)
{
    gnode_t *gn;
    
    while (g) {
	gn = g;
	g = gn->next;
	myfree((char *)(gn->data.ptr), datasize);
	myfree((char *)gn, sizeof(gnode_t));
    }
}


int32 glist_count (glist_t g)
{
    gnode_t *gn;
    int32 n;
    
    for (gn = g, n = 0; gn; gn = gn->next, n++);
    return n;
}


gnode_t *glist_tail (glist_t g)
{
    gnode_t *gn;
    
    if (! g)
	return NULL;
    
    for (gn = g; gn->next; gn = gn->next);
    return gn;
}


glist_t glist_reverse (glist_t g)
{
    gnode_t *gn, *nextgn;
    gnode_t *rev;
    
    rev = NULL;
    for (gn = g; gn; gn = nextgn) {
	nextgn = gn->next;
	
	gn->next = rev;
	rev = gn;
    }

    return rev;
}
