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

#ifndef CSU_IMAGE_INCLUDED
#define CSU_IMAGE_INCLUDED

#define PIX_TYPE double
#define RASTER_ID "CSU_SFI"

/* #define INTERPOLATE_FAST 1 */

typedef struct{
    int width;
    int height;
    int channels;
    PIX_TYPE*** data;
} image;

typedef image* Image;

Image makeImage(int width,int height,int channels);
Image makeZeroImage(int width,int height,int channels);
void freeImage(Image i);

Image duplicateImage(Image im);



/********************* Image Access Functions *******************/

double randBM();

/* Access a single image element using dope vector expantion. symantics: img[x,y,c] */
#define IE( img , x , y , c ) 	( (img)->data[(x)][(y)][(c)] )

/* Safe read-only/range-checked implementation of IE - returns 0.0 if off image. */
#if(!INTERPOLATE_FAST)
PIX_TYPE ie(Image img, int x, int y, int c);
#else
#define ie( img , x , y , c ) ((x < img->width && y < img->height && c < img->channels && x >= 0 && y >=  0 && c >= 0) ? IE(img, x, y, c) : 0.0 )
#endif
/* interpolate a pixel value at a point */


#if(!INTERPOLATE_FAST)
PIX_TYPE interpLinear(Image img, PIX_TYPE x, PIX_TYPE y, int c);
#else
/* Error in Code #define interpLinear(img, x, y, c) ; */
#endif

/********************* Image Manipulation Functions *******************/

Image convolveImage(const Image im, const Image mask);

double convolvePoint(PIX_TYPE x, PIX_TYPE y, int c, const Image im, const Image mask);

void contrastImage(Image im, double scalar);
    /* Multiplies everyvalue in im by scalar */

void brightnessImage(Image im, double scalar);
    /* Adds scalar to every value in im */

void ZeroMeanUnitLength(Image im);
/* scales the pixel so that they have zero mean and the image is unit length*/
void ZeroMeanUnitLengthMasked(Image im, const char **mask);
/* scales the pixel so that they have zero mean and the image is unit length*/

void ZeroMeanOneStdDev(Image im);
/* scales the pixel so that they have zero mean and the image is unit length*/
void ZeroMeanOneStdDevMasked(Image im, const char **mask);
/* scales the pixel so that they have zero mean and the image is unit length*/

double correlateImages(const Image i1, const Image i2);
/* findes the correlation between two images */
double dotImages(const Image i1, const Image i2);
/* find the correlation for images that have zero mean and unit length */

Image computeMeanImage(Image* images, int count);
/* takes an array of images and returns the mean of those images.*/

Image accumulateChannels(const Image im);
/* generates a new image that is the sum of all channels. */

void accumulateImages(Image i1, const Image i2);
/* inplace adds image i2 to image i1 */

void gaussianBlur(Image im, double sigma);

/* Add gausian noise to the image. s is the standard
 * deviation relative to the total image range returns the actual standard deviation
 * of the applied noise
 */
double gaussianNoise(Image im, double s);

/* Performs a histogram equalization function.  Output values are in the range 0 to 1 */

void histEqual(Image i, int num_bins);
void histEqualMask(Image i, int num_bins, const char **mask);
void histEqualRect(Image geo, int num_bins, int top, int left, int bottom, int right);

/* shift the pixel values such that the pixels on the left top and right average 0.0 */
void ZeroBorder(Image im);

/* Smooth image borders */
void smoothImageEdge(Image i, int width);

/* Zero all pixels not in a mask */
void applyMask(Image im, const char **mask);

/* Converts a grayscale image into a mask */
char** imageToMask(Image im, int c);

PIX_TYPE imageMax(Image im, const char **mask);
PIX_TYPE imageMin(Image im, const char **mask);
PIX_TYPE imageMean (Image im, const char **mask);
PIX_TYPE imageVariance (Image im, const char **mask);
PIX_TYPE imageStdDev (Image im, const char **mask);
PIX_TYPE imageMedian (Image im, const char **mask);
PIX_TYPE imageMad (Image im, const char **mask);
int imageNumOfPixels (Image im, const char **mask);

void markPoint(Image im, PIX_TYPE x, PIX_TYPE y);

Image makeThermalColorImage(Image im);

Image makeThermalComboImage(Image thrm, Image bw);


/********************* Image File I/O Functions *******************/

Image readPGMImage(const char*);
Image readRawImage(const char*);
Image readFeretImage(const char *fname, int width, int height,int channels);

void  writePGMImage(const Image, const char*,int channel);
void  writePPMImage(const Image, const char*);
void  writeRawImage(const Image, const char*);
void  writeFeretImage(const Image im, const char *fname);

/********************* Image Statistics ************************************/

typedef struct{
    PIX_TYPE max, min, mean, variance, stddev;
    int max_x, max_y, max_c, min_x, min_y, min_c;
} ImageStatistics;

void computeImageStatistics(ImageStatistics *is, Image im);
void computeImageStatisticsMask(ImageStatistics *is, Image im, const char **mask);
void computeImageStatisticsLocal(ImageStatistics *is, Image im, int x, int y, int dist);

#endif
