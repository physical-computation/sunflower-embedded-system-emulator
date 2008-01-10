/* 
 * fire.c 
 * This file contains the main program and driver for the raytracer.
 *
 *  $Id: fire.c,v 1.21 2002/07/03 04:35:58 johns Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tachyon.h"

#if defined(USEOMF)
#include "omfwrite.h"
  /* AVR26 Resolution  */
#define XRES 640 
#define YRES 496    
#else
  /* MPEG-1 Resolution  */
#define XRES 640
#define YRES 480
#endif
 
#define XS 96 
#define YS 96 
#define ZS 64 
#define NUMFLAMES 120 
#define MAXFRAMES 600
#define FIREOPACITY 14.0

#define RAD 6.28
#define MXV 255

typedef struct {      /* Scalar Volume Visualization Data */
  int loaded;            /* Volume data memory residence flag */
  int xres;              /* volume X axis size                */
  int yres;              /* volume Y axis size                */
  int zres;              /* volume Z axis size                */
  apiflt opacity;        /* opacity per unit length           */
  char name[96];         /* Volume data filename              */
  unsigned char * data;  /* pointer to raw byte volume data   */
} scalarvol;

typedef struct {
  int x;
  int y;
} flm;

#ifdef cube
#define RFILE "/cfs/johns/anim/frame"
#endif

#ifndef cube 
#define RFILE "frame"
#endif

int rt_mynode(void); /* proto */

unsigned char fire[2][XS*YS*ZS + 10000];
flm flames[NUMFLAMES + 10];

void updatefire(void) {
  int x, y, z, i;
  int xx, addr, addr2, addr3, addr4;

  for (i=0; i<NUMFLAMES; i++) {
    flames[i].x += (rand() % 7) - 3;  
    flames[i].y += (rand() % 7) - 3;  

    if (flames[i].x > (XS - 3)) 
      flames[i].x = 3;
    if (flames[i].x < 3) 
      flames[i].x = (XS - 3); 
    if (flames[i].y > (YS - 3)) 
      flames[i].y = 3;
    if (flames[i].y < 3) 
      flames[i].y = (YS - 3); 

    addr = (XS*flames[i].y) + flames[i].x; 
    fire[0][addr - 1     ] = MXV;
    fire[0][addr         ] = MXV;
    fire[0][addr + 1     ] = MXV;
    fire[0][addr - 1 - XS] = MXV;
    fire[0][addr     - XS] = MXV;
    fire[0][addr + 1 - XS] = MXV;
    fire[0][addr - 1 + XS] = MXV;
    fire[0][addr     + XS] = MXV;
    fire[0][addr + 1 + XS] = MXV;

    addr = addr + XS*YS;
    fire[0][addr - 1     ] = MXV;
    fire[0][addr         ] = MXV;
    fire[0][addr + 1     ] = MXV;
    fire[0][addr - 1 - XS] = MXV;
    fire[0][addr     - XS] = MXV;
    fire[0][addr + 1 - XS] = MXV;
    fire[0][addr - 1 + XS] = MXV;
    fire[0][addr     + XS] = MXV;
    fire[0][addr + 1 + XS] = MXV;

    addr = addr + XS*YS;
    fire[0][addr - 1 ] = MXV;
    fire[0][addr     ] = MXV;
    fire[0][addr + 1 ] = MXV;
    fire[0][addr + XS] = MXV;
    fire[0][addr - XS] = MXV;
  }

  for (z=0; z<(ZS - 1); z++) {
    addr3 = z*XS*YS;
    for (y=1; y<(YS - 1); y++) {
      addr2 = y*XS + addr3;
      for (x=1; x<(XS - 1); x++) { 
        addr = x + addr2;

/* NOTE:  This address code takes the 26 points in the box surrounding
          the original point, and averages them with together, to generate
          the new point.  If you desire speed, comment out everything 
          after the "first 6 points" 
          Remember to change the divisor on the line "xx = xx / 26"
*/

      xx = fire[0][addr + 1              ] + /* first 6 adjacent neighbors */
           fire[0][addr - 1              ] +
           fire[0][addr     + XS         ] +
           fire[0][addr     - XS         ] +
           fire[0][addr           + XS*YS] +
           fire[0][addr           - XS*YS] +  

           fire[0][addr - 1 + XS] +  /* middle 4 points */    
           fire[0][addr + 1 + XS] +      
           fire[0][addr - 1 - XS] +      
           fire[0][addr + 1 - XS];      

           addr4=addr - XS*YS;     /* bottom 8 points */
     xx += fire[0][addr4 - 1] +
           fire[0][addr4 + 1];
           addr4=addr + XS - XS*YS; 
     xx += fire[0][addr4 - 1] +  
           fire[0][addr4    ] +
           fire[0][addr4 + 1]; 
           addr4=addr - XS - XS*YS;
     xx += fire[0][addr4 - 1] +   
           fire[0][addr4    ] +
           fire[0][addr4 + 1];   

           addr4=addr + XS*YS;      /* top 8 points */
     xx += fire[0][addr4 - 1] +
           fire[0][addr4 + 1];
           addr4=addr + XS + XS*YS; 
     xx += fire[0][addr4 - 1] +  
           fire[0][addr4    ] +
           fire[0][addr4 + 1];  
           addr4=addr - XS + XS*YS;
     xx += fire[0][addr4 - 1] +
           fire[0][addr4    ] +
           fire[0][addr4 + 1];

        xx = xx / 26;

        if (xx > 1) 
          fire[1][addr + XS*YS] = (unsigned char) xx - 1;
        else
          fire[1][addr + XS*YS] = 0;   

      }
    }
  }
  memcpy(&fire[0], &fire[1], (XS*YS*ZS));
}

int main(int argc, char **argv) {
  SceneHandle scene;
  int i, antialiasing;
  apiflt zoom, aspectratio;
  apivector Ccenter, Cview, Cup;
  apitexture tex1, tex2;
  apitexture p1,p2;
  scalarvol vol;
  apiflt xc,yc;
  void * t1, * t2, * vt1, * lt1;
  char fname[1000];
  char fname2[2000];
#ifdef USEOMF
  int yy;
  unsigned char * rawimage;
  unsigned char * interlacedimage;
  void * omfhandle;
#endif

  rt_initialize(&argc, &argv); 

  scene = rt_newscene();
  
#ifdef USEOMF
  rawimage = malloc(XRES*YRES*4);
  interlacedimage = malloc(XRES*YRES*4);

  omfhandle = OMFopen(XRES, YRES, AVR26, "fire.omf");
#endif

  for (i=0; i<NUMFLAMES; i++) {   /* init the little flames */
    flames[i].x=rand() % XS;
    flames[i].y=rand() % YS; 
  }

  aspectratio=1.0;  
  antialiasing=0; 
  zoom=1.0;

  Ccenter = rt_vector(0.0, 0.0, 0.0);
  Cview = rt_vector(1.0, 0.0, 0.0);
  Cup = rt_vector(0.0, 0.0, 1.0); 

  tex1.col = rt_color(1.0, 1.0, 1.0); 
  tex1.ambient=1.0;  
  tex1.diffuse=0.0;
  tex1.specular=0.0; 
  tex1.opacity=FIREOPACITY;
  tex1.texturefunc=0;
  tex2=tex1;


  vol.loaded=1;
  vol.data=fire[1];  

  p1.col = rt_color(1.0, 1.0, 1.0);
  p1.ambient=0.3; 
  p1.diffuse=0.8; 
  p1.specular=0.0; 
  p1.opacity=1.0;

  p2=p1;

  p1.texturefunc=9;
  p1.ctr = rt_vector(0.0, 0.0, 0.0);
  p1.rot = rt_vector(0.0, 0.0, 0.0);
  p1.scale = rt_vector(0.1, 0.1, 0.1);
  p1.uaxs = rt_vector(1.0, 0.0, 0.0);
  p1.vaxs = rt_vector(0.0, 1.0, 0.0);

  strcpy((char *) &p1.imap, "/disk7/cube/imaps/leafy261.ppm");

  t1=rt_texture(scene, &p1);

  p2.ambient=0.1;
  p2.diffuse=0.1;
  p2.specular=0.8;

  t2=rt_texture(scene, &p2);
  vt1=rt_texture(scene, &tex2);
  lt1=rt_texture(scene, &tex1);

  rt_resolution(scene, XRES, YRES);
  rt_verbose(scene, 0);

  rt_light(scene, lt1, rt_vector(0.0, 0.0, 40.0), 1.0);
  rt_scalarvol(scene, vt1, rt_vector(-1.0, -1.0, -1.0), 
               rt_vector(1.0, 1.0, 1.0), XS, YS, ZS, "", &vol);   

  rt_plane(scene, t1, rt_vector(0.0, 0.0, -1.01), rt_vector(0.0, 0.0, 1.0));

  rt_sphere(scene, t2, rt_vector(0.0, 0.0, 1.8), 0.8); 
 
  for (i=0; i<MAXFRAMES; i++) {  
    sprintf(fname,".%4.4d.tga",i);
    strcpy(fname2,RFILE);
    strcat(fname2, fname);

#ifdef USEOMF
  rt_outputfile(scene, "fire.tga");
  rt_rawimage(scene, rawimage);
#else
  rt_outputfile(scene, fname2);
#endif
     
    if (rt_mynode()==0) printf("Rendering: %s \n",fname2);

    xc=cos(6.28 * i / (1.0 * MAXFRAMES));
    yc=sin(6.28 * i / (1.0 * MAXFRAMES));
    Ccenter.x = xc*3.2; Ccenter.y = yc*3.2;
    Cview.x =  -xc; Cview.y =  -yc;

    srand(i); /* reset random seed.. */
              /* this was what used to cause antialiased fire to choke.. */
    updatefire(); /* run one iteration of the fire code */

    rt_camera_setup(scene, zoom, aspectratio, antialiasing, 
		6, Ccenter, Cview, Cup);

    rt_renderscene(scene);

#ifdef USEOMF
    for (yy=0; yy<YRES; yy+=2) {
      memcpy(&interlacedimage[(yy>>1)*XRES*3], &rawimage[yy*XRES*3], XRES*3);
      memcpy(&interlacedimage[((yy>>1)+1)*XRES*3], &rawimage[(yy+1)*XRES*3], XRES*3);
    }

    OMFwriteframe(omfhandle, interlacedimage);
#endif
  }

#ifdef USEOMF
  OMFclose(omfhandle);
  free(rawimage);
  free(interlacedimage);
#endif

  rt_finalize(); 

  return 0;
}
   
