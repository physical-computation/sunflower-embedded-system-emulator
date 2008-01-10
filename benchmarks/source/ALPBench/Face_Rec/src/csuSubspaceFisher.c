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

/* CSU Headers */
#include "csuCommon.h"

/*
  subtractClassMean
  
  This function is used to subtract a mean vector from a class of images.
  The class is stored in contiguous vectors in the class matrix.  classStart
  and classEnd are the first and last vectors in the class.  The vectors in
  classmatrix are subtracted in place.  This function is expected to be called
  one time for each class.
 */
void subtractClassMean(Matrix classmatrix, int classStart, int classEnd) {
    int i, j;
    Matrix mean = makeMatrix(classmatrix->row_dim, 1);


    for (i = 0; i < classmatrix->row_dim; i++) {
        ME(mean, i, 0) = 0.0;
        for (j = classStart; j <= classEnd; j++)
            ME(mean, i, 0) += ME(classmatrix, i, j);
        ME(mean, i, 0) = ME(mean, i, 0) / (classEnd - classStart + 1);
    }
    for (i = 0; i < classmatrix->row_dim; i++) {
        for (j = classStart; j <= classEnd; j++) {
            ME(classmatrix, i, j) -= ME(mean, i, 0);
        }
    }
    freeMatrix(mean);
}


void fisherVerify(Matrix fisherBasis, Matrix fisherValues, Matrix Sw, Matrix Sb) {
    Matrix SbW = multiplyMatrix(Sb, fisherBasis);
    Matrix SwW = multiplyMatrix(Sw, fisherBasis);
    Matrix D = makeIdentityMatrix(fisherBasis->row_dim);
    Matrix DSwW;
    Matrix zeroMat;
    int i, j;


    MESSAGE("Verifying Fisher Basis.");

    for (i = 0; i < D->row_dim; i++) {
        ME(D, i, i) = ME(fisherValues, i, 0);
    }

    DSwW = multiplyMatrix(D, SwW);
    zeroMat = subtractMatrix(SbW, DSwW);

    for (i = 0; i < zeroMat->row_dim; i++) {
        for (j = 0; j < zeroMat->col_dim; j++) {
            if (!EQUAL_ZERO(ME(zeroMat, i, j), 0.000001)) {
                DEBUG( -1, "Fisher validation failed.");
                printf("Element: (%d,%d) value = %f", i, j, ME(zeroMat, i, j));
                exit(1);
            }
        }
    }
}



/*  findWCSMatrix
  
   This function creates the within-class scatter matrix 'Sw' as
   described in Duda, "Pattern Classification", p.121.
   
   This function takes advantage of the fact that the images for each class 
   are stored in consecutive columns of the data matrix. There is an outer 
   loop through the classes, i.e. subjects, and for each subject, the start
   and finish column indices are determined. Then the data columns for just that 
   class are mean centered. From this, the portion of the scatter
   matrix contributed by this class is created by the outer product of the 
   data matrix with itself, and this is in turn, added into the within 
   class scatter matrix. When done iterating over the classes, the results
   is the final within class scatter matrix.
 */

Matrix findWCSMatrix(Matrix imspca, ImageList* srt, int *numberOfClasses, int writeTextInterm) {
    Matrix Sw;
    Matrix classmatrix = duplicateMatrix(imspca);
    ImageList *subject, *replicate;
    int maxClassSize = 0;
    int repCount;

    *numberOfClasses = 0;
    MESSAGE("Finding within-class scatter matrix.");
    for (subject = srt; subject; subject = subject->next_subject) {
        int classStart, classEnd;
        (*numberOfClasses)++;  /* Count the number of classes */

        DEBUG(3, "Finding the class indexes.");
        classStart = classEnd = subject->imageIndex;
        repCount = 0;
        for (replicate = subject; replicate; replicate = replicate->next_replicate) {
            repCount ++;
            classEnd = replicate->imageIndex;
        }

        if (repCount > maxClassSize) { maxClassSize = repCount; }

        if (repCount == 1 && !quiet) {
            printf("WARNING: class %d only contains one image.\n", *numberOfClasses);
        }

        DEBUG_INT(4, "Lower limit on class.", classStart);
        DEBUG_INT(4, "Upper limit on class.", classEnd);
        DEBUG(3, "Mean centering the class.");
        subtractClassMean(classmatrix, classStart, classEnd);

    }

    DEBUG_CHECK(maxClassSize > 1, "ERROR: at least one class must have more than one replicate.  Make sure your image list is not flat.");

    if (debuglevel > 3)
        printMatrix("classmatrix", classmatrix);

    if (writeTextInterm) { SAVE_MATRIX(classmatrix); } /* output textfiles of intermediate matrices */

    DEBUG(3, "Creating the class scatter matrix.");
    Sw = transposeMultiplyMatrixR(classmatrix, classmatrix);

    if (debuglevel > 3)
        printMatrix("Sw", Sw);
    return Sw;
}


/*  findBCSMatrix
 
 The approach here is to use equation 116 on p. 122 of
 Duda, "Pattern Classification" to solve for the between
 class scatter matrix. The equation reads:
 
 St = Sw + Sb  (St is total scatter matrix)
 
 Solving for Sb,  Sb = St - Sw. Note Sw is computed above.
 
 The total scatter matrix is easy to compute, it is what we
 think of as the covariance matrix when the points have been
 centered.
 */
Matrix findBCSMatrix(Matrix imspca, Matrix Sw) {
    Matrix Sb;
    Matrix St;
    Matrix mean;

    MESSAGE("Finding between-class scatter matrix.");

    mean = get_mean_image(imspca);
    mean_subtract_images(imspca, mean);

    DEBUG(3, "Producing total scatter matrix");
    St = transposeMultiplyMatrixR(imspca, imspca);

    DEBUG(2, "Producing between-class scatter matrix");
    Sb = subtractMatrix(St, Sw);

    freeMatrix(St);
    freeMatrix(mean);

    return Sb;
}

/*  Rw will be the eigenvectors for the within class scatter matrix
    Sw will be the square roots of the eigenvalues for the within class scatter matrix
    These are thought of in terms of a rotation (and reflection) and scale change operator
    that tranforms the imspca points into a new space the within class variance is equal
    in all dimensions.
 
    The between class scatter matrix is transformed into this new space and then its
    eigenvectors and values are determined. The fisher basis vectors are then derived
    by backward mapping the resulting eigen vectors into the original space. If this is
    not clear, and it probably is not, see the paper: "The Geometry of LDA nad PCA
    classifiers Illustrated with 3D Examples, J. Ross Beveridge, CSU Technical Report
    01-101". It is available under papers at
    http://www.cs.colostate.edu/evalfacerec/index.html
 
     */

void fisherTrain(Matrix imspca, ImageList *srt, Matrix *fisherBasis, Matrix *fisherValues, int writeTextInterm) {
    int i;
    int numberOfClasses;
    Matrix G, N, Tmp;
    Matrix Rw = makeIdentityMatrix(imspca->row_dim);
    Matrix Siw = makeIdentityMatrix(imspca->row_dim);
    Matrix Ev = makeMatrix(imspca->row_dim, 1);
    Matrix Evecs = makeMatrix(imspca->row_dim, imspca->row_dim);
    Matrix Mw = findWCSMatrix(imspca, srt, &numberOfClasses, writeTextInterm);
    Matrix Mb = findBCSMatrix(imspca, Mw);
    *fisherValues = makeMatrix(imspca->row_dim, 1);

    MESSAGE2ARG("LDA Training started with %d classes and %d total training images.", numberOfClasses, imspca->col_dim);

    if (writeTextInterm) { SAVE_MATRIX(Mw); SAVE_MATRIX(Mb); } /* output textfiles of intermediate matrices */


    MESSAGE("Computing eigenspace decomposition of within class scatter matrix.");
    cvJacobiEigens_64d(Mw->data, Rw->data, Ev->data, Mw->row_dim, 0.0);

    MESSAGE("Computing the inverse scale matrix derived from eigenvalues and transformed scatter matrix.");
    for (i = 0; i < Ev->row_dim; i++)
        ME(Siw, i, i) = (ME(Ev, i, 0) <= 0.0) ? 0.0 : 1 / ( sqrt( ME(Ev, i, 0) ) );

    G = transposeMultiplyMatrixR(Siw, Rw);
    Tmp = transposeMultiplyMatrixR(Mb, G);
    N = multiplyMatrix(G, Tmp);
    freeMatrix(Tmp);

    if (writeTextInterm) {
        SAVE_MATRIX(Rw);
        SAVE_MATRIX(Ev);
        SAVE_MATRIX(N);
        SAVE_MATRIX(G);
        SAVE_MATRIX(Siw);
    } /* output textfiles of intermediate matrices */

    MESSAGE("Computing eigenspace of transformed between class scatter matrix.");
    cvJacobiEigens_64d(N->data, Evecs->data, (*fisherValues)->data, N->row_dim, 0.0);
    DEBUG(3, "FINSISHED");

    Tmp = multiplyMatrix(Siw, Evecs);
    DEBUG(1, "Calculating fisher basis");
    *fisherBasis = multiplyMatrix(Rw, Tmp);

    if (writeTextInterm) { SAVE_MATRIX(*fisherBasis); SAVE_MATRIX(*fisherValues); SAVE_MATRIX(Evecs); } /* output textfiles of intermediate matrices */

    /* The following verification code does not seem to work so it has been commented out. */
    /* fisherVerify(*fisherBasis, *fisherValues, Mw, Mb); */

    /* Crop the basis to the proper number of vectors */
    (*fisherBasis)->col_dim = numberOfClasses - 1;
    (*fisherValues)->row_dim = numberOfClasses - 1;

    basis_normalize(*fisherBasis);

    MESSAGE2ARG("Completed LDA Training. Fisher basis projection matrix has dimensions %d by %d.", (*fisherBasis)->row_dim, (*fisherBasis)->col_dim);

    /*Freeing memory allocated during LDA Training. */
    freeMatrix(Tmp);
    freeMatrix(Rw);
    freeMatrix(Siw);
    freeMatrix(Ev);
    freeMatrix(Mw);
    freeMatrix(Mb);
    freeMatrix(G);
    freeMatrix(N);
}

