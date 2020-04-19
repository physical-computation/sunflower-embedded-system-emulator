/* 
 * box.c - This file contains the functions for dealing with boxes.
 *
 *  $Id: box.c,v 1.19 2004/05/28 03:47:48 johns Exp $
 */
 
#include "machine.h"
#include "types.h"
#include "macros.h"
#include "box.h"
#include "vector.h"
#include "intersect.h"
#include "util.h"

int box_bbox(void * obj, vector * min, vector * max) {
  box * b = (box *) obj;

  *min = b->min;
  *max = b->max;

  return 1;
}

static object_methods box_methods = {
  (void (*)(const void *, void *))(box_intersect),
  (void (*)(const void *, const void *, const void *, void *))(box_normal),
  box_bbox, 
  free 
};

box * newbox(void * tex, vector min, vector max) {
  box * b;
  
  b=(box *) malloc(sizeof(box));
  memset(b, 0, sizeof(box));
  b->methods = &box_methods;
  b->tex = tex;
  b->min = min; 
  b->max = max;

  return b;
}

void box_intersect(const box * bx, ray * ry) {
  flt a, tx1, tx2, ty1, ty2, tz1, tz2;
  flt tnear, tfar;

  tnear= -FHUGE;
  tfar= FHUGE;

  if (ry->d.x == 0.0) {
    if ((ry->o.x < bx->min.x) || (ry->o.x > bx->max.x)) return;
  }
  else {
    tx1 = (bx->min.x - ry->o.x) / ry->d.x;
    tx2 = (bx->max.x - ry->o.x) / ry->d.x;
    if (tx1 > tx2) { a=tx1; tx1=tx2; tx2=a; } 
    if (tx1 > tnear) tnear=tx1;   
    if (tx2 < tfar)   tfar=tx2;   
  } 
  if (tnear > tfar) return; 
  if (tfar < 0.0) return;
  
  if (ry->d.y == 0.0) { 
    if ((ry->o.y < bx->min.y) || (ry->o.y > bx->max.y)) return;
  }
  else {
    ty1 = (bx->min.y - ry->o.y) / ry->d.y;
    ty2 = (bx->max.y - ry->o.y) / ry->d.y;
    if (ty1 > ty2) { a=ty1; ty1=ty2; ty2=a; } 
    if (ty1 > tnear) tnear=ty1;   
    if (ty2 < tfar)   tfar=ty2;   
  }
  if (tnear > tfar) return; 
  if (tfar < 0.0) return;
 
  if (ry->d.z == 0.0) { 
    if ((ry->o.z < bx->min.z) || (ry->o.z > bx->max.z)) return;
  }
  else {
    tz1 = (bx->min.z - ry->o.z) / ry->d.z;
    tz2 = (bx->max.z - ry->o.z) / ry->d.z;
    if (tz1 > tz2) { a=tz1; tz1=tz2; tz2=a; } 
    if (tz1 > tnear) tnear=tz1;   
    if (tz2 < tfar)   tfar=tz2;   
  }
  if (tnear > tfar) return; 
  if (tfar < 0.0) return;

  ry->add_intersection(tnear, (const object *) bx, ry);
  ry->add_intersection(tfar, (object *) bx, ry);
}

void box_normal(const box * bx, const vector * pnt, const ray * incident, vector * N) {
  vector a, b, c; 
  flt t;
 
  c.x=(bx->max.x + bx->min.x) / 2.0;
  c.y=(bx->max.y + bx->min.y) / 2.0;
  c.z=(bx->max.z + bx->min.z) / 2.0;
 
  VSub((vector *) pnt, &c, N);
  b=(*N);

  a.x=fabs(N->x);
  a.y=fabs(N->y);
  a.z=fabs(N->z);
 
  N->x=0.0;  N->y=0.0;  N->z=0.0;

  t=MYMAX(a.x, MYMAX(a.y, a.z));  

  if (t==a.x) N->x=b.x;  

  if (t==a.y) N->y=b.y; 

  if (t==a.z) N->z=b.z;

  VNorm(N);
}

