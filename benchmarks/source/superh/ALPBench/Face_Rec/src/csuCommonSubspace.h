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


/*
 *  csuCommonSubspace.h
 *
 *  Created by Marcio L. Teixeira on Tue Jun 19 2002.
 *
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

#ifndef CSU_COMMON_SUBSPACE_INCLUDED
#define CSU_COMMON_SUBSPACE_INCLUDED

#include "csuCommon.h"

typedef enum {
  CUTOFF_NONE    = 0,
  CUTOFF_SIMPLE  = 1,
  CUTOFF_ENERGY  = 2,
  CUTOFF_STRETCH = 3,
  CUTOFF_CLASSES = 4
} CutOffMode;

#define DEFAULT_CUTOFF_PERCENT_SIMPLE  60.0
#define DEFAULT_CUTOFF_PERCENT_ENERGY  90.0
#define DEFAULT_CUTOFF_PERCENT_STRETCH 1.0

typedef struct
{
  int numSubjects;
  int numPixels;
  int basisDim;

  Matrix values;
  Matrix basis;
  Matrix mean;

  int useLDA;
  CutOffMode cutOffMode;
  double cutOff;
  int dropNVectors;
} Subspace;

#ifdef THRD
/*#define NUM_THREADS 2*/

struct thread_args {
  Subspace *s;
  ImageList *start_sub;
  ImageList *stop_sub;
  char* imageDirectory;
  Matrix images, vector, smallVector;
  char* name;
  int myid;
  FILE *dbgfile;
};
#endif

void subspaceTrain (Subspace *s, Matrix images, ImageList *srt, int numSubjects, int dropNVectors, CutOffMode cutOffMode, double cutOff, int useLDA, int writeTextInterm);
void writeSubspace (Subspace *s, char *training_filename, char *imageList, int argc, char**argv);
void readSubspace (Subspace *s, const char* trainingFile, int quiet);

void validateBasisIsOrthonormal (Matrix basis, int printlevel);

Matrix centerThenProjectImages (Subspace *s, Matrix images);

void readSubspace (Subspace *s, const char *trainingFile, int quiet);
Matrix readAndProjectImages (Subspace *s, char *imageNamesFile, char *imageDirectory, int *numImages, ImageList **srt);

/* The following are prototypes for functions found in the csuSubspace files. */

/* eigensolvers found in csuSubspaceCVEigen */
int  cvJacobiEigens_32f ( float* A, float* V, float* E, int n, float eps );
int  cvJacobiEigens_64d ( double* A, double* V, double* E, int n, double eps );

/* basis manipulation */
void basis_normalize (Matrix eigenvectors);
void mean_subtract_images (Matrix images, Matrix mean);
Matrix get_mean_image (Matrix images);

/* subspace training functions */
void eigentrain (Matrix *mean, Matrix *eigen_vals, Matrix *eigen_base, Matrix images);
void fisherTrain (Matrix imspca, ImageList *srt, Matrix *fisherBasis, Matrix *fisherValues, int writeTextInterm);

#endif /* CSU_COMMON_SUBSPACE_INCLUDED */
