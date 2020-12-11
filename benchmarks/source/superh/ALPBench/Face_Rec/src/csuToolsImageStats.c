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
File: csuToolsImageStats.c                                                     
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

#define OPENING  "csuToolsImageStats gathers statistics about image files."

/******************************************************************************
*                               INCLUDES                                      *
******************************************************************************/

#include <csuCommon.h>
#include <stdio.h>
#include <math.h>

/******************************************************************************
*                               GLOBALS                                       *
******************************************************************************/
/**
 * The command line arguments are managed by a single structure.
 */
typedef struct {
  char* outputFile;
  char* eyeCoordsFile;
  char* imageDirectory;
  char* maskDirectory;
}
Arguments;

/**
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void
usage (const char* name)
{
  printf("Usage: %s eye_coordinate_file output_file\n\n",name);
  printf ("    -imDir <dir>:       image directory. DEFAULT = \".\"\n");
  printf ("    -maskDir <dir>:     mask directory. DEFAULT = \".\"\n");
  printf ("    -debuglevel <int>:  Level of debug information to display. DEFAULT = 0\n");
  printf ("    -quiet:             Turn off all messages. DEFAULT = messages on\n");
  printf( "    This program gathers statistics about images\n\n\n");
  exit(1);
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
  args->eyeCoordsFile = NULL;
  args->outputFile     = NULL;
  args->maskDirectory  = NULL;

  quiet = 0;
  debuglevel = 0;

  for (i = 1; i < argc; i++) {

    /* Catch common help requests */

    if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
    else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }

    /* Read in input directories */

    else if (readOptionString (argc, argv, &i, "-imDir",     &(args->imageDirectory)));
    else if (readOptionString (argc, argv, &i, "-maskDir",   &(args->maskDirectory)));

    /* other flags */

    else if (readOption       (argc, argv, &i, "-quiet")) { quiet = 1; }
    else if (readOptionInt    (argc, argv, &i, "-debuglevel", &debuglevel));

    /* check if the current argument is an unparsed option */

    else if (checkBadOption(argc,argv,&i));


    else if (param_num == 0) {
      args->eyeCoordsFile = strdup(argv[i]);
      param_num++;
    }

    else if (param_num == 1) {
      args->outputFile = strdup(argv[i]);
      param_num++;
    }
  }

  if (param_num != 2)
    usage(argv[0]);

  /* Print out the program parameters for appropriate debug level */

  DEBUG_INT (1, "Debuging enabled", debuglevel);

  if(debuglevel > 0){
    printf ("***************** Program Parameters *********************\n");
    printf ("eyeCoordsFile: %s\n", args->eyeCoordsFile);
    printf ("imageDirectory: %s\n", args->imageDirectory);
    printf ("outputFile: %s\n",     args->outputFile);
  }
}

char **
loadMask (const char *filename)
{
  Image im = readPGMImage (filename);
  assert (im);
  return imageToMask (im,0);
}

int
main (int argc, char** argv)
{
  Arguments args;
  int i, j;
  FILE *fi, *fo;
  char line [FILE_LINE_LENGTH ];
  char filename [MAX_FILENAME_LENGTH];
  char **combinedEyesMask, **leftEyeMask, **rightEyeMask;
  char **foreheadStripMask, **centerStripMask, **mouthChinStripMask;
  char **leftCheekMask, **rightCheekMask;
  char **leftSideMask, **rightSideMask, **wholeMask;

  MESSAGE(OPENING);
  MESSAGE(VERSION);
  process_command(argc, argv, &args);

  /* Read in the masks */

  combinedEyesMask     = loadMask (makePath (args.maskDirectory, "combinedEyesMask.pgm"));
  foreheadStripMask    = loadMask (makePath (args.maskDirectory, "foreheadHorizontalStripMask.pgm"));
  centerStripMask      = loadMask (makePath (args.maskDirectory, "centerHorizontalStripMask.pgm"));
  mouthChinStripMask   = loadMask (makePath (args.maskDirectory, "mouthChinStripMask.pgm"));
  leftEyeMask          = loadMask (makePath (args.maskDirectory, "leftEyeMask.pgm"));
  rightEyeMask         = loadMask (makePath (args.maskDirectory, "rightEyeMask.pgm"));
  leftCheekMask        = loadMask (makePath (args.maskDirectory, "leftCheekMask.pgm"));
  rightCheekMask       = loadMask (makePath (args.maskDirectory, "rightCheekMask.pgm"));
  leftSideMask         = loadMask (makePath (args.maskDirectory, "leftSideMask.pgm"));
  rightSideMask        = loadMask (makePath (args.maskDirectory, "rightSideMask.pgm"));
  wholeMask            = loadMask (makePath (args.maskDirectory, "wholeMask.pgm"));

  fo = fopen (args.outputFile, "w");
  DEBUG_CHECK (fo,"Error opening output file");

  fi = fopen (args.eyeCoordsFile,"r");
  DEBUG_CHECK (fi,"Error opening eye coordinates file");

  fprintf (fo, "%d\n", 0);

  for (i = 1; ; i++)
    {
      Image im;
      double mean1, mean2, median1, median2;
      double stddev1, stddev2, mad1, mad2, diffMean, diffMedian;
      double ratioSD, ratioMad, tStat;
      double x1, x2, y1, y2;
      double eyeDist, eyeAng;
      int numPix1, numPix2;

      struct {
	int id;
	char *label;
	double *value;
	char   **mask1;
	char   **mask2;
      } predictors[] = {

	/* Whole-Image Predictors */

	{ 1, "MEAN_WHOLE",      &mean1,      wholeMask,          NULL},
	{ 2, "MED_WHOLE",       &median1,    wholeMask,          NULL},
	{ 3, "STD_WHOLE",       &stddev1,    wholeMask,          NULL},
	{ 4, "MAD_WHOLE",       &mad1,       wholeMask,          NULL},
	{ 5, "EYE_DIST",        &eyeDist,    wholeMask,          NULL},
	{ 6, "EYE_ANGLE",       &eyeAng,     wholeMask,          NULL},

	/* Regional Predictors */

	{ 7, "MEAN_EYES",       &mean1,      combinedEyesMask,   NULL},
	{ 8, "MED_EYES",        &median1,    combinedEyesMask,   NULL},
	{ 9, "STD_EYES",        &stddev1,    combinedEyesMask,   NULL},
	{10, "MAD_EYES",        &mad1,       combinedEyesMask,   NULL},

	{11, "MEAN_FH",         &mean1,      foreheadStripMask,  NULL},
	{12, "MED_FH",          &median1,    foreheadStripMask,  NULL},
	{13, "STD_FH",          &stddev1,    foreheadStripMask,  NULL},
	{14, "MAD_FH",          &mad1,       foreheadStripMask,  NULL},

	{15, "MEAN_CENTR",      &mean1,      centerStripMask,    NULL},
	{16, "MED_CENTR",       &median1,    centerStripMask,    NULL},
	{17, "STD_CENTR",       &stddev1,    centerStripMask,    NULL},
	{18, "MAD_CENTR",       &mad1,       centerStripMask,    NULL},

	{19, "MEAN_MC",         &mean1,      mouthChinStripMask, NULL},
	{20, "MED_MC",          &median1,    mouthChinStripMask, NULL},
	{21, "STD_MC",          &stddev1,    mouthChinStripMask, NULL},
	{22, "MAD_MC",          &mad1,       mouthChinStripMask, NULL},

	/* Statistical Contrast Predictors */

	{23, "DMEAN_LR_SIDES",  &diffMean,   leftSideMask,       rightSideMask},
	{24, "DMED_LR_SIDES",   &diffMedian, leftSideMask,       rightSideMask},
	{25, "RSTD_LR_SIDES",   &ratioSD,    leftSideMask,       rightSideMask},
	{26, "RMAD_LR_SIDES",   &ratioMad,   leftSideMask,       rightSideMask},
	{27, "TSTAT_LR_SIDES",  &tStat,      leftSideMask,       rightSideMask},

	{28, "DMEAN_LR_EYES",   &diffMean,   leftEyeMask,        rightEyeMask},
	{29, "DMED_LR_EYES",    &diffMedian, leftEyeMask,        rightEyeMask},
	{30, "RSTD_LR_EYES",    &ratioSD,    leftEyeMask,        rightEyeMask},
	{31, "RMAD_LR_EYES",    &ratioMad,   leftEyeMask,        rightEyeMask},
	{32, "TSTAT_LR_EYES",   &tStat,      leftEyeMask,        rightEyeMask},

	{33, "DMEAN_FH_CENTR",  &diffMean,   foreheadStripMask,  centerStripMask},
	{34, "DMED_FH_CENTR",   &diffMedian, foreheadStripMask,  centerStripMask},
	{35, "RSTD_FH_CENTR",   &ratioSD,    foreheadStripMask,  centerStripMask},
	{36, "RMAD_FH_CENTR",   &ratioMad,   foreheadStripMask,  centerStripMask},
	{37, "TSTAT_FH_CENTR",  &tStat,      foreheadStripMask,  centerStripMask},

	{38, "DMEAN_FH_MC",     &diffMean,   foreheadStripMask,  mouthChinStripMask},
	{39, "DMED_FH_MC",      &diffMedian, foreheadStripMask,  mouthChinStripMask},
	{40, "RSTD_FH_MC",      &ratioSD,    foreheadStripMask,  mouthChinStripMask},
	{41, "RMAD_FH_MC",      &ratioMad,   foreheadStripMask,  mouthChinStripMask},
	{42, "TSTAT_FH_MC",     &tStat,      foreheadStripMask,  mouthChinStripMask},

	{43, "DMEAN_CENTR_MC",  &diffMean,   centerStripMask,    mouthChinStripMask},
	{44, "DMED_CENTR_MC",   &diffMedian, centerStripMask,    mouthChinStripMask},
	{45, "RSTD_CENTR_MC",   &ratioSD,    centerStripMask,    mouthChinStripMask},
	{46, "RMAD_CENTR_MC",   &ratioMad,   centerStripMask,    mouthChinStripMask},
	{47, "TSTAT_CENTR_MC",  &tStat,      centerStripMask,    mouthChinStripMask},

	{48, "DMEAN_LR_CHEEK",  &diffMean,   leftCheekMask,      rightCheekMask},
	{49, "DMED_LR_CHEEK",   &diffMedian, leftCheekMask,      rightCheekMask},
	{50, "RSTD_LR_CHEEK",   &ratioSD,    leftCheekMask,      rightCheekMask},
	{51, "RMAD_LR_CHEEK",   &ratioMad,   leftCheekMask,      rightCheekMask},
	{52, "TSTAT_LR_CHEEK",  &tStat,      leftCheekMask,      rightCheekMask}
      };

      /* Read in a line from the eyes coordinates file */

      fgets (line, FILE_LINE_LENGTH, fi);
      if (feof (fi))
	break;
      
      if (sscanf (line,"%s %lf %lf %lf %lf", filename, &x1, &y1, &x2, &y2) != 5)
	{
	  printf("Error parsing line %d of eye coordinate file. Exiting...",i);
	  exit(1);
	}

      /* Load image and calculate the statistics */

      /* Print the image name, the image id (the first five characters of the name), the number of subject
       covariates and the number of image covariates
      */

      fprintf (fo, "%s\t", filename);
      fprintf (fo, "%c%c%c%c%c\t", filename[0], filename[1], filename[2], filename[3], filename[4]); 
      fprintf (fo, "%d\t%ld\t", 0, NELEM(predictors));
      
      sprintf (filename, "%s.sfi", filename);
      im = readRawImage (makePath (args.imageDirectory, filename));

      eyeDist = sqrt (SQR(x2 - x1) + SQR (y2 - y1));
      eyeAng  = atan ((y2 - y1)/(x2 - x1)) / M_PI * 180;

      for (j = 0; j < NELEM(predictors); j++)
	{
	  mean1 = stddev1 = numPix1 = median1 = mad1 = 0;
	  mean2 = stddev2 = numPix2 = median2 = mad2 = 0;
	  diffMean = diffMedian = ratioSD = ratioMad = tStat = 0;

	  /* Compute the statistics using the first mask */

	  if (predictors[j].mask1)
	    {
	      mean1   = imageMean        (im, (const char**) predictors[j].mask1);
	      stddev1 = imageStdDev      (im, (const char**) predictors[j].mask1);
	      numPix1 = imageNumOfPixels (im, (const char**) predictors[j].mask1);
	      median1 = imageMedian      (im, (const char**) predictors[j].mask1);
	      mad1    = imageMad         (im, (const char**) predictors[j].mask1);
	    }

	  /* Compute the statistics using the second mask */

	  if (predictors[j].mask2)
	    {
	      mean2   = imageMean        (im, (const char**) predictors[j].mask2);
	      stddev2 = imageStdDev      (im, (const char**) predictors[j].mask2);
	      numPix2 = imageNumOfPixels (im, (const char**) predictors[j].mask2);
	      median2 = imageMedian      (im, (const char**) predictors[j].mask2);
	      mad2    = imageMad         (im, (const char**) predictors[j].mask2);
	    }

	  /* Compute the paired statistics */

	  if (predictors[j].mask1 && predictors[j].mask2)
	    {
	      diffMean    = mean1   - mean2;
	      diffMedian  = median1 - median2;
	      ratioSD     = stddev1 / stddev2;
	      ratioMad    = mad1    / mad2;
	      tStat       = (mean1  - mean2) / sqrt (SQR (stddev1) / numPix1 + SQR (stddev2) / numPix2);
	    }
	  
	  /* Apply the operator */

	  fprintf (fo, "%s\t%.10e\t", predictors[j].label, (*predictors[j].value));
	}

      fprintf (fo, "\n");
      
      freeImage (im);
      
      writeProgress ("Generating statistics", i, 0);
    }

  fclose (fo);
  fclose (fi);

  return 0;
}


