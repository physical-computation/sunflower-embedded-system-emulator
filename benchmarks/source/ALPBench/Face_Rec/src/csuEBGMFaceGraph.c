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
$RCSfile: csuEBGMFaceGraph.c,v $
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

#include <stdio.h>
#include <stdlib.h>
#include <csuCommon.h>
#include <csuEBGMUtil.h>


typedef struct {
    char* imageFile;
    char* masksFile;

    char* imageDir;
    char* graphDir;
    char* outputDir;
} Arguments;

void usage( const char* name){
    printf("Usage: %s image_list.srt image_dir training_graph_dir gabor_file output_graph_dir\n\n", name);

    printf("**************************** DISCRIPTION ****************************\n");
    printf("* csuGaborFaceGraph                                                 *\n");
    printf("*                                                                   *\n");
    printf("* The purpose of this exicutable is to read a set of model images   *\n");
    printf("* and their graph discription and build graph discriptions for      *\n");
    printf("* novel images.  This program performes different variations of the *\n");
    printf("* face bunch graph matching algorithm to locate points on a face.   *\n");
    printf("*                                                                   *\n");
    printf("*********************************************************************\n");
    printf("\n");
    printf("Arguments:\n");
    printf("  image_list.srt   - image list that contains all images that need\n");
    printf("                     face graphs.\n");
    printf("  image_dir        - directory containing test imagry.\n");
    printf("  novel_graph_dir  - the directory that contains the graph\n");
    printf("                     generated by csuGaborGraphFit.\n");
    printf("  wavelet_file     - gabor wavelet discription file.\n");
    printf("  facegraph_dir    - output directory where face graphs are stored.\n");
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
    args->imageFile       = NULL;
    args->masksFile       = NULL;

    debuglevel = 0;

    /******* Read command line arguments *******/

    for (i = 1;i < argc;i++) {

        /* Catch common help requests */
        if      (readOption      (argc, argv, &i, "-help" )) { usage(argv[0]); }
        else if (readOption      (argc, argv, &i, "--help")) { usage(argv[0]); }

        /* other flags */
        else if (readOption    (argc, argv, &i, "-quiet")) { quiet = 1; }
        else if (readOption    (argc, argv, &i, "-dcfree")) { WiskottDCFree = 1.0; }
        else if (readOptionInt (argc, argv, &i, "-debuglevel", &debuglevel)) {}

        /* check if the current argument is an unparsed option */
        else if (checkBadOption(argc,argv,&i)) {}

        /* read required arguments */
        else if (param_num == 0) {
            args->imageFile = argv[i];
            param_num++;
        }
        else if (param_num == 1) {
            args->imageDir = argv[i];
            param_num++;
        }
        else if (param_num == 2) {
            args->graphDir = argv[i];
            param_num++;
        }
        else if (param_num == 3) {
            args->masksFile = argv[i];
            param_num++;
        }
        else if (param_num == 4) {
            args->outputDir = argv[i];
            param_num++;
        }
        else{ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    }

    /* make sure that there are the proper number of required arguments */
    if (param_num != 5){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    DEBUG(1,"Debuging enabled");
}


int main(int argc, char** argv){
    Arguments args;
    ImageList *imagenames, *subject, *replicate;
    JetMasks masks;
    GraphDiscription gd;
    Image face;
    FaceGraph graph;
    int i;
    int imagenum = 0, numImage;
    
    processCommand(argc,argv, &args);

    masks = readMasksFile(args.masksFile);
    imagenames = getImageNames(args.imageFile, &numImage);
    
    for(subject = imagenames; subject != NULL; subject = subject->next_subject){
        for(replicate = subject; replicate != NULL; replicate = replicate->next_replicate){
            imagenum++;
            printf("Processing: %s (%5d of %5d) %5.2f%%   \n" , replicate->filename, imagenum, numImage, imagenum*100.0/numImage); fflush(stdout);

            gd = readGraphDiscription(makePath(args.graphDir,replicate->filename));
            face = readRawImage(makePath(args.imageDir,replicate->filename));
            graph = makeFaceGraph(gd->numVert, gd->numVert+gd->numEdge);
            graph->params =masks->params;

            for( i = 0; i < gd->numVert; i++){
                graph->jets[i] = extractJet(gd->verts[i].x,gd->verts[i].y,face,masks);
            }

            for( i = 0; i < gd->numEdge; i++){

                double x = 0.5*(gd->verts[gd->edges[i].vert1].x + gd->verts[gd->edges[i].vert1].x);
                double y = 0.5*(gd->verts[gd->edges[i].vert1].y + gd->verts[gd->edges[i].vert1].y);

                graph->jets[ gd->numVert + i ] = extractJet(x,y,face,masks);
            }

            saveFaceGraph(makePath(args.outputDir,replicate->filename),graph);
            freeImage(face);
            freeGraphDiscription(gd);
            freeFaceGraph(graph);
        }
    }
    printf("\n");

    return 0;
}


