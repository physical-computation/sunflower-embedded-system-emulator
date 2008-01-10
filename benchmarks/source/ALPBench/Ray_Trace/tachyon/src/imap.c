/*
 * imap.c - This file contains code for doing image map type things.  
 *
 *  $Id: imap.c,v 1.23 2004/03/05 16:10:01 johns Exp $
 */

#include "machine.h"
#include "types.h"
#include "imap.h"
#include "util.h"
#include "parallel.h"
#include "imageio.h"
#include "ui.h"

/* XXX globals that must be eliminated! */
rawimage * imagelist[MAXIMGS];
int numimages;

void ResetImages(void) {
  int i;
  numimages=0;
  for (i=0; i<MAXIMGS; i++) {
    imagelist[i]=NULL;
  }
}

void FreeImages(void) {
#if 0
  int i; 
  for (i=0; i<numimages; i++) {
    DeallocateImage(imagelist[i]);
  }
#endif
  ResetImages();
}

void LoadImage(rawimage * image) {
  if (!image->loaded) {
    readimage(image);
    image->loaded=1;
  }
}

rawimage * AllocateImage(const char * filename) { 
  rawimage * newimage = NULL;
  int i, len, intable;

  intable=0;
  if (numimages!=0) {
    for (i=0; i<numimages; i++) {
      if (!strcmp(filename, imagelist[i]->name)) {
        newimage=imagelist[i];
        intable=1;
      }
    }
  }

  if (!intable) {
    newimage=malloc(sizeof(rawimage));
    newimage->loaded=0;
    newimage->xres=0;
    newimage->yres=0;
    newimage->bpp=0;
    newimage->data=NULL;
    len=strlen(filename);
    if (len > 80) 
      return NULL;
    strcpy(newimage->name, filename);

    imagelist[numimages]=newimage;  /* add new one to the table       */ 
    numimages++;                    /* increment the number of images */
  }
 
  return newimage;
}

rawimage * NewImage(int x, int y) {
  rawimage * newimage = NULL;
  newimage=malloc(sizeof(rawimage));
  if (newimage == NULL)
    return NULL;

  newimage->loaded=1;
  newimage->xres=x;
  newimage->yres=y;
  newimage->bpp=0;
  newimage->data=malloc(x*y*3);
  if (newimage->data == NULL) {
    free(newimage);
    return NULL;
  }

  return newimage;
}

void DeallocateImage(rawimage * image) {
  image->loaded=0;
  free(image->data);
}

void FreeMIPMap(mipmap * mip) {
  int i;
  for (i=0; i<mip->levels; i++) {
    DeallocateImage(mip->images[i]);
  } 
  free(mip->images);
  free(mip);
}

mipmap * LoadMIPMap(const char * filename) {
  rawimage * img;
  mipmap * mip;

  img = AllocateImage(filename);
  if (img == NULL)
    return NULL;

  LoadImage(img);

  mip = CreateMIPMap(img); 
  if (mip == NULL) {
    DeallocateImage(img);
    free(mip);
    return NULL;
  }

  return mip;
}

rawimage * DecimateImage(const rawimage * image) {
  rawimage * newimage;
  int x, y, addr, addr2;

  x = (int) image->xres >> 1;
  if (x == 0)
    x = 1;

  y = (int) image->yres >> 1;
  if (y == 0)
    y = 1;

  newimage = NewImage(x, y);

  if (image->xres > 1 && image->yres > 1) {
    for (y=0; y<newimage->yres; y++) {
      for (x=0; x<newimage->xres; x++) {
        addr = (newimage->xres*y + x)*3;
        addr2 = (image->xres*y + x)*3*2;
        newimage->data[addr] = (int)
          (image->data[addr2] + 
           image->data[addr2 + 3] +
           image->data[addr2 + image->xres*3] + 
           image->data[addr2 + (image->xres + 1)*3]) >> 2; 
        addr++;
        addr2++;
        newimage->data[addr] = (int)
          (image->data[addr2] + 
           image->data[addr2 + 3] +
           image->data[addr2 + image->xres*3] + 
           image->data[addr2 + (image->xres + 1)*3]) >> 2; 
        addr++;
        addr2++;
        newimage->data[addr] = (int)
          (image->data[addr2] + 
           image->data[addr2 + 3] +
           image->data[addr2 + image->xres*3] + 
           image->data[addr2 + (image->xres + 1)*3]) >> 2; 
      }
    }
  }
  else if (image->xres == 1) {
    for (y=0; y<newimage->yres; y++) {
      addr = y*3;
      addr2 = y*3*2;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
    }
  }
  else if (image->yres == 1) {
    for (x=0; x<newimage->xres; x++) {
      addr = x*3;
      addr2 = x*3*2;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
    }
  }

  return newimage;
}

mipmap * CreateMIPMap(rawimage * image) {
  mipmap * mip;
  int xlevels, ylevels, i; 
  char msgtxt[1024];
  
  if (image == NULL) 
    return NULL;

  mip = (mipmap *) malloc(sizeof(mipmap));
  if (mip == NULL)
    return NULL;

  xlevels = 0;  
  i = abs(image->xres);
  while (i) {
    i >>= 1; 
    xlevels++;
  }
  
  ylevels = 0;  
  i = abs(image->yres);
  while (i) {
    i >>= 1; 
    ylevels++;
  }

  mip->levels = (xlevels > ylevels) ? xlevels : ylevels; 

  sprintf(msgtxt, "Creating MIP Map: xlevels: %d  ylevels: %d  levels: %d", 
          xlevels, ylevels, mip->levels);
  rt_ui_message(MSG_0, msgtxt);

  mip->images = (rawimage **) malloc(mip->levels * sizeof(rawimage *)); 
  if (mip->images == NULL) {
    free(mip);
    return NULL;
  }
     
  for (i=0; i<mip->levels; i++) {
    mip->images[i] = NULL;
  } 

  mip->images[0] = image;
  for (i=1; i<mip->levels; i++) {
    mip->images[i] = DecimateImage(mip->images[i - 1]);
  }

  return mip;
}

color MIPMap(const mipmap * mip, flt u, flt v, flt d) {
  int mapindex;
  flt mapflt;
  color col, col1, col2;

  if ((u <= 1.0) && (u >=0.0) && (v <= 1.0) && (v >= 0.0)) {
    if (d > 1.0) 
      d = 1.0;
    else if (d < 0.0)
      d = 0.0;

    mapflt = d * (mip->levels - 0.9999); /* convert range to mapindex        */
    mapindex = (int) mapflt;             /* truncate to nearest integer      */
    mapflt = mapflt - mapindex;          /* fractional part of mip map level */

    /* interpolate between two nearest image maps */
    if (mapindex < (mip->levels - 1)) {
      col1 = ImageMap(mip->images[mapindex    ], u, v);
      col2 = ImageMap(mip->images[mapindex + 1], u, v);
      col.r = col1.r + mapflt*(col2.r - col1.r);
      col.g = col1.g + mapflt*(col2.g - col1.g);
      col.b = col1.b + mapflt*(col2.b - col1.b);
    }
    else {
      /* if mapindex is too high, use the highest,  */
      /* with no MIP-Map interpolation.             */
      col  = ImageMap(mip->images[mip->levels - 1], u, v);
    }
  } 
  else {
    col.r=0.0;
    col.g=0.0;
    col.b=0.0;
  }

  return col;
}

color ImageMap(const rawimage * image, flt u, flt v) {
  color col, colx, colx2;
  flt x, y, px, py;
  int ix, iy;
  unsigned char * ptr;

  /*
   *  Perform bilinear interpolation between 4 closest pixels.
   */
  x = (image->xres - 1.0) * u; /* floating point X location */
  ix = (int) x;                /* integer X location        */
  px = x - ix;                 /* fractional X location     */

  y = (image->yres - 1.0) * v; /* floating point Y location */
  iy = (int) y;                /* integer Y location        */
  py = y - iy;                 /* fractional Y location     */

  /* pointer to the left lower pixel */
  ptr  = image->data + ((image->xres * iy) + ix) * 3; 

  /* interpolate between left and right lower pixels */
  colx.r = (flt) ((flt)ptr[0] + px*((flt)ptr[3] - (flt) ptr[0])); 
  colx.g = (flt) ((flt)ptr[1] + px*((flt)ptr[4] - (flt) ptr[1])); 
  colx.b = (flt) ((flt)ptr[2] + px*((flt)ptr[5] - (flt) ptr[2])); 

  /* pointer to the left upper pixel */
  ptr  += image->xres * 3; 

  /* interpolate between left and right upper pixels */
  colx2.r = ((flt)ptr[0] + px*((flt)ptr[3] - (flt)ptr[0])); 
  colx2.g = ((flt)ptr[1] + px*((flt)ptr[4] - (flt)ptr[1])); 
  colx2.b = ((flt)ptr[2] + px*((flt)ptr[5] - (flt)ptr[2])); 

  /* interpolate between upper and lower interpolated pixels */
  col.r = (colx.r + py*(colx2.r - colx.r)) / 255.0;
  col.g = (colx.g + py*(colx2.g - colx.g)) / 255.0;
  col.b = (colx.b + py*(colx2.b - colx.b)) / 255.0;

  return col;
} 


