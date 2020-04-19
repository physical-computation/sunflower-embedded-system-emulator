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


/* quantize.c, quantization / inverse quantization                          */

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

extern int QUANT_THRESHOLD;


static void iquant1_intra _ANSI_ARGS_((short *src, short *dst,
  int dc_prec, unsigned char *quant_mat, int mquant));
static void iquant1_non_intra _ANSI_ARGS_((short *src, short *dst,
  unsigned char *quant_mat, int mquant));


#ifdef LTHREAD
void ptquant(start_mbh, end_mbh)
int start_mbh, end_mbh;
{
  int k, mb_type, comp, cbp;

  /* load quantization matirices into v0 and v1, which are used by
     quant_intra, quant_non_intra, iquant_intra, iquant_non_intra.
     At this point, we are loading the matricies for child threads.
  */

  for (k=start_mbh; k<end_mbh; k++) {

      mb_type = mbinfo[k].mb_type;

      /* determine mquant (rate control) */
      /*mbinfo[k].mquant = rc_calc_mquant(k);*/
      mbinfo[k].mquant = 20;

      /* quantize macroblock */
      if (mb_type & MB_INTRA)
      {
	for (comp=0; comp<block_count; comp++) {
	  int ind = k*block_count+comp;
	  
	  quant_intra(blocks[ind],blocks[ind],
                      dc_prec,intra_q,mbinfo[k].mquant);
	}
        mbinfo[k].cbp = cbp = (1<<block_count) - 1;
	
      }
      else
      {
        cbp = 0;
        
	for (comp=0;comp<block_count;comp++) {
	  
	  int ind = k*block_count+comp;
	  
          cbp = (cbp<<1) | quant_non_intra(blocks[ind],blocks[ind],
                                           inter_q, mbinfo[k].mquant);
	}
	
        mbinfo[k].cbp = cbp;
	
      }

  }

}


void ptiquant(start_k, end_k)
int start_k, end_k;
{
  int k, j;


  for (k=start_k; k<end_k; k++) {

      if (mbinfo[k].mb_type & MB_INTRA)
	for (j=0; j<block_count; j++) {

	  int ind = k*block_count+j;

	  iquant_intra(blocks[ind],blocks[ind],
		       dc_prec, intra_q, mbinfo[k].mquant);
	}
      else
	for (j=0;j<block_count;j++) {

	  int ind = k*block_count+j;

	  iquant_non_intra(blocks[ind],blocks[ind],
			   inter_q,mbinfo[k].mquant);
	}

  }

}


#endif

/* Test Model 5 quantization
 *
 * this quantizer has a bias of 1/8 stepsize towards zero
 * (except for the DC coefficient)
 */


int quant_intra(src,dst,dc_prec,quant_mat,mquant)
short *src, *dst;
int dc_prec;
unsigned char *quant_mat;
int mquant;
{
  int i;
  int x, y, d;
  short dst0;

  x = src[0];
  d = 8>>dc_prec; /* intra_dc_mult */
  dst0 = (x>=0) ? (x+(d>>1))/d : -((-x+(d>>1))/d); /* round(x/d) */

#ifdef SSE2
  {
    /* loop invariant variables */
    int mq_3q = (3*mquant+2)>>2; 
    int mq_2 = 2*mquant;

    __asm {
      mov        eax, [src]          ; /*load src addr */
      mov        edx, [quant_mat]    ; /*load quant_mat addr */
      mov        ebx, [dst]          ;
      movd       xmm6, [mq_3q]       ;
      movd       xmm7, [mq_2]        ;
      pshufd     xmm6, xmm6, 0       ;
      pshufd     xmm7, xmm7, 0       ;
      cvtdq2ps   xmm6, xmm6          ;
      cvtdq2ps   xmm7, xmm7          ;
      pxor       xmm0, xmm0          ;
      mov        ecx, eax            ; /* 8 elements per loop, loop 8 times*/
      add        ecx, 128            ;
      
    q_intra:
      movdqu    xmm1, [eax]       ; /*load 8 shorts from src*/
      movq      xmm3, [edx]       ; /*load 8 bytes from quant_mat*/
      punpcklbw xmm3, xmm0        ; /*qm: unpack bytes to shorts */
      movdqa    xmm4, xmm3        ; /*make copy */
      movdqa    xmm5, xmm3        ;
      punpcklwd  xmm3, xmm0       ; /* from 8 bytes to 8 double words*/ 
      punpckhwd  xmm5, xmm0       ; /* xmm3 low, xmm5 high */
      pxor      xmm2, xmm2        ;
      cvtdq2ps   xmm3, xmm3       ;
      cvtdq2ps   xmm5, xmm5       ;

      psraw     xmm4, 1           ; /*d>>1*/
      psubw     xmm2, xmm1        ;
      pmaxsw    xmm2, xmm1        ; /* abs(x) */
      psllw     xmm2, 5           ; /* 32*abs(x) */
      paddw     xmm4, xmm2        ; /* 32*abs(x)+(d>>1)*/
      movdqa    xmm2, xmm4        ;
      punpckhwd xmm4, xmm0        ; /* xmm2 low, xmm4 high */
      punpcklwd xmm2, xmm0        ; /* unpack to 32-bit words */
      
      cvtdq2ps   xmm2, xmm2       ;
      cvtdq2ps   xmm4, xmm4       ;
      
      divps    xmm2, xmm3         ; /* divided by quant_mat[i] */
      divps    xmm4, xmm5         ;
      movd     xmm5, [QUANT_THRESHOLD] ;
      pshuflw  xmm5, xmm5, 0      ;
      pshufd   xmm5, xmm5, 0      ; /* 8 copies of QUANT_THRESHOLD */
      
      addps    xmm2, xmm6         ; /* add mq_3q */
      addps    xmm4, xmm6         ;
      divps    xmm2, xmm7         ; /* divided by mq_2 */
      divps    xmm4, xmm7         ; /* divided by mq_2 */

      pxor      xmm3, xmm3        ;

      cvttps2dq xmm2, xmm2        ; 

      pcmpgtw   xmm3, xmm1        ; /*create masks for neg */

      cvttps2dq xmm4, xmm4        ;

      packssdw  xmm2, xmm4        ; /*in dw, needs to pack back to w*/
      pminsw    xmm2, xmm5        ;

      movdqa    xmm4, xmm3        ; /* copy the mask */

      pxor      xmm5, xmm5        ;
      pandn     xmm3, xmm2        ; /* preserve all pos values */

      psubw     xmm5, xmm2        ; /*xmm5 has -y*/
      pand      xmm4, xmm5        ; /*mask -y with neg mask */
      por      xmm3, xmm4        ; /*(x>=0)?y:-y*/

      movdqu    [ebx], xmm3       ;

      add   eax, 16         ;
      add   edx, 8          ;
      add   ebx, 16         ;
      cmp   ecx, eax        ;
      jne   q_intra        ;
    }
    
  }

#else /* Non-SSE2 code */

  for (i=0; i<64; ) {

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    d = (3*mquant+2)>>2;
    y = (y+d)/(2*mquant); /* (y+0.75*mquant) / (2*mquant) */
    /*if (y > QUANT_THRESHOLD) y = QUANT_THRESHOLD;*/
    y = (y > QUANT_THRESHOLD)? QUANT_THRESHOLD : y;
    dst[i] = (x>=0) ? y : -y;
    i++;

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    d = (3*mquant+2)>>2;
    y = (y+d)/(2*mquant); /* (y+0.75*mquant) / (2*mquant) */
    /*if (y > QUANT_THRESHOLD) y = QUANT_THRESHOLD;*/
    y = (y > QUANT_THRESHOLD)? QUANT_THRESHOLD : y;
    dst[i] = (x>=0) ? y : -y;
    i++;

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    d = (3*mquant+2)>>2;
    y = (y+d)/(2*mquant); /* (y+0.75*mquant) / (2*mquant) */
    y = (y > QUANT_THRESHOLD)? QUANT_THRESHOLD : y;
    /*if (y > QUANT_THRESHOLD) y = QUANT_THRESHOLD;*/
    dst[i] = (x>=0) ? y : -y;
    i++;

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    d = (3*mquant+2)>>2;
    y = (y+d)/(2*mquant); /* (y+0.75*mquant) / (2*mquant) */
    y = (y > QUANT_THRESHOLD)? QUANT_THRESHOLD : y;
    /*if (y > QUANT_THRESHOLD) y = QUANT_THRESHOLD;*/
    dst[i] = (x>=0) ? y : -y;
    i++;
    
  }
#endif
  dst[0] = dst0;

  return 1;
}


int quant_non_intra(src,dst,quant_mat,mquant)
short *src, *dst;
unsigned char *quant_mat;
int mquant;
{
  int i;
  int x, y, d;
  int nzflag;

#ifdef SSE2
  {
    /* loop invariant variables */
    int mq_2 = 2*mquant;

    __asm {
      mov        eax, [src]          ; /*load src addr */
      mov        edx, [quant_mat]    ; /*load quant_mat addr */
      mov        ebx, [dst]          ;
      mov        edi, 0xffff           ;
      movd       xmm7, [mq_2]        ;
      movd       xmm5, [QUANT_THRESHOLD] ;
      pshuflw    xmm7, xmm7, 0       ;
      pshuflw    xmm5, xmm5, 0      ;
      pshufd     xmm7, xmm7, 0       ;
      pshufd     xmm5, xmm5, 0      ; /* 8 copies of QUANT_THRESHOLD */
      
      pxor    xmm0, xmm0          ;
      mov        ecx, eax            ; /* 8 elements per loop, loop 8 times*/
      add        ecx, 128            ;
      
    q_non_intra:
      pxor      xmm2, xmm2        ;
      movdqu    xmm1, [eax]       ; /*load 8 shorts from src*/
      movq      xmm3, [edx]       ; /*load 8 bytes from quant_mat*/
      punpcklbw xmm3, xmm2        ; /*qm: unpack bytes to shorts */

      movdqa    xmm6, xmm7        ;
      movdqa    xmm4, xmm3        ; /*make copy */
      pmullw    xmm6, xmm3        ; /*denom: quant_mat * mq_2 */
      movdqa    xmm3, xmm6        ;

      punpckhwd  xmm6, xmm2       ; /* xmm3 low, xmm6 high */
      punpcklwd  xmm3, xmm2       ; /* from 8 bytes to 8 double words*/ 
      cvtdq2ps   xmm6, xmm6       ;
      cvtdq2ps   xmm3, xmm3       ;

      psraw     xmm4, 1           ; /*d>>1*/
      psubw     xmm2, xmm1        ;
      pmaxsw    xmm2, xmm1        ; /* abs(x) */
      psllw     xmm2, 5           ; /* 32*abs(x) */
      paddw     xmm4, xmm2        ; /* 32*abs(x)+(d>>1)*/
      movdqa    xmm2, xmm4        ;
      punpckhwd xmm4, xmm0        ; /* xmm2 low, xmm4 high */
      punpcklwd xmm2, xmm0        ; /* unpack to 32-bit words */
      
      cvtdq2ps   xmm2, xmm2       ;
      cvtdq2ps   xmm4, xmm4       ;
      
      divps    xmm2, xmm3         ; /* divided by quant_mat[i] */
      divps    xmm4, xmm6         ;

      pxor      xmm3, xmm3        ;

      cvttps2dq xmm2, xmm2        ; 

      pcmpgtw   xmm3, xmm1        ; /*create masks for neg */

      cvttps2dq xmm4, xmm4        ;

      packssdw  xmm2, xmm4        ; /*in dw, needs to pack back to w*/
      pminsw    xmm2, xmm5        ;
      movdqa    xmm4, xmm3        ; /* copy the mask */

      pxor      xmm6, xmm6        ;
      pandn     xmm3, xmm2        ; /* preserve all pos values */
      pxor     xmm1, xmm1        ;

      psubw     xmm6, xmm2        ; /*xmm5 has -y*/
      pand      xmm4, xmm6        ; /*mask -y with neg mask */
      por      xmm3, xmm4        ; /*(x>=0)?y:-y*/

      pcmpeqb  xmm1, xmm3        ; /* compare to zero*/
      pmovmskb esi, xmm1         ;
      and      edi, esi          ;

      movdqu    [ebx], xmm3       ;

      add   eax, 16         ;
      add   edx, 8          ;
      add   ebx, 16         ;
      cmp   ecx, eax        ;
      jne   q_non_intra     ;
      mov   [nzflag], edi   ;
    }
    if (nzflag!=0xffff) 
      nzflag=1;
    else
      nzflag=0;
    
  }

#else /*original code*/
  nzflag = 0;

  for (i=0; i<64; ) {

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    y /= (2*mquant);
    if (y > QUANT_THRESHOLD)  y = QUANT_THRESHOLD;
    if ((dst[i] = (x>=0 ? y : -y)) != 0) nzflag=1;
    i++;

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    y /= (2*mquant);
    if (y > QUANT_THRESHOLD)  y = QUANT_THRESHOLD;
    if ((dst[i] = (x>=0 ? y : -y)) != 0) nzflag=1;
    i++;

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    y /= (2*mquant);
    if (y > QUANT_THRESHOLD)  y = QUANT_THRESHOLD;
    if ((dst[i] = (x>=0 ? y : -y)) != 0) nzflag=1;
    i++;

    x = src[i];
    d = quant_mat[i];
    y = (32*(x>=0 ? x : -x) + (d>>1))/d; /* round(32*x/quant_mat) */
    y /= (2*mquant);
    if (y > QUANT_THRESHOLD)  y = QUANT_THRESHOLD;
    if ((dst[i] = (x>=0 ? y : -y)) != 0) nzflag=1;
    i++;

  }
#endif

  return nzflag;
}

/* MPEG-2 inverse quantization */

void iquant_intra(src,dst,dc_prec,quant_mat,mquant)
short *src, *dst;
int dc_prec;
unsigned char *quant_mat;
int mquant;
{
  int i, val, sum, tmp;

  if (mpeg1)
    iquant1_intra(src,dst,dc_prec,quant_mat,mquant);
  else {
#if 1
    /*Note: Only the LSB of sum matters for mismatch control, thus,
      there's no need for all the accumulations, XORs do just fine and
      save the initial computation of dst[0] */

    tmp = sum = src[0] << (3-dc_prec);

#ifdef SSE2
    {
      const short upper[8] = {2047,2047,2047,2047,
			      2047,2047,2047,2047};
      const short lower[8] = {-2048,-2048,-2048,-2048,
			      -2048,-2048,-2048,-2048};
      __asm {
	movd       xmm5, [mquant]      ;
	movdqu     xmm6, [upper]       ;
	movdqu     xmm7, [lower]       ;
	mov        eax, [src]          ; /*load src addr */
	mov        edx, [quant_mat]    ; /*load quant_mat addr */
	mov        ebx, [dst]          ;
	pshuflw    xmm5, xmm5, 0       ;
	pshufd     xmm5, xmm5, 0       ;
	pxor       xmm0, xmm0          ;
	pxor       xmm4, xmm4          ; /* running sum */
	mov        ecx, eax            ; /* 8 elements per loop, loop 8 times*/
	add        ecx, 128            ;
      
      iq_intra:
	movdqu    xmm1, [eax]       ; /*load 8 shorts from src*/
	movq      xmm2, [edx]       ; /*load 8 bytes from quant_mat*/
	punpcklbw xmm2, xmm0      ; /*qm: unpack bytes to shorts */
	pmullw    xmm2, xmm5      ; /* qm[i] * mquant */

	movdqa  xmm3, xmm2     ; /*make copy*/

	pmulhw xmm2, xmm1      ;
	pmullw xmm3, xmm1      ;

	movdqa xmm1, xmm3      ;
	punpckhwd xmm3,xmm2     ; /*3 : left half, 4 32-bit */
	punpcklwd xmm1,xmm2    ; /*1 : right half, 4 32-bit */

	psrad  xmm3, 4        ;
	psrad  xmm1, 4        ;
	
	packssdw xmm1, xmm3   ;
	pminsw xmm1, xmm6     ;
	pmaxsw xmm1, xmm7     ;
	
	movdqu [ebx], xmm1    ;
	paddw xmm4, xmm1      ;
	add   eax, 16         ;
	add   edx, 8          ;
	add   ebx, 16         ;
	cmp   ecx, eax        ;
	jne   iq_intra        ;

	pshufd xmm0, xmm4, 0xe ; /*reduction: xmm4 has 8 words*/
	paddw xmm4, xmm0       ; /*xmm4 has 4 words */
	pshuflw xmm0, xmm4, 0xe ; 
	paddw xmm4, xmm0        ; /*xmm4 has 2 words */
	pshuflw xmm0, xmm4, 0x1 ; 
	paddw xmm4, xmm0        ;
	movd [sum], xmm4        ;
      }

    }

#else /* NON-SSE2 code */
    for(i=0; i < 64 ; ) {
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum^= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum^= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum^= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum^= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
    }
#endif
    sum^=dst[0]; /* "subtract" dst[0] out from sum*/

    dst[0] = tmp;

    /* mismatch control */
    if ((sum&1)==0)
      dst[63]^= 1;

#else /*original unrolled code */

    tmp = sum = src[0] << (3-dc_prec);
    
    val = (int)(src[0]*quant_mat[0]*mquant)/16;
    sum -= ((val>2047) ? 2047 : ((val<-2048) ? -2048 : val));
    
    for(i=0; i < 64 ; ) {
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
      val = (int)(src[i]*quant_mat[i]*mquant)/16;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;
      
    }

    dst[0] = tmp;
    
    /* mismatch control */
    if ((sum&1)==0)
      dst[63]^= 1;

#endif

  }
  
     
}


void iquant_non_intra(src,dst,quant_mat,mquant)
short *src, *dst;
unsigned char *quant_mat;
int mquant;
{
  int i, val, sum;
 
  if (mpeg1)
    iquant1_non_intra(src,dst,quant_mat,mquant);
  else {
    
#ifdef SSE2
    {
      const short upper[8] = {2047,2047,2047,2047,
			      2047,2047,2047,2047};

      __asm {
	movd       xmm5, [mquant]      ;
	movdqu     xmm6, [upper]       ;
	mov        eax, [src]          ; /*load src addr */
	mov        edx, [quant_mat]    ; /*load quant_mat addr */
	mov        ebx, [dst]          ;
	pshuflw    xmm5, xmm5, 0       ;
	pshufd     xmm5, xmm5, 0       ;
	pcmpeqw    xmm7, xmm7        ; /* contains -1s */
	pxor      xmm0, xmm0        ;
	pxor       xmm4, xmm4          ; /* running sum */
	mov        ecx, eax            ; /* 8 elements per loop, loop 8 times*/
	add        ecx, 128            ;
      
      iq_non_intra:
	movdqu    xmm1, [eax]       ; /*load 8 shorts from src*/

	psllw xmm1, 1       ; /* 2 * src[i] */
	
	movdqa  xmm2, xmm1      ; /* make 2*src[i] copy*/
	movdqa  xmm3, xmm0      ; /* make copy for 0 */
	pcmpgtw xmm3, xmm1     ; /* generate masks for < 0 */
	paddw   xmm2, xmm7       ; /* 2*src-1 */
	pand    xmm2, xmm3       ;
	movdqa  xmm3, xmm1      ;
	pcmpgtw xmm3, xmm0     ; /* generate masks for > 0 */
	psubw   xmm1, xmm7       ;
	pand    xmm1, xmm3        ;
	por    xmm1, xmm2     ; /* 2*src+(src>0)?1:(src<0)?-1:0*/

	movq      xmm2, [edx]       ; /*load 8 bytes from quant_mat*/
	punpcklbw xmm2, xmm0      ; /*qm: unpack bytes to shorts */
	pmullw    xmm2, xmm5      ; /* qm[i] * mquant */

	movdqa  xmm3, xmm2     ; /*make copy*/

	pmulhw xmm2, xmm1      ;
	pmullw xmm3, xmm1      ;

	movdqa xmm1, xmm3      ;
	punpckhwd xmm3,xmm2     ; /*3 : left half, 4 32-bit */
	punpcklwd xmm1,xmm2    ; /*1 : right half, 4 32-bit */

	psrad  xmm3, 5        ; /* divided by 32 */
	psrad  xmm1, 5        ; 
	
	packssdw xmm1, xmm3   ;
	pminsw xmm1, xmm6     ;
	pandn  xmm6, xmm7     ; /* from 2047 to -2048 */
	pmaxsw xmm1, xmm6     ;
	pandn  xmm6, xmm7     ; /* back to -2047 */
	
	movdqu [ebx], xmm1    ;
	paddw xmm4, xmm1      ;
	add   eax, 16         ;
	add   edx, 8          ;
	add   ebx, 16         ;
	cmp   ecx, eax        ;
	jne   iq_non_intra        ;

	pshufd xmm0, xmm4, 0xe ; /*reduction: xmm4 has 8 words*/
	paddw xmm4, xmm0       ; /*xmm4 has 4 words */
	pshuflw xmm0, xmm4, 0xe ; 
	paddw xmm4, xmm0        ; /*xmm4 has 2 words */
	pshuflw xmm0, xmm4, 0x1 ; 
	paddw xmm4, xmm0        ;
	movd [sum], xmm4        ;
      }

    }

#else /* NON-SSE2 code */
    sum = 0;

    for (i=0; i<64; ) {

      val = src[i];
      if (val!=0)
	val = (int)((2*val+(val>0 ? 1 : -1))*quant_mat[i]*mquant)/32;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;

      val = src[i];
      if (val!=0)
	val = (int)((2*val+(val>0 ? 1 : -1))*quant_mat[i]*mquant)/32;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;

      val = src[i];
      if (val!=0)
	val = (int)((2*val+(val>0 ? 1 : -1))*quant_mat[i]*mquant)/32;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;

      val = src[i];
      if (val!=0)
	val = (int)((2*val+(val>0 ? 1 : -1))*quant_mat[i]*mquant)/32;
      sum+= dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
      i++;

    }
#endif
    /* mismatch control */
    if ((sum&1)==0)
      dst[63]^= 1;
  }
}



/******************* Following are Not used anymore -- Ruchira ***********/



/* MPEG-1 inverse quantization */
static void iquant1_intra(src,dst,dc_prec,quant_mat,mquant)
short *src, *dst;
int dc_prec;
unsigned char *quant_mat;
int mquant;
{
  int i, val;

  dst[0] = src[0] << (3-dc_prec);
  for (i=1; i<64; i++)
  {
    val = (int)(src[i]*quant_mat[i]*mquant)/16;

    /* mismatch control */
    if ((val&1)==0 && val!=0)
      val+= (val>0) ? -1 : 1;

    /* saturation */
    dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
  }
}

static void iquant1_non_intra(src,dst,quant_mat,mquant)
short *src, *dst;
unsigned char *quant_mat;
int mquant;
{
  int i, val;

  for (i=0; i<64; i++)
  {
    val = src[i];
    if (val!=0)
    {
      val = (int)((2*val+(val>0 ? 1 : -1))*quant_mat[i]*mquant)/32;

      /* mismatch control */
      if ((val&1)==0 && val!=0)
        val+= (val>0) ? -1 : 1;
    }

    /* saturation */
    dst[i] = (val>2047) ? 2047 : ((val<-2048) ? -2048 : val);
  }
}
