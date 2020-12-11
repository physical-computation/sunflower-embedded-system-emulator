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
 File: csuFileCommon.c
 Authors: J. Ross Beveridge, David Bolme and Kai She
 Date:    March 15, 2002
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
 * Purpose: This file contains File IO routines used by csuSubspaceTrain
 * and csuSubspaceTest.
 */


/******************************************************************************
*                               INCLUDES                                      *
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#include "csuCommon.h"

/******************************************************************************
*                               GLOBALS                                       *
******************************************************************************/

/**
 * EL and NUM are used to read binary data and convert to proper internal float representation
 */

typedef struct {char a, b, c, d;}
EL;
typedef union {float f; EL elem;} NUM;

/******************************************************************************
*                               FILE UTILITIES                                *
******************************************************************************/

/*
 The following six functions are used to read and write binary information to file
 in a way that is platform independant.  Each function checks the endianness of
 the current architecture and reverses the byte order if nessessary.
 */

typedef struct {char a,b,c,d;} bytes4;
typedef union {float n; bytes4 elem;} float4;
typedef union {int n; bytes4 elem;} int4;

typedef struct {char a,b,c,d,e,f,g,h;} bytes8;
typedef union {double n; bytes8 elem;} double8;

void writeInt(FILE* f, int n){
    int4 tmp;
    tmp.n = n;
    if( isMachineLittleEndian() ){
        fwrite(&(tmp.elem.d),1,1,f);
        fwrite(&(tmp.elem.c),1,1,f);
        fwrite(&(tmp.elem.b),1,1,f);
        fwrite(&(tmp.elem.a),1,1,f);
    }
    else{
        fwrite(&(tmp.elem.a),1,1,f);
        fwrite(&(tmp.elem.b),1,1,f);
        fwrite(&(tmp.elem.c),1,1,f);
        fwrite(&(tmp.elem.d),1,1,f);
    }
}

void writeFloat(FILE* f, float n){
    float4 tmp;
    tmp.n = n;
    if( isMachineLittleEndian() ){
        fwrite(&(tmp.elem.d),1,1,f);
        fwrite(&(tmp.elem.c),1,1,f);
        fwrite(&(tmp.elem.b),1,1,f);
        fwrite(&(tmp.elem.a),1,1,f);
    }
    else{
        fwrite(&(tmp.elem.a),1,1,f);
        fwrite(&(tmp.elem.b),1,1,f);
        fwrite(&(tmp.elem.c),1,1,f);
        fwrite(&(tmp.elem.d),1,1,f);
    }
}

void writeDouble(FILE* f, double n){
    double8 tmp;
    tmp.n = n;
    if( isMachineLittleEndian() ){
        fwrite(&(tmp.elem.h),1,1,f);
        fwrite(&(tmp.elem.g),1,1,f);
        fwrite(&(tmp.elem.f),1,1,f);
        fwrite(&(tmp.elem.e),1,1,f);
        fwrite(&(tmp.elem.d),1,1,f);
        fwrite(&(tmp.elem.c),1,1,f);
        fwrite(&(tmp.elem.b),1,1,f);
        fwrite(&(tmp.elem.a),1,1,f);
    }
    else{
        fwrite(&(tmp.elem.a),1,1,f);
        fwrite(&(tmp.elem.b),1,1,f);
        fwrite(&(tmp.elem.c),1,1,f);
        fwrite(&(tmp.elem.d),1,1,f);
        fwrite(&(tmp.elem.e),1,1,f);
        fwrite(&(tmp.elem.f),1,1,f);
        fwrite(&(tmp.elem.g),1,1,f);
        fwrite(&(tmp.elem.h),1,1,f);
    }
    
}

void readInt(FILE* f, int* n){
    int4 tmp;
    if(isMachineLittleEndian() ){
        fread(&(tmp.elem.d),1,1,f);
        fread(&(tmp.elem.c),1,1,f);
        fread(&(tmp.elem.b),1,1,f);
        fread(&(tmp.elem.a),1,1,f);
    }
    else{
        fread(&(tmp.elem.a),1,1,f);
        fread(&(tmp.elem.b),1,1,f);
        fread(&(tmp.elem.c),1,1,f);
        fread(&(tmp.elem.d),1,1,f);
    }
    *n = tmp.n;
}

void readFloat(FILE* f, float* n){
    float4 tmp;
    if(isMachineLittleEndian() ){

      fread(&(tmp.elem.d),1,1,f);
      fread(&(tmp.elem.c),1,1,f);
      fread(&(tmp.elem.b),1,1,f);
      fread(&(tmp.elem.a),1,1,f);
    }
    else{
      fread(&(tmp.elem.a),1,1,f);
      fread(&(tmp.elem.b),1,1,f);
      fread(&(tmp.elem.c),1,1,f);
      fread(&(tmp.elem.d),1,1,f);
    }
    *n = tmp.n;
}

/*
void readDouble(FILE* f, double* n){
  double8 tmp;

  fread(&tmp, 8, 1, f);
  *n = tmp.n;
}
*/

void readDouble(FILE* f, double* n){
    double8 tmp;

    
    if(isMachineLittleEndian() ){
        fread(&(tmp.elem.h),1,1,f);
        fread(&(tmp.elem.g),1,1,f);
        fread(&(tmp.elem.f),1,1,f);
        fread(&(tmp.elem.e),1,1,f);
        fread(&(tmp.elem.d),1,1,f);
        fread(&(tmp.elem.c),1,1,f);
        fread(&(tmp.elem.b),1,1,f);
        fread(&(tmp.elem.a),1,1,f);
    }
    else{
        fread(&(tmp.elem.a),1,1,f);
        fread(&(tmp.elem.b),1,1,f);
        fread(&(tmp.elem.c),1,1,f);
        fread(&(tmp.elem.d),1,1,f);
        fread(&(tmp.elem.e),1,1,f);
        fread(&(tmp.elem.f),1,1,f);
        fread(&(tmp.elem.g),1,1,f);
        fread(&(tmp.elem.h),1,1,f);
    }
    *n = tmp.n;
}



/**
 * Given a directory name and a file name, this function returns a
 * correctly formed path string.
 *
 * Caveat: This function returns a pointer to an internal buffer,
 *         so the value it returns is valid only until the next
 *         call to makePath
 *
 * @param directoryName Directory part
 * @param fileName File name part
 * @returns Pointer to buffer containing combined path 
 */

char *
makePath (const char *directoryName, const char *fileName)
{
    static char path[255];
    const char *osDependentPathSeparator = "/";
    sprintf (path, "%s%s%s", directoryName, osDependentPathSeparator, fileName);
    return path;
}

/**
 * Ensures that a given directory is writeable. Prints a error message
 * and exits if it is not or if it is not a directory.
 *
 * @param directory Path to an allegedly writeable directory
 * @param message Message to print if allegation is false
 */

void
checkWriteableDirectory (const char *directory, const char *message)
{
    struct stat buf;
    int err = stat (directory, &buf);
    int ok = (err == 0) && S_ISDIR (buf.st_mode)
        && (access (directory, W_OK) == 0);

    if (!ok)
    {
        fprintf (stderr, message, directory);
        fprintf (stderr, "\n");
        exit (1);
    }
}

/**
 * Ensures that a given directory is readable. Prints a error message
 * and exits if it is not or if it is not a directory.
 *
 * @param directory Path to an allegedly readable directory
 * @param message Message to print if allegation is false
 */

void
checkReadableDirectory (const char *directory, const char *message)
{
    struct stat buf;
    int err = stat (directory, &buf);
    int ok = (err == 0) && S_ISDIR (buf.st_mode)
        && (access (directory, R_OK) == 0);
    if (!ok)
    {
        fprintf (stderr, message, directory);
        fprintf (stderr, "\n");
        exit (1);
    }
}

/**
 * Ensures that a given file is readable. Prints a error message
 * and exits if it is not or if it is not a file.
 *
 * @param directory Path to an allegedly readable file
 * @param message Message to print if allegation is false 
 */
void
checkReadableFile (const char *file, const char *message)
{
    struct stat buf;
    int err = stat (file, &buf);
    int ok = (err == 0) && S_ISREG (buf.st_mode) && (access (file, R_OK) == 0);
    if (!ok)
    {
        fprintf (stderr, message, file);
        fprintf (stderr, "\n");
        exit (1);
    }
}

/******************************************************************************
*                     SIMILARITY RANKING UTILITIES                            *
******************************************************************************/

/**
 * Reads all of the names in a file into an array of
 * strings. Each element of the array is a char pointer.
 *
 * If nStrings is non-NULL, the number of strings is
 * returned in nStrings.
 *
 * The array is always one larger than nStrings elements
 * long and the last element is a NULL.
 *
 * @param fileName Name of file to read
 * @param nStrings Optional pointer to integer that will on return contain
 *                 the count of strings read from the file
 * @returns A NULL-terminated list of NULL-terminated strings
 */
ListOfStrings
readListOfStrings (const char *fileName, int *nStrings)
{
    Tokenizer tok;
    void *stringList = NULL;
    FILE *f;
    char *string;
    ListOfStrings array;
    size_t numStrings;

    f = fopen (fileName, "r");
    DEBUG_CHECK (f, "Unable to read file");

    tokenizerInit (&tok, tokenizerStreamReader, f);
    while (!tokenizerEndOfFile (&tok))
    {
        string = strdup (tokenizerGetWord (&tok));
        listAccumulate (&stringList, &string, sizeof (unsigned char *));
    }

    fclose (f);

    /* Add a terminating NULL */
    string = NULL;
    listAccumulate (&stringList, &string, sizeof (unsigned char *));

    array = (ListOfStrings)listToArray (&stringList, sizeof (unsigned char *), &numStrings);
    if (nStrings) *nStrings = numStrings - 1; /* Don't count the NULL */
    return array;
}

/**
 * Frees a list of strings.
 *
 * @param list A NULL-terminated list of NULL-terminated strings.
 */
void
freeListOfStrings (ListOfStrings list)
{
    char **p = list;
    while (*p != NULL)
        free (*p++);
    free (list);
}

/**
 * This function counts the string in a NULL-terminated list of strings
 *
 * @param list A NULL-terminated list of NULL-terminated strings.
 * @returns The count of strings in the list
 */

int
countStrings (ListOfStrings list)
{
  int count = 0;
  char **p = list;
  while (*p++ != NULL)
    count++;
  return count;
}

/******************************************************************************
*                     SIMILARITY RANKING UTILITIES                            *
******************************************************************************/

/**
 * A data structure for associating a name with a distance score.
 *
 * This structure is used internally by "sortSubjectsBySimilarityToProbe"
 */

typedef struct
{
  char *subject;
  double distance;
  int index;
}
DistanceMeasure;

/**
 * A qsort helper function for sorting DistanceMeasure records.
 *
 * This function is used internally by "sortSubjectsBySimilarityToProbe"
 *
 * @param a The first object in the pair to compare
 * @param b The second object in the pair to compare
 */

int
distanceMeasureComparator (const void *a, const void *b)
{
  const DistanceMeasure *d1 = (DistanceMeasure *)a;
  const DistanceMeasure *d2 = (DistanceMeasure *)b;

  if (d1->distance < d2->distance)
    return -1;
  if (d1->distance > d2->distance)
    return 1;
  return 0;
}

/**
 * Given a pointer to an array of names of various subjects, and a probe image,
 * this function sorts the array by similarity to the probe image. As a special
 * case, if the probe image appears in the subject list, it is ranked as most
 * dissimilar. This function takes the name of a distance matrix from which
 * the scores are read. As a special case, if this is NULL, then scores are
 * picked at random, which effectively shuffles the array.
 *
 * If indices is non-NULL, subjects is not shuffled, but instead the indices
 * of the rearranged items are returned in indices.
 *
 * @param probe A probe image against which the subjects are sorted
 * @param subjects A NULL-terminated list of NULL-terminated strings.
 * @param distanceMatrix Distance matrix from which scores are read.
 * @param indices Optional array of integers that will contain the indices of the sorted
 *                elements in the original sequence. This parameter may be NULL.
 */

void
sortSubjectsBySimilarityToProbe (char *probe, ListOfStrings subjects, char *distanceMatrix, int *indices)
{
  int i, j, nDistances;
  DistanceMeasure *distances = NULL;
  DistanceMeasure *toSort = NULL;

  int nSubjects = countStrings (subjects);

  /* If we are using a distance matrix, then we load up the
   * list of distances from the probe image. This allows us
   * to lookup the distances from the probe quickly
   */
  if (distanceMatrix)
    {
      Tokenizer tok;
      FILE *f = fopen (makePath (distanceMatrix, probe), "r");
      void *distanceList = NULL;

      DEBUG_CHECK_1ARG (f, "Unable to open file %s in scores directory", makePath (distanceMatrix, probe));

      tokenizerInit (&tok, tokenizerStreamReader, f);
      while (!tokenizerEndOfFile (&tok))
	{
	  DistanceMeasure m;
	  m.subject = strdup (tokenizerGetWord (&tok));
	  m.distance = atof (tokenizerGetWord (&tok));
	  listAccumulate (&distanceList, &m, sizeof (DistanceMeasure));
	}
      fclose (f);
      distances = (DistanceMeasure*) listToArray (&distanceList, sizeof (DistanceMeasure), (size_t*)&nDistances);
    }

  /* Copy the list of names into the intermediate data structure that allows us to sort the
   * subjects by distance to the probe.
   */

  toSort = (DistanceMeasure*) malloc (nSubjects * sizeof (DistanceMeasure));

  for (j = 0; j < nSubjects; j++)
    {
      toSort[j].subject = subjects[j];
      toSort[j].distance = 0.0;
      toSort[j].index = j;
    }

  /* Read distances between probe and every other image in the subject list.
   * As a special case, a subject is said to be infinitely far away from him/herself.
   * Random scores are used when a distanceMatrix is not provided */

  for (j = 0; j < nSubjects; j++)
    {
      if (strcmp (subjects[j], probe) == 0) {

	/* Probe and subject are the same. Say they are far apart since
	 * we don't want to treat an image and itself as being two replicates
	 * of a person */

	toSort[j].distance = HUGE_VAL;

      } else if (distanceMatrix != NULL) {

	/* Look for the subject in the list of distances and return assign the 
	 * score */

	toSort[j].distance = HUGE_VAL;

	for (i = 0; i < nDistances; ++i)
	  if (strcmp (distances[i].subject, toSort[j].subject) == 0)
	    toSort[j].distance = distances[i].distance;

      } else {

	/* If we are not using a distance matrix, then choose
	 * a random value */

	toSort[j].distance = ((double) random ()) / RAND_MAX;

      }
    }

  /* Now sort the list by similarity to the probe */

  qsort (toSort, nSubjects, sizeof (DistanceMeasure), distanceMeasureComparator);

  /* Copy the data back into the subject list or return the permuted indices */

  if (indices == NULL)
    for (j = 0; j < nSubjects; j++) {
      subjects[j] = toSort[j].subject;
    }
  else
    for (j = 0; j < nSubjects; j++) {
      indices[j] = toSort[j].index;
    } 

  /* Clean up */

  if (distanceMatrix)
    {
      for (i = 0; i < nDistances; ++i)
	free (distances[i].subject);
      free (distances);
    }

  free (toSort);
}

#define FREAD_FLOAT_CNT 256


/******************************************************************************
*                     FUNCTIONS FOR WORKING WITH SFI FILES                    *
******************************************************************************/

/**
 * Read a binary image from a file where the pixels are stored as 32 bit
 * floating point numbers using Sun byte order.  The compiler directive WINDOWS
 * is used to control how the byte order is interpreted when storing the pixels
 * into the data structure images. Images are a one dimensional array of floats
 * that contains each successive image in sequence, where each image has in turn
 * been unrolled into a 1 dimensional vector.
 *
 * @param fname The name of the file from which to read the image.
 * @param n The index of this image in the images array.
 * @param images The matrix into the image is read. Only the nth column is affected.
 */
void readFile(const char *fname, int n, Matrix images) {
    int i;
    FILE *f;
#ifdef FASTREAD
    float fread_buf[FREAD_FLOAT_CNT];
#endif
    /*  char nan_error[100];
	char inf_error[100];*/
    char line[FILE_LINE_LENGTH];
    char imagetype[FILE_LINE_LENGTH];

    /*if (debuglevel > 1)*/
    printf("\nReading file: %s\n", fname); fflush(stdout);

    /* Check to makesure file is of expected size */
    if (images->row_dim != autoFileLength(fname)) {
        DEBUG_CHECK(autoFileLength(fname) < images->row_dim, "File does not contian enough values");
        printf("Warning: file length is greater than vector length.  Croping file...\n");
    }


    f = fopen( fname, "rb" );
    assert(f);
    /*if ( !f ) { printf("Can't open %s\n", fname); exit(1); }*/

    /* check to see if image is of type RASTER_ID */
    fgets(line,FILE_LINE_LENGTH,f); /* only read in enough to determine
                                     * if the file is of proper type */
    sscanf(line,"%s",imagetype);
    if(strcmp(imagetype,RASTER_ID) == 0 || strcmp(imagetype,"CSU_RASTER") ==0){
        rewind(f);
        /* read in the first line to move the buffer to the data. */
        fgets(imagetype,FILE_LINE_LENGTH,f);
    }
    else{
        /* There is no header info. Just read the data */
        rewind(f);
    }

#ifdef FASTREAD
    for (i = 0; i < images->row_dim; ) {
      int numread=0, j=0;
      int toread = images->row_dim - i;

      if (toread > FREAD_FLOAT_CNT)
	toread = FREAD_FLOAT_CNT;

      numread = fread(fread_buf, 4, toread, f);
      assert((toread == numread) && "fread failure");

      for (j=0; j < numread; j++)
	ME(images, i, n) = (FTYPE) fread_buf[j];

      i+= numread;
    }

#else

    /* Set up error messages for use later */
    /*   sprintf(nan_error, "Not A Number value in file: %s", fname);
	 sprintf(inf_error, "Infinite value in file: %s", fname); */

    for (i = 0;i < images->row_dim;i++) {
         float flt;
         /* read in the correct byte order for floating point values */
        readFloat(f, &flt);

#ifdef CHECK_VALS
        /* Check values to make sure they are real */
	/*  FINITE(flt);*/
	if(!finite(n)) { assert(0 && "infinite value");} 

#endif
        /* Save value to images */
        ME(images, i, n) = (FTYPE)flt;
    }
#endif

    fclose(f);
}






/**
 * This code was used to detect the length of old nrm images.  That file
 * format is no longer in use the function was renamed and will soon be deleted
 * -- DSB
 *
 * autoFileLength returns the filelength/4 of imageName. This is used
 * to check the image files and determine at run time how many
 * floating point values. Note, stat returns the number of bytes, and
 * the divide by four converts to the number of floats.  autoFileLength
 * lets the code here auto detect the size of the images, i.e. the row
 * dimension of the covariance matrix, without requiring the user to
 * pass this value in as an argument
 *
 * @param imageName Path to an image
 * @return The size of the image
 */
int autoFileLength(const char* imageName) {
    FILE* f;
    char imagetype[FILE_LINE_LENGTH];
    char line[FILE_LINE_LENGTH];

    /* First, check to see if the file is of type RASTER_ID. This is a newer
       image format which contains the dimensions in a header
     */

    f = fopen(imageName,"r");
    if ( !f ) { printf("Can't open %s\n", imageName); exit(1); }

    fgets(line,FILE_LINE_LENGTH,f); /* only read in enough to determine
                                     * if the file is of proper type */
    fclose(f);

    sscanf(line,"%s",imagetype);

    if (strcmp(imagetype,RASTER_ID) == 0 || strcmp(imagetype,"CSU_RASTER") == 0){
      int w,h,c;

      sscanf(line,"%s %d %d %d",imagetype,&w,&h,&c);
      return w*h*c;
    } else {
      /* Otherwise, we are using an old format and the we return
         the filelength/4 as the size */
      struct stat filestatus;

      DEBUG_CHECK(0,"Old nrm image format no longer supported");
      if (stat(imageName, &filestatus)) {
        fprintf(stderr, "Error: Could not stat file %s\n", imageName);
        exit(1);
      }
      return filestatus.st_size / 4;
    }
}

/**
 * Code to read the image names being passed in and to create the 2 dimensional table
 * of these names. Each row is a different subject. Each column is a replicate for that
 * subject. Linked lists are used along each dimension since prior to reading the file
 * the number of subjects is unknown, as is the number of replicates per subject. Finally,
 * the number of replicates per subject may vary for different subjects.
 *
 * @param filename A filename to assign to the node.
 * @returns A malloc'd ImageList node.
 */

ImageList* createILNode(char* filename) {
    ImageList* node = (ImageList*)malloc(sizeof(ImageList));

    node->filename = (char*)strdup(filename);

    node->imageIndex = 0;
    node->next_replicate = NULL;
    node->next_subject = NULL;
    return node;
}

/**
 * This function reads in the image list file and sets
 * up a subject replicates list.  First the file is
 * probed to determine the maximum line length.  Then
 * the lines are read in and replicate names are parsed.
 *
 * @params imageNamesFile Path to a file containing an srt
 * @params numImages Pointer to integer that will hold the number of names read
 * @return A two dimensional ImageList.
 */
#ifdef THRD
ImageList* getImageNames(char* imageNamesFile, int *numImages, int* numSub) {
#else
ImageList* getImageNames(char* imageNamesFile, int *numImages) {
#endif
    Tokenizer tok;
    char* token;
    FILE* ilf; /* Image List File */
    ImageList *subject = NULL, *replicate, *header = NULL;
    int nImages;
#ifdef THRD
    int nSub=0;
#endif
    
    DEBUG_STRING(2, "Get image Names from file", imageNamesFile);

    ilf = fopen( imageNamesFile, "r");
    DEBUG_CHECK(ilf, "Problem opening image list file");

    nImages = 0;
    tokenizerInit (&tok, tokenizerStreamReader, ilf);
    while (!tokenizerEndOfFile (&tok))
      {
	token = tokenizerGetWord (&tok);
	nImages++;
#ifdef THRD
	nSub++;
#endif

	if (header == NULL) {
	  subject = header = createILNode(token);
	} else {
	  subject->next_subject = createILNode(token);
	  subject = subject->next_subject;
	}

	replicate = subject;
	/*#ifdef THRD*/
	replicate->imageIndex = nImages-1;
	/*#endif*/

        while (!tokenizerEndOfLine(&tok) && !tokenizerEndOfFile(&tok))
	  {
	    token = tokenizerGetWord (&tok);
	    nImages++;

	    replicate->next_replicate = createILNode(token);
	    replicate = replicate->next_replicate;
	    /*#ifdef THRD*/
	replicate->imageIndex = nImages-1;
	/*#endif*/
	  }
      }

    fclose(ilf);

    if (numImages)
#ifdef THRD
      { 
	if (numSub)
	  *numSub = nSub;
	printf("Number of subect is %d\n",nSub);
#endif
      *numImages = nImages;
#ifdef THRD
      }
#endif

    return header;
}

/**
 * This function reads in the image list file.  It then
 * takes the image filenames and reads in every file.
 *
 * @returns A Matrix containing all of the images
 */
Matrix readImages(char *imageNamesFile, char *imageDirectory, int *numPixels, int *numImages, int *numSubjects, ImageList **srt) {
    int i;
    Matrix images;
    ImageList *subject, *replicate;

    DEBUG(1, "Reading training file names from file");

#ifdef THRD
    *srt = getImageNames(imageNamesFile, numImages, NULL);
#else
    *srt = getImageNames(imageNamesFile, numImages);
#endif

    DEBUG_CHECK(*srt, "Error: header no imagenames found in file image list file");

    /* Automatically determine number of pixels in images    */

    DEBUG(1, "Autodetecting number of pixels, i.e. vector length based on the size of image 0.");
    *numPixels = autoFileLength(makePath(imageDirectory, (*srt)->filename));
    DEBUG_INT(1, "Vector length", * numPixels);
    DEBUG_CHECK(*numPixels > 0, "Error positive value required for a Vector Length");

    /* Images stored in the columns of a matrix */
    DEBUG(1, "Allocating image matrix");
    images = makeMatrix(*numPixels, *numImages);

    i = 0;
    (*numSubjects) = 0;
    for (subject = *srt; subject; subject = subject->next_subject) {
        for (replicate = subject; replicate; replicate = replicate->next_replicate) {
            if (debuglevel > 0)
                printf("%s ", replicate->filename);
            replicate->imageIndex = i;
            readFile(makePath(imageDirectory, replicate->filename), i++, images);
        }
        if (debuglevel > 0)
            printf("\n");
        (*numSubjects)++;
    }

    return images;

}

/**
 * This function returns the count of all images in an ImageList
 * takes the image filenames and reads in every file.
 *
 * @param srt An image list
 * @returns The count of images
 */
int numImageInImageList(ImageList *srt) {
    int num = 0;
    ImageList *subject, *replicate;

    for (subject = srt; subject; subject = subject->next_subject) {
        for (replicate = subject; replicate; replicate = replicate->next_replicate) {
            num++;
        }
    }
    return num;
}

/**
 * This function is supposed to free an image list but is not implemented.
 * I put this in as a placeholder.
 *
 * @param srt An image list
 */
void freeImageNames(ImageList *srt) {
  /* Not implemented */
}

/*------------------------------------------------------------------------
Utilities integrated directly into this source from elsewhere in FERET
code distribution.
------------------------------------------------------------------------*/

/**
 * Reads a FERET nrm file
 *
 * @param fn Filename
 * @param numpix Number of pixels to read
 * @returns An malloc'd array of floats
 */
float *readFeretRaster(const char *fn,  int numpix)
{
    FILE *fp;
    float* data;

    fp = fopen(fn, "rb");
    if (fp == NULL)
      {
        fprintf (stderr, "failed to open %s", fn);
	exit(0);
      }

    data = (float*)malloc(sizeof(float)*numpix);
    DEBUG_CHECK (data, "malloc failed");

    if (!fread(data, sizeof(float), numpix, fp))
      {
        fprintf (stderr, "fread in readFeretRaster failed");
	exit(0);
      }

    if (isMachineLittleEndian()) byteswap_4(data, numpix);

    fclose(fp);
    return data;
}

/**
 * Writes a FERET nrm file
 *
 * @param fn Filename
 * @param data Pointer to an array of floats
 * @param numpix Number of pixels to read
 * @returns Whatever was passed in as data
 */
float *writeFeretRaster(const char *fn, float *data, int numpix)
{
    FILE *fp;

    fp = fopen(fn, "wb");
    if (fp == NULL)
      {
        fprintf (stderr, "failed to open %s", fn);
	exit(0);
      }

    if (isMachineLittleEndian()) byteswap_4(data, numpix);

    if (numpix != fwrite(data, sizeof(float), numpix, fp))
      {
        fprintf (stderr, "fwrite in writeFeretRaster failed");
	exit(0);
      }

    fclose(fp);
    return data;
}

/**
 * Reads PGM file
 *
 * @param filename The name of the file to read
 * @param w Pointer to an integer which will receive the width of the image
 * @param h Pointer to an integer which will receive the height of the image
 * @param verbose When true, we print status information
 * @returns A pointer to a malloc'd array of bytes
 */
unsigned char *readImagePGM(const char *filename, int *w, int *h, int verbose)
{
    int  width, height, max, i;
    int  val;
    char fchar;
    char line[100];
    char ftype[16];
    FILE *infile;
    unsigned char *im;

    /* Read first line, and test if it is propoer PNM type */
    if (verbose) fprintf(stdout,"Going to open file %s\n", filename);
    infile = fopen(filename, "rb");
    if (infile == NULL)
      {
        fprintf (stderr, "failed to open %s", filename);
	exit(0);
      }
    fgets(line,100,infile);
    sscanf(line,"%s",ftype);
    if (verbose) fprintf(stdout,"File Type is %s.\n", ftype);

    if (strcmp(ftype,"P5") != 0)
      {
	fprintf (stderr, "Currently only binary pgm files, type P5, supported");
	exit(0);
      }

    /* Read lines, ignoring those starting with Comment Character, until the
        Image Dimensions are read. */
    fchar = '#';
    while (fchar == '#') {
        fgets(line,100,infile);
        sscanf(line, " %c", &fchar);
    }
    if (verbose) fprintf(stdout,"Second non-comment line of image file %s.\n", line);
    sscanf(line, " %d %d", &width, &height);
    *w = width;
    *h = height;
    if (verbose) fprintf(stdout,"The width,  height and size are: %d %d %d\n", width, height, width * height);

    /* Read lines, ignoring those starting with Comment Character, until the
        maximum pixel value is read. */
    fchar = '#';
    while (fchar == '#') {
        fgets(line,100,infile);
        sscanf(line, " %c", &fchar);
    }
    sscanf(line, "%d", &max);
    if (verbose) fprintf(stdout,"The max value for the pixels is: %d\n", max);
    if (! (max == 255)) {
        fprintf(stdout,"readImagePGM: Warning, max value %d for pixels in image %s is not 255\n", max, filename);
    }

    im = (unsigned char*) malloc(sizeof(unsigned char)*width*height);
    DEBUG_CHECK (im, "malloc failed");

    i = 0;
    val = fgetc(infile);
    while (!(val == EOF) && i < width*height) {
        im[i] = (unsigned char) val;
        i++;
        val = fgetc(infile);
    }
    if (verbose) fprintf(stdout,"Read in %d Pixel Values\n", i);
    fclose(infile);
    return( im );
}

/**
 * Writes a PGM file
 *
 * @param fn The name of the file to write
 * @param data An array of floats containing the image data
 * @param numpix Number of pixels. Must be w * h
 * @param w Pointer to an integer which will receive the width of the image
 * @param h Pointer to an integer which will receive the height of the image
 */
void writeImagePGM(const char *fn, float *data, int numpix,int w, int h)
{
    float min, max, sum, scale;
    int  i, val;
    FILE *fp;

    min = data[0];
    max = data[0];
    sum = 0.0;

    for (i = 1; i < numpix; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
        sum = sum + data[i];
    }

    scale = 255.0 / (max - min);

    fp = fopen(fn, "wb");
    if (fp == NULL)
      {
        fprintf (stderr, "failed to open %s", fn);
	exit(0);
      }

    fprintf(fp,"P5\n");
    fprintf(fp,"%d %d\n", w, h);
    fprintf(fp,"255\n");
    for (i = 0; i < numpix; i++) {
        val = (int) ((data[i] - min) * scale);
        fputc(val,fp);
    }
    fclose(fp);
}
