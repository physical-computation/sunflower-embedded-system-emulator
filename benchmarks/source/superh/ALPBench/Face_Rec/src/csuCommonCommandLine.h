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

#ifndef CSU_COMMAND_LINE_INCLUDED
#define CSU_COMMAND_LINE_INCLUDED

#define CL_YES_STRING "YES"
#define CL_NO_STRING  "NO"

#define CL_YES 1
#define CL_NO  0

/*
 *  These functions are used to parse basic command line options
 *  argc and argv are basic command line parameters
 *  i is the current scanning location in the command line
 *  opt is a string giving the option to look for in the command line
 *  additional parameters are option parameters that follow the
 *  option string.
 *
 *  Optional arguments should always be parsed before required arguments.
 *
 *  The functions are maid to be called within a four loop that inc i.
 *  If more that one argument is parsed i is updated to point at the last
 *  argument read to allow exicution to proceed normally on next loop.
 *
 *  Here is an example code illistrating how to parse command lines
 *  with this api:
 *
 *  int i; int num = 0;
 *
 *  Set up default values here.
 *
 *  for(i = 0; i < argc; i++){
 *      if      (readOption(argc, argv, &i, "-verbose"))                   { verbose = 1; }
 *      else if (readOptionInt(argc, argv, &i, "-debuglevel",&debuglevel)) {}
 *      else if (checkBadOption(argc, argv, &i))                           {}
 *      else if (readReqiredString(argc, argv, &i, &num, 0, filename))     {}
 *  }
 *
 *  then check to see if the correct number of required arguments were parsed
 *  if(num != 1){ clParseError(argc, argv, i, "Wrong number of command line arguments."); }
 *
 *  This code also expects that you have a useage function define that takes argv[0]
 *  as its only arguments and exits with value 1.
 */

/* prototype to use for an external usage statement */
void usage(const char* );

/* returns true if opt is parsed and i+=0 */
int readOption(int argc, char** argv, int *i, const char* opt);

/* returns true if opt is parsed and sets arg to point to location in argv that is after flag (the next string) i+=1 */
int readOptionString(int argc, char** argv, int *i, const char* opt, char** arg);

/* returns true if opt is parsed and sets arg to point to location in argv that is after flag (the next string) i+=1 */
int readOptionYesNo(int argc, char** argv, int *i, const char* opt, int* flag);

/* returns true only if opt is parsed and following argument string matches arg
 * This is used to test for options like -hist PRE -hist POST -hist NONE i+=1*/
int readOptionMatch(int argc, char** argv, int *i, const char* opt, const char* arg);

/* this reads in an option and an int. i+=1 */
int readOptionInt(int argc, char** argv, int *i, const char* opt, int *arg);

/* this reads in an option and two ints. i+=2 */
int readOptionInt2(int argc, char** argv, int *i, const char* opt, int *arg1, int *arg2);

/* this reads in an option and a double. i+=1 */
int readOptionDouble(int argc, char** argv, int *i, const char* opt, double *arg);

/* this reads in an option and two doubles. i+=2 */
int readOptionDouble2(int argc, char** argv, int *i, const char* opt, double *arg1, double *arg2);

/* this reads in an option and four doubles. i+=4 */
int readOptionDouble4(int argc, char** argv, int *i, const char* opt, double *arg1, double *arg2, double *arg3, double *arg4);

/* Reads in a required argument, only if num == arg_num i += 0*/
int readRequiredString(int argc, char** argv, int *i, int *num, int arg_num, char** arg);

/* Reads in a required argument, only if num == arg_num i += 0*/
int readRequiredInt(int argc, char** argv, int *i, int *num, int arg_num, int* arg);

/* check to see if current argument starts with a dash and if it does output an error and exit. otherwize return 0 */
int checkBadOption(int argc, char** argv, int *i);

/* Output a command line parsing error */
void clParseError(int argc, char **argv, int i, char* message);

#endif




