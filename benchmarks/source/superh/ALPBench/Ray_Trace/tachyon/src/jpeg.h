/*
 *  jpeg.h - This file deals with JPEG format image files (reading/writing)
 *
 *  $Id: jpeg.h,v 1.2 1999/09/01 16:01:09 johns Exp $
 */ 

int readjpeg(char * name, int * xres, int * yres, unsigned char **imgdata);
int writejpeg(char * name, int xres, int yres, unsigned char *imgdata);
