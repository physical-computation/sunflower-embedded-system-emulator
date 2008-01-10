/*
 *  tgatoyuv.c - converts .tga files to Abekas YUV format .yuv files..
 *
 *   $Id: tgatoyuv.c,v 1.3 1998/05/01 01:14:50 johns Exp $
 */ 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef MPI
#include <mpi.h>
#endif

/* ONLY HANDLES 352x240 images!!! */

#define TGASZ 253440
#define YUVSZ 168960

void readtga(char * fname, unsigned char * mem) {
  unsigned char hdr[40];
  FILE * ifp;

  ifp = fopen(fname, "r");
  if (ifp == NULL) { 
    printf("Error opening %s for input!\n", fname);
    return;
  }

  fread(hdr, 18, 1, ifp); /* read in tga file header */   
  fread(mem, TGASZ, 1, ifp);

  fclose(ifp);
}

void writeyuv(char *fname, unsigned char * data) {
  FILE * ofp;
   
  ofp = fopen(fname, "w+b");

  fwrite(data, YUVSZ, 1, ofp);

  fclose(ofp); 
}

void bgrtoyuv(unsigned char * bgr, unsigned char *yuv) {
  int z,t;
  unsigned char r,g,b;
  unsigned long y1,y2,u,v,u0,u1,u2,v0,v1,v2;
  unsigned char * yuvptr;

  y1=y2=u=v=u0=u1=u2=v0=v1=v2=0;

  yuvptr = yuv;
  z=0;
  for (t=0; t<(TGASZ / 3); t+=2) {

    /* first pel (gives Y and half of chroma) */
    b = bgr[z*3    ]; /* blue  */
    g = bgr[z*3 + 1]; /* green */
    r = bgr[z*3 + 2]; /* red   */

    y1 = 16829 * r + 33039 * g + 6416 * b + (0xffff & y2);
    u1 = -4853 * r - 9530 * g + 14383 * b;
    v1 = 14386 * r - 12046 * g - 2340 * b;
 
    /* second pel (gives Y and quarter U and quarter V) */
    z++;
    b = bgr[z*3    ]; /* blue  */
    g = bgr[z*3 + 1]; /* green */
    r = bgr[z*3 + 2]; /* red   */
    y2 = 16829 * r + 33039 * g + 6416 * b + (0xffff & y1);
    u2 = -2426 * r - 4765 * g + 7191 * b;
    v2 = 7193 * r - 6023 * g - 1170 * b;
 
    /* filter the chroma */
    u = u0 + u1 + u2 + (0xffff & u);
    v = v0 + v1 + v2 + (0xffff & v);

    u0 = u2;
    v0 = v2;
 
    *yuvptr++ = (unsigned char) ((unsigned long) (u >> 16) + 128);
    *yuvptr++ = (unsigned char) ((unsigned long) (y1 >> 16) + 16);
    *yuvptr++ = (unsigned char) ((unsigned long) (v >> 16) + 128);
    *yuvptr++ = (unsigned char) ((unsigned long) (y2 >> 16) + 16);
   
    /* next pel...*/ 
    z++;
  }
}

int main (int argc, char **argv) {
  int fs, fe;
  int i;
  unsigned char *tgadata, *yuvdata;
  char tganame[800];
  char yuvname[800];
  int numnodes;
  int mynode;

  mynode=0;
  numnodes=1;

#ifdef MPI
  MPI_Init(&argc, &argv); /* initialize MPI */

  MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
  MPI_Comm_size(MPI_COMM_WORLD, &numnodes);

  if (mynode == 0) 
    printf("Parallel tga to yuv converter\n");

  MPI_Barrier(MPI_COMM_WORLD);
#endif

  tgadata = malloc(TGASZ);
  yuvdata = malloc(YUVSZ);

  if (mynode == 0)  
    printf("Converting 352x240 .tga files to abekas YUV files...\n");

  if (argc != 4) { 
    if (mynode == 0)  
      printf("Usage is: tgatoyuv basename  startframe#  endframe# \n");
    exit(0);
  }

  if (mynode == 0) 
    printf("Using %s as base filename for frames %s -> %s\n", 
					argv[1], argv[2], argv[3]);

  fs = atoi(argv[2]);
  fe = atoi(argv[3]); 
  
  for (i=(fs + mynode); i<=fe; i+=numnodes) {
    sprintf(tganame, "%s.%04d.tga", argv[1], i);
    sprintf(yuvname, "%s.%04d.yuv", argv[1], i);
    printf("converting %s -> %s . . .\n", tganame, yuvname);

    readtga(tganame, tgadata);
    bgrtoyuv(tgadata, yuvdata);
    writeyuv(yuvname, yuvdata);     
  }

#ifdef MPI
  MPI_Barrier(MPI_COMM_WORLD);

  if (mynode == 0) 
    printf("All nodes complete, all images converted.\n");

  MPI_Finalize(); /* close MPI */
#endif

  return 0;
}

