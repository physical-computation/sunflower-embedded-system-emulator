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
 * subvq.c
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
 * 20.Apr.2001  RAH (rhoughton@mediasite.com, ricky.houghton@cs.cmu.edu)
 *              Updated subvq_free () to free allocated memory
 * 
 * 17-Dec-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added handling of a single sub-vector in subvq_mgau_shortlist().
 * 
 * 15-Dec-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Changes due to moving subvq_t.{frm_sen_eval,frm_gau_eval} to cont_mgau.h.
 * 
 * 14-Dec-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added subvq_t.{frm_sen_eval,frm_gau_eval}.  Changed subvq_frame_eval to
 * 		return the normalization factor.
 * 
 * 06-Dec-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added subvq_subvec_eval_logs3().
 * 
 * 14-Oct-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Changed ci_active flags input to sen_active in subvq_frame_eval().
 * 
 * 21-Jul-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Bugfix in subvq_init() that used g for allocating working areas, even
 *		though g wasn't defined.
 * 
 * 20-Jul-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added subvq_gautbl_eval_logs3() and used it in subvq_frame_eval().
 * 
 * 12-Mar-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Started.
 */


#include "subvq.h"
/* #include "cmd_ln_args.h"	*/ /* RAH, added so we can allow for -vqeval parameter */
#include "s3types.h"
#if defined(THRD) 
//pip:
#include<pthread.h>
#include "utt.h"
#ifdef NEW_EVAL_LOGS3

//pip: we already have this #include<pthread.h>
#include "barrier.h"

#endif
#endif

/* RAH, 5.8.01, VQ_EVAL determines how many vectors are used to
 * compute the shortlist, for now this value is only relevant when n_sv =3.
 * Setting it to 1 means that only the CEP values are estimated, 2 means that 
 * CEP and delta values are estimated, 3 means all three are estimated.
 * Note, we must adjust the beam widths as we muck around with these.
 */
int VQ_EVAL = 3;		


/*
 * Precompute variances/(covariance-matrix-determinants) to simplify Mahalanobis distance
 * calculation.  Also, calculate 1/(det) for the original codebooks, based on the VQ vars.
 * (See comment in libmisc/vector.h.)
 */
static void subvq_maha_precomp (subvq_t *svq, float64 floor)
{
    int32 s;
    float32 *lrd;
    vector_gautbl_t *gautbl;
    
    E_INFO("Precomputing Mahalanobis distance invariants\n");
    
    lrd = (float32 *) ckd_calloc (svq->n_sv * svq->vqsize, sizeof(float32));
    
    for (s = 0; s < svq->n_sv; s++) {
	gautbl = &(svq->gautbl[s]);

	vector_gautbl_var_floor (gautbl, floor);
	
	gautbl->lrd = lrd;
	lrd += svq->vqsize;
	vector_gautbl_maha_precomp (gautbl);
    }
}


static void subvq_map_compact (subvq_t *vq, mgau_model_t *g)
{
    int32 r, c, c2, s;
    
    if (g) {
	if ((g->n_mgau != vq->origsize.r) || (g->max_comp != vq->origsize.c))
	    E_FATAL("Model size conflict: %d x %d (SubVQ) vs %d x %d (Original)\n",
		    vq->origsize.r, vq->origsize.c, g->n_mgau, g->max_comp);
    }
    
    /*
     * Compress map entries by removing invalid ones.  NOTE: There's not much of a consistency
     * check to ensure that the entries remaining do map correctly on to the valid entries in
     * the original (parent) mixture Gaussian model g.  The only check we have is to verify
     * the NUMBER of valid entries (components) in each mixture.
     */
    for (r = 0; r < vq->origsize.r; r++) {
	for (c = 0, c2 = 0; c < vq->origsize.c; c++) {
	    if (vq->map[r][c][0] < 0) {
		/* All ought to be < 0 */
		for (s = 1; s < vq->n_sv; s++) {
		    if (vq->map[r][c][s] >= 0)
			E_FATAL("Partially undefined map[%d][%d]\n", r, c);
		}
	    } else {
		if (c2 != c) {
		    for (s = 0; s < vq->n_sv; s++) {
			if (vq->map[r][c][s] < 0)
			    E_FATAL("Partially undefined map[%d][%d]\n", r, c);
			vq->map[r][c2][s] = vq->map[r][c][s];
		    }
		}
		c2++;
	    }
	}
	
	if (g && (c2 != mgau_n_comp (g, r)))
	    E_FATAL("Mixture %d: #Valid components conflict: %d (SubVQ) vs %d (Original)\n",
		    r, c2, mgau_n_comp(g,r));
	
	/* Invalidate the remaining map entries, for good measure */
	for (; c2 < vq->origsize.c; c2++) {
	    for (s = 0; s < vq->n_sv; s++)
		vq->map[r][c2][s] = -1;
	}
    }
}


/*
 * At this point, a map entries is an index within the subvector; i.e., map[r][c][s] points to
 * a subvq codeword within subvector s.  In evaluating a complete density using it subvq
 * component scores, these maps are used, with 2 lookups for each sub-vector.  To reduce the
 * lookups, linearize the map entries by viewing the subvq scores as a linear array rather than
 * a 2-D array.
 */
static void subvq_map_linearize (subvq_t *vq)
{
    int32 r, c, s;
    
    for (r = 0; r < vq->origsize.r; r++) {
	for (c = 0; (c < vq->origsize.c) && (vq->map[r][c][0] >= 0); c++) {
	    for (s = 0; s < vq->n_sv; s++)
		vq->map[r][c][s] = (s * vq->vqsize) + vq->map[r][c][s];
	}
    }
}


subvq_t *subvq_init (char *file, float64 varfloor, int32 max_sv, mgau_model_t *g)
{
    FILE *fp;
    char line[16384];
    int32 n_sv;		/* #Subvectors in file, as opposed to that used */
    int32 s, k, l, n, r, c;
    char *strp;
    subvq_t *vq;
    
    VQ_EVAL = cmd_ln_int32 ("-vqeval");	/* RAH, Currently only works when n_sv = 3, values computed but ignored in other cases */

    E_INFO("Loading Mixture Gaussian sub-VQ file '%s' (vq_eval: %d)\n", file,VQ_EVAL);
    
    vq = (subvq_t *) ckd_calloc (1, sizeof(subvq_t));
    
    fp = myfopen(file, "r");
    
    /* Read until "Sub-vectors" */
    for (;;) {
	if (fgets (line, sizeof(line), fp) == NULL)
	    E_FATAL("Failed to read VQParam header\n");
	if (sscanf (line, "VQParam %d %d -> %d %d",
		    &(vq->origsize.r), &(vq->origsize.c), &(vq->n_sv), &(vq->vqsize)) == 4)
	    break;
    }
    
    if (g) {
	if ((g->n_mgau != vq->origsize.r) || (g->max_comp != vq->origsize.c))
	    E_FATAL("Model size conflict: %d x %d (SubVQ) vs %d x %d (Original)\n",
		    vq->origsize.r, vq->origsize.c, g->n_mgau, g->max_comp);
    }
    
    if (max_sv < 0)
      max_sv = vq->n_sv;
    if (max_sv < vq->n_sv)
      E_INFO("Using %d subvectors out of %d\n", max_sv, vq->n_sv);
    else if (max_sv > vq->n_sv) {
      E_WARN("#Subvectors specified(%d) > available(%d); using latter\n", max_sv, vq->n_sv);
      max_sv = vq->n_sv;
    }
    
    n_sv = vq->n_sv;
    vq->n_sv = max_sv;
    if (vq->n_sv < VQ_EVAL)	/* RAH, 5.9.01, sanity check to make sure VQ_EVAL isn't higher than the n_sv */
      VQ_EVAL = vq->n_sv;
    vq->featdim = (int32 **) ckd_calloc (vq->n_sv, sizeof(int32 *));
    vq->gautbl = (vector_gautbl_t *) ckd_calloc (vq->n_sv, sizeof(vector_gautbl_t));
    vq->map = (int32 ***) ckd_calloc_3d (vq->origsize.r, vq->origsize.c, vq->n_sv,
					 sizeof(int32));
    
    /* Read subvector sizes and feature dimension maps */
    for (s = 0; s < n_sv; s++) {
	if ((fgets (line, sizeof(line), fp) == NULL) ||
	    (sscanf (line, "Subvector %d length %d%n", &k, &l, &n) != 2) || (k != s))
	    E_FATAL("Error reading length(subvector %d)\n", s);
	
	if (s < vq->n_sv) {
	    vq->gautbl[s].veclen = l;
	    vq->featdim[s] = (int32 *) ckd_calloc (vq->gautbl[s].veclen, sizeof(int32));
	
	    for (strp = line+n, c = 0; c < vq->gautbl[s].veclen; c++) {
		if (sscanf (strp, "%d%n", &(vq->featdim[s][c]), &n) != 1)
		    E_FATAL("Error reading subvector(%d).featdim(%d)\n", s, c);
		strp += n;
	    }
	    
	    vector_gautbl_alloc (&(vq->gautbl[s]), vq->vqsize, vq->gautbl[s].veclen);
	}
    }
    
    /* Echo info for sanity check */
    E_INFO("Original #codebooks(states)/codewords: %d x %d\n", vq->origsize.r, vq->origsize.c);
    E_INFO("Subvectors: %d, VQsize: %d\n", vq->n_sv, vq->vqsize);
    for (s = 0; s < vq->n_sv; s++) {
	E_INFO("SV %d feature dims(%d): ", s, vq->gautbl[s].veclen);
	for (c = 0; c < vq->gautbl[s].veclen; c++)
	    fprintf (stderr, " %2d", vq->featdim[s][c]);
	fprintf (stderr, "\n");
    }
    fflush (stderr);
    
    /* Read VQ codebooks and maps for each subvector */
    for (s = 0; s < n_sv; s++) {
#if 1
      int32 veclen =  vq->gautbl[s].veclen;
      int a_veclen = (veclen%4)?veclen+4-(veclen%4):veclen;
#endif
      E_INFO("Reading subvq %d%s\n", s, (s < vq->n_sv) ? "" : " (not used)");
	
	E_INFO("Reading codebook\n");
	if ((fgets (line, sizeof(line), fp) == NULL) ||
	    (sscanf (line, "Codebook %d", &k) != 1) || (k != s))
	    E_FATAL("Error reading codebook header\n", s);
	
	for (r = 0; r < vq->vqsize; r++) {

	    if (fgets (line, sizeof(line), fp) == NULL)
		E_FATAL("Error reading row(%d)\n", r);
	    
	    if (s >= vq->n_sv)
		continue;
	    
	    for (strp = line, c = 0; c < veclen/*vq->gautbl[s].veclen*/; c++) {

		if (sscanf (strp, "%f %f%n",
			    &(vq->gautbl[s].mean[r][c]), &(vq->gautbl[s].var[r][c]), &k) != 2)
		    E_FATAL("Error reading row(%d) col(%d)\n", r, c);
		strp += k;
	    }
#if 1
	    for (;c<a_veclen;c++) {
	      vq->gautbl[s].mean[r][c]=0.0;
	      vq->gautbl[s].var[r][c]=0.0;
	    }
#endif
	}
	
	E_INFO("Reading map\n");
	if ((fgets (line, sizeof(line), fp) == NULL) ||
	    (sscanf (line, "Map %d", &k) != 1) || (k != s))
	    E_FATAL("Error reading map header\n", s);
	
	for (r = 0; r < vq->origsize.r; r++) {
	    if (fgets (line, sizeof(line), fp) == NULL)
		E_FATAL("Error reading row(%d)\n", r);
	    
	    if (s >= vq->n_sv)
		continue;
	    
	    for (strp = line, c = 0; c < vq->origsize.c; c++) {
		if (sscanf (strp, "%d%n", &(vq->map[r][c][s]), &k) != 1)
		    E_FATAL("Error reading row(%d) col(%d)\n", r, c);
		strp += k;
	    }
	}
	
	fflush (stdout);
    }
    
    if ((fscanf (fp, "%s", line) != 1) || (strcmp (line, "End") != 0))
	E_FATAL("Error reading 'End' token\n");
    
    fclose (fp);
    
    subvq_maha_precomp (vq, varfloor);
    subvq_map_compact (vq, g);
    subvq_map_linearize (vq);
    
    n = 0;
    for (s = 0; s < n_sv; s++) {
	if (vq->gautbl[s].veclen > n)
	    n = vq->gautbl[s].veclen;
    }
    assert (n > 0);
#ifdef THRD
    for (s=0; s < vq->n_sv; s++)
      vq->thrd_subvec[s] = (float32 *) ckd_calloc (n, sizeof(float32));
    
    for (s = 0; s < NUM_THREADS; s++) {
      vq->thrd_gauscore[s] = (int32 *) ckd_calloc (vq->origsize.c, sizeof(int32));
      vq->thrd_mgau_sl[s] = (int32 *) ckd_calloc (vq->origsize.c + 1, sizeof(int32));
    }

#endif
#ifdef USE_ICC
    vq->subvec = (float32 *) _mm_malloc (n*sizeof(float32),16);
#else
    vq->subvec = (float32 *) ckd_calloc (n, sizeof(float32));
#endif
    vq->vqdist = (int32 **) ckd_calloc_2d (vq->n_sv, vq->vqsize, sizeof(int32));

    vq->gauscore = (int32 *) ckd_calloc (vq->origsize.c, sizeof(int32));
    vq->mgau_sl = (int32 *) ckd_calloc (vq->origsize.c + 1, sizeof(int32));

    return vq;
}


/*
 * Based on previously computed subvq scores (Mahalanobis distances), determine the active
 * components in the given mixture (using the vq->map).
 * Return value: #Candidates in the returned shortlist.
 */
static int32 subvq_mgau_shortlist (subvq_t *vq,
				   int32 m,	/* In: Mixture index */
				   int32 n,	/* In: #Components in specified mixture */
				   int32 beam)	/* In: Threshold to select active components */
{
    int32 *gauscore;
    int32 *map;
    int32 i, v, bv, th, nc;
    int32 *sl;
    int32 *vqdist;
    
    vqdist = vq->vqdist[0];	/* Since map is linearized for efficiency, must also
				   look at vqdist[][] as vqdist[] */
    gauscore = vq->gauscore;
    sl = vq->mgau_sl;
    
    /* Special case when vq->n_sv == 3; for speed */
    map = vq->map[m][0];
    bv = MAX_NEG_INT32;
    
    switch (vq->n_sv) {
    case 3:
	for (i = 0; i < n; i++) {
	  if (VQ_EVAL == 1) {
	    v = (int32) vqdist[*map];/* If we are not weighting the cep values, we need to adjust the subvqbeam */
	    map += 3;
	  } else {
	    /* RAH, we are ignoring the delta-delta, scoring the delta twice, strangely this works better than weighting the scores  */
	    /* I believe it has to do with the beam widths */
	    if (VQ_EVAL == 2) {
	      v = vqdist[*(map++)];
	      v += 2 * vqdist[*map]; /* RAH Count delta twice, we can keep the same subvqbeam as vq_eval = 3 if we double the delta*/
	      map += 2;
	    } else {
	      v = vqdist[*(map++)];/* Standard way */
	      v += vqdist[*(map++)]; /*  */
	      v += vqdist[*(map++)]; /*  */
	    }
	  }

	  gauscore[i] = v;
	  
	  if (bv < v)
	    bv = v;
	}
	break;
    case 2:
	for (i = 0; i < n; i++) {
	    v = vqdist[*(map++)];
	    v += vqdist[*(map++)];
	    gauscore[i] = v;
	    
	    if (bv < v)
		bv = v;
	}
	break;
    case 1:
      for (i = 0; i < n; i++) {
	v = vqdist[*(map++)];
	gauscore[i] = v;
	
	if (bv < v)
	  bv = v;
      }
      break;
    default:
      E_FATAL("#Subvectors %d not yet implemented\n", vq->n_sv);
    }
    
    th = bv + beam;
    nc = 0;
    for (i = 0; i < n; i++) {
      if (gauscore[i] >= th)
	sl[nc++] = i;
    }
    sl[nc] = -1;
    
    return nc;
}

#if 0
void subvq_subvec_eval_logs3 (subvq_t *vq, float32 *feat, int32 s)
{
    int32 i;
    int32 *featdim;
    
    /* Extract subvector from feat */
    featdim = vq->featdim[s];
    for (i = 0; i < vq->gautbl[s].veclen; i++)
	vq->subvec[i] = feat[featdim[i]];
    
    /* Evaluate distances between extracted subvector and corresponding codebook */
    vector_gautbl_eval_logs3(&(vq->gautbl[s]), 0, vq->vqsize, vq->subvec, vq->vqdist[s]);
}
#endif

void subvq_gautbl_eval_logs3 (subvq_t *vq, float32 *feat)
{
    int32 s, i;
    int32 *featdim;
    
    for (s = 0; s < vq->n_sv; s++) {
	/* Extract subvector from feat */
	featdim = vq->featdim[s];
	for (i = 0; i < vq->gautbl[s].veclen; i++) {
	  vq->subvec[i] = feat[featdim[i]];
	}
	
	/* Evaluate distances between extracted subvector and corresponding codebook */
    
	/* RAH, only evaluate the first VQ_EVAL set of features */
	if (s < VQ_EVAL) {
	  vector_gautbl_eval_logs3(&(vq->gautbl[s]), 0, vq->vqsize, vq->subvec, 
				   vq->vqdist[s]);
	}
	/*printf("vqsize %d veclen %d\n",vq->vqsize,vq->gautbl[s].veclen);*/

    }
}

#ifdef THRD
#ifdef NEW_EVAL_LOGS3

int32 sema = 0;
extern pthread_mutex_t update_lock;
extern barrier_t *score_barrier;


void thrd_subvq_gautbl_eval_logs3 (int subvec_num, subvq_t *vq, float32 *feat)
{

  if (NUM_THREADS == 1) {
    int32 s, i;
    int32 *featdim;
    
    for (s=0; s < vq->n_sv; s++) {
      /* Extract subvector from feat */
      featdim = vq->featdim[s];
      for (i = 0; i < vq->gautbl[s].veclen; i++) {
	vq->subvec[i] = feat[featdim[i]];
      }

      if (s < VQ_EVAL)
	/* Evaluate distances between extracted subvector and corresponding codebook */
	
	/* RAH, only evaluate the first VQ_EVAL set of features */
	vector_gautbl_eval_logs3(&(vq->gautbl[s]), 0, vq->vqsize, vq->subvec, vq->vqdist[s]);
    }
    
  } else { /* NUM_THREADS > 1 */


  int32 s, i;
  int32 *featdim;
  const int32 vqsize = vq->vqsize;
  const int32 n_sv = vq->n_sv;
  const int32 remainder = (vqsize * n_sv)%NUM_THREADS;
  int32 chunk_size = (vqsize * n_sv)/NUM_THREADS;
  int32 offset;
  int32 subv_ex;
  
  s = subvec_num*chunk_size; /* subvec_num is now the thread no. */
  if (remainder>subvec_num) { s++; chunk_size++; };

  offset = s % vqsize;
  s /= vqsize; /* determine which vector to process */
  if (DEBUG&0x4) fprintf(stderr,"thrd %d chunk_size %d offset %d s %d\n",
			 subvec_num,chunk_size, offset, s);

  assert(s<n_sv && "subvec index out of range!!\n");

#if (NUM_THREADS>1)
  pthread_mutex_lock(&update_lock);

  subv_ex = (sema++)%NUM_THREADS;

  pthread_mutex_unlock(&update_lock);
#else
  subv_ex = (sema++)%NUM_THREADS;
#endif

  if (subv_ex < n_sv) {
    if (DEBUG&0x4) 
      fprintf(stderr,"thread %d subvec %d veclen %d\n",subvec_num,subv_ex,
	      vq->gautbl[subv_ex].veclen);
    /* Extract subvector from feat */
    featdim = vq->featdim[subv_ex];
    for (i = 0; i < vq->gautbl[subv_ex].veclen; i++)
      vq->thrd_subvec[subv_ex][i] = feat[featdim[i]];
  }

  if (DEBUG&0x4) fprintf(stderr,"thread %d before barrier \n",subvec_num);
  
  thread_barrier(subvec_num,score_barrier);

  if (DEBUG&0x4) fprintf(stderr,"thread %d after barrier \n",subvec_num);

  /* Evaluate distances between extracted subvector and corresponding codebook */
  
  /* RAH, only evaluate the first VQ_EVAL set of features */
  if (s < VQ_EVAL) {
    if (offset+chunk_size > vqsize) {
      if (DEBUG&0x4) fprintf(stderr,"thread %d subvec_eval %d offset %d size %d\n",
			     subvec_num,s,offset,vqsize-offset);
      vector_gautbl_eval_logs3(&(vq->gautbl[s]), offset, vqsize-offset, vq->thrd_subvec[s], vq->vqdist[s]);
      chunk_size -= (vqsize-offset);
    } else {
      if (DEBUG&0x4) fprintf(stderr,"thread %d subvec_eval %d offset %d size %d\n",
			     subvec_num,s,offset,chunk_size);
      vector_gautbl_eval_logs3(&(vq->gautbl[s]), offset, chunk_size, vq->thrd_subvec[s], vq->vqdist[s]);
      chunk_size = 0;
    }
    
    while (chunk_size>0) {
      s++;
      if (chunk_size > vqsize) {
	if (DEBUG&0x4) fprintf(stderr,"thread %d subvec_eval %d offset %d size %d\n",
			       subvec_num,s,0,vqsize);
	vector_gautbl_eval_logs3(&(vq->gautbl[s]), 0, vqsize, vq->thrd_subvec[s], vq->vqdist[s]);
	chunk_size-=vqsize;
      } else {
	if (DEBUG&0x4) fprintf(stderr,"thread %d subvec_eval %d offset %d size %d\n",
			       subvec_num,s,0,chunk_size);
	vector_gautbl_eval_logs3(&(vq->gautbl[s]), 0, chunk_size, vq->thrd_subvec[s], vq->vqdist[s]);
	chunk_size=0;
      }

    }
  }

  thread_barrier(subvec_num,score_barrier);

  }
}
#endif /* end of NEW_EVAL_LOGS3 */

#if 0
void thrd_subvq_gautbl_eval_logs3 (int subvec_num, subvq_t *vq, float32 *feat)
{
  int32 s, i;
  int32 *featdim;

  s = subvec_num;

  /* Extract subvector from feat */
  featdim = vq->featdim[s];
  for (i = 0; i < vq->gautbl[s].veclen; i++)
    vq->thrd_subvec[s][i] = feat[featdim[i]];
    
  /* Evaluate distances between extracted subvector and corresponding codebook */
  
  /* RAH, only evaluate the first VQ_EVAL set of features */

  if (s < VQ_EVAL) 
    vector_gautbl_eval_logs3(&(vq->gautbl[s]), 0, vq->vqsize, vq->thrd_subvec[s], vq->vqdist[s]);


}
#endif
#endif /* end THRD */



int32 subvq_frame_eval (subvq_t *vq, mgau_model_t *g, int32 beam, float32 *feat,
			int32 *sen_active, int32 *senscr)
{
  int32 s;
  int32 best, ns, ng;
  
  best = MAX_NEG_INT32;
  ns = 0;
  ng = 0;
  
  if (! vq) {
    /* No subvq model, use the original (SLOW!!) */
    for (s = 0; s < g->n_mgau; s++) {
      if ((! sen_active) || sen_active[s]) {
	senscr[s] = mgau_eval (g, s, NULL, feat);
	if (best < senscr[s])
	  best = senscr[s];
	ns++;
	ng += mgau_n_comp (g, s);
      } else
	senscr[s] = S3_LOGPROB_ZERO;
    }
  } else {
    /* Evaluate subvq model for given feature vector */
    subvq_gautbl_eval_logs3 (vq, feat);

    /* Find mixture component shortlists using subvq scores, and evaluate senones */
    for (s = 0; s < g->n_mgau; s++) {
      if ((! sen_active) || sen_active[s]) {
	ng += subvq_mgau_shortlist (vq, s, mgau_n_comp(g,s), beam);
	
	senscr[s] = mgau_eval (g, s, vq->mgau_sl, feat);
	if (best < senscr[s])
	  best = senscr[s];
	
	ns++;
	
      } else
	senscr[s] = S3_LOGPROB_ZERO;
    }
  }
    
    /* Normalize senone scores */
    for (s = 0; s < g->n_mgau; s++)
	senscr[s] -= best;
    
    g->frm_sen_eval = ns;
    g->frm_gau_eval = ng;
    
    return best;
}

/* RAH, free memory allocated by subvq_init() */
void subvq_free (subvq_t *s)
{
  int i;

  if (s) {
    
    for (i=0;i<s->n_sv;i++) {
      /*      vector_gautbl_free (&(s->gautbl[i]));*/
      if (s->featdim[i]) ckd_free ((void *) s->featdim[i]);
    }


    if (s->featdim) 
      ckd_free ((void *) s->featdim);

    /* Free gaussian table */
    if (s->gautbl) 
      ckd_free ((void *)s->gautbl);      


    /* Free map */
    if (s->map)
      ckd_free_3d ((void ***) s->map);

    if (s->subvec) 
      ckd_free ((void *) s->subvec);

    if (s->vqdist) 
      ckd_free_2d ((void **) s->vqdist);

    if (s->gauscore) 
      ckd_free ((void *) s->gauscore);

    if (s->mgau_sl) 
      ckd_free ((void *) s->mgau_sl);

	
    ckd_free ((void *)s);


  }
}
