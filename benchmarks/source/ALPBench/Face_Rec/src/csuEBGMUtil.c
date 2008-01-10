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
$RCSfile: csuEBGMUtil.c,v $
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

/* CSU INCLUDES */
#include <csuEBGMUtil.h>

/*
 *  GaborJets.c
 *  csuEvalFaceRec
 *
 *  Created by David Bolme on Sat Jul 06 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */


/* Allocate memory for the gabor jet */
GaborJet makeGaborJet(int length){
    GaborJet jet = (GaborJet)malloc(sizeof(gabor_jet));
    assert(jet);

    if(length ){
        jet->length   = length;

        jet->realPart = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
        assert(jet->realPart);
        jet->imagPart = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
        assert(jet->imagPart);
        jet->mag      = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
        assert(jet->mag);
        jet->ang      = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
        assert(jet->ang);
    } else {
        length = 0;
    }

    jet->params = NULL;

    return jet;
}

/* Free memory for the gabor jet */
void     freeGaborJet(GaborJet jet){
    if(jet->length){
        free( jet->realPart );
        free( jet->imagPart );
        free( jet->mag );
        free( jet->ang );
    }

    free(jet);
}

/*
This is a utility function that computes the polar coordinage transform
for the values in a gabor jet.
*/
void computePolar(GaborJet jet){
    int i;

    for( i = 0; i < jet->length; i++){
        /* Find the magnitude of the imaginary points */
        jet->mag[i] = sqrt(SQR(jet->realPart[i])+SQR(jet->imagPart[i]));
        /* Find the angle of the imaginary points */
        if(jet->realPart[i] != 0){
            /* determine which quadrint the point lies in */
            if(jet->realPart[i] >= 0){
                jet->ang[i] = atan(jet->imagPart[i]/jet->realPart[i]);
            }
            else{
                jet->ang[i] = PI + atan(jet->imagPart[i]/jet->realPart[i]);
            }
        }
        else{
	        if(jet->imagPart[i] >= 0){
                jet->ang[i] = PI/2;
            }
            else{
                jet->ang[i] = -PI/2;
            }
        }
    }
}


/* Extract a jet from an image at a paticular location */
GaborJet extractJet(GFTYPE x, GFTYPE y, Image im, JetMasks masks){
    GaborJet jet = NULL;
    if( masks->size ){
        /* This algrithm aproximates the gabor parameters at a location in continuous space
        By computing the gabor convoluation at an integer location close to the contiuous
        point and then shifting the phase parameters the apropreate amount. It is much faster
        then using linear interpolation and it is probably more accurate. */
        int i;
        float rx = ROUND(x);
        float ry = ROUND(y);
        float dx = x - rx;
        float dy = y - ry;
        jet = makeGaborJet(masks->size/2);

        /* Add a pointer to the mask parameters for future use */
        jet->params = masks->params;


        /* Compute the jet coeffecents */
        for( i = 0; i < jet->length; i++){
            jet->realPart[i] = convolvePoint(rx, ry, 0, im, masks->masks[2*i]);
            jet->imagPart[i] = convolvePoint(rx, ry, 0, im, masks->masks[2*i+1]);
            FINITE(jet->realPart[i]);
            FINITE(jet->imagPart[i]);
        }
        computePolar(jet);

        /* Adjust phase coordinates */
        for( i = 0; i < jet->length; i++){
            jet->ang[i] +=  dx * jet->params->kx[2*i] + dy * jet->params->ky[2*i];
        }

        /* Recompute coeffecents based on phase change */
        for( i = 0; i < jet->length; i++){
            jet->realPart[i] = jet->mag[i] * cos(jet->ang[i]);
            jet->imagPart[i] = jet->mag[i] * sin(jet->ang[i]);
        }
    }
    else{
        jet = makeGaborJet(0);
    }

    /* Save the jet location */
    jet->x = x;
    jet->y = y;

    return jet;
}



/* Allocate/Free space for the Jet Params */
GaborJetParams makeJetParams(int length){
    GaborJetParams params = (GaborJetParams)malloc(sizeof(gabor_jet_params));
    assert( params );

    params->length     = length;

    params->wavelength = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->angle      = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->phase      = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->aspect     = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->radius     = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->kx         = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->ky         = (GFTYPE*)malloc(sizeof(GFTYPE)*length);
    params->size       = (int*)malloc(sizeof(int)*length);

    return params;
}


void freeJetParams(GaborJetParams params){

    free( params->wavelength );
    free( params->angle      );
    free( params->phase      );
    free( params->aspect     );
    free( params->radius     );
    free( params->size       );
    free( params->kx         );
    free( params->ky         );

    free( params );
}

/* This function alocates space for gabor masks */
JetMasks makeJetMasks(int size){
    JetMasks masks = (JetMasks)malloc(sizeof(jet_masks));
    assert(masks);

    masks->size = size;
    masks->masks = (Image*)malloc(sizeof(Image)*size);
    masks->params = makeJetParams(size);
    assert(masks->masks);

    return masks;
}

/* This function frees space that was used to hold on to gabor
 * masks */
void freeJetMasks(JetMasks masks){
    int n;

    for(n = 0; n < masks->size; n++){
        freeImage( masks->masks[n] );
    }
    freeJetParams( masks->params );
    free( masks->masks );
    free( masks );
}


/*
    This function creates an image to be used as a convolution mask with a gabor kernel
*/
double WiskottDCFree = 0.0;

Image makeGaborMask(FTYPE lambda, FTYPE theta, FTYPE phi,
                    FTYPE gamma, FTYPE sigma,int size)
{
    Image mask = makeImage(size,size,1);
    int i, j;


    for(j = 0; j < size; j++){
        for(i = 0; i < size; i++){
            FTYPE x = size/2.0 - size + i;
            FTYPE y = size/2.0 - size + j;
            FTYPE xp =  x*cos(theta)+y*sin(theta);
            FTYPE yp = -x*sin(theta)+y*cos(theta);
            FTYPE tmp1 = -(xp*xp+gamma*gamma*yp*yp)/(2*sigma*sigma);
            FTYPE tmp2 = (2*PI*xp/lambda)+phi;
            IE(mask,i,j,0) = exp(tmp1)*(cos(tmp2)-(phi == 0.0)*(WiskottDCFree)*exp(-sigma*sigma*0.5));
        }
    }

    /* Rescale the pixel values to have a standard total length */
    ZeroMeanUnitLength(mask);

    return mask;
}


JetMasks readMasksFile(const char* filename){
    JetMasks masks = NULL;
    int maskCount;
    double lambda, angle, phase, gama, sigma; int maskSize;
    int i;
    FILE* file = fopen(filename,"r");

    if(!file){
        printf("Error opening file: %s",filename);
        exit(1);
    }

    /* Read in the number of Masks. */
    fscanf(file, "%d",&maskCount);
    masks = makeJetMasks(maskCount);

    /* Read in mask parameters and create masks. */

    for(i = 0; i < maskCount; i++){
        if(fscanf(file, "%lf %lf %lf %lf %lf %d", &lambda, &angle, &phase, &gama, &sigma, &maskSize) != 6){
            printf("Error reading mask file: %s line: %d", filename, i);
            exit(1);
        }
        masks->params->wavelength[i] = lambda;
        masks->params->angle[i]      = angle;
        masks->params->phase[i]      = phase;
        masks->params->aspect[i]     = gama;
        masks->params->radius[i]     = sigma;
        masks->params->kx[i]         = 2.0*PI*cos(angle)/lambda;
        masks->params->ky[i]         = 2.0*PI*sin(angle)/lambda;
        masks->params->size[i]       = maskSize;

        masks->masks[i] = makeGaborMask(lambda, angle, phase, gama, sigma, maskSize);
        ZeroMeanUnitLength(masks->masks[i]);
    }

    fclose( file );

    return masks;
}




/* This function reads in a discription of a graph and loads it into
 * a graph discription structure.  The file format is asci were the
 * first token is the number of vericies followed by verticy labels
 * and guess x, y coordinates.  This is followed by the number of edges
 * and the indexes of the vericies that they connect.
 */
GraphDiscription readGraphDiscription(const char* filename){
    FILE* file;
    int i;
    GraphDiscription gd = ALLOCATE(graph_discription,1);
    DEBUG_CHECK(gd,"Error allocating memory");

    file = fopen(filename,"r");
    DEBUG_CHECK_1ARG(file,"Error opening file: %s", filename);

    /* Read the number of vericies in the file */
    DEBUG_CHECK_1ARG(fscanf(file,"%d",&(gd->numVert)) == 1, "Error: could not determin the number of verticies in file %s", filename);

    /* allocate space for vericies and load discriptions */
    gd->verts = ALLOCATE(Vert,gd->numVert);
    gd->vertLabels = ALLOCATE(char*,gd->numVert);
    gd->bunch = ALLOCATE(JetBunch,gd->numVert);
    for(i = 0; i < gd->numVert; i++){
        gd->vertLabels[i] = ALLOCATE(char,LABELMAX);
        DEBUG_CHECK_2ARG(fscanf(file, "%s %lf %lf",  gd->vertLabels[i], &(gd->verts[i].x), &(gd->verts[i].y)) == 3,
                         "Error Parsing vertex %d in file: %s", i, filename);
        gd->bunch[i] = makeJetBunch();

    }

    /* Read the number of edges in the file */
    DEBUG_CHECK_1ARG(fscanf(file,"%d",&(gd->numEdge)) == 1, "Error: could not determin the number of edges in file %s", filename);

    /* allocate space for vericies and load discriptions */
    gd->edges = ALLOCATE(Edge,gd->numEdge);
    for(i = 0; i < gd->numEdge; i++){
        DEBUG_CHECK_2ARG(fscanf(file, "%d %d",&(gd->edges[i].vert1), &(gd->edges[i].vert2)) == 2,
                         "Error Parsing vertex %d in file: %s", i, filename);
    }
    fclose(file);

    return gd;
}

/* This function reads in a discription of a graph and loads it into
 * a graph discription structure.  The file format is asci were the
 * first token is the number of vericies followed by verticy labels
 * and guess x, y coordinates.  This is followed by the number of edges
 * and the indexes of the vericies that they connect.
 */
void saveGraphDiscription(const char* filename, GraphDiscription gd){
    FILE* file;
    int i;

    file = fopen(filename,"w");
    DEBUG_CHECK_1ARG(file,"Error opening file: %s", filename);

    /* Read the number of vericies in the file */
    fprintf(file,"%d\n",(gd->numVert));

    /* allocate space for vericies and load discriptions */
    for(i = 0; i < gd->numVert; i++){
        fprintf(file, "%s %f %f\n",  gd->vertLabels[i], (gd->verts[i].x), (gd->verts[i].y));
    }

    /* Read the number of edges in the file */
    fprintf(file,"%d\n",(gd->numEdge));

    /* allocate space for vericies and load discriptions */
    for(i = 0; i < gd->numEdge; i++){
        fprintf(file, "%d %d\n",(gd->edges[i].vert1), (gd->edges[i].vert2));
    }

    fclose(file);
}

void freeGraphDiscription(GraphDiscription gd){
    int i;

    for(i = 0; i < gd->numVert; i++){
        free(gd->vertLabels[i]);
        freeJetBunch(gd->bunch[i]);
    }

    free(gd->bunch);
    free(gd->verts);
    free(gd->vertLabels);
    free(gd->edges);
    free(gd);
}



Image makeComplexImage(const Image jetImage){
    Image complexImage = makeImage(jetImage->width, jetImage->height, jetImage->channels/2);
    int x,y,c;

    DEBUG_CHECK(jetImage->channels%2 == 0,"jetImage must have an even number of channels");

    for(x = 0; x < complexImage->width; x++){
        for(y = 0; y < complexImage->height; y++){
            for(c = 0; c < complexImage->channels; c++){
                IE(complexImage,x,y,c) =
                sqrt(SQR(IE(jetImage,x,y,c*2))+
                     SQR(IE(jetImage,x,y,c*2+1)));
            }
        }
    }
    return complexImage;
}


Image makeJetImage(const JetMasks masks, Image im){

    Image jets = makeImage(im->width,im->height,im->channels*(masks->size));
    int n,x,y,c;
    for(y = 0; y < im->height; y++){
        for(x = 0; x < im->width; x++){
            for(c = 0; c < im->channels;c++){
                for(n = 0; n < masks->size; n++){
                    IE(jets,x,y,c*masks->size+n) = convolvePoint(x,y,c,im,masks->masks[n]);
                }
            }
        }
    }

    return jets;
}

/**************************** Jet Bunch Code *******************************/

#define JET_BUNCH_DEFAULT_SIZE 32

JetBunch makeJetBunch(){
    JetBunch jb = ALLOCATE(jet_bunch,1);
    jb->allocsize = 0; /* only allocate memory if a jet is added. */
    jb->size = 0;

    jb->jets = ALLOCATE(GaborJet,jb->allocsize);
    return jb;
}

void freeJetBunch(JetBunch jb){
    int i;
    for(i = 0; i < jb->size; i++){
        if(jb->jets[i]) freeGaborJet(jb->jets[i]);
    }
    free(jb->jets);
    free(jb);
}


int addJetToBunch(JetBunch bunch, GaborJet jet){
    if(bunch->size >= bunch->allocsize){
        int i;
        GaborJet* tmpjets = ALLOCATE(GaborJet, bunch->allocsize + JET_BUNCH_DEFAULT_SIZE);
        bunch->allocsize = bunch->allocsize + JET_BUNCH_DEFAULT_SIZE;
        for(i = 0; i < bunch->size; i++){
            tmpjets[i] = bunch->jets[i];
        }
        free(bunch->jets);
        bunch->jets = tmpjets;
    }
    bunch->jets[bunch->size] = jet;
    bunch->size++;
    return bunch->size - 1;
}


/******************************* Face Graph ********************************/

int alloc_faceGraph = 0;

FaceGraph makeFaceGraph( int geosize, int totalsize ){
    int i;
    FaceGraph fg = ALLOCATE(face_graph,1);
    assert(fg);
    fg->geosize = geosize;
    fg->totalsize = totalsize;
    fg->params = NULL;
    fg->jets = ALLOCATE(GaborJet,totalsize);
    for(i = 0; i < totalsize; i++){
        fg->jets[i] = NULL;
    }
    
    alloc_faceGraph++;
    return fg;
}

void freeFaceGraph( FaceGraph fg ){
    int i;
    for(i = 0; i < fg->totalsize; i++){
        if(fg->jets[i] != NULL){
            freeGaborJet(fg->jets[i]);
        }
    }
    free(fg->jets);
    free(fg);
    alloc_faceGraph--;
}

void  writeint(FILE* file, int i){
    fprintf(file,"%d\n",i);
}

void  writereal(FILE* file, FTYPE r){
    fprintf(file,"%e\n",r);
}

int  readint(FILE* file){
    int i;
    fscanf(file,"%d",&i);
    return i;
}

FTYPE  readreal(FILE* file){
    float f;
    fscanf(file,"%f",&f);
    return f;
}

FaceGraph loadFaceGraph(char *filename){
    int i, j;
    int totalsize, geosize, params;
    FaceGraph fg;
    FILE* file = fopen(filename,"rb");
    printf("Loading Graph: %s   \r", filename); fflush(stdout);
    assert(file);
    /* Write out the size */
    geosize = readint(file);
    totalsize = readint(file);

    fg = makeFaceGraph(geosize, totalsize);

    params = readint(file);
    if(params) {

        int gaborsize = readint (file);
        fg->params = makeJetParams(gaborsize);
        for(i = 0; i < fg->params->length; i++){
            fg->params->wavelength[i] = readreal(file);
            fg->params->angle[i] = readreal(file);
            fg->params->phase[i] = readreal(file);
            fg->params->aspect[i] = readreal(file);
            fg->params->radius[i] = readreal(file);
            fg->params->size[i] = readint (file);
            fg->params->kx[i] = readreal(file);
            fg->params->ky[i] = readreal(file);
        }
    }

    for(i = 0; i < fg->totalsize; i++){
        int length;
        /* write out jet i */
        length = readint(file);
        fg->jets[i] = makeGaborJet(length);
        fg->jets[i]->params = fg->params;
        fg->jets[i]->x = readreal(file);
        fg->jets[i]->y = readreal(file);
        for(j = 0; j < fg->jets[i]->length; j++){
            fg->jets[i]->realPart[j] = readreal(file);
            fg->jets[i]->imagPart[j] = readreal(file);
            fg->jets[i]->mag[j] = readreal(file);
            fg->jets[i]->ang[j] = readreal(file);
        }
    }
    fclose(file);
    return fg;
}

void saveFaceGraph(char *filename, FaceGraph fg){
    int i, j;
    FILE* file = fopen(filename,"wb");
    assert(file);
    /* Write out the size */
    writeint(file, fg->geosize);
    writeint(file, fg->totalsize);
    writeint(file, fg->params != NULL);
    if(fg->params) {
        writeint (file, fg->params->length);
        for(i = 0; i < fg->params->length; i++){
            writereal(file, fg->params->wavelength[i]);
            writereal(file, fg->params->angle[i]);
            writereal(file, fg->params->phase[i]);
            writereal(file, fg->params->aspect[i]);
            writereal(file, fg->params->radius[i]);
            writeint (file, fg->params->size[i]);
            writereal(file, fg->params->kx[i]);
            writereal(file, fg->params->ky[i]);
        }
    }

    for(i = 0; i < fg->totalsize; i++){
        /* write out jet i */
        writeint(file, fg->jets[i]->length);
        writereal(file, fg->jets[i]->x);
        writereal(file, fg->jets[i]->y);
        for(j = 0; j < fg->jets[i]->length; j++){
            writereal(file, fg->jets[i]->realPart[j]);
            writereal(file, fg->jets[i]->imagPart[j]);
            writereal(file, fg->jets[i]->mag[j]);
            writereal(file, fg->jets[i]->ang[j]);
        }
    }
    
    fclose(file);
}






