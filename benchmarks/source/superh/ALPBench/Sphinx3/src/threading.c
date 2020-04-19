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


#ifdef THRD
#include <assert.h>
#include <pthread.h>
#include "barrier.h"
#include "threading.h"

extern barrier_t *score_barrier;

//pip: pthread_mutex_t update_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t update_lock;

int thread_arrived = 0;
int wk_idx = 0;

thread_args_t thread_data_array[NUM_THREADS];
#if (NUM_THREADS>1)
pthread_t thread[NUM_THREADS-1];
#endif

int32 best_array[NUM_THREADS];
int32 wbest_array[NUM_THREADS];
int32 ns_array[NUM_THREADS];
int32 ng_array[NUM_THREADS];

int32 *he_best_array[NUM_THREADS];
int32 *he_wbest_array[NUM_THREADS];

lextree_node_t ***next_active_array[NUM_THREADS];
int32 *next_active_size[NUM_THREADS];
int32 *private_n[NUM_THREADS];

extern int VQ_EVAL;

lextree_node_t **double_next_active(int32 t, int32 ltn, int32* array_size)
{
  next_active_size[t][ltn] <<=1;
  next_active_array[t][ltn] = (lextree_node_t **) realloc(next_active_array[t][ltn],
			      next_active_size[t][ltn]*sizeof(lextree_node_t *));
  *array_size = next_active_size[t][ltn];
  return next_active_array[t][ltn];
}

void threading_support_init(kb_t* kb)
{ 
  int32 n, i;

  for (n=0; n<NUM_THREADS; n++) {
    next_active_size[n] = (int32 *) ckd_calloc(kb->n_lextree<<1,sizeof(int32));
    next_active_array[n] = (lextree_node_t***) 
      ckd_calloc(kb->n_lextree<<1, sizeof(lextree_node_t **));
    private_n[n] = (int32 *) ckd_calloc(kb->n_lextree<<1,sizeof(int32));

    for (i=0; i < (kb->n_lextree <<1); i++) {
      next_active_size[n][i] = 16;
      next_active_array[n][i] = (lextree_node_t**) 
	ckd_calloc(16, sizeof(lextree_node_t *));
    }

    he_best_array[n] = (int32 *) ckd_calloc(kb->n_lextree<<1,sizeof(int32));
    he_wbest_array[n] = (int32 *) ckd_calloc(kb->n_lextree<<1,sizeof(int32));
  }
}

static int32 thrd_subvq_mgau_shortlist (int t, 
					subvq_t *vq,
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
    gauscore = vq->thrd_gauscore[t];
    sl = vq->thrd_mgau_sl[t];
    
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


void *frame_eval_thrd_work(thread_args_t* thrd_args)
{
  int t = thrd_args->id;
  int32 start = thrd_args->feval_start;
  int32 end = thrd_args->feval_end;
  scoring_args_t* my_data = thrd_args->score_args;
  subvq_t *vq = my_data->vq;
  mgau_model_t *g = my_data->g;
  int32 beam = my_data->beam;
  float32 *feat = my_data->feat;
  int32 *sen_active = my_data->sen_active;
  int32 *senscr = my_data->senscr;

  int32 s;
  int32 best, ns, ng;
  
  best = MAX_NEG_INT32;
  ns = 0;
  ng = 0;

  if (!vq) {

    /* No subvq model, use the original (SLOW!!) */
    for (s = start; s < end; s++) {
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
    int go = -1;

    /* Evaluate subvq model for given feature vector */
    if (NUM_THREADS > 1) {
#ifdef NEW_EVAL_LOGS3
      thrd_subvq_gautbl_eval_logs3 (t, vq, feat);
#else
      pthread_mutex_lock(&update_lock); 
      
      go = wk_idx++;

      pthread_mutex_unlock(&update_lock); 

      if (NUM_THREADS >= vq->n_sv) {
	if (go < vq->n_sv) 
	  thrd_subvq_gautbl_eval_logs3 (go, vq, feat);
      } else {

 	while (go < vq->n_sv) {
	  thrd_subvq_gautbl_eval_logs3 (go, vq, feat);
	  
	  pthread_mutex_lock(&update_lock);
	  go = wk_idx++;
	  pthread_mutex_unlock(&update_lock);
 	};
      }
      
      thread_barrier(thrd_args->id,score_barrier); 
      if (DEBUG & 2) printf("thread %d barrier 0\n",thrd_args->id);
  
#endif /* end of NEW_EVAL_LOGS3 */

    } else /* NUM_THREADS == 1 */
      /*thrd_subvq_gautbl_eval_logs3(t,vq,feat);*/
      subvq_gautbl_eval_logs3(vq,feat);

    /* Find mixture component shortlists using subvq scores, and evaluate senones */
    for (s = start; s < end; s++) {
      if ((! sen_active) || sen_active[s]) {
	ng += thrd_subvq_mgau_shortlist (t, vq, s, mgau_n_comp(g,s), beam);
	
	senscr[s] = mgau_eval (g, s, vq->thrd_mgau_sl[t], feat);
	if (best < senscr[s])
	  best = senscr[s];
	
	ns++;
	
      } else
	senscr[s] = S3_LOGPROB_ZERO;
    }
  }

  best_array[t] = best;
  ns_array[t] = ns;
  ng_array[t] = ng;
#if (NUM_THREADS>1)
  pthread_mutex_lock(&update_lock);
#endif
  
  if (thread_arrived<NUM_THREADS-1) 
    thread_arrived++;
  else {
    thread_arrived = 0;
    best = best_array[0];
    for (s = 0; s< NUM_THREADS; s++) {
      if (best < best_array[s])
	best = best_array[s];
    }
    best_array[0] = best;
  }
#if (NUM_THREADS>1)    
  pthread_mutex_unlock(&update_lock);
#endif
  thread_barrier(thrd_args->id,score_barrier);
  if (DEBUG & 2) printf("thread %d barrier 0a\n",thrd_args->id);
  

  best = best_array[0];
  for (s = start; s < end; s++) {
    senscr[s] -= best;
  }
  return NULL;
}

void dict2pid_comsenscr_thrd_work(thread_args_t* thrd_args) 
{
  scoring_args_t *my_data = thrd_args->score_args;
  int32 start = thrd_args->com_start;
  int32 end = thrd_args->com_end;
  dict2pid_t *d2p = my_data->d2p;
  int32 *comsenscr = my_data->comsenscr;
  int32 *senscr = my_data->senscr;
  int32 i, j;
  int32 best;
  s3senid_t *comstate, k;

  for (i = start; i < end; i++) {

    comstate = d2p->comstate[i];
    
    best = senscr[comstate[0]];
    for (j = 1;; j++) {
      k = comstate[j];
      if (NOT_S3SENID(k))
	break;
      if (best < senscr[k])
	best = senscr[k];
    }
    
    comsenscr[i] = best + d2p->comwt[i];
  }
}

void lextree_hmm_eval_thrd_work(thread_args_t* thrd_args)
{
    int32 best, wbest, n_st;
    int32 i, k;
    const int32 start = thrd_args->hmmeval_start;
    const int32 end = thrd_args->hmmeval_end;
    const searching_args_t *s_args = thrd_args->search_args;
    const lextree_t *lextree = thrd_args->lextree;
    const kbcore_t *kbc = s_args->kbc;
    const ascr_t *ascr = s_args->ascr;
    const int32 frm = s_args->frm;
    FILE *fp = s_args->fp;

    lextree_node_t **list, *ln;
    mdef_t *mdef;
    dict2pid_t *d2p;
    
    mdef = kbc->mdef;
    d2p = kbc->dict2pid;
    n_st = mdef_n_emit_state (mdef);
    
    list = lextree->active;
    best = MAX_NEG_INT32;
    wbest = MAX_NEG_INT32;

    assert(!fp && n_st==3 && "not qualified");
    for (i = start; i < end; i++) {

      ln = list[i];
      
      assert (ln->frame == frm);
      
      if (! ln->composite)
	k = hmm_vit_eval_3st (&(ln->hmm), mdef->sseq[ln->ssid], ascr->sen);
      else
	k = hmm_vit_eval_3st (&(ln->hmm), d2p->comsseq[ln->ssid], ascr->comsen);
      
      if (best < k)
	best = k;
      
      if (IS_S3WID(ln->wid)) {
	if (wbest < k)
	  wbest = k;
      }
    }
    thrd_args->best = best;
    thrd_args->wbest = wbest;

}

int32 ln_lock(int32 t1, lextree_node_t*ln)
{
  int32 retry, skip=0;

  do 
    {
      retry = 0;
#if (NUM_THREADS>1)
      pthread_mutex_lock(&update_lock);
#endif
      if (ln->synch != 0) {
	if (ln->synch > t1) 
	  skip = 1;
	else
	  retry = 1;
      } else
	ln->synch = t1;
#if (NUM_THREADS>1)
      pthread_mutex_unlock(&update_lock);
#endif
    }
  while (retry);

  return skip;
}

int32 ln_lock_child(int32 t1, lextree_node_t*ln)
{
  int32 retry, skip=0;

  do 
    {
      retry = 0;
#if (NUM_THREADS>1)
      pthread_mutex_lock(&update_lock);
#endif
      if (ln->synch != 0) {
	retry = 1;
      } else
	ln->synch = t1;
      
#if (NUM_THREADS>1)
      pthread_mutex_unlock(&update_lock);
#endif
    }
  while (retry);
  
  return skip;
}

void ln_unlock(int32 t, lextree_node_t*ln)
{
#if (NUM_THREADS>1)
  pthread_mutex_lock(&update_lock);
#endif  
  ln->synch = 0;

#if (NUM_THREADS>1)  
  pthread_mutex_unlock(&update_lock);
#endif
}


void lextree_hmm_ppg_thrd_work(thread_args_t* thrd_args)
{
    mdef_t *mdef;
    int32 nf, newscore;
    const lextree_t *lextree = thrd_args->lextree;
    lextree_node_t **list = lextree->active;
    lextree_node_t *ln, *ln2;
    hmm_t *hmm, *hmm2;
    gnode_t *gn;
    int32 i, n;
    const int32 t = thrd_args->id;
    const int32 t1 = t+1;
    const int32 ltn = thrd_args->lextree_num;

    const int32 start = thrd_args->hmmppg_start;
    const int32 end = thrd_args->hmmppg_end;
    const searching_args_t *s_args = thrd_args->search_args;

    kbcore_t *kbc = s_args->kbc;
    const int32 cf = s_args->frm;
    const int32 th = s_args->th;
    const int32 wth = s_args->wth;
    const int32 pth = s_args->pth;
    vithist_t* vh = s_args->vithist;
    lextree_node_t **next_active = next_active_array[t][ltn];
    int32 next_active_array_size = next_active_size[t][ltn];
    
    mdef = kbcore_mdef(kbc);
    
    nf = cf+1;
    
    /*list = lextree->active;*/
    
    assert(lextree->n_next_active==0);
    n = 0;

    for (i = start; i < end; i++) {
      ln = list[i];
      hmm = &(ln->hmm);

      if ((!ln_lock(t1,ln)) && (ln->frame < nf)) {
	if (hmm->bestscore >= th) {		/* Active in next frm */

	  ln->frame = nf;

	  if (n==next_active_array_size) 
	    next_active = double_next_active(t, ltn, &next_active_array_size);
	  next_active[n++] = ln;
	} else {				/* Deactivate */

	  if (ln->frame !=nf) {
	    ln->frame = -1;
	    hmm_clear (hmm, mdef_n_emit_state(mdef));
	  };
	}
      }

      ln_unlock(t1,ln);
      
      if (NOT_S3WID(ln->wid)) {		/* Not a leaf node */
	
	if (hmm->out.score < pth)
	  continue;			/* HMM exit score not good enough */
	
	/* Transition to each child */
	for (gn = ln->children; gn; gn = gnode_next(gn)) {
	  ln2 = gnode_ptr(gn);
	  hmm2 = &(ln2->hmm);

	  if (!ln_lock_child(t1,ln2)) {
	    newscore = hmm->out.score + (ln2->prob - ln->prob);
	  
	    if ((newscore >= th) && (hmm2->in.score < newscore)) {
	      hmm2->in.score = newscore;
	      hmm2->in.history = hmm->out.history;
	      
	      if (ln2->frame != nf) {
		ln2->frame = nf;
		
		if (n==next_active_array_size) {
		  next_active = double_next_active(t, ltn, &next_active_array_size);
		}
		next_active[n++] = ln2;
	      }
	    }
	  }
	  ln_unlock(t1,ln2);
	}
      } else {			/* Leaf node; word exit */
	    if (hmm->out.score < wth)
		continue;		/* Word exit score not good enough */
	    
	    /* Rescore the LM prob for this word wrt all possible predecessors */

	    /* The children functions have a couple shared variables, more 
	       investigations can unravel them to have less conservative 
	       fine-grained locking    */
#if (NUM_THREADS>1)
	    pthread_mutex_lock(&update_lock);
#endif
	    vithist_rescore (vh, kbc, ln->wid, cf,
			     hmm->out.score - ln->prob, hmm->out.history, lextree->type);
#if (NUM_THREADS>1)
	    pthread_mutex_unlock(&update_lock);
#endif
      }
    }
    if (DEBUG & 0x2) printf("thrd %d lextree %d n %d\n",t,ltn,n);
    private_n[t][ltn] = n;

}

void ThreadJoin() {
#if (NUM_THREADS>1)
  int t,rc, status;

  for(t=0;t < NUM_THREADS-1;t++) {
    rc = pthread_join(thread[t], (void **)&status);
    if (rc)	  {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
  }
#endif
}

void lextree_ssid_active_thrd_work (thread_args_t* t_args, int32 *ssid, 
				    int32 *comssid)
{
  lextree_t *lextree = t_args->lextree;
  lextree_node_t **list, *ln;
  int32 i;
  int start = t_args->hmmeval_start;
  int end = t_args->hmmeval_end;
  
  list = lextree->active;
  
  for (i = start; i < end; i++) {
    ln = list[i];
    if (ln->composite) 
      comssid[ln->ssid] = 1;
    else 
      ssid[ln->ssid] = 1;
  }
}

void sseq2sen_active_thrd_work(thread_args_t* t_args)
{
  int32 ss, cs, i, j;
  s3senid_t *csp, *sp;	/* Composite state pointer */
  mdef_t*mdef = t_args->sa_args->mdef;
  dict2pid_t*d2p = t_args->sa_args->d2p;
  kb_t* kb = t_args->sa_args->kb;
  int32 *sseq = kb->ssid_active;
  int32 *comssid = kb->comssid_active;
  int32 *sen = kb->sen_active;
  int32 n_sseq =  mdef_n_sseq(mdef);
  int32 n_comsseq = d2p->n_comsseq;
  int32 s_start, s_end, c_start, c_end;
  int32 mdef_chunk_size = n_sseq / NUM_THREADS;
  int32 mdef_remainder = n_sseq % NUM_THREADS;
  int32 d2p_chunk_size = n_comsseq / NUM_THREADS;
  int32 d2p_remainder = n_comsseq % NUM_THREADS;
  int32 t = t_args->id;

  s_start = (t==0) ? 0 : 
    (t*mdef_chunk_size + ((t <= mdef_remainder) ? t : mdef_remainder));
  s_end = s_start + mdef_chunk_size + 
    ((t < mdef_remainder) ? 1 : 0);
 
  c_start = (t==0) ? 0 : 
    (t*d2p_chunk_size + ((t <= d2p_remainder) ? t : d2p_remainder));
  c_end = c_start + d2p_chunk_size + 
    ((t < d2p_remainder) ? 1 : 0);
 
  for (ss = s_start; ss < s_end; ss++) {
    if (sseq[ss]) {
      sp = mdef->sseq[ss];
      for (i = 0; i < mdef_n_emit_state(mdef); i++)
	sen[sp[i]] = 1;
    }
  }
  
  for (ss = c_start; ss < c_end; ss++) {
    if (comssid[ss]) {
      csp = d2p->comsseq[ss];
      
      for (i = 0; i < mdef_n_emit_state(mdef); i++) {
	cs = csp[i];
	sp = d2p->comstate[cs];
	
	for (j = 0; IS_S3SENID(sp[j]); j++)
	  sen[sp[j]] = 1;
      }
    }
  }
}

void sen_active_thrd_work(thread_args_t* t_args) 
{
  int32 i;
  const int t = t_args->id;
  int32 chunk_size, remainder;
  int32 s, e;
  lextree_t* lextree;
  const kb_t* kb = t_args->sa_args->kb;
  const mdef_t *mdef = t_args->sa_args->mdef;
  const dict2pid_t *d2p = t_args->sa_args->d2p;
  int32* array_ptr;

  chunk_size = mdef_n_sseq(mdef) / NUM_THREADS;
  remainder = mdef_n_sseq(mdef) % NUM_THREADS;

  s = (t==0) ? 0 : (t*chunk_size + ((t <= remainder) ? t : remainder));
  e =  s + chunk_size + ((t < remainder) ? 1 : 0);
  array_ptr = kb->ssid_active;

#ifdef SSE2
  {
    int32* chunk_ptr = &array_ptr[s]   ;
    int32* end_ptr   = &array_ptr[e-3] ;

    while (((int)chunk_ptr)%16)
      *chunk_ptr++ = 0;
    
    __asm {
      pxor    xmm0, xmm0          ;
      mov     eax, [chunk_ptr]    ;
      mov     ecx, [end_ptr]      ;

    zero_out1:
      movdqa  [eax], xmm0         ;
      add     eax, 16             ;
      cmp     eax, ecx            ;
      jl      zero_out1           ;
      mov     [chunk_ptr], eax    ;
    }
    end_ptr+=3;
    for (;chunk_ptr<end_ptr;chunk_ptr++)
      *chunk_ptr=0;
  }
#else
#pragma novector
  for (i=s; i<e; i++)
    array_ptr[i]=0;
#endif
  
  chunk_size = dict2pid_n_comsseq(d2p) / NUM_THREADS;
  remainder = dict2pid_n_comsseq(d2p) % NUM_THREADS;

  s = (t==0) ? 0 : (t*chunk_size + ((t <= remainder) ? t : remainder));
  e =  s + chunk_size + ((t < remainder) ? 1 : 0);
  array_ptr = kb->comssid_active;
  
#ifdef SSE2
  {
    int32* chunk_ptr = &array_ptr[s]   ;
    int32* end_ptr   = &array_ptr[e-3] ;
    
    while (((int)chunk_ptr)%16)
      *chunk_ptr++ = 0;

    __asm {
      pxor    xmm0, xmm0          ;
      mov     eax, [chunk_ptr]    ;
      mov     ecx, [end_ptr]      ;

    zero_out2:
      movdqa  [eax], xmm0         ;
      add     eax, 16             ;
      cmp     eax, ecx            ;
      jl      zero_out2           ;
      mov     [chunk_ptr], eax    ;
    }
    end_ptr+=3;
    for (;chunk_ptr<end_ptr;chunk_ptr++)
      *chunk_ptr=0;
  }
#else
#pragma novector
  for (i=s; i<e; i++)
    array_ptr[i]=0;
#endif
  
  chunk_size = mdef_n_sen(mdef) / NUM_THREADS;
  remainder = mdef_n_sen(mdef) % NUM_THREADS;

  s = (t==0) ? 0 : (t*chunk_size + ((t <= remainder) ? t : remainder));
  e =  s + chunk_size + ((t < remainder) ? 1 : 0);
  array_ptr = kb->sen_active;
  
#ifdef SSE2
  {
    int32* chunk_ptr = &array_ptr[s]   ;
    int32* end_ptr   = &array_ptr[e-3] ;
    
    while (((int)chunk_ptr)%16)
      *chunk_ptr++ = 0;

    __asm {
      pxor    xmm0, xmm0          ;
      mov     eax, [chunk_ptr]    ;
      mov     ecx, [end_ptr]      ;

    zero_out3:
      movdqa  [eax], xmm0         ;
      add     eax, 16             ;
      cmp     eax, ecx            ;
      jl      zero_out3           ;
      mov     [chunk_ptr], eax    ;
    }
    end_ptr+=3;
    for (;chunk_ptr<end_ptr;chunk_ptr++)
      *chunk_ptr=0;
  }
#else
#pragma novector
  for (i=s; i<e; i++)
   array_ptr[i]=0;
#endif

  thread_barrier(t_args->id,score_barrier);
  
  for (i = 0; i < (kb->n_lextree <<1); i++) {

    lextree = (i < kb->n_lextree) ? kb->ugtree[i] : 
      kb->fillertree[i - kb->n_lextree];

    if (lextree->n_active >= NUM_THREADS) {
      chunk_size = lextree->n_active / NUM_THREADS;
      remainder = lextree->n_active % NUM_THREADS;

      t_args->lextree = lextree;
      t_args->hmmeval_start = (t==0) ? 0 : 
	(t*chunk_size + ((t <= remainder) ? t : remainder));
      t_args->hmmeval_end = t_args->hmmeval_start + chunk_size + 
	((t < remainder) ? 1 : 0);
      
      lextree_ssid_active_thrd_work(t_args, kb->ssid_active, 
				    kb->comssid_active);
      
    } else if (t==NUM_THREADS-1) {
      
      t_args->lextree = lextree;
      t_args->hmmeval_start = 0;
      t_args->hmmeval_end = lextree->n_active;
      
      lextree_ssid_active_thrd_work(t_args, kb->ssid_active, 
				    kb->comssid_active);
    };
  }
    
  thread_barrier(t_args->id,score_barrier);
  sseq2sen_active_thrd_work(t_args);
}

void *scoring_thrd_work(void* thrd_args)
{
  
  thread_args_t* t_args = (thread_args_t*) thrd_args;

  sen_active_thrd_work(t_args);
  thread_barrier(t_args->id,score_barrier);
  thread_barrier(t_args->id,score_barrier);

  frame_eval_thrd_work(t_args);

  thread_barrier(t_args->id,score_barrier);
  if (DEBUG & 2) printf("thread %d barrier 1\n",t_args->id);

  dict2pid_comsenscr_thrd_work(t_args);

  if (PHASES != 0x1) {
    if (PHASES & 0x2) {
      thread_barrier(t_args->id,score_barrier);
    
      thread_barrier(t_args->id, score_barrier);
      if (DEBUG & 2) printf("thread %d barrier 2\n",t_args->id);
      
      new_lextree_hmm_eval_thrd_work(thrd_args);
    }
    
    if (PHASES & 0x4) {
      thread_barrier(t_args->id, score_barrier);
      if (DEBUG & 2) printf("thread %d barrier 3\n",t_args->id);

      thread_barrier(t_args->id, score_barrier);
      if (DEBUG & 2) printf("thread %d barrier 4\n",t_args->id);
      
      new_lextree_hmm_ppg_thrd_work(thrd_args);
    };
  }

  return NULL;
}


void thrd_sen_active_phase(sen_active_args_t* sa_args)
{
#if (NUM_THREADS > 1)
  pthread_attr_t attr;
  int rc;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif
  int32 t;

  for(t=0;t < NUM_THREADS;t++) {
    thread_data_array[t].sa_args = sa_args;
    thread_data_array[t].id = t;

    /* create threads */
#if (NUM_THREADS > 1)
    if (t < NUM_THREADS-1) {
	rc = pthread_create(&thread[t], &attr, scoring_thrd_work,
			    (void*) &thread_data_array[t]); 

	if (rc) {
	  printf("ERROR; return code from pthread_create() is %d\n", rc);
	  exit(-1);
	}
    }
#endif
  }
#if (NUM_THREADS > 1)
  pthread_attr_destroy(&attr);
#endif
  sen_active_thrd_work(&thread_data_array[NUM_THREADS-1]);

  thread_barrier(NUM_THREADS-1,score_barrier);

}

void thrd_scoring_phase(scoring_args_t* score_args)
{
  int rc;
  int t;
  int feval_chunk_size, feval_remainder, feval_tmp;
  int com_chunk_size, com_remainder, com_tmp;
  int32 ns, ng, s;

  feval_chunk_size = score_args->g->n_mgau / NUM_THREADS;
  feval_remainder = score_args->g->n_mgau % NUM_THREADS;

  com_chunk_size = score_args->d2p->n_comstate / NUM_THREADS;
  com_remainder = score_args->d2p->n_comstate % NUM_THREADS;

  wk_idx = 0;

  for(t=0;t < NUM_THREADS;t++) {
    if (t==0) {
      thread_data_array[t].feval_start = 0;
      thread_data_array[t].com_start = 0;
    } else {
      thread_data_array[t].feval_start = thread_data_array[t-1].feval_end;
      thread_data_array[t].com_start = thread_data_array[t-1].com_end;
    }

    feval_tmp = thread_data_array[t].feval_start + feval_chunk_size;
    com_tmp = thread_data_array[t].com_start + com_chunk_size;
      
    if (feval_remainder) {
      feval_tmp++;
      feval_remainder--;
    }

    if (com_remainder) {
      com_tmp++;
      com_remainder--;
    }
      
    thread_data_array[t].feval_end = feval_tmp;
    thread_data_array[t].com_end = com_tmp;
    thread_data_array[t].score_args = score_args;
    
    thread_data_array[t].id = t;

  }

  thread_barrier(NUM_THREADS-1,score_barrier);
  
  frame_eval_thrd_work(&thread_data_array[NUM_THREADS-1]);

  thread_barrier(NUM_THREADS-1,score_barrier);
  if (DEBUG & 2) printf("thread %d barrier 1\n",NUM_THREADS-1);

  ns = ng = 0;
  for (s = 0; s< NUM_THREADS; s++) {
    ns += ns_array[s];
    ng += ng_array[s];
  }

  score_args->g->frm_sen_eval = ns;
  score_args->g->frm_gau_eval = ng;

  dict2pid_comsenscr_thrd_work(&thread_data_array[NUM_THREADS-1]);

  if (PHASES == 0x1)
    ThreadJoin();
  else {
    thread_barrier(NUM_THREADS-1,score_barrier);
    if (DEBUG & 2) printf("thread %d barrier 1\n",NUM_THREADS-1);
  };
}

void new_lextree_hmm_eval_thrd_work(thread_args_t* thrd_args)
{
  int32 i;
  int t = thrd_args->id;
  int chunk_size, remainder;
  const kb_t* kb = thrd_args->search_args->kb;
  lextree_t* lextree;

  for (i = 0; i < (kb->n_lextree <<1); i++) {

    lextree = (i < kb->n_lextree) ? kb->ugtree[i] : 
      kb->fillertree[i - kb->n_lextree];

    if (lextree->n_active >= NUM_THREADS) {
      chunk_size = lextree->n_active / NUM_THREADS;
      remainder = lextree->n_active % NUM_THREADS;

      thrd_args->lextree = lextree;
      thrd_args->hmmeval_start = (t==0) ? 0 : 
	(t*chunk_size + ((t <= remainder) ? t : remainder));
      thrd_args->hmmeval_end = thrd_args->hmmeval_start + chunk_size + 
	((t < remainder) ? 1 : 0);

      if (DEBUG&0x2) printf("thread %d lextree %d start %d end %d n_active %d\n",
			t, i,thrd_args->hmmeval_start, 
			thrd_args->hmmeval_end,lextree->n_active);
      lextree_hmm_eval_thrd_work(thrd_args);

      he_best_array[t][i] = thrd_args->best;
      he_wbest_array[t][i] = thrd_args->wbest;
    } else if (t==NUM_THREADS-1) {
      thrd_args->lextree = lextree;
      thrd_args->hmmeval_start = 0;
      thrd_args->hmmeval_end = lextree->n_active;

      lextree_hmm_eval_thrd_work(thrd_args);

      he_best_array[t][i] = thrd_args->best;
      he_wbest_array[t][i] = thrd_args->wbest;
    };
    
  }
}

void new_thrd_lextree_hmm_eval(searching_args_t* search_args, int32* besthmmscr,
			       int32* bestwordscr, int32 *n_hmm_eval, 
			       int32 *frm_nhmm)
{
  int32 t, i, best, wbest;
  kb_t * kb = search_args->kb;
  lextree_t* lextree;
  
  for(t=0;t < NUM_THREADS;t++) 
    thread_data_array[t].search_args = search_args;
  
  thread_barrier(NUM_THREADS-1,score_barrier);
  if (DEBUG & 2) printf("thread %d barrier 2\n",NUM_THREADS-1);
  
  new_lextree_hmm_eval_thrd_work(&thread_data_array[NUM_THREADS-1]);
  
  if (PHASES >= 0x4) {
    thread_barrier(NUM_THREADS-1,score_barrier);
    if (DEBUG & 2) printf("thread %d barrier 3\n",NUM_THREADS-1);
  } else
    ThreadJoin();

  best = MAX_NEG_INT32;
  wbest = MAX_NEG_INT32;

  for (i = 0; i < (kb->n_lextree <<1); i++) {
    lextree = (i < kb->n_lextree) ? kb->ugtree[i] : 
      kb->fillertree[i - kb->n_lextree];

    if (lextree->n_active) {
      best = he_best_array[NUM_THREADS-1][i];
      wbest = he_wbest_array[NUM_THREADS-1][i];
    }
    
    if (lextree->n_active >= NUM_THREADS) {
      for (t = 0; t<NUM_THREADS-1; t++) {
	if (best < he_best_array[t][i])
	  best = he_best_array[t][i];
	if (wbest < he_wbest_array[t][i])
	  wbest = he_wbest_array[t][i];
      }
    }
    
    lextree->best = best;
    lextree->wbest = wbest;
    
    if (*besthmmscr < best)
      *besthmmscr = best;
    if (*bestwordscr < wbest)
      *bestwordscr = wbest;

    *n_hmm_eval += lextree->n_active;
    *frm_nhmm += lextree->n_active;
  }

}

void new_lextree_hmm_ppg_thrd_work(thread_args_t* thrd_args)
{
  int32 i;
  int t = thrd_args->id;
  int chunk_size, remainder;
  kb_t* kb = thrd_args->search_args->kb;
  lextree_t* lextree;

  for (i = 0; i < (kb->n_lextree <<1); i++) {
    lextree = (i < kb->n_lextree) ? kb->ugtree[i] : 
      kb->fillertree[i - kb->n_lextree];
    
    if (lextree->n_active >= NUM_THREADS) {
      chunk_size = lextree->n_active / NUM_THREADS;
      remainder = lextree->n_active % NUM_THREADS;
      
      thrd_args->lextree = lextree;
      thrd_args->hmmppg_start = (t==0) ? 0 : 
	(t*chunk_size + ((t <= remainder) ? t : remainder));
      thrd_args->hmmppg_end = thrd_args->hmmppg_start + chunk_size + 
	((t < remainder) ? 1 : 0);
      thrd_args->lextree_num = i;
      
      lextree_hmm_ppg_thrd_work(thrd_args);

    } else if (t==NUM_THREADS-1) {
      thrd_args->lextree = lextree;
      thrd_args->hmmppg_start = 0;
      thrd_args->hmmppg_end = lextree->n_active;
      thrd_args->lextree_num = i;
      
      lextree_hmm_ppg_thrd_work(thrd_args);

    };
    
  }
}

void new_thrd_lextree_hmm_propagate(searching_args_t* search_args)
{
  int32 t, i;
  kb_t * kb = search_args->kb;
  lextree_t* lextree;
  int32 sum;
  
  for(t=0;t < NUM_THREADS;t++) 
    thread_data_array[t].search_args = search_args;
  
  thread_barrier(NUM_THREADS-1,score_barrier);
  if (DEBUG & 2) printf("thread %d barrier 4\n",NUM_THREADS-1);
    
  new_lextree_hmm_ppg_thrd_work(&thread_data_array[NUM_THREADS-1]);
  
#if (NUM_THREADS>1)
  ThreadJoin();
#endif

  for (i=0; i < (kb->n_lextree << 1); i++) {
    lextree = (i < kb->n_lextree) ? kb->ugtree[i] :
      kb->fillertree[i - kb->n_lextree];
    
    sum = 0;
    
    if (lextree->n_active >= NUM_THREADS) {
    
      for (t=0; t<NUM_THREADS; t++) {
	if (private_n[t][i]) {
	  memcpy(&lextree->next_active[sum], next_active_array[t][i],
		 private_n[t][i]*sizeof(lextree_node_t *));
	  sum += private_n[t][i];
	}
      } 
      
      lextree->n_next_active = sum;
    } else if (private_n[NUM_THREADS-1][i]) {
      
      memcpy(lextree->next_active, next_active_array[NUM_THREADS-1][i],
	     private_n[NUM_THREADS-1][i]*sizeof(lextree_node_t *));
      
      lextree->n_next_active = private_n[NUM_THREADS-1][i];
    } else {
      lextree->n_next_active = 0;
    }
  } 

}

#endif
