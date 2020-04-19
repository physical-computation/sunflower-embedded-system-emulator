/*
 * light.c - This file contains declarations and defines for light sources.
 *
 *  $Id: light.c,v 1.22 2004/02/03 03:38:22 johns Exp $
 */

#include "machine.h"
#include "types.h"
#include "macros.h"
#include "vector.h"
#include "intersect.h"
#include "util.h"

#define LIGHT_PRIVATE
#include "light.h"

static object_methods light_methods = {
  (void (*)(const void *, void *))(light_intersect),
  (void (*)(const void *, const void *, const void *, void *))(light_normal),
  light_bbox, 
  free 
};

directional_light * newdirectionallight(void * tex, vector dir) {
  directional_light * l;
  
  l=(directional_light *) malloc(sizeof(directional_light));
  memset(l, 0, sizeof(directional_light));
  l->methods = &light_methods;
  l->shade_diffuse = directional_light_shade_diffuse;

  l->tex=tex;
  l->tex->flags = RT_TEXTURE_SHADOWCAST | RT_TEXTURE_ISLIGHT;
  l->tex->diffuse=0.0;
  l->tex->specular=0.0;
  l->tex->opacity=1.0;

  l->dir.x=-dir.x; /* store negated light direction for high shading speed */
  l->dir.y=-dir.y;
  l->dir.z=-dir.z;

  return l;
}

point_light * newpointlight(void * tex, vector ctr, flt rad) {
  point_light * l;
  
  l=(point_light *) malloc(sizeof(point_light));
  memset(l, 0, sizeof(point_light));
  l->methods = &light_methods;
  l->shade_diffuse = simple_point_light_shade_diffuse;

  l->tex=tex;
  l->tex->flags = RT_TEXTURE_SHADOWCAST | RT_TEXTURE_ISLIGHT;
  l->tex->diffuse=0.0;
  l->tex->specular=0.0;
  l->tex->opacity=1.0;

  l->ctr=ctr;
  l->rad=rad;

  l->attenuationfunc = light_no_attenuation;
  l->Kc = 1.0;
  l->Kl = 0.0;
  l->Kq = 0.0;

  l->spotfunc = light_no_falloff;
  l->spotdir.x = 0.0;
  l->spotdir.y = 0.0;
  l->spotdir.z = 1.0;
  l->fallstart = 0.3;
  l->fallend   = 0.7;

  return l;
}

point_light * newspotlight(void * tex, vector ctr, flt rad, vector dir,
                           flt fallstart, flt fallend) {
  point_light * l;

  l=(point_light *) malloc(sizeof(point_light));
  memset(l, 0, sizeof(point_light));
  l->methods = &light_methods;
  l->shade_diffuse = point_light_shade_diffuse;

  l->tex=tex;
  l->tex->flags = RT_TEXTURE_SHADOWCAST | RT_TEXTURE_ISLIGHT;
  l->tex->diffuse=0.0;
  l->tex->specular=0.0;
  l->tex->opacity=1.0;

  l->ctr=ctr;
  l->rad=rad;

  l->attenuationfunc = light_no_attenuation;
  l->Kc = 1.0;
  l->Kl = 0.0;
  l->Kq = 0.0;

  l->spotfunc  = light_spotlight_falloff;
  l->spotdir   = dir; 
  l->fallstart = fallstart;
  l->fallend   = fallend;

  return l;
}

/*
 * Set the attenuation equation for a positional light
 */
void light_set_attenuation(point_light * li, flt Kc, flt Kl, flt Kq) {
  li->Kc = Kc;  /* constant attenuation coefficient  */
  li->Kl = Kl;  /* linear attenuation coefficient    */
  li->Kq = Kq;  /* quadratic attenuation coefficient */
 
  li->attenuationfunc = light_complex_attenuation;
  li->shade_diffuse = point_light_shade_diffuse;   /* use the complex shader */
}


/*
 * Private / Internal lighting routines.
 */



/*
 * Direction Lighting - calculate diffuse contribution of directional light
 */
static flt directional_light_shade_diffuse(directional_light * li, shadedata *shadevars) {
  flt inten;
  shadevars->L = li->dir;                 /* set light direction          */
  shadevars->Llen = FHUGE;                /* infinite distance tolight    */
  VDOT(inten, (shadevars->N), li->dir)    /* light intensity              */
  return inten;
}


/*
 * Point Light - calculate diffuse contribution of positional light
 *               This version handles the simplest case only.
 */
static flt simple_point_light_shade_diffuse(point_light * li, shadedata *shadevars) {
  flt inten, len;

  VSUB(li->ctr, (shadevars->hit), (shadevars->L))  /* find the light vector */

  /* calculate the distance to the light from the hit point */
  len = sqrt(shadevars->L.x*shadevars->L.x + shadevars->L.y*shadevars->L.y + shadevars->L.z*shadevars->L.z) + EPSILON;

  shadevars->L.x /= len;                  /* normalize the light direction */
  shadevars->L.y /= len;
  shadevars->L.z /= len;
  shadevars->Llen = len;                  /* set distance to the light */

  VDOT(inten, shadevars->N, shadevars->L) /* light intensity              */
  return inten;
}


/*
 * Point Light - calculate diffuse contribution of positional light
 *               This version handles spotlights and attenuation as well.
 */
static flt point_light_shade_diffuse(point_light * li, shadedata *shadevars) {
  flt inten, len;

  VSUB(li->ctr, (shadevars->hit), (shadevars->L))  /* find the light vector */

  /* calculate the distance to the light from the hit point */
  len = sqrt(shadevars->L.x*shadevars->L.x + shadevars->L.y*shadevars->L.y + shadevars->L.z*shadevars->L.z) + EPSILON;

  shadevars->L.x /= len;                  /* normalize the light direction */
  shadevars->L.y /= len;
  shadevars->L.z /= len;
  shadevars->Llen = len;                  /* set distance to the light */

  VDOT(inten, shadevars->N, shadevars->L) /* light intensity              */
  inten *= li->attenuationfunc(li, shadevars->Llen);
  inten *= li->spotfunc(li, &shadevars->L);
  return inten;
}


/*
 * Attenuation functions. 
 */
static flt light_no_attenuation(void * vli, flt Llen) {
  return 1.0;
}

static flt light_complex_attenuation(void * vli, flt Llen) {
  point_light * li = (point_light *) vli;
  return 1.0 / (li->Kc + ((li->Kl + li->Kq*Llen) * Llen));
}


/*
 * Falloff functions for spotlights.
 */
static flt light_no_falloff(void * vli, vector * L) {
  return 1.0;
}


static flt light_spotlight_falloff(void * vli, vector * L) {
  point_light * li = (point_light *) vli;
  flt ang, cang;

  VDOT(cang, li->spotdir, (*L)) 

  ang = acos(-cang); 
  
  if (ang > li->fallstart) {
    if (ang > li->fallend) 
      return 0.0;
    else 
      return 1.0 - ((ang - li->fallstart) / (li->fallend - li->fallstart));
  }

  return 1.0;
}


static int light_bbox(void * obj, vector * min, vector * max) {
  return 0; /* lights are unbounded currently */
}


static void light_intersect(const point_light * l, ray * ry) {
  flt b, disc, t1, t2, temp;
  vector V;

  /* Lights do not cast shadows.. */
  if (ry->flags & RT_RAY_SHADOW)
    return;

  VSUB(l->ctr, ry->o, V);
  VDOT(b, V, ry->d); 
  VDOT(temp, V, V);  

  disc=b*b + l->rad*l->rad - temp;

  if (disc<=0.0) return;
  disc=sqrt(disc);

  t2=b+disc;
  if (t2 <= SPEPSILON) 
    return;
  ry->add_intersection(t2, (object *) l, ry);  

  t1=b-disc;
  if (t1 > SPEPSILON) 
    ry->add_intersection(t1, (object *) l, ry);  
}

static void light_normal(const point_light * l, const vector * pnt, const ray * incident, vector * N) {
  flt len;

  N->x = pnt->x - l->ctr.x;
  N->y = pnt->y - l->ctr.y;
  N->z = pnt->z - l->ctr.z;

  len = sqrt(N->x*N->x + N->y*N->y + N->z*N->z);
  N->x /= len;
  N->y /= len;
  N->z /= len;
}





