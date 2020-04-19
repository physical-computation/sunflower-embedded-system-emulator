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
$RCSfile: csuEBGMMeasure.c,v $
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
#include <csuCommon.h>
#include <csuEBGMUtil.h>
#include <csuEBGMSimilarity.h>

/* Recursive block size to optimize cache in distance matrix calculations */
#define MAX_BLOCK_SIZE 8

time_t start_time;

typedef struct distDirNode {
    struct distDirNode *next;
    char* distDirectory;
    FaceGraphSimilarity distMeasure;
}
DistDirNode;


typedef struct {
    char* imageNamesFile;
    char* faceGraphDir;
    DistDirNode* distList;
    int argc;
    char **argv;
}
Arguments;



int completed = 0;
int total = 0;

/* Displays the useage information for this program and information about what the 
   program does. */
void usage( const char* name){
    printf("useage: %s imagelist.srt face_graph_dir dist_measure distance_dir\n\n",name);

    printf("**************************** DISCRIPTION ****************************\n");
    printf("* csuGaborMeasure                                                   *\n");
    printf("*                                                                   *\n");
    printf("* This program reads face graphs from a directory and computes the  *\n");
    printf("* similarity matrix between those graphs.  The matrix is saved to   *\n");
    printf("* the specified distance directory for use with the analysis tools. *\n");
    printf("*                                                                   *\n");
    printf("*********************************************************************\n");
    printf("\n");
    printf("Arguments:\n");
    printf("imagelist.srt  - This is the list of faces for which the image\n");
    printf("                 distances are computed.\n");
    printf("face_graph_dir - The directory that contains the face graphs,\n");
    printf("                 produced by csuGaborFaceGraph.\n");
    printf("dist_measure   - A label specifying the distance measure to be used\n");
    printf("distance_dir   - The output directory that will contain distance\n");
    printf("                 files that can be used by the csuAnalysis tools.\n");
    printf("\n");
    printf("Distance Measure is one of the following options:\n");
    printf("        FGMagnitude\n");
    printf("        FGPhase\n");
    printf("        FGGridSample\n");
    printf("        FGPredictiveStep\n");
    printf("        FGPredictiveIter\n");
    printf("        FGFixedLocalSearch\n");
    printf("        FGNarrowingLocalSearch\n");
    printf("        GeoSimL2\n");
    printf("        GeoSimCorrelation\n");
    printf("        GeoSimLeastSquares\n");
    printf("        GeoSimLeastSquaresPS\n");
    printf("        GeoSimLeastSquaresPI\n");
    printf("        GeoSimLeastSquaresFLS\n");
    printf("        GeoSimLeastSquaresNLS\n");
    printf("\n");
    exit(1);
}
  
        
        
void processCommand(int argc, char** argv, Arguments* args) {
    int i;
    int param_num = 0;

    /******* Set up default values *******/

    args->argv         	 = argv;
    args->argc         	 = argc;
    args->imageNamesFile = NULL;
    args->faceGraphDir   = NULL;
    args->distList       = NULL;

    debuglevel = 0;

    /******* Read command line arguments *******/

    for (i = 1;i < argc;i++) {

        /* Catch common help requests */
        if      (readOption      (argc, argv, &i, "-help" )) { usage(argv[0]); }
        else if (readOption      (argc, argv, &i, "--help")) { usage(argv[0]); }

        /* other flags */
        else if (readOption    (argc, argv, &i, "-quiet")) { quiet = 1; }
        else if (readOptionInt (argc, argv, &i, "-debuglevel", &debuglevel)) {}

        /* check if the current argument is an unparsed option */
        else if (checkBadOption(argc,argv,&i)) {}

        /* read required arguments */
        else if (param_num == 0) {
            args->imageNamesFile = argv[i];
            param_num++;
        }
        else if (param_num == 1) {
            args->faceGraphDir = argv[i];
            param_num++;
        }
        else if (param_num > 1) {
            DistDirNode* tmp = (DistDirNode*) malloc(sizeof(DistDirNode));
            tmp->next = args->distList;
            if      ( strcasecmp(argv[i], "FGMagnitude") == 0 )           { tmp->distMeasure = fgSimMagnitude; }
            else if ( strcasecmp(argv[i], "FGPhase") == 0 )               { tmp->distMeasure = fgSimPhase; }
            else if ( strcasecmp(argv[i], "FGGridSample") == 0 )          { tmp->distMeasure = fgSimPhaseGridSample; }
            else if ( strcasecmp(argv[i], "FGPredictiveStep") == 0 )      { tmp->distMeasure = fgSimPhasePredictiveStep; }
            else if ( strcasecmp(argv[i], "FGPredictiveIter") == 0 )      { tmp->distMeasure = fgSimPhasePredictiveIter; }
            else if ( strcasecmp(argv[i], "FGFixedLocalSearch") == 0 )    { tmp->distMeasure = fgSimPhaseFixedLocalSearch; }
            else if ( strcasecmp(argv[i], "FGNarrowingLocalSearch") == 0 ){ tmp->distMeasure = fgSimPhaseNarrowingLocalSearch; }
            else if ( strcasecmp(argv[i], "GeoSimL2") == 0 )              { tmp->distMeasure = GeometrySimL2; }
            else if ( strcasecmp(argv[i], "GeoSimCorrelation") == 0 )     { tmp->distMeasure = GeometrySimCorrelation; }
            else if ( strcasecmp(argv[i], "GeoSimLeastSquares") == 0 )    { tmp->distMeasure = GeometrySimLeastSquares; }
            else if ( strcasecmp(argv[i], "GeoSimLeastSquaresPS") == 0 )  { tmp->distMeasure = GeometrySimLeastSquaresPS; }
            else if ( strcasecmp(argv[i], "GeoSimLeastSquaresPI") == 0 )  { tmp->distMeasure = GeometrySimLeastSquaresPI; }
            else if ( strcasecmp(argv[i], "GeoSimLeastSquaresFLS") == 0 ) { tmp->distMeasure = GeometrySimLeastSquaresFLS; }
            else if ( strcasecmp(argv[i], "GeoSimLeastSquaresNLS") == 0 ) { tmp->distMeasure = GeometrySimLeastSquaresNLS; }
            else{ clParseError(argc,argv,i,"Unrecognized FaceGraph Similarity function"); }

            i++;
            DEBUG_CHECK( i < argc , "Distance directory expected at end of argument list.");
            tmp->distDirectory = strdup (argv[i]);
            args->distList = tmp;
            param_num++;
        }

    }

    /* make sure that there are the proper number of required arguments */
    if (param_num < 2){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    DEBUG(1,"Debuging enabled");
}



void NormalizeGeometry(FaceGraph f1){
    int i;
    FTYPE sum1 = 0.0;
    FTYPE mean1 = 0.0;
    FTYPE len1 = 0.0;
    FTYPE sqsum1 = 0.0;

    Matrix g1 = makeMatrix(f1->geosize*2,1);

    for (i = 0; i < g1->row_dim; i++) {
        ME(g1,i,0) = (i%2) ? f1->jets[i/2]->y : f1->jets[i/2]->x ;
    }

    for (i = 0; i < g1->row_dim; i++) {
        sum1 += ME(g1,i,0);
    }

    mean1 = sum1 / g1->row_dim;

    for (i = 0; i < g1->row_dim; i++) {
        ME(g1,i,0) -= mean1;
        sqsum1 += ME(g1,i,0)*ME(g1,i,0);
    }

    len1 = 1.0/sqrt(sqsum1);

    for (i = 0; i < g1->row_dim; i++) {
        ME(g1,i,0) = ME(g1,i,0)*len1;
    }

    for (i = 0; i < f1->geosize; i++) {
        f1->jets[i]->x = ME(g1, 2*i  , 0);
        f1->jets[i]->y = ME(g1, 2*i+1, 0);
    }

    freeMatrix(g1);
}


void computeDistanceMatrix(Matrix distance, FaceGraph* graphs, int mini, int maxi, int minj, int maxj, FaceGraphSimilarity distMeasure){
    int i,j;
    static time_t current_time = 0;
    
    if( (maxi - mini > MAX_BLOCK_SIZE) || (maxi - mini > MAX_BLOCK_SIZE) ){
        int sizei = maxi - mini;
        int sizej = maxj - minj;
        computeDistanceMatrix(distance, graphs, mini, mini+sizei/2, minj, minj+sizej/2, distMeasure);
        computeDistanceMatrix(distance, graphs, mini+sizei/2, maxi, minj, minj+sizej/2, distMeasure);
        computeDistanceMatrix(distance, graphs, mini, mini+sizei/2, minj+sizej/2, maxj, distMeasure);
        computeDistanceMatrix(distance, graphs, mini+sizei/2, maxi, minj+sizej/2, maxj, distMeasure);
        if(current_time + 10 < time(NULL)) {
        	/* Estimate the remaining time and print status report */
            int hour, min, sec;
            double remaining_time;
            current_time = time(NULL);
            remaining_time = ((double)total - completed)*((double)current_time - start_time) / completed;
            hour = ((int)remaining_time)/3600;
            min = (((int)remaining_time)%3600)/60;
            sec = ((int)remaining_time)%60;
            printf("Measuring: %010d of %010d  (%5.2f%%)  ETR = %02dh %02dm %02ds\r",completed, total, completed*100.0/total, hour, min, sec);
            fflush(stdout);
        }
        return;
    }

    for(i = mini; i < maxi; i++){
        for(j = minj; j < maxj; j++){
            ME(distance,i,j) = distMeasure(graphs[i], graphs[j]);
            completed++;
        }
    }

}


int main(int argc, char** argv){
    ImageList *imagenames, *subject, *replicate;
    int imageCount;
    Matrix distance;
    FaceGraph* graphs;
    char** names;
    int i, j;
    Arguments args;
    DistDirNode* distrec;
    
    processCommand(argc, argv, &args);
    
    MESSAGE("Reading in image names");
    imagenames = getImageNames(args.imageNamesFile, &imageCount);

    MESSAGE1ARG("Reading in graph files %d",imageCount);

    /* Allocate space for imagenames, face graphs and distance matrix */
    names = ALLOCATE(char*,imageCount);
    graphs = ALLOCATE(FaceGraph, imageCount);
    distance = makeZeroMatrix(imageCount,imageCount);


    MESSAGE("Reading in graph files");
    i = 0;
    for(subject = imagenames; subject != NULL; subject = subject->next_subject){
        for( replicate = subject; replicate != NULL; replicate = replicate->next_replicate){
            printf("Reading in graph: %s\n", replicate->filename); fflush(stdout);
            names[i] = strdup(replicate->filename);
            graphs[i] = loadFaceGraph(makePath(args.faceGraphDir,replicate->filename));
            i++;
        }
    }

    for(distrec = args.distList; distrec != NULL; distrec = distrec->next){
        /* Create distance matrix */
        completed = 0;
        total = imageCount*imageCount;
        
        MESSAGE("Computing Distance Matrix");
        start_time = time(NULL);

        computeDistanceMatrix(distance, graphs, 0, imageCount, 0, imageCount, distrec->distMeasure);

        /* Print out distance files to the distance directory */
        for(i = 0; i < imageCount; i++){
            FILE* distfile = fopen(makePath(distrec->distDirectory,names[i]), "w");

            if(!distfile){
                printf("Error opening distance file: %s\n",makePath(distrec->distDirectory,names[i]));
                exit(1);
            }

            printf("Saving distances for image: %s\n",names[i]); fflush(stdout);

            for(j = 0; j < imageCount; j++){
                fprintf(distfile,"%s\t%16e\n",names[j], ME(distance,i,j));
            }
            fclose(distfile);
        }
    }

    return 0;
}


