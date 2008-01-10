/* 
 * plane.h - This file contains the defines for planes etc.
 *
 *  $Id: plane.h,v 1.12 2002/08/04 21:43:34 johns Exp $
 */

 
object * newplane(void * tex, vector ctr, vector norm);

#ifdef PLANE_PRIVATE
typedef struct {
  RT_OBJECT_HEAD
  flt d;
  vector norm;
} plane; 

static void plane_intersect(const plane *, ray *);
static int plane_bbox(void * obj, vector * min, vector * max);
static void plane_normal(const plane *, const vector *, const ray * incident, vector *);
#endif

