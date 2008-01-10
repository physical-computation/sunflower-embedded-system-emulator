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


/* putvlc.c, generation of variable length codes                            */

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
#include "vlc.h"

/* private prototypes */
static void putDC _ANSI_ARGS_((sVLCtable *tab, int val));

#ifdef LTHREAD
static void put_DC _ANSI_ARGS_((sVLCtable *tab, int val, int id));


/* generate variable length code for luminance DC coefficient */
void put_DClum(val,id)
int val,id;
{
  put_DC(DClumtab,val,id);
}

/* generate variable length code for chrominance DC coefficient */
void put_DCchrom(val,id)
int val,id;
{
  put_DC(DCchromtab,val,id);
}

/* generate variable length code for DC coefficient (7.2.1) */
static void put_DC(tab,val,id)
sVLCtable *tab;
int val,id;
{
  int absval, size;

  absval = (val<0) ? -val : val; /* abs(val) */

  if (absval>2047 || (mpeg1 && absval>255))
  {
    /* should never happen */
    sprintf(errortext,"DC value out of range (%d)\n",val);
    error(errortext);
  }

  /* compute dct_dc_size */
  size = 0;

  while (absval)
  {
    absval >>= 1;
    size++;
  }

  /* generate VLC for dct_dc_size (Table B-12 or B-13) */
  put_bits(tab[size].code,tab[size].len,id);

  /* append fixed length code (dc_dct_differential) */
  if (size!=0)
  {
    if (val>=0)
      absval = val;
    else
      absval = val + (1<<size) - 1; /* val + (2 ^ size) - 1 */
    put_bits(absval,size,id);
  }
}

/* generate variable length code for first coefficient
 * of a non-intra block (7.2.2.2) */
void put_ACfirst(run,val,id)
int run,val,id;
{
  if (run==0 && (val==1 || val==-1)) /* these are treated differently */
    put_bits(2|(val<0),2,id); /* generate '1s' (s=sign), (Table B-14, line 2) */
  else
    put_AC(run,val,0,id); /* no difference for all others */
}

/* generate variable length code for other DCT coefficients (7.2.2) */
void put_AC(run,signed_level,vlcformat,id)
int run,signed_level,vlcformat,id;
{
  int level, len;
  VLCtable *ptab;

  level = (signed_level<0) ? -signed_level : signed_level; /* abs(signed_level) */

  /* make sure run and level are valid */
  if (run<0 || run>63 || level==0 || level>2047 || (mpeg1 && level>255))
  {
    sprintf(errortext,"AC value out of range (run=%d, signed_level=%d)\n",
      run,signed_level);
    error(errortext);
  }

  len = 0;

  if (run<2 && level<41)
  {
    /* vlcformat selects either of Table B-14 / B-15 */
    if (vlcformat)
      ptab = &dct_code_tab1a[run][level-1];
    else
      ptab = &dct_code_tab1[run][level-1];

    len = ptab->len;
  }
  else if (run<32 && level<6)
  {
    /* vlcformat selects either of Table B-14 / B-15 */
    if (vlcformat)
      ptab = &dct_code_tab2a[run-2][level-1];
    else
      ptab = &dct_code_tab2[run-2][level-1];

    len = ptab->len;
  }

  if (len!=0) /* a VLC code exists */
  {
    put_bits(ptab->code,len,id);
    put_bits(signed_level<0,1,id); /* sign */
  }
  else
  {
    /* no VLC for this (run, level) combination: use escape coding (7.2.2.3) */
    put_bits(1l,6,id); /* Escape */
    put_bits(run,6,id); /* 6 bit code for run */
    if (mpeg1)
    {
      /* ISO/IEC 11172-2 uses a 8 or 16 bit code */
      if (signed_level>127)
        put_bits(0,8,id);
      if (signed_level<-127)
        put_bits(128,8,id);
      put_bits(signed_level,8,id);
    }
    else
    {
      /* ISO/IEC 13818-2 uses a 12 bit code, Table B-16 */
      put_bits(signed_level,12,id);
    }
  }
}

/* generate variable length code for macroblock_address_increment (6.3.16) */
void put_addrinc(addrinc,id)
int addrinc,id;
{
  while (addrinc>33)
  {
    put_bits(0x08,11,id); /* macroblock_escape */
    addrinc-= 33;
  }

  put_bits(addrinctab[addrinc-1].code,addrinctab[addrinc-1].len,id);
}

/* generate variable length code for macroblock_type (6.3.16.1) */
void put_mbtype(pict_type,mb_type,id)
int pict_type,mb_type,id;
{
  put_bits(mbtypetab[pict_type-1][mb_type].code,
          mbtypetab[pict_type-1][mb_type].len,id);
}

/* generate variable length code for motion_code (6.3.16.3) */
void put_motioncode(motion_code,id)
int motion_code,id;
{
  int abscode;

  abscode = (motion_code>=0) ? motion_code : -motion_code; /* abs(motion_code) */

  put_bits(motionvectab[abscode].code,motionvectab[abscode].len,id);
  if (motion_code!=0)
    put_bits(motion_code<0,1,id); /* sign, 0=positive, 1=negative */
}

/* generate variable length code for dmvector[t] (6.3.16.3), Table B-11 */
void put_dmv(dmv,id)
int dmv,id;
{
  if (dmv==0)
    put_bits(0,1,id);
  else if (dmv>0)
    put_bits(2,2,id);
  else
    put_bits(3,2,id);
}

/* generate variable length code for coded_block_pattern (6.3.16.4)
 *
 * 4:2:2, 4:4:4 not implemented
 */
void put_cbp(cbp,id)
int cbp,id;
{
  put_bits(cbptable[cbp].code,cbptable[cbp].len,id);
}

#endif

/* generate variable length code for luminance DC coefficient */
void putDClum(val)
int val;
{
  putDC(DClumtab,val);
}

/* generate variable length code for chrominance DC coefficient */
void putDCchrom(val)
int val;
{
  putDC(DCchromtab,val);
}

/* generate variable length code for DC coefficient (7.2.1) */
static void putDC(tab,val)
sVLCtable *tab;
int val;
{
  int absval, size;

  absval = (val<0) ? -val : val; /* abs(val) */

  if (absval>2047 || (mpeg1 && absval>255))
  {
    /* should never happen */
    sprintf(errortext,"DC value out of range (%d)\n",val);
    error(errortext);
  }

  /* compute dct_dc_size */
  size = 0;

  while (absval)
  {
    absval >>= 1;
    size++;
  }

  /* generate VLC for dct_dc_size (Table B-12 or B-13) */
  putbits(tab[size].code,tab[size].len);

  /* append fixed length code (dc_dct_differential) */
  if (size!=0)
  {
    if (val>=0)
      absval = val;
    else
      absval = val + (1<<size) - 1; /* val + (2 ^ size) - 1 */
    putbits(absval,size);
  }
}

/* generate variable length code for first coefficient
 * of a non-intra block (7.2.2.2) */
void putACfirst(run,val)
int run,val;
{
  if (run==0 && (val==1 || val==-1)) /* these are treated differently */
    putbits(2|(val<0),2); /* generate '1s' (s=sign), (Table B-14, line 2) */
  else
    putAC(run,val,0); /* no difference for all others */
}

/* generate variable length code for other DCT coefficients (7.2.2) */
void putAC(run,signed_level,vlcformat)
int run,signed_level,vlcformat;
{
  int level, len;
  VLCtable *ptab;

  level = (signed_level<0) ? -signed_level : signed_level; /* abs(signed_level) */

  /* make sure run and level are valid */
  if (run<0 || run>63 || level==0 || level>2047 || (mpeg1 && level>255))
  {
    sprintf(errortext,"AC value out of range (run=%d, signed_level=%d)\n",
      run,signed_level);
    error(errortext);
  }

  len = 0;

  if (run<2 && level<41)
  {
    /* vlcformat selects either of Table B-14 / B-15 */
    if (vlcformat)
      ptab = &dct_code_tab1a[run][level-1];
    else
      ptab = &dct_code_tab1[run][level-1];

    len = ptab->len;
  }
  else if (run<32 && level<6)
  {
    /* vlcformat selects either of Table B-14 / B-15 */
    if (vlcformat)
      ptab = &dct_code_tab2a[run-2][level-1];
    else
      ptab = &dct_code_tab2[run-2][level-1];

    len = ptab->len;
  }

  if (len!=0) /* a VLC code exists */
  {
    putbits(ptab->code,len);
    putbits(signed_level<0,1); /* sign */
  }
  else
  {
    /* no VLC for this (run, level) combination: use escape coding (7.2.2.3) */
    putbits(1l,6); /* Escape */
    putbits(run,6); /* 6 bit code for run */
    if (mpeg1)
    {
      /* ISO/IEC 11172-2 uses a 8 or 16 bit code */
      if (signed_level>127)
        putbits(0,8);
      if (signed_level<-127)
        putbits(128,8);
      putbits(signed_level,8);
    }
    else
    {
      /* ISO/IEC 13818-2 uses a 12 bit code, Table B-16 */
      putbits(signed_level,12);
    }
  }
}

/* generate variable length code for macroblock_address_increment (6.3.16) */
void putaddrinc(addrinc)
int addrinc;
{
  while (addrinc>33)
  {
    putbits(0x08,11); /* macroblock_escape */
    addrinc-= 33;
  }

  putbits(addrinctab[addrinc-1].code,addrinctab[addrinc-1].len);
}

/* generate variable length code for macroblock_type (6.3.16.1) */
void putmbtype(pict_type,mb_type)
int pict_type,mb_type;
{
  putbits(mbtypetab[pict_type-1][mb_type].code,
          mbtypetab[pict_type-1][mb_type].len);
}

/* generate variable length code for motion_code (6.3.16.3) */
void putmotioncode(motion_code)
int motion_code;
{
  int abscode;

  abscode = (motion_code>=0) ? motion_code : -motion_code; /* abs(motion_code) */

  putbits(motionvectab[abscode].code,motionvectab[abscode].len);
  if (motion_code!=0)
    putbits(motion_code<0,1); /* sign, 0=positive, 1=negative */
}

/* generate variable length code for dmvector[t] (6.3.16.3), Table B-11 */
void putdmv(dmv)
int dmv;
{
  if (dmv==0)
    putbits(0,1);
  else if (dmv>0)
    putbits(2,2);
  else
    putbits(3,2);
}

/* generate variable length code for coded_block_pattern (6.3.16.4)
 *
 * 4:2:2, 4:4:4 not implemented
 */
void putcbp(cbp)
int cbp;
{
  putbits(cbptable[cbp].code,cbptable[cbp].len);
}
