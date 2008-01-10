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


/* motion.c, motion vector decoding                                         */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

#include <stdio.h>

#include "config.h"
#include "global.h"

/* private prototypes */
static void decode_motion_vector _ANSI_ARGS_((int *pred, int r_size, int motion_code,
  int motion_residualesidual, int full_pel_vector));

/* ISO/IEC 13818-2 sections 6.2.5.2, 6.3.17.2, and 7.6.3: Motion vectors */
void motion_vectors(PMV,dmvector,
  motion_vertical_field_select,s,motion_vector_count,mv_format,h_r_size,v_r_size,dmv,mvscale)
int PMV[2][2][2];
int dmvector[2];
int motion_vertical_field_select[2][2];
int s, motion_vector_count, mv_format, h_r_size, v_r_size, dmv, mvscale;
{
  if (motion_vector_count==1)
  {
    if (mv_format==MV_FIELD && !dmv)
    {
      motion_vertical_field_select[1][s] = motion_vertical_field_select[0][s] = Get_Bits(1);
#ifdef TRACE
      if (Trace_Flag)
      {
        printf("motion_vertical_field_select[][%d] (%d): %d\n",s,
          motion_vertical_field_select[0][s],motion_vertical_field_select[0][s]);
      }
#endif /* TRACE */
    }

    motion_vector(PMV[0][s],dmvector,h_r_size,v_r_size,dmv,mvscale,0);

    /* update other motion vector predictors */
    PMV[1][s][0] = PMV[0][s][0];
    PMV[1][s][1] = PMV[0][s][1];
  }
  else
  {
    motion_vertical_field_select[0][s] = Get_Bits(1);
#ifdef TRACE
    if (Trace_Flag)
    {
      printf("motion_vertical_field_select[0][%d] (%d): %d\n",s,
        motion_vertical_field_select[0][s],motion_vertical_field_select[0][s]);
    }
#endif /* TRACE */
    motion_vector(PMV[0][s],dmvector,h_r_size,v_r_size,dmv,mvscale,0);

    motion_vertical_field_select[1][s] = Get_Bits(1);
#ifdef TRACE
    if (Trace_Flag)
    {
      printf("motion_vertical_field_select[1][%d] (%d): %d\n",s,
        motion_vertical_field_select[1][s],motion_vertical_field_select[1][s]);
    }
#endif /* TRACE */
    motion_vector(PMV[1][s],dmvector,h_r_size,v_r_size,dmv,mvscale,0);
  }
}

/* get and decode motion vector and differential motion vector 
   for one prediction */
void motion_vector(PMV,dmvector,
  h_r_size,v_r_size,dmv,mvscale,full_pel_vector)
int *PMV;
int *dmvector;
int h_r_size;
int v_r_size;
int dmv; /* MPEG-2 only: get differential motion vectors */
int mvscale; /* MPEG-2 only: field vector in frame pic */
int full_pel_vector; /* MPEG-1 only */
{
  int motion_code, motion_residual;

  /* horizontal component */
  /* ISO/IEC 13818-2 Table B-10 */
  motion_code = Get_motion_code();

  motion_residual = (h_r_size!=0 && motion_code!=0) ? Get_Bits(h_r_size) : 0;

#ifdef TRACE
  if (Trace_Flag)
  {
    if (h_r_size!=0 && motion_code!=0)
    {
      printf("motion_residual (");
      Print_Bits(motion_residual,h_r_size,h_r_size);
      printf("): %d\n",motion_residual);
    }
  }
#endif /* TRACE */


  decode_motion_vector(&PMV[0],h_r_size,motion_code,motion_residual,full_pel_vector);

  if (dmv)
    dmvector[0] = Get_dmvector();


  /* vertical component */
  motion_code     = Get_motion_code();
  motion_residual = (v_r_size!=0 && motion_code!=0) ? Get_Bits(v_r_size) : 0;

#ifdef TRACE
  if (Trace_Flag)
  {
    if (v_r_size!=0 && motion_code!=0)
    {
      printf("motion_residual (");
      Print_Bits(motion_residual,v_r_size,v_r_size);
      printf("): %d\n",motion_residual);
    }
  }
#endif /* TRACE */

  if (mvscale)
    PMV[1] >>= 1; /* DIV 2 */

  decode_motion_vector(&PMV[1],v_r_size,motion_code,motion_residual,full_pel_vector);

  if (mvscale)
    PMV[1] <<= 1;

  if (dmv)
    dmvector[1] = Get_dmvector();

#ifdef TRACE
  if (Trace_Flag)
    printf("PMV = %d,%d\n",PMV[0],PMV[1]);
#endif /* TRACE */
}

/* calculate motion vector component */
/* ISO/IEC 13818-2 section 7.6.3.1: Decoding the motion vectors */
/* Note: the arithmetic here is more elegant than that which is shown 
   in 7.6.3.1.  The end results (PMV[][][]) should, however, be the same.  */

static void decode_motion_vector(pred,r_size,motion_code,motion_residual,full_pel_vector)
int *pred;
int r_size, motion_code, motion_residual;
int full_pel_vector; /* MPEG-1 (ISO/IEC 11172-1) support */
{
  int lim, vec;

  lim = 16<<r_size;
  vec = full_pel_vector ? (*pred >> 1) : (*pred);

  if (motion_code>0)
  {
    vec+= ((motion_code-1)<<r_size) + motion_residual + 1;
    if (vec>=lim)
      vec-= lim + lim;
  }
  else if (motion_code<0)
  {
    vec-= ((-motion_code-1)<<r_size) + motion_residual + 1;
    if (vec<-lim)
      vec+= lim + lim;
  }
  *pred = full_pel_vector ? (vec<<1) : vec;
}


/* ISO/IEC 13818-2 section 7.6.3.6: Dual prime additional arithmetic */
void Dual_Prime_Arithmetic(DMV,dmvector,mvx,mvy)
int DMV[][2];
int *dmvector; /* differential motion vector */
int mvx, mvy;  /* decoded mv components (always in field format) */
{
  if (picture_structure==FRAME_PICTURE)
  {
    if (top_field_first)
    {
      /* vector for prediction of top field from bottom field */
      DMV[0][0] = ((mvx  +(mvx>0))>>1) + dmvector[0];
      DMV[0][1] = ((mvy  +(mvy>0))>>1) + dmvector[1] - 1;

      /* vector for prediction of bottom field from top field */
      DMV[1][0] = ((3*mvx+(mvx>0))>>1) + dmvector[0];
      DMV[1][1] = ((3*mvy+(mvy>0))>>1) + dmvector[1] + 1;
    }
    else
    {
      /* vector for prediction of top field from bottom field */
      DMV[0][0] = ((3*mvx+(mvx>0))>>1) + dmvector[0];
      DMV[0][1] = ((3*mvy+(mvy>0))>>1) + dmvector[1] - 1;

      /* vector for prediction of bottom field from top field */
      DMV[1][0] = ((mvx  +(mvx>0))>>1) + dmvector[0];
      DMV[1][1] = ((mvy  +(mvy>0))>>1) + dmvector[1] + 1;
    }
  }
  else
  {
    /* vector for prediction from field of opposite 'parity' */
    DMV[0][0] = ((mvx+(mvx>0))>>1) + dmvector[0];
    DMV[0][1] = ((mvy+(mvy>0))>>1) + dmvector[1];

    /* correct for vertical field shift */
    if (picture_structure==TOP_FIELD)
      DMV[0][1]--;
    else
      DMV[0][1]++;
  }
}

#ifdef THRD
void Thrd_motion_vectors(t,PMV,dmvector,
  motion_vertical_field_select,s,motion_vector_count,mv_format,h_r_size,v_r_size,dmv,mvscale)
     int t;
int PMV[2][2][2];
int dmvector[2];
int motion_vertical_field_select[2][2];
int s, motion_vector_count, mv_format, h_r_size, v_r_size, dmv, mvscale;
{
  if (motion_vector_count==1)
  {
    if (mv_format==MV_FIELD && !dmv)
    {
      motion_vertical_field_select[1][s] = motion_vertical_field_select[0][s] = Thrd_Get_Bits(t,1);
    }

    Thrd_motion_vector(t, PMV[0][s],dmvector,h_r_size,v_r_size,dmv,mvscale,0);

    /* update other motion vector predictors */
    PMV[1][s][0] = PMV[0][s][0];
    PMV[1][s][1] = PMV[0][s][1];
  }
  else
  {
    motion_vertical_field_select[0][s] = Thrd_Get_Bits(t,1);
    Thrd_motion_vector(t,PMV[0][s],dmvector,h_r_size,v_r_size,dmv,mvscale,0);

    motion_vertical_field_select[1][s] = Thrd_Get_Bits(t,1);
    Thrd_motion_vector(t, PMV[1][s],dmvector,h_r_size,v_r_size,dmv,mvscale,0);
  }
}

/* get and decode motion vector and differential motion vector 
   for one prediction */
void Thrd_motion_vector(t,PMV,dmvector,
  h_r_size,v_r_size,dmv,mvscale,full_pel_vector)
int t;
int *PMV;
int *dmvector;
int h_r_size;
int v_r_size;
int dmv; /* MPEG-2 only: get differential motion vectors */
int mvscale; /* MPEG-2 only: field vector in frame pic */
int full_pel_vector; /* MPEG-1 only */
{
  int motion_code, motion_residual;

  /* horizontal component */
  /* ISO/IEC 13818-2 Table B-10 */
  motion_code = Thrd_Get_motion_code(t);

  motion_residual = (h_r_size!=0 && motion_code!=0) ? Thrd_Get_Bits(t,h_r_size) : 0;

  decode_motion_vector(&PMV[0],h_r_size,motion_code,motion_residual,full_pel_vector);

  if (dmv)
    dmvector[0] = Thrd_Get_dmvector(t);


  /* vertical component */
  motion_code     = Thrd_Get_motion_code(t);
  motion_residual = (v_r_size!=0 && motion_code!=0) ? Thrd_Get_Bits(t,v_r_size) : 0;

  if (mvscale)
    PMV[1] >>= 1; /* DIV 2 */
  
  decode_motion_vector(&PMV[1],v_r_size,motion_code,motion_residual,full_pel_vector);

  if (mvscale)
    PMV[1] <<= 1;

  if (dmv)
    dmvector[1] = Thrd_Get_dmvector(t);

}
#endif
