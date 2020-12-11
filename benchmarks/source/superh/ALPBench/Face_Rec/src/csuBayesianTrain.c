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
File:    csuBayesianTrain.c                                                            
Authors: Marcio L. Teixeira                      
Date:    May 30, 2002                                                
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


#define OPENING  "Trains the Bayesian Interpersonal/Extrapersonal Classifier."

#define EACH_SUBJECT(srt, subject)           (subject = srt; subject != NULL; subject = subject->next_subject)
#define EACH_REPLICATE(srt, replicate)       (replicate = srt; replicate != NULL; replicate = replicate->next_replicate)

/******************************************************************************
*                               INCLUDES                                      *
******************************************************************************/

#include <csuCommon.h>

/******************************************************************************
*                               GLOBALS                                       *
******************************************************************************/

typedef struct
{
  char *imageList;
  char *imageDirectory;
  char *trainingFilename;

  char *distanceMatrix;
  int maxRank;

  int nExtrapersonal;
  int nIntrapersonal;

  int cutOffMode;
  double cutOff;
  int dropNVectors;

  int argc;
  char **argv;
}
Arguments;

/**
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void usage(const char* name)
{
  printf ("Usage: %s [OPTIONS] training_images.srt training_output_prefix\n", name);
  printf ("  Parameters\n");
  printf ("    training_images.srt:   Input image names.\n");
  printf ("    training_output_prefix Prefix which is used to save the subspaces\n");
  printf ("  Options\n");
  printf ("    -imDir <dir>:          Image directory.\n"
	  "        DEFAULT = \".\"\n");
  printf ("    -ni <n>:               Total number of interpersonal images.\n"
	  "        DEFAULT = 100\n");
  printf ("    -ne <n>:               Total number of extrapersonal images.\n"
	  "        DEFAULT = 100\n");
  printf ("    -distances <file>:     Sort images using this distance matrix.\n"
	  "        DEFAULT = \"Random\"\n");
  printf ("    -maxRank n             When sorting, take these many top-ranked images\n");
  printf("     -dropNVectors <int>: Drops the first N vectors which are normally lighting effects.\n        DEFAULT = 0\n");
  printf("     -cutOffMode <mode>:  Selects the method for eigen vector selection after PCA trianing\n");
  printf("                          and droping the first vectors.  DEFAULT = SIMPLE\n");
  printf("          NONE            Retain eigenvectors.\n");
  printf("          SIMPLE          Retain a percentage of the eigenvectors. \n");
  printf("                          Expects value between 0.0 and 100.0, DEFAULT PERCENT = %f \n", DEFAULT_CUTOFF_PERCENT_SIMPLE);
  printf("          ENERGY          Retain  eigenvectors accounting for a percentage of the total energy.\n");
  printf("                          Expects value between 0.0 and 100.0, DEFAULT PERCENT = %f\n", DEFAULT_CUTOFF_PERCENT_ENERGY);
  printf("          STRETCH         Retains all eigenvectors greater than a percentage of the largest eigenvector.\n");
  printf("                          Expects value between 0.0 and 100.0, DEFAULT PERCENT = %f\n", DEFAULT_CUTOFF_PERCENT_STRETCH);
  printf("          CLASSES         Retains as many eigenvectors as there LDA Classes: use only with LDA.\n");
  printf("                          Ignores cutOff value and uses number of classes instead\n");
  printf("     -cutOff <percent>:   Percentage of eigen vectors to retain (see cutOffMode).\n        DEFAULT = (See cutoff mode)\n");
  printf ("    -quiet:                Turn off all messages.\n"
	  "        DEFAULT = messages on\n");
  printf ("    -debuglevel <int>:     Level of debug information to display"
	  "        (automatically sets quiet to no).\n"
	  "        DEFAULT = 0\n");
  exit (1);
}

/**
 * Process the command line and initialize the variables
 *
 * @param argc The number of arguments
 * @param argv The arguments
 */
void
process_command (int argc, char **argv, Arguments * args)
{
  int i;
  int param_num = 0;
  int cutOffSet = 0;

  /******* Set up default values *******/

  args->argc = argc;
  args->argv = argv;

  args->nIntrapersonal = 100;
  args->nExtrapersonal = 100;
  args->distanceMatrix = NULL;
  args->maxRank        = -1;

  args->cutOffMode      = CUTOFF_SIMPLE;
  args->cutOff          = DEFAULT_CUTOFF_PERCENT_SIMPLE;
  args->dropNVectors    = 0;

  debuglevel = 0;

  /******* Read command line arguments *******/

  for (i = 1; i < argc; i++) {

    /* Catch common help requests */
    if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
    else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }

    /* Read in input directories */
    else if (readOptionString (argc, argv, &i, "-imDir",     &(args->imageDirectory)));
    else if (readOptionString (argc, argv, &i, "-distances", &(args->distanceMatrix)));

    /* Read in number of images to generate */
    else if (readOptionInt    (argc, argv, &i, "-ni", &(args->nIntrapersonal)));
    else if (readOptionInt    (argc, argv, &i, "-ne", &(args->nExtrapersonal)));

    /* Read in maxRank */
    else if (readOptionInt    (argc, argv, &i, "-maxRank", &(args->maxRank)));

    /* Read in PCA training options */
    else if (readOptionInt    (argc, argv, &i, "-dropNVectors", &(args->dropNVectors)));

    else if (readOptionMatch(argc, argv, &i, "-cutOffMode", "NONE"))
      { args->cutOffMode = CUTOFF_NONE; }
    else if (readOptionMatch(argc, argv, &i, "-cutOffMode", "SIMPLE"))
      { args->cutOffMode = CUTOFF_SIMPLE; }
    else if (readOptionMatch(argc, argv, &i, "-cutOffMode", "ENERGY"))
      { args->cutOffMode = CUTOFF_ENERGY; }
    else if (readOptionMatch(argc, argv, &i, "-cutOffMode", "STRETCH"))
      { args->cutOffMode = CUTOFF_STRETCH; }
    else if (readOptionMatch(argc, argv, &i, "-cutOffMode", "CLASSES"))
      { args->cutOffMode = CUTOFF_CLASSES; }

    else if (readOptionDouble (argc, argv, &i, "-cutOff", &(args->cutOff)))
      {
	if ((args->cutOff <= 1.0) && (args->cutOff >= 0.000001)) {
	  printf("WARNING: cutOff value is %f, expects percentage "
		 "between 0.0 and 100.0", args->cutOff);
	  printf("         will proceed assuming value is as desired.");
	}
	cutOffSet = 1;
      }   

    /* other flags */
    else if (readOption       (argc, argv, &i, "-quiet")) { quiet = 1; }
    else if (readOptionInt    (argc, argv, &i, "-debuglevel", &debuglevel));

    /* check if the current argument is an unparsed option */
    else if (checkBadOption(argc,argv,&i));

    /* read required arguments */ 
    else if (param_num == 0) {
      args->imageList = argv[i];
      param_num++;
    }
    else if (param_num == 1) {
      args->trainingFilename = strdup (argv[i]);
      param_num++;
    } 
  }

  /* make sure that there are the proper number of required arguments */

  if (param_num != 2){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

  /* Print out the program parameters for appropriate debug level */

  DEBUG_INT (1, "Debuging enabled", debuglevel);
  if(debuglevel > 0){
    printf("***************** Program Parameters *********************\n");
    printf ("Image directory:  %s\n", args->imageDirectory);
    printf ("Image list:       %s\n", args->imageList);
    printf ("nIntrapersonal:   %d\n", args->nIntrapersonal);
    printf ("nExtrapersonal:   %d\n", args->nExtrapersonal);
    printf ("distanceMatrix:   %s\n ", args->distanceMatrix);
  }
}

/******************************************************************************
*                             DIFFERENCE IMAGE ROUTINES                       *
******************************************************************************/

void
makeDifferenceImages (char *imageDirectory, char *imageList, char *distanceMatrix, int maxRank, int reqNIntra, int reqNExtra, Matrix *intrapersonal, Matrix *extrapersonal)
{
  ImageList* imlist;
  void *nameList = NULL;
  char **nameArray;
  void *subjList = NULL;
  int *subjArray, *shuffledIndices, **sortedBySimilarityToProbe;
  int subjId, probeIdx, galleryIdx, idx, nIntrapersonal, nExtrapersonal, rank, i, numPixels;
  int nImages;
  /* size_t trash; */
  Matrix sourceImages, intraImages, extraImages;
  ImageList *replicate, *subject;
  char *subjName;

  /* Read in a list of all the images */

  imlist = getImageNames (imageList, &nImages);
  subjId = 0;
  for EACH_SUBJECT (imlist, subject) {
      for EACH_REPLICATE (subject, replicate) {
        subjName = strdup (replicate->filename);
	listAccumulate (&nameList, &subjName, sizeof (char *));
	listAccumulate (&subjList, &subjId,   sizeof (int));
	writeProgress ("Reading subjects list", subjId, 0);
      }
      subjId++;
  }

  nameArray = listToNullTerminatedArray (&nameList, sizeof (char *), NULL);
  subjArray = listToNullTerminatedArray (&subjList, sizeof (int),    NULL);

  /* Allocate storage for source images and difference images */

  numPixels = autoFileLength (makePath (imageDirectory, nameArray[0]));

  sourceImages = makeMatrix (numPixels, nImages);
  intraImages  = makeMatrix (numPixels, reqNIntra);
  extraImages  = makeMatrix (numPixels, reqNExtra);

  DEBUG_CHECK (sourceImages != NULL, "Not enough memory to allocate matrix");
  DEBUG_CHECK (intraImages  != NULL, "Not enough memory to allocate matrix");
  DEBUG_CHECK (extraImages  != NULL, "Not enough memory to allocate matrix");

  /* Load in all the source images */

  for (i = 0; i < nImages; i++) {
    readFile (makePath (imageDirectory, nameArray[i]), i, sourceImages);
    writeProgress ("Loading source images", i, nImages);
  }

  /* Write out difference images */

  shuffledIndices = shuffledNumbers (nImages);

  idx = 0;
  rank = 0;
  nIntrapersonal = 0;
  nExtrapersonal = 0;

  if (maxRank == -1)
      maxRank = nImages;

  /* First, for each image generate a list of every other image
   * sorted by similarity to that image */

  sortedBySimilarityToProbe = (int**) malloc  (nImages * sizeof (int*));
  for (probeIdx = 0; probeIdx < nImages; probeIdx++) {
    sortedBySimilarityToProbe[probeIdx] = (int*) malloc  (nImages * sizeof (int));
    if (sortedBySimilarityToProbe[probeIdx] == 0L)
      {
	fprintf (stderr, "Not enough memory to continue\n");
	exit (1);	
      }
    sortSubjectsBySimilarityToProbe (nameArray[probeIdx], nameArray, distanceMatrix, sortedBySimilarityToProbe[probeIdx]);
    writeProgress ("Sorting images", probeIdx, nImages);
  }

  /* Now write out the difference images */

  while (nIntrapersonal < reqNIntra || nExtrapersonal < reqNExtra)
    {
      probeIdx = shuffledIndices[idx];

      idx++;
      if (idx == nImages)
	{
	  idx = 0;
	  rank++;

	  if (rank == maxRank)
	    {
	      fprintf (stderr, "\n");
	      fprintf (stderr, "WARNING: Unable to generate enough images. Try increasing maxRank or adding more subjects to the list.\n");
	      break;
	    }
	}
      
      galleryIdx = sortedBySimilarityToProbe[probeIdx][rank];
	  
      if (galleryIdx == probeIdx || !strcmp (nameArray[probeIdx], nameArray[galleryIdx]))
	continue;

      /* Compute the difference image */

      if (subjArray[probeIdx] == subjArray[galleryIdx])
	{
	  if(nIntrapersonal < reqNIntra) {
	    for (i = 0; i < sourceImages->row_dim; i++)
	      ME (intraImages, i, nIntrapersonal) = ME (sourceImages, i, probeIdx) - ME (sourceImages, i, galleryIdx);

	    nIntrapersonal++;
	  }
	}
      else
	{
	  if( nExtrapersonal < reqNExtra ) {
	    for (i = 0; i < sourceImages->row_dim; i++)
	      ME (extraImages, i, nExtrapersonal) = ME (sourceImages, i, probeIdx) - ME (sourceImages, i, galleryIdx);

	    nExtrapersonal++;
	  }
	}
      
      writeProgress ("Computing difference images", nIntrapersonal + nExtrapersonal, reqNIntra + reqNExtra);
    }

  fprintf (stdout, "Generated %d extrapersonal images\n", nExtrapersonal);
  fprintf (stdout, "Generated %d intrapersonal images\n", nIntrapersonal);

  /* Clean up */

  for (probeIdx = 0; probeIdx < nImages; probeIdx++)
    free (sortedBySimilarityToProbe[probeIdx]);
  free (sortedBySimilarityToProbe);

  freeMatrix (sourceImages);
  freeListOfStrings (nameArray);
  free (subjArray);
  free (shuffledIndices);

  *extrapersonal = extraImages;
  *intrapersonal = intraImages;
}

/******************************************************************************
*                               MAIN PROGRAM                                  *
******************************************************************************/

/*
    main()
 */
int
main (int argc, char *argv[])
{
  Arguments args;
  Matrix intraImages, extraImages;
  Subspace intraSubspace, extraSubspace;
  char filename[256];

  process_command (argc, argv, &args);
  MESSAGE (OPENING);
  MESSAGE (VERSION);

  /* Sanity check */

  checkReadableDirectory (args.imageDirectory, "%s is not a readable directory");
  checkReadableFile (args.imageList, "Cannot read subject replicates list %s");

  makeDifferenceImages (args.imageDirectory,
			args.imageList,
			args.distanceMatrix,
			args.maxRank,
			args.nIntrapersonal,
			args.nExtrapersonal,
			&intraImages,
			&extraImages
			);

  MESSAGE("Training intrapersonal subspace");

  subspaceTrain (&intraSubspace, intraImages, NULL, args.nIntrapersonal, args.dropNVectors, args.cutOffMode, args.cutOff, 0, 0);

  MESSAGE("Training extrapersonal subspace");

  subspaceTrain (&extraSubspace, extraImages, NULL, args.nExtrapersonal, args.dropNVectors, args.cutOffMode, args.cutOff, 0, 0);

  MESSAGE("Saving intrapersonal training file");

  sprintf (filename, "%s.intra", args.trainingFilename);
  writeSubspace (&intraSubspace, filename, args.imageList, args.argc, args.argv);

  MESSAGE("Saving extrapersonal training file");

  sprintf (filename, "%s.extra", args.trainingFilename);
  writeSubspace (&extraSubspace, filename, args.imageList, args.argc, args.argv);

  MESSAGE("Finished Training.");
  
  return 0;

}
