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
 *  csuCommonSubspace.c
 *
 * Authors: Kai She, J. Ross Beveridge, David Bolme and Marcio Teixeira    
 *
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

#include "csuCommon.h"

#if defined(THRD)
#include <pthread.h>
#endif


/* How many lines in the training file header have useful text */
#define TRAINING_HEADER_ENTRIES 10

void
subspaceTrain (Subspace *s, Matrix images, ImageList *srt, int numSubjects, int dropNVectors, CutOffMode cutOffMode, double cutOff, int useLDA, int writeTextInterm)
{
  int i;
  Matrix m;
  int n = 0;    /* The number of eigen vectors to keep */
  double total_energy, energy;
  Matrix tmp;

  /* Initialize structure */

  s->useLDA         = useLDA;
  s->cutOffMode     = cutOffMode;
  s->cutOff         = cutOff;
  s->dropNVectors   = dropNVectors;

  s->numSubjects    = numSubjects;
  s->numPixels      = images->row_dim;

  /*********************************************************************
   * STEP ONE: Calculate the eigenbasis
   ********************************************************************/

  /* Compute the Eigenvalues and Eigenvectors for the covariance matrix
     derived from the images data matrix. The image data is "centered", meaning
     the mean image is subtracted from all images before PCA is performed.
     This centering is done in place, so after this call images are centered. */

  MESSAGE("Computing the PCA eigenspace.");

  eigentrain (&s->mean, &s->values, &s->basis, images);

  MESSAGE("Finished computing eigenspace.");

  /* Numerical roundoff errors may lead to small negative values.
     Strip those before saving the matrix. */

  m = s->values;
  for (i = 0; i < m->row_dim; i++)
    {
      if (ME(m,i,0) < 0) {
	if (ME(m,i,0) < -1e-10)
	  printf("WARNING: Large negative eigenvalue found %f. Truncating to zero.\n", ME(m,i,0));
	ME(m,i,0) = 0;
      }
    }
  
  /*********************************************************************
   * STEP TWO: Drop eigenvectors from the front or truncate them from
   * the back
   ********************************************************************/

  /* 
     The following is used to filter the vectors that are retained after PCA
     training.  The function first optionally removes the vectors from the matrix
     based on the argument -dropNVectors. This argument is always intrepreted in
     terms of absolute numbers, i.e. a value of 1 means drop the first vector, a
     value of 3 means drop the first three. The function then drops vectors from the
     tail based on -cutOffMode and -cutOff arguments. Here, the mode controls how the
     cutoff is performed. The possible modes are:
     
     NONE:    Keep all remaining eigenvectors.
     SIMPLE:  Keep a percentage where the percentage is specified by cutOff.
     ENERGY:  Keep the fewest vectors are such that the sum of energy for them just
     exceeds cutOff times the total energy. 
     STRETCH: Keep all eigenvectors that have eigenvalues greater than a percentage 
     of the largest, where the percentage is specied by cutOff. 
     CLASSES: Keep as many eigenvectors as there are LDA classes.
     
     For both Energy and Stretch, if eigen values/vectors are dropped from the front, 
     these are not included in the determination of the total energy or the max
     eigen value. 
  */

  /* Drop the first vectors  */
    
  DEBUG_CHECK (s->dropNVectors < (s->basis)->col_dim, "Number of vectors to drop must be less than the number of the eigen vectors");

  /* transpose eigenValues for use in this function */
    
  tmp = transposeMatrix (s->values);
  freeMatrix (s->values);
  s->values = tmp;
    
  if (s->dropNVectors && (s->dropNVectors < (s->values)->col_dim))
    {
      tmp = matrixCols (s->basis, s->dropNVectors, (s->basis)->col_dim-1);
      freeMatrix (s->basis);
      s->basis = tmp;

      tmp = matrixCols (s->values, s->dropNVectors, (s->values)->col_dim-1);
      freeMatrix (s->values);
      s->values = tmp;
    }

  /* transpose the eigenValues back to the original order. */
 
  tmp = transposeMatrix (s->values);
  freeMatrix (s->values);
  s->values = tmp;
    
  DEBUG_CHECK((s->values)->row_dim - s->dropNVectors > 0, "Too many eigen vectors droped from front. Can not proceed.");

  switch (s->cutOffMode)
    {
    case CUTOFF_NONE:
      n = (s->basis)->col_dim;
      break;

    case CUTOFF_SIMPLE:
      n = (int)((s->basis)->col_dim * s->cutOff / 100.0);
      break;

    case CUTOFF_ENERGY:
      /* compute total energy - this will not include vectors/values dropped from front. */
      total_energy = 0;
      for (i = 0; i < (s->values)->row_dim; i++) {
	total_energy += ME(s->values, i, 0);
      }

      /* compute cutoff point */
      i = 0;
      energy = 0;
      while ((i < (s->values)->row_dim) && (energy < total_energy * s->cutOff / 100.0)) {
	energy += ME(s->values, i, 0);
	i++;
      }
      n = i;
      break;

    case CUTOFF_STRETCH:
      i = 1;
      while ((i < (s->values)->row_dim) &&
	     (100.0*(ME(s->values, i, 0) / ME(s->values, s->dropNVectors, 0)) > cutOff )) {
	i++;
      }
      n = i;
      break;

    case CUTOFF_CLASSES:
      n = s->numSubjects;
      break;

    default:
      n = 0;
      DEBUG_CHECK (0, "ERROR: Unkown cutoff type");
      break;
    };

  /* Never set the dimensionality of the PCA subspace below the number of
     LDA classes when LDA is being used. Doing so creates a horrible problem
     for LDA: too fee dimensions */
    
  if (s->useLDA && (n < s->numSubjects))
    n = s->numSubjects;
  
  DEBUG_CHECK (n <= (s->basis)->col_dim, "Tried to expand, not contract, PCA space.");
  
  MESSAGE1ARG ("Retaining %d eigen vectors.",n);

  tmp = matrixCols ( s->basis, 0 , n-1);
  freeMatrix (s->basis);
  s->basis = tmp;

  DEBUG_INT (1, "Number of eigen vectors kept.", n);

  DEBUG_CHECK ((s->basis)->col_dim > 0, "All basis vectors deleted after cutoff "
	       "and vector drop was processed.");

  MESSAGE2ARG("Truncating PCA Space. Subspace projection expressed "
	      "as %d by %d matrix.", s->basis->row_dim, s->basis->col_dim);

  /*********************************************************************
   * STEP THREE: Do the LDA if specified
   ********************************************************************/

  if (s->useLDA)
    {
      /* Need to project original images into PCA space */

      Matrix fisherBasis, fisherValues, combinedBasis;
      Matrix imspca = transposeMultiplyMatrixL (s->basis, images);
      
      MESSAGE("Computing Fisher Linear Discriminants for "
	      "training images projected into PCA subspace.");

      fisherTrain (imspca, srt, &fisherBasis, &fisherValues, writeTextInterm);

      combinedBasis = multiplyMatrix (s->basis, fisherBasis);
      basis_normalize (combinedBasis);

      MESSAGE2ARG ("PCA and LDA Combined. Combined projection expressed as %d by "
		   "%d matrix.", combinedBasis->row_dim, combinedBasis->col_dim);

      s->values = fisherValues;
      s->basis  = combinedBasis;
    }
}

/**
   writeSubspace
 
   Writes out a training discription file that includes important
   parameters and command line options and all eigenvectors and eigenvalues
 
   format:
   text: 256 lines reserved
   line1: TRAINING_COMMAND = commandline
   line2: DATE = <training date>
   line3: FILE_LIST = <file contianing the list of images used for training>
   line4: VECTOR_LENGTH = <number of values in each file>
   line5: USE_LDA = <true if lda was turned on>
   line6: PCA_CUTOFF = <Percentage of eigenvectors retianed>
   line7: BASIS_VALUE_COUNT = <number of eigen values>
   line8: BASIS_VECTOR_COUNT = <number of eigen vectors>
   line9: DROPPED_FROM_FRONT = <zero or more dropped from front>
   line10 -> line256: RESERVED
 
   If the additional entries are added to the header, increment the variable
   TRAINING_HEADER_ENTRIES
*/

void
writeSubspace (Subspace *s, char *training_filename, char *imageList, int argc, char**argv)
{
  int i, j;
  FILE* file;
  char *cutOffModeStr;
  time_t ttt = time(0);

  switch (s->cutOffMode)
    {
    case CUTOFF_NONE:    cutOffModeStr = "NONE";    break;
    case CUTOFF_SIMPLE:  cutOffModeStr = "SIMPLE";  break;
    case CUTOFF_ENERGY:  cutOffModeStr = "ENERGY";  break;
    case CUTOFF_STRETCH: cutOffModeStr = "STRETCH"; break;
    default:             cutOffModeStr = "UNKNOWN"; break;
    }
  
  MESSAGE1ARG ("Saving trianing information to file %s", training_filename);

  file = fopen (training_filename, "wb");
  if (!file) {
    printf ("Error: could not open file <%s>\n", training_filename);
    exit (1);
  }
  
  fprintf (file, "TRAINING_COMMAND =");
  
  for (i = 0; i < argc; i++)
    fprintf (file, " %s", argv[i]);

  fprintf (file, "\n");
  fprintf (file, "DATE          = %s", ctime(&ttt));
  fprintf (file, "FILE_LIST     = %s\n", imageList);
  fprintf (file, "VECTOR_LENGTH = %d\n", s->basis->row_dim); /* numPixels */
  fprintf (file, "USE_LDA       = %s\n", s->useLDA ? "YES" : "NO" );

  fprintf (file, "CUTOFF_MODE   = %s\n", cutOffModeStr);
  fprintf (file, "CUTOFF_PERCENTAGE  = %f\n", s->cutOff);
  fprintf (file, "BASIS_VALUE_COUNT  = %d\n", s->values->row_dim);   /* basisDim */
  fprintf (file, "BASIS_VECTOR_COUNT = %d\n", s->basis->col_dim);
  fprintf (file, "DROPPED_FROM_FRONT = %d\n", s->dropNVectors);

  for (i = 11; i < 256; i++){
    fprintf (file, "\n");
  }

  /* write out the pixel count */
  writeInt (file, s->mean->row_dim);

  /* write out the mean vector */
  for (i = 0; i < s->mean->row_dim; i++) {
    writeDouble (file, ME(s->mean,i,0));
  }

  /* write out the number of eigen values */
  writeInt (file, s->values->row_dim);
  
  /* write out the eigen values */
  for (i = 0; i < s->values->row_dim; i++) {
    writeDouble (file, ME(s->values,i,0));
  }

  /* write out the number of basis vectors */
  writeInt (file,s->basis->col_dim);
  
  /* write out the eigen basis.  the size is "pixelcount"X"number of vectors"*/
  for (i = 0; i < s->basis->col_dim; i++) {
    for (j = 0; j < s->basis->row_dim; j++) {
      writeDouble (file, ME(s->basis, j, i));
    }
  }
  fclose (file);
}

/* The contents of the training file are described in csuSupbspaceTrain.c above
 the definition of writeTrainingFile.  This companion function reads one of these
 files and creates the subspace basis vectors and eigenvalues. In the case of PCA+LDA,
 the eigenvalues are for the transformed, or second, of the two symmetric eigenvector
 problems solved. The variable TRAINING_HEADER_ENTRIES defined at the top of this file
 is used to control how many header lines are printed to standard out. It needs to be 
 adjusted if new entries are added to the header. 
*/

void
readSubspace (Subspace *s, const char* trainingFile, int quiet)
{
    int i, j, headerSize, rowDim, colDim;
    char junk[FILE_LINE_LENGTH], text[FILE_LINE_LENGTH];
    char** header;
    FILE* file;

    headerSize = 255;
    header = (char**) malloc (sizeof(char*) * headerSize);
    assert (header);
    for (i = 0; i < headerSize; i++) {
        header[i] = (char*) malloc(sizeof(char) * FILE_LINE_LENGTH);
        assert (header[i]);
    }

    file = fopen (trainingFile, "rb");

    if (!file) {
        printf("Error: could not open file <%s>\n", trainingFile);
        exit(1);
    }

    for (i = 0; i < headerSize; i++)
        fgets(header[i], FILE_LINE_LENGTH, file);

    if (!quiet) {
        printf("\nTraining Header File is:\n");
        for (i = 0; i < TRAINING_HEADER_ENTRIES; i++)
            printf("   Line %d: %s", i, header[i]);
    }

    sscanf(header[7], "%s%s%d", junk, junk, &s->basisDim);
    sscanf(header[4], "%s%*s%s", junk, text);
    
    if (strcmp(text, "NO") == 0)
      s->useLDA = 0;
    else
      s->useLDA = 1;

    readInt (file,&rowDim);
    s->numPixels = rowDim;
    DEBUG_INT (3, "Vector size", rowDim);
    s->mean = makeMatrix(rowDim, 1);
    for (i = 0; i < (s->mean)->row_dim; i++) {
        readDouble (file, &ME(s->mean,i,0));
    }

    readInt (file,&rowDim);
    s->values = makeMatrix (rowDim, 1);
    for (i = 0; i < (s->values)->row_dim; i++) {
        readDouble (file, &ME(s->values,i,0));
    }

    rowDim = s->numPixels;
    readInt (file,&colDim);
    s->basis = makeMatrix (rowDim, colDim);
    for (i = 0; i < (s->basis)->col_dim; i++) {
        for (j = 0; j < (s->basis)->row_dim; j++) {
            readDouble (file, &ME(s->basis, j, i));
        }
    }

    fclose(file);
}

void
validateBasisIsOrthonormal (Matrix basis, int printlevel)
{
  int i, j;
  FTYPE tolerance = 0.000001;
  Matrix test = transposeMultiplyMatrixL(basis, basis);

  for (i = 0; i < test->row_dim; i++) {
    for (j = 0; j < test->col_dim; j++) {
      if (i == j) {
	if (ABS(ME(test, i, j) - 1.0) > tolerance) {
	  fprintf(stderr, "WARNING: Subspace basis failed orthonormality check at (%d, %d) value: %f\n", i, j, ME(test, i, j));
	}
      } else {
	if (ABS(ME(test, i, j)) > tolerance) {
	  fprintf(stderr, "WARNING: Subspace basis failed orthonormality check at (%d, %d) value: %f\n", i, j, ME(test, i, j));
	}
      }
    }
  }
  if (printlevel > 0)
    printf("\nSubspace Basis Passed Orthonormality Check");
}

Matrix
centerThenProjectImages (Subspace *s, Matrix images)
{
    Matrix subspims;

    mean_subtract_images (images, s->mean);
    subspims = transposeMultiplyMatrixL (s->basis, images);
    return subspims;
}
#ifdef THRD
void *thread_work(void* thrd_args) 
{
  struct thread_args *my_data = (struct thread_args*) thrd_args;
  Subspace *s;
  char * imageDirectory;
  ImageList *start_sub, *stop_sub, *subject, *replicate;
  int i, j;
  Matrix images, vector, smallVector;
  char name[FILE_LINE_LENGTH];

  s = my_data->s;
  imageDirectory = my_data->imageDirectory;
  start_sub = my_data->start_sub;
  stop_sub = my_data->stop_sub;
  images = my_data->images;
  vector = makeMatrix(s->numPixels, 1);
  /*vector = my_data->vector;*/
  /*name = my_data->name;*/

  i = start_sub->imageIndex;
  for (subject = start_sub; subject!=stop_sub; subject = subject->next_subject) {
    for (replicate = subject; replicate; replicate = replicate->next_replicate) {
      if (debuglevel > 0)
        printf("%s ", replicate->filename);
      sprintf(name, "%s/%s", imageDirectory, replicate->filename);
      
      /*printf("replicate orignal %d now %d\n",replicate->imageIndex, i);*/
      replicate->imageIndex = i;
      readFile(name, 0, vector);
      
      /*writeProgress("Reading images", i,*numImages);*/

      smallVector = centerThenProjectImages(s, vector);

      /* Copy the smaller vector into the image matrix*/
      for (j = 0; j < smallVector->row_dim; j++) {
	ME(images, j, i) = ME(smallVector, j, 0);
      }
      freeMatrix(smallVector);

      i++;  /* increament the image index */
    }
    if (debuglevel > 0)
      printf("\n");
  }

  return NULL;
}
#endif
/*
 This function reads images in to a vector.  That vector is then mean subtracted
 and then projected onto an optimal basis (PCA or LDA).  Returned is a matrix that
 contains the images after they have been projected onto the subspace.
 */
Matrix
readAndProjectImages (Subspace *s, char *imageNamesFile, char *imageDirectory, int *numImages, ImageList **srt)
{
  int i, j;
  Matrix images, vector, smallVector;
  char name[FILE_LINE_LENGTH];
  ImageList *subject, *replicate;
#ifdef THRD
  int numSubj, ns_per_thread, extra_subj,status;
  struct thread_args thread_data[NUM_THREADS];

  pthread_attr_t attr;
  pthread_t thread[NUM_THREADS];

#endif

  DEBUG(1, "Reading training file names from file");

#ifdef THRD
  *srt = getImageNames(imageNamesFile, numImages, &numSubj);
#else
  *srt = getImageNames(imageNamesFile, numImages);
#endif

  DEBUG_CHECK(*srt, "Error: header no imagenames found in file image list file");

  /* Automatically determine number of pixels in images    */
  sprintf(name, "%s/%s", imageDirectory, (*srt)->filename);
  DEBUG(1, "Autodetecting number of pixels, i.e. vector length based on the size of image 0.");
  DEBUG_CHECK (autoFileLength(name) == s->numPixels, "Images sizes do not match subspace basis vector size");
  DEBUG_INT(1, "Vector length", s->numPixels);
  DEBUG_CHECK(s->numPixels > 0, "Error positive value required for a Vector Length");

  /*Images stored in the columns of a matrix */
  DEBUG(1, "Allocating image matrix");

  images = makeMatrix(s->basis->col_dim, *numImages+1);

#ifdef THRD
  ns_per_thread = numSubj/NUM_THREADS;
  extra_subj = numSubj % NUM_THREADS;

  i=0;
  thread_data[0].start_sub = *srt;
  thread_data[NUM_THREADS-1].stop_sub = NULL;
  j = 0;
  for (subject = *srt; j<NUM_THREADS-1; subject = subject->next_subject) {
    i++;
    if (i==ns_per_thread+((extra_subj)? 1:0)) {
      j++;
      i = 0;
      extra_subj = (extra_subj)? extra_subj-1 : 0;
      thread_data[j-1].stop_sub = thread_data[j].start_sub = subject;
    }
  }
#if (NUM_THREADS>1)
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif

  for(i=0;i < NUM_THREADS;i++)
  {
    int pid=0;
    thread_data[i].s = s;
    thread_data[i].imageDirectory = imageDirectory;
    thread_data[i].images = images;
    thread_data[i].vector = vector;
    thread_data[i].name = name;


#if (NUM_THREADS>1)
    if (i<NUM_THREADS-1) {
      
      /* create threads */
      if (pthread_create(&thread[i], &attr, thread_work,
                       (void*) &thread_data[i]))
        {
          printf("ERROR; return code from pthread_create() is \n");
          exit(-1);
        }
    }
#endif

  }

  /* Free attribute and wait for the other threads */
#if (NUM_THREADS >1)
  pthread_attr_destroy(&attr);
#endif

  thread_work((void*) &thread_data[NUM_THREADS-1]);

#if (NUM_THREADS >1)
  for(i=0;i < NUM_THREADS-1;i++)
  {
    if (pthread_join(thread[i], (void **)&status))
      {
        printf("ERROR; return code from pthread_join() is \n");
        exit(-1);
      }
  }
#endif

#else /* not THRD */
  i = 0;
  vector = makeMatrix(s->numPixels, 1);
  for (subject = *srt; subject; subject = subject->next_subject) {
    for (replicate = subject; replicate; replicate = replicate->next_replicate) {
      if (debuglevel > 0)
        printf("%s ", replicate->filename);
      sprintf(name, "%s/%s", imageDirectory, replicate->filename);
      replicate->imageIndex = i;
      readFile(name, 0, vector);

      writeProgress("Reading images", i,*numImages);
      smallVector = centerThenProjectImages(s, vector);

      /* Copy the smaller vector into the image matrix*/
      for (j = 0; j < smallVector->row_dim; j++) {
	ME(images, j, i) = ME(smallVector, j, 0);
      }
      freeMatrix(smallVector);
      i++;  /* increament the image index */
    }
    if (debuglevel > 0)
      printf("\n");
  }
#endif
    
  return images;
}

/* basis_normalize

Using the snapshot method, the Eigenvectors of the original problem are
derived from the eigenvectors of the smaller covariance matrix by
premultiplying the original eigenvectors by the data matrix. This operation
give Eigenvectors that point in the proper direction, but that are not yet of
unit length. So, we normalize them.
*/
void
basis_normalize(Matrix eigenvectors)
{
    int i, j;
    FTYPE sumsqr, inv_len;
    for (i = 0; i < eigenvectors->col_dim; i++)
      {
        sumsqr = 0.0;
        for (j = 0; j < eigenvectors->row_dim; j++)
	  sumsqr += ME(eigenvectors, j, i) * ME(eigenvectors, j, i);
        if (sumsqr != 0)
	  inv_len = 1.0 / sqrt(sumsqr);
        else
	  inv_len = 0;
        for (j = 0; j < eigenvectors->row_dim; j++)
	  ME(eigenvectors, j, i) *= inv_len;
      }
}


/* mean_subtract_images

This function subtracts a mean image from a set of images
in matrix images
*/

#if 0 /* original code */

void
mean_subtract_images (Matrix images, Matrix mean)
{ 

int i, j;
  for (i = 0; i < images->row_dim; i++) {
    for (j = 0; j < images->col_dim; j++) {
      ME(images, i, j) -= ME(mean, i, 0);
    }
  }
}

#else /* unrolled code */

void
mean_subtract_images (Matrix images, Matrix mean)
{
  int i, j;

  assert(!(images->row_dim %2)  && "unrolled twice");

  for (i = 0; i < images->row_dim; i+=2) {

    for (j = 0; j < images->col_dim; j++) {
      ME(images, i, j) -= ME(mean, i, 0);
      ME(images, i+1, j) -= ME(mean, i+1, 0);
    }
  }
}


#endif /* end unrolled code */



/* get_mean_image

This function takes a matrix of images and returns the mean of
all of the images in the matrix.
*/
Matrix
get_mean_image (Matrix images)
{
  int i, j;
  Matrix mean = makeMatrix(images->row_dim, 1);

  for (i = 0; i < images->row_dim; i++)
    {
      ME(mean, i, 0) = 0.0;
      for (j = 0; j < images->col_dim; j++)
	ME(mean, i, 0) += ME(images, i, j);
      ME(mean, i, 0) = ME(mean, i, 0) / images->col_dim;
    }

  return mean;
}



