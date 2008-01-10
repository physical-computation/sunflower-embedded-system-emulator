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


/* global.h, global variables, function prototypes                          */

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
#include "mpeg2enc.h"

/* choose between declaration (GLOBAL undefined)
 * and definition (GLOBAL defined)
 * GLOBAL is defined in exactly one file (mpeg2enc.c)
 */

#ifndef GLOBAL
#define EXTERN extern
#else
#define EXTERN
#endif

/* prototypes of global functions */

/* conform.c */
void range_checks _ANSI_ARGS_((void));
void profile_and_level_checks _ANSI_ARGS_((void));

/* fdctref.c */
void init_fdct _ANSI_ARGS_((void));
void fdct _ANSI_ARGS_((short *block));

/* idct.c */
void idct _ANSI_ARGS_((short *block));
void init_idct _ANSI_ARGS_((void));

/* motion.c */
void motion_estimation _ANSI_ARGS_((unsigned char *oldorg, unsigned char *neworg,
  unsigned char *oldref, unsigned char *newref, unsigned char *cur,
  unsigned char *curref, int sxf, int syf, int sxb, int syb,
  struct mbinfo *mbi, int secondfield, int ipflag));
#ifdef LTHREAD
void ptmotion_estimation _ANSI_ARGS_((unsigned char *oldorg, unsigned char *neworg,
  unsigned char *oldref, unsigned char *newref, unsigned char *cur,
  unsigned char *curref, int sxf, int syf, int sxb, int syb,
  struct mbinfo *mbi, int secondfield, int ipflag, int start_height, int end_height));
#endif

/* mpeg2enc.c */
void error _ANSI_ARGS_((char *text));

/* predict.c */
void predict _ANSI_ARGS_((unsigned char *reff[], unsigned char *refb[],
  unsigned char *cur[3], int secondfield, struct mbinfo *mbi));
#ifdef LTHREAD
void ptpredict _ANSI_ARGS_((unsigned char *reff[], unsigned char *refb[],
  unsigned char *cur[3], int secondfield, struct mbinfo *mbi, int start_height, int end_height));
#endif

/* putbits.c */
void initbits _ANSI_ARGS_((void));
void putbits _ANSI_ARGS_((int val, int n));
void alignbits _ANSI_ARGS_((void));
int bitcount _ANSI_ARGS_((void));
void flushbits _ANSI_ARGS_((int id));
#ifdef LTHREAD
void put_bits _ANSI_ARGS_((int val, int n, int id));
void align_bits _ANSI_ARGS_((int id));
#endif

/* puthdr.c */
void putseqhdr _ANSI_ARGS_((void));
void putseqext _ANSI_ARGS_((void));
void putseqdispext _ANSI_ARGS_((void));
void putuserdata _ANSI_ARGS_((char *userdata));
void putgophdr _ANSI_ARGS_((int frame, int closed_gop));
void putpicthdr _ANSI_ARGS_((void));
void putpictcodext _ANSI_ARGS_((void));
void putseqend _ANSI_ARGS_((void));

/* putmpg.c */
void putintrablk _ANSI_ARGS_((short *blk, int cc));
void putnonintrablk _ANSI_ARGS_((short *blk));
void putmv _ANSI_ARGS_((int dmv, int f_code));
#ifdef LTHREAD
void put_intrablk _ANSI_ARGS_((short *blk, int cc, int id));
void put_nonintrablk _ANSI_ARGS_((short *blk, int id));
void put_mv _ANSI_ARGS_((int dmv, int f_code, int id));
#endif

/* putpic.c */
void putpict _ANSI_ARGS_((unsigned char *frame));
#ifdef LTHREAD
void ptputpict _ANSI_ARGS_((unsigned char *frame, int start_mbh, int end_mbh, \
		int pmq, int id));
void putpichelper _ANSI_ARGS_((unsigned char *frame));
#endif

/* putseq.c */
void putseq _ANSI_ARGS_((void));

/* putvlc.c */
void putDClum _ANSI_ARGS_((int val));
void putDCchrom _ANSI_ARGS_((int val));
void putACfirst _ANSI_ARGS_((int run, int val));
void putAC _ANSI_ARGS_((int run, int signed_level, int vlcformat));
void putaddrinc _ANSI_ARGS_((int addrinc));
void putmbtype _ANSI_ARGS_((int pict_type, int mb_type));
void putmotioncode _ANSI_ARGS_((int motion_code));
void putdmv _ANSI_ARGS_((int dmv));
void putcbp _ANSI_ARGS_((int cbp));
#ifdef LTHREAD
void put_DClum _ANSI_ARGS_((int val,int id));
void put_DCchrom _ANSI_ARGS_((int val,int id));
void put_ACfirst _ANSI_ARGS_((int run, int val,int id));
void put_AC _ANSI_ARGS_((int run, int signed_level, int vlcformat,int id));
void put_addrinc _ANSI_ARGS_((int addrinc,int id));
void put_mbtype _ANSI_ARGS_((int pict_type, int mb_type,int id));
void put_motioncode _ANSI_ARGS_((int motion_code,int id));
void put_dmv _ANSI_ARGS_((int dmv,int id));
void put_cbp _ANSI_ARGS_((int cbp,int id));
#endif

/* quantize.c */
int quant_intra _ANSI_ARGS_((short *src, short *dst, int dc_prec,
  unsigned char *quant_mat, int mquant));
int quant_non_intra _ANSI_ARGS_((short *src, short *dst,
  unsigned char *quant_mat, int mquant));
void iquant_intra _ANSI_ARGS_((short *src, short *dst, int dc_prec,
  unsigned char *quant_mat, int mquant));
void iquant_non_intra _ANSI_ARGS_((short *src, short *dst,
  unsigned char *quant_mat, int mquant));
#ifdef LTHREAD
void ptiquant _ANSI_ARGS_((int start_k, int end_k));
void ptquant _ANSI_ARGS_((int start_k, int end_k));
#endif


/* ratectl.c */
void rc_init_seq _ANSI_ARGS_((void));
void rc_init_GOP _ANSI_ARGS_((int np, int nb));
void rc_init_pict _ANSI_ARGS_((unsigned char *frame));
void rc_update_pict _ANSI_ARGS_((void));
int rc_start_mb _ANSI_ARGS_((void));
int rc_calc_mquant _ANSI_ARGS_((int j));
void vbv_end_of_picture _ANSI_ARGS_((void));
void calc_vbv_delay _ANSI_ARGS_((void));

/* readpic.c */
void readframe _ANSI_ARGS_((char *fname, unsigned char *frame[]));

/* stats.c */
void calcSNR _ANSI_ARGS_((unsigned char *org[3], unsigned char *rec[3]));
void stats _ANSI_ARGS_((void));

/* transfrm.c */
void transform _ANSI_ARGS_((unsigned char *pred[], unsigned char *cur[],
  struct mbinfo *mbi, short blocks[][64]));
void itransform _ANSI_ARGS_((unsigned char *pred[], unsigned char *cur[],
  struct mbinfo *mbi, short blocks[][64]));
void dct_type_estimation _ANSI_ARGS_((unsigned char *pred, unsigned char *cur,
  struct mbinfo *mbi));
#ifdef LTHREAD
void ptdct_type_estimation _ANSI_ARGS_((unsigned char *pred, unsigned char *cur,
  struct mbinfo *mbi, int start_height, int end_height));
void pttransform _ANSI_ARGS_((unsigned char *pred[], unsigned char *cur[],
  struct mbinfo *mbi, short blocks[][64], int start_height, int end_height));
void ptitransform _ANSI_ARGS_((unsigned char *pred[], unsigned char *cur[],
  struct mbinfo *mbi, short blocks[][64], int start_height, int end_height));
#endif

/* writepic.c */
void writeframe _ANSI_ARGS_((char *fname, unsigned char *frame[]));


/* global variables */

EXTERN char version[]
#ifdef GLOBAL
  ="mpeg2encode V1.2, 96/07/19"
#endif
;

EXTERN char author[]
#ifdef GLOBAL
  ="(C) 1996, MPEG Software Simulation Group"
#endif
;

/* zig-zag scan */
EXTERN unsigned char zig_zag_scan[64]
#ifdef GLOBAL
=
{
  0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,
  12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
  35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
  58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
}
#endif
;

/* alternate scan */
EXTERN unsigned char alternate_scan[64]
#ifdef GLOBAL
=
{
  0,8,16,24,1,9,2,10,17,25,32,40,48,56,57,49,
  41,33,26,18,3,11,4,12,19,27,34,42,50,58,35,43,
  51,59,20,28,5,13,6,14,21,29,36,44,52,60,37,45,
  53,61,22,30,7,15,23,31,38,46,54,62,39,47,55,63
}
#endif
;

/* default intra quantization matrix */
EXTERN unsigned char default_intra_quantizer_matrix[64]
#ifdef GLOBAL
=
{
   8, 16, 19, 22, 26, 27, 29, 34,
  16, 16, 22, 24, 27, 29, 34, 37,
  19, 22, 26, 27, 29, 34, 34, 38,
  22, 22, 26, 27, 29, 34, 37, 40,
  22, 26, 27, 29, 32, 35, 40, 48,
  26, 27, 29, 32, 35, 40, 48, 58,
  26, 27, 29, 34, 38, 46, 56, 69,
  27, 29, 35, 38, 46, 56, 69, 83
}
#endif
;

/* non-linear quantization coefficient table */
EXTERN unsigned char non_linear_mquant_table[32]
#ifdef GLOBAL
=
{
   0, 1, 2, 3, 4, 5, 6, 7,
   8,10,12,14,16,18,20,22,
  24,28,32,36,40,44,48,52,
  56,64,72,80,88,96,104,112
}
#endif
;

/* non-linear mquant table for mapping from scale to code
 * since reconstruction levels are not bijective with the index map,
 * it is up to the designer to determine most of the quantization levels
 */

EXTERN unsigned char map_non_linear_mquant[113] 
#ifdef GLOBAL
=
{
0,1,2,3,4,5,6,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,
16,17,17,17,18,18,18,18,19,19,19,19,20,20,20,20,21,21,21,21,22,22,
22,22,23,23,23,23,24,24,24,24,24,24,24,25,25,25,25,25,25,25,26,26,
26,26,26,26,26,26,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,29,
29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,31,31,31,31,31
}
#endif
;

/* picture data arrays */

/* reconstructed frames */
EXTERN unsigned char *newrefframe[3], *oldrefframe[3], *auxframe[3];
/* original frames */
EXTERN unsigned char *neworgframe[3], *oldorgframe[3], *auxorgframe[3];
/* prediction of current frame */
EXTERN unsigned char *predframe[3];
/* 8*8 block data */
EXTERN short (*blocks)[64];
/* intra / non_intra quantization matrices */
EXTERN unsigned char intra_q[64], inter_q[64];
EXTERN unsigned char chrom_intra_q[64],chrom_inter_q[64];
/* prediction values for DCT coefficient (0,0) */
EXTERN int dc_dct_pred[3];
#ifdef LTHREAD
EXTERN int pt_dc_dct_pred[NUM_THREADS][3];
#endif
/* macroblock side information array */
EXTERN struct mbinfo *mbinfo;
/* motion estimation parameters */
EXTERN struct motion_data *motion_data;
/* clipping (=saturation) table */
EXTERN unsigned char *clp;

/* name strings */
EXTERN char id_string[256], tplorg[256], tplref[256];
EXTERN char iqname[256], niqname[256];
EXTERN char statname[256];
EXTERN char errortext[256];

EXTERN FILE *outfile, *statfile; /* file descriptors */
EXTERN int inputtype; /* format of input frames */

EXTERN int quiet; /* suppress warnings */

/* coding model parameters */

EXTERN int N; /* number of frames in Group of Pictures */
EXTERN int M; /* distance between I/P frames */
EXTERN int P; /* intra slice refresh interval */
EXTERN int nframes; /* total number of frames to encode */
EXTERN int frame0, tc0; /* number and timecode of first frame */
EXTERN int mpeg1; /* ISO/IEC IS 11172-2 sequence */
EXTERN int fieldpic; /* use field pictures */

/* sequence specific data (sequence header) */

EXTERN int horizontal_size, vertical_size; /* frame size (pels) */
EXTERN int width, height; /* encoded frame size (pels) multiples of 16 or 32 */
EXTERN int chrom_width,chrom_height,block_count;
EXTERN int mb_width, mb_height; /* frame size (macroblocks) */
EXTERN int width2, height2, mb_height2, chrom_width2; /* picture size */
EXTERN int aspectratio; /* aspect ratio information (pel or display) */
EXTERN int frame_rate_code; /* coded value of frame rate */
EXTERN double frame_rate; /* frames per second */
EXTERN double bit_rate; /* bits per second */
EXTERN int vbv_buffer_size; /* size of VBV buffer (* 16 kbit) */
EXTERN int constrparms; /* constrained parameters flag (MPEG-1 only) */
EXTERN int load_iquant, load_niquant; /* use non-default quant. matrices */
EXTERN int load_ciquant,load_cniquant;


/* sequence specific data (sequence extension) */

EXTERN int profile, level; /* syntax / parameter constraints */
EXTERN int prog_seq; /* progressive sequence */
EXTERN int chroma_format;
EXTERN int low_delay; /* no B pictures, skipped pictures */


/* sequence specific data (sequence display extension) */

EXTERN int video_format; /* component, PAL, NTSC, SECAM or MAC */
EXTERN int color_primaries; /* source primary chromaticity coordinates */
EXTERN int transfer_characteristics; /* opto-electronic transfer char. (gamma) */
EXTERN int matrix_coefficients; /* Eg,Eb,Er / Y,Cb,Cr matrix coefficients */
EXTERN int display_horizontal_size, display_vertical_size; /* display size */


/* picture specific data (picture header) */

EXTERN int temp_ref; /* temporal reference */
EXTERN int pict_type; /* picture coding type (I, P or B) */
EXTERN int vbv_delay; /* video buffering verifier delay (1/90000 seconds) */


/* picture specific data (picture coding extension) */

EXTERN int forw_hor_f_code, forw_vert_f_code;
EXTERN int back_hor_f_code, back_vert_f_code; /* motion vector ranges */
EXTERN int dc_prec; /* DC coefficient precision for intra coded blocks */
EXTERN int pict_struct; /* picture structure (frame, top / bottom field) */
EXTERN int topfirst; /* display top field first */
/* use only frame prediction and frame DCT (I,P,B,current) */
EXTERN int frame_pred_dct_tab[3], frame_pred_dct;
EXTERN int conceal_tab[3]; /* use concealment motion vectors (I,P,B) */
EXTERN int qscale_tab[3], q_scale_type; /* linear/non-linear quantizaton table */
EXTERN int intravlc_tab[3], intravlc; /* intra vlc format (I,P,B,current) */
EXTERN int altscan_tab[3], altscan; /* alternate scan (I,P,B,current) */
EXTERN int repeatfirst; /* repeat first field after second field */
EXTERN int prog_frame; /* progressive frame */

EXTERN int QUANT_THRESHOLD;

#ifndef USE_ICC
#define __declspec(x)
#define align(x)
#define _mm_malloc(x,y) malloc(x)
#define _mm_free(x) free(x)
#endif

#ifdef INT_DCT
EXTERN __declspec(align(16)) short c[8][8];
EXTERN __declspec(align(16)) short ic[8][8];
#endif
