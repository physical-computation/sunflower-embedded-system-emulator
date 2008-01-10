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
$RCSfile: csuEBGMGraphFit.c,v $
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

/* csuGaborGraphFit */
#include <stdio.h>
#include <stdlib.h>
#include <csuEBGMUtil.h>
#include <csuEBGMSimilarity.h>
#include <csuCommon.h>

typedef struct {
    char* modelFile;
    char* novelFile;
    char* masksFile;

    char* imageDir;
    char* graphDir;
    char* outputDir;

    int bunchSize;

    JetDisplacementEstimator dispEst; 
}
Arguments;
/*
 * Process the command line and initialize the variables
 */

void usage( const char* name){
    printf("usage: %s [OPITIONS] modelImages.srt novelImage.srt wavelet_file image_dir model_graph_dir displace_estimator outputDir\n\n", name);
    printf("\n**************************** DISCRIPTION ****************************\n");
    printf("* csuGaborGraphFit                                                  *\n");
    printf("*                                                                   *\n");
    printf("* The purpose of this exicutable to to read a set of training images*\n");
    printf("* and their graph discription and build graph discriptions for      *\n");
    printf("* novel images.  This program performes different variations of the *\n");
    printf("* face bunch graph matching algorithm to locate points on a face.   *\n");
    printf("*                                                                   *\n");
    printf("*********************************************************************\n");
    printf("\n");
    printf("  ARGUMENTS:\n");
    printf("      model_images.srt  -   A list of images with user built graphs\n");
    printf("      novel_images.srt  -   A list of images to generate graphs\n");
    printf("      wavelet_file      -   A gabor jet discription file\n");
    printf("      image_dir         -   A directory containing images\n");
    printf("      model_graph_dir   -   A directory containing model graphs\n");
    printf("      displaceEstimator -   The name of the algorithm to use for displacement estimation\n");
    printf("      outputDir         -   A directory to store the new graphs discriptions\n");
    printf("\n");

    exit(1);
}

void processCommand(int argc, char** argv, Arguments* args) {
    int i;
    int param_num = 0;

    /******* Set up default values *******/

    args->imageDir     = NULL;
    args->graphDir     = NULL;
    args->outputDir    = NULL;
    args->modelFile       = NULL;
    args->novelFile       = NULL;
    args->masksFile       = NULL;

    args->bunchSize       = 70;

    debuglevel = 0;

    /******* Read command line arguments *******/

    for (i = 1;i < argc;i++) {

        /* Catch common help requests */
        if      (readOption      (argc, argv, &i, "-help" )) { usage(argv[0]); }
        else if (readOption      (argc, argv, &i, "--help")) { usage(argv[0]); }

        else if (readOptionInt (argc, argv, &i, "-bunchSize", &args->bunchSize)) {}
        /* other flags */
        else if (readOption    (argc, argv, &i, "-quiet")) { quiet = 1; }
         else if (readOption    (argc, argv, &i, "-dcfree")) { WiskottDCFree = 1.0; }
       else if (readOptionInt (argc, argv, &i, "-debuglevel", &debuglevel)) {}

        /* check if the current argument is an unparsed option */
        else if (checkBadOption(argc,argv,&i)) {}

        /* read required arguments */
        else if (param_num == 0) {
            args->modelFile = argv[i];
            param_num++;
        }
        else if (param_num == 1) {
            args->novelFile = argv[i];
            param_num++;
        }
        else if (param_num == 2) {
            args->masksFile = argv[i];
            param_num++;
        }
        else if (param_num == 3) {
            args->imageDir = argv[i];
            param_num++;
        }
        else if (param_num == 4) {
            args->graphDir = argv[i];
            param_num++;
        }
        else if (param_num == 5) {
            if      (strcasecmp(argv[i],"DEGridSample") == 0) { args->dispEst = DEGridSample; }
            else if (strcasecmp(argv[i],"DEPredictiveStep") == 0) { args->dispEst = DEPredictiveStep; }
            else if (strcasecmp(argv[i],"DEPredictiveIter") == 0) { args->dispEst = DEPredictiveIter; }
            else if (strcasecmp(argv[i],"DEFixedLocalSearch") == 0) { args->dispEst = DEFixedLocalSearch; }
            else if (strcasecmp(argv[i],"DENarrowingLocalSearch") == 0) { args->dispEst = DENarrowingLocalSearch; }
            else{ clParseError(argc,argv,i,"Unknown Distance Estimator"); }
            param_num++;
        }
        else if (param_num == 6) {
            args->outputDir = argv[i];
            param_num++;
        }
        else{ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    }

    /* make sure that there are the proper number of required arguments */
    if (param_num != 7){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    DEBUG(1,"Debuging enabled");
}


GraphDiscription extractModelJets(ImageList* modelImages, char* imageDir, char* graphDir, JetMasks masks){
    ImageList *subject, *replicate;
    GraphDiscription mj = NULL;
    int count = 0, i;

    for(subject = modelImages; subject != NULL; subject = subject->next_subject){
        for(replicate = subject; replicate != NULL; replicate = replicate->next_replicate){
            /* load the model graph */
            GraphDiscription gd = readGraphDiscription(makePath(graphDir,replicate->filename));

            /* Load the model image */
            Image model = readRawImage(makePath(imageDir,replicate->filename));

            /*MESSAGE2ARG("Extracting Jets From Image #%d.  <%s>", count, replicate->filename);*/
            if(count == 0){
                /* load up a default graph */
                mj = readGraphDiscription(makePath(graphDir,replicate->filename));
            }
            else{
                assert(mj->numVert == gd->numVert);
                for(i = 0; i < mj->numVert; i++){
                    /* Accumulate mean locations */
                    mj->verts[i].x += gd->verts[i].x;
                    mj->verts[i].y += gd->verts[i].y;
                }
            }

            /* extract jets */
                printf("Extracting jets from model image #%03d.  <%s>  |", count, replicate->filename); fflush(stdout);
            for(i = 0; i < gd->numVert; i++){
                printf("#"); fflush(stdout);
                addJetToBunch( mj->bunch[i],
                    extractJet(gd->verts[i].x, gd->verts[i].y, model, masks));
            }
            printf("|\n");

            freeImage(model);
            freeGraphDiscription(gd);
            count++;
        }
    }

    for(i = 0; i < mj->numVert; i++){
        /* Accumulate mean locations */
        mj->verts[i].x /= count;
        mj->verts[i].y /= count;
    }

    return mj;
}

GraphDiscription buildBunchGraph(GraphDiscription modelJets, JetSimilarityMeasure jetSim, int bunchsize){
    int vert;

    assert(bunchsize > 1);
    for(vert = 0; vert < modelJets->numVert; vert++){
        Matrix dist;
        int size, i, j;
        FTYPE maxsim, minsim, smallest;
        int minjet;
        JetBunch finalBunch = makeJetBunch();

        MESSAGE1ARG("Selecting jets for point: %s",modelJets->vertLabels[vert]);

        size = modelJets->bunch[vert]->size;
        assert(bunchsize <= size);
        assert(size > 0);

        dist = makeZeroMatrix(size, size);

        smallest = maxsim = jetSim(modelJets->bunch[vert]->jets[0],modelJets->bunch[vert]->jets[0]);
        for(i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                ME(dist,i,j) = jetSim(modelJets->bunch[vert]->jets[i],modelJets->bunch[vert]->jets[j]);
                maxsim = MAX( maxsim, ME(dist,i,j) );
                smallest = MIN(smallest, ME(dist,i,j));
            }
        }

        /* find the minimum similarity */
        minsim = 0.0;
        minjet = -1;
        for(i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                if(minjet < 0 || minsim > ME(dist,i,j) ){
                    minsim = ME(dist,i,j);
                    minjet = i;
                }
            }
        }

        while(1){
            /* MESSAGE2ARG("    Adding jet %03d to final bunch. sim=%+f",minjet,minsim); */
            /* add that jet to the final bunch */
            addJetToBunch(finalBunch, modelJets->bunch[vert]->jets[minjet]);
            /* eleminate the newly added jet from the old bunch and the distance matrix */
            modelJets->bunch[vert]->jets[minjet] = NULL;
            for( i = 0; i < size; i++){
                /* Make sure new value is greater that the maximum similarity */
                ME(dist,i,minjet) = 2*ABS(maxsim);
            }

            if(finalBunch->size >= bunchsize) break;

            minsim = 0.0;
            minjet = -1;
            for(j = 0; j < size; j++){
                FTYPE locmax = smallest;
                for(i = 0; i < size; i++){
                    /* compute the best similarity to the jets in the data set */
                    if(modelJets->bunch[vert]->jets[i] == NULL){
                        locmax = MAX(locmax,ME(dist,i,j));
                    }
                }
                if(minjet < 0 || minsim > locmax ){
                    minsim = locmax;
                    minjet = j;
                }
            }
        }

        freeJetBunch(modelJets->bunch[vert]);
        modelJets->bunch[vert] = finalBunch;
        freeMatrix(dist);
    }
    return modelJets;
}

/* performs a weighted estimation of the location of the vertex location. */
void guessVertexLocation(int vert, int maxvert, GraphDiscription graphTemplate, GraphDiscription bunchGraph){
    FTYPE locx = 0.0, locy = 0.0, tw = 0.0;
    int i;

    if(vert < 6){
        /* Little basis for an estimate so just return guess
           -- assume these are eye coordiantates. */
        graphTemplate->verts[vert].x = bunchGraph->verts[vert].x;
        graphTemplate->verts[vert].y = bunchGraph->verts[vert].y;
        return;
    }
    for(i = 0; i < maxvert; i++){
        if(i != vert){
            /* compute the displacement in the bunch graph */
            FTYPE dx = -bunchGraph->verts[i].x + bunchGraph->verts[vert].x;
            FTYPE dy = -bunchGraph->verts[i].y + bunchGraph->verts[vert].y;
            /* compute the distance */
            FTYPE dist = sqrt(SQR(dx)+SQR(dy));
            /* compute the wieght */
            FTYPE w = exp(-dist);

            /* compute the sum of the location */
            locx += w*(graphTemplate->verts[i].x + dx);
            locy += w*(graphTemplate->verts[i].y + dy);
            tw += w;
        }
    }
    
    /* compute the final vert location */
    graphTemplate->verts[vert].x = locx/tw;
    graphTemplate->verts[vert].y = locy/tw;
}


void LocatePoint( FTYPE *x, FTYPE *y, JetBunch bunch, Image novel, JetMasks masks, JetDisplacementEstimator dispEst){
    int i, best = 0;
    FTYPE dx = 0.0, dy = 0.0;
    FTYPE bestsim = -1.0e300;
    /* Extract Gabor Jet from image*/
    GaborJet njet = extractJet(*x, *y, novel, masks);

    /* Select the closest matching training jet */
    for(i = 0; i < bunch->size; i++){
        FTYPE sim;
        dx = 0.0; dy = 0.0;
        sim = dispEst( bunch->jets[i], njet, &dx, &dy);

        if(sim > bestsim){
            bestsim = sim;
            best = i;
        }
    }

    /* Compute the new x and y */
    dispEst( bunch->jets[best], njet, &dx, &dy);
    
    *x += dx;
    *y += dy;

    /* Free the jet */
    freeGaborJet( njet );
}

void permuteArray(int arr[], int size){
    int i;
    for(i = 0; i < size*2; i++){
        int x = rand()%size;
        int y = rand()%size;
        int tmp = arr[x];
        arr[x] = arr[y];
        arr[y] = tmp;
    }
}
void locateNovelFeatures(ImageList* novelImages, GraphDiscription graphTemplate, GraphDiscription bunchGraph, JetMasks masks, char* imageDir, char* outputDir, JetDisplacementEstimator dispEst){
    ImageList *subject, *replicate;
    int i;

    for(subject = novelImages; subject != NULL; subject = subject->next_subject){
        for(replicate = subject; replicate != NULL; replicate = replicate->next_replicate){
            Image novel = readRawImage(makePath(imageDir,replicate->filename));
            int *verts = (int*)malloc(sizeof(int)*bunchGraph->numVert);
            printf("Fitting graph for image: %s |", replicate->filename); fflush(stdout);
            for(i = 0; i < bunchGraph->numVert; i++){
                printf("#"); fflush(stdout);
                guessVertexLocation(i, i, graphTemplate, bunchGraph);

                graphTemplate->verts[i].x = graphTemplate->verts[i].x;
                graphTemplate->verts[i].y = graphTemplate->verts[i].y;

                LocatePoint( &(graphTemplate->verts[i].x),  &(graphTemplate->verts[i].y), bunchGraph->bunch[i], novel, masks, dispEst);
                verts[i] = i;
            }
            printf("|\n"); fflush(stdout);

            permuteArray(verts, bunchGraph->numVert);
            freeImage(novel);
            saveGraphDiscription(makePath(outputDir,replicate->filename),graphTemplate);
        }
    }
}


int main(int argc, char** argv){
    GraphDiscription modelJets, bunchGraph, graphTemplate;
    ImageList *modelImages, *novelImages;
    JetMasks masks;
    Arguments args;

    srand(time(NULL));

    processCommand(argc, argv, &args);

    /* build masks */
    masks = readMasksFile(args.masksFile);
    modelImages = getImageNames(args.modelFile, NULL);
    novelImages = getImageNames(args.novelFile, NULL);
    graphTemplate = readGraphDiscription(makePath(args.graphDir,modelImages->filename));

    /* extract model jets */
    modelJets = extractModelJets(modelImages, args.imageDir, args.graphDir, masks);

    /* build jet bunch */
    /* bunchGraph = buildBunchGraph(modelJets, args.distance, args.bunchSize); */
    bunchGraph = modelJets;

    /* locate features in novel image */
    locateNovelFeatures(novelImages, graphTemplate, bunchGraph, masks, args.imageDir, args.outputDir, args.dispEst);

    return 0;
}



