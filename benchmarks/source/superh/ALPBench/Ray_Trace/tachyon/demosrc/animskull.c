/* main.c 
 * This file contains the main program and driver for the raytracer.
 *
 *  $Id: animskull.c,v 1.11 2002/07/03 04:59:28 johns Exp $
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tachyon.h"

int rt_mynode(void); /* proto */

typedef struct {      /* Scalar Volume Visualization Data */
  int loaded;            /* Volume data memory residence flag */
  int xres;              /* volume X axis size                */
  int yres;              /* volume Y axis size                */
  int zres;              /* volume Z axis size                */
  apiflt opacity;        /* opacity per unit length           */
  char name[80];         /* Volume data filename              */
  unsigned char * data;  /* pointer to raw byte volume data   */
} scalarvol;

#define MAXFRAMES 100
#define RAD 6.28

#ifdef cube
#define RFILE "/cfs/johns/anim/frame"
#endif

#ifndef cube 
#define RFILE "outfile"
#endif

int main(int argc, char **argv) {
  SceneHandle scene;
  int i;
  int xres, yres, raydepth, antialiasing, verbosemode;
  apiflt zoom, aspectratio;
  apivector Ccenter, Cview, Cup;
  apivector ctr1;
  apitexture tex1, tex2;
  apivector min3, max3;
  scalarvol vol;
  apiflt xc,yc;
  char fname[1000];
  char fname2[2000];

  rt_initialize(&argc, &argv); 

  scene = rt_newscene();

  xres=256; yres=256;
  aspectratio=1.0; raydepth=18;
  antialiasing=1; verbosemode=0; zoom=1.0;
 
  Ccenter.x=0.0; Ccenter.y=0.0; Ccenter.z=-8.0;
  Cview.x=0.0;   Cview.y=0.0;   Cview.z=1.0;
  Cup.x=0.0;     Cup.y=-1.0;     Cup.z=0.0;
  ctr1.x=20.0;  ctr1.y=20.0; ctr1.z=-40.0;
  
  tex1.col.r=1.0; tex1.col.g=1.0; tex1.col.b=1.0;
  tex1.ambient=1.0;  
  tex1.diffuse=0.0;
  tex1.specular=0.0; 
  tex1.opacity=8.0;
  tex1.texturefunc=0;
  tex2=tex1;

  min3.x=-1.0; min3.y=-1.0; min3.z=-0.40;
  max3.x=1.0;  max3.y=1.0;  max3.z=0.40;

  vol.loaded=0;
   
  for (i=0; i<MAXFRAMES; i++) {  
    sprintf(fname,".%4.4d.tga",i);
    strcpy(fname2,RFILE);
    strcat(fname2, fname);
     
    rt_initialize(&argc, &argv); 
    if (rt_mynode()==0) printf("Rendering: %s \n",fname2);

    xc=cos(6.28 * i / (1.0 * MAXFRAMES));
    yc=sin(6.28 * i / (1.0 * MAXFRAMES));
    Ccenter.x = xc*3.0;
    Ccenter.z = yc*3.0;
    Cview.x =  -xc;
    Cview.z =  -yc;

    rt_outputfile(scene, fname2);
    rt_resolution(scene, xres, yres);
    rt_verbose(scene, verbosemode);

    rt_camera_setup(scene, zoom, aspectratio, antialiasing, 
		raydepth, Ccenter, Cview, Cup);

    rt_light(scene, rt_texture(scene, &tex1), ctr1, 1.0); 

    rt_scalarvol(scene, rt_texture(scene, &tex2), min3, max3, 256, 256, 100, 
	"/disk7/cube/vol/head.256x256x109", &vol);   

    rt_renderscene(scene);
  }
  rt_finalize(); 

  return 0;
}
   
