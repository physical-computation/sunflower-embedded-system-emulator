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
$RCSfile: csuEBGMUtil.h,v $
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

#ifndef CSU_GABOR_INCLUDED
#define CSU_GABOR_INCLUDED

#include <csuCommon.h>

#define LABELMAX 100

#define GFTYPE FTYPE

double WiskottDCFree;

/*************************** Gabor Parameters ******************************/

/* These parameters hold on to the information
 * needed for the gabor kernel.  For many
 * applications, it is assumed that every other
 * gabor kernel is identical but out of phase by
 * 90 degrees (even and odd).  Jets assume this
 * structure and store complex values.  Therefore
 * a jet stores length/2 complex values. */
typedef struct{
    int length;

    GFTYPE* wavelength;
    GFTYPE* angle;
    GFTYPE* phase;
    GFTYPE* aspect;
    GFTYPE* radius;
    int*    size;

    /* Precomputed parameters for displacement estimation */
    GFTYPE* kx;
    GFTYPE* ky;
} gabor_jet_params;

typedef gabor_jet_params *GaborJetParams;

/* Allocate/Free space for the Jet Params */
GaborJetParams makeJetParams(int size);
void freeJetParams(GaborJetParams params);


/******************************** Gabor Mask *******************************/

/* This structure holds on to images of gabor kernels that
 * are used in convolution to determine the gabor coeffecents
 */
typedef struct{
    int size;
    Image *masks;

    GaborJetParams params;
} jet_masks;

typedef jet_masks* JetMasks;

/* Allocate/Free space for the Jet Masks */
JetMasks makeJetMasks(int size);
void     freeJetMasks(JetMasks masks);

Image makeGaborMask(FTYPE wavelength, FTYPE angle, FTYPE phase, FTYPE aspect, FTYPE radius, int size);

JetMasks readMasksFile(const char* filename);

/******************************* Gabor Jets ********************************/
/* Gabor Jet Structure
*
* This structure uses containes the convolution values for a set
* of gabor filters at a paticular location.  Structure elements
* include the location x,y and length of the jet, and the values.
* see Wiskott (Sec. 2.1.1)
*/

typedef struct{
    int length;
    GFTYPE x, y;

    /* Complex specification for the coeffiecients */
    GFTYPE* realPart;
    GFTYPE* imagPart;

    /* Polar transformation of the above complex numbers*/
    GFTYPE* mag;
    GFTYPE* ang;

    /* Parameters used to generate gabor kernels */
    GaborJetParams params;
} gabor_jet;

typedef gabor_jet *GaborJet;

GaborJet makeGaborJet(int length);
void     freeGaborJet(GaborJet jet);
GaborJet extractJet(GFTYPE x, GFTYPE y, Image im, JetMasks masks);

/******************************* Jet Bunch *********************************/

/*
A Jet bunch is a collection of gabor jets that have been extracted from the
same facidual point on the face.  The idea is to form a "bunch" (collection)
of example jets that represent that point.  When trying to localize a point
in a novel image the code can choose a jet that is more similar to the image
being matched.
*/

typedef struct{
    int allocsize;
    int size;

    GaborJet* jets;
} jet_bunch;

typedef jet_bunch *JetBunch;


JetBunch makeJetBunch();
void freeJetBunch(JetBunch bunch);
int addJetToBunch(JetBunch bunch, GaborJet jet);


/************************* Gabor Graph Discription *************************/

typedef struct{
    int vert1, vert2;
} Edge;

typedef struct{
    FTYPE x, y;
} Vert;

typedef struct{
    int numVert;
    int numEdge;
    Vert* verts;
    Edge* edges;
    char** vertLabels;

    JetBunch* bunch;
} graph_discription;

typedef graph_discription *GraphDiscription;

/* This function reads in a discription of a graph and loads it into
 * a graph discription structure.  The file format is asci were the
 * first token is the number of vericies followed by verticy labels
 * and guess x, y coordinates.  This is followed by the number of edges
 * and the indexes of the vericies that they connect.
 */
GraphDiscription readGraphDiscription(const char* filename);
void             saveGraphDiscription(const char* filename,GraphDiscription);
void             freeGraphDiscription(GraphDiscription gd);


/******************************* Face Graph ********************************/

typedef struct{
    int geosize; /* numbers of the located point. */
    int totalsize; /* number fo the located points and interpolated points */
    GaborJetParams params;
    GaborJet* jets;
} face_graph;

typedef face_graph* FaceGraph;

FaceGraph makeFaceGraph( int geosize, int totalsize );
void freeFaceGraph( FaceGraph );

FaceGraph loadFaceGraph(char *filename);
void saveFaceGraph(char *filename, FaceGraph);



/*********************** Extra Utility Functions ***************************/
Image makeJetImage(const JetMasks masks, const Image im);
Image makeComplexImage(const Image jetImage);
JetMasks readMasksFile(const char* filename);




#endif



