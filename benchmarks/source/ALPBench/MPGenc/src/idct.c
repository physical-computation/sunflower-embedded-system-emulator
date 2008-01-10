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


/* idct.c, inverse fast discrete cosine transform                           */

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

/*
 * Copyright (C) 2000-2003 Michel Lespinasse <walken@zoy.org>
 * Copyright (C) 1999-2000 Aaron Holtzman <aholtzma@ess.engr.uvic.ca>
 *
 * This file is part of mpeg2dec, a free MPEG-2 video stream decoder.
 * See http://libmpeg2.sourceforge.net/ for updates.
 *
 * mpeg2dec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpeg2dec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**********************************************************/
/* inverse two dimensional DCT, Chen-Wang algorithm       */
/* (cf. IEEE ASSP-32, pp. 803-816, Aug. 1984)             */
/* 32-bit integer arithmetic (8 bit coefficients)         */
/* 11 mults, 29 adds per DCT                              */
/*                                      sE, 18.8.91       */
/**********************************************************/
/* coefficients extended to 12 bit for IEEE1180-1990      */
/* compliance                           sE,  2.1.94       */
/**********************************************************/

/* this code assumes >> to be a two's-complement arithmetic */
/* right shift: (-2)>>1 == -1 , (-3)>>1 == -2               */

#include <stdio.h>
#include "config.h"
#include "global.h"

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

#define CLIP_BYTE(i)  (((i)<-256) ? -256 : (((i)>255) ? 255 : (i)))

/* global declarations */
void init_idct _ANSI_ARGS_((void));
void idct _ANSI_ARGS_((short *block));
/*
#define ORIGINAL_IDCT 0
#define MATRIX_IDCT 0
*/
#if 0 
/* private data */
static short iclip[1024]; /* clipping table */
static short *iclp;
#endif

#if (ORIGINAL_IDCT)
/* private prototypes */
static void idctrow _ANSI_ARGS_((short *blk));
static void idctcol _ANSI_ARGS_((short *blk));

/* row (horizontal) IDCT
 *
 *           7                       pi         1
 * dst[k] = sum c[l] * src[l] * cos( -- * ( k + - ) * l )
 *          l=0                      8          2
 *
 * where: c[0]    = 128
 *        c[1..7] = 128*sqrt(2)
 */

static void idctrow(blk)
short *blk;
{
  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  /* shortcut */
  if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
        (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
  {
    blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;
    return;
  }

  x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */

  /* first stage */
  x8 = W7*(x4+x5);
  x4 = x8 + (W1-W7)*x4;
  x5 = x8 - (W1+W7)*x5;
  x8 = W3*(x6+x7);
  x6 = x8 - (W3-W5)*x6;
  x7 = x8 - (W3+W5)*x7;
  
  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2);
  x2 = x1 - (W2+W6)*x2;
  x3 = x1 + (W2-W6)*x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  
  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;
  
  /* fourth stage */
  blk[0] = (x7+x1)>>8;
  blk[1] = (x3+x2)>>8;
  blk[2] = (x0+x4)>>8;
  blk[3] = (x8+x6)>>8;
  blk[4] = (x8-x6)>>8;
  blk[5] = (x0-x4)>>8;
  blk[6] = (x3-x2)>>8;
  blk[7] = (x7-x1)>>8;
}

/* column (vertical) IDCT
 *
 *             7                         pi         1
 * dst[8*k] = sum c[l] * src[8*l] * cos( -- * ( k + - ) * l )
 *            l=0                        8          2
 *
 * where: c[0]    = 1/1024
 *        c[1..7] = (1/1024)*sqrt(2)
 */
static void idctcol(blk)
short *blk;
{
  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  /* shortcut */
  if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
        (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))
  {
    blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]=blk[8*6]=blk[8*7]=
      CLIP_BYTE((blk[8*0]+32)>>6);/*iclp[(blk[8*0]+32)>>6];*/
    return;
  }

  x0 = (blk[8*0]<<8) + 8192;

  /* first stage */
  x8 = W7*(x4+x5) + 4;
  x4 = (x8+(W1-W7)*x4)>>3;
  x5 = (x8-(W1+W7)*x5)>>3;
  x8 = W3*(x6+x7) + 4;
  x6 = (x8-(W3-W5)*x6)>>3;
  x7 = (x8-(W3+W5)*x7)>>3;
  
  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2) + 4;
  x2 = (x1-(W2+W6)*x2)>>3;
  x3 = (x1+(W2-W6)*x3)>>3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  
  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;
  
  /* fourth stage */
  blk[8*0] = CLIP_BYTE((x7+x1)>>14);    /*iclp[(x7+x1)>>14];*/
  blk[8*1] = CLIP_BYTE((x3+x2)>>14);    /*iclp[(x3+x2)>>14];*/
  blk[8*2] = CLIP_BYTE((x0+x4)>>14);    /*iclp[(x0+x4)>>14];*/
  blk[8*3] = CLIP_BYTE((x8+x6)>>14);    /*iclp[(x8+x6)>>14];*/
  blk[8*4] = CLIP_BYTE((x8-x6)>>14);    /*iclp[(x8-x6)>>14];*/
  blk[8*5] = CLIP_BYTE((x0-x4)>>14);    /*iclp[(x0-x4)>>14];*/
  blk[8*6] = CLIP_BYTE((x3-x2)>>14);    /*iclp[(x3-x2)>>14];*/
  blk[8*7] = CLIP_BYTE((x7-x1)>>14);    /*iclp[(x7-x1)>>14];*/
}

#else

#if !(MATRIX_IDCT)

// Intel's SSE2 implementation of iDCT
// AP-945
// http://cache-www.intel.com/cd/00/00/01/76/17680_w_idct.pdf

#define BITS_INV_ACC 4 // 4 or 5 for IEEE
#define SHIFT_INV_ROW 16 - BITS_INV_ACC
#define SHIFT_INV_COL 1 + BITS_INV_ACC
const short RND_INV_ROW = 1024 * (6 - BITS_INV_ACC); //1 << (SHIFT_INV_ROW-1)
#define RND_INV_COL (16 * (BITS_INV_ACC - 3)) // 1 << (SHIFT_INV_COL-1)
const short RND_INV_CORR = RND_INV_COL - 1; // correction -1.0 and round

#ifdef SSE2

__declspec(align(16)) short M128_one_corr[8] = {1,1,1,1,1,1,1,1};
__declspec(align(16)) short M128_round_inv_row[8] = {RND_INV_ROW, 0, RND_INV_ROW, 0, RND_INV_ROW, 0, RND_INV_ROW, 0};
__declspec(align(16)) short M128_round_inv_col[8] = {RND_INV_COL, RND_INV_COL, RND_INV_COL, RND_INV_COL, RND_INV_COL, RND_INV_COL, RND_INV_COL, RND_INV_COL};
__declspec(align(16)) short M128_round_inv_corr[8]= {RND_INV_CORR, RND_INV_CORR, RND_INV_CORR, RND_INV_CORR, RND_INV_CORR, RND_INV_CORR, RND_INV_CORR, RND_INV_CORR};
__declspec(align(16)) short M128_tg_1_16[8] = {13036, 13036, 13036, 13036, 13036, 13036, 13036, 13036}; // tg * (2<<16) + 0.5
__declspec(align(16)) short M128_tg_2_16[8] = {27146, 27146, 27146, 27146, 27146, 27146, 27146, 27146}; // tg * (2<<16) + 0.5
__declspec(align(16)) short M128_tg_3_16[8] = {-21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746}; // tg * (2<<16) + 0.5
__declspec(align(16)) short M128_cos_4_16[8] = {-19195, -19195, -19195, -19195, -19195, -19195, -19195, -19195};// cos * (2<<16) + 0.5

//-----------------------------------------------------------------------------
// Table for rows 0,4 - constants are multiplied on cos_4_16
//movq -> w13 w12 w09 w08 w05 w04 w01 w00
// w15 w14 w11 w10 w07 w06 w03 w02
// w29 w28 w25 w24 w21 w20 w17 w16
// w31 w30 w27 w26 w23 w22 w19 w18

__declspec(align(16)) short M128_tab_i_04[] = 
{
	16384, 21407, 16384, 8867, //movq -> w05 w04 w01 w00
	16384, -8867, 16384, -21407, // w13 w12 w09 w08
	16384, 8867, -16384, -21407, // w07 w06 w03 w02
	-16384, 21407, 16384, -8867, // w15 w14 w11 w10
	22725, 19266, 19266, -4520, // w21 w20 w17 w16
	12873, -22725, 4520, -12873, // w29 w28 w25 w24
	12873, 4520, -22725, -12873, // w23 w22 w19 w18
	4520, 19266, 19266, -22725  // w31 w30 w27 w26
};

// Table for rows 1,7 - constants are multiplied on cos_1_16

__declspec(align(16)) short M128_tab_i_17[] =
{
	22725, 29692, 22725, 12299, //movq -> w05 w04 w01 w00
	22725, -12299, 22725, -29692, // w13 w12 w09 w08
	22725, 12299, -22725, -29692, // w07 w06 w03 w02
	-22725, 29692, 22725, -12299, // w15 w14 w11 w10
	31521, 26722, 26722, -6270, // w21 w20 w17 w16
	17855, -31521, 6270, -17855, // w29 w28 w25 w24
	17855, 6270, -31521, -17855, // w23 w22 w19 w18
	6270, 26722, 26722, -31521 // w31 w30 w27 w26
};

// Table for rows 2,6 - constants are multiplied on cos_2_16

__declspec(align(16)) short M128_tab_i_26[] =
{
	21407, 27969, 21407, 11585, //movq -> w05 w04 w01 w00
	21407, -11585, 21407, -27969, // w13 w12 w09 w08
	21407, 11585, -21407, -27969, // w07 w06 w03 w02
	-21407, 27969, 21407, -11585, // w15 w14 w11 w10
	29692, 25172, 25172, -5906, // w21 w20 w17 w16
	16819, -29692, 5906, -16819, // w29 w28 w25 w24
	16819, 5906, -29692, -16819, // w23 w22 w19 w18
	5906, 25172, 25172, -29692 // w31 w30 w27 w26
};

// Table for rows 3,5 - constants are multiplied on cos_3_16

__declspec(align(16)) short M128_tab_i_35[] = 
{
	19266, 25172, 19266, 10426, //movq -> w05 w04 w01 w00
	19266, -10426, 19266, -25172, // w13 w12 w09 w08
	19266, 10426, -19266, -25172, // w07 w06 w03 w02
	-19266, 25172, 19266, -10426, // w15 w14 w11 w10
	26722, 22654, 22654, -5315, // w21 w20 w17 w16
	15137, -26722, 5315, -15137, // w29 w28 w25 w24
	15137, 5315, -26722, -15137, // w23 w22 w19 w18
	5315, 22654, 22654, -26722 // w31 w30 w27 w26
};

//xmm7 = round_inv_row
#define DCT_8_INV_ROW __asm{ \
	__asm pshuflw xmm0, xmm0, 0xD8 \
	__asm pshufd xmm1, xmm0, 0 \
	__asm pmaddwd xmm1, [esi] \
	__asm pshufd xmm3, xmm0, 0x55 \
	__asm pshufhw xmm0, xmm0, 0xD8 \
	__asm pmaddwd xmm3, [esi+32] \
	__asm pshufd xmm2, xmm0, 0xAA \
	__asm pshufd xmm0, xmm0, 0xFF \
	__asm pmaddwd xmm2, [esi+16] \
	__asm pshufhw xmm4, xmm4, 0xD8 \
	__asm paddd xmm1, M128_round_inv_row \
	__asm pshuflw xmm4, xmm4, 0xD8 \
	__asm pmaddwd xmm0, [esi+48] \
	__asm pshufd xmm5, xmm4, 0 \
	__asm pshufd xmm6, xmm4, 0xAA \
	__asm pmaddwd xmm5, [ecx] \
	__asm paddd xmm1, xmm2 \
	__asm movdqa xmm2, xmm1 \
	__asm pshufd xmm7, xmm4, 0x55 \
	__asm pmaddwd xmm6, [ecx+16] \
	__asm paddd xmm0, xmm3 \
	__asm pshufd xmm4, xmm4, 0xFF \
	__asm psubd xmm2, xmm0 \
	__asm pmaddwd xmm7, [ecx+32] \
	__asm paddd xmm0, xmm1 \
	__asm psrad xmm2, 12 \
	__asm paddd xmm5, M128_round_inv_row \
	__asm pmaddwd xmm4, [ecx+48] \
	__asm paddd xmm5, xmm6 \
	__asm movdqa xmm6, xmm5 \
	__asm psrad xmm0, 12 \
	__asm pshufd xmm2, xmm2, 0x1B \
	__asm packssdw xmm0, xmm2 \
	__asm paddd xmm4, xmm7 \
	__asm psubd xmm6, xmm4 \
	__asm paddd xmm4, xmm5 \
	__asm psrad xmm6, 12 \
	__asm psrad xmm4, 12 \
	__asm pshufd xmm6, xmm6, 0x1B \
	__asm packssdw xmm4, xmm6 \
}
#define DCT_8_INV_COL_8 __asm{ \
	__asm movdqa xmm1, XMMWORD PTR M128_tg_3_16 \
	__asm movdqa xmm2, xmm0 \
	__asm movdqa xmm3, XMMWORD PTR [edx+3*16] \
	__asm pmulhw xmm0, xmm1 \
	__asm pmulhw xmm1, xmm3 \
	__asm movdqa xmm5, XMMWORD PTR M128_tg_1_16 \
	__asm movdqa xmm6, xmm4 \
	__asm pmulhw xmm4, xmm5 \
	__asm paddsw xmm0, xmm2 \
	__asm pmulhw xmm5, [edx+1*16] \
	__asm paddsw xmm1, xmm3 \
	__asm movdqa xmm7, XMMWORD PTR [edx+6*16] \
	__asm paddsw xmm0, xmm3 \
	__asm movdqa xmm3, XMMWORD PTR M128_tg_2_16 \
	__asm psubsw xmm2, xmm1 \
	__asm pmulhw xmm7, xmm3 \
	__asm movdqa xmm1, xmm0 \
	__asm pmulhw xmm3, [edx+2*16] \
	__asm psubsw xmm5, xmm6 \
	__asm paddsw xmm4, [edx+1*16] \
	__asm paddsw xmm0, xmm4 \
	__asm paddsw xmm0, XMMWORD PTR M128_one_corr \
	__asm psubsw xmm4, xmm1 \
	__asm movdqa xmm6, xmm5 \
	__asm psubsw xmm5, xmm2 \
	__asm paddsw xmm5, XMMWORD PTR M128_one_corr \
	__asm paddsw xmm6, xmm2 \
	__asm movdqa [edx+7*16], xmm0 \
	__asm movdqa xmm1, xmm4 \
	__asm movdqa xmm0, XMMWORD PTR M128_cos_4_16 \
	__asm paddsw xmm4, xmm5 \
	__asm movdqa xmm2, XMMWORD PTR M128_cos_4_16 \
	__asm pmulhw xmm2, xmm4 \
	__asm movdqa [edx+3*16], xmm6 \
	__asm psubsw xmm1, xmm5 \
	__asm paddsw xmm7, [edx+2*16] \
	__asm psubsw xmm3, [edx+6*16] \
	__asm movdqa xmm6, [edx] \
	__asm pmulhw xmm0, xmm1 \
	__asm movdqa xmm5, [edx+4*16] \
	__asm paddsw xmm5, xmm6 \
	__asm psubsw xmm6, [edx+4*16] \
	__asm paddsw xmm4, xmm2 \
	__asm por xmm4, XMMWORD PTR M128_one_corr \
	__asm paddsw xmm0, xmm1 \
	__asm por xmm0, XMMWORD PTR M128_one_corr \
	__asm movdqa xmm2, xmm5 \
	__asm paddsw xmm5, xmm7 \
	__asm movdqa xmm1, xmm6 \
	__asm paddsw xmm5, XMMWORD PTR M128_round_inv_col \
	__asm psubsw xmm2, xmm7 \
	__asm movdqa xmm7, [edx+7*16] \
	__asm paddsw xmm6, xmm3 \
	__asm paddsw xmm6, XMMWORD PTR M128_round_inv_col \
	__asm paddsw xmm7, xmm5 \
	__asm psraw xmm7, SHIFT_INV_COL \
	__asm psubsw xmm1, xmm3 \
	__asm paddsw xmm1, XMMWORD PTR M128_round_inv_corr \
	__asm movdqa xmm3, xmm6 \
	__asm paddsw xmm2, XMMWORD PTR M128_round_inv_corr \
	__asm paddsw xmm6, xmm4 \
	__asm movdqa [edx], xmm7 \
	__asm psraw xmm6, SHIFT_INV_COL \
	__asm movdqa xmm7, xmm1 \
	__asm paddsw xmm1, xmm0 \
	__asm movdqa [edx+1*16], xmm6 \
	__asm psraw xmm1, SHIFT_INV_COL \
	__asm movdqa xmm6, [edx+3*16] \
	__asm psubsw xmm7, xmm0 \
	__asm psraw xmm7, SHIFT_INV_COL \
	__asm movdqa [edx+2*16], xmm1 \
	__asm psubsw xmm5, [edx+7*16] \
	__asm psraw xmm5, SHIFT_INV_COL \
	__asm movdqa [edx+7*16], xmm5 \
	__asm psubsw xmm3, xmm4 \
	__asm paddsw xmm6, xmm2 \
	__asm psubsw xmm2, [edx+3*16] \
	__asm psraw xmm6, SHIFT_INV_COL \
	__asm psraw xmm2, SHIFT_INV_COL \
	__asm movdqa [edx+3*16], xmm6 \
	__asm psraw xmm3, SHIFT_INV_COL \
	__asm movdqa [edx+4*16], xmm2 \
	__asm movdqa [edx+5*16], xmm7 \
	__asm movdqa [edx+6*16], xmm3 \
}

//assumes src and destination are aligned on a 16-byte boundary

static void idct_M128ASM(short* src)
{
  /*ASSERT(((DWORD)src & 0xf) == 0);*/ //aligned on 16-byte boundary

	__asm mov edx, src

	__asm movdqa xmm0, XMMWORD PTR[edx] //row 1
	__asm lea esi, M128_tab_i_04
	__asm movdqa xmm4, XMMWORD PTR[edx+16*2] //row 3
	__asm lea ecx, M128_tab_i_26
	DCT_8_INV_ROW; //Row 1, tab_i_04 and Row 3, tab_i_26
	__asm movdqa XMMWORD PTR[edx], xmm0
	__asm movdqa XMMWORD PTR[edx+16*2], xmm4

	__asm movdqa xmm0, XMMWORD PTR[edx+16*4] //row 5
	//__asm lea esi, M128_tab_i_04
	__asm movdqa xmm4, XMMWORD PTR[edx+16*6] //row 7
	//__asm lea ecx, M128_tab_i_26
	DCT_8_INV_ROW; //Row 5, tab_i_04 and Row 7, tab_i_26
	__asm movdqa XMMWORD PTR[edx+16*4], xmm0
	__asm movdqa XMMWORD PTR[edx+16*6], xmm4

	__asm movdqa xmm0, XMMWORD PTR[edx+16*3] //row 4
	__asm lea esi, M128_tab_i_35
	__asm movdqa xmm4, XMMWORD PTR[edx+16*1] //row 2
	__asm lea ecx, M128_tab_i_17
	DCT_8_INV_ROW; //Row 4, tab_i_35 and Row 2, tab_i_17
	__asm movdqa XMMWORD PTR[edx+16*3], xmm0
	__asm movdqa XMMWORD PTR[edx+16*1], xmm4

	__asm movdqa xmm0, XMMWORD PTR[edx+16*5] //row 6
	//__asm lea esi, M128_tab_i_35
	__asm movdqa xmm4, XMMWORD PTR[edx+16*7] //row 8
	//__asm lea ecx, M128_tab_i_17
	DCT_8_INV_ROW; //Row 6, tab_i_35 and Row 8, tab_i_17
	//__asm movdqa XMMWORD PTR[edx+80], xmm0
	//__asm movdqa xmm0, XMMWORD PTR [edx+80] /* 0 /* x5 */
	//__asm movdqa XMMWORD PTR[edx+16*7], xmm4
	//__asm movdqa xmm4, XMMWORD PTR [edx+7*16]/* 4 ; x7 */
	DCT_8_INV_COL_8
	// __asm emms
}



#else

//-----------------------------------------------------------------------------
/*
;=============================================================================
;=============================================================================
;=============================================================================
;
; Inverse DCT
;
;-----------------------------------------------------------------------------
;
; This implementation calculates iDCT-2D by a row-column method.
; On the first stage the iDCT-1D is calculated for each row with use
; direct algorithm, on the second stage the calculation is executed
; at once for four columns with use of scaled iDCT-1D algorithm.
; Base R&Y algorithm for iDCT-1D is modified for second stage.
;
;=============================================================================
;-----------------------------------------------------------------------------
;
; The first stage - inverse DCTs of rows
;
;-----------------------------------------------------------------------------
; The 8-point inverse DCT direct algorithm
;-----------------------------------------------------------------------------
;*/

__declspec(align(16)) short tab_i_04[] = 
{
  16384, 21407, 16384, 8867,
  16384, 8867, -16384, -21407,
  16384, -8867, -16384, 21407,
  16384, -21407, 16384, -8867,
  22725, 19266, 12873, 4520,
  19266, -4520, -22725, -12873,
  12873, -22725, 4520, 19266,
  4520, -12873, 19266, -22725,
};

// Table for rows 1,7 - constants are multiplied on cos_1_16

__declspec(align(16)) short tab_i_17[] =
{
  22725, 29692, 22725, 12299,
  22725, 12299, -22725, -29692,
  22725, -12299, -22725, 29692,
  22725, -29692, 22725, -12299,
  31521, 26722, 17855, 6270,
  26722, -6270, -31521, -17855,
  17855, -31521, 6270, 26722,
  6270, -17855, 26722, -31521,
};

// Table for rows 2,6 - constants are multiplied on cos_2_16

__declspec(align(16)) short tab_i_26[] =
{
  21407, 27969, 21407, 11585,
  21407, 11585, -21407, -27969,
  21407, -11585, -21407, 27969,
  21407, -27969, 21407, -11585,
  29692, 25172, 16819, 5906,
  25172, -5906, -29692, -16819,
  16819, -29692, 5906, 25172,
  5906, -16819, 25172, -29692,
};

// Table for rows 3,5 - constants are multiplied on cos_3_16

__declspec(align(16)) short tab_i_35[] = 
{
  19266, 25172, 19266, 10426,
  19266, 10426, -19266, -25172,
  19266, -10426, -19266, 25172,
  19266, -25172, 19266, -10426,
  26722, 22654, 15137, 5315,
  22654, -5315, -26722, -15137,
  15137, -26722, 5315, 22654,
  5315, -15137, 22654, -26722,
};


#define SHIFT_ROUND_ROW(x) (((x)+RND_INV_ROW)>>SHIFT_INV_ROW)

/*
  static const short w[32] = {
  FIX(cos_4_16), FIX(cos_2_16), FIX(cos_4_16), FIX(cos_6_16),
  FIX(cos_4_16), FIX(cos_6_16), -FIX(cos_4_16), -FIX(cos_2_16),
  FIX(cos_4_16), -FIX(cos_6_16), -FIX(cos_4_16), FIX(cos_2_16),
  FIX(cos_4_16), -FIX(cos_2_16), FIX(cos_4_16), -FIX(cos_6_16),
  FIX(cos_1_16), FIX(cos_3_16), FIX(cos_5_16), FIX(cos_7_16),
  FIX(cos_3_16), -FIX(cos_7_16), -FIX(cos_1_16), -FIX(cos_5_16),
  FIX(cos_5_16), -FIX(cos_1_16), FIX(cos_7_16), FIX(cos_3_16),
  FIX(cos_7_16), -FIX(cos_5_16), FIX(cos_3_16), -FIX(cos_1_16) };
*/

#define DCT_8_INV_ROW_SCALAR(x, y, w) \
{ \
  int a0, a1, a2, a3, b0, b1, b2, b3; \
 \
  a0 = x[0] * w[ 0] + x[2] * w[ 1] + x[4] * w[ 2] + x[6] * w[ 3]; \
  a1 = x[0] * w[ 4] + x[2] * w[ 5] + x[4] * w[ 6] + x[6] * w[ 7]; \
  a2 = x[0] * w[ 8] + x[2] * w[ 9] + x[4] * w[10] + x[6] * w[11];\
  a3 = x[0] * w[12] + x[2] * w[13] + x[4] * w[14] + x[6] * w[15];\
  b0 = x[1] * w[16] + x[3] * w[17] + x[5] * w[18] + x[7] * w[19];\
  b1 = x[1] * w[20] + x[3] * w[21] + x[5] * w[22] + x[7] * w[23];\
  b2 = x[1] * w[24] + x[3] * w[25] + x[5] * w[26] + x[7] * w[27];\
  b3 = x[1] * w[28] + x[3] * w[29] + x[5] * w[30] + x[7] * w[31];\
\
  y[0] = SHIFT_ROUND_ROW ( a0 + b0 );\
  y[1] = SHIFT_ROUND_ROW ( a1 + b1 );\
  y[2] = SHIFT_ROUND_ROW ( a2 + b2 );\
  y[3] = SHIFT_ROUND_ROW ( a3 + b3 );\
  y[4] = SHIFT_ROUND_ROW ( a3 - b3 );\
  y[5] = SHIFT_ROUND_ROW ( a2 - b2 );\
  y[6] = SHIFT_ROUND_ROW ( a1 - b1 );\
  y[7] = SHIFT_ROUND_ROW ( a0 - b0 );\
}

/*
;-----------------------------------------------------------------------------
;
; In this implementation the outputs of the iDCT-1D are multiplied
; for rows 0,4 - on cos_4_16,
; for rows 1,7 - on cos_1_16,
; for rows 2,6 - on cos_2_16,
; for rows 3,5 - on cos_3_16
; and are shifted to the left for rise of accuracy
;
; For used constants
; FIX(float_const) = (short) (float_const * (1<<15) + 0.5)
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;
; The second stage - inverse DCTs of columns
;
; The inputs are multiplied
; for rows 0,4 - on cos_4_16,
; for rows 1,7 - on cos_1_16,
; for rows 2,6 - on cos_2_16,
; for rows 3,5 - on cos_3_16
; and are shifted to the left for rise of accuracy
;
;-----------------------------------------------------------------------------
;
; The 8-point scaled inverse DCT algorithm (26a8m)
;
;-----------------------------------------------------------------------------
;
*/

#define SHIFT_COL(x)  ((x)>>SHIFT_INV_COL)
#define CLIP(x)  ((x)>32767?32767:(((x)<-32768)?-32768:(x)))
#define tg_1_16 13036
#define tg_2_16  27146
#define tg_3_16  -21746
#define cos_4_16 -19195

#define DCT_8_INV_COL(x, y)				\
  {							\
    int t0, t1, t2, t3, t4, t5, t6, t7;			\
    int tp03, tm03, tp12, tm12, tp65, tm65;		\
    int tp465, tm465, tp765, tm765;			\
							\
    tp765 = x[8*1] + (((int) x[8*7] * tg_1_16)>>16);		\
    tp465 = (((int)x[8*1] * tg_1_16)>>16) - x[8*7];		\
    tm765 = (((int)x[8*5] * tg_3_16)>>16) + x[8*5] + x[8*3];	\
    tm465 = x[8*5] - (((int)x[8*3] * tg_3_16)>>16)-x[8*3];		\
								\
    t7 = tp765 + tm765 + 1;					\
    tp65 = tp765 - tm765;				\
    t4 = tp465 + tm465;					\
    tm65 = tp465 - tm465 + 1;				\
							\
    t6 = (((int)( tp65 + tm65 ) * cos_4_16)>>16)|0x1;	\
    t6 += tp65+tm65;					\
    t5 = (((int)( tp65 - tm65 ) * cos_4_16)>>16)|0x1;	\
    t5 += (tp65-tm65);					\
							\
    tp03 = x[8*0] + x[8*4];				\
    tp12 = x[8*0] - x[8*4];				\
							\
    tm03 = x[8*2] + (((int)x[8*6] * tg_2_16)>>16);	\
    tm12 = (((int)x[8*2] * tg_2_16)>>16) - x[8*6];	\
    							\
    t0 = tp03 + tm03+RND_INV_COL;			\
    t3 = tp03 - tm03+RND_INV_CORR;			\
    t1 = tp12 + tm12+RND_INV_COL;			\
    t2 = tp12 - tm12+RND_INV_CORR;   			\
    							\
    y[8*0] = SHIFT_COL ( CLIP(t0 + t7 ));		\
    y[8*7] = SHIFT_COL (  CLIP(t0 - t7 ));	\
    y[8*1] = SHIFT_COL (  CLIP(t1 + t6 ));	\
    y[8*6] = SHIFT_COL (  CLIP(t1 - t6 ));	\
    y[8*2] = SHIFT_COL (  CLIP(t2 + t5 ));	\
    y[8*5] = SHIFT_COL (  CLIP(t2 - t5 ));	\
    y[8*3] = SHIFT_COL (  CLIP(t3 + t4 ));	\
    y[8*4] = SHIFT_COL (  CLIP(t3 - t4 ));	\
  }



static void idct_M128ASM_scalar(short* src)
{
  /*04 17 26 35 */
  DCT_8_INV_ROW_SCALAR(src,src,tab_i_04);
  DCT_8_INV_ROW_SCALAR((src+8*4),(src+8*4),tab_i_04);

  DCT_8_INV_ROW_SCALAR((src+8),(src+8),tab_i_17);
  DCT_8_INV_ROW_SCALAR((src+8*7),(src+8*7),tab_i_17);

  DCT_8_INV_ROW_SCALAR((src+8*2),(src+8*2),tab_i_26);
  DCT_8_INV_ROW_SCALAR((src+8*6),(src+8*6),tab_i_26);

  DCT_8_INV_ROW_SCALAR((src+8*3),(src+8*3),tab_i_35);
  DCT_8_INV_ROW_SCALAR((src+8*5),(src+8*5),tab_i_35);

  DCT_8_INV_COL(src, src);
  DCT_8_INV_COL((src+1),(src+1));
  DCT_8_INV_COL((src+2),(src+2));
  DCT_8_INV_COL((src+3),(src+3));
  DCT_8_INV_COL((src+4),(src+4));
  DCT_8_INV_COL((src+5),(src+5));
  DCT_8_INV_COL((src+6),(src+6));
  DCT_8_INV_COL((src+7),(src+7));
}

#endif
#endif
#endif
/*
  ;
  ;-----------------------------------------------------------------------------
*/



/* two dimensional inverse discrete cosine transform */
void idct(block)
short *block;
{
#if (ORIGINAL_IDCT)
    int i;
    for (i=0; i<8; i++)
      idctrow(block+8*i);
    
    for (i=0; i<8; i++)
      idctcol(block+i);
#elif (MATRIX_IDCT)
    int i, j, k;
    int s;
    __declspec(align(16)) short tmp[64];
#ifdef SSE2

    short *b_p, *icp, *tmp_p, *b2_p;

    /* not the most elegant approach, can be improved with other fast IDCT 
       methods out there */
    
    b_p = block;/*block+8*i;*/
    icp = &ic[0][0];
    tmp_p = &tmp[0]; /*&tmp[8*i];*/
    b2_p = &block[63];

    /*	
    for (i=0; i<8; i++) {

      for (j=0; j<8; j++) {
      
        s=0;

        for (k=0; k<8; k++)
          s += ic[j][k] * block[8*i+k];          

        tmp[8*i+j] = s >> 15;
      }
    }
    */
    __asm 
	{
	  mov   ecx, [b2_p]          ;
	  mov   ebx, [b_p]        ; /*load blk ptr*/
	  mov   edx, [icp]       ; /*load ic ptr */
	  mov   eax, [tmp_p]    ;
      
	idct_row_loop:
	  movdqa   xmm0, [ebx]      ; /*load 8 shorts from block*/
	  movdqa   xmm1, [edx]         ; /*ic[0][0..7]*/
	  movdqa   xmm2, [edx+16]      ; /*ic[1][0..7]*/
	  movdqa   xmm3, [edx+32]      ; /*ic[2][0..7]*/
	  movdqa   xmm4, [edx+48]      ; /*ic[3][0..7]*/

	  movdqa   xmm7, [edx+64]      ; /*ic[4][0..7]*/

	  pmaddwd xmm1, xmm0     ; /* xmms : 4 32-bit partial sums */
	  pmaddwd xmm2, xmm0     ;
	  pmaddwd xmm3, xmm0     ;
	  pmaddwd xmm4, xmm0     ;
	  pmaddwd xmm7, xmm0     ;

	  movdqa  xmm5, xmm1     ;
	  movdqa  xmm6, xmm3     ;

	  unpcklpd xmm1, xmm2    ; /*xmm1: 2.1 2.0 1.1 1.0 */
	  unpckhpd xmm5, xmm2    ; /*xmm5: 2.3 2.2 1.3 1.2 */

	  movdqa   xmm2, [edx+80]      ; /*ic[5][0..7]*/

	  unpcklpd xmm3, xmm4    ;
	  unpckhpd xmm6, xmm4    ;

	  movdqa   xmm4, [edx+96]      ; /*ic[6][0..7]*/
	  pmaddwd  xmm2, xmm0    ;

	  paddd  xmm6, xmm3      ; 

	  movdqa   xmm3, [edx+112]      ; /*ic[7][0..7]*/
	  pmaddwd xmm4, xmm0     ;
	  pmaddwd xmm3, xmm0     ; /*2nd batch, 7,2,4,3 */

	  paddd  xmm5, xmm1      ; /*xmm5: 2.a 2.b | 1.a 1.b */

	  movdqa xmm1, xmm5      ;
	  shufps  xmm1, xmm6, 0x8d ; /* 1: 3.a, 2.a, 1.a, 0.a */
	  shufps  xmm5, xmm6, 0xd8 ; /* 5: 3.b, 2.b, 1.b, 0.b */
	  
	  movdqa xmm6, xmm7      ;

	  paddd  xmm1, xmm5      ;

	  movdqa xmm5, xmm4      ;
	  
	  unpcklpd xmm7, xmm2    ; /*xmm7: 2.1 2.0 1.1 1.0 */
	  unpckhpd xmm6, xmm2    ; /*xmm6: 2.3 2.2 1.3 1.2 */

	  unpcklpd xmm4, xmm3    ;
	  unpckhpd xmm5, xmm3    ;

	  paddd xmm6, xmm7       ;
	  paddd xmm5, xmm4       ;

	  movdqa xmm2, xmm6      ;
	  shufps  xmm2, xmm5, 0x8d ; /* 1: 3.a, 2.a, 1.a, 0.a */
	  shufps  xmm6, xmm5, 0xd8 ; /* 5: 3.b, 2.b, 1.b, 0.b */

	  paddd xmm2, xmm6       ;

	  psrad  xmm1, 15        ;
	  psrad  xmm2, 15         ;

	  packssdw xmm1, xmm2    ;

	  movdqa [eax], xmm1     ;

	  add   ebx, 16         ;
	  add   eax, 16         ;

	  cmp   ebx, ecx        ;
	  jb    idct_row_loop   ;
	  
	  ; /* start of second phase */
	mov edx, [tmp_p]        ; /* load tmp blk ptr */
	mov eax, [icp]          ; /* load coeffs table ptr*/
	mov ebx, [b_p]          ;
	mov edi, [b2_p]         ;

      idct_col_outer:
	movdqa xmm6, [eax]      ; /* load ic[0][0..7] */
	
	pxor xmm4, xmm4         ;
	pxor xmm5, xmm5         ;
	mov  ecx, 8             ;

      idct_col_loop:
	movdqa xmm1, [edx]      ; /* load tmp[0][0..7] */
	pshuflw xmm0, xmm6, 0   ; /* copy the lowest word to all positions */
	;pshuflw xmm0, xmm0, 0   ;
	pshufd xmm0, xmm0, 0
	psrldq  xmm6, 2         ; /* prepare for next word*/
	
	movdqa xmm2, xmm0       ;
	pmulhw xmm2, xmm1       ; /*multiply them */
	pmullw xmm0, xmm1       ; /*2 :high words 1: low words */

	movdqa xmm3, xmm0       ; /*3 : low words */
	punpckhwd xmm0,xmm2     ; /*1 : left half, 4 32-bit */
	punpcklwd xmm3,xmm2    ; /*3 : right half, 4 32-bit */

	paddd xmm4, xmm0      ; /*accumulate */
	paddd xmm5, xmm3      ; /*4 : left, 5 : right*/

	add  edx, 16          ;
	
	loop idct_col_loop    ; /* end of inner loop */

	sub  edx, 128           ;
	psrad xmm4, 15        ; /*s >> 15 */
	psrad xmm5, 15        ;
	
	packssdw xmm5, xmm4   ;

	movdqa [ebx], xmm5    ; /*store to block[i][j]*/

	add  ebx, 16          ;
	add  eax, 16          ;
	cmp  ebx, edi         ;
	jb   idct_col_outer   ;
      }
#else /* NON-SSE2 code */

    for (i=0; i<8; i++) { 

      for (j=0; j<8; j++) { 

	s = 0;
	
	for (k=0; k<8; k++)
	  s += ic[j][k] * block[8*i+k];          /* this is block[i][k] */
	tmp[8*i+j] = s >> 15;                         /* this is block[i][j] */

      }  /* for j */

    } /* for i */
    

    /* 	simd multiplication of AxB = C is done as follows:
	If col_dim(A) == N
  	To produce row C[0]

	For i=0 to N-1 {

		// each element in the 0th row of A is multiplied by
		// all elements in row B[i] and a running sum is kep in sum

		sum[0..N-1] = A[0][i] * B[k][0..N-1]   

	} 

	C[0][0..N]  = sum[0..N];

        If we do the above loop for all rows of A, we can produce C entirely.
   */



    for (j=0; j<8; j++) {
      

      for (i=0; i<8; i++) {

	s = 0;
	
	for (k=0; k<8; k++)
	  s += ic[i][k] * tmp[8*k+j];
	block[8*i+j] =  s >> 15;
	
      } /* end of i */

    } /* end of j */

#endif /* end non-SSE2 */

#else /* INTEL IDCT */
#ifdef SSE2
    idct_M128ASM(block);
#else
    idct_M128ASM_scalar(block);
#endif
#endif
}

void init_idct()
{
#if 0
  int i;

  iclp = iclip+512;
  for (i= -512; i<512; i++)
    iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);
#endif
}

