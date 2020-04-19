/* 
 * plane.c - This file contains the functions for dealing with planes.
 *
 *  $Id: plane.c,v 1.22 2002/07/09 16:14:31 johns Exp $
 */
 
#include "machine.h"
#include "types.h"
#include "macros.h"
#include "vector.h"
#include "intersect.h"
#include "util.h"

#define PLANE_PRIVATE
#include "plane.h"

static object_methods plane_methods = {
  (void (*)(const void *, void *))(plane_intersect),
  (void (*)(const void *, const void *, const void *, void *))(plane_normal),
  plane_bbox, 
  free 
};

object * newplane(void * tex, vector ctr, vector norm) {
  plane * p;
  
  p=(plane *) malloc(sizeof(plane));
  memset(p, 0, sizeof(plane));
  p->methods = &plane_methods;

  p->tex = tex;
  p->norm = norm;
  VNorm(&p->norm);
  p->d = -VDot(&ctr, &p->norm);

  return (object *) p;
}

static int plane_bbox(void * obj, vector * min, vector * max) {
  return 0;
}

static void plane_intersect(const plane * pln, ray * ry) {
  flt t,td;
  
  /* may wish to reorder these computations... */
 
  t = -(pln->d + (pln->norm.x * ry->o.x + 
                  pln->norm.y * ry->o.y + 
                  pln->norm.z * ry->o.z));

  td = pln->norm.x * ry->d.x + pln->norm.y * ry->d.y + pln->norm.z * ry->d.z;

  if (td != 0.0) {
    t /= td;
    if (t > 0.0)
      ry->add_intersection(t,(object *) pln, ry);
  }
}

static void plane_normal(const plane * pln, const vector * pnt, const ray * incident, vector * N) {
  *N=pln->norm;
}

