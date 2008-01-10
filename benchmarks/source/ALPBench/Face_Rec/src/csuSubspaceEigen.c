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

#include "csuCommon.h"

/*
  eigen_verify
 
  Verify properties of the eigen vectors.
 
  The eigenbasis should be ortonormal: R'*R - I == 0
  The basis should be decomposed such that: MR - RD == 0
  returns true if tests fail.
*/
int eigen_verify(Matrix M, Matrix lambda, Matrix R) {
    Matrix RtR = transposeMultiplyMatrixL(R, R);
    Matrix identity = makeIdentityMatrix(R->col_dim);

    Matrix MR = multiplyMatrix(M, R);
    Matrix D = makeIdentityMatrix(lambda->row_dim);
    Matrix RD;
    Matrix test;
    int i, j;
    int failed = 0;
    const double tol = 1.0e-7;

    for (i = 0; i < lambda->row_dim; i++) {
        ME(D, i, i) = ME(lambda, i, 0);
    }
    RD = multiplyMatrix(R, D);
    freeMatrix(D);

    DEBUG(2, "Checking orthogonality of eigenvectors");
    test = subtractMatrix(RtR, identity);
    freeMatrix(RtR);
    freeMatrix(identity);

    for (i = 0; i < test->row_dim; i++) {
        for (j = 0; j < test->col_dim; j++) {
            if (!EQUAL_ZERO(ME(test, i, j), tol)) {
                failed = 1;
                MESSAGE("Eigenvectors are not orthogonal to within tolerance.");
                DEBUG_DOUBLE(1, "Matrix Element", ME(test, i, j));
                DEBUG_DOUBLE(1, "Tolerance", tol);
                exit(1);
            }
        }
    }
    freeMatrix(test);

    DEBUG(2, "Checking reconstruction property of eigensystem");
    test = subtractMatrix(MR, RD);
    freeMatrix(MR);
    freeMatrix(RD);

    for (i = 0; i < test->row_dim; i++) {
        for (j = 0; j < test->col_dim; j++) {
            if (!EQUAL_ZERO(ME(test, i, j), tol)) {
                failed = 1;
                MESSAGE("Covariance matrix is not reconstructable to within tolerance.");
                DEBUG_DOUBLE(1, "Matrix Element", ME(test, i, j));
                DEBUG_DOUBLE(1, "Tolerance", tol);
                exit(1);
            }
        }
    }
    freeMatrix(test);

    return failed;
}


/**
  Verify properties of the eigen basis used for pca.
 
  The eigenbasis should be ortonormal: U'*U - I == 0
  The basis should be decomposed such that: X == U*D*V'
 
  returns true if tests fail.
*/
int basis_verify(Matrix X, Matrix U) {
    Matrix UtX = transposeMultiplyMatrixL(U, X);
    Matrix UUtX = multiplyMatrix(U, UtX);
    Matrix UtU = transposeMultiplyMatrixL(U, U);
    Matrix identity = makeIdentityMatrix(U->col_dim);
    Matrix test;
    int i, j;
    int failed = 0;
    const double tol = 1.0e-7;

    freeMatrix(UtX);

    DEBUG(2, "Checking orthogonality of eigenbasis");
    test = subtractMatrix(UtU, identity);
    freeMatrix(UtU);
    freeMatrix(identity);

    for (i = 0; i < test->row_dim; i++) {
        for (j = 0; j < test->col_dim; j++) {
            if (!EQUAL_ZERO(ME(test, i, j), tol)) {
                failed = 1;
                MESSAGE("Eigenbasis is not orthogonal to within tolerance.");
                DEBUG_DOUBLE(1, "Matrix Element", ME(test, i, j));
                DEBUG_DOUBLE(1, "Tolerance", tol);
                exit(1);
            }
        }
    }
    freeMatrix(test);

    DEBUG(2, "Checking reconstruction property of the eigen decomposition");
    test = subtractMatrix(X, UUtX);
    freeMatrix(UUtX);

    for (i = 0; i < test->row_dim; i++) {
        for (j = 0; j < test->col_dim; j++) {
            if (!EQUAL_ZERO(ME(test, i, j), tol)) {
                failed = 1;
                MESSAGE("Data matrix is not reconstructable to within tolerance.");
                DEBUG_DOUBLE(1, "Matrix Element", ME(test, i, j));
                DEBUG_DOUBLE(1, "Tolarence", tol);
                exit(1);
            }
        }
    }
    freeMatrix(test);

    return failed;


}

/**
eigentrain
 
computes the eigen space for matrix images.
 
  This function is used in the training procedure of the face recognition pca
   algorithm.
   INPUT:  the data matrix of images
   OUTPUT: mean: the mean value of the images
           eigen_values: eigenvalues
           eigen_base: eigenvectors
           
   The data matrix is mean centered, and this is a side effect.
*/
void eigentrain(Matrix *mean, Matrix *eigen_vals,
                Matrix *eigen_base, Matrix images) {
    FTYPE p = 0.0;
    Matrix M, eigenvectors;

    DEBUG(1, "Calculating mean image.");
    *mean = get_mean_image(images);

    DEBUG(1, "Calculating the mean centered images for the training set.");
    mean_subtract_images(images, *mean);

    MESSAGE2ARG("Calculating Covariance Matrix: M = images' * images. M is a %d by %d Matrix.", images->col_dim, images->col_dim);
    M = transposeMultiplyMatrixL(images, images);
    DEBUG_INT(3, "Covariance Matrix Rows", M->row_dim);
    DEBUG_INT(3, "Covariance Matrix Cols", M->col_dim);

    DEBUG(2, "Allocating memory for eigenvectors and eigenvalues.");
    eigenvectors = makeMatrix(M->row_dim, M->col_dim);
    *eigen_vals = makeMatrix(M->row_dim, 1);

    MESSAGE("Computing snap shot eigen vectors using the double precision cv eigensolver.");
    cvJacobiEigens_64d(M->data, eigenvectors->data, (*eigen_vals)->data, images->col_dim, p);
    freeMatrix(M);

    DEBUG(1, "Verifying the eigen vectors");
    /* Reconstruct M because it is destroyed in cvJacobiEigens */
    M = transposeMultiplyMatrixL(images, images);
    if (debuglevel >= 3)
        eigen_verify(M, *eigen_vals, eigenvectors);
    freeMatrix(M);

    *eigen_base = multiplyMatrix(images, eigenvectors);
    MESSAGE2ARG("Recovered the %d by %d high resolution eigen basis.", (*eigen_base)->row_dim, (*eigen_base)->col_dim);

    DEBUG(1, "Normalizing eigen basis");
    basis_normalize(*eigen_base);

    /*Remove last elements because they are unneeded.  Mean centering the image
      guarantees that the data points define a hyperplane that passes through
      the origin. Therefore all points are in a k - 1 dimensional subspace.
    */
    (*eigen_base)->col_dim -= 1;
    (*eigen_vals)->row_dim -= 1;
    eigenvectors->col_dim -= 1;

    DEBUG(1, "Verifying eigenbasis");
    if (debuglevel >= 3)
        basis_verify(images, *eigen_base);

    /* The eigenvectors for the smaller covariance (snap shot) are not needed */
    freeMatrix(eigenvectors);
}
