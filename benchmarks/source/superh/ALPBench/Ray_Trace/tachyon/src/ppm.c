/*
 *  ppm.c - This file deals with PPM format image files (reading/writing)
 *
 *  $Id: ppm.c,v 1.15 2004/05/30 22:49:41 johns Exp $
 */ 

/* For our puposes, we're interested only in the 3 byte per pixel 24 bit
   truecolor sort of file..  Probably won't implement any decent checking
   at this point, probably choke on things like the # comments.. */

#include <stdio.h>
#include "machine.h"
#include "types.h"
#include "util.h"
#include "imageio.h" /* error codes etc */
#include "ppm.h"

static int getint(FILE * dfile) {
  char ch[256];
  int i;
  int num;

  num=0; 
  while (num==0) {
    fscanf(dfile, "%s", ch);
      while (ch[0]=='#') {
        fgets(ch, 256, dfile);
      }
    num=sscanf(ch, "%d", &i);
  }
  return i;
}

int readppm(char * name, int * xres, int * yres, unsigned char **imgdata) {
  char data[256];  
  FILE * ifp;
  int i, bytesread;
  int datasize;
 
  ifp=fopen(name, "r");  
  if (ifp==NULL) {
    return IMAGEBADFILE; /* couldn't open the file */
  }
  fscanf(ifp, "%s", data);
 
  if (strcmp(data, "P6")) {
     fclose(ifp);
     return IMAGEUNSUP; /* not a format we support */
  }

  *xres=getint(ifp);
  *yres=getint(ifp);
      i=getint(ifp); /* eat the maxval number */
  fread(&i, 1, 1, ifp); /* eat the newline */ 
  datasize = 3 * (*xres) * (*yres);

  *imgdata=malloc(datasize); 

  bytesread=fread(*imgdata, 1, datasize, ifp);   

  fclose(ifp);

  if (bytesread != datasize) 
    return IMAGEREADERR;
  
  return IMAGENOERR;
}

int writeppm(char *name, int xres, int yres, unsigned char *imgdata) {
  FILE * ofp;
  int i;
 
  ofp=fopen(name, "wb");
  if (ofp==NULL) {
    return IMAGEBADFILE;
  }

  fprintf(ofp, "P6\n");
  fprintf(ofp, "%d %d\n", xres, yres);
  fprintf(ofp, "255\n"); /* maxval */

  for (i=0; i<yres; i++) {
    fwrite(&imgdata[(yres - i - 1)*3*xres], 1, (3*xres), ofp);
  }

  fclose(ofp);

  return IMAGENOERR;
}

