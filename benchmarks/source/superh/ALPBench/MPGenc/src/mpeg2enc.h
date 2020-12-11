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


/* mpg2enc.h, defines and types                                             */

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

#define PICTURE_START_CODE 0x100L
#define SLICE_MIN_START    0x101L
#define SLICE_MAX_START    0x1AFL
#define USER_START_CODE    0x1B2L
#define SEQ_START_CODE     0x1B3L
#define EXT_START_CODE     0x1B5L
#define SEQ_END_CODE       0x1B7L
#define GOP_START_CODE     0x1B8L
#define ISO_END_CODE       0x1B9L
#define PACK_START_CODE    0x1BAL
#define SYSTEM_START_CODE  0x1BBL

/* picture coding type */
#define I_TYPE 1
#define P_TYPE 2
#define B_TYPE 3
#define D_TYPE 4

/* picture structure */
#define TOP_FIELD     1
#define BOTTOM_FIELD  2
#define FRAME_PICTURE 3

/* macroblock type */
#define MB_INTRA    1
#define MB_PATTERN  2
#define MB_BACKWARD 4
#define MB_FORWARD  8
#define MB_QUANT    16

/* motion_type */
#define MC_FIELD 1
#define MC_FRAME 2
#define MC_16X8  2
#define MC_DMV   3

/* mv_format */
#define MV_FIELD 0
#define MV_FRAME 1

/* chroma_format */
#define CHROMA420 1
#define CHROMA422 2
#define CHROMA444 3

/* extension start code IDs */

#define SEQ_ID       1
#define DISP_ID      2
#define QUANT_ID     3
#define SEQSCAL_ID   5
#define PANSCAN_ID   7
#define CODING_ID    8
#define SPATSCAL_ID  9
#define TEMPSCAL_ID 10

/* inputtype */
#define T_Y_U_V 0
#define T_YUV   1
#define T_PPM   2

/* macroblock information */
struct mbinfo {
  int mb_type; /* intra/forward/backward/interpolated */
  int motion_type; /* frame/field/16x8/dual_prime */
  int dct_type; /* field/frame DCT */
  int mquant; /* quantization parameter */
  int cbp; /* coded block pattern */
  int skipped; /* skipped macroblock */
  int MV[2][2][2]; /* motion vectors */
  int mv_field_sel[2][2]; /* motion vertical field select */
  int dmvector[2]; /* dual prime vectors */
  double act; /* activity measure */
  int var; /* for debugging */
};

/* motion data */
struct motion_data {
  int forw_hor_f_code,forw_vert_f_code; /* vector range */
  int sxf,syf; /* search range */
  int back_hor_f_code,back_vert_f_code;
  int sxb,syb;
};

#ifdef LTHREAD
/* struct ME_Args { */
/*   unsigned char *oldorg,*neworg,*oldref,*newref,*cur,*curref; */
/*   int sxf,syf,sxb,syb; */
/*   struct mbinfo *mbi; */
/*   int secondfield,ipflag; */
/* }; */

/* struct Pred_Args { */
/*   unsigned char **reff,**refb,**cur; */
/*   int secondfield; */
/*   struct mbinfo *mbi; */
/* }; */

/* struct DTE_Args { */
/*   unsigned char *pred,*cur; */
/*   struct mbinfo *mbi; */
/* }; */

/* struct Trfm_Args { */
/*   unsigned char **pred, **cur; */
/*   struct mbinfo *mbi; */
/*   short **blocks; */
/* }; */

/* struct IQunt_Args { */
/*   short **blk; */
/*   int dc_prec; */
/*   unsigned char *quant_mat; */
/*   int mquant; */
/* }; */

/* struct PPict_Args { */
/*   int prev_mquant; */
/*   unsigned char *frame; */
/* }; */

struct Data_Args {
  unsigned char *oldorg,*neworg,*oldref,*newref,*cur,*curref;
  int sxf,syf,sxb,syb;
  struct mbinfo *mbi;
  int secondfield,ipflag;
  
  unsigned char **reff, **refb, **pd_cur;
  unsigned char *pred,*dte_cur, **trfm_cur;
  short **blocks;
  int prev_mquant;

  unsigned char **itrfm_cur;
  
};

struct thread_data {
  struct ME_Args *me_ptr;
  struct Pred_Args *pd_ptr;
  struct DTE_Args *dte_ptr;
  struct Trfm_Args *trfm_ptr;
  struct IQunt_Args *iqt_ptr;
  struct Trfm_Args *itrfm_ptr;
  struct PPict_Args *pp_ptr;

  struct Data_Args *data_ptr;

  int s_height, e_height;
  int s_mbh, e_mbh, mode, id;
  int pp_smbh, pp_embh;
};

#endif
