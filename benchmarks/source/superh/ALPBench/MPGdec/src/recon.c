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


/* Predict.c, motion compensation routines                                    */

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

#define FP_OPT 1
#define PRED_TEST_CODE 0

/* private prototypes */
static void form_prediction _ANSI_ARGS_((unsigned char *src[], int sfield,
  unsigned char *dst[], int dfield,
  int lx, int lx2, int w, int h, int x, int y, int dx, int dy,
  int average_flag));

static void form_component_prediction _ANSI_ARGS_((unsigned char *src, 
unsigned char *dst, int lx, int lx2, int w, int h, int x, int y, int dx, int dy,
 int average_flag));

#if FP_OPT
static void form_component_prediction2 _ANSI_ARGS_((unsigned char *src, 
unsigned char *dst, int lx, int lx2, int w, int h, int dx, int dy, 
int average_flag));
#else
static void form_component_prediction2 _ANSI_ARGS_((unsigned char *src, 
unsigned char *dst, int lx, int lx2, int w, int h, int x, int y, int dx, int dy,
 int average_flag));
#endif

extern int phase;

void form_predictions(bx,by,macroblock_type,motion_type,PMV,
		      motion_vertical_field_select,dmvector,stwtype)
int bx, by;
int macroblock_type;
int motion_type;
int PMV[2][2][2], motion_vertical_field_select[2][2], dmvector[2];
int stwtype;
{
  int currentfield;
  unsigned char **predframe;
  int DMV[2][2];
  int stwtop, stwbot;


  stwtop = stwtype%3; /* 0:temporal, 1:(spat+temp)/2, 2:spatial */
  stwbot = stwtype/3;

  if ((macroblock_type & MACROBLOCK_MOTION_FORWARD) 
   || (picture_coding_type==P_TYPE))
  {
    if (picture_structure==FRAME_PICTURE)
    {
      if ((motion_type==MC_FRAME) 
        || !(macroblock_type & MACROBLOCK_MOTION_FORWARD))
      {
        /* frame-based prediction (broken into top and bottom halves
             for spatial scalability prediction purposes) */
        if (stwtop<2)
          form_prediction(forward_reference_frame,0,current_frame,0,
            Coded_Picture_Width,Coded_Picture_Width<<1,16,8,bx,by,
            PMV[0][0][0],PMV[0][0][1],stwtop);

        if (stwbot<2)
          form_prediction(forward_reference_frame,1,current_frame,1,
            Coded_Picture_Width,Coded_Picture_Width<<1,16,8,bx,by,
            PMV[0][0][0],PMV[0][0][1],stwbot);
      }
      else if (motion_type==MC_FIELD) /* field-based prediction */
      {
        /* top field prediction */
        if (stwtop<2)
          form_prediction(forward_reference_frame,motion_vertical_field_select[0][0],
            current_frame,0,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,
            bx,by>>1,PMV[0][0][0],PMV[0][0][1]>>1,stwtop);

        /* bottom field prediction */
        if (stwbot<2)
          form_prediction(forward_reference_frame,motion_vertical_field_select[1][0],
            current_frame,1,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,
            bx,by>>1,PMV[1][0][0],PMV[1][0][1]>>1,stwbot);
      }
      else if (motion_type==MC_DMV) /* dual prime prediction */
      {
        /* calculate derived motion vectors */
        Dual_Prime_Arithmetic(DMV,dmvector,PMV[0][0][0],PMV[0][0][1]>>1);

        if (stwtop<2)
        {
          /* predict top field from top field */
          form_prediction(forward_reference_frame,0,current_frame,0,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,bx,by>>1,
            PMV[0][0][0],PMV[0][0][1]>>1,0);

          /* predict and add to top field from bottom field */
          form_prediction(forward_reference_frame,1,current_frame,0,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,bx,by>>1,
            DMV[0][0],DMV[0][1],1);
        }

        if (stwbot<2)
        {
          /* predict bottom field from bottom field */
          form_prediction(forward_reference_frame,1,current_frame,1,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,bx,by>>1,
            PMV[0][0][0],PMV[0][0][1]>>1,0);

          /* predict and add to bottom field from top field */
          form_prediction(forward_reference_frame,0,current_frame,1,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,bx,by>>1,
            DMV[1][0],DMV[1][1],1);
        }
      }
      else
        /* invalid motion_type */
        printf("invalid motion_type\n");
    }
    else /* TOP_FIELD or BOTTOM_FIELD */
    {
      /* field picture */
      currentfield = (picture_structure==BOTTOM_FIELD);

      /* determine which frame to use for prediction */
      if ((picture_coding_type==P_TYPE) && Second_Field
         && (currentfield!=motion_vertical_field_select[0][0]))
        predframe = backward_reference_frame; /* same frame */
      else
        predframe = forward_reference_frame; /* previous frame */

      if ((motion_type==MC_FIELD)
        || !(macroblock_type & MACROBLOCK_MOTION_FORWARD))
      {
        /* field-based prediction */
        if (stwtop<2)
          form_prediction(predframe,motion_vertical_field_select[0][0],current_frame,0,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,16,bx,by,
            PMV[0][0][0],PMV[0][0][1],stwtop);
      }
      else if (motion_type==MC_16X8)
      {
        if (stwtop<2)
        {
          form_prediction(predframe,motion_vertical_field_select[0][0],current_frame,0,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,bx,by,
            PMV[0][0][0],PMV[0][0][1],stwtop);

          /* determine which frame to use for lower half prediction */
          if ((picture_coding_type==P_TYPE) && Second_Field
             && (currentfield!=motion_vertical_field_select[1][0]))
            predframe = backward_reference_frame; /* same frame */
          else
            predframe = forward_reference_frame; /* previous frame */

          form_prediction(predframe,motion_vertical_field_select[1][0],current_frame,0,
            Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,bx,by+8,
            PMV[1][0][0],PMV[1][0][1],stwtop);
        }
      }
      else if (motion_type==MC_DMV) /* dual prime prediction */
      {
        if (Second_Field)
          predframe = backward_reference_frame; /* same frame */
        else
          predframe = forward_reference_frame; /* previous frame */

        /* calculate derived motion vectors */
        Dual_Prime_Arithmetic(DMV,dmvector,PMV[0][0][0],PMV[0][0][1]);

        /* predict from field of same parity */
        form_prediction(forward_reference_frame,currentfield,current_frame,0,
          Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,16,bx,by,
          PMV[0][0][0],PMV[0][0][1],0);

        /* predict from field of opposite parity */
        form_prediction(predframe,!currentfield,current_frame,0,
          Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,16,bx,by,
          DMV[0][0],DMV[0][1],1);
      }
      else
        /* invalid motion_type */
        printf("invalid motion_type\n");
    }
    stwtop = stwbot = 1;
  }

  if (macroblock_type & MACROBLOCK_MOTION_BACKWARD)
  {
    if (picture_structure==FRAME_PICTURE)
    {
      if (motion_type==MC_FRAME)
      {
        /* frame-based prediction */
        if (stwtop<2)
          form_prediction(backward_reference_frame,0,current_frame,0,
            Coded_Picture_Width,Coded_Picture_Width<<1,16,8,bx,by,
            PMV[0][1][0],PMV[0][1][1],stwtop);

        if (stwbot<2)
          form_prediction(backward_reference_frame,1,current_frame,1,
            Coded_Picture_Width,Coded_Picture_Width<<1,16,8,bx,by,
            PMV[0][1][0],PMV[0][1][1],stwbot);
      }
      else /* field-based prediction */
      {
        /* top field prediction */
        if (stwtop<2)
          form_prediction(backward_reference_frame,motion_vertical_field_select[0][1],
            current_frame,0,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,
            bx,by>>1,PMV[0][1][0],PMV[0][1][1]>>1,stwtop);

        /* bottom field prediction */
        if (stwbot<2)
          form_prediction(backward_reference_frame,motion_vertical_field_select[1][1],
            current_frame,1,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,
            bx,by>>1,PMV[1][1][0],PMV[1][1][1]>>1,stwbot);
      }
    }
    else /* TOP_FIELD or BOTTOM_FIELD */
    {
      /* field picture */
      if (motion_type==MC_FIELD)
      {
        /* field-based prediction */
        form_prediction(backward_reference_frame,motion_vertical_field_select[0][1],
          current_frame,0,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,16,
          bx,by,PMV[0][1][0],PMV[0][1][1],stwtop);
      }
      else if (motion_type==MC_16X8)
      {
        form_prediction(backward_reference_frame,motion_vertical_field_select[0][1],
          current_frame,0,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,
          bx,by,PMV[0][1][0],PMV[0][1][1],stwtop);

        form_prediction(backward_reference_frame,motion_vertical_field_select[1][1],
          current_frame,0,Coded_Picture_Width<<1,Coded_Picture_Width<<1,16,8,
          bx,by+8,PMV[1][1][0],PMV[1][1][1],stwtop);
      }
      else
        /* invalid motion_type */
        printf("invalid motion_type\n");
    }
  }
}

#if FP_OPT

static void form_prediction(src,sfield,dst,dfield,lx,lx2,w,h,x,y,dx,dy,average_flag)
unsigned char *src[]; /* prediction source buffer */
int sfield;           /* prediction source field number (0 or 1) */
unsigned char *dst[]; /* prediction destination buffer */
int dfield;           /* prediction destination field number (0 or 1)*/
int lx,lx2;           /* line strides */
int w,h;              /* prediction block/sub-block width, height */
int x,y;              /* pixel co-ordinates of top-left sample in current MB */
int dx,dy;            /* horizontal, vertical prediction address */
int average_flag;     /* add prediction error to prediction ? */
{
  int sf_val,df_val,sly_val, dly_val, slx_val,xh,yh;
  /* Y */
  form_component_prediction(src[0]+(sfield?lx2>>1:0),dst[0]+(dfield?lx2>>1:0),
    lx,lx2,w,h,x,y,dx,dy,average_flag);

  if (chroma_format!=CHROMA444)
  {
    lx>>=1; lx2>>=1; w>>=1; x>>=1; dx/=2;
  }

  if (chroma_format==CHROMA420)
  {
    h>>=1; y>>=1; dy/=2;
  }
  
  sf_val = sfield?lx2>>1:0;
  df_val = dfield?lx2>>1:0;
  sly_val = lx * (y+(dy>>1));
  dly_val = lx * y;
  slx_val = x + (dx>>1);
  xh = dx & 1;
  yh = dy & 1;
  
  /* Cb */
  form_component_prediction2(src[1]+sf_val+sly_val+slx_val,
			     dst[1]+df_val+dly_val+x,
			     lx,lx2,w,h,xh,yh,average_flag);

  /* Cr */
  form_component_prediction2(src[2]+sf_val+sly_val+slx_val,
			     dst[2]+df_val+dly_val+x,
			     lx,lx2,w,h,xh,yh,average_flag);

}

#else /* not FP_OPT */

static void form_prediction(src,sfield,dst,dfield,lx,lx2,w,h,x,y,dx,dy,average_flag)
unsigned char *src[]; /* prediction source buffer */
int sfield;           /* prediction source field number (0 or 1) */
unsigned char *dst[]; /* prediction destination buffer */
int dfield;           /* prediction destination field number (0 or 1)*/
int lx,lx2;           /* line strides */
int w,h;              /* prediction block/sub-block width, height */
int x,y;              /* pixel co-ordinates of top-left sample in current MB */
int dx,dy;            /* horizontal, vertical prediction address */
int average_flag;     /* add prediction error to prediction ? */
{
  /* Y */
  form_component_prediction(src[0]+(sfield?lx2>>1:0),dst[0]+(dfield?lx2>>1:0),
    lx,lx2,w,h,x,y,dx,dy,average_flag);

  if (chroma_format!=CHROMA444)
  {
    lx>>=1; lx2>>=1; w>>=1; x>>=1; dx/=2;
  }

  if (chroma_format==CHROMA420)
  {
    h>>=1; y>>=1; dy/=2;
  }

  /* Cb */
  form_component_prediction2(src[1]+(sfield?lx2>>1:0),dst[1]+(dfield?lx2>>1:0),
    lx,lx2,w,h,x,y,dx,dy,average_flag);

  /* Cr */
  form_component_prediction2(src[2]+(sfield?lx2>>1:0),dst[2]+(dfield?lx2>>1:0),
    lx,lx2,w,h,x,y,dx,dy,average_flag);
}

#endif /* end not FP_OPT */

/* ISO/IEC 13818-2 section 7.6.4: Forming predictions */
/* NOTE: the arithmetic below produces numerically equivalent results
 *  to 7.6.4, yet is more elegant. It differs in the following ways:
 *
 *   1. the vectors (dx, dy) are based on cartesian frame 
 *      coordiantes along a half-pel grid (always positive numbers)
 *      In contrast, vector[r][s][t] are differential (with positive and 
 *      negative values). As a result, deriving the integer vectors 
 *      (int_vec[t]) from dx, dy is accomplished by a simple right shift.
 *
 *   2. Half pel flags (xh, yh) are equivalent to the LSB (Least
 *      Significant Bit) of the half-pel coordinates (dx,dy).
 * 
 *
 *  NOTE: the work of combining predictions (ISO/IEC 13818-2 section 7.6.7)
 *  is distributed among several other stages.  This is accomplished by 
 *  folding line offsets into the source and destination (src,dst)
 *  addresses (note the call arguments to form_prediction() in Predict()),
 *  line stride variables lx and lx2, the block dimension variables (w,h), 
 *  average_flag, and by the very order in which Predict() is called.  
 *  This implementation design (implicitly different than the spec) 
 *  was chosen for its elegance.
*/

/* assumption - processes 16x8 blocks - ruchira */
#include<assert.h>

static void form_component_prediction(src,dst,lx,lx2,w,h,x,y,dx,dy,average_flag)
unsigned char *src;
unsigned char *dst;
int lx;          /* raster line increment */ 
int lx2;
int w,h;
int x,y;
int dx,dy;
int average_flag;      /* flag that signals bi-directional or Dual-Prime 
                          averaging (7.6.7.1 and 7.6.7.4). if average_flag==1,
                          a previously formed prediction has been stored in 
                          pel_pred[] */
{
  int xint;      /* horizontal integer sample vector: analogous to int_vec[0] */
  int yint;      /* vertical integer sample vectors: analogous to int_vec[1] */
  int xh;        /* horizontal half sample flag: analogous to half_flag[0]  */
  int yh;        /* vertical half sample flag: analogous to half_flag[1]  */
  int i, j, v;
  unsigned char *s;    /* source pointer: analogous to pel_ref[][]   */
  unsigned char *d;    /* destination pointer:  analogous to pel_pred[][]  */

  /* half pel scaling for integer vectors */
  xint = dx>>1;
  yint = dy>>1;


  /* compute the linear address of pel_ref[][] and pel_pred[][] 
     based on cartesian/raster cordinates provided */
  s = src + lx*(y+yint) + x + xint;
  d = dst + lx*y + x;



  /* derive half pel flags */
  xh = dx & 1;
  yh = dy & 1;

  assert(s!=d);

  if (!xh && !yh) /* no horizontal nor vertical half-pel */
    {
      if (average_flag)
    {
#ifdef SSE2

#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = s[j*lx2+i] + d[j*lx2+i];
          tmp[j*w+i] = (v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	
      fcp_loop1:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqa xmm1, [edx] ; /* load dst blk */
	pavgb  xmm1, xmm0  ; /* find avg */
	movdqa [edx], xmm1 ;
	add    edx, ebx    ;
	add    eax, ebx    ;
	loop fcp_loop1     ;
      }
#if (PRED_TEST_CODE)
     for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("1 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j++)
      {

        for (i=0; i<w; i++)
        {
          v = d[i]+s[i];
          d[i] = (v+(v>=0?1:0))>>1;
        }

        s+= lx2;
        d+= lx2;

      }
#endif
	
    }
    else
    {

#ifdef SSE2
#if (PRED_TEST_CODE)
     unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {

        for (i=0; i<w; i++)
        {

          tmp[j*w+i] = s[j*lx2+i];
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	
      fcp_loop2:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqa [edx], xmm0 ; /* copy over */
	add    edx, ebx    ;
	add    eax, ebx    ;
	loop fcp_loop2     ;
      }
#if (PRED_TEST_CODE)      
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("2 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j++)
      {

        for (i=0; i<w; i++)
        {
          d[i] = s[i];
        }
        
        s+= lx2;
        d+= lx2;

      }
#endif

    }




  }
  else if (!xh && yh) /* no horizontal but vertical half-pel */
  {
    if (average_flag)
    {

#ifdef SSE2
#if (PRED_TEST_CODE)
       unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = d[j*lx2+i] + ((unsigned int)(s[j*lx2+i]+s[j*lx2+i+lx]+1)>>1);
          tmp[j*w+i]=(v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* for vertical half pel */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
	
      fcp_loop3:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqu xmm1, [esi] ; /* load s+lx blk */
	movdqa xmm2, [edx] ; /* load dest blk */
	pavgb  xmm1, xmm0  ; /* avg(s,s+lx) */
	pavgb  xmm2, xmm1  ; /* avg(d, avg(s,s+lx)) */
	movdqa [edx], xmm2 ; /* copy over */
	add    edx, ebx    ;
	add    eax, ebx    ;
	add    esi, ebx    ;
	loop fcp_loop3     ;
      }  
#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
	  for (i=0; i<w; i++)
	    {
	      if (d[j*lx2+i]!=tmp[j*w+i])
		printf("3 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	    }
	}
#endif
#else
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+lx]+1)>>1);
          d[i]=(v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;

      }
#endif      
    }
    else
    {

#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          tmp[j*w+i] =  (unsigned int)(s[j*lx2+i]+s[j*lx2+i+lx]+1)>>1;
        }
      }
#endif
     /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* for vertical half pel */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
	
      fcp_loop4:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqu xmm1, [esi] ; /* load s+lx blk */
	pavgb  xmm1, xmm0  ; /* avg(s,s+lx) */
	movdqa [edx], xmm1 ; /* copy over */
	add    edx, ebx    ;
	add    eax, ebx    ;
	add    esi, ebx    ;
	loop fcp_loop4     ;
      }          
#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("4 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j++)
	{
	  for (i=0; i<w; i++)
	    {
	      d[i] = (unsigned int)(s[i]+s[i+lx]+1)>>1;
	    }

	  s+= lx2;
	  d+= lx2;

	}

#endif

    }
  }


  else if (xh && !yh) /* horizontal but no vertical half-pel */
  {
    if (average_flag)
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
	  int offset = j*lx2;
	  v =  d[offset+i] + ((unsigned int)(s[offset+i]+s[offset+i+1]+1)>>1);
          tmp[j*w+i] = (v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	
      fcp_loop5:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqu xmm1, [eax+1] ; /* load s+1 blk */
	movdqa xmm2, [edx] ; /* load dest blk */
	pavgb  xmm1, xmm0  ; /* avg(s,s+1) */
	pavgb  xmm2, xmm1  ;
	movdqa [edx], xmm2 ; /* copy over */
	add    edx, ebx    ;
	add    eax, ebx    ;
	loop fcp_loop5     ;
      }          

#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("5 d %d:%d ref %d\n",j*lx2+i,d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+1]+1)>>1);
          d[i] = (v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;
      }
#endif
    }
    else
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
       unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          tmp[j*w+i] =  (unsigned int)(s[j*lx2+i]+s[j*lx2+i+1]+1)>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	
      fcp_loop6:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqu xmm1, [eax+1] ; /* load s+1 blk */
	pavgb  xmm1, xmm0  ; /* avg(s,s+1) */
	movdqa [edx], xmm1 ; /* copy over */
	add    edx, ebx    ;
	add    eax, ebx    ;
	loop fcp_loop6     ;
      }
#if (PRED_TEST_CODE)
       for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("6 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }          
#endif
#else
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          d[i] = (unsigned int)(s[i]+s[i+1]+1)>>1;
        }

        s+= lx2;
        d+= lx2;
      }
#endif


    }
  }
  else /* if (xh && yh) horizontal and vertical half-pel */
  {
    if (average_flag)
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
	  v = d[j*lx2+i] + ((unsigned int)(s[j*lx2+i]+s[j*lx2+i+1]+s[j*lx2+i+lx]+s[j*lx2+i+lx+1]+2)>>2);
	  tmp[j*w+i] = (v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* for vertical half pel */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
 	
      fcp_loop7:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqu xmm1, [eax+1] ; /* load s+1 blk */
	movdqu xmm2, [esi]  ;
	movdqu xmm3, [esi+1]  ;
	movdqa xmm4, [edx] ; /* load dest blk */

	pavgb  xmm0, xmm1  ; /* avg(s,s+1)*/
	pavgb  xmm2, xmm3  ; /* avg(s+lx, s+lx+1) */
	pavgb  xmm0, xmm2  ; 
	pavgb  xmm0, xmm4  ; /* avg(d, all s) */
	movdqa [edx], xmm0 ; /* store to d */
	add    edx, ebx    ;
	add    eax, ebx    ;
	add    esi, ebx    ;
	loop fcp_loop7     ;
      }

#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i] - tmp[j*w+i] > 1) {
	      printf("7 d %d:%d ref %d\n",j*lx2+i,d[j*lx2+i],tmp[j*w+i]);
	    }
	  }
      }
#endif
#else
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+1]+s[i+lx]+s[i+lx+1]+2)>>2);
          d[i] = (v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;
      }
#endif
    }
    else
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          tmp[j*w+i] = (unsigned int)(s[j*lx2+i]+s[j*lx2+i+1]+s[j*lx2+i+lx]+s[j*lx2+i+lx+1]+2)>>2;

        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* load lx */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
 	
      fcp_loop8:
	movdqu xmm0, [eax] ; /* load src blk */
	movdqu xmm1, [eax+1] ; /* load s+1 blk */
	movdqu xmm2, [esi]  ;
	movdqu xmm3, [esi+1]  ;

	pavgb  xmm0, xmm1  ; /* avg(s,s+1)*/
	pavgb  xmm2, xmm3  ; /* avg(s+lx, s+lx+1) */
	pavgb  xmm0, xmm2  ; 
	movdqa [edx], xmm0 ; /* store to d */
	add    edx, ebx    ;
	add    eax, ebx    ;
	add    esi, ebx    ;
	loop fcp_loop8     ;
      }
#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i] - tmp[j*w+i] > 1)
	      printf("8 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j++)
      {
	for (i=0; i<w; i++)
	  {
	    d[i] = (unsigned int)(s[i]+s[i+1]+s[i+lx]+s[i+lx+1]+2)>>2;
	  }
	
        s+= lx2;
        d+= lx2;
      }

#endif
    }
  }

}


/* assumption - processes 8x4 blocks 
   This method calls the same blocks of the above function since we
   unroll twice - ruchira */
#if FP_OPT
static void form_component_prediction2(src,dst,lx,lx2,w,h,dx,dy,average_flag)
unsigned char *src;
unsigned char *dst;
int lx;          /* raster line increment */ 
int lx2;
int w,h;
int dx,dy;
int average_flag;      /* flag that signals bi-directional or Dual-Prime 
                          averaging (7.6.7.1 and 7.6.7.4). if average_flag==1,
                          a previously formed prediction has been stored in 
                          pel_pred[] */
#else
static void form_component_prediction2(src,dst,lx,lx2,w,h,x,y,dx,dy,average_flag)
unsigned char *src;
unsigned char *dst;
int lx;          /* raster line increment */ 
int lx2;
int w,h;
int x,y;
int dx,dy;
int average_flag;      /* flag that signals bi-directional or Dual-Prime 
                          averaging (7.6.7.1 and 7.6.7.4). if average_flag==1,
                          a previously formed prediction has been stored in 
                          pel_pred[] */
#endif
{
  int xint;      /* horizontal integer sample vector: analogous to int_vec[0] */
  int yint;      /* vertical integer sample vectors: analogous to int_vec[1] */
  int xh;        /* horizontal half sample flag: analogous to half_flag[0]  */
  int yh;        /* vertical half sample flag: analogous to half_flag[1]  */
  int i, j, v;
  unsigned char *s;    /* source pointer: analogous to pel_ref[][]   */
  unsigned char *d;    /* destination pointer:  analogous to pel_pred[][]  */

#if FP_OPT
  s = src;
  d = dst;

  xh = dx;
  yh = dy;
#else
  /* half pel scaling for integer vectors */
  xint = dx>>1;
  yint = dy>>1;

  /* compute the linear address of pel_ref[][] and pel_pred[][] 
     based on cartesian/raster cordinates provided */
  s = src + lx*(y+yint) + x + xint;
  d = dst + lx*y + x;

  /* derive half pel flags */
  xh = dx & 1;
  yh = dy & 1;
#endif

  if (!xh && !yh) /* no horizontal nor vertical half-pel */
  {
    if (average_flag)
    {

#ifdef SSE2

#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = s[j*lx2+i] + d[j*lx2+i];
          tmp[j*w+i] = (v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h==4 w==8 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	mov    esi, ebx    ;
	sal    esi, 1      ;
	sar    ecx, 1      ; /* unroll twice */
	
      fcp2_loop1:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ; /* load 8 bytes from src + lx2 blk */
	movq xmm2, [edx]       ; /* load 8B from dest blk */
	movq xmm3, [edx+ebx]   ; /* load 8 bytes from dest + lx2 blk */

	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm2, xmm3 ;	

	pavgb  xmm2, xmm0  ; /* find avg */

	movq [edx], xmm2  ;
	psrldq xmm2, 8     ;
	movq [edx+ebx], xmm2   ;
	add    edx, esi    ;
	add    eax, esi    ;
	loop fcp2_loop1     ;
      }
#if (PRED_TEST_CODE)
     for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("p2:1 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif

#else /* NON-SSE2 code */
#if 1
     unsigned char *s1,*d1; 
     int v1;
     
     for (j=0; j<h; j+=2) {
       s1 = s+lx2;
       d1 = d+lx2;
	 
       for (i=0; i<w; i++) {
	 v = d[i]+s[i];
	 v1 = d1[i] + s1[i];
	 d[i] = (v+(v>=0?1:0))>>1;
	 d1[i] = (v1+(v1>=0?1:0))>>1;
       }
       s = s1+lx2;
       d = d1+lx2;
     }
#else /* original code */
     for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = d[i]+s[i];
          d[i] = (v+(v>=0?1:0))>>1;
        }
      
        s+= lx2;
        d+= lx2;
      }
#endif
#endif	
    }
    else
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
     unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {

        for (i=0; i<w; i++)
        {

          tmp[j*w+i] = s[j*lx2+i];
        }
      }
#endif
      /* h=4 w==8 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	mov    esi, ebx    ;
	sar    ecx, 1      ;
	sal    esi, 1      ;
	
      fcp2_loop2:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ;
	movq [edx], xmm0       ;
	movq [edx+ebx], xmm1   ;

	add    edx, esi    ;
	add    eax, esi    ;
	loop fcp2_loop2     ;
      }
#if (PRED_TEST_CODE)      
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("p2:2 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j+=2)
      {
        for (i=0; i<w; i++)
        {
          d[i] = s[i];
        }
        
        s+= lx2;
        d+= lx2;

        for (i=0; i<w; i++)
        {
          d[i] = s[i];
        }
        
        s+= lx2;
        d+= lx2;

      }
#endif
    }




  }
  else if (!xh && yh) /* no horizontal but vertical half-pel */
  {
    if (average_flag)
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
       unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          v = d[j*lx2+i] + ((unsigned int)(s[j*lx2+i]+s[j*lx2+i+lx]+1)>>1);
          tmp[j*w+i]=(v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* w==8 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* for vertical half pel */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	sar    ecx, 1      ;
	mov    edi, ebx    ;
	sal    edi, 1      ;
	add    esi, eax    ; /* esi has s+lx */
	
      fcp2_loop3:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ; /* load 8 bytes from src + lx2 blk */
	movq xmm4, [esi]       ; /* load 8B from src+lx blk */
	movq xmm5, [esi+ebx]   ; /* load 8 bytes from src+lx+lx2 blk */
	movq xmm2, [edx]       ; /* load 8B from dest blk */
	movq xmm3, [edx+ebx]   ; /* load 8 bytes from dest + lx2 blk */

	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm4, xmm5 ;
	punpcklqdq xmm2, xmm3 ;

	pavgb xmm0, xmm4   ;
	pavgb xmm2, xmm0   ;
	
	movq [edx], xmm2   ;
	psrldq xmm2, 8     ;
	movq [edx+ebx], xmm2   ;
	add    edx, edi    ;
	add    eax, edi    ;	
	add    esi, edi    ;
	loop fcp2_loop3    ;
      }  
#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
	  for (i=0; i<w; i++)
	    {
	      if (d[j*lx2+i]!=tmp[j*w+i])
		printf("p2:3 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	    }
	}
#endif
#else
      for (j=0; j<h; j+=2)
      {
        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+lx]+1)>>1);
          d[i]=(v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;

        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+lx]+1)>>1);
          d[i]=(v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;

      }
#endif
    }
    else
    {

#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          tmp[j*w+i] =  (unsigned int)(s[j*lx2+i]+s[j*lx2+i+lx]+1)>>1;
        }
      }
#endif
     /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* for vertical half pel */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
	sar    ecx, 1      ;
	mov    edi, ebx    ;
	sal    edi, 1      ;
	
      fcp2_loop4:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ; /* load 8 bytes from src + lx2 blk */
	movq xmm2, [esi]       ; /* load 8B from src+lx blk */
	movq xmm3, [esi+ebx]   ; /* load 8 bytes from src+lx+lx2 blk */
	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm2, xmm3 ;

	pavgb xmm2, xmm0   ;
	movq [edx], xmm2   ;
	psrldq xmm2, 8     ;
	movq [edx+ebx], xmm2   ;

	add    edx, edi    ;
	add    eax, edi    ;	
	add    esi, edi    ;
	loop fcp2_loop4     ;
      }          
#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("p2:4 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j+=2)
      {
        for (i=0; i<w; i++)
        {
          d[i] = (unsigned int)(s[i]+s[i+lx]+1)>>1;
        }

        s+= lx2;
        d+= lx2;

        for (i=0; i<w; i++)
        {
          d[i] = (unsigned int)(s[i]+s[i+lx]+1)>>1;
        }

        s+= lx2;
        d+= lx2;

      }
#endif


    }
  }


  else if (xh && !yh) /* horizontal but no vertical half-pel */
  {
    if (average_flag)
    {

#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
	  int offset = j*lx2;
	  v =  d[offset+i] + ((unsigned int)(s[offset+i]+s[offset+i+1]+1)>>1);
          tmp[j*w+i] = (v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	sar    ecx, 1      ;
	mov    edi, ebx    ;
	sal    edi, 1      ;
	
      fcp2_loop5:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ; /* load 8 bytes from src + lx2 blk */
	movq xmm2, [eax+1]       ; /* load 8B from src+1 blk */
	movq xmm3, [eax+ebx+1]   ; /* load 8 bytes from src+1+lx2 blk */
	movq xmm4, [edx]   ; /* load 8 bytes from dst blk */
	movq xmm5, [edx+ebx]   ; /* load 8 bytes from dst + lx2 blk */
	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm2, xmm3 ;
	punpcklqdq xmm4, xmm5 ;

	pavgb  xmm2, xmm0  ; /* avg(s,s+1) */
	pavgb  xmm2, xmm4  ;

	movq [edx], xmm2   ;
	psrldq xmm2, 8     ;
	movq [edx+ebx], xmm2   ;
	add    eax, edi    ;
	add    edx, edi    ;
	loop fcp2_loop5     ;
      }          

#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("p2:5 d %d:%d ref %d\n",j*lx2+i,d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j+=2)
      {

        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+1]+1)>>1);
          d[i] = (v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;


        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+1]+1)>>1);
          d[i] = (v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;

      }
#endif
    }
    else
    {

#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;
      
      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          tmp[j*w+i] =  (unsigned int)(s[j*lx2+i]+s[j*lx2+i+1]+1)>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	sar    ecx, 1      ;
	mov    edi, ebx    ;
	sal    edi, 1      ;	
	
      fcp2_loop6:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ; /* load 8 bytes from src + lx2 blk */
	movq xmm2, [eax+1]       ; /* load 8B from src+1 blk */
	movq xmm3, [eax+ebx+1]   ; /* load 8 bytes from src+1+lx2 blk */
	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm2, xmm3 ;

	pavgb  xmm2, xmm0  ; /* avg(s,s+1) */
	movq [edx], xmm2   ;
	psrldq xmm2, 8     ;
	movq [edx+ebx], xmm2   ;
	add    edx, edi    ;
	add    eax, edi    ;
	loop fcp2_loop6     ;
      }
#if (PRED_TEST_CODE)
       for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i]!=tmp[j*w+i])
	      printf("p2:6 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }          
#endif
#else
       for (j=0; j<h; j+=2)
	 {
	   for (i=0; i<w; i++)
	     {
	       d[i] = (unsigned int)(s[i]+s[i+1]+1)>>1;
	     }

	   s+= lx2;
	   d+= lx2;

	   for (i=0; i<w; i++)
	     {
	       d[i] = (unsigned int)(s[i]+s[i+1]+1)>>1;
	     }

	   s+= lx2;
	   d+= lx2;

	 }

#endif

    }
  }
  else /* if (xh && yh) horizontal and vertical half-pel */
  {
    if (average_flag)
    {
 
#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
	  v = d[j*lx2+i] + ((unsigned int)(s[j*lx2+i]+s[j*lx2+i+1]+s[j*lx2+i+lx]+s[j*lx2+i+lx+1]+2)>>2);
	  tmp[j*w+i] = (v+(v>=0?1:0))>>1;
        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* for vertical half pel */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
 	sar    ecx, 1      ;
	mov    edi, ebx    ;
	sal    edi, 1      ;	
	
      fcp2_loop7:
	movq xmm0, [eax]   ; /* load 8 bytes from src blk */
	movq xmm1, [eax+ebx]   ; /* load 8 bytes from src + lx2 blk */
	movq xmm2, [eax+1]       ; /* load 8B from src+1 blk */
	movq xmm3, [eax+ebx+1]   ; /* load 8 bytes from src+1+lx2 blk */
	movq xmm4, [esi]   ; /* load 8 bytes from src+lx blk */
	movq xmm5, [esi+ebx]   ; /* load 8 bytes from src+lx + lx2 blk */
	movq xmm6, [esi+1]   ; /* load 8 bytes from src+lx blk */
	movq xmm7, [esi+ebx+1]   ; /* load 8 bytes from src+lx + lx2 blk */

	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm2, xmm3 ;
	movq xmm1, [edx]      ;
	movq xmm3, [edx+ebx]  ;
	punpcklqdq xmm4, xmm5 ;
	punpcklqdq xmm6, xmm7 ;
	punpcklqdq xmm1, xmm3 ;

	pavgb xmm0, xmm2   ;
	pavgb xmm4, xmm6   ;
	pavgb xmm0, xmm4   ;
	pavgb xmm1, xmm0   ;

	movq [edx], xmm1   ;
	psrldq xmm1, 8     ;
	movq [edx+ebx], xmm1   ;

	add    edx, edi    ;
	add    eax, edi    ;
	add    esi, edi    ;
	loop fcp2_loop7     ;
      }

#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i] -tmp[j*w+i]>1) {
	      printf("p2:7 d %d:%d ref %d\n",j*lx2+i,d[j*lx2+i],tmp[j*w+i]);
	    }
	  }
      }
#endif
#else
      for (j=0; j<h; j+=2)
      {
        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+1]+s[i+lx]+s[i+lx+1]+2)>>2);
          d[i] = (v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;
	
        for (i=0; i<w; i++)
        {
          v = d[i] + ((unsigned int)(s[i]+s[i+1]+s[i+lx]+s[i+lx+1]+2)>>2);
          d[i] = (v+(v>=0?1:0))>>1;
        }
     
        s+= lx2;
        d+= lx2;

      }
#endif
    }
    else
    {
#ifdef SSE2
#if (PRED_TEST_CODE)
      unsigned char tmp[128];
      unsigned char *s_t=s, *d_t=d;

      for (j=0; j<h; j++)
      {
        for (i=0; i<w; i++)
        {
          tmp[j*w+i] = (unsigned int)(s[j*lx2+i]+s[j*lx2+i+1]+s[j*lx2+i+lx]+s[j*lx2+i+lx+1]+2)>>2;

        }
      }
#endif
      /* h%4=0 w==16 */
      __asm {
	mov    ecx, [h]    ; /* load in loop counter */
	mov    ebx, [lx2]  ; /* load width of row */
	mov    esi, [lx]   ; /* load lx */
	mov    eax, [s]    ; /* load source addr */
	mov    edx, [d]    ; /* load dest addr */
	add    esi, eax    ; /* esi has s+lx */
 	sar    ecx, 1      ;
	mov    edi, ebx    ;
	sal    edi, 1      ;	
 	
      fcp2_loop8:
	movq xmm0, [eax] ; /* load src blk */
	movq xmm1, [eax+ebx] ; /* load src blk */
	movq xmm2, [eax+1] ; /* load s+1 blk */
	movq xmm3, [eax+ebx+1] ; /* load s+1 blk */
	movq xmm4, [esi]  ;
	movq xmm5, [esi+ebx]  ;
	movq xmm6, [esi+1]  ;
	movq xmm7, [esi+ebx+1]  ;

	punpcklqdq xmm0, xmm1 ;
	punpcklqdq xmm2, xmm3 ;
	punpcklqdq xmm4, xmm5 ;
	punpcklqdq xmm6, xmm7 ;

	pavgb  xmm0, xmm2  ; /* avg(s,s+1)*/
	pavgb  xmm4, xmm6  ; /* avg(s+lx, s+lx+1) */
	pavgb  xmm0, xmm4  ; 

	movq [edx], xmm0 ; /* store to d */
	psrldq xmm0, 8     ;
	movq [edx+ebx], xmm0   ;
	add    edx, edi    ;
	add    eax, edi    ;
	add    esi, edi    ;
	loop fcp2_loop8     ;
      }
#if (PRED_TEST_CODE)
      for (j=0; j<h; j++)
	{
        for (i=0; i<w; i++)
	  {
	    if (d[j*lx2+i] -tmp[j*w+i]>1)
	      printf("p2:8 d %d ref %d\n",d[j*lx2+i],tmp[j*w+i]);
	  }
      }
#endif
#else
      for (j=0; j<h; j+=2)
      {
	for (i=0; i<w; i++){
	    d[i] = (unsigned int)(s[i]+s[i+1]+s[i+lx]+s[i+lx+1]+2)>>2;
	  }
	
        s+= lx2;
        d+= lx2;
	for (i=0; i<w; i++){
	    d[i] = (unsigned int)(s[i]+s[i+1]+s[i+lx]+s[i+lx+1]+2)>>2;
	  }
	
        s+= lx2;
        d+= lx2;
      }
#endif
    }
  }


}






