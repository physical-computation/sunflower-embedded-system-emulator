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
$RCSfile: csuEBGMSimilarity.h,v $
$Author: teixeira $
$Date: 2003/04/23 14:02:31 $
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

#ifndef EBGM_SIMILARITY
#define EBGM_SIMILARITY

#include <csuEBGMUtil.h>
/************************ Jet Similarity Functions *************************/

/* The following functions compute similarity between two
 * Gabor Jets.  They are described in detail csuGaborJets.c
 */
typedef FTYPE (*JetSimilarityMeasure)(GaborJet j1, GaborJet j2);

FTYPE JetSimilarityMag(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityPhase(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDEGridSample(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDEPredictiveStep(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDEPredictiveIter(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDEFixedLocalSearch(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDENarrowingLocalSearch(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityCityBlock(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityCorrelation(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityCovariance(GaborJet j1, GaborJet j2);

/* These similarty measures are discussed in David Bolme's thesis.
   Results were generated using csuEBGMLocalization */
FTYPE JetSimilarityResponseMag(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityResponsePhase(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDisplacementCheckMag(GaborJet j1, GaborJet j2);
FTYPE JetSimilarityDisplacementCheckPhase(GaborJet j1, GaborJet j2);

/********************** Jet Displacement Estimation ************************/

/* This function estimates the displacement of gabor jet
 * relative to a sample jet.  The method that is used is
 * simmillar to auto correlation.  Also look at the similarity
 * function for phase 1 and 3 for related code. */
typedef FTYPE (*JetDisplacementEstimator)(GaborJet j1, GaborJet j2, FTYPE *tdx, FTYPE *tdy);

FTYPE estimateDisplacementRandom (GaborJet j1, GaborJet j2, FTYPE *tdx, FTYPE *tdy);
FTYPE DEGridSample(GaborJet j1, GaborJet j2, FTYPE *tdx, FTYPE *tdy);
FTYPE DEPredictiveStep(GaborJet j1, GaborJet j2, FTYPE *tdx, FTYPE *tdy);
FTYPE DEPredictiveIter(GaborJet j1, GaborJet j2, FTYPE *tdx, FTYPE *tdy);
FTYPE DEFixedLocalSearch(GaborJet j1, GaborJet j2, FTYPE *tdx, FTYPE *tdy);
FTYPE DENarrowingLocalSearch(GaborJet j1, GaborJet j2, FTYPE *dx, FTYPE *dy);

/********************** Face Graph Similarity ************************/
typedef FTYPE (*FaceGraphSimilarity)(FaceGraph, FaceGraph);

FTYPE GeometrySimL2(FaceGraph, FaceGraph);
FTYPE GeometrySimCorrelation(FaceGraph, FaceGraph);
FTYPE GeometrySimLeastSquares(FaceGraph, FaceGraph);
FTYPE GeometrySimLeastSquaresPS(FaceGraph f1, FaceGraph f2);
FTYPE GeometrySimLeastSquaresPI(FaceGraph f1, FaceGraph f2);
FTYPE GeometrySimLeastSquaresFLS(FaceGraph f1, FaceGraph f2);
FTYPE GeometrySimLeastSquaresNLS(FaceGraph f1, FaceGraph f2);
FTYPE GeometrySimWLeastSquaresPI(FaceGraph f1, FaceGraph f2);

FTYPE fgSimMagnitude(FaceGraph, FaceGraph);
FTYPE fgSimPhase(FaceGraph, FaceGraph);
FTYPE fgSimPhaseGridSample(FaceGraph, FaceGraph);
FTYPE fgSimPhasePredictiveStep(FaceGraph, FaceGraph);
FTYPE fgSimPhasePredictiveIter(FaceGraph, FaceGraph);
FTYPE fgSimPhaseFixedLocalSearch(FaceGraph, FaceGraph);
FTYPE fgSimPhaseNarrowingLocalSearch(FaceGraph, FaceGraph);

#endif



