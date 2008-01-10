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


/* putmpg.c, block and motion vector encoding routines                      */

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

/* generate variable length codes for an intra-coded block (6.2.6, 6.3.17) */
void putintrablk(blk,cc)
short *blk;
int cc;
{
  int n, dct_diff, run, signed_level;

  /* DC coefficient (7.2.1) */
  dct_diff = blk[0] - dc_dct_pred[cc]; /* difference to previous block */
  dc_dct_pred[cc] = blk[0];

  if (cc==0)
    putDClum(dct_diff);
  else
    putDCchrom(dct_diff);

  /* AC coefficients (7.2.2) */
  run = 0;
  for (n=1; n<64; n++)
  {
    /* use appropriate entropy scanning pattern */
    signed_level = blk[(altscan ? alternate_scan : zig_zag_scan)[n]];
    if (signed_level!=0)
    {
      putAC(run,signed_level,intravlc);
      run = 0;
    }
    else
      run++; /* count zero coefficients */
  }

  /* End of Block -- normative block punctuation */
  if (intravlc)
    putbits(6,4); /* 0110 (Table B-15) */
  else
    putbits(2,2); /* 10 (Table B-14) */
}

/* generate variable length codes for a non-intra-coded block (6.2.6, 6.3.17) */
void putnonintrablk(blk)
short *blk;
{
  int n, run, signed_level, first;

  run = 0;
  first = 1;

  for (n=0; n<64; n++)
  {
    /* use appropriate entropy scanning pattern */
    signed_level = blk[(altscan ? alternate_scan : zig_zag_scan)[n]];

    if (signed_level!=0)
    {
      if (first)
      {
        /* first coefficient in non-intra block */
        putACfirst(run,signed_level);
        first = 0;
      }
      else
        putAC(run,signed_level,0);

      run = 0;
    }
    else
      run++; /* count zero coefficients */
  }

  /* End of Block -- normative block punctuation  */
  putbits(2,2);
}

/* generate variable length code for a motion vector component (7.6.3.1) */
void putmv(dmv,f_code)
int dmv,f_code;
{
  int r_size, f, vmin, vmax, dv, temp, motion_code, motion_residual;

  r_size = f_code - 1; /* number of fixed length code ('residual') bits */
  f = 1<<r_size;
  vmin = -16*f; /* lower range limit */
  vmax = 16*f - 1; /* upper range limit */
  dv = 32*f;

  /* fold vector difference into [vmin...vmax] */
  if (dmv>vmax)
    dmv-= dv;
  else if (dmv<vmin)
    dmv+= dv;

  /* check value */
  if (dmv<vmin || dmv>vmax)
    if (!quiet)
      fprintf(stderr,"invalid motion vector\n");

  /* split dmv into motion_code and motion_residual */
  temp = ((dmv<0) ? -dmv : dmv) + f - 1;
  motion_code = temp>>r_size;
  if (dmv<0)
    motion_code = -motion_code;
  motion_residual = temp & (f-1);

  putmotioncode(motion_code); /* variable length code */

  if (r_size!=0 && motion_code!=0)
    putbits(motion_residual,r_size); /* fixed length code */
}

#ifdef LTHREAD
void put_mv(dmv,f_code,id)
int dmv,f_code,id;
{
  int r_size, f, vmin, vmax, dv, temp, motion_code, motion_residual;

  r_size = f_code - 1; /* number of fixed length code ('residual') bits */
  f = 1<<r_size;
  vmin = -16*f; /* lower range limit */
  vmax = 16*f - 1; /* upper range limit */
  dv = 32*f;

  /* fold vector difference into [vmin...vmax] */
  if (dmv>vmax)
    dmv-= dv;
  else if (dmv<vmin)
    dmv+= dv;

  /* check value */
  if (dmv<vmin || dmv>vmax)
    if (!quiet)
      fprintf(stderr,"invalid motion vector\n");

  /* split dmv into motion_code and motion_residual */
  temp = ((dmv<0) ? -dmv : dmv) + f - 1;
  motion_code = temp>>r_size;
  if (dmv<0)
    motion_code = -motion_code;
  motion_residual = temp & (f-1);

  put_motioncode(motion_code,id); /* variable length code */

  if (r_size!=0 && motion_code!=0)
    put_bits(motion_residual,r_size,id); /* fixed length code */
}
void put_intrablk(blk,cc,id)
short *blk;
int cc,id;
{
  int n, dct_diff, run, signed_level;

  /* DC coefficient (7.2.1) */
  dct_diff = blk[0] - pt_dc_dct_pred[id][cc]; /* difference to previous block */
  pt_dc_dct_pred[id][cc] = blk[0];

  if (cc==0)
    put_DClum(dct_diff,id);
  else
    put_DCchrom(dct_diff,id);

  /* AC coefficients (7.2.2) */
  run = 0;
  for (n=1; n<64; n++)
  {
    /* use appropriate entropy scanning pattern */
    signed_level = blk[(altscan ? alternate_scan : zig_zag_scan)[n]];
    if (signed_level!=0)
    {
      put_AC(run,signed_level,intravlc,id);
      run = 0;
    }
    else
      run++; /* count zero coefficients */
  }

  /* End of Block -- normative block punctuation */
  if (intravlc)
    put_bits(6,4,id); /* 0110 (Table B-15) */
  else
    put_bits(2,2,id); /* 10 (Table B-14) */
}

/* generate variable length codes for a non-intra-coded block (6.2.6, 6.3.17) */
void put_nonintrablk(blk,id)
short *blk;
int id;
{
  int n, run, signed_level, first;

  run = 0;
  first = 1;

  for (n=0; n<64; n++)
  {
    /* use appropriate entropy scanning pattern */
    signed_level = blk[(altscan ? alternate_scan : zig_zag_scan)[n]];

    if (signed_level!=0)
    {
      if (first)
      {
        /* first coefficient in non-intra block */
        put_ACfirst(run,signed_level,id);
        first = 0;
      }
      else
        put_AC(run,signed_level,0,id);

      run = 0;
    }
    else
      run++; /* count zero coefficients */
  }

  /* End of Block -- normative block punctuation  */
  put_bits(2,2,id);
}

#endif
