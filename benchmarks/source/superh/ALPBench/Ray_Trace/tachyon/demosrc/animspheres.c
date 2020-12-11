/* main.c 
 * This file contains the main program and driver for the raytracer.
 *
 *  $Id: animspheres.c,v 1.13 2002/07/03 04:59:28 johns Exp $
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tachyon.h"

int rt_mynode(void); /* proto */

/* Number of frames to render */
#define MAXFRAMES 400

/* Number of bouncing spheres */
#define NUMSP 16 

/* NTSC Resolution */
/* #define XRES 640 
   #define YRES 480
*/

/* MPEG-1 Resolution */
#define XRES 352
#define YRES 240

#define MAXX 1.0 
#define MAXY 1.0 
#define MAXZ 1.0 

#define MINX -1.0
#define MINY -1.0
#define MINZ -1.0

#define LOOP 200.0
#define LOOP2 100.0
#define RAD 6.28

typedef struct {
     apitexture tex;
      apivector ctr;
         apiflt rad; 
      apivector dir;
     void * voidtex;
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

   sp[i].rad=randflt()*0.5 + 0.05;
   
   sp[i].dir.x=randflt() * 0.05 - 0.02; 
   sp[i].dir.y=randflt() * 0.05 - 0.02; 
   sp[i].dir.z=randflt() * 0.05 - 0.02; 
  }
}

void movesp(void) {
  int i;

  for (i=0; i<NUMSP; i++) {
    sp[i].ctr.x += sp[i].dir.x;
    sp[i].ctr.y += sp[i].dir.y;
    sp[i].ctr.z += sp[i].dir.z;
 
    if (sp[i].ctr.x > MAXX) {
      sp[i].ctr.x = MAXX;
      sp[i].dir.x = -sp[i].dir.x;
    }
    if (sp[i].ctr.x < MINX) {
      sp[i].ctr.x = MINX;
      sp[i].dir.x = -sp[i].dir.x;
    }

    if (sp[i].ctr.y > MAXY) {
      sp[i].ctr.y = MAXY;
      sp[i].dir.y = -sp[i].dir.y;
    }
    if (sp[i].ctr.y < MINY) {
      sp[i].ctr.y = MINY;
      sp[i].dir.y = -sp[i].dir.y;
    }

    if (sp[i].ctr.z > MAXZ) {
      sp[i].ctr.z = MAXZ;
      sp[i].dir.z = -sp[i].dir.z;
    }
    if (sp[i].ctr.z < MINZ) {
      sp[i].ctr.z = MINZ;
      sp[i].dir.z = -sp[i].dir.z;
    }
    sp[i].tex.ctr.x = sp[i].dir.x;
    sp[i].tex.ctr.y = sp[i].dir.y;
    sp[i].tex.ctr.z = sp[i].dir.z;
  }
}

void drawsp(SceneHandle scene) {
  int i;
  apitexture p1;  
  apivector ct1, n1;

  for (i=0; i<NUMSP; i++) {
    sp[i].voidtex = rt_texture(scene, &sp[i].tex);
    rt_sphere(scene, sp[i].voidtex, sp[i].ctr, sp[i].rad);   
  }

  p1.col.r=1.0;
  p1.col.g=1.0;
  p1.col.b=1.0;
  p1.ambient=0.1;
  p1.diffuse=0.5;
  p1.specular=0.4;
  p1.opacity=1.0;
 
  ct1.x=0.0;
  ct1.y=-1.10;
  ct1.z=0.0;

  n1.x=0.0;
  n1.y=1.0;
  n1.z=0.0;
 
  rt_plane(scene, rt_texture(scene, &p1), ct1, n1);  
}

int main(int argc, char **argv) {
  SceneHandle scene;
  int i, j;
  apivector Ccenter, Cview, Cup;
  apivector ctr1, ctr2;
  apitexture tex1, tex2;
  void * vtx1, * vtx2;
  char fname[100];

  rt_initialize(&argc, &argv); 

  Ccenter.x=0.0; Ccenter.y=0.0; Ccenter.z=-3.0;
  Cview.x=0.0;   Cview.y=0.0;   Cview.z=1.0;
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
    scene = rt_newscene();
    vtx1=rt_texture(scene, &tex1);
    vtx2=rt_texture(scene, &tex2);

    sprintf(fname,"outfile.%4.4d.tga",i);

    if (rt_mynode()==0) printf("Rendering: %s\n", fname);

    rt_outputfile(scene, fname);
    rt_resolution(scene, XRES, YRES);
    rt_verbose(scene, 0);

    rt_camera_setup(scene, 1.0, 1.0, 0, 5, Ccenter, Cview, Cup);

    movesp(); 
    drawsp(scene);

    rt_light(scene, vtx1, ctr1, 1.0);
    rt_light(scene, vtx2, ctr2, 1.0);

    rt_renderscene(scene);

    rt_deletescene(scene);

    for (j=0; j<NUMSP; j++)
      free(sp[i].voidtex);  
  }

  rt_finalize();

  return 0;
}
   
