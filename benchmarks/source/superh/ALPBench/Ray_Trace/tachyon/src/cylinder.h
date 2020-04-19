/* 
 * cylinder.h - This file contains the defines for cylinders etc.
 *
 *  $Id: cylinder.h,v 1.10 2002/08/04 21:43:34 johns Exp $
 */

object * newcylinder(void *, vector, vector, flt);
object * newfcylinder(void *, vector, vector, flt);

#ifdef CYLINDER_PRIVATE

typedef struct {
  RT_OBJECT_HEAD
  vector ctr;
  vector axis;
  flt rad;
} cylinder;

static void cylinder_intersect(const cylinder *, ray *);
static void fcylinder_intersect(const cylinder *, ray *);

static int cylinder_bbox(void * obj, vector * min, vector * max);
static int fcylinder_bbox(void * obj, vector * min, vector * max);

static void cylinder_normal(const cylinder *, const vector *, const ray *, vector *);
#endif

