/* 
 * mainanim.c - This file contains an animation driver for the raytracer.
 *
 *  $Id: mainanim.c,v 1.14 2000/12/20 00:09:36 johns Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tachyon.h"

int rt_mynode(void); /* proto */

#define MAXFRAMES 100
#define LOOP 100.0
#define LOOP2 50.0
#define RAD 6.28

#ifdef cube
#define RFILE "/cfs/johns/anim/frame"
#else
#define RFILE "outfile"
#endif

int main(int argc, char **argv) {
  SceneHandle scene;
  int i,j;
  apivector ctr, ctr1, vect2;
  apitexture tex1, tex2;
  apiflt f1, f2;
  char fname[200];
  char fname2[200];

  rt_initialize(&argc, &argv); 

  tex1.col.r=1.0;
  tex1.col.g=1.0;
  tex1.col.b=1.0;
  ctr1.x=5.0;
  ctr1.y=5.0;
  ctr1.z=1.0;
  f1=0.5;

  tex2.col.r=1.0;
  tex2.col.g=1.0;
  tex2.col.b=1.0;
  tex2.ambient=0.1;
  tex2.diffuse=0.7;
  tex2.specular=0.2;
  tex2.opacity=1.0;
  tex2.texturefunc=0;
  f2=1.5;

  vect2.x=0.0;
  vect2.y=1.0;
  vect2.z=0.0;

  for (i=0; i<MAXFRAMES; i++) {
    void * txarray[20];

    scene = rt_newscene();
    rt_resolution(scene, 320, 240);

#ifdef cube
    if (rt_mynode()==0) printf("Rendering frame %d \n",i);
#endif
#ifndef cube
    printf("Rendering frame %d \n",i);
#endif
    ctr1.x= i / 20.0 - 5.0;
    ctr1.y=5.0;
    ctr1.z= i / 20.0 + 5.0; 
    tex1.col.r=1.0;
    tex1.col.g=1.0;
    tex1.col.b=1.0;
    txarray[0] = rt_texture(scene, &tex1);
    rt_light(scene, txarray[0], ctr1, f1);

    ctr1.x= i / 15.0 - 5.0; 
    ctr1.y=0.0;
    ctr1.z=15.0;
    tex1.col.r=0.0;
    tex1.col.g=0.0;
    tex1.col.b=1.0;
    txarray[1] = rt_texture(scene, &tex1);
    rt_light(scene, txarray[1], ctr1, f1 / 3.0 );

    j=i;
    tex2.col.r=1.0;
    tex2.texturefunc=0;
    ctr.x=2.0*sin((j*RAD) / LOOP);
    ctr.z=10.0 + 2.0*cos((j*RAD) / LOOP);
    ctr.y=-2.0;

    txarray[2] = rt_texture(scene, &tex2);
    rt_sphere(scene, txarray[2], ctr, f2);

    tex2.texturefunc=0;
    j=i+(MAXFRAMES/3);
    tex2.col.r=0.5 + 0.5*sin((j*RAD)/ LOOP2);
    ctr.x=2.0*sin((j*RAD) / LOOP);
    ctr.z=10.0 + 2.0*cos((j*RAD) / LOOP);
    ctr.y=-2.0;

    txarray[3] = rt_texture(scene, &tex2);
    rt_sphere(scene, txarray[3], ctr, f2); 

    j=i+((MAXFRAMES * 2)/3);
    tex2.col.r=1.0;
    tex2.col.b=0.5 + 0.5*sin((j*RAD)/ LOOP2);
    ctr.x=2.0*sin((j*RAD) / LOOP);
    ctr.z=10.0 + 2.0*cos((j*RAD) / LOOP);
    ctr.y=-2.0;

    txarray[4] = rt_texture(scene, &tex2);
    rt_sphere(scene, txarray[4], ctr, f2);

    tex2.col.r=0.0;
    tex2.col.b=1.0;
    tex2.ambient=0.0;
    tex2.diffuse=0.0;
    tex2.specular=0.99;

    ctr.x=0.0;
    ctr.y=-2.0;
    ctr.z=10.0;

    txarray[5] = rt_texture(scene, &tex2);
    rt_sphere(scene, txarray[5], ctr, f2); 


    tex2.diffuse=0.8;
    tex2.ambient=0.1;
    tex2.specular=0.2;
    tex2.col.r=1.0;
    ctr.y=-7.0;  

    txarray[6] = rt_texture(scene, &tex2);
    rt_plane(scene, txarray[6], ctr, vect2); 


    sprintf(fname,".%4.4d.tga",i);
    strcpy(fname2, RFILE);
    strcat(fname2, fname);

    rt_outputfile(scene, fname2);

    rt_renderscene(scene);

    rt_deletescene(scene);

    for (j=0; j<7; j++)
      free(txarray[j]);
  }

  rt_finalize();

  return 0;
}
   
