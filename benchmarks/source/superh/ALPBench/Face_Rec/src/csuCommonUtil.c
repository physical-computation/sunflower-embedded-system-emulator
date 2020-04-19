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
File: csuCommonUtil.c
Authors: J. Ross Beveridge, David Bolme, Kai She, Marcio Teixeira                         
Date:    June 11, 2002                                                     
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
 Purpose: This file contains common utilities
*/


/******************************************************************************
*                               INCLUDES                                      *
******************************************************************************/

#include "csuCommon.h"
#include <ctype.h>


/* This function computes the area under the standard normal curve
   from -infinity to a.  This can be used to compute statistical
   significance of a test.  This basically is used like a z curve
   lookup table.
*/
double probNormal(double a){
    int i; double j; double sum; double term;

    j = 1;
    sum = 0;

    for(i = 1; i < 200; i += 2){
        j = j*i;
        term = pow(a,i)/j;
        sum = sum + term;
    }

    return sum*exp(-pow(a,2)/2)/sqrt(2*PI)+0.5;
}

/*
 Shuffle returns an array with integers between 0 and size-1 where their
 order is randomized but there are no duplicates */
int* shuffle(int size) {
    int *vec;
    int *temp;    /* Temporary list of integers to be randomly inserted into vec */
    int bound;    /* The upper bound on indices in temp still to consider */
    int place;    /* Position in temp randomly chosen between zero and bound - 1 */
    int i, j;
    vec = (int*) malloc(sizeof(int) * size);
    assert(vec);
    temp = (int*) malloc(sizeof(int) * size);
    assert(temp);
    for (i = 0; i < size; i++) {
        temp[i] = i;
    }
    /* Starting with bound equal to size, pick a random position in
     temp, copy that value into the next position of the result
     vector, then copy all values above that place in temp down one
     position. This effectively removes the value copied into vector
     from further consideration. In other words, there will be not
     duplicate integers in the resulting vector */
    bound = size;
    for (i = 0; i < size; i++) {

        /* the next line was replaced by DSB to use high order bits - man 3 rand */
        /* place = (int) ((rand() % ((long) bound))); */
        place = (int) (((double)bound)*rand()/(RAND_MAX+1.0));
        
        vec[i] = temp[place];
        for (j = place; j < bound - 1; j++) {
            temp[j] = temp[j + 1];
        }
        bound = bound - 1;
    }
    free(temp);
    return vec;
}

void* csu_allocate(const char* type, int sof, int num, const char* file, int line){
    void* tmp = (void*) malloc(sof*num);
    if(tmp == NULL){
        printf("ERROR: failed to allocate memory.\n"
               "       type: %s - number: %d\n"
               "       in <%s:%d>\n", type, num, file, line);
        exit(1);
    }
    return tmp;
}

/******************************************************************************
*                               TOKENIZER                                     *
******************************************************************************/

/**
 * Initializes an instance of the tokenizer. The 'source' argument is a
 * pointer to a function which returns data. Currently two functions are
 * provided:
 *
 * @param tokenizerStringReader Obtains characters from a null terminated
 *                              string specified by 'arg'
 * @param tokenizerStreamReader Obtains characters from a FILE* specified
 *                              by 'arg'
 */
void
tokenizerInit (Tokenizer * tok, TokenizerDataSourceProc source, void *arg)
{
  tok->i_ptr = tok->in_buff;
  tok->endOfLine = 0;
  tok->endOfFile = 0;
  tok->state = 0;
  tok->availChars = 0;
  tok->dataSource = source;
  tok->dataSrcArg = arg;
}

/**
 * Data source procedure for reading data from a file. This function is
 * provided as an argument to 'tokenizerInit'.
 */
int
tokenizerStreamReader (char *buff, size_t nChars, void **arg)
{
  return fread (buff, sizeof (char), nChars, (FILE *) * arg);
}

/**
 * Data source function for reading data from a null terminated string.
 * This function is provided as an argument to 'tokenizerInit'.
 */
int
tokenizerStringReader (char *buff, size_t nChars, void **arg)
{
  char *c = (char*)*arg;
  int n;

  for (n = 0; (n < nChars) && (*c != '\0'); n++)
    *buff++ = *c++;

  *arg = c;
  return n;
}

/**
 * Reads a single word from the stream. The pointer returned by this
 * function is guaranteed to be valid only until the next invocation
 * of 'tokenizerGetWord'.
 *
 * The tokenizer has the following properties:
 *
 *     - Blank lines are ignored
 *     - Whitespace before or after words are ignored
 *     - Comments can be introduced by a '#' sign
 *     - 'tokenizerEndOfLine' can be used to test whether the word
 *       was the last word of a line
 *     - 'tokenizerEndOfFile' is guaranteed to return true if there
 *       are no other words in the file and the next call to
 *       'tokenizerGetWord' would fail
 */
char *
tokenizerGetWord (Tokenizer * tok)
{
  char c;
  int charClass, emitWord, saveChar;

  /* The tokenizer is implemented as a seven state state machine. State
   * zero is the inital state, state six is the exit state. The states
   * are roughly described as follows:
   *
   *    0 - Inital state: Eat whitespace
   *    1 - Eat comments #1
   *    2 - Save word characters
   *    3 - Eat whitespace
   *    4 - Eat whitespace and newlines
   *    5 - Eat comments #2
   *    6 - End-of-file reached
   *
   * States 1 and 5 are subtely different and represent knowledge about
   * whether there is a word in the word buffer which is waiting to be
   * returned. In general, the state machine can only return a word after
   * it has examined all the characters preceeding the starting character
   * of the next word. This is because the tokenizer must properly
   * indicate that a word is the last valid word in the stream even if
   * that word happens to be followed by several empty lines and/or
   * comments. The need to defer this decision accounts for much of the
   * complexity of the state machine.
   *
   * The rows of the state transition table represent states while the
   * columns corresponds to one of five character classes:
   *
   *    0 - Eof reached        (not a character)
   *    1 - whitespace         (' '  or '\t')
   *    2 - newlines           ('\n' or '\r')
   *    3 - Comment initiator  ('#')
   *    4 - Word char          (everything else)
   */

  static int stateTransitionTable[7][5] = {
    {6, 0, 0, 1, 2},	/* State 0 */
    {6, 1, 0, 1, 1},		/* State 1 */
    {6, 3, 4, 5, 2},		/* State 2 */
    {6, 3, 4, 5, 2},		/* State 3 */
    {6, 4, 4, 5, 2},		/* State 4 */
    {6, 5, 4, 5, 5},		/* State 5 */
    {6, 6, 6, 6, 6}		/* State 6 */
  };

  /* Each state transition may be associated with one or more actions,
   * defined by these bits:
   *
   *   0x1 = CPY: Copy the current char to the word buffer
   *   0x2 = RET: Exit from loop and return contents of word buffer
   *   0x4 = EOF: Assert end-of-file flag
   *   0x8 = EOL: Assert end-of-line flag
   *
   * These actions may be combined:
   *
   *   0x3 = CPY + RET
   *   0x6 = RET + EOF
   *   0xB = CPY + RET + EOL
   *   0xE = RET + EOF + EOL
   */

  static int stateActionTable[7][5] = {
    {0x6, 0x0, 0x0, 0x0, 0x1},	/* State 0 */
    {0x6, 0x0, 0x0, 0x0, 0x0},	/* State 1 */
    {0xE, 0x0, 0x0, 0x0, 0x1},	/* State 2 */
    {0xE, 0x0, 0x0, 0x0, 0x3},	/* State 3 */
    {0xE, 0x0, 0x0, 0x0, 0xB},	/* State 4 */
    {0xE, 0x0, 0x0, 0x0, 0x0},	/* State 5 */
    {0x6, 0x6, 0x6, 0x6, 0x6}	/* State 6 */
  };

  /* Prepare to read new word into word_buff */

  tok->w_ptr = tok->word_buff;

  /* Begin processing characters according to
   * the transition rules */

  while (1)
    {
      /* Read in a new chunk of data, if needed */

      if (tok->availChars == 0)
	{
	  tok->availChars =
	    (*tok->dataSource) (tok->in_buff, NELEM (tok->in_buff),
				&tok->dataSrcArg);
	  tok->i_ptr = tok->in_buff;
	}

      /* Look at the current character and find its character class */

      c = *tok->i_ptr;

      if (tok->availChars == 0)
	charClass = 0;
      else if (c == ' ' || c == '\t')
	charClass = 1;
      else if (c == '\n' || c == '\r')
	charClass = 2;
      else if (c == '#')
	charClass = 3;
      else
	charClass = 4;

      /* Transition to next state based on the character on input */

      saveChar = (stateActionTable[tok->state][charClass] & 0x01);
      emitWord = (stateActionTable[tok->state][charClass] & 0x02);
      tok->endOfFile = (stateActionTable[tok->state][charClass] & 0x04);
      tok->endOfLine = (stateActionTable[tok->state][charClass] & 0x08);
      tok->state = stateTransitionTable[tok->state][charClass];

      /* If we transitioned to an accepting state, break out
       * of the loop so that we can return the token to the caller
       */

      if (emitWord)
	break;

      /* If the transition requires it, copy the character into the
       * word buffer */

      if (saveChar)
	*tok->w_ptr++ = c;

      /* Advance to the next character in the stream */

      tok->i_ptr++;
      tok->availChars--;
    }

  /* Add a NULL terminator, then return the contents of the word
   * buffer to the caller */

  *tok->w_ptr = '\0';
  return tok->word_buff;
}

/* Queries whether the last call to 'tokenizerGetWord' returned the last
 * valid word of the file or stream */
int
tokenizerEndOfFile (Tokenizer * tok)
{
  return tok->endOfFile;
}

/**
 * Queries whether the last call to tokenizerGetWord returned the last
 * word on a line (the last word of the file is always said to be on the
 * end of the line, whether or not that line is followed by a newline) */
int
tokenizerEndOfLine (Tokenizer * tok)
{
  return tok->endOfLine;
}

/**
 * This function tests some of the key functionality of the tokenizer.
 * It aborts the program is something goes awry, but has no side
 * effects if everything checks out */

void
tokenizerTestSuite (Tokenizer * tok)
{
    char *msg = "Tokenizer test failed";
    char* test = "Hello, World!";

    tokenizerInit (tok, tokenizerStringReader, test);
    DEBUG_CHECK (!strcmp ("Hello,", tokenizerGetWord (tok)), msg);
    DEBUG_CHECK (!tokenizerEndOfLine (tok), msg);
    DEBUG_CHECK (!tokenizerEndOfFile (tok), msg);
    DEBUG_CHECK (!strcmp ("World!", tokenizerGetWord (tok)), msg);
    DEBUG_CHECK (tokenizerEndOfLine (tok), msg);
    DEBUG_CHECK (tokenizerEndOfFile (tok), msg);

    test = "\n\nYogi # Bear ";
    tokenizerInit (tok, tokenizerStringReader, test);
    DEBUG_CHECK (!strcmp ("Yogi", tokenizerGetWord (tok)), msg);
    DEBUG_CHECK (tokenizerEndOfLine (tok), msg);
    DEBUG_CHECK (tokenizerEndOfFile (tok), msg);

    test = "";
    tokenizerInit (tok, tokenizerStringReader, test);
    DEBUG_CHECK (!strcmp ("", tokenizerGetWord (tok)), msg);
    DEBUG_CHECK (!tokenizerEndOfLine (tok), msg);
    DEBUG_CHECK (tokenizerEndOfFile (tok), msg);

    test = "\n  \na\tb##b\n#d";
    tokenizerInit (tok, tokenizerStringReader, test);
    DEBUG_CHECK (!strcmp ("a", tokenizerGetWord (tok)), msg);
    DEBUG_CHECK (!tokenizerEndOfLine (tok), msg);
    DEBUG_CHECK (!tokenizerEndOfFile (tok), msg);
    DEBUG_CHECK (!strcmp ("b", tokenizerGetWord (tok)), msg);
    DEBUG_CHECK (tokenizerEndOfLine (tok), msg);
    DEBUG_CHECK (tokenizerEndOfFile (tok), msg);
}

/******************************************************************************
*                               LIST FUNCTIONS                                *
******************************************************************************/

/** Accumulates data into a list.
 *  This function accumulates data into a dynamic data structure.
 *  Once all data has been read, it can efficiently be converted
 *  into an an appropriately sized array by the function
 *  listToArray.
 * 
 *  The first argument is a pointer to a pointer that keeps track
 *  of the first element in the list. This argument ought to be
 *  a pointer to a void pointer initialized to NULL. The second
 *  argument is a pointer to data to insert into the list and the
 *  third argument is the size of that data.
*/

void
listAccumulate (void **base, void *data, size_t size)
{
  typedef struct linkedListNode {
    struct linkedListNode* link;
    char *data;
  } *LinkedListNode;
  
  LinkedListNode node = (LinkedListNode)
    malloc(sizeof(LinkedListNode) + size);
  memcpy (&node->data, data, size);
  node->link = (LinkedListNode) *base;
  *base = node;
}

/**
 * This function packages data that was accumulated by
 * listAccumulate into an array. The first two arguments
 * ought to match the ones passed to listAccumulate. The
 * last argument points to a value where the number of
 * elements will be recorded (this argument may be NULL). 
 */
void *
listToArray (void **base, size_t size, size_t *nelems)
{
  typedef struct linkedListNode {
    struct linkedListNode* link;
	char *data;
  } *LinkedListNode;

  int nItems;
  LinkedListNode node, nextNode;
  void *array, *a_dest;

  /* Count up the number of elements in the list */

  nItems = 0;
  node = (LinkedListNode) *base;
  while(node)
    {
      nItems++;
      node = node->link;
    }

  /* Allocate an array big enough to store the data */

  array = malloc( size * nItems );
  
  /* Copy the data over into the array, freeing the list
     nodes as we go along */

  node = (LinkedListNode) * base;
  a_dest = (void*) ((char *) array + (size * nItems));
  while(node)
    {
      a_dest = (void *) ( (char*) a_dest - size);
      memcpy (a_dest, &node->data, size);
      nextNode = node->link;
      free (node);
      node = nextNode;
    }

  /* Return nelems and result */

  if (nelems != NULL)
    *nelems = nItems;

  return array;
}

/**
 * Same as listToArray, but packs an extra NULL pointer to the end of the
 * array. This is useful for constructing NULL-terminated lists of pointers
 * (such as ListOfStrings).
 */
void *
listToNullTerminatedArray (void **base, size_t size, size_t *nelems)
{
  void *res;

  /* Add NULL-termination */

  char *s = NULL;
  listAccumulate (base, &s, size);
  
  res = listToArray (base, size, nelems);
  if(nelems != NULL)
    (*nelems)--; /* Don't count NULL-termination */

  return res;
}

/******************************************************************************
*                               PERMUTATION FUNCTIONS                         *
******************************************************************************/

/**
 * A data structure for associating a pair of integers.
 *
 * This structure is used internally by "shuffledNumbers"
 */
typedef struct {
  long key;
  long index;
} Pair;

/**
 * A qsort helper function for sorting Pairs.
 *
 * This function is used internally by "shuffledNumbers"
 *
 * @param a The first object in the pair to compare
 * @param b The second object in the pair to compare
 */

int
pairComparator (const void *a, const void *b)
{
  const Pair *p1= (Pair *)a;
  const Pair *p2 = (Pair *)b;

  if (p1->key < p2->key)
    return -1;
  if (p1->key > p2->key)
    return 1;
  return 0;
}

/**
 * Return a shuffled list of number from 0 to max.
 */
int *shuffledNumbers (int max)
{
  int *indices, i;
  Pair *toSort = (Pair*) malloc (max * sizeof (Pair));

  for (i = 0; i < max; i++)
    {
      toSort[i].key   = random();
      toSort[i].index = i;
    }

  qsort (toSort, max, sizeof (Pair), pairComparator);

  indices = (int*) malloc (max * sizeof (int));

  for (i = 0; i < max; i++) {
    indices[i] = toSort[i].index;
  } 

  free (toSort);

  return indices;
}

/******************************************************************************
*                               PROGRESS INDICATOR                            *
******************************************************************************/

/**
 * Writes out a progress indicator to the screen.
 *
 * @param message A message to display on the screen
 * @param value   A value which is used to compute the percent
 * @param max     The maximum value which value can reach, or zero if unknown
 */
void
writeProgress (char *what, int value, int max)
{
  char *symbols = "\\|/-";
  static char *sym = "";

  if (*sym == '\0')
    sym = symbols;

  if (max)
    fprintf (stdout, "%s: %c Processed: %d (%d%%)   \r", what, *sym++, value, 100 * value / max);
  else
    fprintf (stdout, "%s: %c Processed: %d          \r", what, *sym++, value);

  fflush (stdout);
}


/* checks to see if the machine is Little Endian
*  this is used to determine if byte reversal is
*  needed when reading or writing data files in
*  sun byte order.
*/
int isMachineLittleEndian(){
    static int flag = -1;
    if(flag == -1){
        char magic[4] = {0x01, 0x02, 0x03, 0x04};
        unsigned long *longMagic = (unsigned long *) magic;
        if (*longMagic == 0x01020304) {
            flag = 0;
        } else if (*longMagic == 0x04030201) {
            flag = 1;
        } else {
            fprintf(stderr, "Funky Byte Order, I give Up!!\n");
            exit(1);
        }
    }
    return flag;
}

void byteswap_4(void *data, const int numfourbyteelements)
{
    int i;
    int *d4 = (int *)data;
    unsigned char *d1 = (unsigned char *)data;

    if (!(isMachineLittleEndian()))	/* i.e. BIG_ENDIAN */
    {
        for (i = 0 ; i < numfourbyteelements ; i++, d1 += 4 )
            d4[i] = ((int)d1[3] << 24) | ((int)d1[2] << 16) |
                ((int)d1[1] <<  8) |  (int)d1[0];
    }
    else				/* i.e. LITTLE_ENDIAN */
    {
        for (i = 0 ; i < numfourbyteelements ; i++, d1 += 4 )
            d4[i] = ((int)d1[0] << 24) | ((int)d1[1] << 16) |
                ((int)d1[2] <<  8) |  (int)d1[3];
    }
}

/*
 *make a stand alone concatenation of s1+s2
 */
char *strconc(const char *s1, const char *s2)
{
    char *result = (char*) malloc (sizeof(char)*(strlen(s1)+strlen(s2)+1));
    DEBUG_CHECK (result, "malloc failed");

    strcpy (result, s1);
    strcat (result, s2);
    return result;
}

char *strclone(const char *si)
{
    char *so = strdup (si);
    DEBUG_CHECK (so, "malloc failed");
    return so;
}

/*
 * make all the characters of the input string lower case, if applicable
 * changes the input string
 */
char *strlower(char *x)
{
    char *result = x ;
    for (; (*x = (char)tolower((int)*x)); x++);
    return result;
}

/*
 * make all the characters of the input string lower case, if applicable
 * changes the input string
 */
char *strupper(char *x)
{
    char *result = x ;
    for (; (*x = (char)toupper((int)*x)) ; x++);
    return result;
}

/*
 * take two strings: strip the final extension from the first
 * if any then add the second to the first.
 * note the first string's address is altered, it's memory is
 * freed and it points to newly allocated memory afterwards.
 */
char *newextlong(char **filename, const char *extension)
{
    if (extension)
    {
        char *result = 0;
        char *finalperiod = strrchr(*filename, '.');

        if (!finalperiod)	 /* add an extension where there wasn't one */
        {
            char *periodized = strconc(".", extension);
            result = strconc(*filename, periodized);
            free(periodized);
        }
        else
        {				/* replace the existing extension */
            finalperiod[1] = '\0';	/* NULL terminate the string */
            result = strconc(*filename, extension);
        }

        free(*filename);
        *filename = result;
    }
return *filename;
}

/******************************************************************************
*                               MATHEMATICS FUNCTIONS                         *
******************************************************************************/


/*********************** Debug Information **********************/
int debuglevel = 0;
int quiet = 0;

