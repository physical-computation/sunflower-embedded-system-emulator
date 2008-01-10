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
   File: ldatrain.c                                                            
   Authors: Kai She, J. Ross Beveridge and David Bolme                         
   Date:    April. 10 2001                                                     
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

/**
   Purpose: This program loads training face images into a matrix M and        
   compute the eigen space and optionally the fisher space.                    
                                                                             
   Input: Gallery image data from binary file                                  
   Output: A combined projection space in a log file 
*/

#define OPENING  "From Training Imagery, Build a PCA or PCA+LDA Subspace."

/******************************************************************************
 *                               INCLUDES                                      *
 ******************************************************************************/

#include "csuCommon.h"

/******************************************************************************
 *                               GLOBALS                                       *
 ******************************************************************************/

typedef struct {
  char* imageList;
  char* imageDirectory;
  int vecLength;
  char* training_filename;
  int cutOffMode;
  double cutOff;
  int dropNVectors;
  int uselda;
  int writeTextBasis;
  int writeTextValues;
  int writeTextInterm;
  int argc;
  char **argv;
}
Arguments;

/* Remind the user the required command to run the program
   Input: the name of the executable program generated
*/
void usage(const char* prog) {
  printf("Usage: %s [OPTIONS] training_images.[list/srt] training_output\n", prog);
  printf("  Parameters\n");
  printf("    training_images: image names file\n");
  printf("    trainName: training output file\n");
  printf("  Options\n");
  printf("    -lda:                enable lda training. (Must use srt file)\n        DEFAULT = PCA Only\n");
  printf("    -imDir <dir>:        image directory.\n        DEFAULT = \".\"\n");
  printf("    -vecLength <int>:    vector length.\n        DEFAULT = \"Auto\"\n");
  printf("    -dropNVectors <int>: Drops the first N vectors which are normally lighting effects.\n        DEFAULT = 0\n");
  printf("    -cutOffMode <mode>:  Selects the method for eigen vector selection after PCA trianing\n");
  printf("                         and droping the first vectors.  DEFAULT = SIMPLE\n");
  printf("         NONE            Retain eigenvectors.\n");
  printf("         SIMPLE          Retain a percentage of the eigenvectors. \n");
  printf("                         Expects value between 0.0 and 100.0, DEFAULT PERCENT = %f \n", DEFAULT_CUTOFF_PERCENT_SIMPLE);
  printf("         ENERGY          Retain  eigenvectors accounting for a percentage of the total energy.\n");
  printf("                         Expects value between 0.0 and 100.0, DEFAULT PERCENT = %f\n", DEFAULT_CUTOFF_PERCENT_ENERGY);
  printf("         STRETCH         Retains all eigenvectors greater than a percentage of the largest eigenvector.\n");
  printf("                         Expects value between 0.0 and 100.0, DEFAULT PERCENT = %f\n", DEFAULT_CUTOFF_PERCENT_STRETCH);
  printf("         CLASSES         Retains as many eigenvectors as there LDA Classes: use only with LDA.\n");
  printf("                         Ignores cutOff value and uses number of classes instead\n");
  printf("    -cutOff <percent>:   Percentage of eigen vectors to retain (see cutOffMode).\n        DEFAULT = (See cutoff mode)\n");
  printf("    -writeTextBasis:     Causes the program to print the basis vectors to text files.\n        DEFAULT = No\n");
  printf("    -writeTextValues:    Causes the program to print the basis values to text files.\n        DEFAULT = No\n");
  printf("    -writeTextInterm:    Causes the program to print intermediate matricies.\n        DEFAULT = No\n");
  printf("    -outputMatlabAscii:  Causes the program to print matricies in matlab format.\n        DEFAULT = Octave format\n");
  printf("    -quiet:              Turn off all messages.\n        DEFAULT = messages on\n");
  printf("    -debuglevel <int>:   Level of debug information to display (automatically sets quiet to no).\n        DEFAULT = 0\n");
  exit(1);
}

/**
   Process the command line and initialize the variables
   Input:
   argc        is the number of arguments
   argv        are the arguments
   imDir       is the directory of gallery images
   imNames     is the name of the file which contains the gallery
   image names
   vecLength   is the size of the vectors or number of pixels
   of the image
   trianName   is the name of the file which stores the training
   information
   cutOff      is the percentage of the eigenvectors kept to build
   the eigen space
*/

void process_command(int argc, char** argv, Arguments* args) {
  int i;
  int param_num = 0;
  int cutOffSet = 0; /* set to one if -cutOff is found so default does not change */

  args->argc = argc;
  args->argv = argv;

  args->imageDirectory  = strdup (".");
  args->vecLength       = 0;
  args->cutOffMode      = CUTOFF_SIMPLE;
  args->cutOff          = DEFAULT_CUTOFF_PERCENT_SIMPLE;
  args->uselda          = 0;
  args->dropNVectors    = 0;
  args->writeTextBasis  = 0;
  args->writeTextValues = 0;
  args->writeTextInterm = 0;
  debuglevel = 0;

  for (i = 1;i < argc;i++) {

    /* Catch common help requests */
    if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
    else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }

    /* Read in input directories */
    else if (readOptionString (argc, argv, &i, "-imDir", &(args->imageDirectory)));

    else if (readOptionInt    (argc, argv, &i, "-vecLength", &(args->vecLength)));
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

    else if (readOption (argc, argv, &i, "-lda" ))                     { args->uselda = 1; }
    else if (readOption (argc, argv, &i, "-writeTextBasis" ))          { args->writeTextBasis = 1; }
    else if (readOption (argc, argv, &i, "-writeTextValues" ))         { args->writeTextValues = 1; }
    else if (readOption (argc, argv, &i, "-writeTextInterm" ))         { args->writeTextInterm = 1; }
    else if (readOption (argc, argv, &i, "-outputMatlabAscii" ))       { asciiFormat = 1; }

    /* other flags */
    else if (readOption    (argc, argv, &i, "-quiet")) { quiet = 1; }
    else if (readOptionInt (argc, argv, &i, "-debuglevel", &debuglevel)) {}

    /* read required arguments */ 
    else if (param_num == 0) {
      args->imageList = strdup (argv[i]);
      param_num++;
    }
    else if (param_num == 1) {
      args->training_filename = strdup (argv[i]);
      param_num++;
    } 
    else { clParseError(argc,argv,i,"Wrong number of required arguments"); }
  }

  /* make sure that there are the proper number of required arguments */

  if (param_num != 2){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

  /* Choose a default cutoff if none was specified */

  if (!cutOffSet)
    {
      switch (args->cutOffMode)
	{
	case CUTOFF_NONE:
	case CUTOFF_SIMPLE:
	case CUTOFF_CLASSES:
	  args->cutOff = DEFAULT_CUTOFF_PERCENT_SIMPLE;
	  break;
	case CUTOFF_ENERGY:
	  args->cutOff = DEFAULT_CUTOFF_PERCENT_ENERGY;
	  break;
	case CUTOFF_STRETCH:
	  args->cutOff = DEFAULT_CUTOFF_PERCENT_STRETCH;
	  break;
      }
    }

  if ((!args->uselda) && (args->cutOffMode == CUTOFF_CLASSES)) {
    printf("Warning: Cutoff Mode Classes meant for use with lda only.\n");
  }

  /* Print out the program parameters for appropreate debug level */
  DEBUG(1,"Debuging enabled");
  if(debuglevel > 0){
    printf ("***************** Program Parameters *********************\n");
    printf ("Image directory: %s\n", args->imageDirectory);
    printf ("Image list:      %s\n", args->imageList);
    printf ("Training file:   %s\n", args->training_filename);
    printf ("vecLength:       %d\n", args->vecLength);
    printf ("useLda:          %s\n", args->uselda ? "YES" : "NO");
    printf ("cutOff:          %f\n", args->cutOff);
  }
}


/*
  main()
*/
int
main(int argc, char *argv[])
{
  Arguments args;
  int numImages, numSubjects;
  ImageList *srt;
  Matrix images;
  Subspace subspace;
    
  process_command(argc, argv, &args);

  MESSAGE(OPENING);
  MESSAGE(VERSION);

  /* The Matrix images is contains what is commonly calld the data matrix. It
     consists of n images, one image per column. Hence, each images data is 
     contiguous in memory. This choice facilities easy integration with the 
     Eigensolver used below */

  MESSAGE1ARG("Reading images from files in directory %s.", args.imageDirectory);

  images = readImages(args.imageList, args.imageDirectory, &args.vecLength,
		      &numImages, &numSubjects, &srt);
  
  subspaceTrain (&subspace, images, srt, numSubjects, args.dropNVectors, args.cutOffMode, args.cutOff, args.uselda, args.writeTextInterm);

  /* Write out text files for basis and values */
  if (args.writeTextBasis)
    {
      SAVE_MATRIX (subspace.basis);
      SAVE_MATRIX (subspace.mean);
      
      SAVE_MATRIX (subspace.values);
    }

  writeSubspace (&subspace, args.training_filename, args.imageList, args.argc, args.argv);
  
  MESSAGE("Finished Training.");
  
  return 0;
}
