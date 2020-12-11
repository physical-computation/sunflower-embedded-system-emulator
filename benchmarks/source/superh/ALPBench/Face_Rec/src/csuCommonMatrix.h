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

#ifndef MATRIX_INCLUDED
#define MATRIX_INCLUDED

#include "csuCommon.h"

typedef struct {
    int row_dim;  /* Number of rows    */
    int col_dim;  /* Number of columns */
    FTYPE *data;    /* Data stored in standard form: column major */
    FTYPE **cols;
}
matrix;

typedef matrix* Matrix;

Matrix makeMatrix(int row_dim, int col_dim);        /* creates and allocates memory for a matrix */
void freeMatrix(Matrix A);                        /* frees the area allocated to a matrix      */

Matrix makeZeroMatrix(int row_dim, int col_dim);       /* creates, allocates, and initailizes memory for a matrix */
Matrix makeIdentityMatrix(int dim);           /* creates and allocates memory for an identity matrix */

/* ME (Matrix Element) is used to access a matrix element:
*     implements dope vector expansion
*     Remember that a matrix is stored by columns.
*
*  There are two versions of ME avaible and they are toggled with the
*  range check definition below (MATRIX_RANGE_CHECK).  When MATRIX_RANGE_CHECK
*  is enabled it will cause a function to be called that will check that the
*  element to be accessed is in the correct range (determined by ->row_dim
*  and ->col_dim) if it is not a nice error message will be printed indicating
*  file, function and line number where the error occurred and the program will
*  exit.  Range checking is great for debugging however it is also rather expensive
*  (function call and compares) especially within the nested loops of a matrix multiply.
*
*     Matrix A; index i,j
*/

/* #define MATRIX_RANGE_CHECK */
#ifdef MATRIX_RANGE_CHECK
#define  ME( mat , i , j )  ( ( rangeCheck(mat,i,j,__FILE__,__FUNCTION__,__LINE__,#mat))->cols[j][i] )
#else
#define  ME( mat , i , j )  ( (mat)->cols[j][i] )
#endif

Matrix rangeCheck(Matrix mat, int i, int j, const char* file_name, const char* func_name, int line_num, const char* mat_name);

/* creates a new matrix that is the product of A and B */
Matrix multiplyMatrix (const Matrix A, const Matrix B);
/* creates a new matrix that is the product of (A transpose) and B */
Matrix transposeMultiplyMatrixL (const Matrix A, const Matrix B);
/* creates a new matrix that is the product of A and (B transpose) */
Matrix transposeMultiplyMatrixR (const Matrix A, const Matrix B);

/* A - B */
Matrix subtractMatrix (const Matrix A, const Matrix B);
/* A + B */
Matrix addMatrix (const Matrix A, const Matrix B);
/* A += B */
void addMatrixEquals ( Matrix A, const Matrix B);

/* creates a new matrix that is the transpose of A     */
Matrix transposeMatrix(const Matrix A);

/* invert a matrix */
Matrix invertRREF(Matrix);

/* solve a least squares problem - This function returns the solution vector a */
Matrix leastSquares(Matrix A, Matrix b);
Matrix weightedLeastSquares(Matrix A, Matrix b, Matrix W);

/* creates a matrix of consisting of columns col1 to col2 of matrix mat */
Matrix matrixCols(const Matrix mat, int col1, int col2);

/* alocates and fills a duplicate matrix */
Matrix duplicateMatrix(const Matrix mat);

typedef enum {
  octaveFormat = 0x00,
  matlabFormat = 0x01,
  appendToFile = 0x02,

  formatMask   = 0x01
} MatrixSaveMode;

extern MatrixSaveMode asciiFormat;  /* set to zero for octive format otherwize use matlab */
#define  SAVE_MATRIX( mat )  saveMatrixAscii(NULL, #mat, mat, asciiFormat) 
/* saves a matrix to an octive file */
void saveMatrixAscii(const char *fname, const char* label, const Matrix m, MatrixSaveMode format);

/* return a matrix of random elements (from 0.0 to 1.0) */
Matrix makeRandomMatrix (int row_dim, int col_dim);

/* return the sum of the squares of the matrix elements */
FTYPE matrixSumOfSquares (Matrix mat);

void printMatrix(const char* label, Matrix m);

#endif
