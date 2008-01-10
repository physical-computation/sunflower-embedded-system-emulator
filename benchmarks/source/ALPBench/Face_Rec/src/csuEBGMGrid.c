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
$RCSfile: csuEBGMGrid.c,v $
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


#define  GRID_START_X 4
#define  GRID_START_Y 9
#define  GRID_SPACE_X 5
#define  GRID_SPACE_Y 5

typedef struct {
    char* inputDir;
    char* sfiDir;
    char* pgmDir;

    char* imageList;

    char* maskFile;

    int saveMasks;

    int gridStartX;
    int gridStartY;
    int gridSpaceX;
    int gridSpaceY;
}
Arguments;

/*
 * usage
 *
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void usage(const char* name){
    printf("Usage: %s [OPTIONS] input_dir imagelist output_dir mask_file\n",name);
    printf("  Computes Gabor Jets at a regular interval and outputs the values in sfi format\n");
    printf("  OPTIONS:\n");
    printf("    -gridStart x y          - Choose start location for the grid.\n");
    printf("                              Defaults to %d, %d \n", GRID_START_X, GRID_START_Y);
    printf("    -gridSpace x y          - Choose x and y spacing for the grid.\n");
    printf("                              Defaults to %d, %d \n", GRID_SPACE_X, GRID_SPACE_Y);
    printf("  OTHER OPTIONS:\n");
    printf("    -quiet                  - Turns off all messages.\n");
    printf("    -debuglevel <int>       - Turns turns on debuging output.\n");
    exit(1);
}


void processCommand(int argc, char** argv, Arguments* args) {
    int i;
    int param_num = 0;

    args->imageList      = NULL;
    args->maskFile       = NULL;
    args->inputDir       = NULL;
    args->sfiDir         = NULL;
    
    args->gridStartX     = GRID_START_X;
    args->gridStartY     = GRID_START_Y;
    args->gridSpaceX     = GRID_SPACE_X;
    args->gridSpaceY     = GRID_SPACE_Y;

    args->saveMasks      = 1;

    quiet = 0;
    debuglevel = 0;

    /******* Read command line arguments *******/

    for (i = 1;i < argc;i++) {

        /* Catch common help requests */
        if      (readOption      (argc, argv, &i, "-help" )) { usage(argv[0]); }
        else if (readOption      (argc, argv, &i, "--help")) { usage(argv[0]); }

        /* size parameters */
        else if (readOptionInt2(argc, argv, &i, "-gridStart",&(args->gridStartX),&(args->gridStartY))) { }
        else if (readOptionInt2(argc, argv, &i, "-gridSpace",&(args->gridSpaceX),&(args->gridSpaceY))) { }

        /* other flags */
        else if (readOption    (argc, argv, &i, "-quiet")) { quiet = 1; }
        else if (readOptionInt (argc, argv, &i, "-debuglevel", &debuglevel)) {}

        /* check if the current argument is an unparsed option */
        else if (checkBadOption(argc,argv,&i)) {}

        /* read required arguments */
        else if (param_num == 0) {
            args->inputDir = argv[i];
            param_num++;
        }
        else if (param_num == 1) {
            args->imageList = argv[i];
            param_num++;
        }
        else if (param_num == 2) {
            args->sfiDir = argv[i];
            param_num++;
        }
        else if (param_num == 3) {
            args->maskFile = argv[i];
            param_num++;
        }
        else{ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    }

    /* make sure that there are the proper number of required arguments */
    if (param_num != 4){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    /* Print out the program parameters for appropreate debug level */

    DEBUG(1,"Debuging enabled");
}

void convertImages(Arguments* args){
    FILE* list;
    JetMasks masks;
    int x, y,i,j;
    char imagename[MAX_FILENAME_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
    
    MESSAGE("Creating gabor masks.");
    masks = readMasksFile(args->maskFile);

    if(args->saveMasks){
        for(y = 0; y < masks->size; y++){
            char outname[MAX_FILENAME_LENGTH];
            sprintf(outname, "mask%03d.pgm",y);
            writePGMImage(masks->masks[y],outname,0);
        }
    }

       
    list = fopen(args->imageList,"r");
    if(!list){
        printf("Error opening file: %s\n", args->imageList);
        exit(1);
    }

    while(fscanf(list, "%s", imagename) == 1){
        Image im;
        Image grid;
        
        sprintf(filename, "%s/%s", args->inputDir, imagename);
        im = readRawImage(filename);
        MESSAGE1ARG("Processing file: %s",filename);

        /* Find the number of points in the grid */
        
        i = 0;
        for( x = args->gridStartX; x < im->width; x += args->gridSpaceX){
            for( y = args->gridStartY; y < im->height; y+= args->gridSpaceY){
                i++;
            }
        }
        
        grid = makeImage(i,masks->size,1);

        /* Compute convolutions */
        i = 0;
        for( x = args->gridStartX; x < im->width; x += args->gridSpaceX){
            for( y = args->gridStartY; y < im->height; y+= args->gridSpaceY){
                for(j = 0; j < masks->size; j++){
                    if( i < grid->width )
                        IE(grid,i,j,0) = convolvePoint(x, y, 0, im, masks->masks[j]);
                }
                i++;
            }
            
        }

        sprintf(filename, "%s/%s", args->sfiDir, imagename);
        writeRawImage(grid,filename);

        freeImage(grid);
        freeImage(im);

    }

    
    fclose(list);
   
}

int main(int argc, char** argv){
    Arguments args;

    processCommand(argc, argv, &args);

    convertImages(&args);

    return 0;
}



