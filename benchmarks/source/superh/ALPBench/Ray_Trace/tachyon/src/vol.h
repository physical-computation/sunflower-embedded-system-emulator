/*
 * vol.h - Volume rendering definitions etc.
 *
 *
 *  $Id: vol.h,v 1.6 2002/07/09 18:17:26 johns Exp $
 */


void * newscalarvol(void * intex, vector min, vector max, 
                    int xs, int ys, int zs, 
                    const char * fname, scalarvol * invol);

void  LoadVol(scalarvol *);
color scalar_volume_texture(const vector *, const texture *, ray *);

