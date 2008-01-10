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

#include "csuCommon.h"
#include <math.h>

/******************************* Local Fucntions ********************8******/

/* checks to see if the machine is Little Endian
*  this is used to determine if byte reversal is
*  needed when reading or writing data files in
*  sun byte order.
*/
int ImageIsMachineLittleEndian() {
    int flag;
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
    return flag;
}


/**
 * Normally distrubuted random number using the Box-Muller Transformation
 */
double randBM(){
    static int iset = 0;
    static double gset;
    double v1, v2, fac, rsq;
    if(iset == 0){

        /* Pick a point in the unit circle */
        do{
            v1 = 2.0*RANDOM-1.0;
            v2 = 2.0*RANDOM-1.0;
            rsq = SQR(v1) + SQR(v2);
        } while (rsq >= 1.0 || rsq == 0);

        /* Perform the Box-Muller transformation */
        fac = sqrt(-2*log(rsq)/rsq);
        gset = v1*fac;
        iset = 1;
        return v2*fac;
    }
    else {
        /* Return the deviate that was not returned last time */
        iset = 0;
        return gset;
    }
}


/******************************* Image Fucntions ********************8******/

Image makeImage(int width,int height,int channels){
    int x,y;
    Image i = (Image)malloc(sizeof(image));
    assert(i);
    i->data = (PIX_TYPE***)malloc(sizeof(PIX_TYPE**)*width);
    assert(i->data);

    for(x = 0; x < width; x++){
        i->data[x] = (PIX_TYPE**)malloc(sizeof(PIX_TYPE*)*height);
        assert(i->data[x]);
        for(y = 0; y < height; y++){
            i->data[x][y] = (PIX_TYPE*)malloc(sizeof(PIX_TYPE)*channels);
            assert(i->data[x][y]);
        }
    }
    
    i->width = width;
    i->height = height;
    i->channels = channels;

    return i;
}


Image makeZeroImage(int width,int height,int channels){
    Image im = makeImage( width, height, channels);
    int i,j,c;

    for(i=0;i<im->width;i++){
        for(j=0;j<im->height;j++){
            for(c=0;c<im->channels;c++){
                IE(im,i,j,c) = 0.0;
            }
        }
    }
    return im;
}


Image duplicateImage(Image im){
    Image output = makeImage(im->width,im->height,im->channels);
    int i, j, c;

    for(j = 0; j < output->height; j++){
        for(i = 0; i < output->width; i++){
            for(c = 0; c < output->channels; c++){
                IE(output,i,j,c) = IE(im,i,j,c);
            }
        }
    }

    return output;

}

/* Safe read-only/range-checked implementation of IE - returns 0.0 if off image. */
#if(!INTERPOLATE_FAST)
PIX_TYPE ie(Image im, int x, int y, int c){
    double out = ((x < im->width && y < im->height && c < im->channels && x >= 0 && y >=  0 && c >= 0) ? IE(im, x, y, c) : 0.0 );

    return out;
}
#endif


#if(!INTERPOLATE_FAST)
#define INT_FLOOR(a) ((int)(a))
#define INT_CEIL(a)  ((int)(a)+1)

/* interpolate a pixel value at a point */
PIX_TYPE interpLinear(Image img, PIX_TYPE x, PIX_TYPE y, int c){
    PIX_TYPE xfrac = (x - floor(x));
    PIX_TYPE yfrac = (y - floor(y));
    int xLower = INT_FLOOR(x);
    int xUpper = INT_CEIL(x);
    int yLower = INT_FLOOR(y);
    int yUpper = INT_CEIL(y);
    PIX_TYPE valUpper, valLower;

    valUpper = ie(img,xLower,yUpper,c)*(1.0-xfrac) + ie(img,xUpper,yUpper,c)*(xfrac);
    valLower = ie(img,xLower,yLower,c)*(1.0-xfrac) + ie(img,xUpper,yLower,c)*(xfrac);

    return valLower*(1.0-yfrac) + valUpper*(yfrac);
}
#endif

void freeImage(Image i){
    int x, y;
    if(i == 0){
        printf("Warning: free null image\n");
        return;
    }
    for(x = 0; x < i->width; x++){
        for(y = 0; y < i->height; y++){
            free(i->data[x][y]);
        }
        free(i->data[x]);
    }
    free(i->data);
    free(i);
}




Image readPGMImage(const char* filename){
    int  width, height, max, x, y;
    unsigned  val;
    char fchar;
    char line[100];
    char ftype[16];
    FILE *infile;
    Image im;

    /* Read first line, and test if it is propoer PNM type */

    infile = (FILE*) fopen(filename, "rb");
    assert(infile);

    fgets(line,100,infile);
    sscanf(line," %s",ftype);

    if (! (strcmp(ftype,"P5") == 0)) {
        printf("Error <%s,%d>: Currenlty only binary pgm files, type P5, supported",__FILE__,__LINE__);fflush(stdout);
        exit(1);
    }

    /* Read lines, ignoring those starting with Comment Character, until the
        Image Dimensions are read. */
    fchar = '#';
    while (fchar == '#') {
        fgets(line,100,infile);
        sscanf(line, " %c", &fchar);
    }
    sscanf(line, " %d %d", &width, &height);

    /* Read lines, ignoring those starting with Comment Character, until the
        maximum pixel value is read. */
    fchar = '#';
    while (fchar == '#') {
        fgets(line,100,infile);
        sscanf(line, " %c", &fchar);
    }
    sscanf(line, "%d", &max);

    if (! (max == 255)) {
        fprintf(stdout,"readImagePGM: Warning, max value %d for pixels in image %s is not 255\n", max, filename); fflush(stdout);
    }

    im = makeImage(width,height,1);

    val = fgetc(infile);
    for(y = 0; y < height; y++){
        for(x = 0; x < width; x++){
            if(val == EOF){
                printf("Error <%s,%d>: Unexpected end of file: %s",__FILE__,__LINE__,filename);fflush(stdout);
                exit(1);
            }

            IE(im,x,y,0) = (unsigned char)val;
            val = fgetc(infile);
        }
    }

    fclose(infile);

    return( im );
}

/* ---------------------------------------------------------------------------- */
/*  Read a binary image from a file where the pixels are stored as 32 bit
*   floating point numbers using Sun byte order.  The compiler directive WINDOWS
*   is used to control how the byte order is interpreted when storing the pixels
*   into the data structure images. images is a one dimensional array of floats
*   that contains each successive image in sequence, where each image has in turn
*   been unrolled into a 1 dimensional vector.
*
*   INPUT:  fname   is the name of the file from which to read the image.
*           n       is the index of this image in the images array.
*	    images  is the size of the vectors or number of pixels of the image
*/

/* EL and NUM are used to read binary dataand convert to proper internal float representation */
typedef struct {char a,b,c,d;} EL;
typedef union {float f; EL elem;} NUM;

Image readRawImage(const char *fname){
    int i,j,c;
    FILE *f;

    NUM junk;
    char firstline[1000];
    char ftype[1000];

    Image im;

    f = fopen( fname, "rb" );
    if ( !f ) { printf("Can't open %s\n", fname); exit(1); }

    fgets(firstline,1000,f);
    sscanf(firstline,"%s %d %d %d", ftype,&i, &j, &c);

    if( !(strcmp(ftype,RASTER_ID) == 0 || strcmp(ftype,"CSU_RASTER") == 0)){
        printf("Error<%s,%d>: Do not recognize file type '%s'\n",__FILE__,__LINE__, ftype);
        exit(1);
    }

    im = makeImage(i,j,c);

    for(j=0;j<im->height;j++){
        for(i=0;i<im->width;i++){
            for(c=0;c<im->channels;c++){
                /* read in the correct byte order for floating point values */
                if(ImageIsMachineLittleEndian()){
                    fread(&(junk.elem.d),1,1,f);
                    fread(&(junk.elem.c),1,1,f);
                    fread(&(junk.elem.b),1,1,f);
                    fread(&(junk.elem.a),1,1,f);
                }
                else{
                    fread(&(junk.elem.a),1,1,f);
                    fread(&(junk.elem.b),1,1,f);
                    fread(&(junk.elem.c),1,1,f);
                    fread(&(junk.elem.d),1,1,f);
                }

#ifdef RANGE_CHECK
                /* Check values to make sure they are real */
                FINITE( junk.f );
#endif

                /* Save value to images */
                IE(im,i,j,c) = (PIX_TYPE)junk.f;
            }
        }
    }
    fclose(f);

    return im;
}


void  writePGMImage(const Image im, const char* filename,int channel){
    FILE* outfile = fopen(filename,"wb");
    unsigned char pixel;
    double pmax, pmin;
    int x, y;

    assert(im);
    if(!outfile){
        printf("could not open %s for writing.\n",filename);
        exit(1);
    }
    fprintf(outfile,"P5\n");
    fprintf(outfile,"%d %d\n",im->width, im->height);
    fprintf(outfile,"%d\n",255);

    pmax = IE(im,0,0,channel);
    pmin = IE(im,0,0,channel);

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            pmax = MAX(IE(im,x,y,channel), pmax);
            pmin = MIN(IE(im,x,y,channel), pmin);
        }
    }
    /*printf(" Min:%f Max:%f \n", pmin, pmax);*/

    for(y = 0; y < im->height; y++){
        for(x = 0; x < im->width; x++){
            if(pmax-pmin <= 0){
                pixel = 0;
            }
            else{
                int tmp = ROUND(255*(IE(im,x,y,channel)-pmin)/(pmax-pmin));
                if(tmp < 0) tmp = 0;
                if(tmp > 255) tmp = 255;
                pixel = (unsigned char)tmp;
            }
            fputc(pixel,outfile);
        }
    }


    fclose(outfile);
}

void  writePPMImage(const Image im, const char* filename){
    FILE* outfile = fopen(filename,"wb");
    unsigned char pixel;
    double pmax, pmin;
    int x, y, c;

    assert(im);
    assert(im->channels == 3);
    if(!outfile){
        printf("could not open %s for writing.\n",filename);
        exit(1);
    }
    fprintf(outfile,"P6\n");
    fprintf(outfile,"%d %d\n",im->width, im->height);
    fprintf(outfile,"%d\n",255);

    pmax = IE(im,0,0,0);
    pmin = IE(im,0,0,0);

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                pmax = MAX(IE(im,x,y,c), pmax);
                pmin = MIN(IE(im,x,y,c), pmin);
            }
        }
    }

    for(y = 0; y < im->height; y++){
        for(x = 0; x < im->width; x++){
            for(c = 0; c < im->channels; c++){
                if(pmax-pmin <= 0){
                    pixel = 0;
                }
                else{
                    int tmp = ROUND(255*(IE(im,x,y,c)-pmin)/(pmax-pmin));
                    if(tmp < 0) tmp = 0;
                    if(tmp > 255) tmp = 255;
                    pixel = (unsigned char)tmp;
                }
                fputc(pixel,outfile);
            }
        }
    }


    fclose(outfile);
}

void writeRawImage(const Image im, const char *fname){
    int i,j,c;
    FILE *f;

    NUM junk;

    f = fopen( fname, "wb" );
    if ( !f ) { printf("Can't open %s\n", fname); exit(1); }

    fprintf(f,"%s %d %d %d\n", RASTER_ID ,im->width, im->height, im->channels);


    for(j=0;j<im->height;j++){
        for(i=0;i<im->width;i++){
            for(c=0;c<im->channels;c++){
                /* read in the correct byte order for floating point values */
                junk.f = IE(im,i,j,c);

#ifdef RANGE_CHECK
                /* Check values to make sure they are real */
                if(isnan( junk.f)){
                    printf("Error: nan value in file: %s", fname);
                    exit(1);
                }
                if(isinf( junk.f)){
                    printf("Error: inf value in file: %s", fname);
                    exit(1);
                }
#endif

                if(ImageIsMachineLittleEndian()){
                    fwrite(&(junk.elem.d),1,1,f);
                    fwrite(&(junk.elem.c),1,1,f);
                    fwrite(&(junk.elem.b),1,1,f);
                    fwrite(&(junk.elem.a),1,1,f);
                }
                else{
                    fwrite(&(junk.elem.a),1,1,f);
                    fwrite(&(junk.elem.b),1,1,f);
                    fwrite(&(junk.elem.c),1,1,f);
                    fwrite(&(junk.elem.d),1,1,f);
                }

            }
        }
    }
    fclose(f);

}


Image readFeretImage(const char *fname, int width, int height,int channels){
    int i,j,c;
    FILE *f;

    NUM junk;

    Image im;

    f = fopen( fname, "rb" );
    if ( !f ) { printf("Can't open %s\n", fname); exit(1); }

    im = makeImage(width,height,channels);

    for(j=0;j<im->height;j++){
        for(i=0;i<im->width;i++){
            for(c=0;c<im->channels;c++){
                /* read in the correct byte order for floating point values */
                if(ImageIsMachineLittleEndian()){
                    fread(&(junk.elem.d),1,1,f);
                    fread(&(junk.elem.c),1,1,f);
                    fread(&(junk.elem.b),1,1,f);
                    fread(&(junk.elem.a),1,1,f);
                }
                else{
                    fread(&(junk.elem.a),1,1,f);
                    fread(&(junk.elem.b),1,1,f);
                    fread(&(junk.elem.c),1,1,f);
                    fread(&(junk.elem.d),1,1,f);
                }

#ifdef RANGE_CHECK
                /* Check values to make sure they are real */
                FINITE( junk.f);
#endif

                /* Save value to images */
                IE(im,i,j,c) = (PIX_TYPE)junk.f;
            }
        }
    }
    fclose(f);

    return im;
}


void writeFeretImage(const Image im, const char *fname){
    int i,j,c;
    FILE *f;

    NUM junk;

    f = fopen( fname, "wb" );
    if ( !f ) { printf("Can't open %s\n", fname); exit(1); }

    for(j=0;j<im->height;j++){
        for(i=0;i<im->width;i++){
            for(c=0;c<im->channels;c++){
                /* read in the correct byte order for floating point values */
                junk.f = IE(im,i,j,c);

#ifdef RANGE_CHECK
                /* Check values to make sure they are real */
                if(isnan( junk.f)){
                    printf("Error: nan value in file: %s", fname);
                    exit(1);
                }

                if(isinf( junk.f)){
                    printf("Error: inf value in file: %s", fname);
                    exit(1);
		    }
#endif

                if(ImageIsMachineLittleEndian()){
                    fwrite(&(junk.elem.d),1,1,f);
                    fwrite(&(junk.elem.c),1,1,f);
                    fwrite(&(junk.elem.b),1,1,f);
                    fwrite(&(junk.elem.a),1,1,f);
                }
                else{
                    fwrite(&(junk.elem.a),1,1,f);
                    fwrite(&(junk.elem.b),1,1,f);
                    fwrite(&(junk.elem.c),1,1,f);
                    fwrite(&(junk.elem.d),1,1,f);
                }

            }
        }
    }
    fclose(f);

}

double convolvePoint(PIX_TYPE x, PIX_TYPE y, int c, Image im, Image mask){
    int i, j;
    PIX_TYPE mysum = 0;
    PIX_TYPE offsetx = x - mask->width/2.0  - 0.5;
    PIX_TYPE offsety = y - mask->height/2.0 - 0.5;

    if(EQUAL_ZERO(offsetx - TRUNC(offsetx),.01) && EQUAL_ZERO(offsetx - TRUNC(offsetx),.01)){
        mysum = 0;
        for(j = 0; j < mask->height; j++){
            for(i = 0; i < mask->width; i++){
                mysum += ie(im,i+TRUNC(offsetx),j+TRUNC(offsety),c) * IE(mask,i,j,c);
            }
        }
    }
    else{
        mysum = 0;
        for(j = 0; j < mask->height; j++){
            for(i = 0; i < mask->width; i++){
               mysum += interpLinear(im,i+offsetx,j+offsety,c) * IE(mask,i,j,c);
            }
        }
    }
#ifdef RANGE_CHECK
    FINITE(offsetx);
    FINITE(offsety);

    FINITE(mysum);
#endif
    return mysum;
}




Image convolveImage(Image im, Image mask){
    Image output = makeImage(im->width,im->height,im->channels);
    int x,y,c;

    for(y = 0; y < output->height; y++){
        for(x = 0; x < output->width; x++){
            for(c = 0; c < output->channels;c++){
                IE(output,x,y,c) = convolvePoint(x,y,c,im,mask);
            }
        }
    }

    return output;
}



void contrastImage(Image im, double scalar){
    int i, j, c;

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                IE(im,i,j,c) = IE(im,i,j,c)*scalar;
            }
        }
    }

}

void brightnessImage(Image im, double scalar){
    int i, j, c;

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                IE(im,i,j,c) = IE(im,i,j,c)+scalar;
            }
        }
    }

}


void ZeroMeanUnitLength(Image im){
    int i, j, c;

    double mean = 0; double sqrsum = 0; double invlength = 0;

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                mean += IE(im,i,j,c);
            }
        }
    }

    mean = mean / ( im->height*im->width*im->channels );
    /* printf("mean: %f\n",mean); */

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                IE(im,i,j,c) = IE(im,i,j,c)-mean;
                sqrsum += SQR(IE(im,i,j,c));
            }
        }
    }

    /* printf("sqrsum: %f\n",sqrsum); */
    if(sqrsum != 0){
        invlength = 1.0/sqrt(sqrsum);
    } else {
        invlength = 1.0;
    }

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                IE(im,i,j,c) *= invlength;
            }
        }
    }

}



void ZeroMeanUnitLengthMasked(Image im, const char** mask){
/* scales the pixel so that they have zero mean and the image is unit length*/
    int i, j, c;

    double mean = 0; double sqrsum = 0; double invlength = 0;
    int pixels = 0;

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[i][j]){
                    mean += IE(im,i,j,c);
                    pixels++;
                }
            }
        }
    }

    mean = mean / ( pixels );

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[i][j]){
                    IE(im,i,j,c) = IE(im,i,j,c)-mean;
                    sqrsum += SQR(IE(im,i,j,c));
                }
            }
        }
    }

    if(sqrsum != 0){
        invlength = 1.0/sqrt(sqrsum);
    } else {
        invlength = 1.0;
    }

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[i][j]){
                    IE(im,i,j,c) *= invlength;
                }
                else{
                    IE(im,i,j,c) = 0.0;
                }
            }
        }
    }
}

void smoothImageEdge(Image im, int width){
    int i, x, y ,c ;
    for(i = 0; i < width; i++ ){
        double scale = ((double)i)/width;

        /* Scale the horizontal lines */
        for(x = 0; x < im->width; x++){
            for( c = 0; c < im->channels; c++){
                IE(im, x, i, c) *= scale;
                IE(im, x, im->height - i - 1, c) *= scale;
            }
        }
        /* Scale the vertical lines */
        for(y = 0; y < im->height; y++){
            for( c = 0; c < im->channels; c++){
                IE(im, i, y, c) *= scale;
                IE(im, im->width - i - 1, y, c) *= scale;
            }
        }
    }

}


void ZeroMeanOneStdDev(Image im){
    ZeroMeanOneStdDevMasked(im,NULL);
}


void ZeroMeanOneStdDevMasked(Image im, const char** mask){
    /* scales the pixel so that they have zero mean and the image is unit length*/
    int i, j, c;

    double sum = 0.0;
    double sqsum = 0.0;
    double mean;
    double vari;
    double stddev;
    double invlength;
    int pixels = 0;

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[i][j]){
                    sum   += IE(im,i,j,c);
                    sqsum += SQR(IE(im,i,j,c));
                    pixels++;
                }
            }
        }
    }

    mean = sum / pixels;
    vari = sqsum / pixels - SQR(mean);
    stddev = sqrt(vari);

    invlength = 1.0/stddev;

    for(j = 0; j < im->height; j++){
        for(i = 0; i < im->width; i++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[i][j]){
                    IE(im,i,j,c) = (IE(im,i,j,c) - mean) * invlength;
                }
                else{
                    IE(im,i,j,c) = 0.0;
                }
            }
        }
    }
}


double dotImages(const Image i1, const Image i2){
    double dot = 0;
    int i,j,c;

    assert(i1->height == i2->height);
    assert(i1->width == i2->width);
    assert(i1->channels == i2->channels);

    for(j = 0; j < i1->height; j++){
        for(i = 0; i < i1->width; i++){
            for(c = 0; c < i1->channels; c++){
                dot += IE(i1,i,j,c)*IE(i2,i,j,c);
            }
        }
    }

    return dot;

}


double correlateImages(const Image i1, const Image i2){
    int i, j, c;

    double correlation = 0.0;
    double i1mean = 0.0, i2mean = 0.0;
    double i1scale = 0.0, i2scale = 0.0;

    assert(i1->height == i2->height);
    assert(i1->width == i2->width);
    assert(i1->channels == i2->channels);

    /* Pass one: compute the pixel mean. */
    for(j = 0; j < i1->height; j++){
        for(i = 0; i < i1->width; i++){
            for(c = 0; c < i1->channels; c++){
                i1mean += IE(i1,i,j,c);
                i2mean += IE(i2,i,j,c);
            }
        }
    }
    i1mean = i1mean/(i1->height*i1->width*i1->channels);
    i2mean = i2mean/(i2->height*i2->width*i2->channels);

    printf("i1mean:  %f\n",i1mean);
    printf("i2mean:  %f\n",i2mean);
    /* Pass two: compute the unscaled correlation value and the associated scale parameter. */
    for(i = 0; i < i1->width; i++){
        for(j = 0; j < i1->height; j++){
            for(c = 0; c < i1->channels; c++){
                correlation += (IE(i1,i,j,c)-i1mean)*(IE(i2,i,j,c)-i2mean);
                i1scale     += (IE(i1,i,j,c)-i1mean)*(IE(i1,i,j,c)-i1mean);
                i2scale     += (IE(i2,i,j,c)-i2mean)*(IE(i2,i,j,c)-i2mean);
            }
        }
    }

    printf("i1scale: %f\n",i1scale);
    printf("i2scale: %f\n",i2scale);
    correlation = correlation / (sqrt(i1scale)*sqrt(i2scale));

    return correlation;
}



Image accumulateChannels(const Image im){
    int x, y, c;
    Image accum = makeZeroImage(im->width,im->height,1);

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                IE(accum,x,y,0)+=IE(im,x,y,c);
            }
        }
    }

    return accum;
}

void accumulateImages(Image i1, const Image i2){
    /* inplace adds image i2 to image i1 */
    int x, y, c;

    assert(i1->height == i2->height);
    assert(i1->width == i2->width);
    assert(i1->channels == i2->channels);

    for(x = 0; x < i1->width; x++){
        for(y = 0; y < i1->height; y++){
            for(c = 0; c < i1->channels; c++){
                IE(i1,x,y,c)+=IE(i2,x,y,c);
            }
        }
    }
}



/*
 * histEqual
 *
 * Performs an aproximate histogram equalization function.
 * (Multichannel images are supported, however design was for single
 * channel images.)
 *
 * This version of histogram equalization is optimized for use
 * with image based on floating point values.  The output image
 * is a histogram normalized version of the image where pixel values
 * are in the range 0 to 1.
 *
 * ALGORITHM:
 *   1. Determine the range of the pixel values.  And divide that
 *      range into N bins.
 *   2. Treverse the pixels and accumulate pixel counts in appropreate
 *      bins.
 *   3. Equalize the histogram by assigning a range to each bin which
 *      will equalize the probablility distribution
 *   4. For each pixel in the old image look up the apropreate bin and
 *      interpalate its new value based on the range of the bin.
 *
 * Things to note about this algorithm:
 *   1. Pixel values retain there relative ordering because the output
 *      values are interpalated.
 *   2. The algorithm can be used iteritivly to improve equalization.
 *      With enough bins the equalization will be perfict.
 *   3. Bin allocation should be robust to small changes in N.
 *   4. Selecting a small number of bins will act like a smoothing
 *      operator on the histogram.
 */
typedef struct hist_bin{
    int count;
    PIX_TYPE rangeStart;
    PIX_TYPE rangeEnd;
} HistogramBin;

void histEqual(Image im, int N){
    HistogramBin *bins = (HistogramBin*)malloc(sizeof(HistogramBin)*N);
    PIX_TYPE minVal, maxVal, binSize;
    PIX_TYPE totalPixels = im->width*im->height*im->channels;
    int x, y, c;

    assert(bins);

    /*************** find the minimum and maximum values ***************/
    minVal = IE(im,0,0,0);
    maxVal = IE(im,0,0,0);
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                minVal = MIN(minVal,IE(im,x,y,c));
                maxVal = MAX(maxVal,IE(im,x,y,c));
            }
        }
    }

    /********************** determine the bin size ********************/
    binSize = (maxVal - minVal) / N;

    /*************** Find probablity distribution ***************/
    /* initialize counts to zero */
    for(x = 0; x < N; x++){ bins[x].count = 0; }

    /* count individual pixels */
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                int tmp = TRUNC((IE(im,x,y,c)-minVal)/binSize); /* select bin */
                int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */
                bins[bin].count++;
            }
        }
    }

    /***************** Distribute bins *****************/
    c = 0; /* c is an accumulated count value */
    for(x = 0; x < N; x++){
        bins[x].rangeStart = c/totalPixels;
        c += bins[x].count;
        bins[x].rangeEnd = c/totalPixels;
    }

    /***************** create equalized image *************/
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                int tmp = TRUNC((IE(im,x,y,c)-minVal)/binSize); /* select bin */
                PIX_TYPE frac = ((IE(im,x,y,c)-minVal)/binSize) - tmp;
                int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */

                /* compute the linear interpolation for the final pixel value */
                IE(im,x,y,c) = bins[bin].rangeStart*(1.0-frac) + bins[bin].rangeEnd*(frac);
            }
        }
    }

    free(bins);
}


void histEqualMask(Image im, int N, const char **mask){
    HistogramBin *bins = (HistogramBin*)malloc(sizeof(HistogramBin)*N);
    PIX_TYPE minVal, maxVal, binSize;
    PIX_TYPE totalPixels = 0.0;
    int x, y, c;

    assert(bins);

    /*************** find the minimum and maximum values ***************/
    minVal = IE(im,0,0,0);
    maxVal = IE(im,0,0,0);
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[x][y]){
                    minVal = MIN(minVal,IE(im,x,y,c));
                    maxVal = MAX(maxVal,IE(im,x,y,c));
                    totalPixels+=1;
                }
            }
        }
    }

    /********************** determine the bin size ********************/
    binSize = (maxVal - minVal) / N;

    /*************** Find probablity distribution ***************/
    /* initialize counts to zero */
    for(x = 0; x < N; x++){ bins[x].count = 0; }

    /* count individual pixels */
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[x][y]){
                    int tmp = TRUNC((IE(im,x,y,c)-minVal)/binSize); /* select bin */
                    int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */
                    bins[bin].count++;
                }
            }
        }
    }

    /***************** Distribute bins *****************/
    c = 0; /* c is an accumulated count value */
    for(x = 0; x < N; x++){
        bins[x].rangeStart = c/totalPixels;
        c += bins[x].count;
        bins[x].rangeEnd = c/totalPixels;
    }

    /***************** create equalized image *************/
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                if(!mask || mask[x][y]){
                    int tmp = TRUNC((IE(im,x,y,c)-minVal)/binSize); /* select bin */
                    PIX_TYPE frac = ((IE(im,x,y,c)-minVal)/binSize) - tmp;
                    int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */

                    /* compute the linear interpolation for the final pixel value */
                    IE(im,x,y,c) = bins[bin].rangeStart*(1.0-frac) + bins[bin].rangeEnd*(frac);
                }
                else{
                    IE(im,x,y,c) = 0;
                }
            }
        }
    }

    free(bins);
}

void histEqualRect(Image im, int N, int top, int left, int bottom, int right){
    HistogramBin *bins = (HistogramBin*)malloc(sizeof(HistogramBin)*N);
    PIX_TYPE minVal, maxVal, binSize;
    PIX_TYPE totalPixels = im->width*im->height*im->channels;
    int x, y, c;

    assert(bins);
    assert(top >= 0);
    assert(left >= 0);
    assert(bottom <= im->height);
    assert(right <= im->width);
    /*************** find the minimum and maximum values ***************/
    minVal = IE(im,0,0,0);
    maxVal = IE(im,0,0,0);
    for(x = left; x < right; x++){
        for(y = top; y < bottom; y++){
            for(c = 0; c < im->channels; c++){
                minVal = MIN(minVal,IE(im,x,y,c));
                maxVal = MAX(maxVal,IE(im,x,y,c));
            }
        }
    }

    /********************** determine the bin size ********************/
    binSize = (maxVal - minVal) / N;

    /*************** Find probablity distribution ***************/
    /* initialize counts to zero */
    for(x = 0; x < N; x++){ bins[x].count = 0; }

    /* count individual pixels */
    for(x = left; x < right; x++){
        for(y = top; y < bottom; y++){
            for(c = 0; c < im->channels; c++){
                int tmp = TRUNC((IE(im,x,y,c)-minVal)/binSize); /* select bin */
                int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */
                bins[bin].count++;
            }
        }
    }

    /***************** Distribute bins *****************/
    c = 0; /* c is an accumulated count value */
    for(x = 0; x < N; x++){
        bins[x].rangeStart = c/totalPixels;
        c += bins[x].count;
        bins[x].rangeEnd = c/totalPixels;
    }

     /***************** Zero Center bins *****************/
    for(x = 0; x < N; x++){
        int tmp = TRUNC((0.0-minVal)/binSize); /* select bin */
        PIX_TYPE frac = ((0.0-minVal)/binSize) - tmp;
        int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */
        double zeroshift;
        zeroshift = bins[bin].rangeStart*(1.0-frac) + bins[bin].rangeEnd*(frac);

        bins[x].rangeStart -= zeroshift;
        bins[x].rangeEnd   -= zeroshift;
    }

   /***************** create equalized image *************/
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                int tmp = TRUNC((IE(im,x,y,c)-minVal)/binSize); /* select bin */
                PIX_TYPE frac = ((IE(im,x,y,c)-minVal)/binSize) - tmp;
                int bin = MAX(0, MIN(N-1, tmp)); /* Make sure value is in range */

                /* crop frac if it is out of bounds */
                /* compute the linear interpolation for the final pixel value */
                IE(im,x,y,c) = bins[bin].rangeStart*(1.0-frac) + bins[bin].rangeEnd*(frac);
            }
        }
    }

    free(bins);
}

/* shift the pixel values such that the pixels on the left top and right average 0.0 */
void ZeroBorder(Image im){
    int x; int y; int c;
    PIX_TYPE sum = 0.0;
    int count = 0;
    PIX_TYPE zero;

    assert(im);
    assert(im->width > 0);
    assert(im->height > 0);
    for(x = 0; x < im->width; x++){
        for(c = 0; c < im->channels; c++){
            sum += IE(im,x,0,c);
            count++;
        }
    }

    /* start from 1 so that the corners are not counted twice */
    for(y = 1; y < im->height; y++){
        for(c = 0; c < im->channels; c++){
            sum += IE(im,0,y,c);
            sum += IE(im,im->width - 1,y,c);
            count += 2;
        }
    }

    zero = sum / count;
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                IE(im,x,y,c) -= zero;
            }
        }
    }


}

/*	The gaussianNoise function will add zero mean Gaussian
	Random noise to each pixel. The second argument, noise,
	sets the standard deviation. Specifically, the standard
	deviation will be a fraction of the full dynamic range
	of the image where that fraction is determined by the
	noise arguement. Hence, a value of "0.1" for noise will
	set the standard deviation to be 10 percent of the total
	dynamic range in the image.
	*/

double gaussianNoise(Image im, double noise){
    double max;
    double min;
    double stddev;
    int x, y, c;

    /* compute the image range */
    max = IE(im,0,0,0);
    min = IE(im,0,0,0);

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                max = MAX(IE(im,x,y,c), max);
                min = MIN(IE(im,x,y,c), min);
            }
        }
    }

    stddev = noise*(max-min);
    /* Add gaussian noise */
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                double tmp = randBM()*stddev;
                IE(im,x,y,c) += tmp;
            }
        }
    }
    return stddev;
}


void applyMask(Image im, const char **mask){
    int x,y,c;

    if(!mask) return;

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                if(mask[x][y]){
                    /* do nothing */
                }
                else{
                    IE(im,x,y,c) = 0.0;
                }
            }
        }
    }

}

/**
 * Converts a grayscale image into a binary mask.
 * The pixel value midway between the minimum
 * and maximum in the source image is taken as
 * a cutoff. Values above this become 1 in the
 * mask, values below this become 0.
 *
 * @param im Source grayscale image
 * @param c Channel in source image to convert into mask
 * @returns The mask
 */
char**
imageToMask(Image im, int c){
  double max = imageMax(im,NULL);
  double min = imageMin(im,NULL);
  double threshold = (max + min)/2;
  char **mask;
  int i,j;

  mask = (char**) malloc(sizeof(char*)*im->width);
  assert(mask); /* problem allocating space for mask */
  
  for(i = 0; i < im->width; i++){
    mask[i] = (char*) malloc(sizeof(char)*im->height);
    assert(mask[i]); /* problem allocating space for mask */
  }
  
  for(j = 0; j < im->height; j++){
      for(i = 0; i < im->width; i++){
	if (IE(im,i,j,c) >= threshold)
	  mask[i][j] = 1;
	else
	  mask[i][j] = 0;
      }
  }
  
  return mask;
}

void markPoint(Image im, PIX_TYPE X, PIX_TYPE Y){
    PIX_TYPE mx = imageMax(im,NULL);
    /* PIX_TYPE mn = imageMin(im,NULL); */
    int x = ROUND(X);
    int y = ROUND(Y);
    int c;

    if(x - 4 < 0 || x + 4 >= im->width) return;
    if(y - 4 < 0 || y + 4 >= im->height) return;

    for(c = 0; c < im->channels; c++){
        IE(im,x-2,y,c) = mx;
        IE(im,x+2,y,c) = mx;
        IE(im,x-3,y,c) = mx;
        IE(im,x+3,y,c) = mx;
        IE(im,x-4,y,c) = mx;
        IE(im,x+4,y,c) = mx;
        IE(im,x,y-2,c) = mx;
        IE(im,x,y+2,c) = mx;
        IE(im,x,y-3,c) = mx;
        IE(im,x,y+3,c) = mx;
        IE(im,x,y-4,c) = mx;
        IE(im,x,y+4,c) = mx;
    }
}


Image makeThermalColorImage(Image im){
    int x, y;
    PIX_TYPE mx = imageMax(im,NULL);
    PIX_TYPE mn = imageMin(im,NULL);
    Image Thermal = makeImage(im->width, im->height, 3);
    assert(im->channels == 1);

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            PIX_TYPE value = (IE(im,x,y,0) - mn)/(mx-mn);
            if(value < 0.60){
                IE(Thermal,x,y,0) = 0;
                IE(Thermal,x,y,1) = 0;
                IE(Thermal,x,y,2) = exp(-50*(SQR(value-0.60)));
            } else if (value < 0.70){
                IE(Thermal,x,y,0) = 0;
                IE(Thermal,x,y,1) = (value-.60)/.10;
                IE(Thermal,x,y,2) = 1;
            } else if (value < 0.80){
                IE(Thermal,x,y,0) = 0;
                IE(Thermal,x,y,1) = 1;
                IE(Thermal,x,y,2) = (0.80-value)/.10;
            } else if (value < 0.90){
                IE(Thermal,x,y,0) = (value-.80)/.10;
                IE(Thermal,x,y,1) = 1;
                IE(Thermal,x,y,2) = 0;
            } else if (value < 1.0){
                IE(Thermal,x,y,0) = 1;
                IE(Thermal,x,y,1) = (1.0-value)/.10;
                IE(Thermal,x,y,2) = 0;
            } else {
                IE(Thermal,x,y,0) = 1;
                IE(Thermal,x,y,1) = 0;
                IE(Thermal,x,y,2) = 0;
            }
        }
    }
    return Thermal;
}

Image makeThermalComboImage(Image thrm, Image bw){
    int x, y, c;
    PIX_TYPE mxthrm = imageMax(thrm, NULL);
    PIX_TYPE mnthrm = imageMin(thrm, NULL);
    PIX_TYPE mxbw = imageMax(bw, NULL);
    PIX_TYPE mnbw = imageMin(bw, NULL);
    Image combo = makeImage(bw->width, bw->height, 3);

    assert(bw->channels == 1);
    assert(thrm->channels == 3);
    assert(thrm->width == bw->width);
    assert(thrm->height == bw->height);

    for(x = 0; x < bw->width; x++){
        for(y = 0; y < bw->height; y++){
            for(c = 0; c < 3; c++){
                IE(combo,x,y,c) = .5*(IE(thrm,x,y,c) - mnthrm)/(mxthrm-mnthrm)+.5*(IE(bw,x,y,0) - mnbw)/(mxbw-mnbw);
            }
        }
    }
    return combo;
}

/**
 Computes the maximum value of the pixels in an image. If a mask is
 provided only masked values are considered. If mask is NULL, all
 pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The maximum pixel value
*/
PIX_TYPE
imageMax(Image im, const char **mask)
{
    PIX_TYPE m = 0;
    int x, y = 0, c = 0;

    for(x = 0; x < im->width; x++)
      for(y = 0; y < im->height; y++)
	for(c = 0; c < im->channels; c++)
	  if ((!mask) || mask[x][y])
	    {
	      m = IE(im,x,y,c);
	      goto MAX_LOOP; /* Oh no! A goto! */
	    }

    DEBUG (1,"Warning: All pixels masked!");

    for (x = 0 ;x < im->width; x++)
      for (y = 0;y < im->height; y++)
	for (c = 0;c < im->channels; c++)
	  {
	  MAX_LOOP:
	    m = MAX (m,IE(im,x,y,c));
	  }

    return m;
}

/**
 Computes the minium value of the pixels in an image. If a mask is
 provided only masked values are considered. If mask is NULL, all
 pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The minimum pixel value
*/
PIX_TYPE
imageMin(Image im, const char **mask)
{
    PIX_TYPE m = 0;
    int x, y = 0, c = 0;

    for(x = 0; x < im->width; x++)
      for(y = 0; y < im->height; y++)
	for(c = 0; c < im->channels; c++)
	  if ((!mask) || mask[x][y])
	    {
	      m = IE(im,x,y,c);
	      goto MIN_LOOP; /* Argh! A goto! */
	    }

    DEBUG (1, "Warning: All pixels masked!");

    for (x = 0;x < im->width; x++)
        for (y = 0;y < im->height; y++)
            for (c = 0 ;c < im->channels; c++)
	      if ((!mask) || mask[x][y])
		{
		MIN_LOOP:
		  m = MIN (m,IE(im,x,y,c));
		}

    return m;
}

/**
 Computes the mean value of the pixels in an image. If a mask is
 provided only masked values are considered. If mask is NULL, all
 pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The mean pixel value
*/
PIX_TYPE
imageMean (Image im, const char **mask)
{
  int x, y, c, nPix = 0;
  PIX_TYPE sum = 0;

  for (x = 0; x < im->width; x++)
    for (y = 0; y < im->height; y++)
      for (c = 0; c < im->channels; c++)
	if ((!mask) || mask[x][y])
	  {
	    sum += IE(im,x,y,c);
	    nPix++;
	  }

  return sum / nPix;
}

/**
 Computes the variance of the value of the pixels in an image. If
 a mask is provided only masked values are considered. If mask is
 NULL, all pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The mean pixel value
*/
PIX_TYPE
imageVariance (Image im, const char **mask)
{
  int x, y, c, nPix = 0;
  PIX_TYPE sqsum = 0, mean = imageMean (im, mask);
  
  for (x = 0; x < im->width; x++)
    for (y = 0; y < im->height; y++)
      for (c = 0; c < im->channels; c++)
	if ((!mask) || mask[x][y])
	  {
	    nPix++;
	    sqsum += SQR (IE(im,x,y,c) - mean);
	  }

  return sqsum/(nPix - 1);
}

/**
 Computes the standard deviation of the values of the pixels in an
 image. If a mask is provided only masked values are considered.
 If mask is NULL, all pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The std-dev (square root of the variance) of the pixel values.
*/
PIX_TYPE
imageStdDev (Image im, const char **mask)
{
  return sqrt (imageVariance (im, mask));
}

/*
Helper function used by imageMedian
*/

int
pixelComparator (const void *aV, const void *bV)
{
  const PIX_TYPE *a = (const PIX_TYPE*) aV;
  const PIX_TYPE *b = (const PIX_TYPE*) bV;

  if (*a < *b) return -1;
  if (*a > *b) return 1;
  return 0;
}

/**
 Computes the median of the values of the pixels in an
 image. If a mask is provided only masked values are considered.
 If mask is NULL, all pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The median of the pixel values
*/
PIX_TYPE
imageMedian (Image im, const char **mask)
{
  int x, y, c, nPix = imageNumOfPixels (im, mask);
  PIX_TYPE *sortedPixels, *pp, median;

  /* Copy the pixel values into a big array */

  sortedPixels = (PIX_TYPE*) malloc (sizeof (PIX_TYPE) * nPix);

  pp = sortedPixels;
  for (x = 0; x < im->width; x++)
    for (y = 0; y < im->height; y++)
      for (c = 0; c < im->channels; c++)
	if ((!mask) || mask[x][y])
	  *pp++ = IE(im,x,y,c);

  /* Sort the result */

  qsort (sortedPixels, nPix, sizeof (PIX_TYPE), pixelComparator);

  median = sortedPixels [nPix/2];

  free (sortedPixels);

  return median;
}

/**
 Computes the MAD (median absolute deviation) of the values of
 the pixels in an image:

   median (abs (Y[i] - median (Y)))

 If a mask is provided only masked values
 are considered. If mask is NULL, all pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The MAD of the pixel values
*/
PIX_TYPE
imageMad (Image im, const char **mask)
{
  int x, y, c, nPix = imageNumOfPixels (im, mask);
  PIX_TYPE *sortedPixels, *pp, median, mad;

  /* Copy the pixel values into a big array */

  sortedPixels = (PIX_TYPE*) malloc (sizeof (PIX_TYPE) * nPix);

  pp = sortedPixels;
  for (x = 0; x < im->width; x++)
    for (y = 0; y < im->height; y++)
      for (c = 0; c < im->channels; c++)
	if ((!mask) || mask[x][y])
	  *pp++ = IE(im,x,y,c);

  /* Sort the result */

  qsort (sortedPixels, nPix, sizeof (PIX_TYPE), pixelComparator);

  median = sortedPixels [nPix/2];

  /* Now, subtract the median from the original pixel values */

  pp = sortedPixels;
  for (x = 0; x < im->width; x++)
    for (y = 0; y < im->height; y++)
      for (c = 0; c < im->channels; c++)
	if ((!mask) || mask[x][y])
	  *pp++ = fabs(IE(im,x,y,c) - median); 

  /* Sort the result */

  qsort (sortedPixels, nPix, sizeof (PIX_TYPE), pixelComparator);

  mad = sortedPixels [nPix/2];

  free (sortedPixels);

  return mad;
}

/**
 Computes the number of pixels in an image. If a mask is
 provided only masked values are considered. If mask is
 NULL, all pixels are counted.
 
 @param im An image
 @param mask A mask, or NULL
 @returns The number of pixel values in an image.
*/
int
imageNumOfPixels (Image im, const char **mask)
{
  int x, y, nPix = 0;

  if (!mask)
    nPix = im->width * im->height * im->channels;
  else
    {
      for (x = 0; x < im->width; x++)
	for (y = 0; y < im->height; y++)
	  if (mask[x][y])
	    nPix++;
      nPix *= im->channels;
    }

  return nPix;
}


void computeImageStatistics(ImageStatistics *is, Image im){
    int x,y,c;
    PIX_TYPE sum = 0, sqsum = 0;
    assert(is);

    is->min = is->max = IE(im, 0, 0, 0);
    is->min_x = is->min_y = is->min_c = is->max_x = is->max_y = is->max_c = 0;
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                sum += IE(im,x,y,c);

                if(is->min > IE(im,x,y,c)){
                    is->min = IE(im,x,y,c);
                    is->min_x = x; is->min_y = y; is->min_c = c;
                }
                if(is->max < IE(im,x,y,c)){
                    is->max = IE(im,x,y,c);
                    is->max_x = x; is->max_y = y; is->max_c = c;
                }
            }
        }
    }

    is->mean = sum/(im->width*im->height*im->channels);

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
                sqsum += SQR(IE(im,x,y,c)-is->mean);
            }
        }
    }

    is->variance = sqsum/(im->width*im->height*im->channels - 1);
    is->stddev   = sqrt(is->variance);
}

void computeImageStatisticsMask(ImageStatistics *is, Image im, const char **mask){
    int x,y,c, first = 1;
    PIX_TYPE sum = 0, sqsum = 0, nPix = 0;
    assert(is);

    is->min = is->max = IE(im, 0, 0, 0);
    is->min_x = is->min_y = is->min_c = is->max_x = is->max_y = is->max_c = 0;
    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
	      if(!mask || mask[x][y]){
                sum += IE(im,x,y,c);
		nPix++;
		
                if(first || is->min > IE(im,x,y,c)){
		  is->min = IE(im,x,y,c);
		  is->min_x = x; is->min_y = y; is->min_c = c;
                }
                if(first || is->max < IE(im,x,y,c)){
		  is->max = IE(im,x,y,c);
		  is->max_x = x; is->max_y = y; is->max_c = c;
                }
		first = 0;
	      }
            }
        }
    }

    is->mean = sum/nPix;

    for(x = 0; x < im->width; x++){
        for(y = 0; y < im->height; y++){
            for(c = 0; c < im->channels; c++){
	       if(!mask || mask[x][y]){
		 sqsum += SQR(IE(im,x,y,c) - is->mean);
	       }
            }
        }
    }

    is->variance = sqsum/(nPix - 1);
    is->stddev   = sqrt(is->variance);
}


void computeImageStatisticsLocal(ImageStatistics *is, Image im, int xt, int yt, int dist){
    int x,y,c;
    PIX_TYPE sum = 0, sqsum = 0;
    assert(is);

    dist = ABS(dist);
    is->min = is->max = ie(im, xt, yt, 0);
    is->min_x = is->max_x = xt;
    is->min_y = is->max_y = yt;
    is->min_c = is->max_c = 0;
    for(x = MAX(xt - dist,0); x <= MIN(xt+dist,im->width-1); x++){
        for(y = MAX(yt-dist,0); y <= MIN(yt+dist,im->height-1); y++){
            for(c = 0; c < im->channels; c++){
                sum += ie(im,x,y,c);

                if(is->min > ie(im,x,y,c)){
                    is->min = ie(im,x,y,c);
                    is->min_x = x; is->min_y = y; is->min_c = c;
                }
                if(is->max < ie(im,x,y,c)){
                    is->max = ie(im,x,y,c);
                    is->max_x = x; is->max_y = y; is->max_c = c;
                }
            }
        }
    }

    is->mean = sum/(2*dist*2*dist);

    for(x = MAX(xt - dist,0); x <= MIN(xt+dist,im->width-1); x++){
        for(y = MAX(yt-dist,0); y <= MIN(yt+dist,im->height-1); y++){
            for(c = 0; c < im->channels; c++){
                sqsum += SQR(ie(im,x,y,c)-is->mean);
            }
        }
    }

    is->variance = sqsum/(2*dist*2*dist-1);
    is->stddev   = sqrt(is->variance);
}




