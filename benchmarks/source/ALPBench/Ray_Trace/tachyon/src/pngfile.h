/*
 *  pngfile.h - This file deals with PNG format image files (reading/writing)
 *
 *  $Id: pngfile.h,v 1.1 2001/02/26 18:26:00 johns Exp $
 */ 

int readpng(char * name, int * xres, int * yres, unsigned char **imgdata);
int writepng(char * name, int xres, int yres, unsigned char *imgdata);
