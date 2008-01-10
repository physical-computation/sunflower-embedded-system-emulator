/* 
 * box.h - This file contains the defines for boxes etc.
 *
 *  $Id: box.h,v 1.8 2002/08/04 21:43:34 johns Exp $
 */

 
typedef struct {
  RT_OBJECT_HEAD
  vector min;
  vector max;
} box; 


box * newbox(void * tex, vector min, vector max);
void box_intersect(const box *, ray *);
void box_normal(const box *, const vector *, const ray *, vector *);
