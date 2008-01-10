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


/* systems.c, systems-specific routines                                 */

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

#include "config.h"
#include "global.h"

/* initialize buffer, call once before first getbits or showbits */

/* parse system layer, ignore everything we don't need */
void Next_Packet()
{
  unsigned int code;
  int l;

  for(;;)
  {
    code = Get_Long();

    /* remove system layer byte stuffing */
    while ((code & 0xffffff00) != 0x100)
      code = (code<<8) | Get_Byte();

    switch(code)
    {
    case PACK_START_CODE: /* pack header */
      /* skip pack header (system_clock_reference and mux_rate) */
      ld->Rdptr += 8;
      break;
    case VIDEO_ELEMENTARY_STREAM:   
      code = Get_Word();             /* packet_length */
      ld->Rdmax = ld->Rdptr + code;

      code = Get_Byte();

      if((code>>6)==0x02)
      {
        ld->Rdptr++;
        code=Get_Byte();  /* parse PES_header_data_length */
        ld->Rdptr+=code;    /* advance pointer by PES_header_data_length */
        printf("MPEG-2 PES packet\n");
        return;
      }
      else if(code==0xff)
      {
        /* parse MPEG-1 packet header */
        while((code=Get_Byte())== 0xFF);
      }
       
      /* stuffing bytes */
      if(code>=0x40)
      {
        if(code>=0x80)
        {
          fprintf(stderr,"Error in packet header\n");
          exit(1);
        }
        /* skip STD_buffer_scale */
        ld->Rdptr++;
        code = Get_Byte();
      }

      if(code>=0x30)
      {
        if(code>=0x40)
        {
          fprintf(stderr,"Error in packet header\n");
          exit(1);
        }
        /* skip presentation and decoding time stamps */
        ld->Rdptr += 9;
      }
      else if(code>=0x20)
      {
        /* skip presentation time stamps */
        ld->Rdptr += 4;
      }
      else if(code!=0x0f)
      {
        fprintf(stderr,"Error in packet header\n");
        exit(1);
      }
      return;
    case ISO_END_CODE: /* end */
      /* simulate a buffer full of sequence end codes */
      l = 0;
      while (l<2048)
      {
        ld->Rdbfr[l++] = SEQUENCE_END_CODE>>24;
        ld->Rdbfr[l++] = SEQUENCE_END_CODE>>16;
        ld->Rdbfr[l++] = SEQUENCE_END_CODE>>8;
        ld->Rdbfr[l++] = SEQUENCE_END_CODE&0xff;
      }
      ld->Rdptr = ld->Rdbfr;
      ld->Rdmax = ld->Rdbfr + 2048;
      return;
    default:
      if(code>=SYSTEM_START_CODE)
      {
        /* skip system headers and non-video packets*/
        code = Get_Word();
        ld->Rdptr += code;
      }
      else
      {
        fprintf(stderr,"Unexpected startcode %08x in system layer\n",code);
        exit(1);
      }
      break;
    }
  }
}



void Flush_Buffer32()
{
  int Incnt;

  ld->Bfr = 0;

  Incnt = ld->Incnt;
  Incnt -= 32;

  if (System_Stream_Flag && (ld->Rdptr >= ld->Rdmax-4))
  {
    while (Incnt <= 24)
    {
      if (ld->Rdptr >= ld->Rdmax)
        Next_Packet();
      ld->Bfr |= Get_Byte() << (24 - Incnt);
      Incnt += 8;
    }
  }
  else
  {
    while (Incnt <= 24)
    {
      if (ld->Rdptr >= ld->Rdbfr+2048)
        Fill_Buffer();
      ld->Bfr |= *ld->Rdptr++ << (24 - Incnt);
      Incnt += 8;
    }
  }
  ld->Incnt = Incnt;

#ifdef VERIFY 
  ld->Bitcnt += 32;
#endif /* VERIFY */
}


unsigned int Get_Bits32()
{
  unsigned int l;

  l = Show_Bits(32);
  Flush_Buffer32();

  return l;
}


int Get_Long()
{
  int i;

  i = Get_Word();
  return (i<<16) | Get_Word();
}


#ifdef THRD
void Thrd_Flush_Buffer32(int t)
{
  int Incnt;

  thrd_buf[t] = 0;

  Incnt = thrd_Incnt[t];
  Incnt -= 32;

  if (System_Stream_Flag && (ld->Rdptr >= ld->Rdmax-4))
  {
    if (System_Stream_Flag) { printf("System_Stream_Flag is 1\n"); exit(1); }
    while (Incnt <= 24)
    {
      if (ld->Rdptr >= ld->Rdmax)
        Next_Packet();
      ld->Bfr |= Get_Byte() << (24 - Incnt);
      Incnt += 8;
    }
  }
  else
  {
    while (Incnt <= 24)
    {
      thrd_buf[t] |= *thrd_ptr[t]++ << (24 - Incnt);
      Incnt += 8;
    }
  }
  thrd_Incnt[t] = Incnt;

}


unsigned int Thrd_Get_Bits32(int t)
{
  unsigned int l;

  l = Thrd_Show_Bits(t,32);
  Thrd_Flush_Buffer32(t);

  return l;
}


int Thrd_Get_Long(t)
int t;
{
  int i;

  i = Thrd_Get_Word(t);
  return (i<<16) | Thrd_Get_Word(t);
}

#endif
