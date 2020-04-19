/*
 *  imageio.c - This file deals with reading/writing image files
 *
 *  $Id: imageio.c,v 1.14 2001/02/26 18:37:49 johns Exp $
 */ 

/* For our puposes, we're interested only in the 3 byte per pixel 24 bit
 * truecolor sort of file..
 */

#include <stdio.h>
#include "machine.h"
#include "types.h"
#include "util.h"
#include "imageio.h"
#include "ppm.h"     /* 24-bit PPM files */
#include "tgafile.h" /* 24-bit Truevision Targa files */
#include "jpeg.h"    /* JPEG files */
#include "pngfile.h" /* PNG files  */
#include "sgirgb.h"  /* 24-bit SGI RGB files */
#include "winbmp.h"  /* 24-bit Windows Bitmap files */
#include "ui.h"      /* UI error messages */

static 
int fakeimage(char * name, int * xres, int * yres, unsigned char ** imgdata) {
  int i, imgsize;
  char msgtxt[2048];
  sprintf(msgtxt, "Error loading image %s.  Faking it using solid gray.", name);
  rt_ui_message(MSG_0, msgtxt);
   
  *xres = 4;
  *yres = 4;
  imgsize = 3 * (*xres) * (*yres);
  *imgdata = malloc(imgsize);
  for (i=0; i<imgsize; i++) {
    (*imgdata)[i] = 255;
  }

  return IMAGENOERR;
}


int readimage(rawimage * img) {
  int rc;
  int xres, yres;
  unsigned char * imgdata;
  char * name = img->name;
  char msgtxt[2048];

  if (strstr(name, ".ppm")) { 
    rc = readppm(name, &xres, &yres, &imgdata);
  }
  else if (strstr(name, ".tga")) {
    rc = readtga(name, &xres, &yres, &imgdata);
  }
  else if (strstr(name, ".jpg")) {
    rc = readjpeg(name, &xres, &yres, &imgdata);
  }
  else if (strstr(name, ".png")) {
    rc = readpng(name, &xres, &yres, &imgdata);
  }
  else if (strstr(name, ".gif")) {
    rc = IMAGEUNSUP; 
  }
  else if (strstr(name, ".tiff")) {
    rc = IMAGEUNSUP; 
  }
  else if (strstr(name, ".rgb")) {
    rc = IMAGEUNSUP; 
  }
  else if (strstr(name, ".xpm")) {
    rc = IMAGEUNSUP; 
  }
  else {
    rc = readppm(name, &xres, &yres, &imgdata);
  } 

  switch (rc) {
    case IMAGEREADERR:
      sprintf(msgtxt, "Short read encountered while loading image %s", name);
      rt_ui_message(MSG_0, msgtxt);
      rc = IMAGENOERR; /* remap to non-fatal error */
      break;

    case IMAGEUNSUP:
      sprintf(msgtxt, "Cannot read unsupported format for image %s", name);
      rt_ui_message(MSG_0, msgtxt);
      break;
  }    

  /* If the image load failed, create a tiny white colored image to fake it */ 
  /* this allows a scene to render even when a file can't be loaded */
  if (rc != IMAGENOERR) {
    rc = fakeimage(name, &xres, &yres, &imgdata);
  }

  /* If we succeeded in loading the image, return it. */
  if (rc == IMAGENOERR) { 
    img->xres = xres;
    img->yres = yres;
    img->bpp = 3;  
    img->data = imgdata;
  }

  return rc;
}
 

int writeimage(char * name, int xres, int yres, unsigned char *imgdata, 
               int format) {
  if (imgdata == NULL) 
    return IMAGENULLDATA;

  switch (format) {
    case RT_FORMAT_PPM:
      return writeppm(name, xres, yres, imgdata);
    
    case RT_FORMAT_SGIRGB:
      return writergb(name, xres, yres, imgdata);

    case RT_FORMAT_JPEG:
      return writejpeg(name, xres, yres, imgdata);

    case RT_FORMAT_PNG:
      return writepng(name, xres, yres, imgdata);

    case RT_FORMAT_WINBMP:
      return writebmp(name, xres, yres, imgdata);

    case RT_FORMAT_TARGA:
    default:
      return writetga(name, xres, yres, imgdata);       
  } 
}


