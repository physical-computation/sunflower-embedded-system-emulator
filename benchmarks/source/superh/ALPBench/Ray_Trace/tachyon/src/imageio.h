/*
 *  imageio.h - This file deals with reading/writing image files 
 *
 *  $Id: imageio.h,v 1.5 1999/09/01 19:34:10 johns Exp $
 */ 

/* For our puposes, we're interested only in the 3 byte per pixel 24 bit
   truecolor sort of file.. */

#define IMAGENOERR     0  /* no error */
#define IMAGEBADFILE   1  /* can't find or can't open the file */
#define IMAGEUNSUP     2  /* the image file is an unsupported format */
#define IMAGEALLOCERR  3  /* not enough remaining memory to load this image */
#define IMAGEREADERR   4  /* failed read, short reads etc */
#define IMAGEWRITEERR  5  /* failed write, short writes etc */
#define IMAGENULLDATA  6  /* image to write was a null pointer */

int readimage(rawimage *);
int writeimage(char * name, int xres, int yres, 
               unsigned char *imgdata, int format);


