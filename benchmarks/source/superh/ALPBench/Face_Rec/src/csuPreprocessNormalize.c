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
 *  csuFace2Norm.c
 *  csuFace2Norm
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

#include <csuCommon.h>

#define OPENING "csuFace2Norm - Normalize face data from pgm files"

/* default normalization parameters */
#define ELLIPSE_X    65.0
#define ELLIPSE_Y    50.0
#define ELLIPSE_A    64.0
#define ELLIPSE_B    118.0

#define EDGE_PRE     0
#define EDGE_POST    0

#define EYE_LX       30.0
#define EYE_LY       45.0
#define EYE_RX       100.0
#define EYE_RY       45.0

#define SIZE_WIDTH   130
#define SIZE_HEIGHT  150

#define NOISE        0.0
#define SCALE        1.0
#define SHIFT_X      0.0
#define SHIFT_Y      0.0

#define HIST_NONE    0
#define HIST_PRE     1
#define HIST_POST    2


typedef struct {
    char* eyeFile;
    char* inputDir;
    char* nrmDir;
    char* sfiDir;
    char* pgmDir;

    int geoType;
    int histType;
    int nrmType;
    int preNormType;
    int maskType;
    int preEdge;
    int postEdge;

    int sizeWidth;
    int sizeHeight;

    double scale;
    double noise;
    double shiftX;
    double shiftY;
    int reflect;
    int configSuffix;
    
    double eyeLx;
    double eyeLy;
    double eyeRx;
    double eyeRy;

    double ellipseX;
    double ellipseY;
    double ellipseA;
    double ellipseB;
}
Arguments;

/*
 * usage
 *
 * Display a standard usage parameters or help if there is a problem with the
 * command line.
 */
void usage(const char* name){
    printf("Usage: %s [OPTIONS] eye_coordinate_file input_dir\n",name);
    printf("  OUTPUT OPTIONS:  (You must specify at least one output directory)\n");
    printf("    -nrm <DIR_NAME>         - Directory to store images in nrm format (feret\n"
           "                              compatible)\n");
    printf("    -sfi <DIR_NAME>         - Directory to store images in raw format (gabor\n"
           "                              compatible)\n");
    printf("    -pgm <DIR_NAME>         - Directory to store images in pgm format ()\n");
    printf("  NORMALIZATION OPTIONS:\n");
    printf("    -mask <YES/NO>          - Specify an ellipse that defines a mask.\n"); 
    printf("                              Defaults to \"Yes\".\n");
    printf("    -mask-param x y a b     - Specify an ellipse that defines a mask. \n");
    printf("                              Defaults to FERET Standard, %0.1f, %0.1f, %0.1f, %0.1f \n", 
                                          ELLIPSE_X, ELLIPSE_Y, ELLIPSE_A, ELLIPSE_B);
    printf("    -hist <TYPE>            - Select the type of histogram equalization.\n");
    printf("                              Default is \"POST\", Options are \"PRE\", \"POST\", \"NONE\". \n");
    printf("                              \"PRE\"  - equalize raw 0-255 pixels, like original NIST code.\n");
    printf("                              \"POST\" - pixels under mask only after geometric normalization.\n");
    printf("    -geometric <YES/NO>     - Turns on/off geometric normalization. Defaults to \"Yes\".\n");
    printf("    -eye-target lx ly rx ry - Choose the target eye coordinates.\n");
    printf("                              Defaults to FERET Standard, %0.1f, %0.1f, %0.1f, %0.1f \n", 
                                          EYE_LX, EYE_LY, EYE_RX, EYE_RY);
    printf("    -size width height      - Choose the target size for the images.\n");
    printf("                              Defaults to %d, %d \n", SIZE_WIDTH, SIZE_HEIGHT);
    printf("    -pixelNorm <YES/NO>     - Turns on/off pixel value normalization. Defaults to \"Yes\"\n");
    printf("    -postNorm <YES/NO>      - Turns on/off pixel value normalization. Defaults to \"Yes\"\n");
    printf("    -preNorm <YES/NO>       - Turns on a pixel normalization before geometric normalization. Defaults to \"NO\"\n");
    printf("    -preEdge <INT>          - Blends the edge of the image (INT pixels wide) before geometric normalization. Defaults to \"0\"\n");
    printf("    -postEdge <INT>         - Blends the edge of the image (INT pixels wide) after normalization. Defaults to \"NO\"\n");
    printf("    -noise <double>         - Add zero mean Gausian noise to each pixel.\n");
    printf("                              The Standard Deviation is set to argument value times the dynamic\n");
    printf("                              range of the image. Defaults to %f indicating zero noise added.\n", NOISE);
    printf("    -reflect <YES/NO>       - Reflect the image down the center of the face.\n");
    printf("    -half                   - Easy way to produce half sized images.\n");
    printf("    -scale f                - Easy way to produce scaled images.\n");
    printf("    -shift dx dy            - Shift all eye targets by dx and dy.\n");
    printf("							  Default is no shift, i.e. 0 0.\n");
    printf("                              Postive dx moves the face to the left. Postive dy moves the face up.\n");
    printf("  OTHER OPTIONS:\n");
    printf("    -configSuffix           - Normalization configuration encoded in new image file names.\n");
    printf("    -quiet                  - Turns off all messages.\n");
    printf("    -debuglevel <int>       - Turns turns on debuging output.\n");
    exit(1);
}

/*
 * Process the command line and initialize the variables
 */

void processCommand(int argc, char** argv, Arguments* args) {
    int i;
    int param_num = 0;

    /******* Set up default values *******/
    args->geoType      = CL_YES;
    args->histType     = HIST_POST;
    args->nrmType      = CL_YES;
    args->preNormType  = CL_NO;
    args->maskType     = CL_YES;
    args->preEdge      = EDGE_PRE;
    args->postEdge     = EDGE_POST;

    args->sizeWidth    = SIZE_WIDTH;
    args->sizeHeight   = SIZE_HEIGHT;

    args->eyeLx        = EYE_LX;
    args->eyeLy        = EYE_LY;
    args->eyeRx        = EYE_RX;
    args->eyeRy        = EYE_RY;

    args->scale        = SCALE;
    args->noise        = NOISE;
    args->shiftX       = SHIFT_X;
    args->shiftY       = SHIFT_Y;
    args->reflect      = CL_NO;
    args->configSuffix = CL_NO;

    args->ellipseX     = ELLIPSE_X;
    args->ellipseY     = ELLIPSE_Y;
    args->ellipseA     = ELLIPSE_A;
    args->ellipseB     = ELLIPSE_B;

    args->inputDir     = NULL;
    args->nrmDir       = NULL;
    args->sfiDir       = NULL;
    args->pgmDir       = NULL;

    debuglevel = 0;

    /******* Read command line arguments *******/

    for (i = 1;i < argc;i++) {

        /* Catch common help requests */
        if      (readOption      (argc, argv, &i, "-help" )) { usage(argv[0]); }
        else if (readOption      (argc, argv, &i, "--help")) { usage(argv[0]); }

        /******* Read in output directories *******/
        else if (readOptionString(argc, argv, &i, "-nrm", &(args->nrmDir))){ }
        else if (readOptionString(argc, argv, &i, "-sfi", &(args->sfiDir))){ }
        else if (readOptionString(argc, argv, &i, "-pgm", &(args->pgmDir))){ }

        /* read in shift option */
        else if (readOptionDouble2(argc, argv, &i, "-shift", &(args->shiftX), &(args->shiftY))) { }
        /******* Read in normalization options *******/
        /* Mask parameters */
        else if (readOptionDouble4(argc, argv, &i, "-mask-param", &(args->ellipseX), &(args->ellipseY), &(args->ellipseA), &(args->ellipseB))) { }
        else if (readOptionYesNo(argc, argv, &i, "-mask", &(args->maskType))) { }

        /* size parameters */
        else if (readOptionInt2(argc, argv, &i, "-size",&(args->sizeWidth),&(args->sizeHeight))) { }
        
        /* geometric parameters */
        else if (readOptionDouble4(argc, argv, &i, "-eye-target", &(args->eyeLx), &(args->eyeLy),  &(args->eyeRx), &(args->eyeRy))) { }
        else if (readOptionYesNo  (argc, argv, &i, "-geometric",&(args->geoType))) { }
        else if (readOptionYesNo  (argc, argv, &i, "-reflect"  ,&(args->reflect))) { }
        else if (readOption       (argc, argv, &i, "-half")) { args->scale = 0.5; }
        else if (readOptionDouble (argc, argv, &i, "-scale", &(args->scale))) { }        
        
        /* histogram parameters */
        else if (readOptionMatch(argc, argv, &i, "-hist", "NONE")) { args->histType   = HIST_NONE; }
        else if (readOptionMatch(argc, argv, &i, "-hist", "PRE" )) { args->histType   = HIST_PRE;  }
        else if (readOptionMatch(argc, argv, &i, "-hist", "POST")) { args->histType   = HIST_POST; }

        /* Image Edge Smoothing */
        else if (readOptionInt (argc, argv, &i, "-preEdge", &args->preEdge)) {}
        else if (readOptionInt (argc, argv, &i, "-postEdge", &args->postEdge)) {}
        
        /* Turn on noise */
        else if (readOptionDouble(argc, argv, &i, "-noise", &(args->noise))) { }
        
        /* Turn of ZeroMeanOneStdDev */
        else if (readOptionYesNo(argc, argv, &i, "-pixelNorm", &args->nrmType)) { }

        /* Turn of ZeroMeanOneStdDev */
        else if (readOptionYesNo(argc, argv, &i, "-preNorm", &args->preNormType)) { }

        /* Turn on file name configuration encoding */
        else if (readOption(argc, argv, &i, "-configSuffix")) { args->configSuffix = CL_YES; }

        /* other flags */
        else if (readOption    (argc, argv, &i, "-quiet")) { quiet = 1; }
        else if (readOptionInt (argc, argv, &i, "-debuglevel", &debuglevel)) {}

        /* check if the current argument is an unparsed option */
        else if (checkBadOption(argc,argv,&i)) {}

        /* read required arguments */ 
        else if (param_num == 0) {
            args->eyeFile = argv[i];
            param_num++;
        }
        else if (param_num == 1) {
            args->inputDir = argv[i];
            param_num++;
        }
        else{ clParseError(argc,argv,i,"Wrong number of required arguments"); }
        
    }

    /* make sure that there are the proper number of required arguments */
    if (param_num != 2){ clParseError(argc,argv,i,"Wrong number of required arguments"); }

    /* Print out the program parameters for appropreate debug level */
    DEBUG_CHECK(args->nrmDir || args->pgmDir || args->sfiDir,"You must specify at least one output directory.");

    DEBUG(1,"Debuging enabled");
    if(debuglevel > 0){
        printf("***************** Program Parameters *********************\n");
        printf("Eye coordinates file: %s\n", args->eyeFile ? args->eyeFile : "NONE");
        printf("Input directory:      %s\n", args->inputDir ? args->inputDir : "NONE");
        printf("nrm   directory:      %s\n", args->nrmDir ? args->nrmDir : "NONE");
        printf("sfi   directory:      %s\n", args->sfiDir ? args->sfiDir : "NONE");
        printf("pgm   directory:      %s\n", args->pgmDir ? args->pgmDir : "NONE");
        printf("Output Size:          %dX%d\n", args->sizeWidth, args->sizeHeight);
        printf("Geometric Norm:       %s\n", args->geoType ? "YES" : "NO");
        printf("Eye Target:           (%0.2f %0.2f %0.2f %0.2f)\n", args->eyeLx, args->eyeLy, args->eyeRx, args->eyeRy);
        printf("Histogram Norm:       %d\n", args->histType);
        printf("Value Norm:           %s\n", args->nrmType ? "YES" : "NO");
        printf("Apply Mask:           %s\n", args->maskType ? "YES" : "NO");
        printf("Mirror Reflect:       %s\n", args->reflect ? "YES" : "NO");
        printf("Config Suffix:        %s\n", args->configSuffix ? "YES" : "NO");
        printf("Mask Parameters:      (%0.2f %0.2f %0.2f %0.2f)\n", args->ellipseX, args->ellipseY, args->ellipseA, args->ellipseB);
    }    
}


/* Masks are no longer read from a file.  It is created by scan converting an ellipse. */
double ellipseFunc( double i, double j, double x, double y, double a, double b){
    return 1 - (SQR(x-i)/SQR(a) + SQR(y-j)/SQR(b));
}

char** generateMask(int width, int height, double x, double y, double a, double b){
    char **mask;
    int i,j;

    mask = (char**) malloc(sizeof(char*)*width);
    assert(mask); /* problem allocating space for mask */

    for(i = 0; i < width; i++){
        mask[i] = (char*) malloc(sizeof(char)*height);
        assert(mask[i]); /* problem allocating space for mask */
    }

    for(j = 0; j < height; j++){
        for(i = 0; i < width; i++){
            if(ellipseFunc(i,j,x,y,a,b) > 0){
                mask[i][j] = 1;
            }
            else{
                mask[i][j] = 0;                
            }
        }
    }

    return mask;
}


/********************* Geometric Transformation Code *************************/

/* This function performs a 3X3 two dimentional perspective transform to an image
This is used to perform geomentric normalization */
Image transformImage(Image source, int newWidth, int newHeight, const Matrix m){
    Image dest = makeImage(newWidth, newHeight, source->channels);
    Matrix point = makeMatrix(3,1);
    Matrix inv;
    Matrix pt;
    int x, y, c;

    assert(m->row_dim == 3);
    assert(m->col_dim == 3);

    /* initialize homogenius point */
    ME(point,2,0) = 1;

    /* find the inverse transformation to convert from dest to source */
    inv = invertRREF(m);

    for(x = 0; x < dest->width; x++){
        for(y = 0; y < dest->height; y++){
            /* calculate source point */
            ME(point,0,0) = x;
            ME(point,1,0) = y;
            ME(point,2,0) = 1.0;
            pt = multiplyMatrix(inv, point);
            ME(pt,0,0) = ME(pt,0,0) / ME(pt,2,0);
            ME(pt,1,0) = ME(pt,1,0) / ME(pt,2,0);

            for(c = 0; c < dest->channels; c++){
                /* interpolate value */
                IE(dest,x,y,c) = interpLinear(source, ME(pt,0,0),ME(pt,1,0),c);
            }
            /* clean up */
            freeMatrix(pt);
        }
    }
    freeMatrix(point);
    freeMatrix(inv);

    return dest;
}

/* Return a translation matrix */
Matrix translateMatrix(double dx, double dy){
    Matrix transform = makeIdentityMatrix(3);

    ME(transform,0,2) = dx;
    ME(transform,1,2) = dy;

    return transform;
}

/* Return a scale Matrix */
Matrix scaleMatrix(double s){
    Matrix transform = makeIdentityMatrix(3);
    ME(transform,0,0) = s;
    ME(transform,1,1) = s;

    return transform;
}

/* Return a rotation matrix */
Matrix rotateMatrix(double theta){
    Matrix transform = makeIdentityMatrix(3);

    ME(transform,0,0) = cos(theta);
    ME(transform,1,1) = cos(theta);
    ME(transform,0,1) = -sin(theta);
    ME(transform,1,0) = sin(theta);

    return transform;
}

Matrix reflectMatrix(int bool_x, int bool_y){
    Matrix transform = makeIdentityMatrix(3);

    if(bool_x) ME(transform,0,0) = -1;
    if(bool_y) ME(transform,1,1) = -1;
    return transform;
}


typedef struct {
    FTYPE x1, y1, x2, y2;
} TwoPoints;

Matrix generateTransform(const TwoPoints *source, const TwoPoints *dest, int reflect){
    double sourceMidX = (source->x1 + source->x2)*0.5;
    double sourceMidY = (source->y1 + source->y2)*0.5;
    double destMidX = (dest->x1 + dest->x2)*0.5;
    double destMidY = (dest->y1 + dest->y2)*0.5;
    Matrix translate1 = translateMatrix(-sourceMidX, -sourceMidY); /* translate the left point to the origin */
    Matrix translate2 = translateMatrix(destMidX, destMidY); /* translate the origin to the left destination */

    /* compute the scale that needs to be applyed to the image */
    double sdist = sqrt(SQR(source->x1 - source->x2) + SQR(source->y1 - source->y2));
    double ddist = sqrt(SQR(dest->x1 - dest->x2) + SQR(dest->y1 - dest->y2));
    double s = ddist/sdist; 
    Matrix scale = scaleMatrix(s);

    /* compute the rotation that needs to be applyed to the image */
    double stheta = atan((source->y2 -source->y1)/(source->x2 - source->x1));
    double dtheta = atan((dest->y2 -dest->y1)/(dest->x2 - dest->x1));
    double theta  = dtheta - stheta;
    Matrix rotate = rotateMatrix(theta);

    /* compute the reflection if nessicary */
    Matrix reflection = reflectMatrix(reflect,0);

    /* build the final transformation */
    Matrix tmp1 = multiplyMatrix(scale, translate1);
    Matrix tmp2 = multiplyMatrix(rotate, tmp1);
    Matrix tmp3 = multiplyMatrix(reflection, tmp2);
    Matrix transform = multiplyMatrix(translate2,tmp3);

    /* free temporary matricies */
    freeMatrix(translate1);
    freeMatrix(translate2);
    freeMatrix(scale);
    freeMatrix(rotate);
    freeMatrix(reflection);
    freeMatrix(tmp1);
    freeMatrix(tmp2);
    freeMatrix(tmp3);

    /* return final transformation */
    return transform;
}

void scaleArgs(Arguments* args, double scale){
    args->eyeLx *= scale;
    args->eyeLy *= scale;
    args->eyeRx *= scale;
    args->eyeRy *= scale;

    args->ellipseX *= scale;
    args->ellipseY *= scale;
    args->ellipseA *= scale;
    args->ellipseB *= scale;

    args->sizeWidth  = ROUND(args->sizeWidth*scale);
    args->sizeHeight = ROUND(args->sizeHeight*scale);
}

/*	The code to generate a file suffix will build a string that is
	compact and easily selected for using standard unix "ls". However,
	it is not obvious. Here is the encoding:
		c<0/1>			Apply mask, 0=no, 1=yes	
		f<0/1>			Pixel Value Normalization, 0=no, 1=yes	
		g<0/1>			Geometric Normalization, 0=no, 1=yes
		h<0/1/2>		Histogram Equalization, 0=no, 1=pre, 2=post
		n<0/1>			Indepenent Pixel Gaussian Noise, 0=no, 1=yes
		x<0/1>			Reduce image size by half, 0=no, 1=yes
		m<0/1>          Mirror reflect the image, 0=no, 1=yes
		<l/r/u/d><#>	Shift source image left, right, up or down by amount #	
		or t0           If there is no translational shift.
		
	All but the last are always included in a file name. The shift tag only 
	appears if an image is shifted.
	
	Normal defaulst will produce suffix "c1f1g1h2n0x0"
	*/

char* imageSuffix(Arguments* args) {
	const int len = 32; /* large enough to include shift indicators plus slack */
	char* s       = (char*) malloc(sizeof(char)*len);
	strcpy(s,"c0f0g0h0n0x0m0");  /* Starts out with zeroes as default settings */
	if (args->maskType == CL_YES) 		s[1]  = '1';
	if (args->nrmType  == CL_YES) 		s[3]  = '1';
	if (args->geoType  == CL_YES)		s[5]  = '1';
	if (args->histType == HIST_PRE)  	s[7]  = '1';
	if (args->histType == HIST_POST) 	s[7]  = '2';
	if (args->noise    != 0.0)			s[9]  = '1';
	if (args->scale    == 0.5)			s[11] = '1';
	if (args->reflect  == CL_YES)	    s[13] = '1';					
	if (args->shiftX < 0.0) 			sprintf(s,"%s%s%d", s, "r", (int) ceil(-args->shiftX));
	if (args->shiftX > 0.0) 			sprintf(s,"%s%s%d", s, "l", (int) ceil( args->shiftX));
	if (args->shiftY < 0.0) 			sprintf(s,"%s%s%d", s, "d", (int) ceil(-args->shiftY));
	if (args->shiftY > 0.0) 			sprintf(s,"%s%s%d", s, "u", (int) ceil( args->shiftY));		
	if (args->shiftX == 0.0 && args->shiftY == 0.0) sprintf(s,"%s%s", s, "t0");
	return s;
}

void convertImages(Arguments* args){
    char** mask = NULL;
    TwoPoints source, dest;
    FILE* eyeList;
    char line[ FILE_LINE_LENGTH ];
    char filename[MAX_FILENAME_LENGTH];
    char imagename[MAX_FILENAME_LENGTH];
    char suffix[MAX_FILENAME_LENGTH];
    int i;

    scaleArgs(args, args->scale);
    
    dest.x1 = args->eyeLx;
    dest.y1 = args->eyeLy;
    dest.x2 = args->eyeRx;
    dest.y2 = args->eyeRy;
    
	/* Prepare file suffix encoding preprocessing settings, blank if not requested */
	if (args->configSuffix) {
		sprintf(suffix,"_%s", imageSuffix(args)); }
	else {
		suffix[1] = '\0'; }	

    if(args->maskType == CL_YES){
        MESSAGE("Creating Mask.");
        mask = generateMask(args->sizeWidth, args->sizeHeight, args->ellipseX, args->ellipseY, args->ellipseA, args->ellipseB);
    }

    eyeList = fopen(args->eyeFile,"r");
    DEBUG_CHECK(eyeList,"Error opening eye coordinates file");

    for(i = 1;;i++){
        Image pgm;
        Image geo;
        Matrix transform;
        
        fgets(line, FILE_LINE_LENGTH, eyeList);
        if(feof(eyeList)) break;

        if(sscanf(line,"%s %lf %lf %lf %lf",filename, &(source.x1), &(source.y1), &(source.x2), &(source.y2)) != 5){
            printf("Error parsing line %d of eye coordinate file. Exiting...",i);
            exit(1);
        }

        /* shift the eye coordinates if neccessary */
        source.x1 += args->shiftX;
        source.y1 += args->shiftY;
        source.x2 += args->shiftX;
        source.y2 += args->shiftY;

        sprintf(imagename,"%s/%s.pgm",args->inputDir,filename);

        MESSAGE1ARG("Processing image: %s",filename);
        
        pgm = readPGMImage(imagename);

        if(args->histType == HIST_PRE){
            DEBUG(1,"   Performing Pre Histogram Equalization.");
            histEqual(pgm,256);
        }

        if(args->preNormType == CL_YES){
            DEBUG(1,"   Performing Pre Pixel Normalization.");
            ZeroMeanOneStdDev(pgm);
        }
        
        if(args->preEdge){
            smoothImageEdge(pgm, args->preEdge);
        }

        if(args->geoType == CL_YES){
            DEBUG(1,"   Performing Geometric Normalization.");
            transform = generateTransform(&source,&dest,args->reflect);
            geo = transformImage(pgm,args->sizeWidth,args->sizeHeight,transform);
        }
        else{
            transform = makeIdentityMatrix(3);
            geo = transformImage(pgm,args->sizeWidth,args->sizeHeight,transform);
        }

        if(args->noise != 0.0){
            DEBUG(1,"   Adding Gausian Noise.");
            gaussianNoise(geo,args->noise);
        }
            

        if(args->histType == HIST_POST){
            DEBUG(1,"   Performing Post Histogram Equalization.");
            histEqualMask(geo,256, (const char**) mask);
        }

        if(args->nrmType == CL_YES){
            DEBUG(1,"   Performing final value normalization and Applying Mask.");
            ZeroMeanOneStdDevMasked(geo, (const char **) mask);
        }
        else{
            DEBUG(1,"   No Value Normalization. Just Applying Mask.");
            applyMask(geo, (const char **) mask);
        }

        if(args->postEdge){
            smoothImageEdge(geo, args->postEdge);
        }
        
        if(args->nrmDir){
            sprintf(imagename,"%s/%s%s.nrm", args->nrmDir, filename, suffix);
            DEBUG_STRING(1,"   Saving nrm: %s",imagename);
            writeFeretImage(geo,imagename);
        }
        if(args->pgmDir){
            sprintf(imagename,"%s/%s%s.pgm", args->pgmDir, filename, suffix);
            DEBUG_STRING(1,"   Saving pgm: %s",imagename);
            writePGMImage(geo,imagename,0);
        }
        if(args->sfiDir){
            sprintf(imagename,"%s/%s%s.sfi", args->sfiDir, filename, suffix);
            DEBUG_STRING(1,"   Saving sfi: %s",imagename);
            writeRawImage(geo,imagename);
        }

        freeImage(geo);
        freeImage(pgm);
        freeMatrix(transform);
    }

    fclose(eyeList);
    
}

int main(int argc, char** argv){
    Arguments args;
    processCommand(argc, argv, &args);
    MESSAGE(OPENING);
    MESSAGE(VERSION);

    MESSAGE("Converting Images...");
    convertImages(&args);

    return 0;
}






