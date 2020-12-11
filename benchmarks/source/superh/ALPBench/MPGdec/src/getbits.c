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


/* getbits.c, bit level routines                                            */

/*
 * All modifications (mpeg2decode -> mpeg2play) are
 * Copyright (C) 1996, Stefan Eckart. All Rights Reserved.
 */

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
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "config.h"
#include "global.h"

/* initialize buffer, call once before first getbits or showbits */

void Initialize_Buffer()
{
  ld->Incnt = 0;
  ld->Rdptr = ld->Rdbfr + 2048;
  ld->Rdmax = ld->Rdptr;

#ifdef VERIFY
  /*  only the verifier uses this particular bit counter 
   *  Bitcnt keeps track of the current parser position with respect
   *  to the video elementary stream being decoded, regardless 
   *  of whether or not it is wrapped within a systems layer stream 
   */
  ld->Bitcnt = 0;
#endif

  ld->Bfr = 0;
  Flush_Buffer(0); /* fills valid data into bfr */
}

void Fill_Buffer()
{
  int Buffer_Level;

  Buffer_Level = read(ld->Infile,ld->Rdbfr,2048);
  ld->Rdptr = ld->Rdbfr;

  if (System_Stream_Flag)
    ld->Rdmax -= 2048;

  
  /* end of the bitstream file */
  if (Buffer_Level < 2048)
  {
    /* just to be safe */
    if (Buffer_Level < 0)
      Buffer_Level = 0;

    /* pad until the next to the next 32-bit word boundary */
    while (Buffer_Level & 3)
      ld->Rdbfr[Buffer_Level++] = 0;

	/* pad the buffer with sequence end codes */
    while (Buffer_Level < 2048)
    {
      ld->Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE>>24;
      ld->Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE>>16;
      ld->Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE>>8;
      ld->Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE&0xff;
    }
  }
}


/* MPEG-1 system layer demultiplexer */

int Get_Byte()
{
  while(ld->Rdptr >= ld->Rdbfr+2048)
  {
    read(ld->Infile,ld->Rdbfr,2048);
    ld->Rdptr -= 2048;
    ld->Rdmax -= 2048;
  }
  return *ld->Rdptr++;
}

/* extract a 16-bit word from the bitstream buffer */
int Get_Word()
{
  int Val;

  Val = Get_Byte();
  return (Val<<8) | Get_Byte();
}


/* return next n bits (right adjusted) without advancing */

unsigned int Show_Bits(N)
int N;
{
  return ld->Bfr >> (32-N);
}


/* return next bit (could be made faster than Get_Bits(1)) */

unsigned int Get_Bits1()
{
  return Get_Bits(1);
}


/* advance by n bits */

void Flush_Buffer(N)
int N;
{
  int Incnt;

  ld->Bfr <<= N;

  Incnt = ld->Incnt -= N;

  if (Incnt <= 24)
  {
    if (System_Stream_Flag && (ld->Rdptr >= ld->Rdmax-4))
    {
      do
      {
        if (ld->Rdptr >= ld->Rdmax)
          Next_Packet();
        ld->Bfr |= Get_Byte() << (24 - Incnt);
        Incnt += 8;
      }
      while (Incnt <= 24);
    }
    else if (ld->Rdptr < ld->Rdbfr+2044)
    {
      do
      {
        ld->Bfr |= *ld->Rdptr++ << (24 - Incnt);
        Incnt += 8;
      }
      while (Incnt <= 24);
    }
    else
    {
      do
      {
        if (ld->Rdptr >= ld->Rdbfr+2048)
          Fill_Buffer();
        ld->Bfr |= *ld->Rdptr++ << (24 - Incnt);
        Incnt += 8;
      }
      while (Incnt <= 24);
    }
    ld->Incnt = Incnt;
  }

#ifdef VERIFY 
  ld->Bitcnt += N;
#endif /* VERIFY */

}


/* return next n bits (right adjusted) */

unsigned int Get_Bits(N)
int N;
{
  unsigned int Val;

  Val = Show_Bits(N);
  Flush_Buffer(N);

  return Val;
}


#ifdef THRD 
/* These are thread-specific functions. They are duplicated so they can operate
   on different buffers */

void Thrd_Initialize_Buffer(t)
int t;
{
  thrd_buf[t] = 0;
  Thrd_Flush_Buffer(t, 0); /* fills valid data into bfr */
}


/* MPEG-1 system layer demultiplexer */

int Thrd_Get_Byte(int t)
{
  return *thrd_ptr[t]++;
}

/* extract a 16-bit word from the bitstream buffer */
int Thrd_Get_Word(int t)
{
  int Val;

  Val = Thrd_Get_Byte(t);
  return (Val<<8) | Thrd_Get_Byte(t);
}


/* return next n bits (right adjusted) without advancing */

unsigned int Thrd_Show_Bits(t, N)
int t;
int N;
{
  return thrd_buf[t] >> (32-N);
}


/* return next bit (could be made faster than Get_Bits(1)) */

unsigned int Thrd_Get_Bits1(int t)
{
  return Thrd_Get_Bits(t, 1);
}


/* advance by n bits */

void Thrd_Flush_Buffer(t, N)
int t;
int N;
{
  int Incnt;

  thrd_buf[t] <<= N;

  Incnt = thrd_Incnt[t] -= N;
  
  if (Incnt <= 24) {
    if (System_Stream_Flag && (ld->Rdptr >= ld->Rdmax-4)) {
      assert(0&&"System_Strem_Flag is 1\n");
      do {
        if (ld->Rdptr >= ld->Rdmax)
          Next_Packet();
        ld->Bfr |= Get_Byte() << (24 - Incnt);
        Incnt += 8;
      } while (Incnt <= 24);
    }  else {
      do {
        thrd_buf[t] |= *thrd_ptr[t]++ << (24 - Incnt);
        Incnt += 8;
      }  while (Incnt <= 24);
    }
    thrd_Incnt[t] = Incnt;
  }
  
}


/* return next n bits (right adjusted) */

unsigned int Thrd_Get_Bits(t,N)
int t,N;
{
  unsigned int Val;
  
  Val = Thrd_Show_Bits(t,N);
  Thrd_Flush_Buffer(t,N);

  return Val;
}
#endif
