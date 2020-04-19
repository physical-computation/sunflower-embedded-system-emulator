/*
 * coordsys.c -  Routines to translate from one coordinate system to another.
 *
 *  $Id: coordsys.c,v 1.2 1995/03/07 03:53:38 johns Exp $
 */

#include "machine.h"
#include "types.h"
#include "coordsys.h"

void xytopolar(flt x, flt y, flt rad, flt * u, flt * v) {
  flt r1;
  r1=x*x + y*y;  
  *v=sqrt(r1 / (rad*rad));
  if (y<0.0) 
    *u=1.0 - acos(x/sqrt(r1))/TWOPI;
  else 
    *u= acos(x/sqrt(r1))/TWOPI; 
}

void xyztocyl(vector pnt, flt height, flt * u, flt * v) {
  flt r1;

  r1=pnt.x*pnt.x + pnt.y*pnt.y;

  *v=pnt.z / height;
  if (pnt.y<0.0) 
    *u=1.0 - acos(pnt.x/sqrt(r1))/TWOPI;
  else 
    *u=acos(pnt.x/sqrt(r1))/TWOPI;
}

void xyztospr(vector pnt, flt * u, flt * v) {
  flt r1, phi, theta;
 
  r1=sqrt(pnt.x*pnt.x + pnt.y*pnt.y + pnt.z*pnt.z);

  phi=acos(-pnt.y/r1);   
  *v=phi/3.1415926;

  theta=acos((pnt.x/r1)/sin(phi))/TWOPI;

  if (pnt.z > 0.0) 
    *u = theta;
  else 
    *u = 1 - theta; 
}


