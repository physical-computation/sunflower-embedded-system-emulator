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


/* fdctref.c, forward discrete cosine transform, double precision           */

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

#include <math.h>

#include "config.h"
#include "global.h"

#ifndef PI
# ifdef M_PI
#  define PI M_PI
# else
#  define PI 3.14159265358979323846
# endif
#endif

/* global declarations */
void init_fdct _ANSI_ARGS_((void));
void fdct _ANSI_ARGS_((short *block));

/*#define INTEL_FAST_DCT !(ORIGINAL_IDCT)*/

/* private data */
#ifndef INT_DCT
static double c[8][8]; /* transform coefficients */
#endif


void init_fdct()
{
  int i, j;
  double s;

#if !(INTEL_FAST_DCT)
  for (i=0; i<8; i++)
  {
    s = (i==0) ? sqrt(0.125) : 0.5;

    for (j=0; j<8; j++){

#ifdef INT_DCT
      /* scaled with 2^15*/
      c[i][j] = (short) ((s * cos((PI/8.0)*i*(j+0.5)) * 32768) + 0.5); 
      ic[j][i] = c[i][j];
#else
      c[i][j] = s * cos((PI/8.0)*i*(j+0.5));
#endif
    }
      
  }
#endif

}


#ifdef INT_DCT

#if (INTEL_FAST_DCT)
/*
////////////////////////////////////////////////////////////////////////
//
// DCT_8_FRW_COL(), equivalent c_code
//
// This C-code can be substituted for the same __asm block
//
// I found several *DISCREPANCIES* between the AP-922 C-listing 
// and actual corrected code (shown below).
//
////////////////////////////////////////////////////////////////////////
*/

#define BITS_FRW_ACC	2 //; 2 or 3 for accuracy
#define SHIFT_FRW_COL	BITS_FRW_ACC
#define SHIFT_FRW_ROW	(BITS_FRW_ACC + 17)
#define RND_FRW_ROW	(1 << (SHIFT_FRW_ROW-1))

#define LEFT_SHIFT( x ) ((x) << (SHIFT_FRW_COL) )   /* left shift*/
#define LEFT_SHIFT1( x ) ((x) << (SHIFT_FRW_COL+1) ) /* left shift+1*/
/* pmulhw/pmulhrw emulation macros */
#define X86_PMULHW( X ) ((short) ( ((int)X)>>16 ))   /*Intel MMX*/

const static short _tg_1_16   = 13036;  /* tg * (2<<16) + 0.5 */
const static short _tg_2_16   = 27146;  /* tg * (2<<16) + 0.5 */
const static short _tg_3_16   =-21746;  /* tg * (2<<16) + 0.5 */
const static short _cos_4_16  =-19195;  /* cos * (2<<16) + 0.5 */
const static short _ocos_4_16 = 23170;  /* cos * (2<<15) + 0.5 */
const static short _one_corr  =     1;  /* rounding compensation */
#define SHIFT_AND_ROUND_FRW_ROW( x )  ( ((x)+RND_FRW_ROW) >> SHIFT_FRW_ROW )

static short tab_frw_01234567[] = {  /* forward_dct coeff table */
16384, 16384, 21407, -8867,
16384, -16384, 8867, 21407,
16384, 16384, 8867, -21407,
-16384, 16384, -21407, -8867,
22725, 12873, 19266, -22725,
12873, 4520, 4520, 19266,
19266, 4520, -4520, -12873,
-22725, 19266, -12873, -22725,

22725, 22725, 29692, -12299,
22725, -22725, 12299, 29692,
22725, 22725, 12299, -29692,
-22725, 22725, -29692, -12299,
31521, 17855, 26722, -31521,
17855, 6270, 6270, 26722,
26722, 6270, -6270, -17855,
-31521, 26722, -17855, -31521,

21407, 21407, 27969, -11585,
21407, -21407, 11585, 27969,
21407, 21407, 11585, -27969,
-21407, 21407, -27969, -11585,
29692, 16819, 25172, -29692,
16819, 5906, 5906, 25172,
25172, 5906, -5906, -16819,
-29692, 25172, -16819, -29692,

19266, 19266, 25172, -10426,
19266, -19266, 10426, 25172,
19266, 19266, 10426, -25172,
-19266, 19266, -25172, -10426,
26722, 15137, 22654, -26722,
15137, 5315, 5315, 22654,
22654, 5315, -5315, -15137,
-26722, 22654, -15137, -26722,

16384, 16384, 21407, -8867,
16384, -16384, 8867, 21407,
16384, 16384, 8867, -21407,
-16384, 16384, -21407, -8867,
22725, 12873, 19266, -22725,
12873, 4520, 4520, 19266,
19266, 4520, -4520, -12873,
-22725, 19266, -12873, -22725,

19266, 19266, 25172, -10426,
19266, -19266, 10426, 25172,
19266, 19266, 10426, -25172,
-19266, 19266, -25172, -10426,
26722, 15137, 22654, -26722,
15137, 5315, 5315, 22654,
22654, 5315, -5315, -15137,
-26722, 22654, -15137, -26722,

21407, 21407, 27969, -11585,
21407, -21407, 11585, 27969,
21407, 21407, 11585, -27969,
-21407, 21407, -27969, -11585,
29692, 16819, 25172, -29692,
16819, 5906, 5906, 25172,
25172, 5906, -5906, -16819,
-29692, 25172, -16819, -29692,

22725, 22725, 29692, -12299,
22725, -22725, 12299, 29692,
22725, 22725, 12299, -29692,
-22725, 22725, -29692, -12299,
31521, 17855, 26722, -31521,
17855, 6270, 6270, 26722,
26722, 6270, -6270, -17855,
-31521, 26722, -17855, -31521,

};

#ifdef SSE2

#define INP eax		// pointer to (short *blk)
#define OUT ecx		// pointer to output (temporary store space qwTemp[])
#define TABLE ebx	// pointer to tab_frw_01234567[]
#define TABLEF ebx  // pointer to tg_all_16
#define round_frw_row edx
//#define round_frw_col edx

#define x0 INP + 0*16
#define x1 INP + 1*16
#define x2 INP + 2*16
#define x3 INP + 3*16
#define x4 INP + 4*16
#define x5 INP + 5*16
#define x6 INP + 6*16
#define x7 INP + 7*16
#define y0 OUT + 0*16
#define y1 OUT + 1*16
#define y2 OUT + 2*16
#define y3 OUT + 3*16
#define y4 OUT + 4*16
#define y5 OUT + 5*16
#define y6 OUT + 6*16
#define y7 OUT + 7*16

#define SHIFT_FRW_ROW_CLIP2	(4)  // 4-bit shift -> { 32768 <> 2048 }
#define SHIFT_FRW_ROW_CLIP1	( SHIFT_FRW_ROW - SHIFT_FRW_ROW_CLIP2 )

__declspec(align(16)) const static short one_corr[8] = {1,1,1,1,1,1,1,1};/* = 0x0001000100010001;*/
const static int r_frw_row[4] = {RND_FRW_ROW, RND_FRW_ROW, RND_FRW_ROW, RND_FRW_ROW};

__declspec(align(16)) const static short tg_all_16[] = {
  13036, 13036, 13036, 13036, 13036, 13036, 13036, 13036,		/* tg * (2<<16) + 0.5*/
  27146, 27146, 27146, 27146, 27146, 27146, 27146, 27146, 		// tg * (2<<16) + 0.5 
  -21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746,	// tg * (2<<16) + 0.5
  -19195, -19195, -19195, -19195, -19195, -19195, -19195, -19195,	//cos * (2<<16) + 0.5
  23170, 23170, 23170, 23170, 23170, 23170, 23170, 23170 };	//cos * (2<<15) + 0.5

#define tg_1_16 TABLEF + 0
#define tg_2_16 TABLEF + 16
#define tg_3_16 TABLEF + 32
#define cos_4_16 TABLEF + 48
#define ocos_4_16 TABLEF + 64


void
fdct( short *block )
{
    static __int64 xt70[2]; // xt7xt6xt5xt4, xt3xt2xt1xt0
    static int a0, a1, a2, a3, b0, b1, b2, b3;
    static short *sptr, *optr, *tf; // tf = table_ptr
    static short *xt = (short *) &xt70[0];
    static int j;
    
    const static short _tg_1_16   = 13036;  //tg * (2<<16) + 0.5
    const static short _tg_2_16   = 27146;  //tg * (2<<16) + 0.5
    const static short _tg_3_16   =-21746;  //tg * (2<<16) + 0.5
    const static short _cos_4_16  =-19195;  //cos * (2<<16) + 0.5
    const static short _ocos_4_16 = 23170;  //cos * (2<<15) + 0.5
    const static short _one_corr  =     1;  //rounding compensation

    static short t0, t1, t2, t3, t4, t5, t6, t7;
    static short tp03, tm03, tp12, tm12, tp65, tm65;
    static short tp465, tm465, tp765, tm765;

    __asm {

    ////////////////////////////////////////////////////////////////////////
    //
    // The high-level pseudocode for the fdct_mm32() routine :
    //
    // fdct_mm32()
    // {
    //    forward_dct_col03(); // dct_column transform on cols 0-3
    //    forward_dct_col47(); // dct_column transform on cols 4-7
    //    for ( j = 0; j < 8; j=j+1 )
    //      forward_dct_row1(j); // dct_row transform on row #j
    // }

	mov INP, dword ptr [block];		;// input data is row 0 of block[]
    ;// transform the left half of the matrix (4 columns)

    lea TABLEF, dword ptr [tg_all_16];
     mov OUT, INP;

//	lea round_frw_col, dword ptr [r_frw_col]
    // for ( i = 0; i < 2; i = i + 1)
    // the for-loop is executed twice.  We are better off unrolling the 
    // loop to avoid branch misprediction.

  mmx32_fdct_col07: // begin processing columns 0-3
    movdqa xmm0, [x1] ; 0 ; x1

    movdqa xmm1, [x6] ; 1 ; x6
     movdqa xmm2, xmm0 ; 2 ; x1

    movdqa xmm3, [x2] ; 3 ; x2
     paddsw xmm0, xmm1 ; t1 = x[1] + x[6]

    movdqa xmm4, [x5] ; 4 ; x5
     psllw xmm0, SHIFT_FRW_COL ; t1

    movdqa xmm5, [x0] ; 5 ; x0
     paddsw xmm4, xmm3 ; t2 = x[2] + x[5]

    paddsw xmm5, [x7] ; t0 = x[0] + x[7]
     psllw xmm4, SHIFT_FRW_COL ; t2

    movdqa xmm6, xmm0 ; 6 ; t1
     psubsw xmm2, xmm1 ; 1 ; t6 = x[1] - x[6]

   movdqa xmm1, xmmword ptr [tg_2_16] ; 1 ; tg_2_16
     psubsw xmm0, xmm4 ; tm12 = t1 - t2

    movdqa xmm7, [x3] ; 7 ; x3
     pmulhw xmm1, xmm0 ; tm12*tg_2_16

    paddsw xmm7, [x4] ; t3 = x[3] + x[4]
     psllw xmm5, SHIFT_FRW_COL ; t0

    paddsw xmm6, xmm4 ; 4 ; tp12 = t1 + t2
     psllw xmm7, SHIFT_FRW_COL ; t3

    movdqa xmm4, xmm5 ; 4 ; t0
     psubsw xmm5, xmm7 ; tm03 = t0 - t3

    paddsw xmm1, xmm5 ; y2 = tm03 + tm12*tg_2_16
     paddsw xmm4, xmm7 ; 7 ; tp03 = t0 + t3

    por xmm1, xmmword ptr one_corr ; correction y2 +0.5
     psllw xmm2, SHIFT_FRW_COL+1 ; t6

    pmulhw xmm5, xmmword ptr [tg_2_16] ; tm03*tg_2_16
     movdqa xmm7, xmm4 ; 7 ; tp03

    psubsw xmm3, [x5] ; t5 = x[2] - x[5]
     psubsw xmm4, xmm6 ; y4 = tp03 - tp12

    movdqa [y2], xmm1 ; 1 ; save y2
     paddsw xmm7, xmm6 ; 6 ; y0 = tp03 + tp12
     
    movdqa xmm1, [x3] ; 1 ; x3
     psllw xmm3, SHIFT_FRW_COL+1 ; t5

    psubsw xmm1, [x4] ; t4 = x[3] - x[4]
     movdqa xmm6, xmm2 ; 6 ; t6
    
    movdqa [y4], xmm4 ; 4 ; save y4
     paddsw xmm2, xmm3 ; t6 + t5

    pmulhw xmm2, xmmword ptr [ocos_4_16] ; tp65 = (t6 + t5)*cos_4_16
     psubsw xmm6, xmm3 ; 3 ; t6 - t5

    pmulhw xmm6, xmmword ptr [ocos_4_16] ; tm65 = (t6 - t5)*cos_4_16
     psubsw xmm5, xmm0 ; 0 ; y6 = tm03*tg_2_16 - tm12

    por xmm5, xmmword ptr one_corr ; correction y6 +0.5
     psllw xmm1, SHIFT_FRW_COL ; t4

    por xmm2, xmmword ptr one_corr ; correction tp65 +0.5
     movdqa xmm4, xmm1 ; 4 ; t4

    movdqa xmm3, [x0] ; 3 ; x0
     paddsw xmm1, xmm6 ; tp465 = t4 + tm65

    psubsw xmm3, [x7] ; t7 = x[0] - x[7]
     psubsw xmm4, xmm6 ; 6 ; tm465 = t4 - tm65

    movdqa xmm0, xmmword ptr [tg_1_16] ; 0 ; tg_1_16
     psllw xmm3, SHIFT_FRW_COL ; t7

    movdqa xmm6, xmmword ptr [tg_3_16] ; 6 ; tg_3_16
     pmulhw xmm0, xmm1 ; tp465*tg_1_16

    movdqa [y0], xmm7 ; 7 ; save y0
     pmulhw xmm6, xmm4 ; tm465*tg_3_16

    movdqa [y6], xmm5 ; 5 ; save y6
     movdqa xmm7, xmm3 ; 7 ; t7

    movdqa xmm5, xmmword ptr [tg_3_16] ; 5 ; tg_3_16
     psubsw xmm7, xmm2 ; tm765 = t7 - tp65

    paddsw xmm3, xmm2 ; 2 ; tp765 = t7 + tp65
     pmulhw xmm5, xmm7 ; tm765*tg_3_16

    paddsw xmm0, xmm3 ; y1 = tp765 + tp465*tg_1_16
     paddsw xmm6, xmm4 ; tm465*tg_3_16

    pmulhw xmm3, xmmword ptr [tg_1_16] ; tp765*tg_1_16
     ;//

    por xmm0, xmmword ptr one_corr ; correction y1 +0.5
     paddsw xmm5, xmm7 ; tm765*tg_3_16

    psubsw xmm7, xmm6 ; 6 ; y3 = tm765 - tm465*tg_3_16
     add INP, 0x08   ; // increment pointer

    movdqa [y1], xmm0 ; 0 ; save y1
     paddsw xmm5, xmm4 ; 4 ; y5 = tm765*tg_3_16 + tm465

    movdqa [y3], xmm7 ; 7 ; save y3
     psubsw xmm3, xmm1 ; 1 ; y7 = tp765*tg_1_16 - tp465

    movdqa [y5], xmm5 ; 5 ; save y5
    movdqa [y7], xmm3 ; 3 ; save y7 (columns 0-4)

    // end of forward_dct_col07() 
    //  done with dct_col transform


  ////////////////////////////////////////////////////////////////////////
  //
  // fdct_mmx32_rows() --
  // the following subroutine performs the row-transform operation,
  //
  //  The output is stored into block[], destroying the original
  //  source data.

  //  v1.01 - output is range-clipped to {-2048, +2047}

	mov INP, dword ptr [block];		;// row 0
	 mov edi, 0x08;	//x = 8

	lea TABLE, dword ptr [tab_frw_01234567]; // row 0
	 mov OUT, INP;

	lea round_frw_row, dword ptr [r_frw_row];
	// for ( x = 8; x > 0; --x )  // transform 1 row per iteration

// ---------- loop begin
  lp_mmx_fdct_row1:
    movq xmm0, xmmword ptr [INP]   ; /* xmm0 = 3 2 1 0 */
    movq xmm1, xmmword ptr [INP+8] ; /* xmm1 = 7 6 5 4 */

    pxor   xmm2, xmm2            ;
    pshufd xmm0, xmm0, 0x44      ; /* xmm0 = 3 2 1 0 3 2 1 0 */
    pshuflw xmm1, xmm1, 0x1b     ; /*xmm1 = 4 5 6 7 */
    psubsw  xmm2, xmm1           ; /*xmm2 = -4 -5 -6 -7 */
    punpcklqdq xmm1, xmm2        ; /* xmm1 = -4 -5 -6 -7 4 5 6 7*/
    paddsw  xmm0, xmm1           ; /* xmm0 = [3-4, 2-5, 1-6, 0-7, 3+4, 2+5, 1+6, 0+7] */
    
    pshuflw xmm0, xmm0, 0xd8     ; /* xmm0 = 7 6 5 4 3 1 2 0 */
    pshufd xmm1, xmm0, 0         ; /* xmm1 = 2 0 2 0 2 0 2 0*/
    pmaddwd xmm1, [TABLE]        ;
    pshufd  xmm2, xmm0, 0x55     ;
    pshufhw xmm0, xmm0, 0xd8     ;
    pshufd  xmm3, xmm0, 0xaa     ;
    pmaddwd xmm3, [TABLE+32]     ;
    pmaddwd xmm2, [TABLE+16]     ;
    paddd   xmm1, [round_frw_row] ;
    paddd   xmm3, [round_frw_row] ;
    pshufd  xmm0, xmm0, 0xff     ;
    pmaddwd xmm0, [TABLE+48]     ;

    paddd   xmm1, xmm2           ; /* xmm1 = a3 a2 a1 a0 */
    paddd   xmm0, xmm3           ; /* xmm0 = b3 b2 b1 b0 */

    psrad xmm1, SHIFT_FRW_ROW_CLIP1 ;
    psrad xmm0, SHIFT_FRW_ROW_CLIP1 ;

    movdqa xmm2, xmm1   ;
    punpckldq xmm1, xmm0   ;
    punpckhdq xmm2, xmm0   ;
    packssdw xmm1, xmm2    ;
    psraw xmm1, SHIFT_FRW_ROW_CLIP2;

    movdqa [OUT], xmm1             ;
    add OUT, 16;  // increment row-output address by 1 row
    add INP, 16;  // increment row-address by 1 row
    sub edi, 0x01;   // i = i - 1
    add TABLE,64;  // increment to next table
    cmp edi, 0x00;
    jg lp_mmx_fdct_row1;  // begin fdct processing on next row
    }
    
/*
    ////////////////////////////////////////////////////////////////////////
    //
    // DCT_8_FRW_COL(), equivalent c_code
    //
    // This C-code can be substituted for the same __asm block
    //
    // I found several *DISCREPANCIES* between the AP-922 C-listing 
    // and actual corrected code (shown below).
    //
    ////////////////////////////////////////////////////////////////////////

    sptr = (short *) block;
    optr = (short *) block; // output will overwrite source data!

    for ( j = 0; j < 8; j=j+1 ) // dct_frw_col1 loop
    {
      // read source-data column #j into xt[0..7]
      xt[7] = sptr[7*8];
      xt[6] = sptr[6*8];
      xt[5] = sptr[5*8];
      xt[4] = sptr[4*8];

      xt[3] = sptr[3*8];
      xt[2] = sptr[2*8];
      xt[1] = sptr[1*8];
      xt[0] = sptr[0*8];
 
#define  LEFT_SHIFT( x ) ((x) << (SHIFT_FRW_COL) )   // left shift
#define LEFT_SHIFT1( x ) ((x) << (SHIFT_FRW_COL+1) ) // left shift+1

      t0 = LEFT_SHIFT ( xt[0] + xt[7] );
      t1 = LEFT_SHIFT ( xt[1] + xt[6] );
      t2 = LEFT_SHIFT ( xt[2] + xt[5] );
      t3 = LEFT_SHIFT ( xt[3] + xt[4] );
      t4 = LEFT_SHIFT ( xt[3] - xt[4] );
      t5 = LEFT_SHIFT1( xt[2] - xt[5] ); // *** DISCREPANCY
      t6 = LEFT_SHIFT1( xt[1] - xt[6] ); // *** DISCREPANCY
      t7 = LEFT_SHIFT ( xt[0] - xt[7] );
 
      tp03 = t0 + t3;
      tm03 = t0 - t3;
      tp12 = t1 + t2;
      tm12 = t1 - t2;

// pmulhw/pmulhrw emulation macros 
#define X86_PMULHW( X ) ((short) ( ((int)X)>>16 ))   //Intel MMX
//#define X86_PMULHRW( X ) ((short) ( ( (((int)X)>>15)+1) >>1) ) //3DNow-MMX

      optr[0*8] = tp03 + tp12;
      optr[4*8] = tp03 - tp12;
      optr[2*8] = tm03 + X86_PMULHW( tm12 * _tg_2_16 );
      optr[2*8] = optr[2*8] | _one_corr; // one_correction
      optr[6*8] = X86_PMULHW( tm03 * _tg_2_16 ) - tm12;
      optr[6*8] = optr[6*8] | _one_corr; // one_correction
 
      tp65 = X86_PMULHW( (t6 +t5 )*_ocos_4_16 ); // *** DISCREPANCY
      tp65 = tp65 | _one_corr; // one_correction
      tm65 = X86_PMULHW( (t6 -t5 )*_ocos_4_16 ); // *** DISCREPANCY
  
      tp765 = t7 + tp65;
      tm765 = t7 - tp65;
      tp465 = t4 + tm65;
      tm465 = t4 - tm65;
 
      optr[1*8]  = tp765 + X86_PMULHW( tp465 * _tg_1_16 );
      optr[1*8]  = optr[1*8] | _one_corr; // one_correction
      optr[7*8] = X86_PMULHW( tp765 * _tg_1_16 ) - tp465;

//    optr[5*8] = X86_PMULHW( tm765 * _tg_3_16 ) + tm465; // *** DISCREPANCY
      // from pg8 of AP-922,  ICONST = [ const*(2^16) + 0.5 ]
      //                      const * x = PMULHW( ICONST,x ) + x
      // The constant "tg_3_16" > 0.5, thus _tg_3_16 is encoded as tg_3_16-1.0
      // optr[5*8] = X86_PMULHW( tm765 * ( tg_3_16 - 1.0 ) ) + tm465
      //           = [tm765*tg_3_16 - tm765] + tm465
      //
      // optr[5*8] + tm765 = [ tm765*tg_3_16 ] + tm465 + tm765
      //                   = [ tm765*tg_3_16 ] + tm465 <-- what we want

      optr[5*8] = X86_PMULHW( tm765 * _tg_3_16 ) + tm465 + tm765;

//    optr[3*8] = tm765 - X86_PMULHW( tm465 * _tg_3_16 ); // *** DISCREPANCY
      // The following operations must be performed in the shown order!
      // same trick (as shown for optr[5*8]) applies to optr[3*8]

      optr[3*8] = X86_PMULHW( tm465 * _tg_3_16 ) + tm465;
      optr[3*8] = tm765 - optr[3*8];

    ++sptr;   // increment source pointer +1 column
    ++optr;   // increment output pointer +1 column
  } // end for ( j = 0 ..., end of C_equivalent code for forward_dct_col_1
 
    ////////////////////////////////////////////////////////////////////////
    //
    // DCT8_FRW_ROW1(), equivalent c_code
    //
    // This C-code can be substituted for the same __asm block
    // For a derivation of this code, please read fdctmm32.doc
    ////////////////////////////////////////////////////////////////////////

    sptr = (short *) block;
    optr = (short *) block; // output will overwrite source data!
    tf = &tab_frw_01234567[ 0 ]; // fdct_row load table_forward_w

    for ( j = 0; j < 8; j=j+1 ) // dct_frw_row1 loop
    {
        // forward_dct_row input arithmetic + shuffle
        xt[3] = sptr[3] + sptr[4];
        xt[2] = sptr[2] + sptr[5];
        xt[1] = sptr[1] + sptr[6];
        xt[0] = sptr[0] + sptr[7];

        xt[7] = sptr[3] - sptr[4];
        xt[6] = sptr[2] - sptr[5];
        xt[5] = sptr[1] - sptr[6];
        xt[4] = sptr[0] - sptr[7];
  

        a3 = ( xt[0]*tf[10]+ xt[2]*tf[11]) + ( xt[1]*tf[14]+ xt[3]*tf[15]);
        a2 = ( xt[0]*tf[8] + xt[2]*tf[9] ) + ( xt[1]*tf[12]+ xt[3]*tf[13]);
        a1 = ( xt[0]*tf[2] + xt[2]*tf[3] ) + ( xt[1]*tf[6] + xt[3]*tf[7] );
        a0 = ( xt[0]*tf[0] + xt[2]*tf[1] ) + ( xt[1]*tf[4] + xt[3]*tf[5] );
        tf += 16;  // increment table pointer

        b3 = ( xt[4]*tf[10]+ xt[6]*tf[11]) + ( xt[5]*tf[14]+ xt[7]*tf[15]);
        b2 = ( xt[4]*tf[8] + xt[6]*tf[9] ) + ( xt[5]*tf[12]+ xt[7]*tf[13]);
        b1 = ( xt[4]*tf[2] + xt[6]*tf[3] ) + ( xt[5]*tf[6] + xt[7]*tf[7] );
        b0 = ( xt[4]*tf[0] + xt[6]*tf[1] ) + ( xt[5]*tf[4] + xt[7]*tf[5] );
        tf += 16;  // increment table pointer

        // apply rounding constants to scaled elements
        // note, in the MMX implementation, the shift&round is done *last.*
        // Here, the C-code applies the shifts 1st, then the clipping.
#define SHIFT_AND_ROUND_FRW_ROW( x )  ( ((x)+RND_FRW_ROW) >> SHIFT_FRW_ROW )

        a3 = SHIFT_AND_ROUND_FRW_ROW( a3 );
        a2 = SHIFT_AND_ROUND_FRW_ROW( a2 );
        a1 = SHIFT_AND_ROUND_FRW_ROW( a1 );
        a0 = SHIFT_AND_ROUND_FRW_ROW( a0 );

        b3 = SHIFT_AND_ROUND_FRW_ROW( b3 );
        b2 = SHIFT_AND_ROUND_FRW_ROW( b2 );
        b1 = SHIFT_AND_ROUND_FRW_ROW( b1 );
        b0 = SHIFT_AND_ROUND_FRW_ROW( b0 );

        // v1.01, clip output results to range {-2048, +2047}

        // In the MMX implementation, the "clipper" is integrated into
        // the shift&round operation (thanks to packssdw)
        a3 = (a3 > 2047) ?  2047 : a3; // ceiling @ +2047
        a2 = (a2 > 2047) ?  2047 : a2; // ceiling @ +2047
        a1 = (a1 > 2047) ?  2047 : a1; // ceiling @ +2047
        a0 = (a0 > 2047) ?  2047 : a0; // ceiling @ +2047
        b3 = (b3 > 2047) ?  2047 : b3; // ceiling @ +2047
        b2 = (b2 > 2047) ?  2047 : b2; // ceiling @ +2047
        b1 = (b1 > 2047) ?  2047 : b1; // ceiling @ +2047
        b0 = (b0 > 2047) ?  2047 : b0; // ceiling @ +2047

        a3 = (a3 <-2048) ? -2048 : a3; // floor   @ -2048
        a2 = (a2 <-2048) ? -2048 : a2; // floor   @ -2048
        a1 = (a1 <-2048) ? -2048 : a1; // floor   @ -2048
        a0 = (a0 <-2048) ? -2048 : a0; // floor   @ -2048
        b3 = (b3 <-2048) ? -2048 : b3; // floor   @ -2048
        b2 = (b2 <-2048) ? -2048 : b2; // floor   @ -2048
        b1 = (b1 <-2048) ? -2048 : b1; // floor   @ -2048
        b0 = (b0 <-2048) ? -2048 : b0; // floor   @ -2048


        // forward_dct_row, assign outputs
        optr[ 3 ] = b1;
        optr[ 2 ] = a1;
        optr[ 1 ] = b0;
        optr[ 0 ] = a0;

        optr[ 7 ] = b3;
        optr[ 6 ] = a3;
        optr[ 5 ] = b2;
        optr[ 4 ] = a2;

        sptr += 8;   // increment source pointer +1 row
        optr += 8;   // increment output pointer +1 row
    } // end for ( j = 0 ..., end of C_equivalent code for forward_dct_row_1
  */  
} // fdct_mm32( short *block )

#else /* non-SIMD code */

void fdct(block)
short *block;
{
  int a0, a1, a2, a3, b0, b1, b2, b3;
  short *sptr, *optr, *tf; /* tf = table_ptr*/
  int j;
  short t0, t1, t2, t3, t4, t5, t6, t7;
  short tp03, tm03, tp12, tm12, tp65, tm65;
  short tp465, tm465, tp765, tm765;
  short xt[80];

  sptr = (short *) block;
  optr = (short *) block; /* output will overwrite source data!*/
  
  for ( j = 0; j < 8; j++ ) /* dct_frw_col1 loop*/
    {      /* read source-data column #j into xt[0..7]*/
      xt[7] = sptr[7*8];
      xt[6] = sptr[6*8];
      xt[5] = sptr[5*8];
      xt[4] = sptr[4*8];
      
      xt[3] = sptr[3*8];
      xt[2] = sptr[2*8];
      xt[1] = sptr[1*8];
      xt[0] = sptr[0*8];
      
      
      t0 = LEFT_SHIFT ( xt[0] + xt[7] );
      t1 = LEFT_SHIFT ( xt[1] + xt[6] );
      t2 = LEFT_SHIFT ( xt[2] + xt[5] );
      t3 = LEFT_SHIFT ( xt[3] + xt[4] );
      t4 = LEFT_SHIFT ( xt[3] - xt[4] );
      t5 = LEFT_SHIFT1 ( xt[2] - xt[5] ); /*** DISCREPANCY*/
      t6 = LEFT_SHIFT1 ( xt[1] - xt[6] ); /*** DISCREPANCY*/
      t7 = LEFT_SHIFT ( xt[0] - xt[7] );
      
      tp03 = t0 + t3;
      tm03 = t0 - t3;
      tp12 = t1 + t2;
      tm12 = t1 - t2;

      optr[0*8] = tp03 + tp12;
      optr[4*8] = tp03 - tp12;
      optr[2*8] = tm03 + X86_PMULHW( tm12 * _tg_2_16 );
      optr[2*8] = optr[2*8] | _one_corr; /* one_correction*/
      optr[6*8] = X86_PMULHW( tm03 * _tg_2_16 ) - tm12;
      optr[6*8] = optr[6*8] | _one_corr; /* one_correction */
 
      tp65 = X86_PMULHW( (t6 +t5 )*_ocos_4_16 ); /* *** DISCREPANCY */
      tp65 = tp65 | _one_corr; /* one_correction */
      tm65 = X86_PMULHW( (t6 -t5 )*_ocos_4_16 ); /* *** DISCREPANCY */
  
      tp765 = t7 + tp65;
      tm765 = t7 - tp65;
      tp465 = t4 + tm65;
      tm465 = t4 - tm65;
 
      optr[1*8]  = tp765 + X86_PMULHW( tp465 * _tg_1_16 );
      optr[1*8]  = optr[1*8] | _one_corr; /* one_correction */
      optr[7*8] = X86_PMULHW( tp765 * _tg_1_16 ) - tp465;

      optr[5*8] = X86_PMULHW( tm765 * _tg_3_16 ) + tm465 + tm765;

      optr[3*8] = X86_PMULHW( tm465 * _tg_3_16 ) + tm465;
      optr[3*8] = tm765 - optr[3*8];

      ++sptr;   /* increment source pointer +1 column */
      ++optr;   /* increment output pointer +1 column */
    } /* end for ( j = 0 ..., end of C_equivalent code for forward_dct_col_1 */
    
  /* ////////////////////////////////////////////////////////////////////////
  //
  // DCT8_FRW_ROW1(), equivalent c_code
  //
  // This C-code can be substituted for the same __asm block
  // For a derivation of this code, please read fdctmm32.doc
  ////////////////////////////////////////////////////////////////////////
  */

  sptr = (short *) block;
  optr = (short *) block; /* output will overwrite source data! */
  tf = &tab_frw_01234567[ 0 ]; /* fdct_row load table_forward_w */

  for ( j = 0; j < 8; j++ ) /* dct_frw_row1 loop */
    {
      /* forward_dct_row input arithmetic + shuffle */
      xt[3] = sptr[3] + sptr[4];
      xt[2] = sptr[2] + sptr[5];
      xt[1] = sptr[1] + sptr[6];
      xt[0] = sptr[0] + sptr[7];
      
      xt[7] = sptr[3] - sptr[4];
      xt[6] = sptr[2] - sptr[5];
      xt[5] = sptr[1] - sptr[6];
      xt[4] = sptr[0] - sptr[7];
  
      /*
	6 7 14 15
	4 5 13 12
	2 3 10 11
	0 1 8 9

	22 23 30 31
	20 21 28 29
	18 19 26 27
	16 17 24 25
       */
      a3 = ( xt[0]*tf[6]+ xt[2]*tf[7]) + ( xt[1]*tf[14]+ xt[3]*tf[15]);
      a2 = ( xt[0]*tf[4] + xt[2]*tf[5] ) + ( xt[1]*tf[12]+ xt[3]*tf[13]);
      a1 = ( xt[0]*tf[2] + xt[2]*tf[3] ) + ( xt[1]*tf[10]+ xt[3]*tf[11] );
      a0 = ( xt[0]*tf[0] + xt[2]*tf[1] ) + ( xt[1]*tf[8] + xt[3]*tf[9] );

      b3 = ( xt[4]*tf[22] + xt[6]*tf[23]) + ( xt[5]*tf[30]+ xt[7]*tf[31]);
      b2 = ( xt[4]*tf[20] + xt[6]*tf[21] ) + ( xt[5]*tf[28]+ xt[7]*tf[29]);
      b1 = ( xt[4]*tf[18] + xt[6]*tf[19] ) + ( xt[5]*tf[26] + xt[7]*tf[27] );
      b0 = ( xt[4]*tf[16] + xt[6]*tf[17] ) + ( xt[5]*tf[24] + xt[7]*tf[25] );

      tf += 32;  /* increment table pointer */
      /*
      // apply rounding constants to scaled elements
      // note, in the MMX implementation, the shift&round is done *last.*
      // Here, the C-code applies the shifts 1st, then the clipping.
      */

      a3 = SHIFT_AND_ROUND_FRW_ROW( a3 );
      a2 = SHIFT_AND_ROUND_FRW_ROW( a2 );
      a1 = SHIFT_AND_ROUND_FRW_ROW( a1 );
      a0 = SHIFT_AND_ROUND_FRW_ROW( a0 );

      b3 = SHIFT_AND_ROUND_FRW_ROW( b3 );
      b2 = SHIFT_AND_ROUND_FRW_ROW( b2 );
      b1 = SHIFT_AND_ROUND_FRW_ROW( b1 );
      b0 = SHIFT_AND_ROUND_FRW_ROW( b0 );
      /*
      // v1.01, clip output results to range {-2048, +2047}

      // In the MMX implementation, the "clipper" is integrated into
      // the shift&round operation (thanks to packssdw)
      */
      a3 = (a3 > 2047) ?  2047 : a3; /* ceiling @ +2047 */
      a2 = (a2 > 2047) ?  2047 : a2; 
      a1 = (a1 > 2047) ?  2047 : a1; 
      a0 = (a0 > 2047) ?  2047 : a0; 
      b3 = (b3 > 2047) ?  2047 : b3; 
      b2 = (b2 > 2047) ?  2047 : b2; 
      b1 = (b1 > 2047) ?  2047 : b1; 
      b0 = (b0 > 2047) ?  2047 : b0; 

      a3 = (a3 <-2048) ? -2048 : a3; /* floor   @ -2048 */
      a2 = (a2 <-2048) ? -2048 : a2; 
      a1 = (a1 <-2048) ? -2048 : a1; 
      a0 = (a0 <-2048) ? -2048 : a0; 
      b3 = (b3 <-2048) ? -2048 : b3; 
      b2 = (b2 <-2048) ? -2048 : b2; 
      b1 = (b1 <-2048) ? -2048 : b1; 
      b0 = (b0 <-2048) ? -2048 : b0; 


      /* forward_dct_row, assign outputs*/
      optr[ 3 ] = b1;
      optr[ 2 ] = a1;
      optr[ 1 ] = b0;
      optr[ 0 ] = a0;

      optr[ 7 ] = b3;
      optr[ 6 ] = a3;
      optr[ 5 ] = b2;
      optr[ 4 ] = a2;

      sptr += 8;   /* increment source pointer +1 row*/
      optr += 8;   /* increment output pointer +1 row*/
    } /* end for ( j = 0 ..., end of C_equivalent code for forward_dct_row_1 */
} /* fdct_mm32( short *block )*/


#endif /* end non-SIMD code */


#else

void fdct(block)
short *block;
{
  __declspec(align(16)) short tmp[64];
  
#ifdef SSE2
    short *b_p, *cp, *tmp_p, *b2_p;

    /* not the most elegant approach, can be improved with other fast IDCT 
       methods out there */
    
    b_p = block;/*block+8*i;*/
    cp = &c[0][0];
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
	  mov   edx, [cp]       ; /*load ic ptr */
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
	mov eax, [cp]          ; /* load coeffs table ptr*/
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

  int i, j, k;
  int s;

  for (i=0; i<8; i++) { 

 
    for (j=0; j<8; j++) { 


      s = 0;
      for (k=0; k<8; k++)
        s += c[j][k] * block[8*i+k];          /* this is block[i][k] */

      tmp[8*i+j] = s >> 15;                         /* this is block[i][j] */
    }
  }
  for (j=0; j<8; j++) {

    for (i=0; i<8; i++) {

      s = 0;

      for (k=0; k<8; k++)
        s += c[i][k] * tmp[8*k+j];

      block[8*i+j] =  s >> 15;

    } /* end of i */
    
    
  } /* end of j */
#endif
}

#endif /* end non INTEL_FAST_DCT*/

#else /* NOT INT_DCT */
/* The following code comes with the original MSSG implementation */

void fdct(block)
short *block;
{

  double s;
  double tmp[64];
  int i, j,k;


  for (i=0; i<8; i++) { 

 
    for (j=0; j<8; j++) { 

      s = 0.0;
      for (k=0; k<8; k++)
        s += c[j][k] * block[8*i+k];          /* this is block[i][k] */
      tmp[8*i+j] = s;

    }  /* for j */
  

  } /* for i */


  for (j=0; j<8; j++) {

    for (i=0; i<8; i++) {

      s = 0.0;

      for (k=0; k<8; k++)
        s += c[i][k] * tmp[8*k+j];

      block[8*i+j] = (int)floor(s+0.499999);

      /*
       * reason for adding 0.499999 instead of 0.5:
       * s is quite often x.5 (at least for i and/or j = 0 or 4)
       * and setting the rounding threshold exactly to 0.5 leads to an
       * extremely high arithmetic implementation dependency of the result;
       * s being between x.5 and x.500001 (which is now incorrectly rounded
       * downwards instead of upwards) is assumed to occur less often
       * (if at all)
       */

    } /* end of i */


  } /* end of j */



}

#endif


