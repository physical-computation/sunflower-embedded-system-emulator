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


/**
File: csuSubspaceProject.c                                                     
Authors: J. Ross Beveridge, David Bolme, Kai She                            
Date: May 24, 2002                                                       
*/

/*
Copyright (c) 2003 Colorado State University

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#define OPENING  "Project Test Images int Subspace and Compute Pairwise Distances."
#define PRINT_MAT_DIM 0
/*
 Purpose: This program projects face images into a subspace and then
 computes the distances between faces with one of several alternative
 distance measures. The subspace is defined in a training file produced
 by csuSubspaceTrain.  The subspace may be either a simple PCA
 subspace, or it may be a PCA followed by LDA subspace. The images are
 specified in a file of image names. These may be grouped either as one
 name per line or several, although for this program there is no
 difference between the two in terms of what is done.  The resulting
 distances are written out to a series of files, one per image, all
 placed in a user specified directory.
*/

/*  CSU INCLUDES */
#include "csuCommon.h"

#if defined(THRD) 
#include "threading.h"
#include <pthread.h>
#endif

/*
 The command line arguments are managed by a single structure.
*/
typedef struct distDirNode {
    struct distDirNode *next;
    char* distDirectory;
    char* distName;
}
DistDirNode;

typedef struct {
    char* trainingFile;
    char* imageNamesFile;
    char* imageDirectory;
    DistDirNode* distList;
    int argc;
    char **argv;
    int lastphase;
    char* ind_name;
    char* of_name;
    char* of_dir;
    char* subspace_name;
}
Arguments;

#ifdef THRD
typedef struct {
  int id;
  Matrix ims;
  Matrix values;
  Matrix distances;
  Matrix optVals;
  int numImages;
  char* distName;
  int startj, endj;
} dist_thrd_args;
#endif

/* ----------------------------------------------------------------------------------- */
/* Remind the user the usage of running this program.
   The command of running the program should be: run and the index of the training sets. 
   All the other sets except the training sets will go to testing sets automatically.
   INPUT:  prog is the excutable program name.
*/
void usage(const char* prog) {
    printf("Usage: %s [OPTIONS] trianingFile imageNamesFile.[list/srt] [distanceDirectory distName]+\n", prog);
    printf("  Parameters:\n");
    printf("     trainingFile ....  Name of file containing subspace generated by training.\n");
    printf("     imageNamesFile...  Name of file containing test images: union of possible probe and gallery sets.\n");
    printf("  List of distance measures and corresponding distance directories: \n");
    printf("     distDirectory ...  Directory where distances files are to be written\n");
    printf("     distName ........  The distance measure to use, must be one of:\n");
    printf("        Euclidean ....  Standard Euclidean Distance, i.e. L2 norm.\n");
    printf("        Cityblock ....  Standard L1 norm.\n");
    printf("        Covariance ...  Sometimes called angle in FERET: dot product after unit normalization.\n");
    printf("        Correlation ..  Normalize images to mean zero and standard deviation one, then take dot product.\n");
    printf("        MahCosine ....  This measure is the cosine of the angle between the two feature vectors in Mahlinobis space.\n");
    printf("        MahL1 ........  L1 norm distance in Mahalanobis space.\n");
    printf("        MahL2 ........  L2 norm distance in Mahalanobis space.\n");
    printf("        YamborAngle ..  This is the same measure as \"MahAngle\" in \n"
           "                        version 4.0 and before.  It has been depricated \n"
	   "                        because the measure is not properly formulated.  \n"
	   "                        MahCosine has replaced it in newer versions. \n");
    printf("        YamborDist ...  This is the distance measure presented as \n"
           "                        \"Mahalinobis Distance\" in Wendy Yambor's \n"
	   "                        Thesis.  It is included here for completness.\n");
    printf("        ldaSoft ......  For LDA only, a variant of L2 use by Wenyi Zhao that weights dimensions by lambda.\n");
    printf("    -imDir <dir>:       image directory. DEFAULT = \".\"\n");
    printf("    -debuglevel <int>:  Level of debug information to display. DEFAULT = 0\n");
    printf("    -quiet:             Turn off all messages. DEFAULT = messages on\n");

    exit(1);
}

void process_command(int argc, char** argv, Arguments* args) {
    int i;
    int param_num = 0;

    /******* Set up default values *******/

    args->argc = argc;
    args->argv = argv;

    args->imageDirectory = strdup (".");

    args->distList = NULL;

    quiet = 0;
    debuglevel = 0;
    args->lastphase = 0;
    args->ind_name = strdup (".");
    args->of_name = strdup (".");
    args->of_dir = strdup (".");
    args->subspace_name=strdup (".");

    for (i = 1; i < argc; i++) {

      /* Catch common help requests */
      if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
      else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }

      /* Read in input directories */
      else if (readOptionString (argc, argv, &i, "-imDir", &(args->imageDirectory)));

      /* other flags */
      else if (readOption       (argc, argv, &i, "-quiet")) { quiet = 1; }
      else if (readOption       (argc, argv, &i, "-lastphase")) { args->lastphase = 1; }
      else if (readOptionString (argc, argv, &i, "-infile", &(args->ind_name)));
      else if (readOptionString (argc, argv, &i, "-outfile", &(args->of_name)));
      else if (readOptionString (argc, argv, &i, "-outdir", &(args->of_dir )));
      else if (readOptionString (argc, argv, &i, "-subspace", &(args->subspace_name)));
      else if (readOptionInt    (argc, argv, &i, "-debuglevel", &debuglevel));

      else if (param_num == 0) {
	args->trainingFile = strdup (argv[i]);
	param_num++;
      } else if (param_num == 1) {
	args->imageNamesFile = strdup (argv[i]);
	param_num++;
      } else if (param_num > 1) {
	DistDirNode* tmp = (DistDirNode*) malloc(sizeof(DistDirNode));
	tmp->next = args->distList;
	tmp->distDirectory = strdup (argv[i]);
	i++;
	tmp->distName = strdup (argv[i]);
	args->distList = tmp;
	param_num++;
      }
    }
    /*printf("in %s ofn %s ofd %s ss %s\n",args->ind_name, args->of_name, args->of_dir, args->subspace_name);*/
    if (param_num < 3)
        usage(argv[0]);

    
    /* Print out the program parameters for appropriate debug level */
    
    DEBUG_INT (1, "Debuging enabled", debuglevel);

    MESSAGE("bef print\n");


  if (debuglevel > 0) 
    {
      printf ("***************** Program Parameters *********************\n");

    MESSAGE("aft 1st msg print\n");

      printf ("imageNamesFile: %s\n", args->imageNamesFile);
      printf ("imageDirectory: %s\n", args->imageDirectory);
    }

    MESSAGE("after all msgs\n");
   
}

/* ===========================================================================
 New distance measure functions. These functions take a single matrix of
 column vectors, each column representing a different image after projection
 into subspace. They also take the column indices of the two images to be
 compared. 
 
 All of these functions return "distances" in the sense that smaller is better.
 So, for example, covariance and correlation are flipped and shifted so that
 zero is the best score between two images.
 
 The last measures also take the variance estimates for the subspace dimension,
 in other words the eigenvalues from the subspace decomposition. These are used
 to normalize measures by variance along the dimensions.
*/

/*  The L1 norm and L2 norms are standard. This code actually does the square root
 for L2, not strictly speaking necessary for a nearest neighbor classifier.
*/
FTYPE distanceCityBlock(const Matrix ims, int i, int j) {
    int k;
    FTYPE sum = 0.0;
    for (k = 0; k < ims->row_dim; k++) {
        sum += ABS((ME(ims, k, i) - ME(ims, k, j)));
    }
    return sum;
}

FTYPE distanceEuclidean(const Matrix ims, int i, int j) {
    int k;
    FTYPE sum = 0.0;
    for (k = 0; k < ims->row_dim; k++) {
        sum += (ME(ims, k, i) - ME(ims, k, j)) * (ME(ims, k, i) - ME(ims, k, j));
    }
    return sqrt(sum) ;
}


/* This is the standard covariance definition, it is the cosine
 of the angle between the two vectors. When the vectors are 
 identical, it is 1.0, when they are orthogonal, it is zero. 
 
 However, in order produce a distance, this function returns
 one minus the covariance. 
*/

FTYPE distanceCovariance(const Matrix ims, int i, int j) {
    int k;
    FTYPE magi = 0.0, magj = 0.0, dot = 0.0;

    for (k = 0; k < ims->row_dim; k++) {
        magi += ME(ims, k, i) * ME(ims, k, i);
        magj += ME(ims, k, j) * ME(ims, k, j);
        dot += ME(ims, k, i) * ME(ims, k, j);
    }
    return (1.0 - (dot / (sqrt(magi) * sqrt(magj))));
}

/**
    This is a standard correlation measure between two vectors with
 one major caveat.  Because all subsequent processing treats these
 measures as distance, the correlation mapped into the range 0 to 2
 as follows:
  correlation  1  goes to 0
  correlation -1  goes to 2.
*/

FTYPE distanceCorrelation(const Matrix ims, int i, int j) {
    int k;
    FTYPE n;
    FTYPE mui = 0.0, muj = 0.0, stdi = 0.0, stdj = 0.0, dot = 0.0, corr;

    n = (FTYPE) ims->row_dim;
    for (k = 0; k < n; k++) {
        mui += ME(ims, k, i);
        muj += ME(ims, k, j);
    }
    mui = mui / n;
    muj = muj / n;

    for (k = 0; k < n; k++) {
        stdi += (ME(ims, k, i) - mui) * (ME(ims, k, i) - mui);
        stdj += (ME(ims, k, j) - muj) * (ME(ims, k, j) - muj);
        dot += (ME(ims, k, i) - mui) * (ME(ims, k, j) - muj);
    }
    stdi = sqrt((1.0 / (n - 1.0)) * stdi);
    stdj = sqrt((1.0 / (n - 1.0)) * stdj);

    corr = ( 1.0 / (n - 1.0)) * (dot / (stdi * stdj));

    return ( 1.0 - corr);
}

/* This is what gets called Mahalanobis distance in the FERET evaluations. It
 is somewhat ill named. It is closer to a covariance without normalization
 by length and with a correction applied to each dimension. The correction
 is indeed the scale factor required to tranform into the space of unit
 variance in all dimensions. Hence, lacking a better shorthand term, this
 is called Mahalanobis Angle.
*/
FTYPE distanceYamborAngle(Matrix ims, int i, int j, Matrix values) {
    int k;
    FTYPE sum = 0.0;
    FTYPE sum_i = 0.0;
    FTYPE sum_j = 0.0;
    for (k = 0;k < ims->row_dim;k++) {
        sum_i += ME(ims, k, i) * ME(ims, k, i);
        sum_j += ME(ims, k, j) * ME(ims, k, j);
    }
    for (k = 0; k < ims->row_dim; k++) {
        sum -= ME(ims, k, i) / sqrt(sum_i) * (ME(ims, k, j) / sqrt(sum_j)) * (1 / (sqrt(ME(values, k, 0))));
    }
    return sum;
}

/* This is what gets called Mahalanobis distance in the FERET evaluations. It
 is somewhat ill named. It is closer to a covariance without normalization
 by length and with a correction applied to each dimension. The correction
 is indeed the scale factor required to tranform into the space of unit
 variance in all dimensions. Hence, lacking a better shorthand term, this
 is called Mahalanobis Angle.
*/

#if 1

/* The number of parameters is changed to serve multithreading purposes */
#ifdef SSE2

FTYPE distanceMahCosine(Matrix ims, int i, int j, Matrix values,
			Matrix vi, Matrix vj, Matrix optVals) {

    /*static Matrix optVals = NULL;*/
    int k;
    FTYPE sum_i = 0.0;
    FTYPE sum_j = 0.0;
    FTYPE scale_i = 0.0;
    FTYPE scale_j = 0.0;
    FTYPE sum = 0.0;
    __declspec(align(16)) static const FTYPE const0[2] = {0.0, 0.0};

    FTYPE *vi_p = &ME(vi,0,0), *vj_p = &ME(vj,0,0);
    FTYPE *ims_i_p =  &ME(ims,0,i), *ims_j_p =  &ME(ims,0,j);
    FTYPE *optval_p = &ME(optVals, 0, 0);
    FTYPE *val_p = &ME(values, 0, 0);
    int vi_row_dim = vi->row_dim;

    /* Transform the vector into Mahalanobis space */
    __asm {
      mov     eax, [ims_i_p]    ;
      mov     ecx, [vi_row_dim] ;
      mov     ebx, [ims_j_p]    ;
      mov     edx, [optval_p]   ;
      mov     esi, [vi_p]       ;
      mov     edi, [vj_p]       ;
      and     ecx, 0xfffffffe   ;
      sal     ecx, 3            ;
      add     ecx, esi          ; /* end point for loop */
      movapd  xmm0, [const0]    ; /* running sums */
      movapd  xmm1, [const0]    ; /* xmm0:sum_i, xmm1:sum_j*/
      
    dist_mah_cosine1:
      movapd  xmm2, [eax]       ; /* load ims_i */
      movapd  xmm4, [edx]       ; /* load optval */
      movapd  xmm3, [ebx]       ; /* load ims_j */

      mulpd   xmm2, xmm4        ; /* ims_i * optval */
      mulpd   xmm3, xmm4        ; /* ims_j * optval */
      movapd  [esi], xmm2       ;
      movapd  [edi], xmm3       ;
      mulpd   xmm2, xmm2        ; 
      mulpd   xmm3, xmm3        ; 

      addpd   xmm0, xmm2        ;
      addpd   xmm1, xmm3        ;
      add     eax, 16           ;
      add     ebx, 16           ;
      add     edx, 16           ;
      add     esi, 16           ;
      add     edi, 16           ;
      cmp     ecx, esi          ;
      jne     dist_mah_cosine1  ;     

      mov     ecx, [vi_row_dim] ;
      and     ecx, 0x1          ;
      cmp     ecx, 0x0          ;
      je      dist_mah_consine1_end  ;

      movsd  xmm2, [eax]       ; /* load ims_i */
      movsd  xmm4, [edx]       ; /* load optval */
      movsd  xmm3, [ebx]       ; /* load ims_j */

      mulsd   xmm2, xmm4        ; /* ims_i * optval */
      mulsd   xmm3, xmm4        ; /* ims_j * optval */
      movsd  [esi], xmm2       ;
      movsd  [edi], xmm3       ;
      mulsd   xmm2, xmm2        ; 
      mulsd   xmm3, xmm3        ; 

      addsd   xmm0, xmm2        ;
      addsd   xmm1, xmm3        ;    

    dist_mah_consine1_end:

      ;mov     [ims_i_p], eax    ;
      ;mov     [ims_j_p], ebx    ;
      ;mov     [optval_p], edx    ;
      ;mov     [vi_p], esi    ;
      ;mov     [vj_p], edi    ;
      
      pshufd  xmm2, xmm0, 0xe   ; /* reduction */
      pshufd  xmm3, xmm1, 0xe   ;
      addsd   xmm0, xmm2        ;
      addsd   xmm1, xmm3        ;
      movsd   [sum_i], xmm0     ;
      movsd   [sum_j], xmm1     ;
    }

   scale_i = 1.0/sqrt(sum_i);
   scale_j = 1.0/sqrt(sum_j);
   vi_p = &ME(vi,0,0);
   vj_p = &ME(vj,0,0);
  
   /* Unit length the vectors and find the dot product (cos of angle) */
   
   __asm {
      mov     ecx, [vi_row_dim] ;
      mov     eax, [vi_p]       ;
      mov     ebx, [vj_p]       ;
      mov     edx, ecx          ;
      and     edx, 0x1          ;
      and     ecx, 0xfffffffe   ;
      sal     ecx, 3            ;
      add     ecx, eax          ; /* end point for loop */
      movapd  xmm0, [const0]    ; /* running sums */
      
   dist_mah_cosine2:
      movapd  xmm2, [eax]       ; /* load vi */
      movapd  xmm3, [ebx]       ; /* load vj */
      
      mulpd   xmm2, xmm3        ; /* vi * vj */
      addpd   xmm0, xmm2        ; /* accumulate */

      add     eax, 16           ;
      add     ebx, 16           ;
      cmp     ecx, eax          ;
      jne     dist_mah_cosine2  ;     

      cmp     edx, 0x0          ;
      je      dist_mah_consine2_end ;

      movsd   xmm2, [eax]       ;
      movsd   xmm3, [ebx]       ;
      mulsd   xmm2, xmm3        ;
      addsd   xmm0, xmm2        ;

   dist_mah_consine2_end:
      pshufd  xmm2, xmm0, 0xe   ; /* reduction */
      addsd   xmm0, xmm2        ;
      movsd   [sum], xmm0     ;
   }
   
   sum = sum * scale_i * scale_j;
   
   /* return the negetive similarity or distance */
   return -sum;
}

#else /* Non-SIMD code */
FTYPE distanceMahCosine(Matrix ims, int i, int j, Matrix values,
			Matrix vi, Matrix vj, Matrix optVals) {
    int k;
    FTYPE sum_i = 0.0;
    FTYPE sum_j = 0.0;
    FTYPE scale_i = 0.0;
    FTYPE scale_j = 0.0;
    FTYPE sum = 0.0;
    static const FTYPE const0[2] = {0.0, 0.0};

    FTYPE *vi_p = &ME(vi,0,0), *vj_p = &ME(vj,0,0);
    FTYPE *ims_i_p =  &ME(ims,0,i), *ims_j_p =  &ME(ims,0,j);
    FTYPE *optval_p = &ME(optVals, 0, 0);
    FTYPE *val_p = &ME(values, 0, 0);

    /* Transform the vector into Mahalanobis space */
    for(k = 0; k < vi->row_dim-1; k+=2){
      FTYPE res_i, res_j;
      
      res_i = (*ims_i_p++)  *  (*optval_p);
      *vi_p++ = res_i;
      sum_i += res_i * res_i;



      res_j = (*ims_j_p++)  *  (*optval_p++);
      *vj_p++ = res_j;
      sum_j += res_j * res_j;


      
      /* 2nd iteration of unrolled loop */

      res_i = (*ims_i_p++)  *  (*optval_p);
      *vi_p++ = res_i;
      sum_i += res_i * res_i;



      res_j = (*ims_j_p++)  *  (*optval_p++);
      *vj_p++ = res_j;
      sum_j += res_j * res_j;


      /* Original code
        ME(vi,k,0) = ME(ims,k,i)*ME(optVals,k,0);
        sum_i += ME(vi,k,0)*ME(vi,k,0);

        ME(vj,k,0) = ME(ims,k,j)*ME(optVals,k,0);
        sum_j += ME(vj,k,0)*ME(vj,k,0);
      */

    }
  
    if (k < vi->row_dim) {      /* remainder of the loop */
      FTYPE res_i, res_j;

      res_i = (*ims_i_p++)  *  (*optval_p);
      *vi_p++ = res_i;
      sum_i += res_i * res_i;



      res_j = (*ims_j_p++)  *  (*optval_p++);
      *vj_p++ = res_j;
      sum_j += res_j * res_j;

    }

    
    scale_i = 1.0/sqrt(sum_i);
    scale_j = 1.0/sqrt(sum_j);


    vi_p = &ME(vi,0,0);
    vj_p = &ME(vj,0,0);

    /* Unit length the vectors and find the dot product (cos of angle) */
    for(k = 0; k < vi->row_dim-1; k+=2){

      sum += (*vi_p++) * (*vj_p++);
      sum += (*vi_p++) * (*vj_p++);

      /* Original code 
	 sum += ME(vi,k,0)*ME(vj,k,0);
      */
    }

    if (k < vi->row_dim) {     /* remainder of the loop */
      sum += (*vi_p++) * (*vj_p++);
    }

    sum = sum * scale_i * scale_j;

    /* return the negetive similarity or distance */
    return -sum;
}
#endif /* End non-SSE2 */

#else /* Original definition */

FTYPE distanceMahCosine(Matrix ims, int i, int j, Matrix values) {
    static int first_call = 1;
    static Matrix optVals = NULL;
    int k;
    Matrix vi = makeMatrix(ims->row_dim,1);
    Matrix vj = makeMatrix(ims->row_dim,1);
    FTYPE sum_i = 0.0;
    FTYPE sum_j = 0.0;
    FTYPE scale_i = 0.0;
    FTYPE scale_j = 0.0;
    FTYPE sum = 0.0;
    
    if(first_call){
        first_call = 0;
        optVals = duplicateMatrix(values);
        for(k = 0; k < optVals->row_dim; k++){
            ME(optVals,k,0) = 1.0/sqrt(ME(values, k, 0));
        }
    }

    /* Transform the vector into Mahalanobis space */
    for(k = 0; k < vi->row_dim; k++){
        ME(vi,k,0) = ME(ims,k,i)*ME(optVals,k,0);
        sum_i += ME(vi,k,0)*ME(vi,k,0);
        ME(vj,k,0) = ME(ims,k,j)*ME(optVals,k,0);
        sum_j += ME(vj,k,0)*ME(vj,k,0);
    }
    
    scale_i = 1.0/sqrt(sum_i);
    scale_j = 1.0/sqrt(sum_j);

    /* Unit length the vectors and find the dot product (cos of angle) */
    for(k = 0; k < vi->row_dim; k++){
        sum += ME(vi,k,0)*ME(vj,k,0);
    }
    sum = sum * scale_i * scale_j;

    /* Free the temporary vectors. */
    freeMatrix(vi);
    freeMatrix(vj);

    /* return the negetive similarity or distance */
    return -sum;
}
#endif
/*    L2 norm distance in Mahalanobis space
 */
FTYPE distanceMahL2(const Matrix ims, int i, int j, const Matrix values) {
    int k;
    FTYPE sum = 0.0;
    static int first_call = 1;
    static Matrix optVals = NULL;

    if(first_call){
        first_call = 0;
        optVals = duplicateMatrix(values);
        for(k = 0; k < optVals->row_dim; k++){
            ME(optVals,k,0) = 1.0/ME(values, k, 0);
        }
    }

    for (k = 0; k < ims->row_dim; k++) {
        sum += (ME(ims, k, i) - ME(ims, k, j)) * (ME(ims, k, i) - ME(ims, k, j)) * ME(optVals,k,0);
    }

    return sqrt(sum);
}


FTYPE distanceYamborDistance(const Matrix ims, int i, int j, const Matrix values) {
    int k;
    FTYPE sum = 0.0;
    static int first_call = 1;
    static Matrix optVals = NULL;

    if(first_call){
        first_call = 0;
        optVals = duplicateMatrix(values);
        for(k = 0; k < optVals->row_dim; k++){
            ME(optVals,k,0) = 1.0/ME(values, k, 0);
        }
    }

    for (k = 0; k < ims->row_dim; k++) {
        sum += ME(ims, k, i)*ME(ims, k, j)*ME(optVals,k,0);
    }

    return -sum;
}

/*    L1 norm distance in Mahalanobis space
 */
FTYPE distanceMahL1(const Matrix ims, int i, int j, const Matrix values) {
    int k;
    FTYPE sum = 0.0;
    static int first_call = 1;
    static Matrix optVals = NULL;

    if(first_call){
        first_call = 0;
        optVals = duplicateMatrix(values);
        for(k = 0; k < optVals->row_dim; k++){
            ME(optVals,k,0) = 1.0/sqrt(ME(values, k, 0));
        }
    }
    
    for (k = 0; k < ims->row_dim; k++) {
        sum += ABS(ME(ims, k, i) - ME(ims, k, j)) * ME(optVals,k,0);
    }
    return sum;
}

/* The soft distance measure proposed by WenYi Zhao is essentially the L2 norm
 with each axis weighted by the associated eigen value, i.e. variance, raised
 to the the power 0.2.  This is not obvious, but considerable discussion
 of this design appears in WenYi's Dissertation.
*/

FTYPE distanceLDASoft(Matrix ims, int i, int j, Matrix values) {
    int k;
    FTYPE sum = 0.0;
    for (k = 0; k < ims->row_dim; k++) {
        sum += pow(ME(values, k, 0), 0.2) * (ME(ims, k, i) - ME(ims, k, j)) * (ME(ims, k, i) - ME(ims, k, j));
    }
    return sum;
}


char** getNameByIndex(ImageList **srt, int numImages) {
    int i;
    ImageList *subject, *replicate;

    char** nameByIndex;

    /*  Allocate space for the image names for each index in images matrix */
    nameByIndex = (char**) malloc(sizeof(char*) * numImages);
    assert(nameByIndex);
    for (i = 0; i < numImages; i++) {
        nameByIndex[i] = (char*) malloc(sizeof(char) * FILE_LINE_LENGTH);
        assert(nameByIndex[i]);
    }

    /*  Move across columns and down rows of subject replicates table constructing
     an array of image file names indexed by the same index as the images matrix. */
    i = 0;
    for (subject = *srt; subject; subject = subject->next_subject) {
        for (replicate = subject; replicate; replicate = replicate->next_replicate) {
            if ((i != replicate->imageIndex) || (i == numImages)) {
                fprintf(stderr, "Error: replicate indices off or out of bounds.\n");
                exit(1);
            }
            strcpy(nameByIndex[i], replicate->filename);
            i++;
        }
    }
    return nameByIndex;
}

Matrix computeDistances(const Matrix ims, const Matrix values, int numImages, char* distName) {
    int i, j;
    Matrix distances = makeMatrix(numImages, numImages);

    if (strcmp(distName, "Euclidean") == 0) {
        for (i = 0; i < numImages; i++) {
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceEuclidean(ims, i, j);
            }
        }
    } else if ((strcmp(distName, "Cityblock") == 0) || (strcmp(distName, "CityBlock") == 0)) {
        for (i = 0; i < numImages; i++) {
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceCityBlock(ims, i, j);
            }
        }
    } else if (strcmp(distName, "Covariance") == 0) {
        for (i = 0; i < numImages; i++) {
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceCovariance(ims, i, j);
            }
        }
    } else if (strcmp(distName, "Correlation") == 0) {
        for (i = 0; i < numImages; i++) {
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceCorrelation(ims, i, j);
            }
        }
    } else if (strcmp(distName, "YamborAngle") == 0) {
        for (i = 0; i < numImages; i++) {
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceYamborAngle(ims, i, j, values);
            }
        }
    } else if (strcmp(distName, "MahCosine") == 0) {
      Matrix vi = makeMatrix(ims->row_dim,1);
      Matrix vj = makeMatrix(ims->row_dim,1);
      Matrix optVals = duplicateMatrix(values);
      for(i = 0; i < optVals->row_dim; i++){
	ME(optVals,i,0) = 1.0/sqrt(ME(values, i, 0));
      }
      for (i = 0; i < numImages; i++) {
	writeProgress("Computing distances for MahCosine", i,numImages);
	for (j = 0; j < numImages; j++) {
	  ME(distances, i, j) = distanceMahCosine(ims, i, j, values, vi, vj, optVals);
	}
      }
    } else if (strcmp(distName, "MahL1") == 0) {
        for (i = 0; i < numImages; i++) {
            writeProgress("Computing distances for MahL1", i,numImages);
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceMahL1(ims, i, j, values);
            }
        }
    } else if (strcmp(distName, "MahL2") == 0) {
        for (i = 0; i < numImages; i++) {
            writeProgress("Computing distances for MahL2", i,numImages);
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceMahL2(ims, i, j, values);
            }
        }
    } else if (strcmp(distName, "YamborDistance") == 0) {
        for (i = 0; i < numImages; i++) {
            writeProgress("Computing distances for Windy", i,numImages);
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceYamborDistance(ims, i, j, values);
            }
        }
    } else if ((strcmp(distName, "ldaSoft") == 0) || (strcmp(distName, "LDASoft") == 0)) {
        for (i = 0; i < numImages; i++) {
            for (j = 0; j < numImages; j++) {
                ME(distances, i, j) = distanceLDASoft(ims, i, j, values);
            }
        }
    } else {
        fprintf(stderr, "Error: The distance measure %s is not implemented! \n", distName);
        exit(1);
    }

    return distances;
}
#ifdef THRD
void *dist_thrd_work(void* thrd_args) {
  dist_thrd_args *thrd_data = (dist_thrd_args*) thrd_args;
  Matrix ims = thrd_data->ims;
  Matrix values = thrd_data->values;
  int numImages = thrd_data->numImages;
  char*distName = thrd_data->distName;
  Matrix distances = thrd_data->distances;
  int startj = thrd_data->startj;
  int endj = thrd_data->endj;
  Matrix optVals = thrd_data->optVals;
  Matrix vi, vj;
  int j;
  

  if (strcmp(distName, "Euclidean") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceEuclidean(ims, numImages, j);
    }
  } else if ((strcmp(distName, "Cityblock") == 0) || (strcmp(distName, "CityBlock") == 0)) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceCityBlock(ims, numImages, j);
    }
  } else if (strcmp(distName, "Covariance") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceCovariance(ims, numImages, j);
    }
  } else if (strcmp(distName, "Correlation") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceCorrelation(ims, numImages, j);
            }
  } else if (strcmp(distName, "YamborAngle") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceYamborAngle(ims, numImages, j, values);
            }
  } else if (strcmp(distName, "MahCosine") == 0) {
    vi = makeMatrix(ims->row_dim, 1);
    vj = makeMatrix(ims->row_dim, 1);
    
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceMahCosine(ims, numImages, j, values,
					      vi, vj, optVals);
    }
  } else if (strcmp(distName, "MahL1") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceMahL1(ims, numImages, j, values);
    }
  } else if (strcmp(distName, "MahL2") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceMahL2(ims, numImages, j, values);
    }
  } else if (strcmp(distName, "YamborDistance") == 0) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceYamborDistance(ims, numImages, j, values);
            }
  } else if ((strcmp(distName, "ldaSoft") == 0) || (strcmp(distName, "LDASoft") == 0)) {
    for (j = startj; j < endj; j++) {
      ME(distances, j, 0) = distanceLDASoft(ims, numImages, j, values);
    }
  } else {
    fprintf(stderr, "Error: The distance measure %s is not implemented! \n", distName);
    exit(1);
  } 
  return NULL;
}

#endif

Matrix computeOneDistances(const Matrix ims, const Matrix values, int numImages, char* distName) {

    Matrix distances = makeMatrix(numImages, 1);



#ifdef THRD
    int remainder, chunk_size, tmp, rc, status, t, k, is_mahcosine=0;
    dist_thrd_args thrd_data_array[NUM_THREADS];
    Matrix optVals;

    pthread_attr_t attr;
#if (NUM_THREADS>1)
    pthread_t thread[NUM_THREADS-1];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif

    if (strcmp(distName, "MahCosine") == 0) {
      is_mahcosine = 1;
      optVals = duplicateMatrix(values);
      for(k = 0; k < optVals->row_dim; k++){
	ME(optVals,k,0) = 1.0/sqrt(ME(values, k, 0));
      }
    }

    chunk_size = numImages/NUM_THREADS;
    remainder = numImages%NUM_THREADS;

    for(t=0;t < NUM_THREADS;t++) {
      if (t==0)
	thrd_data_array[0].startj = 0;
      else
	thrd_data_array[t].startj = thrd_data_array[t-1].endj;
      
      tmp = thrd_data_array[t].startj + chunk_size;
      
      if (remainder) {
	tmp++;
	remainder--;
      }
      
      thrd_data_array[t].endj = tmp;
      
      thrd_data_array[t].id = t;
      thrd_data_array[t].ims = ims;
      thrd_data_array[t].values = values;
      thrd_data_array[t].numImages = numImages;
      thrd_data_array[t].distName = distName;
      thrd_data_array[t].distances = distances;
      if (is_mahcosine)
	thrd_data_array[t].optVals = optVals;

      /* create threads */
      if (t!=NUM_THREADS-1) {
#if (NUM_THREADS>1)
	rc = pthread_create(&thread[t], &attr, dist_thrd_work, 
			    (void*) &thrd_data_array[t]); 

	if (rc) {
	  printf("ERROR; return code from pthread_create() is %d\n", rc);
	  exit(-1);
	}
#endif
      }
    
    } /* for */

    
    dist_thrd_work(&thrd_data_array[NUM_THREADS-1]);
    
#if (NUM_THREADS>1)
    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0;t < NUM_THREADS-1;t++) {
	rc = pthread_join(thread[t], (void **)&status);
	if (rc)	  {
	  printf("ERROR; return code from pthread_join() is %d\n", rc);
	  exit(-1);
	}
    }
#endif



#else
    int j;
    Matrix optVals, vi, vj;

    if (strcmp(distName, "Euclidean") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceEuclidean(ims, numImages, j);
            }
    } else if ((strcmp(distName, "Cityblock") == 0) || (strcmp(distName, "CityBlock") == 0)) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceCityBlock(ims, numImages, j);
            }
    } else if (strcmp(distName, "Covariance") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceCovariance(ims, numImages, j);
            }
    } else if (strcmp(distName, "Correlation") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceCorrelation(ims, numImages, j);
            }
    } else if (strcmp(distName, "YamborAngle") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceYamborAngle(ims, numImages, j, values);
            }
    } else if (strcmp(distName, "MahCosine") == 0) {
            optVals = duplicateMatrix(values);
	    vi = makeMatrix(ims->row_dim, 1);
	    vj = makeMatrix(ims->row_dim, 1);
	    for(j = 0; j < optVals->row_dim; j++)
	      ME(optVals,j,0) = 1.0/sqrt(ME(values, j, 0)); 

            for (j = 0; j < numImages; j++) {
	      ME(distances, j, 0) = distanceMahCosine(ims, numImages, j, values,
						      vi, vj, optVals);
            }
    } else if (strcmp(distName, "MahL1") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceMahL1(ims, numImages, j, values);
            }
    } else if (strcmp(distName, "MahL2") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceMahL2(ims, numImages, j, values);
            }
    } else if (strcmp(distName, "YamborDistance") == 0) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceYamborDistance(ims, numImages, j, values);
            }
    } else if ((strcmp(distName, "ldaSoft") == 0) || (strcmp(distName, "LDASoft") == 0)) {
            for (j = 0; j < numImages; j++) {
                ME(distances, j, 0) = distanceLDASoft(ims, numImages, j, values);
            }
    } else {
        fprintf(stderr, "Error: The distance measure %s is not implemented! \n", distName);
        exit(1);
    }
#endif
    return distances;
}

void writeDistancesForImage(char* distDirectory, char* filename, const Matrix distances, 
			    int col, char** names) {
    char name[MAX_FILENAME_LENGTH];
    FILE *file;
    int i;

    sprintf(name, "%s/%s", distDirectory, filename);
    file = fopen(name, "w");
    if (!file) {
        printf("Error: could not open file <%s>\n", name);
        exit(1);
    }
    for (i = 0; i < distances->row_dim; i++) {
        fprintf(file, "%s\t%.16e\n", names[i], ME(distances, i, col));
    }
    fclose(file);
}

void writeMatrix(Matrix m, FILE* fd) {
  if (!fwrite(m, sizeof(matrix), 1, fd)) 
    assert(0&&"write error");
  printf("wrote matrix of size %d\n",sizeof(matrix));
  if (!fwrite(m->data, sizeof(FTYPE), m->row_dim*m->col_dim, fd)) 
    assert(0&&"write error");
  printf("wrote matrix of size %d\n",sizeof(FTYPE)*m->row_dim*m->col_dim);

}

Matrix readMatrix(FILE* fd) {
  Matrix ret = (Matrix) malloc(sizeof(matrix));

  fread(ret, sizeof(matrix), 1, fd);
  ret = makeMatrix(ret->row_dim, ret->col_dim);
  fread(ret->data, sizeof(FTYPE), ret->row_dim*ret->col_dim, fd);

  return ret;
}

/* ===========================================================================
   MAIN
 
   The arguments are processed and then the subspace and related information is
   read from the training file written by csuSubspaceTrain.  The subspace basis
   is read into a matrix. If the basis vectors are for a PCA subspace, then the 
   basis vectors are tested for orthonormality. While this should not be 
   necessary, it is a prudent check to see that nothing has gone wrong either 
   in the training phase or in the transcription of the subspace basis from the
   training code to the testing code. 
 
   Once the training information is read, then the images specified in the 
   imageNamesFile are read into the images matrix. This matrix is then mean 
   centered using the mean, or centroid, associated with the training data. 
   Next, the images are projected into subspace and the distances between all
   pairs of images are computed. Finally, these distances are written to files,
   one per image.
*/

int main(int argc, char *argv[]) {
  int i;
  Arguments args;
  int numImages;
#ifdef THRD
  int numSubjs;
#endif
  ImageList *srt;
  Matrix subspims, distances;
  Subspace subspace;
  char **nameByIndex;
  DistDirNode* ddn;

  FILE *sfile;
  int ind = 0;


  MESSAGE(OPENING);
  MESSAGE(VERSION);
  process_command(argc, argv, &args);

  if (args.ind_name) ind = 1;

  if (!args.lastphase) {

    readSubspace (&subspace, args.trainingFile, quiet);
    MESSAGE1ARG("Reading image data from directory %s and projecting onto the new basis.", args.imageDirectory);
    subspims = readAndProjectImages(&subspace, args.imageNamesFile, args.imageDirectory, &numImages, &srt);

    sfile = fopen("subspace.dat","w");
    fwrite(&subspace,sizeof(subspace),1,sfile);
    writeMatrix(subspace.values, sfile);
    writeMatrix(subspace.basis, sfile);
    writeMatrix(subspace.mean, sfile);
    writeMatrix(subspims, sfile);
    fclose(sfile);

  } else { /*last phase*/
    char subspace_file[256];
    
    sprintf(subspace_file,"%s/subspace.dat",args.subspace_name);
    sfile = fopen(subspace_file,"r");
    
    fread(&subspace, sizeof(subspace), 1, sfile);
    subspace.values = readMatrix(sfile);
    subspace.basis = readMatrix(sfile);
    subspace.mean = readMatrix(sfile);
    subspims = readMatrix(sfile);

    if (PRINT_MAT_DIM) {
      printf("subspace.values rom_dim: %d col_dim: %d\n", subspace.values->row_dim,
	     subspace.values->col_dim);
      printf("subspace.basis rom_dim: %d col_dim: %d\n", subspace.basis->row_dim,
	     subspace.basis->col_dim);
      printf("subspace.mean rom_dim: %d col_dim: %d\n", subspace.mean->row_dim,
	     subspace.mean->col_dim);
      printf("subspims rom_dim: %d col_dim: %d\n", subspims->row_dim,
	     subspims->col_dim);
    }

#ifdef THRD
    srt = getImageNames(args.imageNamesFile, &numImages, &numSubjs);
#else
    srt = getImageNames(args.imageNamesFile, &numImages);
#endif

    if (ind) {
      char img_file_name[256];
      Matrix vector = makeMatrix(subspace.numPixels, 1);
      Matrix smallVector;
      int j;

      sprintf(img_file_name, "%s/%s",args.imageDirectory, args.ind_name);
      readFile(img_file_name, 0, vector);

#ifdef THRD
      smallVector = thrd_centerthenproject(&subspace, vector);
#else
      smallVector = centerThenProjectImages(&subspace, vector);
#endif

      if (PRINT_MAT_DIM)
	printf("smallVector rom_dim: %d col_dim: %d\n", smallVector->row_dim,
	       smallVector->col_dim);     

      for (j = 0; j < smallVector->row_dim; j++) {
	ME(subspims, j, numImages) = ME(smallVector, j, 0);
      }

    };

    nameByIndex = getNameByIndex(&srt, numImages);

    for (ddn = args.distList; ddn != NULL; ddn = ddn->next) {
      if (!ind) {
	MESSAGE1ARG("Computing distances with distance measure %s.", 
		    ddn->distName);
	distances = computeDistances(subspims, subspace.values, numImages, 
				     ddn->distName);
	
	MESSAGE2ARG("Writing distance files for %d test images to directory %s", 
		    numImages, ddn->distDirectory);
	for (i = 0; i < numImages; i++) {
	  writeProgress("Writing distance files", i,numImages);
	  writeDistancesForImage(ddn->distDirectory, nameByIndex[i], distances, 
				 i, nameByIndex);
	}
	freeMatrix(distances);
      } else {


	distances = computeOneDistances(subspims, subspace.values, numImages,
					ddn->distName);

	printf("writing to %s/%s\n",args.of_dir, args.of_name);
	writeDistancesForImage(args.of_dir, args.of_name, distances, 
			       0, nameByIndex);
	
	if (PRINT_MAT_DIM)
	  printf("distances rom_dim: %d col_dim: %d\n", distances->row_dim,
		 distances->col_dim);     
	
      }
    }
  }

  return 0;
}
