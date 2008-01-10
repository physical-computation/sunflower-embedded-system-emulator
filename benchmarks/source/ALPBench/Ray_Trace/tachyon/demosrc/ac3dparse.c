/*
 * Code for parsing ac3d modeler files
 *
 * See http://www.comp.lancs.ac.uk/computing/users/andy/ac3d/
 * for more information on ac3d.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> /* needed for toupper(), macro.. */

#include "tachyon.h" /* ray tracer api */

#define PARSE_INTERNAL
#include "ac3dparse.h" /* self protos */
#undef PARSE_INTERNAL

static texentry textable[NUMTEXS]; /* texture lookup table */
static int numtextures;         /* number of TEXDEF textures               */
static int numobjectsparsed;    /* total number of objects parsed so far   */
static apicolor scenebackcol;   /* scene background color                  */

static int stringcmp(char * a, char * b) {
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


static void GetAC3DString(FILE * dfile, char * str) {
   char c;
   int i=0;

   /* find opening quote */
   while ((!feof(dfile)) && ((c = fgetc(dfile)) != '"'));
    
   /* scan in string until closing quote */ 
   while ((!feof(dfile)) && ((c = fgetc(dfile)) != '"')) {
     str[i] = c; 
     i++;
   }
   str[i]='\0'; /* nul terminate the string */
}

static void reset_tex_table(void) {
  numtextures=0; 
  memset(&textable, 0, sizeof(textable));
}

static errcode add_texture(void * tex, char name[TEXNAMELEN]) {
  textable[numtextures].tex=tex;
  strcpy(textable[numtextures].name, name); 

  numtextures++;
  if (numtextures > NUMTEXS) {
    printf("Parse: %d textures allocated, texture slots full!\n", numtextures);
    numtextures--; /* keep writing over last texture if we've run out.. */
    return PARSEALLOCERR;
  }

  return PARSENOERR;
}

static errcode GetString(FILE * dfile, char * string) {
  char data[100];

  fscanf(dfile,"%s",data);
  if (stringcmp(data, string) != 0) {
    printf("parse: Expected %s, got %s \n",string, data);
    printf("parse: Error while parsing object: %d \n",numobjectsparsed);
    return PARSEBADSYNTAX;
  }

  return PARSENOERR;
}

unsigned int ParseAC3D(char * modelfile, SceneHandle scene) {
  FILE * dfile;
  char filehdr[255];
  errcode rc;
 
  reset_tex_table(); 
  dfile=NULL;

  dfile=fopen(modelfile,"r");
  if (dfile==NULL) {
    return PARSEBADFILE;
  }

  fscanf(dfile, "%s", filehdr);
  if (strcmp(filehdr, "AC3Db")) {
    printf("Unknown version of AC3D Model File Format\n");
    fclose(dfile);
    return PARSEBADFILE;
  }

  rc = GetScenedefs(dfile, scene); 
  if (rc != PARSENOERR)
    return rc;

  scenebackcol.r = 0.0; /* default background is black */
  scenebackcol.g = 0.0;
  scenebackcol.b = 0.0;

  numobjectsparsed=0;
  while ((rc = GetScene(dfile, scene)) == PARSENOERR) {
    numobjectsparsed++;
  } 
  fclose(dfile);

  if (rc == PARSEEOF)
    rc = PARSENOERR;

  rt_background(scene, scenebackcol);

  return rc;
}


static errcode GetScenedefs(FILE * dfile, SceneHandle scene) {
  apivector Ccenter, Cview, Cup;
  apiflt zoom, aspectratio;
  int raydepth, antialiasing;

  rt_outputfile(scene, "outfile.tga");
  rt_resolution(scene, 512, 512);
  rt_verbose(scene, 0);

  zoom=1.0;
  aspectratio=1.0;
  antialiasing=0;
  raydepth=6;

  Ccenter.x = 0.0;
  Ccenter.y = 0.0;
  Ccenter.z = -5.0;

  Cview.x = 0.0;
  Cview.y = 0.0;
  Cview.z = 1.0;

  Cup.x = 0.0;
  Cup.y = 1.0;
  Cup.z = 0.0;

  rt_camera_setup(scene, zoom, aspectratio, antialiasing, raydepth,
              Ccenter, Cview, Cup);

  { /* lighting hack */
    apivector ctr;
    apitexture tex;

    memset(&tex, 0, sizeof(apitexture));

    tex.col.r = 1.0;
    tex.col.g = 1.0;
    tex.col.b = 1.0;
    ctr.x = 0.0;
    ctr.y = 0.0;
    ctr.z = -100.0;
 
    rt_light(scene, rt_texture(scene, &tex), ctr, 1.0);
  }

  return PARSENOERR;
}

static errcode GetScene(FILE * dfile, SceneHandle scene) {
  char objtype[80];
 
  if (fscanf(dfile, "%s", objtype) != 1) {
    return PARSEEOF; /* end parsing */
  }
  if (!stringcmp(objtype, "MATERIAL")) {
    return GetMaterial(dfile, scene);
  }	
  if (!stringcmp(objtype, "OBJECT")) {
    return GetObject(dfile, scene);
  }

  printf("Found bad token: %s expected an object type\n", objtype);
  return PARSEBADSYNTAX;
}

static errcode GetMaterial(FILE * dfile, SceneHandle scene) {
  apitexture tex;
  float a,b,c,d;
  int e;
  errcode rc;
  char texname[TEXNAMELEN];

  fscanf(dfile, "%s", texname);

  rc = GetString(dfile, "RGB");
  fscanf(dfile, "%f %f %f", &a, &b, &c); 
  tex.col.r = a;
  tex.col.g = b;
  tex.col.b = c;
  tex.diffuse = (a + b + c) / 3.0;

  rc = GetString(dfile, "AMB");
  fscanf(dfile, "%f %f %f", &a, &b, &c); 
  tex.ambient= ((a + b + c) / 3.0);

  rc |= GetString(dfile, "EMIS");
  fscanf(dfile, "%f %f %f", &a, &b, &c);

  rc |= GetString(dfile, "SPEC");
  fscanf(dfile, "%f %f %f", &a, &b, &c);
  tex.specular= (a + b + c) / 3.0;

  rc |= GetString(dfile, "SHI");
  fscanf(dfile, "%d", &e);

  rc |= GetString(dfile, "TRANS");
  fscanf(dfile, "%f", &d);  
  tex.opacity= (d > 0.99) ? 0.0 : (1.0 - d);
  tex.texturefunc = 0;

  add_texture(rt_texture(scene, &tex), texname); 

  return rc;
}

static void RmatIdentity(RotMat rmat) {
  int i, j;
  for (j=0; j<3; j++) {
    for (i=0; i<3; i++) {
      if (i==j)
        rmat[j][i] = 1.0;
      else 
        rmat[j][i] = 0.0;
    }
  }
} 

static void RmatMult(RotMat A, RotMat B, RotMat C) {
  int i, j, k;

  for (k=0; k<3; k++) {
    for (j=0; j<3; j++) {
      A[k][j] = 0.0;
      for (i=0; i<3; i++) {
        A[k][j] += B[k][i] * C[i][j];  
      }
    }
  }
}

static void RmatTmatMult(TransMat NT, RotMat R, TransMat T) {
  int i, j;
  for (j=0; j<3; j++) {
    NT[j] = 0.0;
    for (i=0; i<3; i++) {
      NT[j] += R[j][i] * T[i];
    }
  } 
}

static void tlist_add_tri(tri_list ** tlist, int v0, int v1, int v2, 
                          int smooth, int texnum) {
  tri_list * newlist;

  newlist = (tri_list *) malloc(sizeof(tri_list));
  newlist->v0 = v0;
  newlist->v1 = v1;
  newlist->v2 = v2;
  newlist->smooth = smooth;
  newlist->texnum = texnum;
  newlist->next = *tlist;  

  *tlist = newlist;
}

static void tlist_delete(tri_list ** tlist) {
  tri_list * tmp; 
  tri_list * next; 
  
  tmp = *tlist;
  
  while (tmp != NULL) {
    next = tmp->next;
    free(tmp);
    tmp = next;
  }
 
  *tlist = NULL;
}


static void clear_normals(apivector * normals, int numverts) {
  apivector tmp; 
  int i;

  tmp.x = tmp.y = tmp.z = 0.0;

  for (i=0; i<numverts; i++) {
    normals[i] = tmp;
  }
}


static void normalize(apivector * vec) {
  double length;
  length = sqrt((vec->x * vec->x) + (vec->y * vec->y) + (vec->z * vec->z));
  vec->x /= length;
  vec->y /= length;
  vec->z /= length;
}

static void renormalize_normals(apivector * normals, int numverts) {
  int i;
  
  for (i=0; i<numverts; i++) {
    normalize(&normals[i]);
  }
}

static apivector tri_normal(apivector * v0, apivector * v1, apivector * v2) {
  apivector U, V;
  apivector norm;

  U.x = v1->x - v0->x;
  U.y = v1->y - v0->y;
  U.z = v1->z - v0->z;

  V.x = v2->x - v0->x;
  V.y = v2->y - v0->y;
  V.z = v2->z - v0->z;

  norm.x = (U.y * V.z) - (U.z * V.y);
  norm.y = (U.z * V.x) - (U.x * V.z);
  norm.z = (U.x * V.y) - (U.y * V.x);  

  normalize(&norm);

  return norm;
}

static void gen_triangles(SceneHandle scene, tri_list * tlist, 
                          apivector * vertex, apivector * normal) {
  tri_list * cur;

  cur = tlist;
  while (cur != NULL) {
    if (cur->smooth) {
      rt_stri(scene, textable[cur->texnum].tex, 
        vertex[cur->v0], vertex[cur->v1], vertex[cur->v2],
        normal[cur->v0], normal[cur->v1], normal[cur->v2]);
     
    }
    else {
      rt_tri(scene, textable[cur->texnum].tex, 
        vertex[cur->v0], vertex[cur->v1], vertex[cur->v2]);
    }

    cur = cur->next;
  }
}


static errcode GetRecurseObject(SceneHandle scene, FILE *dfile, 
                                RotMat rmat, TransMat tmat) {
  char token[256], objname[128], name[128], texname[128], urlname[100];
  RotMat localrmat, newrmat;
  TransMat localtmat, newtmat;
  float txrepa, txrepb, a, b;
  int numvert, numsurf, surf, materialnum, numrefs, numkids, data;
  int i, j;
  apivector * vertexarray = NULL; 
  apivector * normalarray = NULL; 
  int * refarray = NULL;
  tri_list * tlist = NULL;
  errcode rc = PARSENOERR;

  /* zero out variables */
  numvert = numsurf = surf = materialnum = numrefs = numkids = data = 0;

  RmatIdentity(localrmat);
  localtmat[0] = 0.0;
  localtmat[1] = 0.0;
  localtmat[2] = 0.0;
  fscanf(dfile, "%s", objname);
  fscanf(dfile, "%s", token);
  if (!stringcmp(token, "NAME")) {
    GetAC3DString(dfile, name);
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "DATA")) {
    fscanf(dfile, "%d", &data);
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "TEXTURE")) {
    fscanf(dfile, "%s", texname);
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "TEXREP")) {
    fscanf(dfile, "%f %f", &txrepa, &txrepb);
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "ROT")) {
    for (j=0; j<3; j++) {
      for(i=0; i<3; i++) {
        fscanf(dfile, "%f", &localrmat[j][i]);
      }
    } 
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "LOC")) {
    for (j=0; j<3; j++) {
      fscanf(dfile, "%f", &localtmat[j]);
    } 
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "ROT")) {
    for (j=0; j<3; j++) {
      for(i=0; i<3; i++) {
        fscanf(dfile, "%f", &localrmat[j][i]);
      }
    } 
    fscanf(dfile, "%s", token);
  } 

  /* Perform Matrix Transforms for local coordinate system */
  RmatTmatMult(newtmat, rmat, localtmat);
  newtmat[0] += tmat[0]; 
  newtmat[1] += tmat[1]; 
  newtmat[2] += tmat[2]; 
  RmatMult(newrmat, rmat, localrmat);

  if (!stringcmp(token, "URL")) {
    fscanf(dfile, "%s", urlname);
    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "TEXTURE")) {
    GetAC3DString(dfile, token);
    fscanf(dfile, "%s", token);
  }  
  if (!stringcmp(token, "NUMVERT")) {
    TransMat vtx, tvtx;

    fscanf(dfile, "%d", &numvert);

    vertexarray = (apivector *) malloc(numvert * sizeof(apivector));
    normalarray = (apivector *) malloc(numvert * sizeof(apivector));

    /* initialize the normal array */
    clear_normals(normalarray, numvert);

    /* load and transform vertices */
    for (i=0; i<numvert; i++) {
      fscanf(dfile, "%f %f %f", &vtx[0], &vtx[1], &vtx[2]);
      /* transform vertices */
      RmatTmatMult(tvtx, newrmat, vtx);
      vertexarray[i].x = tvtx[0] + newtmat[0];
      vertexarray[i].y = tvtx[1] + newtmat[1];
      vertexarray[i].z = tvtx[2] + newtmat[2];
    }    

    fscanf(dfile, "%s", token);
  } 
  if (!stringcmp(token, "NUMSURF")) {
    fscanf(dfile, "%d", &numsurf);
    fscanf(dfile, "%s", token);

    for (j=0; j<numsurf; j++) {
      numrefs = surf = materialnum = 0;
      
      if (!stringcmp(token, "SURF")) {

/* COMPILER BUG!!! */
/* This is messed up friend.. */
/* if done using a normal fscanf() the program nails its */
/* stack, and seg faults.  This *must* be a compiler or  */
/* libc bug. */ 
#if 1	
        fscanf(dfile, "%s", token);
        sscanf(token, "%x", &surf);
#else
        fscanf(dfile, "%x", &surf);
#endif
        fscanf(dfile, "%s", token);
      } 
      if (!stringcmp(token, "MAT")) {
        fscanf(dfile, "%d", &materialnum);
        fscanf(dfile, "%s", token);
      } 
      if (!stringcmp(token, "REFS")) 
        fscanf(dfile, "%d", &numrefs);  
      else 
        return (rc |= PARSEBADSYNTAX);

      refarray = (int *) malloc(numrefs * sizeof(int));
      for (i=0; i<numrefs; i++) {
        fscanf(dfile, "%d %f %f", &refarray[i], &a, &b);  
      }  

      /* generate triangles/polygons here */
      /* ignore all lines, points and other non-surface primatives */
      if ((surf & 0xF) == 0) {
        int v0, vold, vnew;
        apivector trinorm;
        
        /* add in surface normal to vertices */
        trinorm = tri_normal(&vertexarray[refarray[1]],         
                             &vertexarray[refarray[0]], 
                             &vertexarray[refarray[2]]); 
       
        for (i=0; i<numrefs; i++) {
          normalarray[refarray[i]].x += trinorm.x;
          normalarray[refarray[i]].y += trinorm.y;
          normalarray[refarray[i]].z += trinorm.z;
        }

        v0 = refarray[0];
        vold = refarray[1];

        for (i=2; i<numrefs; i++) {
          vnew = refarray[i];
          tlist_add_tri(&tlist, vold, v0, vnew, surf & 0x10, materialnum);
          vold = vnew; 
        }
      }
    
      free(refarray);
      refarray=NULL;
      fscanf(dfile, "%s", token);
    }
  } 

  if ((vertexarray != NULL) && (normalarray != NULL)) {
    /* now that all vertex normals have been summed, we'll renormalize */
    renormalize_normals(normalarray, numvert);

    gen_triangles(scene, tlist, vertexarray, normalarray);
    tlist_delete(&tlist); 
  }

  /* free vertex and normal arrays */
  if (vertexarray != NULL) 
    free(vertexarray);
  vertexarray = NULL;

  if (normalarray != NULL) 
    free(normalarray);
  normalarray = NULL;



  if (!stringcmp(token, "KIDS")) 
    fscanf(dfile, "%d", &numkids);
  else 
    return (rc |= PARSEBADSYNTAX);

  /* Recurse to handle child geometry */ 
  while (numkids > 0) {
    rc |= GetString(dfile, "OBJECT");
    rc |= GetRecurseObject(scene, dfile, newrmat, newtmat); 
    numkids--;
  }
 
  return rc; 
}

static errcode GetObject(FILE *dfile, SceneHandle scene) {
  RotMat defaultrmat;
  TransMat defaulttmat;
  errcode rc;

  RmatIdentity(defaultrmat);
  defaulttmat[0] = 0.0;
  defaulttmat[1] = 0.0;
  defaulttmat[2] = 0.0;

  rc = GetRecurseObject(scene, dfile, defaultrmat, defaulttmat);

  return rc;
}



 
