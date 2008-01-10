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
$RCSfile: csuEBGMMasks.c,v $
$Author: teixeira $
$Date: 2003/04/23 14:02:31 $
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

#include <csuCommon.h>
#include <csuEBGMUtil.h>


typedef struct {
    char* maskFile;
    char* outputDir;
}
Arguments;

/*
 * usage
 *
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void usage(const char* name){
    printf("Usage: %s mask_file outputdir\n",name);
    exit(1);
}


void processCommand(int argc, char** argv, Arguments* args) {
    int i;
    int param_num = 0;

    debuglevel = 0;

    /******* Read command line arguments *******/

    for (i = 1;i < argc;i++) {

        /* Catch common help requests */
        if      (readOption      (argc, argv, &i, "-help" )) { usage(argv[0]); }
        else if (readOption      (argc, argv, &i, "--help")) { usage(argv[0]); }

        /* check if the current argument is an unparsed option */
        else if (checkBadOption(argc,argv,&i)) {}

        /* read required arguments */
        else if (param_num == 0) {
            args->maskFile = argv[i];
            param_num++;
        }
        else if (param_num == 1) {
            args->outputDir = argv[i];
            param_num++;
        }
        else{ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    }

    /* make sure that there are the proper number of required arguments */
    if (param_num != 2){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    /* Print out the program parameters for appropreate debug level */

    DEBUG(1,"Debuging enabled");
}

void outputMasks(Arguments* args){
    JetMasks masks;
    int y;

    MESSAGE("Creating gabor masks.");
    masks = readMasksFile(args->maskFile);

    MESSAGE("Saving Masks to files");
    for(y = 0; y < masks->size; y++){
        char outname[MAX_FILENAME_LENGTH];
        sprintf(outname, "%s/mask%03d.pgm", args->outputDir,y);
        writePGMImage(masks->masks[y],outname,0);
    }



}

int main(int argc, char** argv){
    Arguments args;

    processCommand(argc, argv, &args);

    outputMasks(&args);

    return 0;
}



