/*
File: csuCovariatesStudy.c                                                     
Authors: David Bolme, Marcio L. Teixeira                           
Date: ???                                                    
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


using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <csuCommon.h>

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#define OPENING  "Generate the data file for the covariate study."
#define VERSION  "Version 4.0, released Oct 3, 2002."

#define MAX_FILES 10

/******************************************************************************
*                       OUTPUT CONFIGURATION                                  *
******************************************************************************/

#define DELIMITER ","

/******************************************************************************
*                         HELPFUL TYPEDEFS AND MACROS                         *
******************************************************************************/

/* Define custom string names that are more
   descriptive of their meanings
*/
typedef string                                 ImageId;
typedef string                                 CovLabel;
typedef string                                 CovValue;
typedef string                                 DistName;
typedef map<CovLabel, map<ImageId, CovValue> > CovariateMap;
typedef map<ImageId, double>                   DistanceMap;
typedef map< int , int >                       Histogram;

#define EACH_SUBJECT(srt, subject)           (ImageList* subject = srt; subject != NULL; subject = subject->next_subject)
#define EACH_REPLICATE(srt, replicate)       (ImageList* replicate = srt; replicate != NULL; replicate = replicate->next_replicate)

/******************************************************************************
*                               GLOBALS                                       *
******************************************************************************/

/**
 * The command line arguments are managed by a single structure.
 */
typedef struct {
  char *imageTable;
  char *covariateFiles[MAX_FILES];
  char *distanceDirs[MAX_FILES];
  int nCovariateFiles;
  int nDistanceDirs;
  int nRank;
  char *trainingFile;
  char *outputFile;
}
Arguments;

/**
   Prints out a usage statement
   
   @param prog_name The program name
 */
void
usage (const char* prog_name)
{
  cout << prog_name
       << "[options] image_table.srt training_table.srt outputFile.txt\n\n"
       << "The following options must be given at least once. For multiple files, they can be specified multiple times:\n"
       << "    -dist distanceMatrix          :  output distances and ranks for this measure\n"
       << "    -cov covariatesFile           :  load covariates from this file\n"
       << "The following option causes the n-closest images to be considered, rather than just\n"
       << "the replicates of the same subject\n"
       << "    -nrank n\n"
       << endl;

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

  args->imageTable      = NULL;
  args->trainingFile    = NULL;
  args->nCovariateFiles = 0;
  args->nDistanceDirs   = 0;
  args->nRank           = 0;

  quiet = 0;
  debuglevel = 0;

  for (i = 1; i < argc; i++) {

    /* Catch common help requests */
    if      (readOption       (argc, argv, &i, "-help" )) { usage(argv[0]); }
    else if (readOption       (argc, argv, &i, "--help")) { usage(argv[0]); }


    /* other flags */
    else if (readOption       (argc, argv, &i, "-quiet")) { quiet = 1; }
    else if (readOptionInt    (argc, argv, &i, "-debuglevel", &debuglevel));

    else if (readOptionString (argc, argv, &i, "-cov",  &args->covariateFiles[args->nCovariateFiles]))
      {
	args->nCovariateFiles++;
	DEBUG_CHECK(args->nCovariateFiles < MAX_FILES, "Too many covariate files specified");
      }

    else if (readOptionString (argc, argv, &i, "-dist", &args->distanceDirs[args->nDistanceDirs]))
      {
	args->nDistanceDirs++;
	DEBUG_CHECK(args->nDistanceDirs < MAX_FILES, "Too many distance directories specified");
      }

    else if (readOptionInt    (argc, argv, &i, "-nrank", &args->nRank));

    /* check if the current argument is an unparsed option */
    else if (checkBadOption(argc,argv,&i));

    else if (param_num == 0) {
      args->imageTable = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 1) {
      args->trainingFile = strdup(argv[i]);
      param_num++;
    }
    else if (param_num == 2) {
      args->outputFile = strdup(argv[i]);
      param_num++;
    }
  }

  if (param_num < 3)
    usage(argv[0]);

  /* Print out the program parameters for appropriate debug level */

  DEBUG_INT (1, "Debuging enabled", debuglevel);

  if(debuglevel > 0){
    printf ("***************** Program Parameters *********************\n");
    printf ("imageTable: %s\n", args->imageTable);
    printf ("trainingFile: %s\n", args->trainingFile);
  }
}

/******************************************************************************
*                            UTILTY FUNCTIONS                                 *
******************************************************************************/


/**
   Remove a file extention to form an image id

   @param s A filename
   @returns The filename with the extension removed
 */
ImageId
removeExtention (std::string s)
{
  int i;
  for (i = 0; i < s.length (); i++)
    if (s[i] == '.')
      break;

  return s.substr (0, i);
}

/**
   Remove the full path to leave only a distance measure name
   
   @param s A path
   @returns The string with the path part removed
 */
DistName
removePath (std::string s)
{
  DistName d;

  while (s[s.length ()-1] == '/')
    s = s.substr (0, s.length ()-1);

  for (int i = s.length () -1; i > 0; i--)
    if (s[i] == '/')
      {
	d = s.substr (i+1, s.length ()-1);
	return d;
      }

  return s;
}

/**
   This reads in an image covariates file. The lines of the file has this particular structure:
   
   imageId subjectNum numOfSubjCovs numOfImageCovs (subjCovLabel cov)* (imgCovLabel img)*

   @param filename The covariates file
   @return A map of covariates
 */
void
readCovariates (char* filename, CovariateMap &subjectCovariates, CovariateMap &imageCovariates)
{
  int filelength;
  ifstream file (filename);

  if (!file)
    {
      cout << "Error opening file: "
	   << filename
	   << endl;

      exit (1);
    }

  file >> filelength; // Ignored
  
  for (; 1 ;)
    {
      ImageId id, sub;
      int num1, num2;

      file >> id
	   >> sub
	   >> num1
	   >> num2;

      if (file.eof()) break;

      for (int j = 0; j < num1; j++)
	{
	  CovLabel label;
	  CovValue value;
     
	  file >> label
	       >> value;

	  subjectCovariates[label][id] = value;
	}

      for (int j = 0; j < num2; j++)
	{
	  CovLabel label;
	  CovValue value;
     
	  file >> label
	       >> value;

	  imageCovariates[label][id] = value;
	}
    }
}

/**
   Looks up a particular covariate in the map that was read
   in by readCovariates
   
   @param map The map that was read by readCovariates
   @param cov A covariate lable
   @param img An image name
 */
CovValue
getCovariateForImage (CovariateMap m, CovLabel cov, ImageId img)
{
  if (m.count (cov) == 0 || m[cov].count (img) == 0)
    {
      cout << endl
	   << "Error could not find covariate pair for <"
	   << img
	   << ","
	   << cov
	   << ">"
	   << endl;
      
      exit (1);
    }
  return m[cov][img];
}

/**
  Reads in a distance file
   
 @param filename
 @return A image distance map
 */
DistanceMap
readDistances (char* filename)
{
  DistanceMap m;

  ifstream file (filename);
  ImageId id1 = removeExtention (filename);
  string image2;
  double distance;

  while (file >> image2 >> distance)
    {
      ImageId id2 = removeExtention (image2);
      m[id2] = distance;
    }

  return m;
}

/**
   Looks up the distance of a particular image

   @param map A image distance map
   @param img An image
   @return A distance
*/
double
getImageDistance (DistanceMap map, ImageId img, DistName name)
{
  if (map.count (img) == 0)
    {
      cout << "Error accessing distances: <"
	   << img
	   << ","
	   << name
	   << ">"
	   << endl;
      
      exit(1);
    }
  return map[img];
}

/**
   Looks up the rank of a particular image

   @param map A image distance map
   @param img An image
   @return A rank number
*/
int
getImageRank (DistanceMap m, ImageId img)
{
  int rank = 0;
  for (DistanceMap::iterator each = m.begin(); each != m.end (); each++)
    if ((each->second < m[img]) && (img.substr (0,5) != each->first.substr (0,5)))
      rank++;
  return rank;
}

/**
test if the image id contained in the training image list
*/
int
inTraining (ImageId id, ImageList *trlist )
{
  int result = 0;
  for EACH_SUBJECT (trlist, subject)
    {
      ImageId tid = removeExtention(subject->filename);
      if (tid == id)
	result = 1;
    }
  return result;
}

void
printCovariates (ofstream &fout,
		 int rowNumber,
		 int subjectNumber,
		 ImageList *image1,
		 ImageList *image2,
		 CovariateMap subjectCovariates,
		 CovariateMap imageCovariates,
		 DistanceMap *distances,
		 int nDistances,
		 ImageList* trlist) {

  ImageId id1 = removeExtention (image1->filename);
  ImageId id2 = removeExtention (image2->filename);

  string subjectID1 = std::string (image1->filename).substr (0,5);
  string subjectID2 = std::string (image2->filename).substr (0,5);

  fout << rowNumber          << DELIMITER
       << subjectNumber      << DELIMITER
       << subjectID1         << DELIMITER
       << subjectID2         << DELIMITER
       << id1                << DELIMITER
       << id2                << DELIMITER
       << image1->imageIndex << DELIMITER
       << image2->imageIndex << DELIMITER;
	      
  /* Prints out the covariates for the pair. First, print covariates which
     are common to both images (the subject covariates), then print the
     covariates which are different for each image (the image covariates).
  */

  for (CovariateMap::iterator cov = subjectCovariates.begin(); cov != subjectCovariates.end (); cov++)
    fout << getCovariateForImage (subjectCovariates, cov->first, id1)
	 << DELIMITER;

  for (CovariateMap::iterator cov = imageCovariates.begin(); cov != imageCovariates.end (); cov++)
    {
      fout << getCovariateForImage (imageCovariates, cov->first, id1) << DELIMITER;
      fout << getCovariateForImage (imageCovariates, cov->first, id2) << DELIMITER;
    }

  /* Find the distance between the two images and the rank, using both the distance 1 and
   * the distance two metrics
   */

  for (int i = 0; i < nDistances; i++)
    {
      fout.setf (ios::scientific);
      fout << std::setprecision (16)
	   << getImageDistance (distances[i], id2,"?")
	   << DELIMITER
	
	   << getImageRank (distances[i], id2)
	   << DELIMITER;
    }
  
  fout << inTraining(id1, trlist)
       << endl;
}

/******************************************************************************
*                            MAIN PROGRAM                                     *
******************************************************************************/

int
main (int argc,char** argv)
{
  Arguments args;

  ImageList* imlist;
  ImageList* trlist; /* Training Image Names */
  DistName     distName1;
  DistName     distName2;
  DistanceMap  *distances;
  CovariateMap subjectCovariates;
  CovariateMap imageCovariates;
  Histogram imageHist;
  Histogram compareHist;
  int nImages, i;

  MESSAGE(OPENING);
  MESSAGE(VERSION);
  process_command(argc, argv, &args);

  imlist = getImageNames (args.imageTable, &nImages);
  trlist = getImageNames (args.trainingFile, NULL);

  /* Read in a list of all the images */

  void *nameList = NULL;
  char **nameArray;
  void *subjList = NULL;
  int *subjArray;

  int subjId = 0;

  for EACH_SUBJECT (imlist, subject) {
      for EACH_REPLICATE (subject, replicate) {
	char *subjName = strdup (subject->filename);
	listAccumulate (&nameList, &subjName, sizeof (char *));
	listAccumulate (&subjList, &subjId,   sizeof (int));

	writeProgress ("Reading subjects list", subjId, 0);
      }
      subjId++;
  }

  nameArray = (char **) listToNullTerminatedArray (&nameList, sizeof (char *), NULL);
  subjArray = (int*) listToNullTerminatedArray (&subjList, sizeof (int),    NULL);

  /* Read the covariates file(s) */

  for (i = 0; i < args.nCovariateFiles; i++)
    {
      cout << "Reading covariates from: " << args.covariateFiles[i] << endl;
      readCovariates (args.covariateFiles[i], subjectCovariates, imageCovariates);
    }

  ofstream fout (args.outputFile);

  /* Print out file header
   */
  fout << "ROW_NUMBER"     << DELIMITER 
       << "SUBJECT_NUMBER" << DELIMITER
       << "SUBJECT_ID_1"   << DELIMITER
       << "SUBJECT_ID_2"   << DELIMITER
       << "IMAGE_1_ID"     << DELIMITER
       << "IMAGE_2_ID"     << DELIMITER
       << "IMAGE_1_INDEX"  << DELIMITER
       << "IMAGE_2_INDEX"  << DELIMITER;

  for (CovariateMap::iterator cov = subjectCovariates.begin(); cov != subjectCovariates.end (); cov++)
    fout << "SUBJ_"    << cov->first  << DELIMITER;
  
  for (CovariateMap::iterator cov = imageCovariates.begin(); cov != imageCovariates.end (); cov++)
    {
      fout << "IMAGE_1_" << cov->first << DELIMITER;
      fout << "IMAGE_2_" << cov->first << DELIMITER;
    }

  for (i = 0; i < args.nDistanceDirs; i++)
    {
      DistName distName = removePath (args.distanceDirs[i]);
      fout << distName << DELIMITER
	   << distName << "_RANK" << DELIMITER;
    }

  fout << "FLAG"
       << endl;

  cout << "Writing covariate information file" << endl;

  distances = new DistanceMap[args.nDistanceDirs];

  /* Allocate memory for sorting image by distance */

  int *sortedBySimilarityToProbe = (int*) malloc  (nImages * sizeof (int));
  if (sortedBySimilarityToProbe == 0L)
    {
      fprintf (stderr, "Not enough memory to continue\n");
      exit (1);
    }

  /* Output covariate information file
   */
  int subjectNumber = 0;
  int rowNumber = 0;
  int progress = 0;

  for EACH_SUBJECT (imlist, subject)
    {
      /* We assign a unique number to each relicate
       */
      int n = 0;
      
      for EACH_REPLICATE (subject, replicate) {
	replicate->imageIndex = n++;
      }

      /* We keep a histogram of the number of replicates that
       * are present for any given subject
       */
      imageHist[n]++;

      /* If nRank != 0, then rather than looping through the replicates, we loop through
       * the n closest neighbors */
      if(args.nRank && args.nDistanceDirs)
	{
	  for (i = 0; i < args.nDistanceDirs; i++)
	    distances[i] = readDistances (makePath (args.distanceDirs[i], subject->filename));

	  sortSubjectsBySimilarityToProbe (subject->filename, nameArray, args.distanceDirs[0], sortedBySimilarityToProbe);
	  
	  for (int i = 0; i < args.nRank; i++) {
	    ImageList image2;
	    image2.filename = nameArray[sortedBySimilarityToProbe[i]];
	    image2.imageIndex = i;
	    image2.next_replicate = NULL;
	    image2.next_subject = NULL;
	    
	    printCovariates (fout,
			     rowNumber,
			     subjectNumber,
			     subject, 
			     &image2,
			     subjectCovariates,
			     imageCovariates,
			     distances,
			     args.nDistanceDirs,
			     trlist);
	  }

	  for EACH_REPLICATE (subject, replicate) {
	    progress++;
	  }
	}
      else
      /* Now iterate over all possible pairings of replicates
       */
	{
	  for EACH_REPLICATE (subject, image1)
	    {
	      progress++;
	      
	      for (i = 0; i < args.nDistanceDirs; i++)
		distances[i] = readDistances (makePath (args.distanceDirs[i], image1->filename));
	      
	      for  EACH_REPLICATE (image1, image2)
		{
		  /* Histogram the number of comparisons between the first image index
		   * and the second image index.  This was implemented to verify that
		   * the the distribution of comparisons that was generated by our
		   * statistical software. This code assumes that there are no more than
		   * 10 replicates per subject. The histogram is built such that the first
		   * digit is the first image index and the second digit is the second
		   * image index. This histograms the distributions of image comparisions
		   * such as 1->2, 1->3, 1->4, 2->3, 2->4 ... */
		  
		  compareHist [image1->imageIndex*10+image2->imageIndex]++;
		  
		  printCovariates (fout,
				   rowNumber,
				   subjectNumber,
				   image1, 
				   image2,
				   subjectCovariates,
				   imageCovariates,
				   distances,
				   args.nDistanceDirs,
				   trlist);
		  
		  rowNumber++;
		}
	    }
	}
      
      subjectNumber++;
      writeProgress ("Writing covariate file", progress, nImages);
    }
  
  /* We print out a histogram which the distribution of the
     number of replicates that are present for the subjects
  */

  cout << endl
       << endl
       << "Count of Images"        << endl
       << "# of replicates: count" << endl;
  
  for (Histogram::iterator each = imageHist.begin(); each != imageHist.end (); each++)
    cout << each->first
	 << ": "
	 << each->second << endl;

  /* Print out the distribution of image pairings. For more information
   * see comment above.
   */

  cout << endl
       << endl
       << "Count of comparisons" << endl;

  cout << "Pairing: count" << endl;

  for (Histogram::iterator each = compareHist.begin (); each != compareHist.end (); each++)
    cout << each->first
	 << ": "
	 << each->second << endl;

  return 0;
}
