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


/* transfrm.c,  forward / inverse transformation                            */

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
#include <math.h>
#include "config.h"
#include "global.h"

/* private prototypes*/
static void add_pred _ANSI_ARGS_((unsigned char *pred, unsigned char *cur,
  int lx, short *blk));
static void sub_pred _ANSI_ARGS_((unsigned char *pred, unsigned char *cur,
  int lx, short *blk));

/* subtract prediction and transform prediction error */
void transform(pred,cur,mbi,blocks)
unsigned char *pred[], *cur[];
struct mbinfo *mbi;
short blocks[][64];
{
  int i, j, i1, j1, k, n, cc, offs, lx;

  k = 0;

  for (j=0; j<height2; j+=16)
    for (i=0; i<width; i+=16)
    {
      for (n=0; n<block_count; n++)
      {
        cc = (n<4) ? 0 : (n&1)+1; /* color component index */
        if (cc==0)
        {
          /* luminance */
          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type)
          {
            /* field DCT */
            offs = i + ((n&1)<<3) + width*(j+((n&2)>>1));
            lx = width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i + ((n&1)<<3) + width2*(j+((n&2)<<2));
            lx = width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += width;
        }
        else
        {
          /* chrominance */

          /* scale coordinates */
          i1 = (chroma_format==CHROMA444) ? i : i>>1;
          j1 = (chroma_format!=CHROMA420) ? j : j>>1;

          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type
              && (chroma_format!=CHROMA420))
          {
            /* field DCT */
            offs = i1 + (n&8) + chrom_width*(j1+((n&2)>>1));
            lx = chrom_width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i1 + (n&8) + chrom_width2*(j1+((n&2)<<2));
            lx = chrom_width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += chrom_width;
        }

	/* Note: sub pred writes into v1 (block) and fdct uses (v1) */

        sub_pred(pred[cc]+offs,cur[cc]+offs,lx,blocks[k*block_count+n]);
        fdct(blocks[k*block_count+n]);
      }

      k++;
    }

}


#ifdef LTHREAD
void pttransform(pred,cur,mbi,blocks,start_height,end_height)
unsigned char *pred[], *cur[];
struct mbinfo *mbi;
short blocks[][64];
int start_height,end_height;
{
  int i, j, i1, j1, k, n, cc, offs, lx;

  k = (start_height>>4)*(width>>4);

  for (j=start_height; j<end_height; j+=16)
    for (i=0; i<width; i+=16)
    {
      for (n=0; n<block_count; n++)
      {
        cc = (n<4) ? 0 : (n&1)+1; /* color component index */
        if (cc==0)
        {
          /* luminance */
          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type)
          {
            /* field DCT */
            offs = i + ((n&1)<<3) + width*(j+((n&2)>>1));
            lx = width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i + ((n&1)<<3) + width2*(j+((n&2)<<2));
            lx = width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += width;
        }
        else
        {
          /* chrominance */

          /* scale coordinates */
          i1 = (chroma_format==CHROMA444) ? i : i>>1;
          j1 = (chroma_format!=CHROMA420) ? j : j>>1;

          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type
              && (chroma_format!=CHROMA420))
          {
            /* field DCT */
            offs = i1 + (n&8) + chrom_width*(j1+((n&2)>>1));
            lx = chrom_width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i1 + (n&8) + chrom_width2*(j1+((n&2)<<2));
            lx = chrom_width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += chrom_width;
        }

	/* Note: sub pred writes into v1 (block) and fdct uses (v1) */

        sub_pred(pred[cc]+offs,cur[cc]+offs,lx,blocks[k*block_count+n]);
        fdct(blocks[k*block_count+n]);
      }

      k++;
    }

}

void ptitransform(pred,cur,mbi,blocks,start_height,end_height)
unsigned char *pred[],*cur[];
struct mbinfo *mbi;
short blocks[][64];
int start_height, end_height;
{
  int i, j, i1, j1, k, n, cc, offs, lx;


  k = (start_height>>4)*(width>>4);

  for (j=start_height; j<end_height; j+=16)
    for (i=0; i<width; i+=16)
    {
      for (n=0; n<block_count; n++)
      {
        cc = (n<4) ? 0 : (n&1)+1; /* color component index */

        if (cc==0)
        {
          /* luminance */
          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type)
          {
            /* field DCT */
            offs = i + ((n&1)<<3) + width*(j+((n&2)>>1));
            lx = width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i + ((n&1)<<3) + width2*(j+((n&2)<<2));
            lx = width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += width;
        }
        else
        {
          /* chrominance */

          /* scale coordinates */
          i1 = (chroma_format==CHROMA444) ? i : i>>1;
          j1 = (chroma_format!=CHROMA420) ? j : j>>1;

          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type
              && (chroma_format!=CHROMA420))
          {
            /* field DCT */
            offs = i1 + (n&8) + chrom_width*(j1+((n&2)>>1));
            lx = chrom_width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i1 + (n&8) + chrom_width2*(j1+((n&2)<<2));
            lx = chrom_width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += chrom_width;
        }

        idct(blocks[k*block_count+n]);
        add_pred(pred[cc]+offs,cur[cc]+offs,lx,blocks[k*block_count+n]);
      }

      k++;
    }

}

#endif

/* inverse transform prediction error and add prediction */
void itransform(pred,cur,mbi,blocks)
unsigned char *pred[],*cur[];
struct mbinfo *mbi;
short blocks[][64];
{
  int i, j, i1, j1, k, n, cc, offs, lx;

  k = 0;

  for (j=0; j<height2; j+=16)
    for (i=0; i<width; i+=16)
    {
      for (n=0; n<block_count; n++)
      {
        cc = (n<4) ? 0 : (n&1)+1; /* color component index */

        if (cc==0)
        {
          /* luminance */
          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type)
          {
            /* field DCT */
            offs = i + ((n&1)<<3) + width*(j+((n&2)>>1));
            lx = width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i + ((n&1)<<3) + width2*(j+((n&2)<<2));
            lx = width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += width;
        }
        else
        {
          /* chrominance */

          /* scale coordinates */
          i1 = (chroma_format==CHROMA444) ? i : i>>1;
          j1 = (chroma_format!=CHROMA420) ? j : j>>1;

          if ((pict_struct==FRAME_PICTURE) && mbi[k].dct_type
              && (chroma_format!=CHROMA420))
          {
            /* field DCT */
            offs = i1 + (n&8) + chrom_width*(j1+((n&2)>>1));
            lx = chrom_width<<1;
          }
          else
          {
            /* frame DCT */
            offs = i1 + (n&8) + chrom_width2*(j1+((n&2)<<2));
            lx = chrom_width2;
          }

          if (pict_struct==BOTTOM_FIELD)
            offs += chrom_width;
        }

        idct(blocks[k*block_count+n]);
        add_pred(pred[cc]+offs,cur[cc]+offs,lx,blocks[k*block_count+n]);
      }

      k++;
    }

}

/* add prediction and prediction error, saturate to 0...255 */
static void add_pred(pred,cur,lx,blk)
unsigned char *pred, *cur;
int lx;
short *blk;
{
  int i, j;
#ifdef SSE2
  const short upper[8]={255,255,255,255,
			255,255,255,255};

  __asm {
    movdqu xmm5, [upper] ;
    mov	  ebx,	[blk]	; /*load the address of blk in ebx*/
    mov	  edx,	[pred]	;/*  load the address of pred in edx */
    mov     eax,  [lx]    ;
    mov     esi, [cur]    ;
    mov     edi, eax      ;
    sal     edi, 1        ;
    mov     ecx,  4       ;
    pxor    xmm4, xmm4    ;
    
  add_pred_loop:
    movdqu  xmm0, [ebx]     ;/* load 8W from blk */
    movdqu  xmm2, [ebx+16]  ;
    movq  xmm1, [edx]      ;/* load 8B from pred */
    movq  xmm3, [edx+eax]  ;/* load another 8B from pred */

    punpcklbw xmm1, xmm4  ; /*unpack bytes to words */
    punpcklbw xmm3, xmm4  ;

    paddsw xmm1, xmm0    ;/*add signed words */
    paddsw xmm3, xmm2    ;

    pminsw xmm1, xmm5    ;
    pminsw xmm3, xmm5    ;
    pmaxsw xmm1, xmm4    ;
    pmaxsw xmm3, xmm4    ;

    packuswb xmm1, xmm1  ; /* pack words to bytes */
    packuswb xmm3, xmm3  ;
    
    movq  [esi], xmm1        ; 
    movq  [esi+eax], xmm3    ;

    add     ebx, 32      ;
    add     edx, edi      ;
    add     esi, edi      ;
    sub     ecx, 1        ;
    cmp     ecx, 0        ;
    jnz     add_pred_loop ;
  }
#else
  for (j=0; j<8; j++)
  {

    for (i=0; i<8; i++)
      cur[i] = clp[blk[i] + pred[i]];
    blk+= 8;
    cur+= lx;
    pred+= lx;
  }
#endif


}

/* subtract prediction from block data */
static void sub_pred(pred,cur,lx,blk)
unsigned char *pred, *cur;
int lx;
short *blk;
{

  int i, j;
#ifdef SSE2
  __asm {
    mov	  ebx,	[blk]	; /*load the address of blk in ebx*/
    mov	  edx,	[pred]	;/*  load the address of pred in edx */
    mov     eax,  [lx]    ;
    mov     esi, [cur]    ;
    mov     edi, eax      ;
    sal     edi, 1        ;
    mov     ecx,  4       ;
    pxor xmm4, xmm4    ;
    
  sub_pred_loop:
    movq  xmm0, [esi]     ;/* load 8B from cur */
    movq  xmm2, [esi+eax]  ;
    movq  xmm1, [edx]      ;/* load 8B from pred */
    movq  xmm3, [edx+eax]  ;/* load another 8B from pred */

    punpcklbw xmm0, xmm4  ;/*unpack bytes to words */
    punpcklbw xmm1, xmm4  ;
    punpcklbw xmm2, xmm4  ;/*unpack bytes to words */
    punpcklbw xmm3, xmm4  ;

    psubw xmm0, xmm1      ;
    psubw xmm2, xmm3      ;

    movdqu  [ebx], xmm0     ; 
    movdqu  [ebx+16], xmm2  ;

    add     ebx, 32      ;
    add     edx, edi      ;
    add     esi, edi      ;
    sub     ecx, 1        ;
    cmp     ecx, 0        ;
    jnz     sub_pred_loop ;
  }
#else
  for (j=0; j<4; j++)  /* unrolled twice - Ruchira */
  {


    for (i=0; i<8; i++)
      blk[i] = cur[i] - pred[i];
    blk+= 8;
    cur+= lx;
    pred+= lx;


    for (i=0; i<8; i++)
      blk[i] = cur[i] - pred[i];
    blk+= 8;
    cur+= lx;
    pred+= lx;

  }
#endif
}

/*
 * select between frame and field DCT
 *
 * preliminary version: based on inter-field correlation
 */
void dct_type_estimation(pred,cur,mbi)
unsigned char *pred,*cur;
struct mbinfo *mbi;
{
  short blk0[128], blk1[128];
  int i, j, i0, j0, k, offs, s0, s1, sq0, sq1, s01;
  double d, r;

  k = 0;

  for (j0=0; j0<height2; j0+=16)
    for (i0=0; i0<width; i0+=16)
    {
      if (frame_pred_dct || pict_struct!=FRAME_PICTURE)
        mbi[k].dct_type = 0;
      else
      {
        /* interlaced frame picture */
        /*
         * calculate prediction error (cur-pred) for top (blk0)
         * and bottom field (blk1)
         */
        for (j=0; j<8; j++)
        {
          offs = width*((j<<1)+j0) + i0;
          for (i=0; i<16; i++)
          {
            blk0[16*j+i] = cur[offs] - pred[offs];
            blk1[16*j+i] = cur[offs+width] - pred[offs+width];
            offs++;
          }
        }
        /* correlate fields */
        s0=s1=sq0=sq1=s01=0;

        for (i=0; i<128; i++)
        {
          s0+= blk0[i];
          sq0+= blk0[i]*blk0[i];
          s1+= blk1[i];
          sq1+= blk1[i]*blk1[i];
          s01+= blk0[i]*blk1[i];
        }

        d = (sq0-(s0*s0)/128.0)*(sq1-(s1*s1)/128.0);

        if (d>0.0)
        {
          r = (s01-(s0*s1)/128.0)/sqrt(d);
          if (r>0.5)
            mbi[k].dct_type = 0; /* frame DCT */
          else
            mbi[k].dct_type = 1; /* field DCT */
        }
        else
          mbi[k].dct_type = 1; /* field DCT */
      }
      k++;
    }
}

#ifdef LTHREAD
void ptdct_type_estimation(pred,cur,mbi,start_height,end_height)
unsigned char *pred,*cur;
struct mbinfo *mbi;
int start_height,end_height;
{
  short blk0[128], blk1[128];
  int i, j, i0, j0, k, offs, s0, s1, sq0, sq1, s01;
  double d, r;

  k = (start_height>>4)*(width>>4);

  for (j0=start_height; j0<end_height; j0+=16)
    for (i0=0; i0<width; i0+=16)
    {
      if (frame_pred_dct || pict_struct!=FRAME_PICTURE)
        mbi[k].dct_type = 0;
      else
      {
        /* interlaced frame picture */
        /*
         * calculate prediction error (cur-pred) for top (blk0)
         * and bottom field (blk1)
         */
        for (j=0; j<8; j++)
        {
          offs = width*((j<<1)+j0) + i0;
          for (i=0; i<16; i++)
          {
            blk0[16*j+i] = cur[offs] - pred[offs];
            blk1[16*j+i] = cur[offs+width] - pred[offs+width];
            offs++;
          }
        }
        /* correlate fields */
        s0=s1=sq0=sq1=s01=0;

        for (i=0; i<128; i++)
        {
          s0+= blk0[i];
          sq0+= blk0[i]*blk0[i];
          s1+= blk1[i];
          sq1+= blk1[i]*blk1[i];
          s01+= blk0[i]*blk1[i];
        }

        d = (sq0-(s0*s0)/128.0)*(sq1-(s1*s1)/128.0);

        if (d>0.0)
        {
          r = (s01-(s0*s1)/128.0)/sqrt(d);
          if (r>0.5)
            mbi[k].dct_type = 0; /* frame DCT */
          else
            mbi[k].dct_type = 1; /* field DCT */
        }
        else
          mbi[k].dct_type = 1; /* field DCT */
      }
      k++;
    }
}

#endif
