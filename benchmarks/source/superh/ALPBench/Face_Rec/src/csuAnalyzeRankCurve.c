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


/**
  File:    csuRankCurve.c
  Authors: J. Ross Beveridge, David S. Bolme
  Date:    May 24, 2002                                                  
  ============================================================================
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

/*  csuRankCurve reads distances between probe and gallery images and generates
    a single new text file that lists recognition rates at different ranks.
*/

#define OPENING "Cumulative Match Curve from Probe, Gallery and Distances Files."

/*  STANDARD INCLUDES */
#include <stdio.h>
#include <stdlib.h> 
/*#include <sys/types.h>*/
/*#include <sys/stat.h>*/
/*#include <fcntl.h>*/
#include <string.h>
#include <math.h>
#include <assert.h>

/*  CSU INCLUDES */
#include <csuCommon.h>

/* Global flags */
/* The command line arguments are managed by a single structure. */
typedef struct {
    char* probeNamesFile;
    char* galleryNamesFile;
    char** distDirs;
    int numDistDirs;
    char* outFile;
    char* outDir;
    char* imDir;
    int idWidth;
    int argc;
    char **argv;
}
Arguments;


/* usage
 
   Remind the user the usage of running this program.
   The command of running the program should be: run and the index of the training sets. 
   All the other sets except the training sets will go to testing sets automatically.
 
   INPUT:  prog is the excutable program name.
*/
void usage(const char* prog) {
    printf("Usage: %s [OPTIONS] probesNamesFile.[list/srt] galleryNamesFile.[list/srt] [distDir]+ \n", prog);
    printf("  Parameters\n");
    printf("     probeNamesFile....... Name of file containing probe images names.\n");
    printf("     galleryNamesFile .... Name of file containing gallery image names.\n");
    printf("     [distDir]+ .......... Directories where the distance file are stored.\n");
    printf("    -filePrefix <name> ... Prefix to the name of the cumulative match curve output file. DEFAULT = Rank\n");
    printf("    -outDir <dir> ........ Output file directory. DEFAULT = \".\"\n");
    printf("    -imDir <dir> ......... Image directory. DEFAULT = \".\"\n");
    printf("    -idWidth <int> ....... Assume subject ID is fixed width prefix of file name. DEFAULT = 5\n");
    printf("    -debuglevel <int> .... Level of debug information to display. DEFAULT = 0\n");
    printf("    -quiet ............... Turn off all messages. DEFAULT = messages on\n");

    exit(1);
}

/*
   process_command
 
   process the command line arguments.  see usage for a description of functionality.
 */
void process_command(int argc, char** argv, Arguments* args) {
    int i;
    int param_num;

    param_num = 0;

    args->argc = argc;
    args->argv = argv;

    if (argc < 3)
        usage(argv[0]);
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0) { usage(argv[0]); exit(1); }
    }

    /* Allocate space for character arrays that are part of the arguments structure */
    args->probeNamesFile = (char*) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
    args->galleryNamesFile = (char*) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
    args->distDirs = (char**) malloc(sizeof(char*) * argc);

    /* Setup Default Values */
    args->imDir = strdup(".");
    args->outDir = strdup(".");
    args->outFile = strdup("Rank");

    args->idWidth = 5;
    quiet = 0;
    debuglevel = 0;
    args->numDistDirs = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-imDir") == 0) {
            DEBUG_CHECK(++i < argc, "Bad parameter list");
            free(args->imDir);
            args->imDir = strdup(argv[i]);
        } else if (strcmp(argv[i], "-outDir") == 0) {
            DEBUG_CHECK(++i < argc, "Bad parameter list");
            free(args->outDir);
            args->outDir = strdup(argv[i]);
        } else if (strcmp(argv[i], "-filePrefix") == 0) {
            DEBUG_CHECK(++i < argc, "Bad parameter list");
            free(args->outFile);
            args->outFile = strdup(argv[i]);
        } else if (strcmp(argv[i], "-debuglevel") == 0) {
            DEBUG_CHECK(++i < argc, "Bad parameter list");
            debuglevel = atoi(argv[i]);
        } else if (strcmp(argv[i], "-idWidth") == 0) {
            DEBUG_CHECK(++i < argc, "Bad parameter list");
            args->idWidth = atoi(argv[i]);
        } else if (strcmp(argv[i], "-quiet") == 0) {
            quiet = 1;
            continue;
        } else if (argv[i][0] == '-') {
            printf("Unknown option: %s\n", argv[i]);
            usage(argv[0]);
        } else if (param_num == 0) {
            args->probeNamesFile = strdup(argv[i]);
            param_num++;
        } else if (param_num == 1) {
            args->galleryNamesFile = strdup(argv[i]);
            param_num++;
        } else if (param_num >= 2) {
            args->distDirs[args->numDistDirs] = strdup(argv[i]);
            args->numDistDirs++;
            param_num++;
        }
    }

    if (param_num < 3)
        usage(argv[0]);

    /* Print out the program parameters for appropreate debug level */
    DEBUG_INT ( 1, "Debuging enabled", debuglevel);
    DEBUG_STRING( 3, "Parameter: ", args->probeNamesFile);
    DEBUG_STRING( 3, "Parameter: ", args->galleryNamesFile);
    DEBUG_INT ( 3, "Parameter: ", args->numDistDirs);
    for (i = 0; i < args->numDistDirs; i++) {
        DEBUG_STRING( 3, "Parameter: ", args->distDirs[i]);
    }
    DEBUG_STRING( 3, "Parameter: ", args->outFile);
    DEBUG_STRING( 3, "Parameter: ", args->outDir);
    DEBUG_STRING( 3, "Parameter: ", args->imDir);
    DEBUG_INT ( 3, "Parameter: ", args->idWidth);
}


/* sameSubject
 *
 * Compares two image names to determine if they corrispond to the same subject.
 */
int sameSubject(char* name1, char* name2, int width) {
    int len1, len2, i, flag;

    len1 = strlen(name1);
    len2 = strlen(name2);

    DEBUG_CHECK(len1 > width, "Name shorter than specified prefix size found.");
    DEBUG_CHECK(len2 > width, "Name shorter than specified prefix size found.");

    flag = 1;
    for (i = 0; i < width; i++) {
        if (name1[i] != name2[i])
            flag = 0;
    }

    return flag;
}


/*  isInImageList
 *
 *  This checks to see if filename is included in the ImageNames list. This is done by traversing
 *  the list and determining if the filenames match.
 */
int isInImagelist(char* filename, ImageList* ImageNames) {
    ImageList *subject, *replicate;

    for (subject = ImageNames; subject != NULL ; subject = subject->next_subject) {
        for (replicate = subject; replicate != NULL ; replicate = replicate->next_replicate) {
            if (strcmp(filename, replicate->filename) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

/*  getProbeRank
 *
 *   This function reads the distance file for a probe image and determines the rank of the
 *   nearest image of the same subject.  The gallery distances are stored in an array.  As
 *   they are read in the distance to the closest image of the same subject is determined.
 *   The array is then scanned again to determine the number of images that are closer to
 *   the original (the rank).  The file is then closed and the rank is returned.  
 */
typedef struct {
    char* galleryName;
    double distance;
}
DistanceRec;

int getProbeRank(char* distDir, char* probeName, int numGalleryImages,
                 ImageList* galleryImageNames, DistanceRec* distRecs, int width) {
    int i;
    FILE* distanceFile = NULL;
    char filename[MAX_FILENAME_LENGTH];
    char galleryName[MAX_FILENAME_LENGTH];
    double distance;
    double minDistance;
    int rank = -1;
    int noMinFound = 1;

    sprintf(filename, "%s/%s", distDir, probeName);

    if (debuglevel > 0)
        fprintf(stdout, "Reading probe distance file: %s ", filename);
    fflush(stdout);

    distanceFile = fopen(filename, "r");

    if(!distanceFile){
        printf("Error opening distance file: %s", filename);
        exit(1);
    }

    minDistance = 0.0;
    noMinFound = 1;
    i = 0;
    while ((fscanf(distanceFile, "%s %lf", galleryName, &distance) == 2) && (i < numGalleryImages)) {

        if ( !distRecs[i].galleryName && isInImagelist(galleryName, galleryImageNames) ) {
            distRecs[i].galleryName = strdup(galleryName);
            if (sameSubject(probeName, galleryName, width)) {
                if (noMinFound) {
                    minDistance = distance; noMinFound = 0;}
                else {
                    minDistance = MIN(minDistance , distance); }
            }
            distRecs[i].distance = distance;
            i++;
        } else if ( distRecs[i].galleryName && strcmp(distRecs[i].galleryName, galleryName) == 0 ) {
            if (sameSubject(probeName, galleryName, width)) {
                if (noMinFound) {
                    minDistance = distance;
                    noMinFound = 0;
                } else {
                    minDistance = minDistance < distance ? minDistance : distance;
                }
            }
            distRecs[i].distance = distance;
            i++;
        }
    }

    if (debuglevel > 0)
        printf("MinDistance: %f ", minDistance);
    DEBUG_CHECK(i == numGalleryImages, "Error: All distance files must list images in the same order");

    fclose(distanceFile);

    rank = 0;

    for (i = 0; i < numGalleryImages; i++) {
        if (!sameSubject(probeName, distRecs[i].galleryName, width) && distRecs[i].distance <= minDistance)
            rank++;
    }

    if (debuglevel > 0)
        fprintf(stdout, "Rank: %d\n", rank);
    fflush(stdout);

    return rank;
}


/*  allocate/free RankCurves
 *
 *  These functions allocates a two dimensional array used for storing the cumlative
 *  rank curves.
 */

int** allocateRankCurves(int numDistDirs, int numGalleryImages) {
    int **rankcurve;
    int i, j;

    rankcurve = (int**)malloc(sizeof(int*) * numDistDirs);
    DEBUG_CHECK(rankcurve, "Error allocating memory");

    for ( j = 0; j < numDistDirs; j++) {
        rankcurve[j] = (int*) malloc( sizeof(int*) * numGalleryImages );
        DEBUG_CHECK(rankcurve[j], "Error allocating memory");
        for ( i = 0; i < numGalleryImages; i++) {
            rankcurve[j][i] = 0;
        }
    }
    return rankcurve;
}

void freeRankCurves(int **rankcurve, int numDistDirs, int numGalleryImages) {
    int j;

    for ( j = 0; j < numDistDirs; j++) {
        free(rankcurve[j]);
    }

    free(rankcurve);
}

/* buildRankCurves
 *
 * This function reads in each probe file consecutivly and builds rank curves for
 * each distance directory. 
 */
void buildRankCurves(ImageList* probeImageNames, int numProbeImages,
                     ImageList* galleryImageNames, int numGalleryImages, Arguments *args) {
    int i, j;
    int rank;
    int** rankcurve;
    ImageList* subject;
    ImageList* replicate;
    FILE* imageRankFile;
    FILE* cumulativeRankFile;
    DistanceRec* distRecs = (DistanceRec*)malloc(sizeof(DistanceRec) * numGalleryImages);
    char filename[MAX_FILENAME_LENGTH];

    /* Set up the rank curve and output files */
    sprintf(filename, "%s/%s_Images.txt", args->outDir, args->outFile);
    imageRankFile = fopen(filename, "w");
    DEBUG_CHECK(imageRankFile, "Error opening file");

    sprintf(filename, "%s/%s_Curve.txt", args->outDir, args->outFile);
    cumulativeRankFile = fopen(filename, "w");
    DEBUG_CHECK(cumulativeRankFile, "Error opening file");

    /* Allocate Memory for the rank curve data */
    rankcurve = allocateRankCurves(args->numDistDirs, numGalleryImages);

    /* Build up the rank information */
    DEBUG_CHECK(distRecs, "Error Allocating Memory.");
    for (i = 0; i < numGalleryImages; i++) {
        distRecs[i].galleryName = NULL;
        distRecs[i].distance = 0.0;
    }
    /* Print out file headers */
    fprintf(imageRankFile, "%s", "ProbeName");
    for (i = 0; i < args->numDistDirs; i++) {
        fprintf(imageRankFile, "\t%s", args->distDirs[i]);
    }
    fprintf(imageRankFile, "\n");

    for (subject = probeImageNames; subject != NULL ; subject = subject->next_subject) {
        for (replicate = subject; replicate != NULL ; replicate = replicate->next_replicate) {
            printf("Processing Probe: %s       \r",replicate->filename);
            fprintf(imageRankFile, "%s", replicate->filename);
            for (i = 0; i < args->numDistDirs; i++) {
                int rank = getProbeRank(args->distDirs[i], replicate->filename, numGalleryImages, galleryImageNames, distRecs, args->idWidth);
                rankcurve[i][rank]++; /* add to the ranking histogram */
                fprintf(imageRankFile, "\t%d", rank);
            }
            fprintf(imageRankFile, "\n");
        }
    }
    free(distRecs);

    /* Output the final rank curve information. */
    fprintf(cumulativeRankFile, "%s", "Rank");
    for (j = 0; j < args->numDistDirs; j++) {
        fprintf(cumulativeRankFile, "\t%s", args->distDirs[j]);
        fprintf(cumulativeRankFile, "\t%s", args->distDirs[j]);
    }
    fprintf(cumulativeRankFile, "\n");

    rank = 0;
    for ( i = 0; i < numGalleryImages; i++) {
        fprintf(cumulativeRankFile, "%d", i);
        for (j = 0; j < args->numDistDirs; j++) {
            if (i != 0)
                rankcurve[j][i] += rankcurve[j][i - 1];
            fprintf(cumulativeRankFile, "\t%d", rankcurve[j][i]);
            fprintf(cumulativeRankFile, "\t%f", ((float)rankcurve[j][i]) / numProbeImages);
        }
        fprintf(cumulativeRankFile, "\n");
    }

    /* clean up the files */
    freeRankCurves(rankcurve, args->numDistDirs, numGalleryImages);
    fclose(cumulativeRankFile);
    fclose(imageRankFile);
}

/* main */
int main(int argc, char *argv[]) {

    Arguments args;
    ImageList *probeImageNames, *galleryImageNames;
    int numProbeImages, numGalleryImages;


    MESSAGE(OPENING);
    MESSAGE(VERSION);

    process_command(argc, argv, &args);

    probeImageNames = getImageNames(args.probeNamesFile, &numProbeImages);
    galleryImageNames = getImageNames(args.galleryNamesFile, &numGalleryImages);

    buildRankCurves(probeImageNames, numProbeImages, galleryImageNames, numGalleryImages, &args);

    printf("\nFinished. \n");
    return 0;
}

