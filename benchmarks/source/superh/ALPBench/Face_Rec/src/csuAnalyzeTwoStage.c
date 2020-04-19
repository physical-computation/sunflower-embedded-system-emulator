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
File: csuAnalyzePostprocessing.c                                                     
Authors: Marcio L. Teixeira                           
Date: May 24, 2002                                                       
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

/** @file
 Purpose: This program performs an experiment where the performance of
 a two-stage classifier is examined
*/

#define OPENING  "Performs a two-stage classifier test"

/******************************************************************************
*                               INCLUDES                                      *
******************************************************************************/

/*  CSU INCLUDES */
#include <csuCommon.h>

/******************************************************************************
*                               GLOBALS                                       *
******************************************************************************/

/* Global flags */

/**
 * The command line arguments are managed by a single structure.
 */
typedef struct {
  char* allImageList;

  char* probeImageList;
  char* galleryImageList;

  char* stageOneDistanceMatrix;
  char* stageTwoDistanceMatrix;
  
  char* resultsFile;

  int   maxRank;
}
Arguments;

/**
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void usage(const char* name) {
    printf ("Usage: %s [OPTIONS] all.srt gallery.srt probe.srt stageOneDistanceMatrix stageTwoDistanceMatrix prefix"
	   "imageNamesFile.[list/srt] maxLikelihoodDistDirectory bayesianDistDirectory\n", name);
    printf ("  Parameters:\n");
    printf ("     allImageList ....      List of combined probe and gallery images\n"); 
    printf ("     probeImageList ....    List of probe images\n");
    printf ("     galleryImageList ....  List of gallery images\n");
    printf ("     stageOneDistanceMatrix ....    Distance matrix to be used for stage one\n");
    printf ("     stageTwoDistanceMatrix ....    Distance matrix to be used for stage two\n");
    printf ("     prefix ....                    Prefix of the external data\n");
    printf ("    -maxRank n          Compute rank curve up to this value (DEFAULT=50)\n");
    printf ("    -debuglevel <int>:  Level of debug information to display. DEFAULT = 0\n");
    printf ("    -quiet:             Turn off all messages. DEFAULT = messages on\n");

    exit(0);
}

/**
 * Process the command line and initialize the variables
 *
 * @param argc The number of arguments
 * @param argv The arguments
 */
void
process_command(int argc, char** argv, Arguments* args)
{
  int i;
  int param_num = 0;

  /******* Set up default values *******/

  args->allImageList   = NULL;
  args->probeImageList   = NULL;
  args->galleryImageList = NULL;

  args->stageOneDistanceMatrix = NULL;
  args->stageTwoDistanceMatrix = NULL;

  args->resultsFile = NULL;

  args->maxRank = -1;

  quiet = 0;
  debuglevel = 0;

  for (i = 1; i < argc; i++) {

    /* Catch common help requests */
    if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
    else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }

    /* other flags */
    else if (readOption       (argc, argv, &i, "-quiet")) { quiet = 1; }
    else if (readOptionInt    (argc, argv, &i, "-debuglevel", &debuglevel));

    /* Read in number of images to generate */
    else if (readOptionInt    (argc, argv, &i, "-maxRank", &(args->maxRank)));

    /* check if the current argument is an unparsed option */
    else if (checkBadOption(argc,argv,&i));

    else if (param_num == 0) {
      args->allImageList = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 1) {
      args->galleryImageList = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 2) {
      args->probeImageList = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 3) {
      args->stageOneDistanceMatrix = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 4) {
      args->stageTwoDistanceMatrix = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 5) {
      args->resultsFile = strdup(argv[i]);
      param_num++;
    }
  }

  if (param_num < 5)
    usage(argv[0]);

  /* Print out the program parameters for appropriate debug level */

  DEBUG_INT (1, "Debuging enabled", debuglevel);

  if(debuglevel > 0){
    printf ("***************** Program Parameters *********************\n");
    printf ("allImageList: %s\n", args->allImageList);
    printf ("galleryImageList: %s\n", args->galleryImageList);
    printf ("probeImageList: %s\n", args->probeImageList);
    printf ("stageOneDistanceMatrix: %s\n", args->stageOneDistanceMatrix);
    printf ("stageTwoDistanceMatrix: %s\n", args->stageTwoDistanceMatrix);
  }
}

/******************************************************************************
*                            MAIN PROGRAM                                     *
******************************************************************************/

typedef struct {
  char **nameArray;
  int *subjArray;
} SubjectComparator;

SubjectComparator*
getSubjectComparator (char *allSrt)
{
  FILE *fi;
  void *nameList = NULL;
  void *subjList = NULL;
  SubjectComparator *comparator;
  unsigned int subjId;
  size_t nImages;
  Tokenizer tok;

  fi = fopen (allSrt, "r");
  DEBUG_CHECK_1ARG (fi, "Unable to read image list %s", allSrt);

  tokenizerInit (&tok, tokenizerStreamReader, fi);

  subjId = 0;

  while (!tokenizerEndOfFile (&tok))
    {
      char *subjName = strdup (tokenizerGetWord (&tok));
      listAccumulate (&nameList, &subjName, sizeof (char *));
      listAccumulate (&subjList, &subjId,   sizeof (int));

      while ((!tokenizerEndOfLine (&tok)) && (!tokenizerEndOfFile (&tok)))
	{
	  subjName = strdup (tokenizerGetWord (&tok));
	  listAccumulate (&nameList, &subjName, sizeof (char *));
	  listAccumulate (&subjList, &subjId,   sizeof (int));
	}

      writeProgress ("Reading subjects list", subjId, 0);
      subjId++;
    }

  fclose (fi);

  comparator = (SubjectComparator*) malloc (sizeof (SubjectComparator));
  comparator->nameArray = listToNullTerminatedArray (&nameList, sizeof (char *), &nImages);
  comparator->subjArray = listToNullTerminatedArray (&subjList, sizeof (int),    &nImages);
  return comparator;
}

int
getSubjectId (SubjectComparator *comparator, char *subject)
{
  int i;

  for (i = 0; comparator->nameArray[i] != NULL; i++)
    if (!strcmp (comparator->nameArray[i], subject))
      break;

  return comparator->subjArray[i];
}

/* ===========================================================================
 MAIN
*/
int main(int argc, char *argv[]) 
{
  Arguments args;
  SubjectComparator *comparator;
  ListOfStrings probeImages, galleryImages;
  int *stageOneRankCurve, *stageTwoRankCurve, *combinedRankCurve, *indices, *subjIds;
  int nProbe, nGallery, rankOfProbe, i, r, j;
  FILE *fo;
  char buff[256];

  MESSAGE(OPENING);
  MESSAGE(VERSION);
  process_command(argc, argv, &args);

  /* Sanity check */

  checkReadableFile (args.allImageList, "Cannot read the combined image list %s");
  checkReadableFile (args.galleryImageList, "Cannot read the gallery image list %s");
  checkReadableFile (args.probeImageList,   "Cannot read the probe image list %s");

  /* Begin execution */
  
  MESSAGE1ARG("Reading combined image list from %s", args.allImageList);
  
  comparator = getSubjectComparator (args.allImageList);

  MESSAGE1ARG("Reading probe image list from %s", args.probeImageList);

  probeImages = readListOfStrings (args.probeImageList, &nProbe);

  MESSAGE1ARG("Reading gallery image list from %s", args.galleryImageList);

  galleryImages = readListOfStrings (args.galleryImageList, &nGallery);

  if (args.maxRank == -1 || args.maxRank > nGallery)
    args.maxRank = nGallery;

  /* Compute the rank curves */

  stageOneRankCurve = (int*) malloc (args.maxRank * sizeof (int));
  stageTwoRankCurve = (int*) malloc (args.maxRank * sizeof (int));
  combinedRankCurve = (int*) malloc (args.maxRank * sizeof (int));

  indices = (int*) malloc (nGallery * sizeof (int));
  subjIds = (int*) malloc (nGallery * sizeof (int));

  for (i = 0; i < args.maxRank; i++)
    {
      stageOneRankCurve[i] = 0;
      stageTwoRankCurve[i] = 0;
      combinedRankCurve[i] = 0;
    }

  for (i = 0; i < nProbe; i++)
    {
      char *probe = probeImages [i];
      int leastRanked, probeSubj = getSubjectId (comparator, probe);

      sortSubjectsBySimilarityToProbe (probe, galleryImages, args.stageOneDistanceMatrix, NULL);
      sortSubjectsBySimilarityToProbe (probe, galleryImages, args.stageTwoDistanceMatrix, indices);

      /* Look up subject ids */

      for (j = 0; j < nGallery; j++)
	subjIds [j] = getSubjectId (comparator, galleryImages[j]);


      /* Stage one cumulative rank curve */

      rankOfProbe = args.maxRank;
      for (r = 0; r < args.maxRank; r++)
	  if (probeSubj == subjIds[r])
	    {
	      rankOfProbe = r;
	      break;
	    }

      for (r = rankOfProbe; r < args.maxRank; r++)
	stageOneRankCurve[r]++;

      /* Stage two cumulative rank curve */

      rankOfProbe = args.maxRank;
      for (r = 0; r < args.maxRank; r++)
	  if (probeSubj == subjIds[indices[r]])
	    {
	      rankOfProbe = r;
	      break;
	    }

      for (r = rankOfProbe; r < args.maxRank; r++)
	stageTwoRankCurve[r]++;

      /* Two-stage rank-one curve */

      for (r = 0; r < args.maxRank; r++)
	{
	  for (leastRanked = 0; leastRanked < nGallery; leastRanked++)
	      if (indices[leastRanked] <= r)
		break;

	  if (probeSubj == subjIds[indices[leastRanked]])
	    combinedRankCurve[r]++;
	}

      writeProgress ("Computing the rank curves", i, nProbe);
    }

  /* Save out the results */

  sprintf (buff, "%s_PCARankOne_Curve.txt", args.resultsFile);
  fo = fopen (buff, "w");
  fprintf (fo,"rank PCA PCA\n");
  for (r = 0; r < args.maxRank; r++)
      fprintf (fo, "%d %d %f\n", r+1, stageOneRankCurve[0], ((double)stageOneRankCurve[0])/nProbe);
  fclose (fo);

  sprintf (buff, "%s_BayesianRankOne_Curve.txt", args.resultsFile);
  fo = fopen (buff, "w");
  fprintf (fo,"rank Bayesian Bayesian\n");
  for (r = 0; r < args.maxRank; r++)
      fprintf (fo, "%d %d %f\n", r+1, stageTwoRankCurve[0], ((double)stageTwoRankCurve[0])/nProbe);
  fclose (fo);

  sprintf (buff, "%s_PCA_Curve.txt", args.resultsFile);
  fo = fopen (buff, "w");
  fprintf (fo,"rank PCA PCA\n");
  for (r = 0; r < args.maxRank; r++)
      fprintf (fo, "%d %d %f\n", r+1, stageOneRankCurve[r], ((double)stageOneRankCurve[r])/nProbe);
  fclose (fo);

  sprintf (buff, "%s_Bayesian_Curve.txt", args.resultsFile);
  fo = fopen (buff, "w");
  fprintf (fo,"rank Bayesian Bayesian\n");
  for (r = 0; r < args.maxRank; r++)
      fprintf (fo, "%d %d %f\n", r+1, stageTwoRankCurve[r], ((double)stageTwoRankCurve[r])/nProbe);
  fclose (fo);

  sprintf (buff, "%s_CombinedRankOne_Curve.txt", args.resultsFile);
  fo = fopen (buff, "w");
  fprintf (fo,"rank Combined Combined\n");
  for (r = 0; r < args.maxRank; r++)
      fprintf (fo, "%d %d %f\n", r+1, combinedRankCurve[r], ((double)combinedRankCurve[r])/nProbe);
  fclose (fo);

  return 0;
}
