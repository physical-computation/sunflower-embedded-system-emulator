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

#include <csuCommon.h>

#define OPENING  "csuToolsSFI2PGM convert SFI images to PGM format."

void usage(const char* name){
    printf("Usage: %s [-m] sfifiles\n\n",name);
    printf( "    This program converts sfi images to pgm format.\n\n"
            "    If the first argument is -m then the program runs in multichannel mode\n"
            "    In default mode the first channel of the sfi image is saved to a file.\n"
            "    In multichannel mode every channel of the image is saved to its own pgm image.\n"
            "    The pgm image will have a channel number appended to its file name \"*_XXX.pgm\".\n");
    exit(1);
}

void removeExtention(char* filename){
    int i;

    for(i = strlen(filename); i >= 0; i--){
        if(filename[i] == '.'){
            filename[i] = 0;
            break;
        }
        if(filename[i] == '/'){
            break;
        }
    }
}

int main(int argc, char** argv){
    Image im;
    char outfilename[MAX_FILENAME_LENGTH];
    int c;
    int multichannel = 0;
    int i = 1;

    printf("%s\n%s\n",OPENING,VERSION);
    if(argc == 1 || strcmp(argv[1],"-help") == 0 || strcmp(argv[1],"--help") == 0){
        usage(argv[0]);
    }

    if(strcmp(argv[1],"-m") == 0){ multichannel = 1; i++; }

    for(;i < argc; i++){
        printf("Converting: %s\n",argv[i]);
        im = readRawImage(argv[i]);
        
        removeExtention(argv[i]);

        if(multichannel){
            for(c = 0; c < im->channels; c++){
                sprintf(outfilename,"%s_%03d.pgm",argv[i],c);
                printf("    Writing: %s\n",outfilename);
                writePGMImage(im,outfilename,c);
            }
        }
        else{
            sprintf(outfilename,"%s.pgm",argv[i]);
            printf("    Writing: %s\n",outfilename);
            writePGMImage(im,outfilename,0);
        }
    }
    printf("done.\n");

    return 0;
}


