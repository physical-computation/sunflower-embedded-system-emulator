/* 
 * fire.c 
 * This file contains the main program and driver for the raytracer.
 *
 *  $Id: hypertex.c,v 1.13 2002/07/03 04:59:29 johns Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tachyon.h"

int rt_mynode(void); /* proto */

#define XRES 352
#define YRES 240

/* MPEG-1 Resolution  */
/*
 *#define XRES 352 
 *#define YRES 240    
 */
 
/* Pro Video Full Frame NTSC 
 *  #define XRES 768
 *  #define YRES 576
 */
 
/* PC VGA, and NTSC Resolution  
 * #define XRES 640
 * #define YRES 480
 */

#define MAXFRAMES 60
#define OPACITY 4.0

#define RAD 6.28

typedef struct {      /* Scalar Volume Visualization Data */
  int loaded;            /* Volume data memory residence flag */
  int xres;              /* volume X axis size                */
  int yres;              /* volume Y axis size                */
  int zres;              /* volume Z axis size                */
  apiflt opacity;        /* opacity per unit length           */
  char name[80];         /* Volume data filename              */
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

/* global frame number */
static int framenumber=0;

apiflt hypertex1(double x, double y, double z) {
  double value, dist;
  double xx, yy, zz;

  xx = x - 0.5;
  yy = y - 0.5;
  zz = z - 0.5;

  dist = sqrt(xx*xx + yy*yy + zz*zz);  
 
  if (dist < 0.35) {
    value = 1.0;
  }
  else {
    value = 0.0;
  }

  return value; 
}
   

apiflt hypertex2(double x, double y, double z) {
  double xx, yy, zz;
  double dist;
  double value;

  xx = x - 0.5; 
  yy = y - 0.5;
  zz = z - 0.5;

  dist = sqrt(xx*xx + yy*yy + zz*zz) + (framenumber / 200);

  value = (1.0 + cos(dist * 48.0) / (2*dist + 1.0)) / 2.0;

  value = value*value*value; 

  return value;
}

int main(int argc, char **argv) {
  SceneHandle scene;
  int i;
  int raydepth, antialiasing, verbosemode;
  apiflt zoom, aspectratio;
  apivector Ccenter, Cview, Cup;
  apivector ctr1;
  apitexture tex1, tex2;
  apivector min3, max3;
  apitexture p1;
  apivector ct1, n1;
  apiflt xc,yc;
  void * t1;
  char fname[1000];
  char fname2[2000];

  rt_initialize(&argc, &argv); 

  scene = rt_newscene();

  aspectratio=1.0; raydepth=8;
  antialiasing=0; verbosemode=0; zoom=1.0;
 
  Ccenter.x=0.0; Ccenter.y=0.0; Ccenter.z=0.0;
  Cview.x=1.0;   Cview.y=0.0;   Cview.z=0.0;
  Cup.x=0.0;     Cup.y=0.0;     Cup.z=1.0;
  ctr1.x=2.0;  ctr1.y=2.0; ctr1.z=2.0;
  
  tex1.col.r=1.0; tex1.col.g=1.0; tex1.col.b=1.0;
  tex1.ambient=0.5;  
  tex1.diffuse=1.0;
  tex1.specular=0.0; 
  tex1.opacity=OPACITY;
  tex1.texturefunc=0;
  tex2=tex1;

  min3.x=-1.0; min3.y=-1.0; min3.z=-1.0;
  max3.x=1.0;  max3.y=1.0;  max3.z=1.0;

  p1.col.r=1.0; p1.col.g=1.0; p1.col.b=1.0;
  p1.ambient=0.3; p1.diffuse=0.8; p1.specular=0.0; p1.opacity=1.0;

  p1.texturefunc=9;
    p1.ctr.x=0.0;   p1.ctr.y=0.0;   p1.ctr.z=0.0;
    p1.rot.x=0.0;   p1.rot.y=0.0;   p1.rot.z=0.0;
  p1.scale.x=0.1; p1.scale.y=0.1; p1.scale.z=0.1; 
   p1.uaxs.x=1.0;  p1.uaxs.y=0.0;  p1.uaxs.z=0.0;
   p1.vaxs.x=0.0;  p1.vaxs.y=1.0;  p1.vaxs.z=0.0;
#ifdef cube
  strcpy((char *) &p1.imap, "/cfs/johns/imaps/leafy261.ppm");
#endif
#ifndef cube
  strcpy((char *) &p1.imap, "/disk7/cube/imaps/leafy261.ppm");
#endif  

  t1=rt_texture(scene, &p1);

  for (i=0; i<MAXFRAMES; i++) {  
    rt_initialize(&argc, &argv); 

    sprintf(fname,".%4.4d.tga",i);
    strcpy(fname2,RFILE);
    strcat(fname2, fname);
    
    framenumber++;
 
    if (rt_mynode()==0) printf("Rendering: %s \n",fname2);

    xc=cos(6.28 * (i + 200) / (1.0 * MAXFRAMES));
    yc=sin(6.28 * (i + 200) / (1.0 * MAXFRAMES));
    Ccenter.x = xc*5.2; 
    Ccenter.y = yc*5.2;
    Cview.x =  -xc; 
    Cview.y =  -yc;

    rt_outputfile(scene, fname2);
    rt_resolution(scene, XRES, YRES);
    rt_verbose(scene, verbosemode);

    rt_camera_setup(scene, zoom, aspectratio, antialiasing, 
                    raydepth, Ccenter, Cview, Cup);

    rt_light(scene, rt_texture(scene, &tex1), ctr1, 1.0); 

    rt_extvol(scene, rt_texture(scene, &tex2), min3, max3, 64, hypertex1); 

    ct1.x=0.0; ct1.y=0.0; ct1.z=-1.01;
    n1.x=0.0; n1.y=0.0; n1.z=1.0;
    rt_plane(scene, t1, ct1, n1);  

    ct1.x=0.0; ct1.y=1.8; ct1.z=1.8;

    rt_renderscene(scene);
  }
  rt_finalize(); 

  return 0;
}
   
