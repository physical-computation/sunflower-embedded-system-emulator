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
 *  csuUtilities.h
 *  GaborGUI
 *
 *  Created by David  Bolme on Sun Jun 09 2002.
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

#ifndef CSU_COMMON_UTIL_INCLUDED
#define CSU_COMMON_UTIL_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 *  This file was created to contian simple commonly used macros
 *  and functions so that they don't have to be rewritten at every
 *  use.
 */ 

/******************************************************************************
*                           GLOBAL VARIABLES AND CONSTANTS                    *
******************************************************************************/

#include <assert.h>

/* The csu codes are setup to use double floats */
#define FTYPE double

/* First guess at the line length for an image list file */
#define FILE_LINE_LENGTH 512

/* Max length of a filename */
#define MAX_FILENAME_LENGTH 2048

/******************************************************************************
*                           NUMERICAL MACROS                                  *
******************************************************************************/

/*************** Commonly used numerical macros ***************/

#define MIN(v1, v2) 		( ((v1) > (v2)) ? (v2) : (v1) )
#define MAX(v1, v2) 		( ((v1) < (v2)) ? (v2) : (v1) )
#define ABS(v)      		( ((v) < 0)     ? -(v) : (v)  )
#define SQR(v)      		( (v) * (v) )
#define ROUND(v)    		( (int) ((v)+0.5) )
#define TRUNC(v)    		( (int) (v) )
#define EQUAL_ZERO(v,tol)  	( ABS(v) < tol )
#define DIST_2D(x1, y1, x2, y2) (sqrt(SQR((x1)-(x2))+SQR((y1)-(y2)))) 
#define FINITE(n) if(!finite(n)) printf("ERROR: Not Finite - %10s:%5f <%10s,%10s,%3d>\n",#n,n,__FILE__,__FUNCTION__,__LINE__);

#define PI          3.141592654
    /* Random number between 0 and 1 */
#define RANDOM   (fabs(((double)random())/RAND_MAX))

/* This function computes the area under the standard normal curve
   from -infinity to a.  This can be used to compute statistical
   significance of a test.  This basically is used like a z curve
   lookup table.
*/
double probNormal(double a);

/*
 Shuffle returns an array with integers between 0 and size-1 where their
 order is randomized but there are no duplicates */
int* shuffle(int size);

/*************** Commonly used utility macros ****************/

/* This macro will allocate space for an N legth array of type */
/* to allocate just one do ALLOCATE(type,1) */
#define ALLOCATE( type , number ) ( (type*)csu_allocate(#type, sizeof(type),(number), __FILE__, __LINE__) )
void* csu_allocate(const char*, int sof, int num, const char* file, int line);

    /* Number of elements in an array */
#define NELEM(v) (sizeof(v)/sizeof(v[0]))

/*************** Commonly used string macros *****************/
/*#define STRING_TO_DOUBLE(s, d) {                               \
    char* test;                                                \
    (d) = strtod((s),&test);                                   \
    if(test == (s)){                                           \
        printf("Error converting string to double: %s",(s));   \
        exit(1);                                               \
    }                                                         \
}*/

/*#define STRING_TO_INT(s, i) {                                 \
    char* test;                                               \
    (i) = strtod((s),&test);                                  \
    if(test == (s)){                                          \
        printf("Error converting string to int: %s",(s));     \
        exit(1);                                              \
    }                                                         \
}*/

/******************************************************************************
*                           TOKENIZER OBJECT                                  *
******************************************************************************/

typedef int (*TokenizerDataSourceProc) (char *buff, size_t nChars,
					void **arg);

typedef struct
{
  int state;
  char in_buff[512];
  char *i_ptr;
  char word_buff[100];
  char *w_ptr;
  int endOfLine;
  int endOfFile;
  int availChars;
  TokenizerDataSourceProc dataSource;
  void *dataSrcArg;
}
Tokenizer;

/* Tokenizer methods */


void tokenizerInit (Tokenizer * tok, TokenizerDataSourceProc source, void *arg);
char *tokenizerGetWord (Tokenizer * tok);
int tokenizerEndOfFile (Tokenizer * tok);
int tokenizerEndOfLine (Tokenizer * tok);
void tokenizerTestSuite (Tokenizer * tok);

/* The following two functions aren't to be used directly. They are used
 * as the "source" parameter to "tokenizerInit" */

int tokenizerStreamReader (char *buff, size_t nChars, void **arg);
int tokenizerStringReader (char *buff, size_t nChars, void **arg);

/******************************************************************************
*                           UTILITY FUNCTIONS                                 *
******************************************************************************/

/* List functions */

void listAccumulate (void **base, void *data, size_t size);
void * listToArray (void **base, size_t size, size_t *nelems);
void * listToNullTerminatedArray (void **base, size_t size, size_t *nelems);

/* Permutation functions */

int *shuffledNumbers (int max);

/* Progress indicator */

void writeProgress (char *what, int value, int max);

/* Used in several places to see how double floats represented on this machine */
int isMachineLittleEndian();

void byteswap_4(void *data, const int numfourbyteelements);

/* Mathematics functions */

/* String functions */

char *strconc(const char *s1, const char *s2);
char *strclone(const char *si);
char *strlower(char *x);
char *strupper(char *x);
char *newextlong(char **filename, const char *extension);

/******************************************************************************
*                            DEBUGGING ROUTINES                               *
******************************************************************************/

#include <time.h>
#include <stdio.h>
extern int debuglevel;
extern int quiet;

#define MESSAGE( message ) \
    { \
        time_t tttt = time(0); \
        if(quiet == 0 || debuglevel != 0) printf(message); \
        if(quiet == 0 || debuglevel != 0) printf("\n"); \
        if(debuglevel > 0) printf("    [%s]<%s():line %u> %s\n", __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt)); \
        fflush(stdout); \
    }

#define MESSAGE1ARG( message, arg1 ) \
    { \
        time_t tttt = time(0); \
        if(quiet == 0 || debuglevel != 0) printf(message, arg1); \
        if(quiet == 0 || debuglevel != 0) printf("\n"); \
        if(debuglevel > 0) printf("    [%s]<%s():line %u> %s\n", __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt)); \
        fflush(stdout); \
    }

#define MESSAGE2ARG( message, arg1, arg2 ) \
    { \
        time_t tttt = time(0); \
        if(quiet == 0 || debuglevel != 0) printf(message, arg1, arg2); \
        if(quiet == 0 || debuglevel != 0) printf("\n"); \
        if(debuglevel > 0) printf("    [%s]<%s():line %u> %s\n", __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt)); \
        fflush(stdout); \
    }


#define DEBUG( level , message ) \
    if( level == -1 || level <= debuglevel ){ \
        time_t tttt = time(0); \
        printf( "**DEBUG %d** \"%s\"\n             at [%s]<%s():line %u>\n             %s\n", level, message , __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt)); \
        fflush(stdout); \
    }

#define DEBUG_FLOAT( level , message , value) \
    if( level == -1 || level <= debuglevel ){\
        time_t tttt = time(0); \
        printf( "**DEBUG %d** \"%s\" %s=%f\n             at [%s]<%s():line %u>\n             %s\n", level, message, #value , value , __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt)); \
        fflush(stdout);\
    }

#define DEBUG_DOUBLE( level , message , value) \
    if( level == -1 || level <= debuglevel ){\
        time_t tttt = time(0); \
        printf( "**DEBUG %d** \"%s\" %s=%f\n             at [%s]<%s():line %u>\n             %s\n", level, message, #value , value , __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt)); \
        fflush(stdout); \
    }

#define DEBUG_INT( level , message , value) \
    if( level == -1 || level <= debuglevel ){\
        time_t tttt = time(0); \
        printf( "**DEBUG %d** \"%s\" %s=%d\n             at [%s]<%s():line %u>\n             %s\n", level, message, #value , value , __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt));\
        fflush(stdout); \
    }

#define DEBUG_STRING( level , message , value) \
    if( level == -1 || level <= debuglevel ){\
        time_t tttt = time(0); \
        printf( "**DEBUG %d** \"%s\" %s=\"%s\"\n             at [%s]<%s():line %u>\n             %s\n", level, message , #value,value , __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt));\
        fflush(stdout);\
    }

#define DEBUG_CHECK( condition , message) \
if( !(condition) ){\
    time_t tttt = time(0); \
        printf( "**DEBUG CHECK** Failed check (%s) \"%s\"\n             at [%s]<%s():line %u>\n             %s\n", #condition, message , __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt));\
            fflush(stdout); \
                exit(1);\
}

#define DEBUG_CHECK_1ARG( condition , message, arg) \
if( !(condition) ){\
    time_t tttt = time(0); \
        printf( "**DEBUG CHECK** Failed check (%s) \"",#condition);\
            printf( message, arg);\
                printf("\"\n             at [%s]<%s():line %u>\n             %s\n", __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt));\
                    fflush(stdout); \
                        exit(1);\
}

#define DEBUG_CHECK_2ARG( condition , message, arg1, arg2 ) \
if( !(condition) ){\
    time_t tttt = time(0); \
    printf( "**DEBUG CHECK** Failed check (%s) \"",#condition);\
    printf( message, arg1, arg2);\
    printf("\"\n             at [%s]<%s():line %u>\n             %s\n", __FILE__ , __FUNCTION__ , __LINE__, ctime(&tttt));\
    fflush(stdout); \
    exit(1);\
}

#define NOT_IMPLEMENTED  DEBUG_CHECK(0,"This function has not yet been implemented");


#endif /*CSU_COMMON_UTIL_INCLUDED*/
