/*  
 * This file contains the main program and driver for the raytracer.
 *
 *  $Id: animspheres2.c,v 1.11 2002/07/03 04:59:29 johns Exp $
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tachyon.h"

#define XRES 352 
#define YRES 240
#define NUMSP 12 

#define MYPI 3.1415926
#define MYTPI 6.282

int rt_mynode(void);

#define MAXFRAMES 400
#define MAXX 1.0 
#define MAXY 1.0 
#define MAXZ 1.0 

#define MINX -1.0
#define MINY -1.0
#define MINZ -1.0

#define LOOP 200.0
#define LOOP2 100.0
#define RAD 6.28

#ifdef cube
#define RFILE "/cfs/johns/anim/frame"
#endif

#ifndef cube 
#define RFILE "outfile"
#endif


typedef struct {
     apitexture tex;
      apivector ctr;
         apiflt rad; 
      apivector dir;
} asphere;

asphere sp[NUMSP];

apiflt randflt(void) {
  long a;
  apiflt f;
 
  a=rand() % 1000;

  f=(a*1.0) / 1000.0;
  return f; 
}


void initspheres(void) {
  int i;
  apiflt t1;

  for (i=0; i<NUMSP; i++) {
   sp[i].tex.col.r=randflt() / 3.0 + 0.66;         
   sp[i].tex.col.g=randflt() / 3.0 + 0.66;         
   sp[i].tex.col.b=randflt() / 3.0 + 0.66;         
   t1=randflt()*0.9;
   
   sp[i].tex.ambient=0.1;
   sp[i].tex.diffuse=t1;
   sp[i].tex.specular=0.9 - t1;
   sp[i].tex.opacity=1.0;
   
   sp[i].tex.scale.x=1.0;
   sp[i].tex.scale.y=1.0;
   sp[i].tex.scale.z=1.0;

   sp[i].tex.rot.x=0.0;
   sp[i].tex.rot.y=0.0;
   sp[i].tex.rot.z=0.0;
   sp[i].tex.texturefunc=rand() % 7;
    

   sp[i].ctr.x=randflt() * 2.0 - 1.0;
   sp[i].ctr.y=randflt() * 2.0 - 1.0;
   sp[i].ctr.z=randflt() * 2.0 - 1.0;

   sp[i].rad=1.00;
   
   sp[i].dir.x=randflt() * 0.05 - 0.02; 
   sp[i].dir.y=randflt() * 0.05 - 0.02; 
   sp[i].dir.z=randflt() * 0.05 - 0.02; 
  }
}


void movesp(int frame) {
  int i;

  for (i=0; i<6; i++) {
    sp[i].ctr.x = 3.0 * sin(((i + 1.0)/6.0)*MYTPI + MYTPI*((1.0 * frame) / MAXFRAMES));
    sp[i].ctr.y = 3.0 * cos(((i + 1.0)/6.0)*MYTPI + MYTPI*((1.0 * frame) / MAXFRAMES));
    sp[i].ctr.z = 1.0;
  }

  for (i=0; i<6; i++) {
    sp[i+6].ctr.x = 3.0 + 3.0 * sin(((i + 1.0)/6.0)*MYTPI + MYTPI*((1.0 * frame) / MAXFRAMES));
    sp[i+6].ctr.y = 1.0;
    sp[i+6].ctr.z = 3.0 * cos(((i + 1.0)/6.0)*MYTPI + MYTPI*((1.0 * frame) / MAXFRAMES));
  }

}

void drawsp(SceneHandle scene) {
  int i;
  apitexture p1;  
  apivector ct1, n1;

  for (i=0; i<NUMSP; i++) {
    rt_sphere(scene, rt_texture(scene, &sp[i].tex), sp[i].ctr, sp[i].rad);   
  }

  p1.col.r=1.0;
  p1.col.g=1.0;
  p1.col.b=1.0;
  p1.ambient=0.1;
  p1.diffuse=0.5;
  p1.specular=0.4;
  p1.opacity=1.0;
 
  ct1.x=0.0;
  ct1.y=-5.10;
  ct1.z=0.0;

  n1.x=0.0;
  n1.y=1.0;
  n1.z=0.0;
 
  rt_plane(scene, rt_texture(scene, &p1), ct1, n1);  

}

int main(int argc, char **argv) {
  SceneHandle scene;
  int i;
  int xres, yres, raydepth, antialiasing, verbosemode;
  apiflt zoom, aspectratio;
  apivector Ccenter, Cview, Cup;
  apivector ctr1, ctr2;
  apitexture tex1, tex2;
  char fname[100];
  char fname2[200];

  scene = rt_newscene();

  xres=XRES; 
  yres=YRES;
  aspectratio=1.0;
  raydepth=18;
  antialiasing=0; 
  verbosemode=0;
  zoom=1.0;
 
  Ccenter.x=1.5; Ccenter.y=3.0; Ccenter.z=-8.0;
  Cview.x=0.0;   Cview.y=-0.2;   Cview.z=0.8;
  Cup.x=0.0;     Cup.y=1.0;     Cup.z=0.0;

  ctr1.x=20.0;  ctr1.y=20.0; ctr1.z=-40.0;
  ctr2.x=-20.0; ctr2.y=20.0; ctr2.z=-40.0;
  
  tex1.col.r=1.0;
  tex1.col.g=0.5;
  tex1.col.b=0.0;
  tex1.ambient=1.0;
  tex1.opacity=1.0;
  tex2=tex1;
  tex2.col.r=0.0;
  tex2.col.b=1.0;

  initspheres();
   
  for (i=0; i<MAXFRAMES; i++) {  
    sprintf(fname,".%4.4d.tga",i);
    strcpy(fname2,RFILE);
    strcat(fname2, fname);

    rt_initialize(&argc, &argv); 
    if (rt_mynode()==0) printf("Rendering: %s \n",fname2);

    rt_outputfile(scene, fname2);
    rt_resolution(scene, xres, yres);
    rt_verbose(scene, verbosemode);

    rt_camera_setup(scene, zoom, aspectratio, antialiasing, raydepth,
      Ccenter, Cview, Cup);

    rt_light(scene, rt_texture(scene, &tex1), ctr1, 1.0);
    rt_light(scene, rt_texture(scene, &tex2), ctr2, 1.0);
    movesp(i);  
    drawsp(scene);

    rt_renderscene(scene);
  }

  return 0;
}
   
