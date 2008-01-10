/*
 * api.c - This file contains all of the API calls that are defined for
 *         external driver code to use.  
 * 
 *  $Id: api.c,v 1.140 2004/06/01 16:00:28 johns Exp $
 */

#include "machine.h"
#include "types.h"
#include "macros.h"

#include "parallel.h"
#include "threads.h"

#include "box.h"
#include "cylinder.h"
#include "plane.h"
#include "quadric.h"
#include "ring.h"
#include "sphere.h"
#include "triangle.h"
#include "vol.h"
#include "extvol.h"

#include "texture.h"
#include "light.h"
#include "render.h"
#include "trace.h"
#include "camera.h"
#include "vector.h"
#include "intersect.h"
#include "shade.h"
#include "util.h"
#include "imap.h"
#include "global.h"
#include "ui.h"
#include "shade.h"

typedef void * SceneHandle;
typedef vector apivector;
typedef color  apicolor;

typedef struct {
  int texturefunc; /* which texture function to use */
  apicolor col;    /* base object color */
  int shadowcast;  /* does the object cast a shadow */
  apiflt ambient;  /* ambient lighting */
  apiflt diffuse;  /* diffuse reflection */
  apiflt specular; /* specular reflection */
  apiflt opacity;  /* how opaque the object is */
  apivector ctr;   /* origin of texture */
  apivector rot;   /* rotation of texture around origin */ 
  apivector scale; /* scale of texture on x,y,z */
  apivector uaxs;  /* planar map U axis */
  apivector vaxs;  /* planar map V axis */
  char * imap[96]; /* name of image map*/
} apitexture;

apivector rt_vector(apiflt x, apiflt y, apiflt z) {
  apivector v;

  v.x = x;
  v.y = y;
  v.z = z;

  return v;
}

apicolor rt_color(float r, float g, float b) {
  apicolor c;
  
  c.r = r;
  c.g = g;
  c.b = b;
  
  return c;
}

int rt_initialize(int * argc, char ***argv) {
  InitTextures();

  if (!parinitted) {
    rt_par_init(argc, argv); 
    parinitted=1;
  }

  return rt_mynode(); /* return our node id */ 
}

void rt_finalize(void) {
  FreeTextures();
  rt_par_finish();
}

void rt_renderscene(SceneHandle voidscene) {
  scenedef * scene = (scenedef *) voidscene;
  renderscene(scene);
}

void rt_aa_maxsamples(SceneHandle voidscene, int maxsamples) {
  scenedef * scene = (scenedef *) voidscene;

  if (maxsamples >= 0)
    scene->antialiasing=maxsamples;
  else  
    scene->antialiasing=0;
}

void rt_camera_setup(SceneHandle voidscene, apiflt zoom, apiflt aspectratio, 
	int antialiasing, int raydepth, 
	apivector camcent, apivector viewvec, apivector upvec) {
  scenedef * scene = (scenedef *) voidscene;

  camerasetup(scene, zoom, camcent, viewvec, upvec);

  scene->aspectratio=aspectratio;

  rt_aa_maxsamples(voidscene, antialiasing);

  scene->raydepth=raydepth; 
}

void rt_camera_projection(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;
  cameraprojection(&scene->camera, mode);
}

void rt_camera_dof(SceneHandle voidscene, flt focallength, flt aperture) {
  scenedef * scene = (scenedef *) voidscene;
  cameradof(&scene->camera, focallength, aperture);
}

void rt_camera_position(SceneHandle voidscene, apivector camcent, 
                        apivector viewvec, apivector upvec) {
  scenedef * scene = (scenedef *) voidscene;
  cameraposition(&scene->camera, camcent, viewvec, upvec);
}

void rt_get_camera_position(SceneHandle voidscene, apivector * camcent, 
                        apivector * viewvec, apivector * upvec, apivector * rightvec) {
  scenedef * scene = (scenedef *) voidscene;
  
  getcameraposition(&scene->camera, camcent, viewvec, upvec, rightvec);
}

void rt_camera_frustum(SceneHandle voidscene, flt left, flt right, flt bottom, flt top) {
  scenedef * scene = (scenedef *) voidscene;
  camerafrustum(&scene->camera, left, right, bottom, top);
}

void rt_outputfile(SceneHandle voidscene, const char * outname) {
  scenedef * scene = (scenedef *) voidscene;
  if (strlen(outname) > 0) {
    strcpy((char *) &scene->outfilename, outname);
    scene->writeimagefile = 1;
  }
  else {
    scene->writeimagefile = 0;
  }
}

void rt_outputformat(SceneHandle voidscene, int format) {
  scenedef * scene = (scenedef *) voidscene;
  scene->imgformat = format; 
}

void rt_resolution(SceneHandle voidscene, int hres, int vres) {
  scenedef * scene = (scenedef *) voidscene;
  scene->hres=hres;
  scene->vres=vres;
  scene->scenecheck = 1;
}

void rt_get_resolution(SceneHandle voidscene, int *hres, int *vres) {
  scenedef * scene = (scenedef *) voidscene;
  *hres = scene->hres;
  *vres = scene->vres;
}

void rt_verbose(SceneHandle voidscene, int v) {
  scenedef * scene = (scenedef *) voidscene;
  scene->verbosemode = v;
}

void rt_rawimage(SceneHandle voidscene, unsigned char *img) {
  scenedef * scene = (scenedef *) voidscene;
  scene->img = img;
  scene->imginternal = 0;
  scene->scenecheck = 1;
}

void rt_set_numthreads(SceneHandle voidscene, int numthreads) {
  scenedef * scene = (scenedef *) voidscene;
#ifdef THR
  if (numthreads > 0) {
    scene->numthreads = numthreads;
  }
  else {
    scene->numthreads = rt_thread_numprocessors();
  }

  /* force set of # kernel threads  */
  rt_thread_setconcurrency(scene->numthreads);

#else
  scene->numthreads = 1;
#endif
  scene->scenecheck = 1;
}

void rt_background(SceneHandle voidscene, apicolor col) {
  scenedef * scene = (scenedef *) voidscene;
  scene->background.r = col.r;
  scene->background.g = col.g;
  scene->background.b = col.b;
}

void rt_fog_parms(SceneHandle voidscene, apicolor col, apiflt start, apiflt end, apiflt density) {
  scenedef * scene = (scenedef *) voidscene;
  scene->fog.col = col;
  scene->fog.start = start;
  scene->fog.end = end;
  scene->fog.density = density;
}

void rt_fog_mode(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;

  switch (mode) {
    case RT_FOG_LINEAR:
      scene->fog.fog_fctn = fog_color_linear;
      break;

    case RT_FOG_EXP:
      scene->fog.fog_fctn = fog_color_exp;
      break;

    case RT_FOG_EXP2:
      scene->fog.fog_fctn = fog_color_exp2;
      break;

    case RT_FOG_NONE: 
    default:
      scene->fog.fog_fctn = NULL;
      break;
  }
}

void rt_trans_mode(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;
  scene->transmode = mode; 
}

void rt_boundmode(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;
  scene->boundmode = mode;
  scene->scenecheck = 1;
}

void rt_boundthresh(SceneHandle voidscene, int threshold) {
  scenedef * scene = (scenedef *) voidscene;
 
  if (threshold > 1) {
    scene->boundthresh = threshold;
  }
  else {
    if (rt_mynode() == 0) {
      rt_ui_message(MSG_0, "Out-of-range automatic bounding threshold.\n");
      rt_ui_message(MSG_0, "Automatic bounding threshold reset to default.\n");
    }
    scene->boundthresh = BOUNDTHRESH;
  }
  scene->scenecheck = 1;
}

void rt_shadermode(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;

  /* Main shader used for whole scene */
  switch (mode) {
    case RT_SHADER_LOWEST:
      scene->shader = (color (*)(void *)) lowest_shader;
      break;
    case RT_SHADER_LOW:
      scene->shader = (color (*)(void *)) low_shader;
      break;
    case RT_SHADER_MEDIUM:
      scene->shader = (color (*)(void *)) medium_shader;
      break;
    case RT_SHADER_HIGH:
      scene->shader = (color (*)(void *)) full_shader;
      break;
    case RT_SHADER_FULL:
      scene->shader = (color (*)(void *)) full_shader;
      break;
    case RT_SHADER_AUTO:
    default:
      scene->shader = NULL;
      break;
  }
}

void rt_phong_shader(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;
  switch (mode) {
    case RT_SHADER_NULL_PHONG:
      scene->phongfunc = shade_nullphong;
      break;
    case RT_SHADER_BLINN_FAST:
      scene->phongfunc = shade_blinn_fast;
      break;
    case RT_SHADER_BLINN:
      scene->phongfunc = shade_blinn;
      break;
    default: 
    case RT_SHADER_PHONG:
      scene->phongfunc = shade_phong;
      break;
  }
}

SceneHandle rt_newscene(void) {
  scenedef * scene;
  SceneHandle voidscene;
  apicolor bgcolor;

  bgcolor.r = 0.0;
  bgcolor.g = 0.0;
  bgcolor.b = 0.0;

  scene = (scenedef *) malloc(sizeof(scenedef));
  memset(scene, 0, sizeof(scenedef));             /* clear all valuas to 0  */

  voidscene = (SceneHandle) scene;

  rt_outputfile(voidscene, "/tmp/outfile.tga");   /* default output file    */
  rt_outputformat(voidscene, RT_FORMAT_TARGA);    /* default iamge format   */
  rt_resolution(voidscene, 512, 512);             /* 512x512 resolution     */
  rt_verbose(voidscene, 0);                       /* verbose messages off   */
  rt_rawimage(voidscene, NULL);                   /* raw image output off   */
  rt_boundmode(voidscene, RT_BOUNDING_ENABLED);   /* spatial subdivision on */
  rt_boundthresh(voidscene, BOUNDTHRESH);         /* default threshold      */
  rt_camera_setup(voidscene, 1.0, 1.0, 0, 6,
                  rt_vector(0.0, 0.0, 0.0),
                  rt_vector(0.0, 0.0, 1.0),
                  rt_vector(0.0, 1.0, 0.0));
  rt_camera_dof(voidscene, 1.0, 0.0);
  rt_shadermode(voidscene, RT_SHADER_AUTO);
  rt_phong_shader(voidscene, RT_SHADER_BLINN);

  rt_background(voidscene, bgcolor);
  rt_fog_mode(voidscene, RT_FOG_NONE);
  rt_fog_parms(voidscene, bgcolor, 0.0, 1.0, 1.0);
  rt_trans_mode(voidscene, RT_TRANS_ORIG);

  scene->objgroup.boundedobj = NULL;
  scene->objgroup.unboundedobj = NULL;
  scene->objgroup.numobjects = 0;

  scene->texlist = NULL;
  scene->lightlist = NULL;
  scene->cliplist = NULL;
  scene->numlights = 0;
  scene->scenecheck = 1;
  scene->parbuf = NULL;
  scene->threads = NULL;
  scene->threadparms = NULL;
  scene->flags = RT_SHADE_NOFLAGS;
 
  rt_set_numthreads(voidscene, -1);         /* auto determine num threads */ 

  /* number of distributed memory nodes, fills in array of node/cpu info */
  scene->nodes = rt_getcpuinfo(&scene->cpuinfo);
  scene->mynode = rt_mynode();

  return scene;
}



void rt_deletescene(SceneHandle voidscene) {
  scenedef * scene = (scenedef *) voidscene;
  list * cur, * next;

  if (scene != NULL) {
    if (scene->imginternal) {
      free(scene->img);
    }

    /* tear down and deallocate persistent rendering threads */
    destroy_render_threads(scene);

    /* tear down and deallocate persistent scanline receives */
    if (scene->parbuf != NULL)
      rt_delete_scanlinereceives(scene->parbuf);

    free(scene->cpuinfo);
    free_objects(scene->objgroup.boundedobj);
    free_objects(scene->objgroup.unboundedobj);

    /* free all lights */
    cur = scene->lightlist;
    while (cur != NULL) {
      next = cur->next;
      free(cur); /* we don't free list contents, other code did this already */
      cur = next;
    }    

    /* free all textures */
    cur = scene->texlist;
    while (cur != NULL) {
      next = cur->next;
      ((texture *) cur->item)->methods->freetex(cur->item); /* free texture */
      free(cur); /* free list entry */
      cur = next;
    }

    /* free all clipping planes */
    cur = scene->cliplist;
    while (cur != NULL) {
      next = cur->next;
      free(((clip_group *) cur->item)->planes); /* free array of clip planes */
      free(cur->item);                          /* free clip group struct    */
      free(cur);                                /* free list entry           */
      cur = next;
    }    

    FreeTextures();
    free(scene);
  }
}

void apitextotex(apitexture * apitex, texture * tx) {
  standard_texture * tex = (standard_texture *) tx;
  tex->img = NULL;
 
  switch(apitex->texturefunc) {
    case RT_TEXTURE_3D_CHECKER: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(checker_texture);
      break;

    case RT_TEXTURE_GRIT: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(grit_texture);
      break;

    case RT_TEXTURE_MARBLE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(marble_texture);
      break;

    case RT_TEXTURE_WOOD: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(wood_texture);
      break;

    case RT_TEXTURE_GRADIENT: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(gnoise_texture);
      break;
	
    case RT_TEXTURE_CYLINDRICAL_CHECKER: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(cyl_checker_texture);
      break;

    case RT_TEXTURE_CYLINDRICAL_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_cyl_texture);
      tex->img=LoadMIPMap((char *) apitex->imap);
      break;

    case RT_TEXTURE_SPHERICAL_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_sphere_texture);
      tex->img=LoadMIPMap((char *) apitex->imap);
      break;

    case RT_TEXTURE_PLANAR_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_plane_texture);
      tex->img=LoadMIPMap((char *) apitex->imap);
      break;

    case RT_TEXTURE_CONSTANT: 
    default: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(constant_texture);
      break;
  }

       tex->ctr = apitex->ctr;
       tex->rot = apitex->rot;
     tex->scale = apitex->scale;
      tex->uaxs = apitex->uaxs;
      tex->vaxs = apitex->vaxs;
   tex->ambient = apitex->ambient;
   tex->diffuse = apitex->diffuse;
  tex->specular = apitex->specular;
   tex->opacity = apitex->opacity;
       tex->col = apitex->col; 

  tex->flags = RT_TEXTURE_SHADOWCAST;
  tex->phong = 0.0;
  tex->phongexp = 0.0;
  tex->phongtype = 0;
}

void * rt_texture(SceneHandle sc, apitexture * apitex) {
  scenedef * scene = (scenedef *) sc;
  texture * tex;
  list * lst;

  tex = new_standard_texture();
  apitextotex(apitex, tex); 

  /* add texture to the scene texture list */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) tex;
  lst->next = scene->texlist;
  scene->texlist = lst;

  return(tex);
}

void rt_tex_phong(void * voidtex, apiflt phong, apiflt phongexp, int type) {
  texture * tex = (texture *) voidtex;
  tex->phong = phong;
  tex->phongexp = phongexp;
  tex->phongtype = type;
}

static void add_bounded_object(scenedef * scene, object * obj) {
  object * objtemp;

  if (obj == NULL)
    return;

  obj->id = new_objectid(scene);
  objtemp = scene->objgroup.boundedobj;
  scene->objgroup.boundedobj = obj;
  obj->nextobj = objtemp;
  obj->clip = scene->curclipgroup;

  /* XXX Clipping ought to be applied to objects before they */
  /*     are even added to the internal data structures, so  */
  /*     they aren't even considered during rendering.       */
  
  scene->scenecheck = 1;
}

static void add_unbounded_object(scenedef * scene, object * obj) {
  object * objtemp;

  if (obj == NULL)
    return;

  obj->id = new_objectid(scene);
  objtemp = scene->objgroup.unboundedobj;
  scene->objgroup.unboundedobj = obj;
  obj->nextobj = objtemp;
  obj->clip = scene->curclipgroup;
  scene->scenecheck = 1;
}

void * rt_light(SceneHandle voidscene, void * tex, apivector ctr, apiflt rad) {
  point_light * li;
  scenedef * scene = (scenedef *) voidscene;
  list * lst;

  li=newpointlight(tex, ctr, rad);

  /* add light to the scene lightlist */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) li;
  lst->next = scene->lightlist;
  scene->lightlist = lst;
  scene->numlights++;

  /* add light as an object as well... */
  add_bounded_object((scenedef *) scene, (object *)li);

  return li;
}

void * rt_directional_light(SceneHandle voidscene, void * tex, apivector dir) {
  directional_light * li;
  scenedef * scene = (scenedef *) voidscene;
  list * lst;

  VNorm(&dir);
  li=newdirectionallight(tex, dir);

  /* add light to the scene lightlist */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) li;
  lst->next = scene->lightlist;
  scene->lightlist = lst;
  scene->numlights++;

  return li;
}

void * rt_spotlight(SceneHandle voidscene, void * tex, apivector ctr, apiflt rad, apivector dir,
                  apiflt start, apiflt end) {
  flt fallstart, fallend;
  point_light * li;
  scenedef * scene = (scenedef *) voidscene;
  list * lst;

  fallstart = start * 3.1415926 / 180.0;
  fallend   = end   * 3.1415926 / 180.0;
  VNorm(&dir);
  li = newspotlight(tex, ctr, rad, dir, fallstart, fallend);

  /* add light to the scene lightlist */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) li;
  lst->next = scene->lightlist;
  scene->lightlist = lst;
  scene->numlights++;
 
  /* add light as an object as well... */
  add_bounded_object(scene, (object *) li);

  return li;
}

void rt_light_attenuation(void * vli, apiflt Kc, apiflt Kl, apiflt Kq) {
  light_set_attenuation((point_light *) vli, Kc, Kl, Kq);
}

void rt_scalarvol(SceneHandle scene, void * tex, apivector min, apivector max,
	int xs, int ys, int zs, const char * fname, scalarvol * invol) {
  add_bounded_object((scenedef *) scene, (object *) newscalarvol(tex, min, max, xs, ys, zs, fname, invol));
}

void rt_extvol(SceneHandle scene, void * tex, apivector min, apivector max, int samples, flt (* evaluator)(flt, flt, flt)) {
  add_bounded_object((scenedef *) scene, (object *) newextvol(tex, min, max, samples, evaluator));
}

void rt_box(SceneHandle scene, void * tex, apivector min, apivector max) {
  add_bounded_object((scenedef *) scene, (object *) newbox(tex, min, max));
} 

void rt_cylinder(SceneHandle scene, void * tex, apivector ctr, apivector axis, apiflt rad) {
  add_unbounded_object((scenedef *) scene, newcylinder(tex, ctr, axis, rad));
}

void rt_fcylinder(SceneHandle scene, void * tex, apivector ctr, apivector axis, apiflt rad) {
  add_bounded_object((scenedef *) scene, newfcylinder(tex, ctr, axis, rad));
}

void rt_plane(SceneHandle scene, void * tex, apivector ctr, apivector norm) {
  add_unbounded_object((scenedef *) scene, newplane(tex, ctr, norm));
} 

void rt_ring(SceneHandle scene, void * tex, apivector ctr, apivector norm, apiflt a, apiflt b) {
  add_bounded_object((scenedef *) scene, newring(tex, ctr, norm, a, b));
} 

void rt_sphere(SceneHandle scene, void * tex, apivector ctr, apiflt rad) {
  add_bounded_object((scenedef *) scene, newsphere(tex, ctr, rad));
}

void rt_tri(SceneHandle scene, void * tex, apivector v0, apivector v1, apivector v2) {
  add_bounded_object((scenedef *) scene, newtri(tex, v0, v1, v2));
} 

void rt_stri(SceneHandle scene, void * tex, apivector v0, apivector v1, apivector v2, 
		apivector n0, apivector n1, apivector n2) {
  add_bounded_object((scenedef *) scene, newstri(tex, v0, v1, v2, n0, n1, n2));
} 

void rt_vcstri(SceneHandle scene, void * tex, apivector v0, apivector v1, apivector v2, 
		apivector n0, apivector n1, apivector n2,
		apicolor c0, apicolor c1, apicolor c2) {
  add_bounded_object((scenedef *) scene, newvcstri(tex, v0, v1, v2, n0, n1, n2, c0, c1, c2));
} 

void rt_tristripscnv3fv(SceneHandle voidscene, void * tex,
                        int numverts, const float * cnv, 
                        int numstrips, int *vertsperstrip, int *facets) {
  int strip, t, v;
  int stripaddr[2][3] = { {0, 1, 2}, {1, 0, 2} };
  scenedef * scene = (scenedef *) voidscene;

  /* render triangle strips one triangle at a time 
   * triangle winding order is:
   *   v0, v1, v2, then v2, v1, v3, then v2, v3, v4, etc.
   */
  /* loop over all of the triangle strips */
  for (strip=0, v=0; strip < numstrips; strip++) {
    /* loop over all triangles in this triangle strip */
    for (t=0; t < (vertsperstrip[strip] - 2); t++) {
      apivector v0, v1, v2;
      apivector n0, n1, n2;
      apicolor  c0, c1, c2;
      int a0, a1, a2;
      list * lst;

      standard_texture * newtex=malloc(sizeof(texture));
      memcpy(newtex, tex, sizeof(texture));
  
      /* add texture to the scene texture list */
      lst = (list *) malloc(sizeof(list));
      lst->item = (void *) tex;
      lst->next = scene->texlist;
      scene->texlist = lst;

      /* render one triangle, using lookup table to fix winding order */
      a0 = facets[v + (stripaddr[t & 0x01][0])] * 10;
      a1 = facets[v + (stripaddr[t & 0x01][1])] * 10;
      a2 = facets[v + (stripaddr[t & 0x01][2])] * 10;

      c0.r = cnv[a0 + 0];
      c0.g = cnv[a0 + 1];
      c0.b = cnv[a0 + 2];
      n0.x = cnv[a0 + 4];
      n0.y = cnv[a0 + 5];
      n0.z = cnv[a0 + 6];
      v0.x = cnv[a0 + 7];
      v0.y = cnv[a0 + 8];
      v0.z = cnv[a0 + 9];

      c1.r = cnv[a1 + 0];
      c1.g = cnv[a1 + 1];
      c1.b = cnv[a1 + 2];
      n1.x = cnv[a1 + 4];
      n1.y = cnv[a1 + 5];
      n1.z = cnv[a1 + 6];
      v1.x = cnv[a1 + 7];
      v1.y = cnv[a1 + 8];
      v1.z = cnv[a1 + 9];

      c2.r = cnv[a2 + 0];
      c2.g = cnv[a2 + 1];
      c2.b = cnv[a2 + 2];
      n2.x = cnv[a2 + 4];
      n2.y = cnv[a2 + 5];
      n2.z = cnv[a2 + 6];
      v2.x = cnv[a2 + 7];
      v2.y = cnv[a2 + 8];
      v2.z = cnv[a2 + 9];

      add_bounded_object((scenedef *) scene, newvcstri(newtex, v0, v1, v2, n0, n1, n2, c0, c1, c2)); 
      v++; /* move on to next vertex */
    }
    v+=2; /* last two vertices are already used by last triangle */
  }
}


void rt_quadsphere(SceneHandle scene, void * tex, apivector ctr, apiflt rad) {
  quadric * q;
  flt factor;
  q=(quadric *) newquadric();
  factor= 1.0 / (rad*rad);
  q->tex=tex;
  q->ctr=ctr;
 
  q->mat.a=factor;
  q->mat.b=0.0;
  q->mat.c=0.0;
  q->mat.d=0.0;
  q->mat.e=factor;
  q->mat.f=0.0;
  q->mat.g=0.0;
  q->mat.h=factor;
  q->mat.i=0.0;
  q->mat.j=-1.0;
 
  add_unbounded_object((scenedef *) scene, (object *)q);
}


void rt_clip_fv(SceneHandle voidscene, int numplanes, float *planes) {
  list * lst;
  clip_group * clip; 
  int i;
  scenedef * scene = (scenedef *) voidscene;

  /* copy clipping info into structure */
  clip = (clip_group *) malloc(sizeof(clip_group));
  clip->numplanes = numplanes;
  clip->planes = (flt *) malloc(numplanes * sizeof(flt) * 4);
  for (i=0; i<(numplanes*4); i++) {
    clip->planes[i] = planes[i];
  }  

  /* add clipping info to the scene clip list */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) clip;
  lst->next = scene->cliplist;
  scene->cliplist = lst;

  /* all objects added from this point on are added with this clip group */
  scene->curclipgroup = clip;
}


void rt_clip_dv(SceneHandle voidscene, int numplanes, double *planes) {
  list * lst;
  clip_group * clip; 
  int i;
  scenedef * scene = (scenedef *) voidscene;

  /* copy clipping info into structure */
  clip = (clip_group *) malloc(sizeof(clip_group));
  clip->numplanes = numplanes;
  clip->planes = (flt *) malloc(numplanes * sizeof(flt) * 4);
  for (i=0; i<(numplanes*4); i++) {
    clip->planes[i] = planes[i];
  }  

  /* add clipping info to the scene clip list */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) clip;
  lst->next = scene->cliplist;
  scene->cliplist = lst;

  /* all objects added from this point on are added with this clip group */
  scene->curclipgroup = clip;
}


void rt_clip_off(SceneHandle voidscene) {
  scenedef * scene = (scenedef *) voidscene;

  /* all objects added from this point on are added without clipping */
  scene->curclipgroup = NULL;
}


