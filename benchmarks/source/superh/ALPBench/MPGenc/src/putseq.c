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


/* putseq.c, sequence level routines                                        */

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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "global.h"

#ifdef LTHREAD
#if (NUM_THREADS>1)
#include <pthread.h>
#endif


void *thread_work(void* threadargs) {
  struct thread_data *my_data = (struct thread_data*) threadargs;

  /* Arguments for motion estimation*/
  struct Data_Args *mda = my_data->data_ptr; /*mda stands for my data args */
  int start_height = my_data->s_height;
  int end_height = my_data->e_height;
  struct mbinfo *mbi = mda->mbi;
  

  ptmotion_estimation(mda->oldorg, mda->neworg, mda->oldref, mda->newref, mda->cur,
		      mda->curref, mda->sxf, mda->syf, mda->sxb, mda->syb, mbi,
		      mda->secondfield, mda->ipflag, start_height, 
		      end_height);
  
  ptpredict(mda->reff, mda->refb, mda->pd_cur, mda->secondfield, mbi, start_height, 
	    end_height);
  
  ptdct_type_estimation(mda->pred,mda->dte_cur,mbi, start_height, end_height);
  
  pttransform(mda->pd_cur, mda->trfm_cur,mbi,(short (*)[64])mda->blocks,
	       start_height, end_height);
  
  ptquant(my_data->s_mbh, my_data->e_mbh);
  
  ptputpict(mda->dte_cur, my_data->pp_smbh, my_data->pp_embh, 
	    mda->prev_mquant, my_data->id);
  
  ptiquant(my_data->s_mbh, my_data->e_mbh);
  ptitransform(mda->pd_cur, mda->itrfm_cur,mbi,(short (*)[64])mda->blocks,
	       start_height, end_height);

  return NULL;
}

struct thread_data thread_data_array[NUM_THREADS];
struct Data_Args data_args;

void thread_work_dist()
{
  int tmp, tmp1;
  int rc, t, status;

  int excess, chunk;

#if (NUM_THREADS>1)
  pthread_attr_t attr;
  pthread_t thread[NUM_THREADS-1];
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif

  chunk = height2/NUM_THREADS;
  excess = (height2/16)%NUM_THREADS;

  for(t=0;t < NUM_THREADS;t++)
  {
    if (t==0) {
      thread_data_array[t].s_height = 0;
      thread_data_array[t].s_mbh = 0;
      thread_data_array[t].pp_smbh = 0;
      tmp = chunk;
    } else {
      thread_data_array[t].s_height = thread_data_array[t-1].e_height;
      thread_data_array[t].s_mbh = thread_data_array[t-1].e_mbh;
      thread_data_array[t].pp_smbh = thread_data_array[t-1].pp_embh;
      tmp = thread_data_array[t-1].e_height + chunk;
    }

    if (excess) {
      tmp = tmp + 16 - (tmp%16);
      excess--;
    } else {
      tmp = tmp - (tmp%16);
    }
    tmp1 = tmp/16;

    thread_data_array[t].e_height = tmp;
    thread_data_array[t].pp_embh = tmp1;
    thread_data_array[t].e_mbh = tmp1*mb_width;
    
    thread_data_array[t].id = t;
    
    thread_data_array[t].data_ptr = &data_args;

#if (NUM_THREADS>1)
     /* create threads */
    if (t<NUM_THREADS-1) {
      rc = pthread_create(&thread[t], &attr, thread_work, 
			  (void*) &thread_data_array[t]); 
      if (rc)
	{
	  printf("ERROR; return code from pthread_create() is %d\n", rc);
	  exit(-1);
	}
    }
#endif
  }

#if (NUM_THREADS>1)
  /* Free attribute and wait for the other threads */
  pthread_attr_destroy(&attr);
#endif

  /* disabled rate control, detail can be found at putpic.c */

  putpicthdr();
  
  if (!mpeg1)
    putpictcodext();
  
  alignbits(); /*align common bitstream buffer */

  thread_work(&thread_data_array[NUM_THREADS-1]);
  
#if (NUM_THREADS>1)
  for(t=0;t < NUM_THREADS-1;t++)
    {
      rc = pthread_join(thread[t], (void **)&status);
      if (rc)
	{
	  printf("ERROR; return code from pthread_join() is %d\n", rc);
	  exit(-1);
	}
      flushbits(t);
      
    }
#endif
  
  flushbits(NUM_THREADS-1);

}

#endif

void putseq()
{
  /* this routine assumes (N % M) == 0 */
  int i, j, k, f, f0, n, np, nb, sxf, syf, sxb, syb;
  int ipflag;
  /*FILE *fd;*/
  char name[256];
  unsigned char *neworg[3], *newref[3];
#ifndef QUIET
  static char ipb[5] = {' ','I','P','B','D'};
#endif

  rc_init_seq(); /* initialize rate control */
  /* sequence header, sequence extension and sequence display extension */
  putseqhdr();
  if (!mpeg1)
  {
    putseqext();
    putseqdispext();
  }

  /* optionally output some text data (description, copyright or whatever) */
  if (strlen(id_string) > 1)
    putuserdata(id_string);

  /* loop through all frames in encoding/decoding order */
  for (i=0; i<nframes; i++)
  {
  
  if (!quiet)
    {
      fprintf(stderr,"Encoding frame %d ",i);
      fflush(stderr);
    }

    /* f0: lowest frame number in current GOP
     *
     * first GOP contains N-(M-1) frames,
     * all other GOPs contain N frames
     */
    f0 = N*((i+(M-1))/N) - (M-1);

    if (f0<0)
      f0=0;

    if (i==0 || (i-1)%M==0)
    {
      /* I or P frame */
      for (j=0; j<3; j++)
      {
        /* shuffle reference frames */
        neworg[j] = oldorgframe[j];
        newref[j] = oldrefframe[j];
        oldorgframe[j] = neworgframe[j];
        oldrefframe[j] = newrefframe[j];
        neworgframe[j] = neworg[j];
        newrefframe[j] = newref[j];
      }

      /* f: frame number in display order */
      f = (i==0) ? 0 : i+M-1;
      if (f>=nframes)
        f = nframes - 1;

      if (i==f0) /* first displayed frame in GOP is I */
      {
        /* I frame */
        pict_type = I_TYPE;
        forw_hor_f_code = forw_vert_f_code = 15;
        back_hor_f_code = back_vert_f_code = 15;

        /* n: number of frames in current GOP
         *
         * first GOP contains (M-1) less (B) frames
         */
        n = (i==0) ? N-(M-1) : N;

        /* last GOP may contain less frames */
        if (n > nframes-f0)
          n = nframes-f0;

        /* number of P frames */
        if (i==0)
          np = (n + 2*(M-1))/M - 1; /* first GOP */
        else
          np = (n + (M-1))/M - 1;

        /* number of B frames */
        nb = n - np - 1;

        rc_init_GOP(np,nb);

        putgophdr(f0,i==0); /* set closed_GOP in first GOP only */
      }
      else
      {
        /* P frame */
        pict_type = P_TYPE;
        forw_hor_f_code = motion_data[0].forw_hor_f_code;
        forw_vert_f_code = motion_data[0].forw_vert_f_code;
        back_hor_f_code = back_vert_f_code = 15;
        sxf = motion_data[0].sxf;
        syf = motion_data[0].syf;
      }
    }
    else
    {
      /* B frame */
      for (j=0; j<3; j++)
      {
        neworg[j] = auxorgframe[j];
        newref[j] = auxframe[j];
      }

      /* f: frame number in display order */
      f = i - 1;
      pict_type = B_TYPE;
      n = (i-2)%M + 1; /* first B: n=1, second B: n=2, ... */
      forw_hor_f_code = motion_data[n].forw_hor_f_code;
      forw_vert_f_code = motion_data[n].forw_vert_f_code;
      back_hor_f_code = motion_data[n].back_hor_f_code;
      back_vert_f_code = motion_data[n].back_vert_f_code;
      sxf = motion_data[n].sxf;
      syf = motion_data[n].syf;
      sxb = motion_data[n].sxb;
      syb = motion_data[n].syb;
    }

    temp_ref = f - f0;
    frame_pred_dct = frame_pred_dct_tab[pict_type-1];
    q_scale_type = qscale_tab[pict_type-1];
    intravlc = intravlc_tab[pict_type-1];
    altscan = altscan_tab[pict_type-1];

#ifndef QUIET
    fprintf(statfile,"\nFrame %d (#%d in display order):\n",i,f);
    fprintf(statfile," picture_type=%c\n",ipb[pict_type]);
    fprintf(statfile," temporal_reference=%d\n",temp_ref);
    fprintf(statfile," frame_pred_frame_dct=%d\n",frame_pred_dct);
    fprintf(statfile," q_scale_type=%d\n",q_scale_type);
    fprintf(statfile," intra_vlc_format=%d\n",intravlc);
    fprintf(statfile," alternate_scan=%d\n",altscan);

    if (pict_type!=I_TYPE)
    {
      fprintf(statfile," forward search window: %d...%d / %d...%d\n",
        -sxf,sxf,-syf,syf);
      fprintf(statfile," forward vector range: %d...%d.5 / %d...%d.5\n",
        -(4<<forw_hor_f_code),(4<<forw_hor_f_code)-1,
        -(4<<forw_vert_f_code),(4<<forw_vert_f_code)-1);
    }

    if (pict_type==B_TYPE)
    {
      fprintf(statfile," backward search window: %d...%d / %d...%d\n",
        -sxb,sxb,-syb,syb);
      fprintf(statfile," backward vector range: %d...%d.5 / %d...%d.5\n",
        -(4<<back_hor_f_code),(4<<back_hor_f_code)-1,
        -(4<<back_vert_f_code),(4<<back_vert_f_code)-1);
    }
#endif
    sprintf(name,tplorg,f+frame0);
    readframe(name,neworg);

    /*** WE DO ONLY FRAME PIC *** Partha */
    if (fieldpic)
    {
      if (!quiet)
      {
        fprintf(stderr,"\nfirst field  (%s) ",topfirst ? "top" : "bot");
        fflush(stderr);
      }

      pict_struct = topfirst ? TOP_FIELD : BOTTOM_FIELD;

      motion_estimation(oldorgframe[0],neworgframe[0],
                        oldrefframe[0],newrefframe[0],
                        neworg[0],newref[0],
                        sxf,syf,sxb,syb,mbinfo,0,0);

      predict(oldrefframe,newrefframe,predframe,0,mbinfo);
      dct_type_estimation(predframe[0],neworg[0],mbinfo);
      transform(predframe,neworg,mbinfo,blocks);

      putpict(neworg[0]);

      for (k=0; k<mb_height2*mb_width; k++)
      {
        if (mbinfo[k].mb_type & MB_INTRA)
          for (j=0; j<block_count; j++)
            iquant_intra(blocks[k*block_count+j],blocks[k*block_count+j],
                         dc_prec,intra_q,mbinfo[k].mquant);
        else
          for (j=0;j<block_count;j++)
            iquant_non_intra(blocks[k*block_count+j],blocks[k*block_count+j],
                             inter_q,mbinfo[k].mquant);
      }

      itransform(predframe,newref,mbinfo,blocks);
      calcSNR(neworg,newref);
      stats();

      if (!quiet)
      {
        fprintf(stderr,"second field (%s) ",topfirst ? "bot" : "top");
        fflush(stderr);
      }

      pict_struct = topfirst ? BOTTOM_FIELD : TOP_FIELD;

      ipflag = (pict_type==I_TYPE);
      if (ipflag)
      {
        /* first field = I, second field = P */
        pict_type = P_TYPE;
        forw_hor_f_code = motion_data[0].forw_hor_f_code;
        forw_vert_f_code = motion_data[0].forw_vert_f_code;
        back_hor_f_code = back_vert_f_code = 15;
        sxf = motion_data[0].sxf;
        syf = motion_data[0].syf;
      }

      motion_estimation(oldorgframe[0],neworgframe[0],
                        oldrefframe[0],newrefframe[0],
                        neworg[0],newref[0],
                        sxf,syf,sxb,syb,mbinfo,1,ipflag);

      predict(oldrefframe,newrefframe,predframe,1,mbinfo);
      dct_type_estimation(predframe[0],neworg[0],mbinfo);
      transform(predframe,neworg,mbinfo,blocks);

      putpict(neworg[0]);

      for (k=0; k<mb_height2*mb_width; k++)
      {
        if (mbinfo[k].mb_type & MB_INTRA)
          for (j=0; j<block_count; j++)
            iquant_intra(blocks[k*block_count+j],blocks[k*block_count+j],
                         dc_prec,intra_q,mbinfo[k].mquant);
        else
          for (j=0;j<block_count;j++)
            iquant_non_intra(blocks[k*block_count+j],blocks[k*block_count+j],
                             inter_q,mbinfo[k].mquant);
      }

      itransform(predframe,newref,mbinfo,blocks);
      calcSNR(neworg,newref);
      stats();
    }
    else
    {
      pict_struct = FRAME_PICTURE;

      /* do motion_estimation
       *
       * uses source frames (...orgframe) for full pel search
       * and reconstructed frames (...refframe) for half pel search
       */
#ifdef LTHREAD

      data_args.oldorg = oldorgframe[0];
      data_args.neworg = neworgframe[0];
      data_args.oldref = oldrefframe[0];
      data_args.newref = newrefframe[0];
      data_args.cur = neworg[0];
      data_args.curref = newref[0];
      data_args.sxf = sxf;
      data_args.syf = syf;
      data_args.sxb = sxb;
      data_args.syb = syb;
      data_args.mbi = mbinfo;
      data_args.secondfield = 0;
      data_args.ipflag = 0;

      data_args.reff = oldrefframe;
      data_args.refb = newrefframe;
      data_args.pd_cur = predframe;

      data_args.pred = predframe[0];
      data_args.dte_cur = neworg[0];

      data_args.trfm_cur = neworg;
      data_args.blocks = (short **)blocks;
      
      data_args.prev_mquant = 20;
      data_args.itrfm_cur = newref;

      thread_work_dist();

#else
      motion_estimation(oldorgframe[0],neworgframe[0],
                        oldrefframe[0],newrefframe[0],
                        neworg[0],newref[0],
                        sxf,syf,sxb,syb,mbinfo,0,0);
      predict(oldrefframe,newrefframe,predframe,0,mbinfo);
      dct_type_estimation(predframe[0],neworg[0],mbinfo);
      transform(predframe,neworg,mbinfo,blocks);

      putpict(neworg[0]); 


      for (k=0; k<mb_height*mb_width; k++)
	{
	  if (mbinfo[k].mb_type & MB_INTRA)
	    for (j=0; j<block_count; j++) {
	      
	      int ind = k*block_count+j;	      
	      iquant_intra(blocks[ind],blocks[ind],
			   dc_prec,intra_q,mbinfo[k].mquant);
	    }
	  else
	    for (j=0;j<block_count;j++) {
	      
	      int ind = k*block_count+j;
	      iquant_non_intra(blocks[ind],blocks[ind],
			       inter_q,mbinfo[k].mquant);
	    }
	}
      
      itransform(predframe,newref,mbinfo,blocks);

#endif	
#ifndef QUIET
      calcSNR(neworg,newref);
      stats();
#endif

    }
    sprintf(name,tplref,f+frame0);
    writeframe(name,newref);
  }

  putseqend();

}

