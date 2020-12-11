/*
 * vol.h - Volume rendering definitions etc.
 *
 *
 *  $Id: extvol.h,v 1.10 2002/08/04 21:43:34 johns Exp $
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
} extvol;

extvol * newextvol(void * voidtex, vector min, vector max, 
                   int samples, flt (* evaluator)(flt, flt, flt));
color ext_volume_texture(const vector *, const texture *, ray *);


