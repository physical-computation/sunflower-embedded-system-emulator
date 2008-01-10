/* 
 * parse.c - an UltraLame (tm) parser for simple data files...
 *
 *  $Id: parse.c,v 1.59 2004/05/31 03:56:21 johns Exp $
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> /* needed for toupper(), macro.. */

#include "tachyon.h"  /* Tachyon ray tracer API */

#ifdef USELIBMGF
#include "mgfparse.h" /* MGF parser code */
#endif

#define PARSE_INTERNAL
#include "parse.h" /* self protos */
#undef PARSE_INTERNAL

#define ERROR_READBUF_SIZE 65536

/*
 * a is unknown compared string, b must be upper case already...)
 */
static int stringcmp(const char * a, const char * b) {
  int i, s, l;

  s=strlen(a);
  l=strlen(b);

  if (s != l) 
    return 1;

  for (i=0; i<s; i++) {
    if (toupper(a[i]) != b[i]) {
      return 1;
    }
  }
  return 0;
}

static void reset_tex_table(parsehandle * ph, SceneHandle scene) {
  apitexture apitex;
  
  ph->maxtextures=512; 
  ph->numtextures=0; 
  ph->textable = (texentry *) malloc(ph->maxtextures * sizeof(texentry));
  memset(ph->textable, 0, ph->maxtextures * sizeof(texentry));

  apitex.col.r=1.0;
  apitex.col.g=1.0; 
  apitex.col.b=1.0; 
  apitex.ambient=0.1;
  apitex.diffuse=0.9;
  apitex.specular=0.0;
  apitex.opacity=1.0;
  apitex.texturefunc=0;

  ph->defaulttex.tex=rt_texture(scene, &apitex);
  rt_hash_init(&ph->texhash, 1024);
}

static void free_tex_table(parsehandle * ph, SceneHandle scene) {
  int i;

  rt_hash_destroy(&ph->texhash);

  for (i=0; i<ph->numtextures; i++) {
    free(ph->textable[i].name);
  }
  
  free(ph->textable);
  ph->textable = NULL; 

  ph->numtextures = 0;
}

static errcode add_texture(parsehandle * ph, void * tex, const char * name) {
  ph->textable[ph->numtextures].tex=tex;
  ph->textable[ph->numtextures].name = malloc(strlen(name) + 1);
  strcpy(ph->textable[ph->numtextures].name, name); 
  rt_hash_insert(&ph->texhash, ph->textable[ph->numtextures].name, ph->numtextures);

  ph->numtextures++;
  if (ph->numtextures >= ph->maxtextures) {
    texentry * newblock;
    int newsize;

    newsize = 2 * ph->maxtextures;
    newblock = realloc(ph->textable, newsize * sizeof(texentry)); 
    if (newblock != NULL) {
      ph->maxtextures = newsize;
      ph->textable = newblock;
      return PARSENOERR;
    } else {
      printf("Parse: %d textures allocated, texture slots full!\n", ph->numtextures);
      ph->numtextures--; /* keep writing over last texture if we've run out.. */
      return PARSEALLOCERR;
    }
  }

  return PARSENOERR;
}

static void * find_texture(parsehandle * ph, const char * name) {
  int i;

  i=rt_hash_lookup(&ph->texhash, name);
  if (i != HASH_FAIL) {
    return ph->textable[i].tex;
  }

  printf("Undefined texture '%s', using default. \n", name);
  return(ph->defaulttex.tex); 
}

apiflt degtorad(apiflt deg) {
  apiflt tmp;
  tmp=deg * 3.1415926 / 180.0;
  return tmp;
}

static void degvectoradvec(apivector * degvec) {
  apivector tmp;

  tmp.x=degtorad(degvec->x);
  tmp.y=degtorad(degvec->y);
  tmp.z=degtorad(degvec->z);
  *degvec=tmp;
}

static void InitRot3d(RotMat * rot, apiflt x, apiflt y, apiflt z) {
  rot->rx1=cos(y)*cos(z);
  rot->rx2=sin(x)*sin(y)*cos(z) - cos(x)*sin(z);
  rot->rx3=sin(x)*sin(z) + cos(x)*cos(z)*sin(y);
  
  rot->ry1=cos(y)*sin(z);
  rot->ry2=cos(x)*cos(z) + sin(x)*sin(y)*sin(z);
  rot->ry3=cos(x)*sin(y)*sin(z) - sin(x)*cos(z);

  rot->rz1=sin(y);
  rot->rz2=sin(x)*cos(y);
  rot->rz3=cos(x)*cos(y);
}

static void Rotate3d(RotMat * rot, apivector * vec) {
  apivector tmp;
  tmp.x=(vec->x*(rot->rx1) + vec->y*(rot->rx2) + vec->z*(rot->rx3));
  tmp.y=(vec->x*(rot->ry1) + vec->y*(rot->ry2) + vec->z*(rot->ry3));
  tmp.z=(vec->x*(rot->rz1) + vec->y*(rot->rz2) + vec->z*(rot->rz3));
  *vec=tmp; 
}

static void Scale3d(apivector * scale, apivector * vec) {
  vec->x=vec->x * scale->x;
  vec->y=vec->y * scale->y;
  vec->z=vec->z * scale->z;
}

static void Trans3d(apivector * trans, apivector * vec) {
  vec->x+=trans->x;
  vec->y+=trans->y;
  vec->z+=trans->z;
}

static void PrintSyntaxError(parsehandle * ph, 
                             const char * string, const char * found) {
  long streampos, readsize;
  long i, j, linecount;
  char cbuf[ERROR_READBUF_SIZE];

  streampos = ftell(ph->ifp);

  /* count lines up to approximate position where error occured */ 
  fseek(ph->ifp, 0, SEEK_SET); 

  i=0;
  linecount=0;
  while (i < streampos) {
    if ((streampos - i) > ERROR_READBUF_SIZE) {
      readsize = ERROR_READBUF_SIZE;
    } else {
      readsize = streampos - i;
    }

    fread(cbuf, readsize, 1, ph->ifp);
    i+=readsize;
    for (j=0; j<readsize; j++) {
      if (cbuf[j] == '\n') {
        linecount++;
      }
    } 
  }

  printf("Parse Error:\n");
  printf("   Encountered a syntax error in file %s\n", ph->filename); 
  printf("   Expected to find %s\n", string);
  printf("   Actually found: %s\n", found);
  printf("   Error occured at or prior to file offset %ld, line %ld\n",
         streampos, linecount);
  printf("   Error position is only approximate, but should be close\n\n");

  fseek(ph->ifp, streampos, SEEK_SET); /* return to previous offset */
}

static errcode GetString(parsehandle * ph, const char * string) {
  char data[255];

  fscanf(ph->ifp, "%s", data);
  if (stringcmp(data, string) != 0) {
    PrintSyntaxError(ph, string, data);
    return PARSEBADSYNTAX;
  }

  return PARSENOERR;
}

unsigned int readmodel(const char * modelfile, SceneHandle scene) {
  parsehandle ph;
  errcode rc;

  memset(&ph, 0, sizeof(ph));
  ph.filename = modelfile;
  ph.ifp=fopen(modelfile, "r");
  if (ph.ifp == NULL) {
    return PARSEBADFILE;
  }

  reset_tex_table(&ph, scene); 

  rc = GetScenedefs(&ph, scene); 
  if (rc == PARSENOERR) {
    ph.numobjectsparsed=0;
    while ((rc = GetObject(&ph, scene)) == PARSENOERR) {
      ph.numobjectsparsed++;
    } 
    fclose(ph.ifp);
  
    if (rc == PARSEEOF)
      rc = PARSENOERR;
  }

  free_tex_table(&ph, scene);

  return rc;
}

static errcode ReadIncludeFile(parsehandle * ph, const char * includefile, SceneHandle scene) {
  errcode rc;
  const char * oldfilename = ph->filename;
  FILE * oldfp = ph->ifp;

  if (strcmp(includefile, ph->filename) == 0) {
    printf("Warning: possible self-recursive include of file %s\n", 
           includefile);
  }

  ph->filename=includefile;
  ph->ifp=fopen(includefile, "r");
  if (ph->ifp == NULL) {
    printf("Parser failed trying to open file: %s\n", includefile);

    /* restore old file pointers etc */
    ph->filename=oldfilename;
    ph->ifp = oldfp;

    return PARSEBADSUBFILE;
  }

  while ((rc = GetObject(ph, scene)) == PARSENOERR) {
    ph->numobjectsparsed++;
  } 
  fclose(ph->ifp);

  /* restore old file pointers etc */
  ph->filename=oldfilename;
  ph->ifp = oldfp;
  
  if (rc == PARSEEOF){
    rc = PARSENOERR;
  }

  return rc;
}

static errcode GetScenedefs(parsehandle * ph, SceneHandle scene) {
  int xres, yres;
  errcode rc = PARSENOERR;

  rc |= GetString(ph, "BEGIN_SCENE"); 
  
  rc |= GetString(ph, "RESOLUTION");
  fscanf(ph->ifp, "%d %d", &xres, &yres);

  rt_outputfile(scene, "outfile.tga");
  rt_resolution(scene, xres, yres);
  rt_verbose(scene, 0);

  return rc;
}

static errcode GetCamera(parsehandle * ph, SceneHandle scene) {
  apivector Ccenter, Cview, Cup;
  apiflt zoom, aspectratio;
  int raydepth, antialiasing;
  float a, b, c, d;
  errcode rc = PARSENOERR;
  char data[255];

  fscanf(ph->ifp, "%s", data);
  if (stringcmp(data, "PROJECTION") == 0) {
    fscanf(ph->ifp, "%s", data);
    if (stringcmp(data, "FISHEYE") ==0) {
      rt_camera_projection(scene, RT_PROJECTION_FISHEYE);
    } else if (stringcmp(data, "PERSPECTIVE") ==0) {
      rt_camera_projection(scene, RT_PROJECTION_PERSPECTIVE);
    } else if (stringcmp(data, "PERSPECTIVE_DOF") ==0) {
      rt_camera_projection(scene, RT_PROJECTION_PERSPECTIVE_DOF);

      rc |= GetString(ph, "FOCALLENGTH");
      fscanf(ph->ifp, "%f", &a);  

      rc |= GetString(ph, "APERTURE");
      fscanf(ph->ifp, "%f", &b);  

      rt_camera_dof(scene, a, b);
    } else if (stringcmp(data, "ORTHOGRAPHIC") ==0) {
      rt_camera_projection(scene, RT_PROJECTION_ORTHOGRAPHIC);
    }

    rc |= GetString(ph, "ZOOM");
    fscanf(ph->ifp, "%f", &a);  
    zoom=a;
  } else if (stringcmp(data, "ZOOM") == 0) {
    fscanf(ph->ifp, "%f", &a);
    zoom=a;
  } else {
    rc = PARSEBADSYNTAX;
    return rc;
  }

  rc |= GetString(ph, "ASPECTRATIO");
  fscanf(ph->ifp, "%f", &b);  
  aspectratio=b;

  rc |= GetString(ph, "ANTIALIASING");
  fscanf(ph->ifp, "%d", &antialiasing);

  rc |= GetString(ph, "RAYDEPTH");
  fscanf(ph->ifp, "%d", &raydepth);

  rc |= GetString(ph, "CENTER");
  fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
  Ccenter.x = a;
  Ccenter.y = b;
  Ccenter.z = c;

  rc |= GetString(ph, "VIEWDIR");
  fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
  Cview.x = a;
  Cview.y = b;
  Cview.z = c;

  rc |= GetString(ph, "UPDIR");
  fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
  Cup.x = a;
  Cup.y = b;
  Cup.z = c;

  rt_camera_setup(scene, zoom, aspectratio, antialiasing, raydepth,
                  Ccenter, Cview, Cup);

  fscanf(ph->ifp, "%s", data);
  if (stringcmp(data, "FRUSTUM") == 0) {
    fscanf(ph->ifp, "%f %f %f %f", &a, &b, &c, &d);
    rt_camera_frustum(scene, a, b, c, d);
    fscanf(ph->ifp, "%s", data);
    if (stringcmp(data, "END_CAMERA") != 0) {
      rc |= PARSEBADSYNTAX;
      return rc;
    }
  } else if (stringcmp(data, "END_CAMERA") != 0) {
    rc |= PARSEBADSYNTAX;
    return rc;
  }


  return rc;
}

static errcode GetObject(parsehandle * ph, SceneHandle scene) {
  char objtype[256];
 
  if (fscanf(ph->ifp, "%s", objtype) == EOF) {
    return PARSEEOF;
  }
  if (!stringcmp(objtype, "TRI")) {
    return GetTri(ph, scene);
  }
  if (!stringcmp(objtype, "STRI")) {
    return GetSTri(ph, scene);
  }
  if (!stringcmp(objtype, "VCSTRI")) {
    return GetVCSTri(ph, scene);
  }
  if (!stringcmp(objtype, "SPHERE")) {
    return GetSphere(ph, scene);
  }
  if (!stringcmp(objtype, "FCYLINDER")) {
    return GetFCylinder(ph, scene);
  }
  if (!stringcmp(objtype, "RING")) {
    return GetRing(ph, scene);
  }
  if (!stringcmp(objtype, "POLYCYLINDER")) {
    return GetPolyCylinder(ph, scene);
  }
  if (!stringcmp(objtype, "CYLINDER")) {
    return GetCylinder(ph, scene);
  }
  if (!stringcmp(objtype, "PLANE")) {
    return GetPlane(ph, scene);
  }
  if (!stringcmp(objtype, "BOX")) {
    return GetBox(ph, scene);
  }
  if (!stringcmp(objtype, "SCALARVOL")) {
    return GetVol(ph, scene);
  }
  if (!stringcmp(objtype, "TEXDEF")) {
    return GetTexDef(ph, scene);
  }	
  if (!stringcmp(objtype, "TEXALIAS")) {
    return GetTexAlias(ph);
  }
  if (!stringcmp(objtype, "LIGHT")) {
    return GetLight(ph, scene);
  }
  if (!stringcmp(objtype, "DIRECTIONAL_LIGHT")) {
    return GetDirLight(ph, scene);
  }
  if (!stringcmp(objtype, "SPOTLIGHT")) {
    return GetSpotLight(ph, scene);
  }
  if (!stringcmp(objtype, "SCAPE")) {
    return GetLandScape(ph, scene);
  }
  if (!stringcmp(objtype, "CAMERA")) {
    return GetCamera(ph, scene);
  }
  if (!stringcmp(objtype, "TPOLYFILE")) {
    return GetTPolyFile(ph, scene);
  }
  if (!stringcmp(objtype, "MGFFILE")) {
#ifdef USELIBMGF
    return GetMGFFile(ph, scene);
#else
    printf("MGF File Parsing is not available in this build.\n");
    return PARSEBADSYNTAX;
#endif
  }
  if (!stringcmp(objtype, "#")) {
    int c;
    while (1) {
      c=fgetc(ph->ifp);
      if (c == EOF || c == '\n')    /* eat comment text */
        return PARSENOERR;
    } 
  }
  if (!stringcmp(objtype, "BACKGROUND")) {
    return GetBackGnd(ph, scene);
  }
  if (!stringcmp(objtype, "FOG")) {
    return GetFog(ph, scene);
  }
  if (!stringcmp(objtype, "INCLUDE")) {
    char includefile[FILENAME_MAX];
    fscanf(ph->ifp, "%s", includefile);
    return ReadIncludeFile(ph, includefile, scene);
  }
  if (!stringcmp(objtype, "START_CLIPGROUP")) {
    return GetClipGroup(ph, scene);
  }
  if (!stringcmp(objtype, "END_CLIPGROUP")) {
    return GetClipGroupEnd(ph, scene);
  }
  if (!stringcmp(objtype, "END_SCENE")) {
    return PARSEEOF; /* end parsing */
  }

  PrintSyntaxError(ph, "an object or other declaration", objtype);

  return PARSEBADSYNTAX;
}

static errcode GetVector(parsehandle * ph, apivector * v1) {
  float a, b, c;
 
  if (fscanf(ph->ifp, "%f %f %f", &a, &b, &c) != 3) 
    return PARSEBADSYNTAX;

  v1->x=a;
  v1->y=b;
  v1->z=c;

  return PARSENOERR;
}

static errcode GetFloat(parsehandle * ph, apiflt * f) {
  float a;
  if (fscanf(ph->ifp, "%f", &a) != 1) 
    return PARSEBADSYNTAX;

  *f = a;

  return PARSENOERR;
}

static errcode GetColor(parsehandle * ph, apicolor * c1) {
  float r, g, b;
  int rc; 

  rc = GetString(ph, "COLOR"); 
  fscanf(ph->ifp, "%f %f %f", &r, &g, &b);
  c1->r=r;
  c1->g=g;
  c1->b=b;

  return rc;
}

static errcode GetTexDef(parsehandle * ph, SceneHandle scene) {
  char texname[TEXNAMELEN];

  fscanf(ph->ifp, "%s", texname);
  add_texture(ph, GetTexBody(ph, scene, 0), texname); 

  return PARSENOERR;
}

static errcode GetTexAlias(parsehandle * ph) {
  char texname[TEXNAMELEN];
  char aliasname[TEXNAMELEN];

  fscanf(ph->ifp, "%s", texname);
  fscanf(ph->ifp, "%s", aliasname);
  add_texture(ph, find_texture(ph, aliasname), texname); 

  return PARSENOERR;
}


static errcode GetTexture(parsehandle * ph, SceneHandle scene, void ** tex) {
  char tmp[255];
  errcode rc = PARSENOERR;

  fscanf(ph->ifp, "%s", tmp);
  if (!stringcmp(tmp, "TEXTURE")) {	
    *tex = GetTexBody(ph, scene, 0);
  }
  else
    *tex = find_texture(ph, tmp);

  return rc;
}

void * GetTexBody(parsehandle * ph, SceneHandle scene, int modeflag) {
  char tmp[255];
  float a,b,c,d, phong, phongexp;
  int phongtype;
  apitexture tex;
  void * voidtex; 
  errcode rc;

  rc = GetString(ph, "AMBIENT");
  fscanf(ph->ifp, "%f", &a); 
  tex.ambient=a;

  rc |= GetString(ph, "DIFFUSE");
  fscanf(ph->ifp, "%f", &b);
  tex.diffuse=b;

  rc |= GetString(ph, "SPECULAR");
  fscanf(ph->ifp, "%f", &c);
  tex.specular=c;

  rc |= GetString(ph, "OPACITY");
  fscanf(ph->ifp, "%f", &d);  
  tex.opacity=d;

  fscanf(ph->ifp, "%s", tmp);
  if (!stringcmp(tmp, "PHONG")) {
    fscanf(ph->ifp, "%s", tmp);
    if (!stringcmp(tmp, "METAL")) {
      phongtype = RT_PHONG_METAL;
    }
    else if (!stringcmp(tmp, "PLASTIC")) {
      phongtype = RT_PHONG_PLASTIC;
    }
    else {
      phongtype = RT_PHONG_PLASTIC;
    } 

    fscanf(ph->ifp, "%f", &phong);
    GetString(ph, "PHONG_SIZE");
    fscanf(ph->ifp, "%f", &phongexp);
    fscanf(ph->ifp, "%s", tmp);
  } else { 
    /* assume we found "COLOR" otherwise */
    phong = 0.0;
    phongexp = 100.0;
    phongtype = RT_PHONG_PLASTIC;
  }

  /* if we're processing normal objects, use the regular */
  /* texture definition pattern.                         */
  /* VCSTri objects skip the normal color and texture    */
  /* function definition since they are unused.          */ 
  if (modeflag == 0) { 
    if (stringcmp(tmp, "COLOR")) {
      rc |= PARSEBADSYNTAX;
    }

    fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
    tex.col.r = a;
    tex.col.g = b;
    tex.col.b = c;
 
    rc |= GetString(ph, "TEXFUNC");

    /* this really ought to be a string, not a number... */
    fscanf(ph->ifp, "%d", &tex.texturefunc);

    switch (tex.texturefunc) {
      case RT_TEXTURE_CONSTANT:
      default: 
        break;

      case RT_TEXTURE_3D_CHECKER:
      case RT_TEXTURE_GRIT:
      case RT_TEXTURE_MARBLE:
      case RT_TEXTURE_WOOD:
      case RT_TEXTURE_GRADIENT:
      case RT_TEXTURE_CYLINDRICAL_CHECKER:
        rc |= GetString(ph, "CENTER");
        rc |= GetVector(ph, &tex.ctr);
        rc |= GetString(ph, "ROTATE");
        rc |= GetVector(ph, &tex.rot);
        rc |= GetString(ph, "SCALE");
        rc |= GetVector(ph, &tex.scale);
        break;

      case RT_TEXTURE_CYLINDRICAL_IMAGE:
      case RT_TEXTURE_SPHERICAL_IMAGE:
        fscanf(ph->ifp, "%s", tex.imap);
        rc |= GetString(ph, "CENTER");
        rc |= GetVector(ph, &tex.ctr);
        rc |= GetString(ph, "ROTATE");
        rc |= GetVector(ph, &tex.rot);
        rc |= GetString(ph, "SCALE");
        rc |= GetVector(ph, &tex.scale);
        break;
     
      case RT_TEXTURE_PLANAR_IMAGE:
        fscanf(ph->ifp, "%s", tex.imap);
        rc |= GetString(ph, "CENTER");
        rc |= GetVector(ph, &tex.ctr);
        rc |= GetString(ph, "ROTATE");
        rc |= GetVector(ph, &tex.rot);
        rc |= GetString(ph, "SCALE");
        rc |= GetVector(ph, &tex.scale);
        rc |= GetString(ph, "UAXIS");
        rc |= GetVector(ph, &tex.uaxs);
        rc |= GetString(ph, "VAXIS");
        rc |= GetVector(ph, &tex.vaxs);
        break;
    }
  } else {
    if (stringcmp(tmp, "VCST")) {
      rc |= PARSEBADSYNTAX;
    }

    /* if we're processing VCSTri objects, set some defaults */
    tex.col.r = 1.0;
    tex.col.g = 1.0;
    tex.col.b = 1.0;
    tex.texturefunc = 0; /* set to none by default, gets reset anyway */
  }

  voidtex = rt_texture(scene, &tex);
  rt_tex_phong(voidtex, phong, phongexp, phongtype);

  return voidtex;
}

static errcode GetDirLight(parsehandle * ph, SceneHandle scene) {
  char tmp[255];
  apivector dir;
  apitexture tex;
  float r, g, b;
  errcode rc;

  memset(&tex, 0, sizeof(apitexture)); 

  rc = GetString(ph, "DIRECTION"); 
  rc |= GetVector(ph, &dir); 

  fscanf(ph->ifp, "%s", tmp);
  if (!stringcmp(tmp, "COLOR")) {
    fscanf(ph->ifp, "%f %f %f", &r, &g, &b);
    tex.col.r=r;
    tex.col.g=g;
    tex.col.b=b;

    rt_directional_light(scene, rt_texture(scene, &tex), dir);
  }

  return rc;
}

static errcode GetLight(parsehandle * ph, SceneHandle scene) {
  char tmp[255];
  apiflt rad, Kc, Kl, Kq;
  apivector ctr;
  apitexture tex;
  float r, g, b, a;
  errcode rc;
  void * li;

  memset(&tex, 0, sizeof(apitexture)); 

  rc = GetString(ph, "CENTER"); 
  rc |= GetVector(ph, &ctr); 
  rc |= GetString(ph, "RAD");
  fscanf(ph->ifp, "%f", &a);  /* read in radius */ 
  rad=a;

  fscanf(ph->ifp, "%s", tmp);
  if (!stringcmp(tmp, "COLOR")) {
    fscanf(ph->ifp, "%f %f %f", &r, &g, &b);
    tex.col.r=r;
    tex.col.g=g;
    tex.col.b=b;

    li = rt_light(scene, rt_texture(scene, &tex), ctr, rad);
  }
  else { 
    if (stringcmp(tmp, "ATTENUATION"))
      return -1;

    rc |= GetString(ph, "CONSTANT");
    fscanf(ph->ifp, "%f", &a); 
    Kc=a;
    rc |= GetString(ph, "LINEAR");
    fscanf(ph->ifp, "%f", &a); 
    Kl=a;
    rc |= GetString(ph, "QUADRATIC");
    fscanf(ph->ifp, "%f", &a); 
    Kq=a;
    rc |= GetColor(ph, &tex.col);

    li = rt_light(scene, rt_texture(scene, &tex), ctr, rad);

    rt_light_attenuation(li, Kc, Kl, Kq);
  } 

  return rc;
}

static errcode GetSpotLight(parsehandle * ph, SceneHandle scene) {
  char tmp[255];
  apiflt rad, Kc, Kl, Kq;
  apivector ctr;
  apitexture tex;
  apivector direction;
  apiflt start, end;
  float r, g, b, a;
  errcode rc;
  void * li;

  memset(&tex, 0, sizeof(apitexture)); 

  rc = GetString(ph, "CENTER"); 
  rc |= GetVector(ph, &ctr); 
  rc |= GetString(ph,"RAD");
  fscanf(ph->ifp, "%f", &a);  /* read in radius */ 
  rad=a;
 
  rc |= GetString(ph, "DIRECTION"); 
  rc |= GetVector(ph, &direction); 
  rc |= GetString(ph, "FALLOFF_START");
  fscanf(ph->ifp, "%f",&a);
  start=a;
  rc |= GetString(ph, "FALLOFF_END");
  fscanf(ph->ifp, "%f", &a);
  end=a;
   
  fscanf(ph->ifp, "%s", tmp);
  if (!stringcmp(tmp, "COLOR")) {
    fscanf(ph->ifp, "%f %f %f", &r, &g, &b);
    tex.col.r=r;
    tex.col.g=g;
    tex.col.b=b;

    li = rt_spotlight(scene, rt_texture(scene, &tex), ctr, rad, direction, start, end);
  } 
  else {
    if (stringcmp(tmp, "ATTENUATION"))
      return -1;
    rc |= GetString(ph, "CONSTANT");
    fscanf(ph->ifp, "%f", &a);
    Kc=a;
    rc |= GetString(ph, "LINEAR");
    fscanf(ph->ifp, "%f", &a);
    Kl=a;
    rc |= GetString(ph, "QUADRATIC");
    fscanf(ph->ifp, "%f", &a);
    Kq=a;
    rc |= GetColor(ph, &tex.col);

    li = rt_spotlight(scene, rt_texture(scene, &tex), ctr, rad, direction, start, end);
    rt_light_attenuation(li, Kc, Kl, Kq);
  }

  return rc;
}


static errcode GetFog(parsehandle * ph, SceneHandle scene) {
  char tmp[255];
  apicolor fogcol; 
  float start, end, density;
  errcode rc = PARSENOERR;
 
  fscanf(ph->ifp, "%s", tmp); 
  if (!stringcmp(tmp, "LINEAR")) {
    rt_fog_mode(scene, RT_FOG_LINEAR);
  } else if (!stringcmp(tmp, "EXP")) {
    rt_fog_mode(scene, RT_FOG_EXP);
  } else if (!stringcmp(tmp, "EXP2")) {
    rt_fog_mode(scene, RT_FOG_EXP2);
  } else if (!stringcmp(tmp, "OFF")) {
    rt_fog_mode(scene, RT_FOG_NONE);
  }

  rc |= GetString(ph, "START");
  fscanf(ph->ifp, "%f", &start);

  rc |= GetString(ph, "END");
  fscanf(ph->ifp, "%f", &end);

  rc |= GetString(ph, "DENSITY");
  fscanf(ph->ifp, "%f", &density);

  rc |= GetColor(ph, &fogcol);

  rt_fog_parms(scene, fogcol, start, end, density);

  return PARSENOERR;
}


static errcode GetBackGnd(parsehandle * ph, SceneHandle scene) {
  float r,g,b;
  apicolor scenebackcol; 
  
  fscanf(ph->ifp, "%f %f %f", &r, &g, &b);

  scenebackcol.r=r;
  scenebackcol.g=g;
  scenebackcol.b=b;
  rt_background(scene, scenebackcol);

  return PARSENOERR;
}


static errcode GetCylinder(parsehandle * ph, SceneHandle scene) {
  apiflt rad;
  apivector ctr, axis;
  void * tex;
  float a;
  errcode rc;

  rc = GetString(ph, "CENTER");
  rc |= GetVector(ph, &ctr);
  rc |= GetString(ph, "AXIS");
  rc |= GetVector(ph, &axis);
  rc |= GetString(ph, "RAD");
  fscanf(ph->ifp, "%f", &a);
  rad=a;

  rc |= GetTexture(ph, scene, &tex);
  rt_cylinder(scene, tex, ctr, axis, rad); 

  return rc;
}

static errcode GetFCylinder(parsehandle * ph, SceneHandle scene) {
  apiflt rad;
  apivector ctr, axis;
  apivector pnt1, pnt2;
  void * tex;
  float a;
  errcode rc;

  rc = GetString(ph, "BASE");
  rc |= GetVector(ph, &pnt1);
  rc |= GetString(ph, "APEX");
  rc |= GetVector(ph, &pnt2);

  ctr=pnt1;
  axis.x=pnt2.x - pnt1.x; 
  axis.y=pnt2.y - pnt1.y;
  axis.z=pnt2.z - pnt1.z;

  rc |= GetString(ph, "RAD");
  fscanf(ph->ifp, "%f", &a);
  rad=a;

  rc |= GetTexture(ph, scene, &tex);
  rt_fcylinder(scene, tex, ctr, axis, rad); 

  return rc;
}
 
static errcode GetPolyCylinder(parsehandle * ph, SceneHandle scene) {
  apiflt rad;
  apivector * temp;
  void * tex;
  float a;
  int numpts, i;
  errcode rc;

  rc = GetString(ph, "POINTS");
  fscanf(ph->ifp, "%d", &numpts);

  temp = (apivector *) malloc(numpts * sizeof(apivector));

  for (i=0; i<numpts; i++) {
    rc |= GetVector(ph, &temp[i]);
  }         

  rc |= GetString(ph, "RAD");
  fscanf(ph->ifp, "%f", &a);
  rad=a;

  rc |= GetTexture(ph, scene, &tex);
  rt_polycylinder(scene, tex, temp, numpts, rad); 

  free(temp);

  return rc;
}
 

static errcode GetSphere(parsehandle * ph, SceneHandle scene) {
  apiflt rad;
  apivector ctr;
  void * tex;
  float a;
  errcode rc;
 
  rc = GetString(ph, "CENTER");
  rc |= GetVector(ph, &ctr); 
  rc |= GetString(ph, "RAD");
  fscanf(ph->ifp, "%f", &a); 
  rad=a;

  rc |= GetTexture(ph, scene, &tex); 
 
  rt_sphere(scene, tex, ctr, rad);

  return rc;
}

static errcode GetPlane(parsehandle * ph, SceneHandle scene) {
  apivector normal;
  apivector ctr;
  void * tex;
  errcode rc;

  rc = GetString(ph, "CENTER");
  rc |= GetVector(ph, &ctr);
  rc |= GetString(ph, "NORMAL");
  rc |= GetVector(ph, &normal);
  rc |= GetTexture(ph, scene, &tex);

  rt_plane(scene, tex, ctr, normal);

  return rc;
}

static errcode GetVol(parsehandle * ph, SceneHandle scene) {
  apivector min, max;
  int x,y,z;  
  char fname[255];
  void * tex;
  errcode rc;
 
  rc = GetString(ph, "MIN");
  rc |= GetVector(ph, &min);
  rc |= GetString(ph, "MAX");
  rc |= GetVector(ph, &max);
  rc |= GetString(ph, "DIM");
  fscanf(ph->ifp, "%d %d %d ", &x, &y, &z);
  rc |= GetString(ph, "FILE");
  fscanf(ph->ifp, "%s", fname);  
  rc |= GetTexture(ph, scene, &tex);
 
  rt_scalarvol(scene, tex, min, max, x, y, z, fname, NULL); 

  return rc;
}

static errcode GetBox(parsehandle * ph, SceneHandle scene) {
  apivector min, max;
  void * tex;
  errcode rc;

  rc = GetString(ph, "MIN");
  rc |= GetVector(ph, &min);
  rc |= GetString(ph, "MAX");
  rc |= GetVector(ph, &max);
  rc |= GetTexture(ph, scene, &tex);

  rt_box(scene, tex, min, max);

  return rc;
}

static errcode GetRing(parsehandle * ph, SceneHandle scene) {
  apivector normal;
  apivector ctr;
  void * tex;
  float a,b;
  errcode rc;
 
  rc = GetString(ph, "CENTER");
  rc |= GetVector(ph, &ctr);
  rc |= GetString(ph, "NORMAL");
  rc |= GetVector(ph, &normal);
  rc |= GetString(ph, "INNER");
  fscanf(ph->ifp, " %f ", &a);
  rc |= GetString(ph, "OUTER");
  fscanf(ph->ifp, " %f ", &b);
  rc |= GetTexture(ph, scene, &tex);
 
  rt_ring(scene, tex, ctr, normal, a, b);

  return rc;
}

static errcode GetTri(parsehandle * ph, SceneHandle scene) {
  apivector v0,v1,v2;
  void * tex;
  errcode rc;

  rc = GetString(ph, "V0");
  rc |= GetVector(ph, &v0);

  rc |= GetString(ph, "V1");
  rc |= GetVector(ph, &v1);

  rc |= GetString(ph, "V2");
  rc |= GetVector(ph, &v2);

  rc |= GetTexture(ph, scene, &tex);

  rt_tri(scene, tex, v0, v1, v2);

  return rc;
}

static errcode GetSTri(parsehandle * ph, SceneHandle scene) {
  apivector v0,v1,v2,n0,n1,n2;
  void * tex;
  errcode rc;

  rc = GetString(ph, "V0");
  rc |= GetVector(ph, &v0);

  rc |= GetString(ph, "V1");
  rc |= GetVector(ph, &v1);

  rc |= GetString(ph, "V2");
  rc |= GetVector(ph, &v2);
  
  rc |= GetString(ph, "N0");
  rc |= GetVector(ph, &n0);

  rc |= GetString(ph, "N1");
  rc |= GetVector(ph, &n1);

  rc |= GetString(ph, "N2");
  rc |= GetVector(ph, &n2);

  rc |= GetTexture(ph, scene, &tex);
  
  rt_stri(scene, tex, v0, v1, v2, n0, n1, n2);

  return rc;
}

static errcode GetVCSTri(parsehandle * ph, SceneHandle scene) {
  apivector v0,v1,v2,n0,n1,n2;
  apicolor  c0,c1,c2;
  float a, b, c;

  void * tex;
  errcode rc;

  rc = GetString(ph, "V0");
  rc |= GetVector(ph, &v0);

  rc |= GetString(ph, "V1");
  rc |= GetVector(ph, &v1);

  rc |= GetString(ph, "V2");
  rc |= GetVector(ph, &v2);
  
  rc |= GetString(ph, "N0");
  rc |= GetVector(ph, &n0);

  rc |= GetString(ph, "N1");
  rc |= GetVector(ph, &n1);

  rc |= GetString(ph, "N2");
  rc |= GetVector(ph, &n2);

  rc |= GetString(ph, "C0");
  fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
  c0.r = a;
  c0.g = b;
  c0.b = c;

  rc |= GetString(ph, "C1");
  fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
  c1.r = a;
  c1.g = b;
  c1.b = c;

  rc |= GetString(ph, "C2");
  fscanf(ph->ifp, "%f %f %f", &a, &b, &c);
  c2.r = a;
  c2.g = b;
  c2.b = c;

  tex = GetTexBody(ph, scene, 1);
  
  rt_vcstri(scene, tex, v0, v1, v2, n0, n1, n2, c0, c1, c2);

  return rc;
}


static errcode GetLandScape(parsehandle * ph, SceneHandle scene) {
  void * tex;
  apivector ctr;
  apiflt wx, wy;
  int m, n;
  float a,b;
  errcode rc;

  rc = GetString(ph, "RES");
  fscanf(ph->ifp, "%d %d", &m, &n);

  rc |= GetString(ph, "SCALE");
  fscanf(ph->ifp, "%f %f", &a, &b);   
  wx=a;
  wy=b;

  rc |= GetString(ph, "CENTER");
  rc |= GetVector(ph, &ctr);

  rc |= GetTexture(ph, scene, &tex);

  rt_landscape(scene, tex, m, n, ctr, wx, wy);

  return rc;
}

static errcode GetTPolyFile(parsehandle * ph, SceneHandle scene) {
  void * tex;
  apivector ctr, rot, scale;
  apivector v1, v2, v0;
  char ifname[255];
  FILE *ifp;
  int v, totalpolys;
  RotMat RotA;
  errcode rc;

  totalpolys=0;

  rc = GetString(ph, "SCALE"); 
  rc |= GetVector(ph, &scale);

  rc |= GetString(ph, "ROT");
  rc |= GetVector(ph, &rot);

  degvectoradvec(&rot); 
  InitRot3d(&RotA, rot.x, rot.y, rot.z);

  rc |= GetString(ph, "CENTER");
  rc |= GetVector(ph, &ctr);

  rc |= GetString(ph, "FILE");
  fscanf(ph->ifp, "%s", ifname);

  rc |= GetTexture(ph, scene, &tex);

  if ((ifp=fopen(ifname, "r")) == NULL) {
    printf("Can't open data file %s for input!! Aborting...\n", ifname);
    return PARSEBADSUBFILE;
  }

  while (!feof(ifp)) {
    fscanf(ifp, "%d", &v);
    if (v != 3) { break; }

    totalpolys++;
    v=0; 
     
    rc |= GetVector(ph, &v0);
    rc |= GetVector(ph, &v1);
    rc |= GetVector(ph, &v2);

    Scale3d(&scale, &v0);
    Scale3d(&scale, &v1);
    Scale3d(&scale, &v2);

    Rotate3d(&RotA, &v0); 
    Rotate3d(&RotA, &v1); 
    Rotate3d(&RotA, &v2); 

    Trans3d(&ctr, &v0);
    Trans3d(&ctr, &v1);
    Trans3d(&ctr, &v2);

    rt_tri(scene, tex, v1, v0, v2);
  }

  fclose(ifp);

  return rc;
}


static errcode GetClipGroup(parsehandle * ph, SceneHandle scene) {
  char objtype[256];

  if (fscanf(ph->ifp, "%s", objtype) == EOF) {
    return PARSEEOF;
  }

  if (!stringcmp(objtype, "NUMPLANES")) {
    int numplanes, i; 
    float * planes;

    if (fscanf(ph->ifp, "%d", &numplanes) != 1)
      return PARSEBADSYNTAX;

    planes = (float *) malloc(numplanes * 4 * sizeof(float));

    for (i=0; i<(numplanes * 4); i++) {
      if (fscanf(ph->ifp, "%f", &planes[i]) != 1)
        return PARSEBADSYNTAX;
    } 

    rt_clip_fv(scene, numplanes, planes);
    free(planes);

    return PARSENOERR;
  }

  return PARSEBADSYNTAX;
}


static errcode GetClipGroupEnd(parsehandle * ph, SceneHandle scene) {
  rt_clip_off(scene);
  return PARSENOERR;
}


#ifdef USELIBMGF
static errcode GetMGFFile(parsehandle * ph, SceneHandle scene) {
  char ifname[255];

  fscanf(ph->ifp, "%s", ifname); /* get MGF filename */
  if (ParseMGF(ifname, scene, 0) == MGF_NOERR)
    return PARSENOERR;
  
  return PARSENOERR; /* XXX hack */ 
}
#endif



 
