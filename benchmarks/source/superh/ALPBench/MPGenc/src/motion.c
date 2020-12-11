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


/* motion.c, motion estimation                                              */

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

#define THRESHOLD 64

/* private prototypes */

static void frame_ME _ANSI_ARGS_((unsigned char *oldorg, unsigned char *neworg,
  unsigned char *oldref, unsigned char *newref, unsigned char *cur,
  int i, int j, int sxf, int syf, int sxb, int syb, struct mbinfo *mbi));

static void field_ME _ANSI_ARGS_((unsigned char *oldorg, unsigned char *neworg,
  unsigned char *oldref, unsigned char *newref, unsigned char *cur,
  unsigned char *curref, int i, int j, int sxf, int syf, int sxb, int syb,
  struct mbinfo *mbi, int secondfield, int ipflag));

static void frame_estimate _ANSI_ARGS_((unsigned char *org,
  unsigned char *ref, unsigned char *mb,
  int i, int j,
  int sx, int sy, int *iminp, int *jminp, int *imintp, int *jmintp,
  int *iminbp, int *jminbp, int *dframep, int *dfieldp,
  int *tselp, int *bselp, int imins[2][2], int jmins[2][2]));

static void field_estimate _ANSI_ARGS_((unsigned char *toporg,
  unsigned char *topref, unsigned char *botorg, unsigned char *botref,
  unsigned char *mb, int i, int j, int sx, int sy, int ipflag,
  int *iminp, int *jminp, int *imin8up, int *jmin8up, int *imin8lp,
  int *jmin8lp, int *dfieldp, int *d8p, int *selp, int *sel8up, int *sel8lp,
  int *iminsp, int *jminsp, int *dsp));

static void dpframe_estimate _ANSI_ARGS_((unsigned char *ref,
  unsigned char *mb, int i, int j, int iminf[2][2], int jminf[2][2],
  int *iminp, int *jminp, int *imindmvp, int *jmindmvp,
  int *dmcp, int *vmcp));

static void dpfield_estimate _ANSI_ARGS_((unsigned char *topref,
  unsigned char *botref, unsigned char *mb,
  int i, int j, int imins, int jmins, int *imindmvp, int *jmindmvp,
  int *dmcp, int *vmcp));

static int fullsearch _ANSI_ARGS_((unsigned char *org, unsigned char *ref,
  unsigned char *blk,
  int lx, int i0, int j0, int sx, int sy, int h, int xmax, int ymax,
  int *iminp, int *jminp));

static int dist1 _ANSI_ARGS_((unsigned char *blk1, unsigned char *blk2,
  int lx, int hx, int hy, int h, int distlim));

static int dist2 _ANSI_ARGS_((unsigned char *blk1, unsigned char *blk2,
  int lx, int hx, int hy, int h));

static int bdist1 _ANSI_ARGS_((unsigned char *pf, unsigned char *pb,
  unsigned char *p2, int lx, int hxf, int hyf, int hxb, int hyb, int h));

static int bdist2 _ANSI_ARGS_((unsigned char *pf, unsigned char *pb,
  unsigned char *p2, int lx, int hxf, int hyf, int hxb, int hyb, int h));

static int variance _ANSI_ARGS_((unsigned char *p, int lx));


/*
 * motion estimation for progressive and interlaced frame pictures
 *
 * oldorg: source frame for forward prediction (used for P and B frames)
 * neworg: source frame for backward prediction (B frames only)
 * oldref: reconstructed frame for forward prediction (P and B frames)
 * newref: reconstructed frame for backward prediction (B frames only)
 * cur:    current frame (the one for which the prediction is formed)
 * sxf,syf: forward search window (frame coordinates)
 * sxb,syb: backward search window (frame coordinates)
 * mbi:    pointer to macroblock info structure
 *
 * results:
 * mbi->
 *  mb_type: 0, MB_INTRA, MB_FORWARD, MB_BACKWARD, MB_FORWARD|MB_BACKWARD
 *  MV[][][]: motion vectors (frame format)
 *  mv_field_sel: top/bottom field (for field prediction)
 *  motion_type: MC_FRAME, MC_FIELD
 *
 * uses global vars: pict_type, frame_pred_dct
 */

void motion_estimation(oldorg,neworg,oldref,newref,cur,curref,
  sxf,syf,sxb,syb,mbi,secondfield,ipflag)
unsigned char *oldorg,*neworg,*oldref,*newref,*cur,*curref;
int sxf,syf,sxb,syb;
struct mbinfo *mbi;
int secondfield,ipflag;
{
  int i, j;

  /* loop through all macroblocks of the picture */
  for (j=0; j<height2; j+=16)
  {
    for (i=0; i<width; i+=16)
    {

      if (pict_struct==FRAME_PICTURE)
        frame_ME(oldorg,neworg,oldref,newref,cur,i,j,sxf,syf,sxb,syb,mbi);
      else
        field_ME(oldorg,neworg,oldref,newref,cur,curref,i,j,sxf,syf,sxb,syb,
		 mbi,secondfield,ipflag);
      mbi++;
    }
    

  }
  
}

#ifdef LTHREAD

void ptmotion_estimation(oldorg,neworg,oldref,newref,cur,curref,
  sxf,syf,sxb,syb,mbi,secondfield,ipflag,start_height, end_height)
unsigned char *oldorg,*neworg,*oldref,*newref,*cur,*curref;
int sxf,syf,sxb,syb;
struct mbinfo *mbi;
int secondfield,ipflag;
int start_height, end_height;
{
  int i, j;
  struct mbinfo *local_mbi;

  local_mbi = mbi + (start_height>>4) * (width>>4);

  /* loop through all macroblocks of the picture */
  for (j=start_height; j<end_height; j+=16)
  {
    for (i=0; i<width; i+=16)
    {

      if (pict_struct==FRAME_PICTURE)
        frame_ME(oldorg,neworg,oldref,newref,cur,i,j,sxf,syf,sxb,syb,local_mbi);
      else
        field_ME(oldorg,neworg,oldref,newref,cur,curref,i,j,sxf,syf,sxb,syb,
		 local_mbi,secondfield,ipflag);
      
      local_mbi++;
    }
  }

  if (!quiet)
    fputc('\n',stderr);


}


#endif

static void frame_ME(oldorg,neworg,oldref,newref,cur,i,j,sxf,syf,sxb,syb,mbi)
unsigned char *oldorg,*neworg,*oldref,*newref,*cur;
int i,j,sxf,syf,sxb,syb;
struct mbinfo *mbi;
{
  int imin,jmin,iminf,jminf,iminr,jminr;
  int imint,jmint,iminb,jminb;
  int imintf,jmintf,iminbf,jminbf;
  int imintr,jmintr,iminbr,jminbr;
  int var,v0;
  int dmc,dmcf,dmcr,dmci,vmc,vmcf,vmcr,vmci;
  int dmcfield,dmcfieldf,dmcfieldr,dmcfieldi;
  int tsel,bsel,tself,bself,tselr,bselr;
  unsigned char *mb;
  int imins[2][2],jmins[2][2];
  int imindp,jmindp,imindmv,jmindmv,dmc_dp,vmc_dp;

  mb = cur + i + width*j;

  var = variance(mb,width);

  if (pict_type==I_TYPE)
    mbi->mb_type = MB_INTRA;
  else if (pict_type==P_TYPE)
  {
    if (frame_pred_dct)
    {
      dmc = fullsearch(oldorg,oldref,mb,
                       width,i,j,sxf,syf,16,width,height,&imin,&jmin);
      vmc = dist2(oldref+(imin>>1)+width*(jmin>>1),mb,
                  width,imin&1,jmin&1,16);
      mbi->motion_type = MC_FRAME;
    }
    else
    {
      frame_estimate(oldorg,oldref,mb,i,j,sxf,syf,
        &imin,&jmin,&imint,&jmint,&iminb,&jminb,
        &dmc,&dmcfield,&tsel,&bsel,imins,jmins);

      if (M==1)
        dpframe_estimate(oldref,mb,i,j>>1,imins,jmins,
          &imindp,&jmindp,&imindmv,&jmindmv,&dmc_dp,&vmc_dp);

      /* select between dual prime, frame and field prediction */
      if (M==1 && dmc_dp<dmc && dmc_dp<dmcfield)
      {
        mbi->motion_type = MC_DMV;
        dmc = dmc_dp;
        vmc = vmc_dp;
      }
      else if (dmc<=dmcfield)
      {
        mbi->motion_type = MC_FRAME;
        vmc = dist2(oldref+(imin>>1)+width*(jmin>>1),mb,
                    width,imin&1,jmin&1,16);
      }
      else
      {
        mbi->motion_type = MC_FIELD;
        dmc = dmcfield;
        vmc = dist2(oldref+(tsel?width:0)+(imint>>1)+(width<<1)*(jmint>>1),
                    mb,width<<1,imint&1,jmint&1,8);
        vmc+= dist2(oldref+(bsel?width:0)+(iminb>>1)+(width<<1)*(jminb>>1),
                    mb+width,width<<1,iminb&1,jminb&1,8);
      }
    }

    /* select between intra or non-intra coding:
     *
     * selection is based on intra block variance (var) vs.
     * prediction error variance (vmc)
     *
     * blocks with small prediction error are always coded non-intra
     * even if variance is smaller (is this reasonable?)
     */
    if (vmc>var && vmc>=9*256)
      mbi->mb_type = MB_INTRA;
    else
    {
      /* select between MC / No-MC
       *
       * use No-MC if var(No-MC) <= 1.25*var(MC)
       * (i.e slightly biased towards No-MC)
       *
       * blocks with small prediction error are always coded as No-MC
       * (requires no motion vectors, allows skipping)
       */
      v0 = dist2(oldref+i+width*j,mb,width,0,0,16);
      if (4*v0>5*vmc && v0>=9*256)
      {
        /* use MC */
        var = vmc;
        mbi->mb_type = MB_FORWARD;
        if (mbi->motion_type==MC_FRAME)
        {
          mbi->MV[0][0][0] = imin - (i<<1);
          mbi->MV[0][0][1] = jmin - (j<<1);
        }
        else if (mbi->motion_type==MC_DMV)
        {
          /* these are FRAME vectors */
          /* same parity vector */
          mbi->MV[0][0][0] = imindp - (i<<1);
          mbi->MV[0][0][1] = (jmindp<<1) - (j<<1);

          /* opposite parity vector */
          mbi->dmvector[0] = imindmv;
          mbi->dmvector[1] = jmindmv;
        }
        else
        {
          /* these are FRAME vectors */
          mbi->MV[0][0][0] = imint - (i<<1);
          mbi->MV[0][0][1] = (jmint<<1) - (j<<1);
          mbi->MV[1][0][0] = iminb - (i<<1);
          mbi->MV[1][0][1] = (jminb<<1) - (j<<1);
          mbi->mv_field_sel[0][0] = tsel;
          mbi->mv_field_sel[1][0] = bsel;
        }
      }
      else
      {
        /* No-MC */
        var = v0;
        mbi->mb_type = 0;
        mbi->motion_type = MC_FRAME;
        mbi->MV[0][0][0] = 0;
        mbi->MV[0][0][1] = 0;
      }
    }
  }
  else /* if (pict_type==B_TYPE) */
  {
    if (frame_pred_dct)
    {
      /* forward */
      dmcf = fullsearch(oldorg,oldref,mb,
                        width,i,j,sxf,syf,16,width,height,&iminf,&jminf);
      vmcf = dist2(oldref+(iminf>>1)+width*(jminf>>1),mb,
                   width,iminf&1,jminf&1,16);

      /* backward */
      dmcr = fullsearch(neworg,newref,mb,
                        width,i,j,sxb,syb,16,width,height,&iminr,&jminr);
      vmcr = dist2(newref+(iminr>>1)+width*(jminr>>1),mb,
                   width,iminr&1,jminr&1,16);

      /* interpolated (bidirectional) */
      vmci = bdist2(oldref+(iminf>>1)+width*(jminf>>1),
                    newref+(iminr>>1)+width*(jminr>>1),
                    mb,width,iminf&1,jminf&1,iminr&1,jminr&1,16);

      /* decisions */

      /* select between forward/backward/interpolated prediction:
       * use the one with smallest mean sqaured prediction error
       */
      if (vmcf<=vmcr && vmcf<=vmci)
      {
        vmc = vmcf;
        mbi->mb_type = MB_FORWARD;
      }
      else if (vmcr<=vmci)
      {
        vmc = vmcr;
        mbi->mb_type = MB_BACKWARD;
      }
      else
      {
        vmc = vmci;
        mbi->mb_type = MB_FORWARD|MB_BACKWARD;
      }

      mbi->motion_type = MC_FRAME;
    }
    else
    {
      /* forward prediction */
      frame_estimate(oldorg,oldref,mb,i,j,sxf,syf,
        &iminf,&jminf,&imintf,&jmintf,&iminbf,&jminbf,
        &dmcf,&dmcfieldf,&tself,&bself,imins,jmins);

      /* backward prediction */
      frame_estimate(neworg,newref,mb,i,j,sxb,syb,
        &iminr,&jminr,&imintr,&jmintr,&iminbr,&jminbr,
        &dmcr,&dmcfieldr,&tselr,&bselr,imins,jmins);

      /* calculate interpolated distance */
      /* frame */
      dmci = bdist1(oldref+(iminf>>1)+width*(jminf>>1),
                    newref+(iminr>>1)+width*(jminr>>1),
                    mb,width,iminf&1,jminf&1,iminr&1,jminr&1,16);

      /* top field */
      dmcfieldi = bdist1(
                    oldref+(imintf>>1)+(tself?width:0)+(width<<1)*(jmintf>>1),
                    newref+(imintr>>1)+(tselr?width:0)+(width<<1)*(jmintr>>1),
                    mb,width<<1,imintf&1,jmintf&1,imintr&1,jmintr&1,8);

      /* bottom field */
      dmcfieldi+= bdist1(
                    oldref+(iminbf>>1)+(bself?width:0)+(width<<1)*(jminbf>>1),
                    newref+(iminbr>>1)+(bselr?width:0)+(width<<1)*(jminbr>>1),
                    mb+width,width<<1,iminbf&1,jminbf&1,iminbr&1,jminbr&1,8);

      /* select prediction type of minimum distance from the
       * six candidates (field/frame * forward/backward/interpolated)
       */
      if (dmci<dmcfieldi && dmci<dmcf && dmci<dmcfieldf
          && dmci<dmcr && dmci<dmcfieldr)
      {
        /* frame, interpolated */
        mbi->mb_type = MB_FORWARD|MB_BACKWARD;
        mbi->motion_type = MC_FRAME;
        vmc = bdist2(oldref+(iminf>>1)+width*(jminf>>1),
                     newref+(iminr>>1)+width*(jminr>>1),
                     mb,width,iminf&1,jminf&1,iminr&1,jminr&1,16);
      }
      else if (dmcfieldi<dmcf && dmcfieldi<dmcfieldf
               && dmcfieldi<dmcr && dmcfieldi<dmcfieldr)
      {
        /* field, interpolated */
        mbi->mb_type = MB_FORWARD|MB_BACKWARD;
        mbi->motion_type = MC_FIELD;
        vmc = bdist2(oldref+(imintf>>1)+(tself?width:0)+(width<<1)*(jmintf>>1),
                     newref+(imintr>>1)+(tselr?width:0)+(width<<1)*(jmintr>>1),
                     mb,width<<1,imintf&1,jmintf&1,imintr&1,jmintr&1,8);
        vmc+= bdist2(oldref+(iminbf>>1)+(bself?width:0)+(width<<1)*(jminbf>>1),
                     newref+(iminbr>>1)+(bselr?width:0)+(width<<1)*(jminbr>>1),
                     mb+width,width<<1,iminbf&1,jminbf&1,iminbr&1,jminbr&1,8);
      }
      else if (dmcf<dmcfieldf && dmcf<dmcr && dmcf<dmcfieldr)
      {
        /* frame, forward */
        mbi->mb_type = MB_FORWARD;
        mbi->motion_type = MC_FRAME;
        vmc = dist2(oldref+(iminf>>1)+width*(jminf>>1),mb,
                    width,iminf&1,jminf&1,16);
      }
      else if (dmcfieldf<dmcr && dmcfieldf<dmcfieldr)
      {
        /* field, forward */
        mbi->mb_type = MB_FORWARD;
        mbi->motion_type = MC_FIELD;
        vmc = dist2(oldref+(tself?width:0)+(imintf>>1)+(width<<1)*(jmintf>>1),
                    mb,width<<1,imintf&1,jmintf&1,8);
        vmc+= dist2(oldref+(bself?width:0)+(iminbf>>1)+(width<<1)*(jminbf>>1),
                    mb+width,width<<1,iminbf&1,jminbf&1,8);
      }
      else if (dmcr<dmcfieldr)
      {
        /* frame, backward */
        mbi->mb_type = MB_BACKWARD;
        mbi->motion_type = MC_FRAME;
        vmc = dist2(newref+(iminr>>1)+width*(jminr>>1),mb,
                    width,iminr&1,jminr&1,16);
      }
      else
      {
        /* field, backward */
        mbi->mb_type = MB_BACKWARD;
        mbi->motion_type = MC_FIELD;
        vmc = dist2(newref+(tselr?width:0)+(imintr>>1)+(width<<1)*(jmintr>>1),
                    mb,width<<1,imintr&1,jmintr&1,8);
        vmc+= dist2(newref+(bselr?width:0)+(iminbr>>1)+(width<<1)*(jminbr>>1),
                    mb+width,width<<1,iminbr&1,jminbr&1,8);
      }
    }

    /* select between intra or non-intra coding:
     *
     * selection is based on intra block variance (var) vs.
     * prediction error variance (vmc)
     *
     * blocks with small prediction error are always coded non-intra
     * even if variance is smaller (is this reasonable?)
     */
    if (vmc>var && vmc>=9*256)
      mbi->mb_type = MB_INTRA;
    else
    {
      var = vmc;
      if (mbi->motion_type==MC_FRAME)
      {
        /* forward */
        mbi->MV[0][0][0] = iminf - (i<<1);
        mbi->MV[0][0][1] = jminf - (j<<1);
        /* backward */
        mbi->MV[0][1][0] = iminr - (i<<1);
        mbi->MV[0][1][1] = jminr - (j<<1);
      }
      else
      {
        /* these are FRAME vectors */
        /* forward */
        mbi->MV[0][0][0] = imintf - (i<<1);
        mbi->MV[0][0][1] = (jmintf<<1) - (j<<1);
        mbi->MV[1][0][0] = iminbf - (i<<1);
        mbi->MV[1][0][1] = (jminbf<<1) - (j<<1);
        mbi->mv_field_sel[0][0] = tself;
        mbi->mv_field_sel[1][0] = bself;
        /* backward */
        mbi->MV[0][1][0] = imintr - (i<<1);
        mbi->MV[0][1][1] = (jmintr<<1) - (j<<1);
        mbi->MV[1][1][0] = iminbr - (i<<1);
        mbi->MV[1][1][1] = (jminbr<<1) - (j<<1);
        mbi->mv_field_sel[0][1] = tselr;
        mbi->mv_field_sel[1][1] = bselr;
      }
    }
  }

  mbi->var = var;
}

/*
 * motion estimation for field pictures
 *
 * oldorg: original frame for forward prediction (P and B frames)
 * neworg: original frame for backward prediction (B frames only)
 * oldref: reconstructed frame for forward prediction (P and B frames)
 * newref: reconstructed frame for backward prediction (B frames only)
 * cur:    current original frame (the one for which the prediction is formed)
 * curref: current reconstructed frame (to predict second field from first)
 * sxf,syf: forward search window (frame coordinates)
 * sxb,syb: backward search window (frame coordinates)
 * mbi:    pointer to macroblock info structure
 * secondfield: indicates second field of a frame (in P fields this means
 *              that reference field of opposite parity is in curref instead
 *              of oldref)
 * ipflag: indicates a P type field which is the second field of a frame
 *         in which the first field is I type (this restricts predictions
 *         to be based only on the opposite parity (=I) field)
 *
 * results:
 * mbi->
 *  mb_type: 0, MB_INTRA, MB_FORWARD, MB_BACKWARD, MB_FORWARD|MB_BACKWARD
 *  MV[][][]: motion vectors (field format)
 *  mv_field_sel: top/bottom field
 *  motion_type: MC_FIELD, MC_16X8
 *
 * uses global vars: pict_type, pict_struct
 */
static void field_ME(oldorg,neworg,oldref,newref,cur,curref,i,j,
  sxf,syf,sxb,syb,mbi,secondfield,ipflag)
unsigned char *oldorg,*neworg,*oldref,*newref,*cur,*curref;
int i,j,sxf,syf,sxb,syb;
struct mbinfo *mbi;
int secondfield,ipflag;
{
  int w2;
  unsigned char *mb, *toporg, *topref, *botorg, *botref;
  int var,vmc,v0,dmc,dmcfieldi,dmc8i;
  int imin,jmin,imin8u,jmin8u,imin8l,jmin8l,dmcfield,dmc8,sel,sel8u,sel8l;
  int iminf,jminf,imin8uf,jmin8uf,imin8lf,jmin8lf,dmcfieldf,dmc8f,self,sel8uf,sel8lf;
  int iminr,jminr,imin8ur,jmin8ur,imin8lr,jmin8lr,dmcfieldr,dmc8r,selr,sel8ur,sel8lr;
  int imins,jmins,ds,imindmv,jmindmv,vmc_dp,dmc_dp;

  w2 = width<<1;

  mb = cur + i + w2*j;
  if (pict_struct==BOTTOM_FIELD)
    mb += width;

  var = variance(mb,w2);

  if (pict_type==I_TYPE)
    mbi->mb_type = MB_INTRA;
  else if (pict_type==P_TYPE)
  {
    toporg = oldorg;
    topref = oldref;
    botorg = oldorg + width;
    botref = oldref + width;

    if (secondfield)
    {
      /* opposite parity field is in same frame */
      if (pict_struct==TOP_FIELD)
      {
        /* current is top field */
        botorg = cur + width;
        botref = curref + width;
      }
      else
      {
        /* current is bottom field */
        toporg = cur;
        topref = curref;
      }
    }

    field_estimate(toporg,topref,botorg,botref,mb,i,j,sxf,syf,ipflag,
                   &imin,&jmin,&imin8u,&jmin8u,&imin8l,&jmin8l,
                   &dmcfield,&dmc8,&sel,&sel8u,&sel8l,&imins,&jmins,&ds);

    if (M==1 && !ipflag)  /* generic condition which permits Dual Prime */
      dpfield_estimate(topref,botref,mb,i,j,imins,jmins,&imindmv,&jmindmv,
        &dmc_dp,&vmc_dp);

    /* select between dual prime, field and 16x8 prediction */
    if (M==1 && !ipflag && dmc_dp<dmc8 && dmc_dp<dmcfield)
    {
      /* Dual Prime prediction */
      mbi->motion_type = MC_DMV;
      dmc = dmc_dp;     /* L1 metric */
      vmc = vmc_dp;     /* we already calculated L2 error for Dual */

    }
    else if (dmc8<dmcfield)
    {
      /* 16x8 prediction */
      mbi->motion_type = MC_16X8;
      /* upper half block */
      vmc = dist2((sel8u?botref:topref) + (imin8u>>1) + w2*(jmin8u>>1),
                  mb,w2,imin8u&1,jmin8u&1,8);
      /* lower half block */
      vmc+= dist2((sel8l?botref:topref) + (imin8l>>1) + w2*(jmin8l>>1),
                  mb+8*w2,w2,imin8l&1,jmin8l&1,8);
    }
    else
    {
      /* field prediction */
      mbi->motion_type = MC_FIELD;
      vmc = dist2((sel?botref:topref) + (imin>>1) + w2*(jmin>>1),
                  mb,w2,imin&1,jmin&1,16);
    }

    /* select between intra and non-intra coding */
    if (vmc>var && vmc>=9*256)
      mbi->mb_type = MB_INTRA;
    else
    {
      /* zero MV field prediction from same parity ref. field
       * (not allowed if ipflag is set)
       */
      if (!ipflag)
        v0 = dist2(((pict_struct==BOTTOM_FIELD)?botref:topref) + i + w2*j,
                   mb,w2,0,0,16);
      if (ipflag || (4*v0>5*vmc && v0>=9*256))
      {
        var = vmc;
        mbi->mb_type = MB_FORWARD;
        if (mbi->motion_type==MC_FIELD)
        {
          mbi->MV[0][0][0] = imin - (i<<1);
          mbi->MV[0][0][1] = jmin - (j<<1);
          mbi->mv_field_sel[0][0] = sel;
        }
        else if (mbi->motion_type==MC_DMV)
        {
          /* same parity vector */
          mbi->MV[0][0][0] = imins - (i<<1);
          mbi->MV[0][0][1] = jmins - (j<<1);

          /* opposite parity vector */
          mbi->dmvector[0] = imindmv;
          mbi->dmvector[1] = jmindmv;
        }
        else
        {
          mbi->MV[0][0][0] = imin8u - (i<<1);
          mbi->MV[0][0][1] = jmin8u - (j<<1);
          mbi->MV[1][0][0] = imin8l - (i<<1);
          mbi->MV[1][0][1] = jmin8l - ((j+8)<<1);
          mbi->mv_field_sel[0][0] = sel8u;
          mbi->mv_field_sel[1][0] = sel8l;
        }
      }
      else
      {
        /* No MC */
        var = v0;
        mbi->mb_type = 0;
        mbi->motion_type = MC_FIELD;
        mbi->MV[0][0][0] = 0;
        mbi->MV[0][0][1] = 0;
        mbi->mv_field_sel[0][0] = (pict_struct==BOTTOM_FIELD);
      }
    }
  }
  else /* if (pict_type==B_TYPE) */
  {
    /* forward prediction */
    field_estimate(oldorg,oldref,oldorg+width,oldref+width,mb,
                   i,j,sxf,syf,0,
                   &iminf,&jminf,&imin8uf,&jmin8uf,&imin8lf,&jmin8lf,
                   &dmcfieldf,&dmc8f,&self,&sel8uf,&sel8lf,&imins,&jmins,&ds);

    /* backward prediction */
    field_estimate(neworg,newref,neworg+width,newref+width,mb,
                   i,j,sxb,syb,0,
                   &iminr,&jminr,&imin8ur,&jmin8ur,&imin8lr,&jmin8lr,
                   &dmcfieldr,&dmc8r,&selr,&sel8ur,&sel8lr,&imins,&jmins,&ds);

    /* calculate distances for bidirectional prediction */
    /* field */
    dmcfieldi = bdist1(oldref + (self?width:0) + (iminf>>1) + w2*(jminf>>1),
                       newref + (selr?width:0) + (iminr>>1) + w2*(jminr>>1),
                       mb,w2,iminf&1,jminf&1,iminr&1,jminr&1,16);

    /* 16x8 upper half block */
    dmc8i = bdist1(oldref + (sel8uf?width:0) + (imin8uf>>1) + w2*(jmin8uf>>1),
                   newref + (sel8ur?width:0) + (imin8ur>>1) + w2*(jmin8ur>>1),
                   mb,w2,imin8uf&1,jmin8uf&1,imin8ur&1,jmin8ur&1,8);

    /* 16x8 lower half block */
    dmc8i+= bdist1(oldref + (sel8lf?width:0) + (imin8lf>>1) + w2*(jmin8lf>>1),
                   newref + (sel8lr?width:0) + (imin8lr>>1) + w2*(jmin8lr>>1),
                   mb+8*w2,w2,imin8lf&1,jmin8lf&1,imin8lr&1,jmin8lr&1,8);

    /* select prediction type of minimum distance */
    if (dmcfieldi<dmc8i && dmcfieldi<dmcfieldf && dmcfieldi<dmc8f
        && dmcfieldi<dmcfieldr && dmcfieldi<dmc8r)
    {
      /* field, interpolated */
      mbi->mb_type = MB_FORWARD|MB_BACKWARD;
      mbi->motion_type = MC_FIELD;
      vmc = bdist2(oldref + (self?width:0) + (iminf>>1) + w2*(jminf>>1),
                   newref + (selr?width:0) + (iminr>>1) + w2*(jminr>>1),
                   mb,w2,iminf&1,jminf&1,iminr&1,jminr&1,16);
    }
    else if (dmc8i<dmcfieldf && dmc8i<dmc8f
             && dmc8i<dmcfieldr && dmc8i<dmc8r)
    {
      /* 16x8, interpolated */
      mbi->mb_type = MB_FORWARD|MB_BACKWARD;
      mbi->motion_type = MC_16X8;

      /* upper half block */
      vmc = bdist2(oldref + (sel8uf?width:0) + (imin8uf>>1) + w2*(jmin8uf>>1),
                   newref + (sel8ur?width:0) + (imin8ur>>1) + w2*(jmin8ur>>1),
                   mb,w2,imin8uf&1,jmin8uf&1,imin8ur&1,jmin8ur&1,8);

      /* lower half block */
      vmc+= bdist2(oldref + (sel8lf?width:0) + (imin8lf>>1) + w2*(jmin8lf>>1),
                   newref + (sel8lr?width:0) + (imin8lr>>1) + w2*(jmin8lr>>1),
                   mb+8*w2,w2,imin8lf&1,jmin8lf&1,imin8lr&1,jmin8lr&1,8);
    }
    else if (dmcfieldf<dmc8f && dmcfieldf<dmcfieldr && dmcfieldf<dmc8r)
    {
      /* field, forward */
      mbi->mb_type = MB_FORWARD;
      mbi->motion_type = MC_FIELD;
      vmc = dist2(oldref + (self?width:0) + (iminf>>1) + w2*(jminf>>1),
                  mb,w2,iminf&1,jminf&1,16);
    }
    else if (dmc8f<dmcfieldr && dmc8f<dmc8r)
    {
      /* 16x8, forward */
      mbi->mb_type = MB_FORWARD;
      mbi->motion_type = MC_16X8;

      /* upper half block */
      vmc = dist2(oldref + (sel8uf?width:0) + (imin8uf>>1) + w2*(jmin8uf>>1),
                  mb,w2,imin8uf&1,jmin8uf&1,8);

      /* lower half block */
      vmc+= dist2(oldref + (sel8lf?width:0) + (imin8lf>>1) + w2*(jmin8lf>>1),
                  mb+8*w2,w2,imin8lf&1,jmin8lf&1,8);
    }
    else if (dmcfieldr<dmc8r)
    {
      /* field, backward */
      mbi->mb_type = MB_BACKWARD;
      mbi->motion_type = MC_FIELD;
      vmc = dist2(newref + (selr?width:0) + (iminr>>1) + w2*(jminr>>1),
                  mb,w2,iminr&1,jminr&1,16);
    }
    else
    {
      /* 16x8, backward */
      mbi->mb_type = MB_BACKWARD;
      mbi->motion_type = MC_16X8;

      /* upper half block */
      vmc = dist2(newref + (sel8ur?width:0) + (imin8ur>>1) + w2*(jmin8ur>>1),
                  mb,w2,imin8ur&1,jmin8ur&1,8);

      /* lower half block */
      vmc+= dist2(newref + (sel8lr?width:0) + (imin8lr>>1) + w2*(jmin8lr>>1),
                  mb+8*w2,w2,imin8lr&1,jmin8lr&1,8);
    }

    /* select between intra and non-intra coding */
    if (vmc>var && vmc>=9*256)
      mbi->mb_type = MB_INTRA;
    else
    {
      var = vmc;
      if (mbi->motion_type==MC_FIELD)
      {
        /* forward */
        mbi->MV[0][0][0] = iminf - (i<<1);
        mbi->MV[0][0][1] = jminf - (j<<1);
        mbi->mv_field_sel[0][0] = self;
        /* backward */
        mbi->MV[0][1][0] = iminr - (i<<1);
        mbi->MV[0][1][1] = jminr - (j<<1);
        mbi->mv_field_sel[0][1] = selr;
      }
      else /* MC_16X8 */
      {
        /* forward */
        mbi->MV[0][0][0] = imin8uf - (i<<1);
        mbi->MV[0][0][1] = jmin8uf - (j<<1);
        mbi->mv_field_sel[0][0] = sel8uf;
        mbi->MV[1][0][0] = imin8lf - (i<<1);
        mbi->MV[1][0][1] = jmin8lf - ((j+8)<<1);
        mbi->mv_field_sel[1][0] = sel8lf;
        /* backward */
        mbi->MV[0][1][0] = imin8ur - (i<<1);
        mbi->MV[0][1][1] = jmin8ur - (j<<1);
        mbi->mv_field_sel[0][1] = sel8ur;
        mbi->MV[1][1][0] = imin8lr - (i<<1);
        mbi->MV[1][1][1] = jmin8lr - ((j+8)<<1);
        mbi->mv_field_sel[1][1] = sel8lr;
      }
    }
  }

  mbi->var = var;
}

/*
 * frame picture motion estimation
 *
 * org: top left pel of source reference frame
 * ref: top left pel of reconstructed reference frame
 * mb:  macroblock to be matched
 * i,j: location of mb relative to ref (=center of search window)
 * sx,sy: half widths of search window
 * iminp,jminp,dframep: location and value of best frame prediction
 * imintp,jmintp,tselp: location of best field pred. for top field of mb
 * iminbp,jminbp,bselp: location of best field pred. for bottom field of mb
 * dfieldp: value of field prediction
 */
static void frame_estimate(org,ref,mb,i,j,sx,sy,
  iminp,jminp,imintp,jmintp,iminbp,jminbp,dframep,dfieldp,tselp,bselp,
  imins,jmins)
unsigned char *org,*ref,*mb;
int i,j,sx,sy;
int *iminp,*jminp;
int *imintp,*jmintp,*iminbp,*jminbp;
int *dframep,*dfieldp;
int *tselp,*bselp;
int imins[2][2],jmins[2][2];
{
  int dt,db,dmint,dminb;
  int imint,iminb,jmint,jminb;

  /* frame prediction */
  *dframep = fullsearch(org,ref,mb,width,i,j,sx,sy,16,width,height,
                        iminp,jminp);

  /* predict top field from top field */
  dt = fullsearch(org,ref,mb,width<<1,i,j>>1,sx,sy>>1,8,width,height>>1,
                  &imint,&jmint);

  /* predict top field from bottom field */
  db = fullsearch(org+width,ref+width,mb,width<<1,i,j>>1,sx,sy>>1,8,width,height>>1,
                  &iminb,&jminb);

  imins[0][0] = imint;
  jmins[0][0] = jmint;
  imins[1][0] = iminb;
  jmins[1][0] = jminb;

  /* select prediction for top field */
  if (dt<=db)
  {
    dmint=dt; *imintp=imint; *jmintp=jmint; *tselp=0;
  }
  else
  {
    dmint=db; *imintp=iminb; *jmintp=jminb; *tselp=1;
  }

  /* predict bottom field from top field */
  dt = fullsearch(org,ref,mb+width,width<<1,i,j>>1,sx,sy>>1,8,width,height>>1,
                  &imint,&jmint);

  /* predict bottom field from bottom field */
  db = fullsearch(org+width,ref+width,mb+width,width<<1,i,j>>1,sx,sy>>1,8,width,height>>1,
                  &iminb,&jminb);

  imins[0][1] = imint;
  jmins[0][1] = jmint;
  imins[1][1] = iminb;
  jmins[1][1] = jminb;

  /* select prediction for bottom field */
  if (db<=dt)
  {
    dminb=db; *iminbp=iminb; *jminbp=jminb; *bselp=1;
  }
  else
  {
    dminb=dt; *iminbp=imint; *jminbp=jmint; *bselp=0;
  }

  *dfieldp=dmint+dminb;
}

/*
 * field picture motion estimation subroutine
 *
 * toporg: address of original top reference field
 * topref: address of reconstructed top reference field
 * botorg: address of original bottom reference field
 * botref: address of reconstructed bottom reference field
 * mb:  macroblock to be matched
 * i,j: location of mb (=center of search window)
 * sx,sy: half width/height of search window
 *
 * iminp,jminp,selp,dfieldp: location and distance of best field prediction
 * imin8up,jmin8up,sel8up: location of best 16x8 pred. for upper half of mb
 * imin8lp,jmin8lp,sel8lp: location of best 16x8 pred. for lower half of mb
 * d8p: distance of best 16x8 prediction
 * iminsp,jminsp,dsp: location and distance of best same parity field
 *                    prediction (needed for dual prime, only valid if
 *                    ipflag==0)
 */
static void field_estimate(toporg,topref,botorg,botref,mb,i,j,sx,sy,ipflag,
  iminp,jminp,imin8up,jmin8up,imin8lp,jmin8lp,dfieldp,d8p,selp,sel8up,sel8lp,
  iminsp,jminsp,dsp)
unsigned char *toporg, *topref, *botorg, *botref, *mb;
int i,j,sx,sy;
int ipflag;
int *iminp, *jminp;
int *imin8up, *jmin8up, *imin8lp, *jmin8lp;
int *dfieldp,*d8p;
int *selp, *sel8up, *sel8lp;
int *iminsp, *jminsp, *dsp;
{
  int dt, db, imint, jmint, iminb, jminb, notop, nobot;

  /* if ipflag is set, predict from field of opposite parity only */
  notop = ipflag && (pict_struct==TOP_FIELD);
  nobot = ipflag && (pict_struct==BOTTOM_FIELD);

  /* field prediction */

  /* predict current field from top field */
  if (notop)
    dt = 65536; /* infinity */
  else
    dt = fullsearch(toporg,topref,mb,width<<1,
                    i,j,sx,sy>>1,16,width,height>>1,
                    &imint,&jmint);

  /* predict current field from bottom field */
  if (nobot)
    db = 65536; /* infinity */
  else
    db = fullsearch(botorg,botref,mb,width<<1,
                    i,j,sx,sy>>1,16,width,height>>1,
                    &iminb,&jminb);

  /* same parity prediction (only valid if ipflag==0) */
  if (pict_struct==TOP_FIELD)
  {
    *iminsp = imint; *jminsp = jmint; *dsp = dt;
  }
  else
  {
    *iminsp = iminb; *jminsp = jminb; *dsp = db;
  }

  /* select field prediction */
  if (dt<=db)
  {
    *dfieldp = dt; *iminp = imint; *jminp = jmint; *selp = 0;
  }
  else
  {
    *dfieldp = db; *iminp = iminb; *jminp = jminb; *selp = 1;
  }


  /* 16x8 motion compensation */

  /* predict upper half field from top field */
  if (notop)
    dt = 65536;
  else
    dt = fullsearch(toporg,topref,mb,width<<1,
                    i,j,sx,sy>>1,8,width,height>>1,
                    &imint,&jmint);

  /* predict upper half field from bottom field */
  if (nobot)
    db = 65536;
  else
    db = fullsearch(botorg,botref,mb,width<<1,
                    i,j,sx,sy>>1,8,width,height>>1,
                    &iminb,&jminb);

  /* select prediction for upper half field */
  if (dt<=db)
  {
    *d8p = dt; *imin8up = imint; *jmin8up = jmint; *sel8up = 0;
  }
  else
  {
    *d8p = db; *imin8up = iminb; *jmin8up = jminb; *sel8up = 1;
  }

  /* predict lower half field from top field */
  if (notop)
    dt = 65536;
  else
    dt = fullsearch(toporg,topref,mb+(width<<4),width<<1,
                    i,j+8,sx,sy>>1,8,width,height>>1,
                    &imint,&jmint);

  /* predict lower half field from bottom field */
  if (nobot)
    db = 65536;
  else
    db = fullsearch(botorg,botref,mb+(width<<4),width<<1,
                    i,j+8,sx,sy>>1,8,width,height>>1,
                    &iminb,&jminb);

  /* select prediction for lower half field */
  if (dt<=db)
  {
    *d8p += dt; *imin8lp = imint; *jmin8lp = jmint; *sel8lp = 0;
  }
  else
  {
    *d8p += db; *imin8lp = iminb; *jmin8lp = jminb; *sel8lp = 1;
  }
}

static void dpframe_estimate(ref,mb,i,j,iminf,jminf,
  iminp,jminp,imindmvp, jmindmvp, dmcp, vmcp)
unsigned char *ref, *mb;
int i,j;
int iminf[2][2], jminf[2][2];
int *iminp, *jminp;
int *imindmvp, *jmindmvp;
int *dmcp,*vmcp;
{
  int pref,ppred,delta_x,delta_y;
  int is,js,it,jt,ib,jb,it0,jt0,ib0,jb0;
  int imins,jmins,imint,jmint,iminb,jminb,imindmv,jmindmv;
  int vmc,local_dist;

  /* Calculate Dual Prime distortions for 9 delta candidates
   * for each of the four minimum field vectors
   * Note: only for P pictures!
   */

  /* initialize minimum dual prime distortion to large value */
  vmc = 1 << 30;

  for (pref=0; pref<2; pref++)
  {
    for (ppred=0; ppred<2; ppred++)
    {
      /* convert Cartesian absolute to relative motion vector
       * values (wrt current macroblock address (i,j)
       */
      is = iminf[pref][ppred] - (i<<1);
      js = jminf[pref][ppred] - (j<<1);

      if (pref!=ppred)
      {
        /* vertical field shift adjustment */
        if (ppred==0)
          js++;
        else
          js--;

        /* mvxs and mvys scaling*/
        is<<=1;
        js<<=1;
        if (topfirst == ppred)
        {
          /* second field: scale by 1/3 */
          is = (is>=0) ? (is+1)/3 : -((-is+1)/3);
          js = (js>=0) ? (js+1)/3 : -((-js+1)/3);
        }
        else
          continue;
      }

      /* vector for prediction from field of opposite 'parity' */
      if (topfirst)
      {
        /* vector for prediction of top field from bottom field */
        it0 = ((is+(is>0))>>1);
        jt0 = ((js+(js>0))>>1) - 1;

        /* vector for prediction of bottom field from top field */
        ib0 = ((3*is+(is>0))>>1);
        jb0 = ((3*js+(js>0))>>1) + 1;
      }
      else
      {
        /* vector for prediction of top field from bottom field */
        it0 = ((3*is+(is>0))>>1);
        jt0 = ((3*js+(js>0))>>1) - 1;

        /* vector for prediction of bottom field from top field */
        ib0 = ((is+(is>0))>>1);
        jb0 = ((js+(js>0))>>1) + 1;
      }

      /* convert back to absolute half-pel field picture coordinates */
      is += i<<1;
      js += j<<1;
      it0 += i<<1;
      jt0 += j<<1;
      ib0 += i<<1;
      jb0 += j<<1;

      if (is >= 0 && is <= (width-16)<<1 &&
          js >= 0 && js <= (height-16))
      {
        for (delta_y=-1; delta_y<=1; delta_y++)
        {
          for (delta_x=-1; delta_x<=1; delta_x++)
          {
            /* opposite field coordinates */
            it = it0 + delta_x;
            jt = jt0 + delta_y;
            ib = ib0 + delta_x;
            jb = jb0 + delta_y;

            if (it >= 0 && it <= (width-16)<<1 &&
                jt >= 0 && jt <= (height-16) &&
                ib >= 0 && ib <= (width-16)<<1 &&
                jb >= 0 && jb <= (height-16))
            {
              /* compute prediction error */
              local_dist = bdist2(
                ref + (is>>1) + (width<<1)*(js>>1),
                ref + width + (it>>1) + (width<<1)*(jt>>1),
                mb,             /* current mb location */
                width<<1,       /* adjacent line distance */
                is&1, js&1, it&1, jt&1, /* half-pel flags */
                8);             /* block height */
              local_dist += bdist2(
                ref + width + (is>>1) + (width<<1)*(js>>1),
                ref + (ib>>1) + (width<<1)*(jb>>1),
                mb + width,     /* current mb location */
                width<<1,       /* adjacent line distance */
                is&1, js&1, ib&1, jb&1, /* half-pel flags */
                8);             /* block height */

              /* update delta with least distortion vector */
              if (local_dist < vmc)
              {
                imins = is;
                jmins = js;
                imint = it;
                jmint = jt;
                iminb = ib;
                jminb = jb;
                imindmv = delta_x;
                jmindmv = delta_y;
                vmc = local_dist;
              }
            }
          }  /* end delta x loop */
        } /* end delta y loop */
      }
    }
  }

  /* Compute L1 error for decision purposes */
  local_dist = bdist1(
    ref + (imins>>1) + (width<<1)*(jmins>>1),
    ref + width + (imint>>1) + (width<<1)*(jmint>>1),
    mb,
    width<<1,
    imins&1, jmins&1, imint&1, jmint&1,
    8);
  local_dist += bdist1(
    ref + width + (imins>>1) + (width<<1)*(jmins>>1),
    ref + (iminb>>1) + (width<<1)*(jminb>>1),
    mb + width,
    width<<1,
    imins&1, jmins&1, iminb&1, jminb&1,
    8);

  *dmcp = local_dist;
  *iminp = imins;
  *jminp = jmins;
  *imindmvp = imindmv;
  *jmindmvp = jmindmv;
  *vmcp = vmc;
}

static void dpfield_estimate(topref,botref,mb,i,j,imins,jmins,
  imindmvp, jmindmvp, dmcp, vmcp)
unsigned char *topref, *botref, *mb;
int i,j;
int imins, jmins;
int *imindmvp, *jmindmvp;
int *dmcp,*vmcp;
{
  unsigned char *sameref, *oppref;
  int io0,jo0,io,jo,delta_x,delta_y,mvxs,mvys,mvxo0,mvyo0;
  int imino,jmino,imindmv,jmindmv,vmc_dp,local_dist;

  /* Calculate Dual Prime distortions for 9 delta candidates */
  /* Note: only for P pictures! */

  /* Assign opposite and same reference pointer */
  if (pict_struct==TOP_FIELD)
  {
    sameref = topref;    
    oppref = botref;
  }
  else 
  {
    sameref = botref;
    oppref = topref;
  }

  /* convert Cartesian absolute to relative motion vector
   * values (wrt current macroblock address (i,j)
   */
  mvxs = imins - (i<<1);
  mvys = jmins - (j<<1);

  /* vector for prediction from field of opposite 'parity' */
  mvxo0 = (mvxs+(mvxs>0)) >> 1;  /* mvxs // 2 */
  mvyo0 = (mvys+(mvys>0)) >> 1;  /* mvys // 2 */

  /* vertical field shift correction */
  if (pict_struct==TOP_FIELD)
    mvyo0--;
  else
    mvyo0++;

  /* convert back to absolute coordinates */
  io0 = mvxo0 + (i<<1);
  jo0 = mvyo0 + (j<<1);

  /* initialize minimum dual prime distortion to large value */
  vmc_dp = 1 << 30;

  for (delta_y = -1; delta_y <= 1; delta_y++)
  {
    for (delta_x = -1; delta_x <=1; delta_x++)
    {
      /* opposite field coordinates */
      io = io0 + delta_x;
      jo = jo0 + delta_y;

      if (io >= 0 && io <= (width-16)<<1 &&
          jo >= 0 && jo <= (height2-16)<<1)
      {
        /* compute prediction error */
        local_dist = bdist2(
          sameref + (imins>>1) + width2*(jmins>>1),
          oppref  + (io>>1)    + width2*(jo>>1),
          mb,             /* current mb location */
          width2,         /* adjacent line distance */
          imins&1, jmins&1, io&1, jo&1, /* half-pel flags */
          16);            /* block height */

        /* update delta with least distortion vector */
        if (local_dist < vmc_dp)
        {
          imino = io;
          jmino = jo;
          imindmv = delta_x;
          jmindmv = delta_y;
          vmc_dp = local_dist;
        }
      }
    }  /* end delta x loop */
  } /* end delta y loop */

  /* Compute L1 error for decision purposes */
  *dmcp = bdist1(
    sameref + (imins>>1) + width2*(jmins>>1),
    oppref  + (imino>>1) + width2*(jmino>>1),
    mb,             /* current mb location */
    width2,         /* adjacent line distance */
    imins&1, jmins&1, imino&1, jmino&1, /* half-pel flags */
    16);            /* block height */

  *imindmvp = imindmv;
  *jmindmvp = jmindmv;
  *vmcp = vmc_dp;
}




/*
 * Three step block matching algorithm - though it is called "fullsearch"
 *
 * blk: top left pel of (16*h) block
 * h: height of block
 * lx: distance (in bytes) of vertically adjacent pels in ref,blk
 * org: top left pel of source reference picture
 * ref: top left pel of reconstructed reference picture
 * i0,j0: center of search window
 * sx,sy: half widths of search window
 * xmax,ymax: right/bottom limits of search area
 * iminp,jminp: pointers to where the result is stored
 *              result is given as half pel offset from ref(0,0)
 *              i.e. NOT relative to (i0,j0)
 */

/*

Search window in ref image:

                     2sx
(ilow,jlow)                          (ihigh,jlow)  
   | ------------------------------------|
   |                                     |
   |                                     |
   |                                     |
   |                                     |
   |               (i0,j0)               |
   |                                     | 2sy
   |                                     |
   |                                     |
   |                                     |
   |                                     |
   | ------------------------------------|(xmax, ymax)
(jhihg, ilow)                       (ihigh, jhigh)   

Block (stored at memory address blk)
| ------|
|       |
|       | h    
|       |     
| ------|

*/



static int fullsearch(org,ref,blk,lx,i0,j0,sx,sy,h,xmax,ymax,iminp,jminp)
     unsigned char *org,*ref,*blk;
     int lx,i0,j0,sx,sy,h,xmax,ymax;
     int *iminp,*jminp;
{
  int i,j,imin,jmin,ilow,ihigh,jlow,jhigh;
  int d,dmin;
  /* int sxy; not used */
  int p;

  /* STEP ONE: 
     search at the center of the reference image */

  imin = i0;
  jmin = j0;
  dmin = dist1(org+imin+lx*jmin, /* mem address of ref block */
	       blk,              /* mem address of macro block to be found */
	       lx,               /* distance (in bytes) of vertically adjacent pels */
	       0,                /* no horiz interpolation */ 
	       0,                /* no vertical interpolation */ 
	       h, 
	       65536);           /*  bail out if sum exceeds 65536 */


  /* now dmin contains the distance between block and the block
     in the center of the ref image */


  /*  sxy = (sx>sy) ? sx : sy;  -- not used */


  /* STEP TWO:
     If we did not find a good match at the center of 
     the reference image from the first step we do this step.
     We search starting at the end of the lower bottom
     of the reference frame to the upper right of the 
     reference frame, advancing FOUR PELS in each step */



  if (dmin > 64) {

    int w,z;
    int limitx, limity;
    

    /* calculate the low and upper pixels of the reference 
       image - ilow, jlow, ihigh, jhigh */
    
    ilow = i0 - sx;
    ihigh = i0 + sx;

    if (ilow<0)
      ilow = 0;

    if (ihigh>xmax-16)
      ihigh = xmax-16;

    jlow = j0 - sy;
    jhigh = j0 + sy;

    if (jlow<0)
      jlow = 0;

    if (jhigh>ymax-h)
      jhigh = ymax-h;


    limitx = (sx/4);
    limity= (sy/4);
    

    
    /* Now do the search around the center */

    for (w = -limitx; w<= limitx; w++)  {
  
      for( z = -limity; z<=limity; z++)
	{

	  i=i0+(4*w);
	  j=j0+(4*z);

	  if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	    {
	      d = dist1(org+i+lx*j, blk, lx, 0, 0, h, dmin);
	      if (d<dmin)
		{
		  imin=i;
		  jmin=j;
		  dmin=d;
		  
		}
	    }
	}
		  	  
    }  /* end for */




    /* STEP THREE:
       Search around the center: First we will start at a distance
       from center of 2 (p), then move to 1. So, do the following
       while-loop two times. Each iteration compares 8 blocks.
    */

    p = 2;
    i0 = imin;
    j0 = jmin;

    while(p>0) {

      i=i0-p;   /* 1: top left block */
      j=j0-p;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0-p;   /* 2: bottom left */
      j=j0+p;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0+p;  /* 3: top right */
      j=j0-p;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0+p; /* 4: bottom right */
      j=j0+p;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0+p; /* 5: center right */
      j=j0;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0;  /* 6: bottom center */
      j=j0-p;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0-p;  /* 7: center left */
      j=j0;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
      i=i0;  /* 8: bottom center */
      j=j0+p;
      if (i>=ilow && i<=ihigh && j>=jlow && j<=jhigh)
	{
	  d = dist1(org+i+lx*j,blk,lx,0,0,h,dmin);
	  if (d<dmin)
	    {
	      imin=i;
	      jmin=j;
	      dmin=d;
	    }
	}
		   

      i0=imin;
      j0=jmin;
      p/=2;


    } /* end while */

  } /* if dmin > 64 */	
   


  /* HALF PEL:
     Now do half pel around the (imin, jmin) found from above
     steps to refine the motion vector further.
  */

  dmin = 65536;
  imin <<= 1;
  jmin <<= 1;
  ilow = imin - (imin>0);
  ihigh = imin + (imin<((xmax-16)<<1));
  jlow = jmin - (jmin>0);
  jhigh = jmin + (jmin<((ymax-h)<<1));

  for (j=jlow; j<=jhigh; j++) {
    for (i=ilow; i<=ihigh; i++) { 

      d = dist1(ref+(i>>1)+lx*(j>>1),blk,lx,i&1,j&1,h,dmin);

	if (d<dmin)
	  {
	    dmin = d;
	    imin = i;
	    jmin = j;
	  }
    }
  }


  /* Now store the motion vector at the corresponding 
     location */

  *iminp = imin;
  *jminp = jmin;
  
  return dmin;
}

#if 0
      __asm
	{
	  mov	  ebx,	[p1]	; /*load the address of p1 in ebx*/
	  mov	  edx,	[p2]	;/*  load the address of p2 in edx */
	  mov     eax,  [lx]    ;
	  mov     ecx,  [h]     ; /* loop counter */
	  pxor    xmm2, xmm2    ;
	  
	dist1_x0_y0:
	  movdqu  xmm0, [ebx]     ;/* load 16 pixels from p1 */
	  movdqu  xmm1, [edx]     ;/* load 16 pixels from p2 */
	  psadbw  xmm0, xmm1	;/* results are in [15:0] and [79:64] */
	  paddusw xmm2, xmm0    ; /* keep 2 partial sums */

	  add     ebx, eax      ;
	  add     edx, eax      ;
	  sub     ecx, 1        ;
	  cmp     ecx, 0        ;
	  jnz     dist1_x0_y0   ;

	  pshufd      xmm1, xmm2, 0xe  ; /* reduction */
	  paddd       xmm1, xmm2       ;
	  movd        [s],  xmm1       ;
	}
#endif

/*  

 * total absolute difference between two (16*h) blocks
 * including optional half pel interpolation of blk1 (hx,hy)
 * blk1,blk2: addresses of top left pels of both blocks
 * lx:        distance (in bytes) of vertically adjacent pels
 * hx,hy:     flags for horizontal and/or vertical interpolation
 * h:         height of block (usually 8 or 16)
 * distlim:   bail out if sum exceeds this value
 */

static int dist1(blk1,blk2,lx,hx,hy,h,distlim)
     unsigned char *blk1,*blk2;  /* blk1 ref, blk2 macroblock */
     int lx,hx,hy,h;
     int distlim;
{
  unsigned char *p1,*p1a,*p2;
  int i,j;

  int s;
  int v;
  int sa=0;

  if (!hx && !hy){


    s = 0;
    p1 = blk1;
    p2 = blk2;

#ifdef SSE2
      __asm
	{
	  mov	  ebx,	[p1]	; /*load the address of p1 in ebx*/
	  mov	  edx,	[p2]	;/*  load the address of p2 in edx */
	  mov     eax,  [lx]    ;
	  mov     ecx,  [h]     ; /* loop counter */
	  mov     esi, [distlim] ;
	  pxor    xmm3, xmm3    ;
	  movd    xmm3, esi     ;
	  pshufd  xmm3, xmm3, 0x44 ;
	  pxor    xmm2, xmm2    ;
	  
	dist1_x0_y0:
	  movdqu  xmm0, [ebx]     ;/* load 16 pixels from p1 */
	  movdqu  xmm1, [edx]     ;/* load 16 pixels from p2 */
	  psadbw  xmm0, xmm1	;/* results are in [15:0] and [79:64] */
	  paddusw xmm2, xmm0    ; /* keep 2 partial sums */

	  movdqa  xmm4, xmm2    ;
	  pcmpgtw xmm4, xmm3    ;
	  movd    esi, xmm4     ;
	  cmp     esi, 0xffffffff ;
	  je      dist1_branch_out    ;

	  add     ebx, eax      ;
	  add     edx, eax      ;
	  sub     ecx, 1        ;
	  cmp     ecx, 0        ;
	  jnz     dist1_x0_y0   ;
	dist1_branch_out:
	  pshufd      xmm1, xmm2, 0xe  ; /* reduction */
	  paddd       xmm1, xmm2       ;
	  movd        [s],  xmm1       ;
	}

#else
    for (j=0; j<h; j++) {
      for (i=0; i<16; i++) {
       	v = p1[i] - p2[i];
	s += (v >= 0) ? v : -v;
      }
      /*if (s >= distlim)
	break;*/
     
      p1+= lx;
      p2+= lx;
    }
#endif
    
    
  }



  else if (hx && !hy) {


       
    s = 0;         
    p1 = blk1;
    p2 = blk2;

#ifdef SSE2
    __asm
      {
	mov	eax,	[p1]			;
	mov	ebx,	[p2]			;
	mov     ecx,    [h]                     ;
	mov     edx,    [lx]                    ;
	pxor    xmm3, xmm3                      ; /*partial sums */

      dist1_x1_y0:
	movdqu	xmm0,	[eax]			;/*16 pixels of p1[i] in mm0*/
	movdqu	xmm1,	[eax+1]			;/*16 pixels of p1[i+1] in mm1*/
	movdqu	xmm2,	[ebx]			;/*16 pixels of p2[i] in mm2*/
	pavgb	xmm0,	xmm1			;/* xmm0 has the average of the 16 pixels*/
	psadbw	xmm0,	xmm2			;/* xmm0 has the abs diff of p1 half pel and p2*/
	paddd   xmm3, xmm0                      ;/*accumulate partial sums*/
	add     eax, edx       ;
	add     ebx, edx       ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	jnz     dist1_x1_y0       ;

	pshufd      xmm1, xmm3, 0xe  ;
	paddd       xmm1, xmm3       ;
	movd        [s],  xmm1       ;
      }	
#else
    for (j=0; j<h; j++) {

     for (i=0; i<16; i++) {
       
	v = ((unsigned int)(p1[i]+p1[i+1]+1)>>1) - p2[i];
	if (v>=0)
	  s+= v;
	else
          s-= v;
      }

      p1+= lx; 
      p2+= lx;
     



    } /* end of outer foor loop */
#endif
  }




  else if (!hx && hy) {

    s = 0;
    p1 = blk1;
    p2 = blk2;
    p1a = p1 + lx;
   

#ifdef SSE2
    __asm
      {	
	mov	eax,	[p1]	;
	mov	ebx,	[p1a]	;
	mov	edx,	[p2]	;
	mov     esi,    [lx]    ;
	mov     ecx,  [h]   ;
	pxor    xmm3,   xmm3    ; /*running sums */

      dist1_x0_y1:
	movdqu	xmm0,	[eax]	;/*16 pixels of p1 in xmm0*/
	movdqu	xmm1,	[ebx]	;/*16 pixels of p1a in xmm1*/
	movdqu	xmm2,	[edx]	;/*16 pixels of p2 in xmm2*/
	pavgb	xmm0,	xmm1	;/* xmm0 has the average of the 16 pixels btwn p1 & p1a*/
	psadbw	xmm0,	xmm2	;/* xmm0 has the abs diff of p1 half pel and p2*/
	paddd   xmm3, xmm0    ;
	mov     eax, ebx      ;
	add     ebx, esi      ;
	add     edx, esi      ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	jnz dist1_x0_y1

	pshufd      xmm1, xmm3, 0xe  ;
	paddd       xmm1, xmm3       ;
	movd        [s],  xmm1       ;
      }
#else
    for (j=0; j<h; j++) {

	for (i=0; i<16; i++) {
	  
	    v = ((unsigned int)(p1[i]+p1a[i]+1)>>1) - p2[i];
	    if (v>=0)
	      s+= v;
	    else
	      s-= v;
	    
	} /* end of inner for */

	p1 = p1a;
	p1a+= lx;
	p2+= lx;


    } /* end of outer for */
#endif
    
  }




  else {  /* if (hx && hy) */
  

    s = 0;
    p1 = blk1;
    p2 = blk2;
    p1a = p1 + lx;

#ifdef SSE2

    __asm
      {
	mov	eax,	[p1]			;
	mov	ebx,	[p1a]			;
	mov	edx,	[p2]            ;
	mov     ecx,    [h]             ;
	mov     esi,    [lx]            ;
	pxor    xmm5, xmm5              ;
      dist1_x1_y1:
	movdqu	xmm0,	[eax]			;/*	16 pixels of p1[i] in xmm0*/
	movdqu	xmm1,	[eax+1]			;/*	16 pixels of p1[i+1] in xmm1*/
	movdqu	xmm2,	[ebx]			;/*	16 pixels of p1a[i] in xmm2*/
	movdqu	xmm3,	[ebx+1]			;/*	16 pixels of p1a[i+1] in xmm3*/
	movdqu	xmm4,	[edx]			;/*	16 pixels of p2[i] in xmm4*/
	pavgb	xmm0,	xmm1			;/* xmm0 has the average of the 16 pixels btwn p1[i] & p1[i+1]*/
	pavgb	xmm2,	xmm3			;/* xmm2 has the avg of the 16 pels btwn btwn p1a[i] & p1a[i+1]*/
	pavgb	xmm0,	xmm2			;/* xmm0 has the avg of all 4*/
	psadbw	xmm0,	xmm4			;/* xmm0 has the abs diff*/
	paddd   xmm5, xmm0    ;
	mov     eax, ebx      ;
	add     ebx, esi      ;
	add     edx, esi      ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	jnz dist1_x1_y1      ;

	pshufd      xmm1, xmm5, 0xe  ;
	paddd       xmm1, xmm5       ;
	movd        [s],  xmm1       ;
      }
#else

    for (j=0; j<h; j++) {
      for (i=0; i<16; i++) {
        v = ((unsigned int)(p1[i]+p1[i+1]+p1a[i]+p1a[i+1]+2)>>2) - p2[i];
        if (v>=0)
          s+= v;
        else
          s-= v;
      }
      p1 = p1a;
      p1a+= lx;
      p2+= lx;

    } /* end of outer for */
#endif
  }

  return s;
}



#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))




/*
 * total squared difference between two (16*h) blocks
 * including optional half pel interpolation of blk1 (hx,hy)
 * blk1,blk2: addresses of top left pels of both blocks
 * lx:        distance (in bytes) of vertically adjacent pels
 * hx,hy:     flags for horizontal and/or vertical interpolation
 * h:         height of block (usually 8 or 16)
 */
static int dist2(blk1,blk2,lx,hx,hy,h)
     unsigned char *blk1,*blk2;  /* blk1 ref, blk2 macroblock */
     int lx,hx,hy,h;
{
  unsigned char *p1,*p1a,*p2;
  int i,j;

  int s;
  int v;
  int sa=0;


  if (!hx && !hy){

    s = 0;
    p1 = blk1;
    p2 = blk2;

#ifdef SSE2

      __asm
	{	   
	  mov		ebx,	[p1]	; /*load the address of p1 in ecx */
	  mov		edx,	[p2]	; /*load the address of p2 in edx*/
	  mov           ecx,    [h]     ; /*load loop counter */
	  mov           eax,    [lx]    ;
	  pxor	       xmm6, xmm6    ; /*initialize it to zero*/
	  pxor	       xmm4, xmm4    ; /*running sum: initialize it to zero*/

	dist2_x0_y0:
	  movdqu       xmm0, [ebx]     ; /*load 16 pixels from p1*/
	  movdqu       xmm1, [edx]     ; /*load 16 pixels from p2*/
	  movdqa       xmm2, xmm0    ; /*make a copy of p1 in xmm2*/
	  psubusb      xmm0, xmm1    ; /*p1 - p2*/
	  psubusb      xmm1, xmm2    ; /*p2 - p1*/
	  por	       xmm0, xmm1    ; /*xmm0 has the absolute diff for the 16 pixels*/
	  movdqa	xmm3, xmm0    ; /*make a copy of the result*/
	  punpcklbw     xmm0, xmm6  ; /*unpack bytes to words*/
	  punpckhbw    xmm3, xmm6  ;
	  pmaddwd	xmm0,	xmm0		; /*xmm0 contains 4 double words d1*d1+d2*d2, d3*d3+d4*d4, ..., d6*d6+d7*d7*/
	  pmaddwd       xmm3,	xmm3		; /*xmm3 contains 4 double words d8*d8+d9*d9, ..., d14*d14+d15*d15*/
	  paddd	        xmm0,	xmm3		; /*add double words*/
	  paddd         xmm4,   xmm0            ; /* accumulate */
	  add           ebx, eax       ;
	  add           edx, eax       ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	  jnz          dist2_x0_y0    ;

	  pshufd        xmm0, xmm4, 0xe  ;
	  paddd         xmm4, xmm0      ; /* contains 2 32-bit words */
	  pshuflw        xmm0, xmm4, 0xe ;
	  paddd         xmm4, xmm0      ; /* contains 1 32-bit words */
	  movd          [s], xmm4       ;
	}
#else
    for (j=0; j<h; j++) {
      for (i=0; i<16; i++) {
       	v = p1[i] - p2[i];
	s += v*v;
      }

      p1+= lx;
      p2+= lx;
    }
#endif
  }



  else if (hx && !hy) {

    s = 0;
    p1 = blk1;
    p2 = blk2;

#ifdef SSE2
    __asm
      {
	mov     eax,	[p1]	;
	mov	ebx,	[p2]	;
	mov     ecx,    [h]     ;       
	mov     edx,    [lx]    ;
	pxor	xmm6,	xmm6		;/*initialize xmm6 to zero*/
	pxor	xmm4,	xmm4		;/*initialize xmm6 to zero*/
	
      dist2_x1_y0:
	movdqu	xmm0,	[eax]		;/* 16 pixels of p1[i] in mm0*/
	movdqu	xmm1,	[eax+1]	        ;/*	16 pixels of p1[i+1] in mm1*/
        movdqu	xmm2,	[ebx]		;/*	16 pixels of p2[i] in mm2*/
	pavgb	xmm0,	xmm1            ;/**/
	movdqa	xmm1,	xmm0		;/*p1[i]/2 + p1[i+1]/2 in xmm1*/
	psubusb	xmm0,	xmm2		;/*xmm0 = positive values of p1[i]/2 + p1[i+1]/2 - p2[i], else = 0*/
	psubusb	xmm2,	xmm1		;/*xmm2 = positive values of p2[i] - {p1[i]/2 + p1[i+1]/2} else = 0*/
	por	xmm0,	xmm2		;/*xmm0 contains the absolute difference for 16 pels*/
	movdqa	xmm3,	xmm0		;/*xmm1 contains the absolute difference for 16 pels*/
	punpcklbw	xmm0,	xmm6	;/*unpack low part from byte to word */
	punpckhbw	xmm3,	xmm6	;/*unpack high part from byte to word */
	pmaddwd	xmm0,	xmm0		;/* xmm0 contains 4 double words d1*d1+d2*d2, d3*d3+d4*d4, ..., d6*d6+d7*d7*/
	pmaddwd xmm3,	xmm3		;/* xmm3 contains 4 double words d8*d8+d9*d9, ..., d14*d14+d15*d15*/
	paddd	xmm0,	xmm3		;/* add double words*/
	paddd   xmm4,   xmm0            ;/* running partial sums */
	add     eax, edx                ;
	add     ebx, edx                ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	jnz dist2_x1_y0                ;

	pshufd        xmm0, xmm4, 0xe  ;
	paddd         xmm4, xmm0      ; /* contains 2 32-bit words */
	pshuflw        xmm0, xmm4, 0xe ;
	paddd         xmm4, xmm0      ; /* contains 1 32-bit words */
	movd          [s], xmm4       ;
      }
#else

    for (j=0; j<h; j++) {

      for (i=0; i<16; i++) {
       
	v = ((unsigned int)(p1[i]+p1[i+1]+1)>>1) - p2[i];
	s += v*v;

      }

      p1+= lx;
      p2+= lx;
     
    } /* end of outer foor loop */
#endif
  }


  else if (!hx && hy) {


    s = 0;
    p1 = blk1;
    p2 = blk2;
    p1a = p1 + lx;
#ifdef SSE2
    __asm
      {			
	mov	eax,	[p1]	;
	mov	ebx,	[p1a]	;
	mov	edx,	[p2]	;
	mov     ecx,    [h]     ;
	mov     esi,    [lx]    ;
	pxor	xmm6,	xmm6		;/*	initialize xmm6 to zero*/
	pxor	xmm4,	xmm4		;/*running sum:initialize xmm4 to zero*/

      dist2_x0_y1:
	movdqu	xmm0,	[eax]		;/*	16 pixels of p1[i] in mm0*/
	movdqu	xmm1,	[ebx]		;/*	16 pixels of p1a[i] in mm1*/
	movdqu	xmm2,	[edx]		;/*	16 pixels of p2[i] in mm2*/
	pavgb	xmm0,	xmm1            ;
	movdqa	xmm1,	xmm0		;/*	p1[i]/2 + p1a[i]/2 in xmm1*/
	psubusb	xmm0,	xmm2		;/*	xmm0 = positive values of p1[i]/2 + p1a[i]/2 - p2[i], else = 0*/
	psubusb	xmm2,	xmm1		;/*	xmm2 = positive values of p2[i] - {p1[i]/2 + p1a[i]/2} else = 0*/
	por	xmm0,	xmm2		;/*	xmm0 contains the absolute difference for 16 pels*/
	movdqa	xmm3,	xmm0		;/*	xmm1 contains the absolute difference for 16 pels*/
	punpcklbw	xmm0,	xmm6	;/*	unpack low part from byte to word */
	punpckhbw	xmm3,	xmm6	;/*	unpack high part from byte to word */
	pmaddwd	xmm0,	xmm0		;/* xmm0 contains 4 double words d1*d1+d2*d2, d3*d3+d4*d4, ..., d6*d6+d7*d7*/
	pmaddwd xmm3,	xmm3		;/* xmm3 contains 4 double words d8*d8+d9*d9, ..., d14*d14+d15*d15*/
	paddd	xmm0,	xmm3		;/* add double words*/
	paddd   xmm4,   xmm0            ;
	add     eax, esi                ;
	add     ebx, esi                ;
	add     edx, esi                ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	jnz dist2_x0_y1                ;

	pshufd      xmm0, xmm4, 0xe  ;
	paddd       xmm4, xmm0      ; /* contains 2 32-bit words */
	pshuflw     xmm0, xmm4, 0xe ;
	paddd       xmm4, xmm0      ; /* contains 1 32-bit words */
	movd        [s], xmm4       ;
      }
#else

    for (j=0; j<h; j++) {

	for (i=0; i<16; i++) {
	  
	    v = ((unsigned int)(p1[i]+p1a[i]+1)>>1) - p2[i];
	    s += v*v;


	} /* end of inner for */


	p1 = p1a;
	p1a+= lx;
	p2+= lx;



    } /* end of outer for */
#endif  
  }




  else {  /* if (hx && hy) */
  

    s = 0;
    p1 = blk1;
    p2 = blk2;
    p1a = p1 + lx;

#ifdef SSE2
    __asm
      {
	mov	eax,	[p1]	;
	mov	ebx,	[p1a]	;
	mov	edx,	[p2]    ;
	mov     ecx,    [h]     ;
	mov     esi,    [lx]    ;
	pxor	xmm6,	xmm6		;/*	initialize xmm6 to zero*/
	pxor	xmm5,	xmm5		;/*	initialize xmm5 to zero*/

      dist2_x1_y1:
	movdqu	xmm0,	[eax]		;/*	16 pixels of p1[i] in xmm0*/
	movdqu	xmm1,	[eax+1]		;/*	16 pixels of p1[i+1] in xmm1*/
	movdqu	xmm2,	[ebx]		;/*	16 pixels of p1a[i] in xmm2*/
	movdqu	xmm3,	[ebx+1]		;/*	16 pixels of p1a[i+1] in xmm3*/
	movdqu	xmm4,	[edx]		;/*	16 pixels of p2[i] in xmm4*/
	pavgb	xmm0,	xmm1            ;
	pavgb	xmm2,	xmm3  ;
	pavgb	xmm0,	xmm2  ;
	movdqa	xmm1,	xmm0		;/*	p1[i]/4 + p1[i+1]/4 + p1a[i]/4 + p1a[i+1]/4 in xmm1*/
	psubusb	xmm0,	xmm4		;/*	xmm0 = positive values of p1[i]/4 + p1[i+1]/4 + p1a[i]/4 + p1a[i+1]/4 - p2[i], else = 0*/
	psubusb	xmm4,	xmm1		;/*	xmm4 = positive values of p2[i] - {p1[i]/4 + p1[i+1]/4 + p1a[i]/4 + p1a[i+1]/4} else = 0*/
	por	xmm0,	xmm4		;/*	xmm0 contains the absolute difference for 16 pixels*/
	movdqa	xmm1,	xmm0		;/*	xmm1 contains the absolute difference for 16 pels*/
	punpcklbw	xmm0,	xmm6	;/*	unpack low part from byte to word */
	punpckhbw	xmm1,	xmm6	;/*	unpack high part from byte to word */
	pmaddwd	xmm0,	xmm0		;/* xmm0 contains 4 double words d1*d1+d2*d2, d3*d3+d4*d4, ..., d6*d6+d7*d7*/
	pmaddwd xmm1,	xmm1		;/* xmm1 contains 4 double words d8*d8+d9*d9, ..., d14*d14+d15*d15*/
	paddd	xmm0,	xmm1		;/* add double words*/
	paddd   xmm5,   xmm0            ;
	add     eax, esi                ;
	add     ebx, esi                ;
	add     edx, esi                ;
	sub     ecx, 1        ;
	cmp     ecx, 0        ;
	jnz dist2_x1_y1              ;
	
	pshufd      xmm0, xmm5, 0xe  ; /* reduction */
	paddd       xmm5, xmm0      ; /* contains 2 32-bit words */
	pshuflw     xmm0, xmm5, 0xe ;
	paddd       xmm5, xmm0      ; /* contains 1 32-bit words */
	movd        [s], xmm5       ;
      }
#else
    for (j=0; j<h; j++) {

      for (i=0; i<16; i++) {
        v = ((unsigned int)(p1[i]+p1[i+1]+p1a[i]+p1a[i+1]+2)>>2) - p2[i];
        s += v*v;

      }


      p1 = p1a;
      p1a+= lx;
      p2+= lx;



    } /* end of outer for */
#endif
  }

  return s;
}

#include<assert.h>

/*
 * absolute difference error between a (16*h) block and a bidirectional
 * prediction
 *
 * p2: address of top left pel of block
 * pf,hxf,hyf: address and half pel flags of forward ref. block
 * pb,hxb,hyb: address and half pel flags of backward ref. block
 * h: height of block
 * lx: distance (in bytes) of vertically adjacent pels in p2,pf,pb
 */
static int bdist1(pf,pb,p2,lx,hxf,hyf,hxb,hyb,h)
unsigned char *pf,*pb,*p2;
int lx,hxf,hyf,hxb,hyb,h;
{
  unsigned char *pfa,*pfb,*pfc,*pba,*pbb,*pbc;
  int i,j;
  int s,v;
  int sa=0;

  pfa = pf + hxf;
  pfb = pf + lx*hyf;
  pfc = pfb + hxf;

  pba = pb + hxb;
  pbb = pb + lx*hyb;
  pbc = pbb + hxb;

  s = 0;

#ifdef SSE2
  __asm 
    {
      pxor xmm7, xmm7    ; /* initialize running sum */
    }
  
  for (j=0; j<h; j++) {
    __asm
      {
	mov	eax,	[pf]	;
	mov	ebx,	[pfa]	;
	mov	edx,	[pfb]    ;
	mov     edi,    [pfc]    ;
	mov     esi,    [p2]
	movdqu	xmm0,	[eax]		;
	movdqu	xmm1,	[ebx]		;
	movdqu	xmm2,	[edx]		;
	movdqu	xmm3,	[edi]		;
	mov	eax,	[pb]	;
	mov	ebx,	[pba]	;
	mov	edx,	[pbb]    ;
	mov     edi,    [pbc]    ;
	movdqu	xmm4,	[eax]		;
	movdqu	xmm5,	[ebx]		;
	movdqu	xmm6,	[edx]		;

	pavgb	xmm0,	xmm1            ; /* avg(pf,pfa) */
	movdqu	xmm1,	[edi]		;
	pavgb	xmm2,	xmm3            ; /* avg(pfb, pfc)*/
	movdqu	xmm3,	[esi]		; /* load p2 */
	pavgb   xmm0,   xmm2            ; /* avg(pf, pfa, pfb, pfc) */

	pavgb   xmm4,   xmm5            ; /* avg(pb, pba) */
	pavgb   xmm6,   xmm1            ; /* avg(pbb, pbc) */
	pavgb   xmm4,   xmm6            ; /* avg(pb, pba, pbb, pbc) */
	pavgb   xmm0,   xmm4            ; /* avg(pfx, pbx) */

	psadbw	xmm0,	xmm3	;/* xmm0 has the abs diff of p1 half pel and p2*/
	paddd   xmm7, xmm0      ;
      }
    p2+= lx-16;
    pf+= lx-16;
    pfa+= lx-16;
    pfb+= lx-16;
    pfc+= lx-16;
    pb+= lx-16;
    pba+= lx-16;
    pbb+= lx-16;
    pbc+= lx-16;
  }
  __asm 
    {
      pshufd      xmm1, xmm7, 0xe  ; /* reduction */
      paddd       xmm1, xmm7       ;
      movd        [s],  xmm1       ;
    }

#else
  for (j=0; j<h; j++)
  {

    for (i=0; i<16; i++)
    {
      v = ((((unsigned int)(*pf++ + *pfa++ + *pfb++ + *pfc++ + 2)>>2) +
            ((unsigned int)(*pb++ + *pba++ + *pbb++ + *pbc++ + 2)>>2) + 1)>>1)
           - *p2++;
      if (v>=0)
        s+= v;
      else
        s-= v;
    }
    p2+= lx-16;
    pf+= lx-16;
    pfa+= lx-16;
    pfb+= lx-16;
    pfc+= lx-16;
    pb+= lx-16;
    pba+= lx-16;
    pbb+= lx-16;
    pbc+= lx-16;
  }
#endif

  return s;
}







/*
 * squared error between a (16*h) block and a bidirectional
 * prediction
 *
 * p2: address of top left pel of block
 * pf,hxf,hyf: address and half pel flags of forward ref. block
 * pb,hxb,hyb: address and half pel flags of backward ref. block
 * h: height of block
 * lx: distance (in bytes) of vertically adjacent pels in p2,pf,pb
 */
static int bdist2(pf,pb,p2,lx,hxf,hyf,hxb,hyb,h)
unsigned char *pf,*pb,*p2;
int lx,hxf,hyf,hxb,hyb,h;
{
  unsigned char *pfa,*pfb,*pfc,*pba,*pbb,*pbc;
  int i,j;
  int s,v;
  int sa=0;

  pfa = pf + hxf;
  pfb = pf + lx*hyf;
  pfc = pfb + hxf;

  pba = pb + hxb;
  pbb = pb + lx*hyb;
  pbc = pbb + hxb;

#ifdef SSE2
  __asm 
    {
      pxor xmm7, xmm7    ; /* initialize running sum */
      pxor xmm6, xmm6    ; /* for unpacking */
    }
  
  for (j=0; j<h; j++) {
    __asm
      {
	mov	eax,	[pf]	;
	mov	ebx,	[pfa]	;
	mov	edx,	[pfb]    ;
	mov     edi,    [pfc]    ;
	mov     esi,    [p2]
	movdqu	xmm0,	[eax]		;
	movdqu	xmm1,	[ebx]		;
	movdqu	xmm2,	[edx]		;
	movdqu	xmm3,	[edi]		;
	mov	eax,	[pb]	;
	mov	ebx,	[pba]	;
	mov	edx,	[pbb]    ;
	mov     edi,    [pbc]    ;
	movdqu	xmm4,	[eax]		;
	movdqu	xmm5,	[ebx]		;

	pavgb	xmm0,	xmm1            ; /* avg(pf,pfa) */
	movdqu	xmm1,	[edi]		;
	pavgb	xmm2,	xmm3            ; /* avg(pfb, pfc)*/
	movdqu	xmm3,	[esi]		; /* load p2 */
	pavgb   xmm0,   xmm2            ; /* avg(pf, pfa, pfb, pfc) */
	movdqu	xmm2,	[edx]		;

	pavgb   xmm4,   xmm5            ; /* avg(pb, pba) */
	pavgb   xmm2,   xmm1            ; /* avg(pbb, pbc) */
	pavgb   xmm4,   xmm2            ; /* avg(pb, pba, pbb, pbc) */
	pavgb   xmm0,   xmm4            ; /* avg(pfx, pbx) */

	movdqa	xmm1,	xmm0		;/*	p1[i]/4 + p1[i+1]/4 + p1a[i]/4 + p1a[i+1]/4 in xmm1*/
	psubusb	xmm0,	xmm3		;/*	xmm0 = positive values of p1[i]/4 + p1[i+1]/4 + p1a[i]/4 + p1a[i+1]/4 - p2[i], else = 0*/
	psubusb	xmm3,	xmm1		;/*	xmm3 = positive values of p2[i] - {p1[i]/4 + p1[i+1]/4 + p1a[i]/4 + p1a[i+1]/4} else = 0*/
	por	xmm0,	xmm3		;/*	xmm0 contains the absolute difference for 16 pixels*/
	movdqa	xmm1,	xmm0		;/*	xmm1 contains the absolute difference for 16 pels*/
	punpcklbw	xmm0,	xmm6	;/*	unpack low part from byte to word */
	punpckhbw	xmm1,	xmm6	;/*	unpack high part from byte to word */
	pmaddwd	xmm0,	xmm0		;/* xmm0 contains 4 double words d1*d1+d2*d2, d3*d3+d4*d4, ..., d6*d6+d7*d7*/
	pmaddwd xmm1,	xmm1		;/* xmm1 contains 4 double words d8*d8+d9*d9, ..., d14*d14+d15*d15*/
	paddd	xmm0,	xmm1		;/* add double words*/
	paddd   xmm7, xmm0      ;
      }
    p2+= lx-16;
    pf+= lx-16;
    pfa+= lx-16;
    pfb+= lx-16;
    pfc+= lx-16;
    pb+= lx-16;
    pba+= lx-16;
    pbb+= lx-16;
    pbc+= lx-16;
  }
  __asm 
    {
      pshufd      xmm0, xmm7, 0xe  ; /* reduction */
      paddd       xmm7, xmm0      ; /* contains 2 32-bit words */
      pshuflw     xmm0, xmm7, 0xe ;
      paddd       xmm7, xmm0      ; /* contains 1 32-bit words */
      movd        [s], xmm7       ;
    }
#else

  s = 0;

  for (j=0; j<h; j++)
  {
    for (i=0; i<16; i++)
    {
      v = ((((unsigned int)(*pf++ + *pfa++ + *pfb++ + *pfc++ + 2)>>2) +
            ((unsigned int)(*pb++ + *pba++ + *pbb++ + *pbc++ + 2)>>2) + 1)>>1)
          - *p2++;
      s+=v*v;
    }
    p2+= lx-16;
    pf+= lx-16;
    pfa+= lx-16;
    pfb+= lx-16;
    pfc+= lx-16;
    pb+= lx-16;
    pba+= lx-16;
    pbb+= lx-16;
    pbc+= lx-16;
  }
#endif



  return s;
}



  
/* variance of a (16*16) block, multiplied by 256
 * p:  address of top left pel of block
 * lx: distance (in bytes) of vertically adjacent pels
 */
static int variance(p,lx)
unsigned char *p;
int lx;
{
  int i,j;
  unsigned int v,s,s2;
  unsigned int sa, s2a;

  sa=s2a=0;
  s = s2 = 0;

#ifdef SSE2
  __asm
    {	    
      mov	eax,	[p]		;/* load the address of p1 in ecx*/
      mov       ecx,    16              ;
      mov       ebx,    [lx]            ;
      pxor	xmm6,	xmm6		;/* initialize xmm6 to zero*/
      pxor      xmm3,   xmm3            ;
      pxor      xmm4,   xmm4            ;

    variance_loop:
      movdqu	xmm0,	[eax]		;/* loads the 16 pixels from p : p[1]...p[16]*/

      movdqa	xmm1,	xmm0		;/* make a copy of xmm0*/
      movdqa	xmm2,	xmm0            ;
      psadbw	xmm2,	xmm6		;/* collapsing 16 bytes into two words      		        */
      paddd     xmm3,   xmm2            ;/* 2 32-bit running sums */

      punpckhbw	xmm0,	xmm6		;/* xmm0 : contains p[0] to p[7] in words*/
      punpcklbw	xmm1,	xmm6		;/* xmm1 : contains p[8] to p[15] in words*/
      pmaddwd	xmm0,	xmm0		;/* xmm0 contains 4 double words p[1]^2+p[2]^2, ..., p[6]^2+p[7]^2*/
      pmaddwd	xmm1,	xmm1		;/* xmm1 contains 4 double words p[8]^2+p[9]^2, ..., p[14]^2+p[15]^2*/
      paddd	xmm0,	xmm1		;/* add double words*/
      paddd     xmm4,   xmm0            ;/* 4 32-bit running sums */
      add       eax, ebx                ;
      sub     ecx, 1        ;
      cmp     ecx, 0        ;
      jnz variance_loop                ;

      pshufd      xmm1, xmm3, 0xe  ;
      paddd       xmm1, xmm3       ;
      movd        [s],  xmm1       ;
      pshufd      xmm0, xmm4, 0xe  ;
      paddd       xmm4, xmm0      ; /* contains 2 32-bit words */
      pshuflw     xmm0, xmm4, 0xe ;
      paddd       xmm4, xmm0      ; /* contains 1 32-bit words */
      movd        [s2], xmm4       ;
    }
#else
  for (j=0; j<16; j++) {

    for (i=0; i<16; i++) {
      v = *p++;
      s+= v;
      s2+= v*v;
    }

    p+= lx-16;
  }
#endif
  return s2 - (s*s)/256;
}





