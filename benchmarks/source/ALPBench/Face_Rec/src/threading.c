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
#include <stdlib.h>
#include "threading.h"

pthread_t thread[NUM_THREADS];

barrier_t* thrd_barrier;

barrier_t * thread_barrier_init(int n_clients) {
  barrier_t *barrier;

#if (NUM_THREADS>1)

  barrier = (barrier_t *) malloc(sizeof(barrier_t));

  if (barrier != NULL) {
    barrier->n_clients = n_clients;
    barrier->n_waiting = 0;
    barrier->phase = 0;
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->wait_cv, NULL);
  }
#endif
  return barrier;
}

void thread_barrier_destroy(barrier_t *barrier) {
#if (NUM_THREADS>1)
  pthread_mutex_destroy(&barrier->lock);
  pthread_cond_destroy(&barrier->wait_cv);
  free(barrier);
#endif
}

void thread_barrier(int t, barrier_t *barrier) {
#if (NUM_THREADS>1)
  int my_phase;

  pthread_mutex_lock(&barrier->lock);
  my_phase = barrier->phase;
  barrier->n_waiting++;

  if (barrier->n_waiting == barrier->n_clients) {
    barrier->n_waiting = 0;
    barrier->phase = 1 - my_phase;
    pthread_cond_broadcast(&barrier->wait_cv);
  }

  while (barrier->phase == my_phase) {
    pthread_cond_wait(&barrier->wait_cv, &barrier->lock);
  }

  pthread_mutex_unlock(&barrier->lock);
#endif
}

thrd_args_t thrd_data_array[NUM_THREADS];

void thrd_mean_subtract_images(Matrix images, Matrix mean, int s, int e)
{
  FTYPE *img_ptr, *mn_ptr;
#ifdef SSE2
  int i;

  img_ptr = &ME(images, s, 0);
  mn_ptr = &ME(mean, s, 0);

  /* In order to use packed aligned load, need to loop leep. */
  if (s&1) {
    *img_ptr -= *mn_ptr;
    s++; img_ptr++; mn_ptr++;
  }

  i = (e-s)>>1;

  __asm
    {
      mov      ecx, [i]    ;
      mov      edx, [mn_ptr]     ;
      mov      ebx, [img_ptr]    ;
      
    loop_label:
      movapd   xmm0, [ebx]             ; /*load 2 doubles from images*/
      movapd   xmm1, [edx]             ; /*load 2 doubles from mean */
      subpd    xmm0, xmm1              ; /*xmm0 has (img-mn) */
      movapd   [ebx], xmm0             ;     
      add      ebx, 16                 ;
      add      edx, 16                 ;
      sub      ecx, 1                  ;
      cmp      ecx, 0                  ;
      jnz      loop_label              ;
    }
  i <<= 1;
  i = s + i;

#else
  int i;
  img_ptr = &ME(images, s, 0);
  mn_ptr = &ME(mean, s, 0);
 
  for (i = s; i < e-1; i+=2) {
    *img_ptr -= *mn_ptr;
    *(img_ptr+1) -= *(mn_ptr+1);
    img_ptr+=2; mn_ptr+=2;
    /*ME(images, i, 0) -= ME(mean, i, 0);
      ME(images, i+1, 0) -= ME(mean, i+1, 0);*/
  }
#endif  

  if (i<e)
    ME(images, i, 0) -= ME(mean, i, 0);

}

void thrd_transposeMultiplyMatrixL(Matrix A, Matrix B, 
				   Matrix P, int sa, int ea)
{
  int i, j, k;
  double sum;
  __declspec(align(16)) static const double packed_zero[2] = {0.0, 0.0};
  int a_row_dim = A->row_dim;

#ifdef SSE2

  for ( j = 0; j < B->col_dim; j++) {    /* orig: B->coldim (== 1) */
    double *B_addr = &ME(B, 0, j);
    double *A_addr = &ME(A, 0, sa);
    
    for ( i = sa; i < ea; i++) {           /* original; A->col_dim */
           
      sum = 0.0;

      k = a_row_dim >> 1;
      __asm
	{
	  movapd xmm2, [packed_zero]     ; /*packed sum*/
	  mov    ebx, [A_addr]    ;
	  mov    edx, [B_addr]    ;
	  mov    ecx, [k]         ;
	  sar    ecx, 1           ;

	tmml_loop:
	  movapd   xmm0, [ebx]             ; /*load 2 doubles from A*/
	  movapd   xmm1, [edx]             ; /*load 2 doubles from B */
	  movapd   xmm3, [ebx+16]             ; /*load 2 doubles from A*/
	  movapd   xmm4, [edx+16]             ; /*load 2 doubles from B */
	  mulpd    xmm0, xmm1              ; 
	  mulpd    xmm3, xmm4              ; 
	  addpd    xmm2, xmm0              ; /*accumulate partial sums*/
	  addpd    xmm2, xmm3              ; /*accumulate partial sums*/
	  add      ebx, 32                 ;
	  add      edx, 32                 ;
	  sub      ecx, 1                  ;
	  cmp      ecx, 0                  ;
	  jnz      tmml_loop               ;
	  
	  pshufd   xmm0, xmm2, 14          ; /*reduction */
	  addsd    xmm0, xmm2              ;
	  movsd    [sum], xmm0 ;
	    
	}
      
      ME(P, i, j) = sum;
      A_addr+=a_row_dim;
    }
  }

#else  
  FTYPE sum1;
  
  for ( j = 0; j < B->col_dim; j++) {    /* orig: B->coldim (== 1) */
    double *A_addr = &ME(A, 0, sa);
    
    for ( i = sa; i < ea; i++) {           /* original; A->col_dim */
      double *B_addr = &ME(B, 0, j);

      sum = sum1=0.0;

      for (k = 0; k < a_row_dim; k+=2) {
	sum+= (*A_addr) * (*B_addr);
	sum1+= (*(A_addr+1)) * (*(B_addr+1));
	A_addr+=2;B_addr+=2;
	/*sum += ME(A, k, i) * ME(B, k, j);*/
      }
      sum += sum1;
      ME(P, i, j) = sum;
    }
  }
#endif
}

void ThreadJoin() 
{ 
#if (NUM_THREADS>1)
  int t, rc, status;
  for(t=0;t < NUM_THREADS-1;t++) {
    rc = pthread_join(thread[t], (void **)&status);
    if (rc)	  {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
  }
#endif
}

void* main_thrd_work(void* thrd_args) {
  thrd_args_t *my_data = (thrd_args_t*) thrd_args;
  int t = my_data->id;
  Matrix images = my_data->images;
  int chunk = images->row_dim / NUM_THREADS;
  int remainder = images->row_dim % NUM_THREADS;
  Subspace *s = my_data->s;
  int start;
  int end;

  start = (t==0) ? 0 : t*chunk;
  start += (t <= remainder) ? t:remainder;
  end = (t < remainder)? 1:0;
  end += start + chunk;

  /*printf("thread %d sub start %d end %d\n",t,start,end);*/
  thrd_mean_subtract_images(images, s->mean, start, end);

  thread_barrier(t, thrd_barrier);

  /* the mean subtract is in synch */

  chunk = s->basis->col_dim / NUM_THREADS;
  remainder = s->basis->col_dim % NUM_THREADS;

  start = (t==0) ? 0 : t*chunk;
  start += (t <= remainder) ? t:remainder;
  end = (t < remainder)? 1:0;
  end += start + chunk;
  
  /*printf("thread %d mul start %d end %d\n",t,start,end);*/
  thrd_transposeMultiplyMatrixL(s->basis, images, my_data->subspims,
				start, end);

  return NULL;
}

Matrix subspims;

Matrix thrd_centerthenproject(Subspace *s, Matrix images) {
  int t;
  int chunk;
  int remainder;
  int start;
  int end;
#if (NUM_THREADS>1)
  pthread_attr_t attr;
  int rc;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  thrd_barrier = thread_barrier_init(NUM_THREADS);
#endif

  subspims = makeMatrix(s->basis->col_dim, images->col_dim);

  for(t=0; t < NUM_THREADS; t++) {
    
    thrd_data_array[t].id = t;
    
    thrd_data_array[t].images = images;
    thrd_data_array[t].s = s;
    thrd_data_array[t].subspims = subspims;
    
#if (NUM_THREADS>1)
    /* create threads */
    if (t!=NUM_THREADS-1) {
      rc = pthread_create(&thread[t], &attr, main_thrd_work, 
			  (void*) &thrd_data_array[t]); 
      if (rc) {
	printf("ERROR; return code from pthread_create() is %d\n", rc);
	exit(-1);
      }
    }
#endif
  }

#if (NUM_THREADS>1)
  /* Free attribute and wait for the other threads */
  pthread_attr_destroy(&attr);
#endif

  main_thrd_work(&thrd_data_array[NUM_THREADS-1]);
  ThreadJoin();
  return subspims;
}


#endif /* end ifdef THRD */
