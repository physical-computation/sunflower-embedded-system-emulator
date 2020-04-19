/*
 * Code for parsing NFF model files.
 *
 * See the NFF.DOC file in the SPD Distribution by Eric Haines 
 * for more information on NFF.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> /* needed for toupper(), macro.. */

#include "tachyon.h"  /* ray tracer api */
#include "nffparse.h" /* nff protos */

static void * curtexture;
static apicolor backgr;

/* protos */
int NFFGetObject(FILE * dfile, SceneHandle scene);


int NFFstringcmp(char * a, char * b) {
  int i, s, l;

  s=strlen(a);
  l=strlen(b);

  if (s != l)
    return 1;

  for (i=0; i<s; i++) {
    if (toupper(a[i]) != toupper(b[i])) {
      return 1;
    }
  }
  return 0;
}


void NFFGetString(FILE * dfile, char * string) {
  char data[100];

  fscanf(dfile,"%s",data);

  if (NFFstringcmp(data, string) != 0) {
    printf("parse: Expected %s, got %s \n",string, data);
    printf("Rendering terminated. \n");
    exit(1);
  }
}

void NFFGetVector(FILE * dfile, apivector * v1) {
  float a, b, c;

  fscanf(dfile, "%f %f %f", &a, &b, &c);
  v1->x=a;
  v1->y=b;
  v1->z=c;
}

unsigned int ParseNFF(char *nffname, SceneHandle scene) {
  FILE * dfile;
  backgr.r = 0.0;
  backgr.g = 0.0;
  backgr.b = 0.0;

  if (nffname == NULL) {
    return NFFBADFILE;
  }
  else {
    dfile=fopen(nffname,"r");
    if (dfile==NULL) {
      return NFFBADFILE;
    }
  }

  while (NFFGetObject(dfile, scene) == NFFNOERR); 

  rt_background(scene, backgr); 
  
  return NFFNOERR;
}

int NFFGetScenedefs(FILE * dfile, SceneHandle scene) {
  apivector Ccenter, Cview, Cup;
  apiflt zoom, aspectratio;
  int raydepth, antialiasing;
  int xres, yres, verbose;
  apivector lookat;
  float vangle, hither;
 
  NFFGetString(dfile, "from");
  NFFGetVector(dfile, &Ccenter);

  NFFGetString(dfile, "at");
  NFFGetVector(dfile, &lookat);
  Cview.x = lookat.x - Ccenter.x;
  Cview.y = lookat.y - Ccenter.y;
  Cview.z = lookat.z - Ccenter.z;

  NFFGetString(dfile, "up");
  NFFGetVector(dfile, &Cup);

  NFFGetString(dfile, "angle");
  fscanf(dfile, "%f", &vangle);
  zoom = 1.0; /* XXX fix me later */
  aspectratio = 1.0;
 
  NFFGetString(dfile, "hither");
  fscanf(dfile, "%f", &hither);
 
  NFFGetString(dfile, "resolution");
  fscanf(dfile, "%d %d", &xres, &yres);

  antialiasing = 0;
  raydepth = 6;
  verbose = 0;

  rt_outputfile(scene, "outfile.tga");
  rt_resolution(scene, xres, yres);
  rt_verbose(scene, verbose);

  rt_camera_setup(scene, zoom, aspectratio, antialiasing, raydepth,
              Ccenter, Cview, Cup);

  rt_background(scene, backgr); 

  return NFFNOERR; 
}


int NFFGetBackground(FILE *dfile, SceneHandle scene) {
  float r, g, b; 
  fscanf(dfile, "%f %f %f", &r, &g, &b);
  backgr.r = r;
  backgr.g = g;
  backgr.b = b;

  return NFFNOERR; 
}

int NFFGetLight(FILE *dfile, SceneHandle scene) {
  apiflt rad;
  apivector ctr;
  apitexture tex;
  float r, g, b;

  NFFGetVector(dfile, &ctr);
  rad = 1.0; /* XXX hack for now */

  r = g = b = 1.0; 

  fscanf(dfile, "%f %f %f", &r, &g, &b);
  tex.col.r = r;
  tex.col.g = g;
  tex.col.b = b;
 
  rt_light(scene, rt_texture(scene, &tex), ctr, rad);

  return NFFNOERR;
}

int NFFGetTexture(FILE *dfile, SceneHandle scene) {
  apitexture tex;
  float r, g, b, Kd, Ks, Shine, T, IOR;

  fscanf(dfile, "%f %f %f %f %f %f %f %f", 
         &r, &g, &b, &Kd, &Ks, &Shine, &T, &IOR);
  tex.col.r = r;
  tex.col.g = g;
  tex.col.b = b;
  tex.ambient = 0.1;
  tex.diffuse = Kd;
  tex.specular = Ks;
  tex.opacity = (T > 0.99) ? 0.0 : (1.0 - T);
  tex.texturefunc = 0;

  curtexture = rt_texture(scene, &tex); /* XXX memory leak city, */
                                /* we aren't keeping track of these... */
  return NFFNOERR;
}

int NFFGetCylCone(FILE *dfile, SceneHandle scene) {
  apivector pnt1, pnt2;
  apivector ctr, axis;
  float baserad, apexrad;
 
  NFFGetVector(dfile, &pnt1);
  fscanf(dfile, "%f", &baserad);

  NFFGetVector(dfile, &pnt2);
  fscanf(dfile, "%f", &apexrad);

  ctr=pnt1;
  axis.x=pnt2.x - pnt1.x;
  axis.y=pnt2.y - pnt1.y;
  axis.z=pnt2.z - pnt1.z;

  /* XXX should really be cone */
  rt_fcylinder(scene, curtexture, ctr, axis, baserad); 

  return NFFNOERR;
}

int NFFGetSphere(FILE *dfile, SceneHandle scene) {
  apiflt rad;
  apivector ctr;
  float a;  

  NFFGetVector(dfile, &ctr);
  fscanf(dfile, "%f", &a);
  rad = a;

  rt_sphere(scene, curtexture, ctr, rad);

  return NFFNOERR;
}

int NFFGetPolygon(FILE *dfile, SceneHandle scene) {
  int numverts, i;
  apivector v0, vold, vnew;

  fscanf(dfile, "%d", &numverts); 
  NFFGetVector(dfile, &v0);
  NFFGetVector(dfile, &vold);

  for (i=2; i<numverts; i++) {
    NFFGetVector(dfile, &vnew);
    rt_tri(scene, curtexture, v0, vold, vnew);
    vold = vnew;
  } 

  return NFFNOERR;
}

int NFFGetPatch(FILE *dfile, SceneHandle scene) {
  int numverts, i;
  apivector v0, n0;
  apivector vold, nold;
  apivector vnew, nnew;

  fscanf(dfile, "%d", &numverts); 
  NFFGetVector(dfile, &v0);
  NFFGetVector(dfile, &n0);
  NFFGetVector(dfile, &vold);
  NFFGetVector(dfile, &nold);

  for (i=2; i<numverts; i++) {
    NFFGetVector(dfile, &vnew);
    NFFGetVector(dfile, &nnew);
    rt_stri(scene, curtexture, v0, vold, vnew, n0, nold, nnew) ;
    vold = vnew;
    nold = nnew;
  } 

  return NFFNOERR;
}

int NFFGetObject(FILE * dfile, SceneHandle scene) {
  char objtype[80];
  if (fscanf(dfile, "%s", objtype) != 1) 
    return NFFEOF; /* end parsing */

  if (!NFFstringcmp(objtype, "v")) {
    return NFFGetScenedefs(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "b")) {
    return NFFGetBackground(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "l")) {
    return NFFGetLight(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "f")) {
    return NFFGetTexture(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "s")) {
    return NFFGetSphere(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "c")) {
    return NFFGetCylCone(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "p")) {
    return NFFGetPolygon(dfile, scene);
  }
  if (!NFFstringcmp(objtype, "pp")) {
    return NFFGetPatch(dfile, scene);
  }

  return NFFEOF;
}


