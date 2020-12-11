/* 
 * quadric.h - This file contains the defines for quadrics.
 *
 *  $Id: quadric.h,v 1.11 2002/08/04 21:43:34 johns Exp $
 */

typedef struct {
  flt a; flt b; flt c;
  flt d; flt e; flt f;
  flt g; flt h; flt i; flt j;
} quadmatrix;

 
typedef struct {
  RT_OBJECT_HEAD
  vector ctr;
  quadmatrix mat;
} quadric; 


quadric * newquadric(void);
void quadric_intersect(const quadric *, ray *);
void quadric_normal(const quadric *, const vector *, const ray *, vector *);



