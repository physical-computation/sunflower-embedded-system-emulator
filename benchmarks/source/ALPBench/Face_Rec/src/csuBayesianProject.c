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
File: csuBayesianProject.c                                                     
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
 Purpose: This program projects face images into intrapersonal and
 extrapersonal subspaces and computes a "distance" score based on the
 maximum likelihood and bayesian estimates. The images are specified
 in a file of image names. These may be grouped either as one
 name per line or several, although for this program there is no
 difference between the two in terms of what is done.  The resulting
 distances are written out to a series of files, one per image, all
 placed in a user specified directory.
*/

/* References:

     [1] Moghaddam and Pentland, "Probabilistic Visual Learning"
     [2] She, Kai. _Master's Thesis_, Chapter 5
     [3] Moghaddam and Pentland, "Beyond Eigenfaces"
 */

#define OPENING  "Project Test Images into intrapersonal and extrapersonal subspace and Compute Pairwise Distances."

/******************************************************************************
*                               INCLUDES                                      *
******************************************************************************/

/*  CSU INCLUDES */
#include <csuCommon.h>

/******************************************************************************
*                               GLOBALS                                       *
******************************************************************************/

/* How many lines in the training file header have useful text */
#define TRAINING_HEADER_ENTRIES 10

/* Global flags */

/**
 * The command line arguments are managed by a single structure.
 */
typedef struct {
  char* intrapersonalTrainingFile;
  char* extrapersonalTrainingFile;
  char* imageNamesFile;
  char* imageDirectory;
  char* maxLikelihoodDistDirectory;
  char* bayesianDistDirectory;
  char* distanceMatrix;
  int   maxRank;
}
Arguments;

/**
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void usage(const char* name) {
    printf ("Usage: %s [OPTIONS] intrapersonalTrainingFile extrapersonalTrainingFile "
	   "imageNamesFile.[list/srt] maxLikelihoodDistDirectory bayesianDistDirectory\n", name);
    printf ("  Parameters:\n");
    printf ("     intrapersonalTrainingFile ....  Name of file containing subspace generated by training\n"
	   "                                     on the intrapersonal images\n");
    printf ("     extrapersonalTrainingFile ....  Name of file containing subspace generated by training\n"
	   "                                     on the extrapersonal images\n");
    printf ("     imageNamesFile...               Name of file containing test images: union of possible probe and gallery sets.\n");
    printf ("     maxLikelihoodDistDirectory ...  Directory where distances files are to be written\n");
    printf ("     bayesianDistDirectory ...       Directory where distances files are to be written\n");
    printf ("    -imDir <dir>:       image directory. DEFAULT = \".\"\n");
    printf ("    -debuglevel <int>:  Level of debug information to display. DEFAULT = 0\n");
    printf ("    -quiet:             Turn off all messages. DEFAULT = messages on\n");
    printf ("    -distances <file>:  Use Bayesian for post-processing. Sort images using this distance matrix, then classify top n candidates.\n");
    printf ("    -maxRank n          Present this many images when post-processing (DEFAULT=20)\n");

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

  args->imageDirectory = NULL;
  args->intrapersonalTrainingFile = NULL;
  args->extrapersonalTrainingFile = NULL;
  args->imageNamesFile = NULL;
  args->maxLikelihoodDistDirectory = NULL;
  args->bayesianDistDirectory = NULL;

  args->distanceMatrix = NULL;
  args->maxRank = 50;

  quiet = 0;
  debuglevel = 0;

  for (i = 1; i < argc; i++) {

    /* Catch common help requests */
    if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
    else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }

    /* Read in input directories */
    else if (readOptionString (argc, argv, &i, "-imDir",     &(args->imageDirectory)));

    /* other flags */
    else if (readOption       (argc, argv, &i, "-quiet")) { quiet = 1; }
    else if (readOptionInt    (argc, argv, &i, "-debuglevel", &debuglevel));

    else if (readOptionString (argc, argv, &i, "-distances", &(args->distanceMatrix)));

    /* Read in number of images to generate */
    else if (readOptionInt    (argc, argv, &i, "-maxRank", &(args->maxRank)));

    /* check if the current argument is an unparsed option */
    else if (checkBadOption(argc,argv,&i));


    else if (param_num == 0) {
      args->intrapersonalTrainingFile = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 1) {
      args->extrapersonalTrainingFile = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 2) {
      args->imageNamesFile = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 3) {
      args->maxLikelihoodDistDirectory = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 4) {
      args->bayesianDistDirectory = strdup(argv[i]);
      param_num++;
    }
  }

  if (param_num < 4)
    usage(argv[0]);

  /* Print out the program parameters for appropriate debug level */

  DEBUG_INT (1, "Debuging enabled", debuglevel);

  if(debuglevel > 0){
    printf ("***************** Program Parameters *********************\n");
    printf ("intrapersonalTrainingFile: %s\n", args->intrapersonalTrainingFile);
    printf ("extrapersonalTrainingFile: %s\n", args->extrapersonalTrainingFile);
    printf ("imageNamesFile: %s\n", args->imageNamesFile);
    printf ("maxLikelihoodDistDirectory: %s\n", args->maxLikelihoodDistDirectory);
    printf ("bayesianDistDirectory: %s\n", args->bayesianDistDirectory);
    printf ("imageDirectory: %s\n", args->imageDirectory);
  }
}

/******************************************************************************
*                            I/O ROUTINES                                     *
******************************************************************************/

void
writeDistanceMatrix (char* distDirectory, Matrix distances, char** images, int nSubjects)
{
  FILE *file;
  int i, j;
  
  for (i = 0; i < nSubjects; i++)
    {
      file = fopen (makePath (distDirectory, images[i]), "w");
      DEBUG_CHECK (file, "Could not write out distance matrix\n");
      for (j = 0; j < nSubjects; j++)
	fprintf (file, "%s\t%.20e\n", images[j], ME(distances, j, i));
      fclose (file);
    }
}

/******************************************************************************
 *           REVISED BAYESIAN PROBABILITY EQUATIONS                           *
 *****************************************************************************/

/* Use nomenclature consistent with Moghaddam and Pentland */

#define CONST_N(s)  (s->numPixels)
#define CONST_M(s)  (s->basis->col_dim)  /* Number of principal components that we kept */
#define CONST_Nt(s) (s->basisDim)

FTYPE
computeRho (Subspace *s)
{
  int i;
  FTYPE tmp = 0.0;

  tmp = 0.0;
  for (i = CONST_M(s); i < CONST_Nt(s); i++)
      tmp += ME(s->values,i,0);

  return tmp / (CONST_Nt(s) - CONST_M(s));
}

FTYPE
computeMaxLikelihood (Matrix delta, Subspace *s, FTYPE rho)
{
  int i;
  Matrix y, deltaCentered;
  FTYPE mahSumOfSquares, ySumOfSquares, deltaSumOfSquares;
  FTYPE ySquared, epsilonSquaredOfX, score;

  /* y is the projection of X into the s. Note that
   * as a side-effect, centerThenProjectImages mean
   * centers deltaCentered */

  deltaCentered = duplicateMatrix(delta);
  y = centerThenProjectImages(s, deltaCentered);

  mahSumOfSquares   = 0.0;
  ySumOfSquares     = 0.0;
  deltaSumOfSquares = 0.0;

  for (i = 0; i < CONST_M(s); i++)
    if (ME(s->values,i,0) != 0.0)
      {
	ySquared = SQR(ME(y,i,0));

	mahSumOfSquares += ySquared / ME(s->values,i,0);
	ySumOfSquares   += ySquared;
      }

  for (i = 0; i < CONST_N(s); i++) 
    deltaSumOfSquares += SQR(ME(deltaCentered,i,0));

  epsilonSquaredOfX = deltaSumOfSquares - ySumOfSquares;
  score = mahSumOfSquares + epsilonSquaredOfX / rho;

  /* Clean up */

  freeMatrix (deltaCentered);
  freeMatrix (y);

  return  score;
}

/******************************************************************************
*                            MAIN PROGRAM                                     *
******************************************************************************/

/* ===========================================================================
 MAIN
 
 The arguments are processed and then the subspaces and related information is
 read from the training files written by csuSubspaceTrain.  The subspace basis
 is read into a matrix.
 
 Once the training information is read, then the images specified in the 
 imageNamesFile are read into the images matrix. This matrix is then mean 
 centered using the mean, or centroid, associated with the training data. 
 Next, the images are projected into both the intrapersonal and extrapersonal
 subspaces and the maximum likelihood and bayesian distances between all
 pairs of images are computed. Finally, these distances are written to files,
 one per image.
*/
int main(int argc, char *argv[]) {
    int nImages, numPixels;
    int i, j, k, n;
    int progress, progressMax;
    Arguments args;
    ListOfStrings images;
    Matrix maxLikelihoodDistances;
    Matrix bayesianDistances;
    Matrix delta;
    Matrix data;
    Subspace intrapersonal;
    Subspace extrapersonal;
    FTYPE iRho, eRho, iLikelihood, eLikelihood;

    MESSAGE(OPENING);
    MESSAGE(VERSION);
    process_command(argc, argv, &args);

    /* Sanity check */

    checkWriteableDirectory (args.bayesianDistDirectory, "%s is not a writeable directory");
    checkWriteableDirectory (args.maxLikelihoodDistDirectory, "%s is not a writeable directory");
    checkReadableFile (args.intrapersonalTrainingFile, "Cannot read the training file %s");
    checkReadableFile (args.extrapersonalTrainingFile, "Cannot read the training file %s");
    checkReadableFile (args.imageNamesFile, "Cannot read the image list %s");

    /* Begin execution */

    MESSAGE1ARG("Reading intrapersonal subspace %s", args.intrapersonalTrainingFile);
    readSubspace (&intrapersonal, args.intrapersonalTrainingFile, quiet);

    MESSAGE1ARG("Reading extrapersonal subspace %s", args.extrapersonalTrainingFile);
    readSubspace (&extrapersonal, args.extrapersonalTrainingFile, quiet);

    MESSAGE1ARG("Reading image list from %s", args.imageNamesFile);

    images = readListOfStrings (args.imageNamesFile, &nImages);

    /* Allocate storage for difference image */

    numPixels = autoFileLength (makePath (args.imageDirectory, images[0]));

    delta                  = makeMatrix (numPixels, 1);
    data                   = makeMatrix (numPixels,nImages);
    maxLikelihoodDistances = makeMatrix (nImages, nImages);
    bayesianDistances      = makeMatrix (nImages, nImages);

    DEBUG_CHECK(delta                  != NULL, "Not enough memory to allocate matrix");
    DEBUG_CHECK(maxLikelihoodDistances != NULL, "Not enough memory to allocate matrix");
    DEBUG_CHECK(bayesianDistances      != NULL, "Not enough memory to allocate matrix");
    DEBUG_CHECK(data                   != NULL, "Not enough memory to allocate matrix");

    /* Load images */

    for (i = 0; i < nImages; i++) {
      readFile (makePath (args.imageDirectory, images[i]), i, data);
      writeProgress ("Loading images", i, nImages);
    }

    /* Compute rho for both the interpersonal and extrapersonal subspaces */

    iRho = computeRho(&intrapersonal);
    eRho = computeRho(&extrapersonal);

    printf ("iRho: %e eRho: %e\n", iRho, eRho);

    if (args.distanceMatrix == NULL)
      {
	/* Compute the difference images and the Bayesian similarities */

	progress = 0;
	progressMax = (SQR(nImages)+nImages)/2;
	
	for (i = 0; i < nImages; i++)
	  for (j = 0; j <= i; j++)
	    {
	      /* Compute the difference image */
	      
	      for (k = 0; k < data->row_dim; k++)
		ME (delta, k, 0) = ME (data, k, i) - ME (data, k, j);
	  
	      /* Compute the similarity scores */
	      
	      iLikelihood = computeMaxLikelihood (delta, &intrapersonal, iRho);
	      eLikelihood = computeMaxLikelihood (delta, &extrapersonal, eRho);
	  
	      ME(maxLikelihoodDistances, i, j) = iLikelihood;
	      ME(bayesianDistances, i, j)      = iLikelihood + eLikelihood;
	      
	      /* Assume symmetry */

	      ME(maxLikelihoodDistances, j, i) = ME(maxLikelihoodDistances, i, j);
	      ME(bayesianDistances, j, i)      = ME(bayesianDistances, i, j);
	  
	      writeProgress ("Computing distances", progress++, progressMax);
	    }
      }
    else
      {
	/* Run bayesian as a post-processing step*/

	int *indices = (int*) malloc (nImages * sizeof (int));

	/* Clear the matrix */

	for (i = 0; i < nImages; i++)
	  for (j = 0; j <= i; j++)
	    {
	      ME(maxLikelihoodDistances, j, i) = ME(maxLikelihoodDistances, i, j) = HUGE_VAL;
	      ME(bayesianDistances, j, i) = ME(bayesianDistances, i, j) = HUGE_VAL;
	    }

	progress = 0;
	progressMax = nImages;
      
	for (i = 0; i < nImages; i++) {
	  sortSubjectsBySimilarityToProbe (images[i], images, args.distanceMatrix, indices);

	  for (n = 0; n <= args.maxRank; n++) {
	    j = indices[n];
	  
	    /* Compute the difference image */
	    
	    for (k = 0; k < data->row_dim; k++)
	      ME (delta, k, 0) = ME (data, k, i) - ME (data, k, j);

	    /* Compute the similarity scores */
	      
	    iLikelihood = computeMaxLikelihood (delta, &intrapersonal, iRho);
	    eLikelihood = computeMaxLikelihood (delta, &extrapersonal, eRho);
	  
	    ME(maxLikelihoodDistances, i, j) = iLikelihood;
	    ME(bayesianDistances, i, j)      = iLikelihood + eLikelihood;
	      
	    /* Assume symmetry */

	    ME(maxLikelihoodDistances, j, i) = ME(maxLikelihoodDistances, i, j);
	    ME(bayesianDistances, j, i)      = ME(bayesianDistances, i, j);
	}

	writeProgress ("Computing distances", progress++, progressMax);
	}
      }

    /* Save out the distance matrix */

    MESSAGE1ARG("Writing out distance matrix to %s", args.bayesianDistDirectory);

    writeDistanceMatrix (args.bayesianDistDirectory, bayesianDistances, images, nImages);

    MESSAGE1ARG("Writing out distance matrix to %s", args.maxLikelihoodDistDirectory);

    writeDistanceMatrix (args.maxLikelihoodDistDirectory, maxLikelihoodDistances, images, nImages);

    /* Clean up */

    freeMatrix (delta);
    freeMatrix (maxLikelihoodDistances);
    freeMatrix (bayesianDistances);
    freeMatrix (data);
    freeListOfStrings (images);

    return 0;
}
