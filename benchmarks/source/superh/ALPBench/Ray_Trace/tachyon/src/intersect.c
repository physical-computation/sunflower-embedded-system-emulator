/* 
 * intersect.c - This file contains code for CSG and intersection routines.
 *
 *  $Id: intersect.c,v 1.38 2004/05/31 03:56:23 johns Exp $
 */

#include "machine.h"
#include "types.h"
#include "intersect.h"
#include "macros.h"

unsigned int new_objectid(scenedef * scene) {
  return scene->objgroup.numobjects++; /* generate unique object ID's */
}

unsigned int max_objectid(scenedef * scene) {
  return scene->objgroup.numobjects;
}

void free_objects(object * start) {
  object * cur;
  object * next;

  cur=start; 
  while (cur != NULL) { 
    next=cur->nextobj;
    cur->methods->freeobj(cur);
    cur=next;
  }
}


void intersect_objects(ray * ry) {
  object * cur;
  object temp;

  reset_intersection(ry); /* eliminate any existing intersections */

  /* do unbounded objects first, to help early-exit bounded object tests */
  temp.nextobj = ry->scene->objgroup.unboundedobj;
  cur = &temp;
  while ((cur=cur->nextobj) != NULL)          
    cur->methods->intersect(cur, ry); 

  /* do bounded objects last, taking advantage of early-exit opportunities */
  temp.nextobj = ry->scene->objgroup.boundedobj;
  cur = &temp;
  while ((cur=cur->nextobj) != NULL)          
    cur->methods->intersect(cur, ry); 
}


/* Only keeps closest intersection, no clipping, no CSG */
void add_regular_intersection(flt t, const object * obj, ray * ry) {
  if (t > EPSILON) {
    /* if we hit something before maxdist update maxdist */
    if (t < ry->maxdist) {
      ry->maxdist = t;
      ry->intstruct.num=1;
      ry->intstruct.closest.obj = obj;
      ry->intstruct.closest.t = t;
    }
  }
}

/* Only keeps closest intersection, also handles clipping, no CSG */
void add_clipped_intersection(flt t, const object * obj, ray * ry) {
  if (t > EPSILON) {
    /* if we hit something before maxdist update maxdist */
    if (t < ry->maxdist) {

      /* handle clipped object tests */
      if (obj->clip != NULL) {
        vector hit;
        int i;

        RAYPNT(hit, (*ry), t);    /* find the hit point for further tests */
        for (i=0; i<obj->clip->numplanes; i++) {
          if ((obj->clip->planes[i * 4    ] * hit.x +
               obj->clip->planes[i * 4 + 1] * hit.y +
               obj->clip->planes[i * 4 + 2] * hit.z) > 
               obj->clip->planes[i * 4 + 3]) {
            return; /* hit point was clipped */
          } 
        } 
      }

      ry->maxdist = t;
      ry->intstruct.num=1;
      ry->intstruct.closest.obj = obj;
      ry->intstruct.closest.t = t;
    }
  }
}

int closest_intersection(flt * t, object const ** obj, ray * ry) {
  if (ry->intstruct.num > 0) {
      *t = ry->intstruct.closest.t;
    *obj = ry->intstruct.closest.obj;
  } 

  return ry->intstruct.num;
}
/* End of CSG-unsafe */


/* Only meant for shadow rays, unsafe for anything else */
void add_shadow_intersection(flt t, const object * obj, ray * ry) {
  if (t > EPSILON) {
    /* if we hit something before maxdist update maxdist */
    if (t < ry->maxdist) {
      /* if this object doesn't cast a shadow, quit out.. */
      if (!(obj->tex->flags & RT_TEXTURE_SHADOWCAST)) {
        return;
      }

      ry->maxdist = t;
      ry->intstruct.num=1;

      /* if we hit *anything* before maxdist, and we're firing a */
      /* shadow ray, then we are finished ray tracing the shadow */
      ry->flags |= RT_RAY_FINISHED;
    }
  }
}

/* Only meant for clipped shadow rays, unsafe for anything else */
void add_clipped_shadow_intersection(flt t, const object * obj, ray * ry) {
  if (t > EPSILON) {
    /* if we hit something before maxdist update maxdist */
    if (t < ry->maxdist) {
      /* if this object doesn't cast a shadow, quit out.. */
      if (!(obj->tex->flags & RT_TEXTURE_SHADOWCAST)) {
        return;
      }

      /* handle clipped object tests */
      if (obj->clip != NULL) {
        vector hit;
        int i;

        RAYPNT(hit, (*ry), t);    /* find the hit point for further tests */
        for (i=0; i<obj->clip->numplanes; i++) {
          if ((obj->clip->planes[i * 4    ] * hit.x +
               obj->clip->planes[i * 4 + 1] * hit.y +
               obj->clip->planes[i * 4 + 2] * hit.z) > 
               obj->clip->planes[i * 4 + 3]) {
            return; /* hit point was clipped */
          } 
        } 
      }

      ry->maxdist = t;
      ry->intstruct.num=1;

      /* if we hit *anything* before maxdist, and we're firing a */
      /* shadow ray, then we are finished ray tracing the shadow */
      ry->flags |= RT_RAY_FINISHED;
    }
  }
}


int shadow_intersection(ray * ry) {
  if (ry->intstruct.num > 0) 
    return 1;

  return 0;
}

