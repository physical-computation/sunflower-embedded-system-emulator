/* 
 * intersect.h - This file contains the declarations and defines for the
 *               functions that manage intersection, bounding and CSG..
 *
 *  $Id: intersect.h,v 1.17 2004/05/31 03:56:23 johns Exp $
 */

unsigned int new_objectid(scenedef *);
void free_objects(object *);
void intersect_objects(ray *);

void add_clipped_intersection(flt, const object *, ray *);
void add_regular_intersection(flt, const object *, ray *);
int closest_intersection(flt *, object const **, ray *);

void add_shadow_intersection(flt, const object *, ray *);
void add_clipped_shadow_intersection(flt, const object *, ray *);
int shadow_intersection(ray *);

#define reset_intersection(ry) \
	(ry)->intstruct.num = 0

