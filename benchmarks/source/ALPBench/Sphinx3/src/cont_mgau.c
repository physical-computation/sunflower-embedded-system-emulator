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
 * cont_mgau.c -- Mixture Gaussians for continuous HMM models.
 *
 * **********************************************
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * ALL RIGHTS RESERVED.
 * **********************************************
 *
 * HISTORY
 * 
 * 20.Apr.2001  RAH (rhoughton@mediasite.com, ricky.houghton@cs.cmu.edu)
 *              Added mgau_free to free memory allocated by mgau_init()
 * 15-Dec-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added mgau_var_nzvec_floor().
 * 
 * 28-Mar-1999	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University.
 * 		Started.
 */


#include "s3types.h"
#include "bio.h"
#include "vector.h"
#include "logs3.h"
#include "cont_mgau.h"

#if 0/*def SSE2*/
#include <emmintrin.h>
#endif
#define MGAU_PARAM_VERSION	"1.0"	/* Sphinx-3 file format version for mean/var */
#define MGAU_MIXW_VERSION	"1.0"	/* Sphinx-3 file format version for mixw */
#define MGAU_MEAN		1
#define MGAU_VAR		2


/*
 * Sphinx-3 model mean and var files have the same format.  Use this routine for reading
 * either one.
 */
static int32 mgau_file_read(mgau_model_t *g, char *file_name, int32 type)
{
    char tmp;
    FILE *fp;
    int32 i, k, n;
    int32 n_mgau;
    int32 n_feat;
    int32 n_density;
    int32 veclen;
    int32 byteswap, chksum_present;
    float32 *buf, **pbuf;
    char **argname, **argval;
    uint32 chksum;
#ifdef USE_ICC
    int32 a_veclen;
#endif
    
    E_INFO("Reading mixture gaussian file '%s'\n", file_name);
    
    fp = myfopen (file_name, "rb");
    
    /* Read header, including argument-value info and 32-bit byteorder magic */
    if (bio_readhdr (fp, &argname, &argval, &byteswap) < 0)
	E_FATAL("bio_readhdr(%s) failed\n", file_name);
    
    /* Parse argument-value list */
    chksum_present = 0;
    for (i = 0; argname[i]; i++) {
	if (strcmp (argname[i], "version") == 0) {
	    if (strcmp(argval[i], MGAU_PARAM_VERSION) != 0)
		E_WARN("Version mismatch(%s): %s, expecting %s\n",
		       file_name, argval[i], MGAU_PARAM_VERSION);
	} else if (strcmp (argname[i], "chksum0") == 0) {
	    chksum_present = 1;	/* Ignore the associated value */
	}
    }
    bio_hdrarg_free (argname, argval);
    argname = argval = NULL;
    
    chksum = 0;
    
    /* #Codebooks */
    if (bio_fread (&n_mgau, sizeof(int32), 1, fp, byteswap, &chksum) != 1)
	E_FATAL("fread(%s) (#codebooks) failed\n", file_name);
    if (n_mgau >= MAX_S3MGAUID) {
	E_FATAL("%s: #Mixture Gaussians (%d) exceeds limit(%d) enforced by MGAUID type\n",
		file_name, n_mgau, MAX_S3MGAUID);
    }
    
    /* #Features/codebook */
    if (bio_fread (&n_feat, sizeof(int32), 1, fp, byteswap, &chksum) != 1)
	E_FATAL("fread(%s) (#features) failed\n", file_name);
    if (n_feat != 1)
	E_FATAL("#Features streams(%d) != 1\n", n_feat);
    
    /* #Gaussian densities/feature in each codebook */
    if (bio_fread (&n_density, sizeof(int32), 1, fp, byteswap, &chksum) != 1)
	E_FATAL("fread(%s) (#density/codebook) failed\n", file_name);
    
    /* Vector length of feature stream */
    if (bio_fread (&veclen, sizeof(int32), 1, fp, byteswap, &chksum) != 1)
	E_FATAL("fread(%s) (feature vector-length) failed\n", file_name);
    
    /* #Floats to follow; for the ENTIRE SET of CODEBOOKS */
    if (bio_fread (&n, sizeof(int32), 1, fp, byteswap, &chksum) != 1)
	E_FATAL("fread(%s) (total #floats) failed\n", file_name);
    if (n != n_mgau * n_density * veclen) {
	E_FATAL("%s: #float32s(%d) doesn't match dimensions: %d x %d x %d\n",
		file_name, n, n_mgau, n_density, veclen);
    }

    if (type == MGAU_MEAN) {
	/* Allocate memory for mixture gaussian densities */
#ifdef USE_ICC
      a_veclen = (veclen%4)?veclen+4-(veclen%4):veclen;
      n = n_mgau*n_density*a_veclen;
#endif
      g->n_mgau = n_mgau;
	g->max_comp = n_density;
	g->veclen = veclen;
	g->mgau = (mgau_t *) ckd_calloc (n_mgau, sizeof(mgau_t));

#ifdef USE_ICC
	buf = (float32 *) _mm_malloc (n*sizeof(float),16);
#else
	buf = (float32 *) ckd_calloc (n, sizeof(float));
#endif
	pbuf = (float32 **) ckd_calloc (n_mgau * n_density, sizeof(float32 *));
	
	for (i = 0; i < n_mgau; i++) {
	    g->mgau[i].n_comp = n_density;
	    g->mgau[i].mean = pbuf;
	    
	    for (k = 0; k < n_density; k++) {
		g->mgau[i].mean[k] = buf;
#ifdef USE_ICC
		buf += a_veclen; /*veclen;*/
#else
		buf += veclen;
#endif
	    }
	    pbuf += n_density;
	}
	
	buf = g->mgau[0].mean[0];	/* Restore buf to original value */
    } else {
#ifdef USE_ICC
      a_veclen = (veclen%4)?veclen+4-(veclen%4):veclen;
      n = n_mgau*n_density*a_veclen;
#endif
	assert (type == MGAU_VAR);
	
	if (g->n_mgau != n_mgau)
	    E_FATAL("#Mixtures(%d) doesn't match that of means(%d)\n", n_mgau, g->n_mgau);
	if (g->max_comp != n_density)
	    E_FATAL("#Components(%d) doesn't match that of means(%d)\n", n_density, g->max_comp);
	if (g->veclen != veclen)
	    E_FATAL("#Vector length(%d) doesn't match that of means(%d)\n", veclen, g->veclen);
	
#ifdef USE_ICC
	buf = (float32 *) _mm_malloc (n*sizeof(float),16);
#else
	buf = (float32 *) ckd_calloc (n, sizeof(float32));
#endif
	pbuf = (float32 **) ckd_calloc (n_mgau * n_density, sizeof(float32 *));
	
	for (i = 0; i < n_mgau; i++) {
	    if (g->mgau[i].n_comp != n_density)
		E_FATAL("Mixture %d: #Components(%d) doesn't match that of means(%d)\n",
			i, n_density, g->mgau[i].n_comp);
	    
	    g->mgau[i].var = pbuf;
	    
	    for (k = 0; k < n_density; k++) {
		g->mgau[i].var[k] = buf;
#ifdef USE_ICC
		buf += a_veclen; /*veclen;*/
#else
		buf += veclen;
#endif
	    }
	    pbuf += n_density;
	}
	
	buf = (float32 *) ckd_calloc (n_mgau * n_density, sizeof(float32));
	for (i = 0; i < n_mgau; i++) {
	    g->mgau[i].lrd = buf;
	    buf += n_density;
	}
	
	buf = g->mgau[0].var[0];	/* Restore buf to original value */
    }

#ifdef USE_ICC
    if (type == MGAU_MEAN) {
      for (i=0; i < n_mgau; i++) {
	for (k=0; k < n_density; k++) {
	  /*	  printf("g->mgau[%d].mean[%d] %p\n",i,k,g->mgau[i].mean[k]);*/
	  bio_fread(g->mgau[i].mean[k], sizeof(float32), veclen, fp, byteswap, &chksum);
	}
      }
    } else {
      for (i=0; i < n_mgau; i++) {
	for (k=0; k < n_density; k++) {
	  bio_fread(g->mgau[i].var[k], sizeof(float32), veclen, fp, byteswap, &chksum);
	  g->mgau[i].var[k][veclen]=0.0; /* to avoid loop peel in mgau_eval*/
	}
      }
    }
#else   
    /* Read mixture gaussian densities data */
    if (bio_fread (buf, sizeof(float32), n, fp, byteswap, &chksum) != n)
	E_FATAL("fread(%s) (densitydata) failed\n", file_name);
#endif

    if (chksum_present)
	bio_verify_chksum (fp, byteswap, chksum);

    if (fread (&tmp, 1, 1, fp) == 1)
	E_FATAL("%s: More data than expected\n", file_name);
    
    fclose(fp);
    
    E_INFO("%d mixture Gaussians, %d components, veclen %d\n", n_mgau, n_density, veclen);
    
    return 0;
}


static int32 mgau_mixw_read(mgau_model_t *g, char *file_name, float64 mixwfloor)
{
    char **argname, **argval;
    char eofchk;
    FILE *fp;
    int32 byteswap, chksum_present;
    uint32 chksum;
    int32 *buf;
    float32 *pdf;
    int32 i, j, n;
    int32 n_mgau;
    int32 n_feat;
    int32 n_comp;
    int32 n_err;
    
    E_INFO("Reading mixture weights file '%s'\n", file_name);
    
    fp = myfopen (file_name, "rb");
    
    /* Read header, including argument-value info and 32-bit byteorder magic */
    if (bio_readhdr (fp, &argname, &argval, &byteswap) < 0)
	E_FATAL("bio_readhdr(%s) failed\n", file_name);
    
    /* Parse argument-value list */
    chksum_present = 0;
    for (i = 0; argname[i]; i++) {
	if (strcmp (argname[i], "version") == 0) {
	    if (strcmp(argval[i], MGAU_MIXW_VERSION) != 0)
		E_WARN("Version mismatch(%s): %s, expecting %s\n",
			file_name, argval[i], MGAU_MIXW_VERSION);
	} else if (strcmp (argname[i], "chksum0") == 0) {
	    chksum_present = 1;	/* Ignore the associated value */
	}
    }
    bio_hdrarg_free (argname, argval);
    argname = argval = NULL;

    chksum = 0;

    /* Read #senones, #features, #codewords, arraysize */
    if ((bio_fread (&n_mgau, sizeof(int32), 1, fp, byteswap, &chksum) != 1) ||
	(bio_fread (&n_feat, sizeof(int32), 1, fp, byteswap, &chksum) != 1) ||
	(bio_fread (&n_comp, sizeof(int32), 1, fp, byteswap, &chksum) != 1) ||
	(bio_fread (&n, sizeof(int32), 1, fp, byteswap, &chksum) != 1)) {
	E_FATAL("bio_fread(%s) (arraysize) failed\n", file_name);
    }
    if (n_feat != 1)
	E_FATAL("#Features streams(%d) != 1\n", n_feat);
    if (n != n_mgau * n_comp) {
	E_FATAL("%s: #float32s(%d) doesn't match header dimensions: %d x %d\n",
		file_name, i, n_mgau, n_comp);
    }
    if (n_mgau != g->n_mgau)
	E_FATAL("%s: #Mixture Gaussians(%d) doesn't match mean/var parameters(%d)\n",
		n_mgau, g->n_mgau);
    
    buf = (int32 *) ckd_calloc (n_mgau * n_comp, sizeof(int32));
    for (i = 0; i < n_mgau; i++) {
	if (n_comp != mgau_n_comp(g,i))
	    E_FATAL("Mixture %d: #Weights(%d) doesn't match #Gaussian components(%d)\n",
		    i, n_comp, mgau_n_comp(g,i));
	
	g->mgau[i].mixw = buf;
	buf += g->mgau[i].n_comp;
    }
    
    /* Temporary structure to read in floats before conversion to (int32) logs3 */
    pdf = (float32 *) ckd_calloc (n_comp, sizeof(float32));
    
    /* Read mixw data, normalize, floor, convert to logs3 */
    n_err = 0;
    for (i = 0; i < n_mgau; i++) {
	if (bio_fread((void *)pdf, sizeof(float32), n_comp, fp, byteswap, &chksum) != n_comp)
	    E_FATAL("bio_fread(%s) (arraydata) failed\n", file_name);
	
	/* Normalize and floor */
	if (vector_is_zero (pdf, n_comp)) {
	    n_err++;
	    for (j = 0; j < n_comp; j++)
		mgau_mixw(g,i,j) = S3_LOGPROB_ZERO;
	} else {
	    vector_nz_floor (pdf, n_comp, mixwfloor);
	    vector_sum_norm (pdf, n_comp);
	    for (j = 0; j < n_comp; j++)
		mgau_mixw(g,i,j) = (pdf[j] != 0.0) ? logs3(pdf[j]) : S3_LOGPROB_ZERO;
	}
    }
    if (n_err > 0)
	E_ERROR("Weight normalization failed for %d senones\n", n_err);

    ckd_free (pdf);

    if (chksum_present)
	bio_verify_chksum (fp, byteswap, chksum);
    
    if (fread (&eofchk, 1, 1, fp) == 1)
	E_FATAL("More data than expected in %s\n", file_name);

    fclose(fp);

    E_INFO("Read %d x %d mixture weights\n", n_mgau, n_comp);
    
    return 0;
}


/*
 * Compact each mixture Gaussian in the given model by removing any uninitialized components.
 * A component is considered to be uninitialized if its variance is the 0 vector.  Compact by
 * copying the data rather than moving pointers.  Otherwise, malloc pointers could get
 * corrupted.
 */
static void mgau_uninit_compact (mgau_model_t *g)
{
    int32 m, c, c2, n, nm;
    
    E_INFO("Removing uninitialized Gaussian densities\n");
    
    n = 0;
    nm = 0;
    for (m = 0; m < mgau_n_mgau(g); m++) {
	for (c = 0, c2 = 0; c < mgau_n_comp(g,m); c++) {
	    if (! vector_is_zero (mgau_var(g,m,c), mgau_veclen(g))) {
		if (c2 != c) {
		    memcpy (mgau_mean(g,m,c2), mgau_mean(g,m,c),
			    mgau_veclen(g) * sizeof(float32));
		    memcpy (mgau_var(g,m,c2), mgau_var(g,m,c),
			    mgau_veclen(g) * sizeof(float32));
		    mgau_mixw(g,m,c2) = mgau_mixw(g,m,c);
		}
		c2++;
	    } else {
		n++;
	    }
	}
	mgau_n_comp(g,m) = c2;
	if (c2 == 0) {
	    fprintf (stderr, " %d", m);
	    fflush (stderr);
	    nm++;
	}
    }
    if (nm > 0)
	fprintf (stderr, "\n");
    
    if ((nm > 0) || (n > 0))
	E_INFO ("%d densities removed (%d mixtures removed entirely)\n", n, nm);
}


static void mgau_var_floor (mgau_model_t *g, float64 floor)
{
  int32 m, c, i, n;
  
  E_INFO("Applying variance floor\n");
  n = 0;
  for (m = 0; m < mgau_n_mgau(g); m++) {
    for (c = 0; c < mgau_n_comp(g,m); c++) {
      for (i = 0; i < mgau_veclen(g); i++) {
	if (g->mgau[m].var[c][i] < floor) {
	  g->mgau[m].var[c][i] = (float32) floor;
	  n++;
	}
      }
    }
  }
  E_INFO("%d variance values floored\n", n);
}


int32 mgau_var_nzvec_floor (mgau_model_t *g, float64 floor)
{
  int32 m, c, i, n, l;
  float32 *var;
  
  E_INFO("Applying variance floor to non-zero variance vectors\n");
  
  l = mgau_veclen(g);
  
  n = 0;
  for (m = 0; m < mgau_n_mgau(g); m++) {
    for (c = 0; c < mgau_n_comp(g,m); c++) {
      var = g->mgau[m].var[c];
      
      if (! vector_is_zero (var, l)) {
	for (i = 0; i < l; i++) {
	  if (var[i] < floor) {
	    var[i] = (float32) floor;
	    n++;
	  }
	}
      }
    }
  }
  
  E_INFO("%d variance values floored\n", n);
  
  return n;
}


/*
 * Some of the Mahalanobis distance computation (between Gaussian density means and given
 * vectors) can be carried out in advance.  (See comment in .h file.)
 */
static int32 mgau_precomp (mgau_model_t *g)
{
    int32 m, c, i;
    float64 lrd;
    
    E_INFO("Precomputing Mahalanobis distance invariants\n");
    
    for (m = 0; m < mgau_n_mgau(g); m++) {
	for (c = 0; c < mgau_n_comp(g,m); c++) {
	    lrd = 0.0;
	    for (i = 0; i < mgau_veclen(g); i++) {
		lrd += log(g->mgau[m].var[c][i]);
		
		/* Precompute this part of the exponential */
		g->mgau[m].var[c][i] = (float32) (1.0 / (g->mgau[m].var[c][i] * 2.0));
	    }
	    
	    lrd += mgau_veclen(g) * log(2.0 * PI);	/* (2pi)^velen */
	    mgau_lrd(g,m,c) = (float32)(-0.5 * lrd);	/* Reciprocal, sqrt */
	}
    }
    
    return 0;
}


/* At the moment, S3 models have the same #means in each codebook and 1 var/mean */
mgau_model_t *mgau_init (char *meanfile, char *varfile, float64 varfloor,
			 char *mixwfile, float64 mixwfloor,
			 int32 precomp)
{
    mgau_model_t *g;
    
    assert (meanfile != NULL);
    assert (varfile != NULL);
    assert (varfloor >= 0.0);
    assert (mixwfile != NULL);
    assert (mixwfloor >= 0.0);
    
    g = (mgau_model_t *) ckd_calloc (1, sizeof(mgau_model_t));
    
    /* Read means and (diagonal) variances for all mixture gaussians */
    mgau_file_read (g, meanfile, MGAU_MEAN);
    mgau_file_read (g, varfile, MGAU_VAR);
    mgau_mixw_read (g, mixwfile, mixwfloor);
    
    mgau_uninit_compact (g);		/* Delete uninitialized components */
    
    if (varfloor > 0.0)
	mgau_var_floor (g, varfloor);	/* Variance floor after above compaction */
    
    if (precomp)
	mgau_precomp (g);		/* Precompute Mahalanobis distance invariants */
    
    g->distfloor = logs3_to_log (S3_LOGPROB_ZERO);	/* Floor for Mahalanobis distance values */
    
    return g;
}


int32 mgau_comp_eval (mgau_model_t *g, int32 s, float32 *x, int32 *score)
{
    mgau_t *mgau;
    int32 veclen;
    float32 *m, *v;
    float64 dval, diff, f;
    int32 bs;
    int32 i, c;



    
    veclen = mgau_veclen(g);
    mgau = &(g->mgau[s]);
    f = log_to_logs3_factor();

    bs = MAX_NEG_INT32;
    for (c = 0; c < mgau->n_comp; c++) {
	m = mgau->mean[c];
	v = mgau->var[c];
	dval = mgau->lrd[c];
	
	for (i = 0; i < veclen; i++) {
	    diff = x[i] - m[i];
	    dval -= diff * diff * v[i];
	}
	
	if (dval < g->distfloor)
	    dval = g->distfloor;
	
	score[c] = (int32) (f * dval);
	if (score[c] > bs)
	    bs = score[c];
    }
    
    return bs;
}


/*******************************************************************/
/* original code */
#if 0
int32 mgau_eval (mgau_model_t *g, int32 m, int32 *active, float32 *x)
{
    mgau_t *mgau;
    int32 veclen, score;
    float32 *m1, *m2, *v1, *v2;
    float64 dval1, dval2, diff1, diff2, f;
    int32 i, j, c;
    
    veclen = mgau_veclen(g);
    mgau = &(g->mgau[m]);
    f = log_to_logs3_factor();
    score = S3_LOGPROB_ZERO;
    
    if (! active) {	/* No short list; use all */
	for (c = 0; c < mgau->n_comp-1; c += 2) {	/* Interleave 2 components for speed */
	    m1 = mgau->mean[c];
	    m2 = mgau->mean[c+1];
	    v1 = mgau->var[c];
	    v2 = mgau->var[c+1];
	    dval1 = mgau->lrd[c];
	    dval2 = mgau->lrd[c+1];
	    
	    for (i = 0; i < veclen; i++) {
		diff1 = x[i] - m1[i];
		dval1 -= diff1 * diff1 * v1[i];
		diff2 = x[i] - m2[i];
		dval2 -= diff2 * diff2 * v2[i];
	    }
	    
	    if (dval1 < g->distfloor)	/* Floor */
		dval1 = g->distfloor;
	    if (dval2 < g->distfloor)
		dval2 = g->distfloor;
	    
	    score = logs3_add (score, (int32)(f * dval1) + mgau->mixw[c]);
	    score = logs3_add (score, (int32)(f * dval2) + mgau->mixw[c+1]);
	}
	
	/* Remaining iteration if n_mean odd */
	if (c < mgau->n_comp) {
	    m1 = mgau->mean[c];
	    v1 = mgau->var[c];
	    dval1 = mgau->lrd[c];
	    
	    for (i = 0; i < veclen; i++) {
		diff1 = x[i] - m1[i];
		dval1 -= diff1 * diff1 * v1[i];
	    }
	    
	    if (dval1 < g->distfloor)
		dval1 = g->distfloor;
	    
	    score = logs3_add (score, (int32)(f * dval1) + mgau->mixw[c]);
	}
    } else {
	for (j = 0; active[j] >= 0; j++) {
	    c = active[j];
	    
	    m1 = mgau->mean[c];
	    v1 = mgau->var[c];
	    dval1 = mgau->lrd[c];
	    
	    for (i = 0; i < veclen; i++) {
		diff1 = x[i] - m1[i];
		dval1 -= diff1 * diff1 * v1[i];
	    }
	    
	    if (dval1 < g->distfloor)
		dval1 = g->distfloor;
	    
	    score = logs3_add (score, (int32)(f * dval1) + mgau->mixw[c]);
	}
    }
    
    return score;
}
#else /* unrolled or SIMD code */

#ifdef SSE2 /* SIMD code */


/* 
   inner loop of mgau_eval (simd version)
   returns dval1 
*/

float64 mgau_eval_inner(const mgau_t* mgau, const int32 veclen, const int32 c, 
			const float32 *x) {

  __declspec(align(16)) static const float32 zero[4] = {0.0, 0.0, 0.0, 0.0};
  __declspec(align(16)) static const float32 mask[4] = {1.0, 1.0, 1.0, 0.0};
  float32 *m1, *v1, tmp;
  float64 dval1, diff1;
  int32 i;

  m1 = mgau->mean[c];
  v1 = mgau->var[c];
  dval1 = mgau->lrd[c];
  i = (veclen+1)>>2;
  /*assert(veclen==39);*/
#if 0
  float32 *x_ptr, *m1_ptr, *v1_ptr;
  __m128 xmm_psum = _mm_load_ps(zero);

  x_ptr = x;
  m1_ptr = m1;
  v1_ptr = v1;
  
  __m128 xmm_mask = _mm_load_ps(mask);
  __m128d x_dval1 = _mm_load_sd(&dval1);
  
  for (i=0;i<=veclen;i+=4,v1_ptr+=4,m1_ptr+=4,x_ptr+=4) {
    __m128 xmm_v1 = _mm_load_ps(v1_ptr);
    __m128 xmm_diff = _mm_sub_ps(_mm_load_ps(x_ptr),_mm_load_ps (m1_ptr));
    xmm_psum = _mm_add_ps(xmm_psum,_mm_mul_ps(xmm_v1,_mm_mul_ps(xmm_diff, xmm_diff)));
  }

#if 0
  __m128 xmm_v1 = _mm_load_ps(v1_ptr);
  __m128 xmm_diff = _mm_sub_ps(_mm_load_ps(x_ptr),_mm_load_ps (m1_ptr));
  
  xmm_psum = _mm_add_ps(xmm_psum,_mm_mul_ps(xmm_mask,_mm_mul_ps(xmm_v1,_mm_mul_ps(xmm_diff, xmm_diff))));
#endif
  __m128 xmm_shuf = _mm_shuffle_ps(xmm_psum, xmm_psum,14);
  xmm_psum = _mm_add_ps(xmm_shuf,xmm_psum);

  __m128d xmm_tmp;
  xmm_tmp = _mm_cvtss_sd(xmm_tmp,_mm_add_ss(xmm_psum,_mm_shuffle_ps(xmm_psum, xmm_psum,1)));
  _mm_store_sd(&dval1, _mm_sub_sd(x_dval1,xmm_tmp));
    
#else /*end of intrinsics*/
  __asm
    {
      mov    ecx, [i]       ;
      movaps xmm7, [zero]        ;
      movaps xmm6, xmm7        ;
      mov    eax, [x]          ;
      mov    edx, [m1]         ;
      mov    ebx, [v1]         ;

    mgau_eloop:
#ifdef PREFETCH
      prefetchnta [ebx+320]        ;
      prefetchnta [edx+320]        ;
#endif
      movaps xmm0, [eax]        ; /*load 4 floats from x */
      movaps xmm1, [edx]        ; /*load 4 floats from m1*/
      movaps xmm2, [ebx]        ; /*load 4 floats from v1 */

      movaps xmm3, [eax+16]        ; /*load 4 floats from x */
      movaps xmm4, [edx+16]        ; /*load 4 floats from m1*/
      movaps xmm5, [ebx+16]        ; /*load 4 floats from v1 */

      add    eax, 32       ;
      add    ebx, 32       ;
      add    edx, 32       ;

      subps  xmm0, xmm1        ; /*xmm0 has x-m  */
      subps  xmm3, xmm4        ;

      mulps  xmm0, xmm0        ; /*xmm0 has (x-m)^2  */
      mulps  xmm3, xmm3        ; /*xmm0 has (x-m)^2  */

      mulps  xmm0, xmm2        ; /*xmm0 has (x-m)^2*v1 */
      mulps  xmm3, xmm5        ; /*xmm0 has (x-m)^2*v1 */

      addps  xmm7, xmm0        ; /*partial sum at xmm2*/
      addps  xmm6, xmm3        ; /*partial sum at xmm2*/

      sub     ecx, 2        ;
      cmp     ecx, 0        ;
      jnz     mgau_eloop    ;
    }
#if 0
#ifdef MGAU_NO_LOOP  /*39 elements assumed, undef to unassume */     

      movaps xmm0, [eax]        ; /*load 4 floats from x */
      movaps xmm1, [edx]        ; /*load 4 floats from m1*/
      subps  xmm0, xmm1        ; /*xmm0 has x-m  */
      movaps xmm1, [ebx]        ; /*load 4 floats from v1 */
      mulps  xmm0, xmm0        ; /*xmm0 has (x-m)^2  */
      mulps  xmm0, xmm1        ; /*xmm0 has (x-m)^2*v1 */
      mulps  xmm0, xmm3         ;
      addps  xmm2, xmm0        ; /*partial sum at xmm2*/
    }
#else
    }
  i <<= 2;

  for (;i<veclen; i++) {
    diff1 = x[i] - m1[i];
    dval1 -= diff1 * diff1 * v1[i];
  };
#endif
#endif
 /* reducing partial sum to tmp*/
  __asm
    {
      movsd  xmm4, [dval1]     ;, xmm4     ;
      addps  xmm7, xmm6        ;
      pshufd xmm1, xmm7, 14    ; /* move higher 2 floats to xmm1 */
      addps  xmm7, xmm1        ;
      pshufd xmm1, xmm7, 1     ; /* move 2nd lowest float to xmm1 */
      addss  xmm7, xmm1        ;
      cvtss2sd xmm7, xmm7      ;
      subsd  xmm4, xmm7        ;
      movsd  [dval1],xmm4      ;
      ;movss  [tmp], xmm7        ;
    };

/*dval1 -= tmp;*/
#endif

  return dval1;

}


#else /* unrolled code */

float64 mgau_eval_inner(const mgau_t* mgau, const int32 veclen, const int32 c, const float32 *x) {

  float32 *m1, *v1;
  float64 vdiff[4],vdval[4];
  float64 dval1, diff1;
  int32 i;

  m1 = mgau->mean[c];
  v1 = mgau->var[c];
  dval1 = mgau->lrd[c];

  /* clears vdval, reset v0 and start from the next record of m1 and v1 */
  vdval[0] = vdval[1] = vdval[2] = vdval[3] = 0.0;

  
#pragma novector
  for (i = 0; i < veclen-3; i+=4) {

    /*_mm_prefetch(&m1[i]+40,_MM_HINT_NTA);
      _mm_prefetch(&v1[i]+40,_MM_HINT_NTA);*/

    vdiff[0] = x[i] - m1[i];
    vdval[0] += vdiff[0] * vdiff[0] * v1[i];
    
    vdiff[1] = x[i+1] - m1[i+1];
    vdval[1] += vdiff[1] * vdiff[1] * v1[i+1];
    
    vdiff[2] = x[i+2] - m1[i+2];
    vdval[2] += vdiff[2] * vdiff[2] * v1[i+2];
    
    vdiff[3] = x[i+3] - m1[i+3];
    vdval[3] += vdiff[3] * vdiff[3] * v1[i+3];

  }

#ifndef MGAU_NO_LOOP

  for (;i<veclen; i++) {
    vdiff[0] = x[i] - m1[i];
    vdval[0] += vdiff[0] * vdiff[0] * v1[i];
  };
#else
    vdiff[0] = x[i] - m1[i];
    vdval[0] += vdiff[0] * vdiff[0] * v1[i];

    vdiff[1] = x[i+1] - m1[i+1];
    vdval[1] += vdiff[1] * vdiff[1] * v1[i+1];

    vdiff[2] = x[i+2] - m1[i+2];
    vdval[2] += vdiff[2] * vdiff[2] * v1[i+2];
#endif

  /* do the reduction */
  dval1 -= (vdval[0] + vdval[1] + vdval[2] +vdval[3] );

  return dval1;

}


#endif /* end of unrolled code */

int32 mgau_eval (mgau_model_t *g, int32 m, int32 *active, float32 *x)
{
    mgau_t *mgau;
    int32 veclen, score;
    float32 *m1, *m2, *v1, *v2;
    float64 dval1, /*dval2, diff1, diff2,*/ f;
    int32 i, j, c;
    /*  float64 vdiff[4],vdval[4]; */
    

    veclen = mgau_veclen(g);
    mgau = &(g->mgau[m]);
    f = log_to_logs3_factor();
    score = S3_LOGPROB_ZERO;
        
    if (! active) {	/* No short list; use all */



      for (c = 0; c < mgau->n_comp; c++) {

	dval1 = mgau_eval_inner(mgau, veclen, c, x);
	if (dval1 < g->distfloor)  dval1 = g->distfloor;

	score = logs3_add (score, (int32)(f * dval1) + mgau->mixw[c]);
      }
    } else {

      for (j = 0; active[j] >= 0; j++) {  /* start from j=1*/
	c = active[j];
	
	dval1 = mgau_eval_inner(mgau, veclen, c, x);
	if (dval1 < g->distfloor)  dval1 = g->distfloor;
	
	score = logs3_add (score, (int32)(f * dval1) + mgau->mixw[c]);
      }
    }

    return score;
}


#endif









/* RAH, free memory allocated in mgau_init
   I've not verified that this function catches all of the leaks, just most of them.
 */
void mgau_free (mgau_model_t *g)
{
  if (g) {
    if (g->mgau[0].mean) 
      ckd_free ((void *) g->mgau[0].mean);

    /* Free memory allocated for the var structure*/

    if (g->mgau[0].var) 
      ckd_free ((void *) g->mgau[0].var);
    if (g->mgau[0].lrd) 
      ckd_free ((void *) g->mgau[0].lrd);

    /* Free memory allocated for the mixture weights*/

    if (g->mgau[0].mixw) 
      ckd_free ((void *) g->mgau[0].mixw);
    
    if (g->mgau)
      ckd_free ((void *) g->mgau);
    ckd_free ((void *) g);
  }
}
