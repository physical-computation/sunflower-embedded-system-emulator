/*
 * parvol.h - Volume rendering definitions etc.
 *
 *
 *  $Id: parvol.h,v 1.4 2002/08/04 21:43:34 johns Exp $
 */

typedef struct {
  RT_OBJECT_HEAD
  vector min;
  vector max;
  flt ambient;
  flt diffuse;
  flt opacity;
  int samples;
  flt (* evaluator)(flt, flt, flt);
} parvol;

parvol * newparvol();
color par_volume_texture(vector *, texture *, ray *);


