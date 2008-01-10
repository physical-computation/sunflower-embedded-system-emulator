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


#include "subvq.h"
#include "dict2pid.h"
#include "lextree.h"
#include "kbcore.h"
#include "kb.h"
#include "ascr.h"
//pip: #include <pthread.h>

typedef struct {
  kb_t *kb;
  mdef_t *mdef;
  dict2pid_t *d2p;
} sen_active_args_t;

typedef struct {
  subvq_t* vq;
  mgau_model_t * g;
  int32 beam;
  float32 *feat;
  int32* sen_active;
  int32* senscr;
  
  dict2pid_t *d2p;
  int32 *comsenscr;
} scoring_args_t;

typedef struct {
  kb_t *kb;
  lextree_t *lextree;
  kbcore_t *kbc;
  ascr_t *ascr;
  int32 frm;
  FILE *fp;

  int32 ptranskip;
  vithist_t *vithist;
  int32 th, pth, wth;
  
} searching_args_t;

typedef struct {
  int id;
  int32 feval_start, feval_end;
  int32 com_start, com_end;
  int32 hmmeval_start, hmmeval_end;
  int32 hmmppg_start, hmmppg_end;

  int32 best, wbest; /* hmm_eval results */
  int32 lextree_num;
#ifdef NEW_PAR
  int32 n_mgau, n_comstate;
#endif
  lextree_t* lextree;
  sen_active_args_t *sa_args;
  scoring_args_t *score_args;
  searching_args_t *search_args;
} thread_args_t;

void ThreadJoin();
void threading_support_init(kb_t* kb);
void *frame_eval_thrd_work(thread_args_t* thrd_args);

void dict2pid_comsenscr_thrd_work(thread_args_t* thrd_args);
void lextree_hmm_eval_thrd_work(thread_args_t* thrd_args);
void lextree_hmm_ppg_thrd_work(thread_args_t* thrd_args);
void thrd_scoring_phase(scoring_args_t* score_args);
void new_thrd_lextree_hmm_eval(searching_args_t* search_args, int32* besthmmscr,
			       int32* bestwordscr, int32 *n_hmm_eval, 
			       int32 *frm_nhmm);
void new_lextree_hmm_eval_thrd_work(thread_args_t* thrd_args);
void new_thrd_lextree_hmm_propagate(searching_args_t* search_args);
void new_lextree_hmm_ppg_thrd_work(thread_args_t* thrd_args);

