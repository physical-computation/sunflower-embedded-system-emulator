/*
 * spaceball.c - code to perform camera fly-throughs using a spaceball
 * 
 *  $Id: spaceball.c,v 1.2 2001/08/19 06:57:06 johns Exp $
 */

#ifdef USESPACEBALL

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "trackball.h"   /* quaternion code */
#include "tachyon.h"     /* main ray tracer api headers */
#include "sball.h"       /* spaceball code */
#include "spaceball.h"   /* protos for this file */

void * tachyon_init_spaceball(SceneHandle scene, char * port) {
  sbHandle * bh;

  bh = (sbHandle *) malloc(sizeof(sbHandle));

  if (bh != NULL) {
    memset(bh, 0, sizeof(sbHandle));

    bh->sball = sball_open(port);
    if (bh->sball == NULL) {
      free(bh);
      return NULL;
    } 
  }

  rt_get_camera_position(scene, &bh->orig_camcent, &bh->orig_camviewvec, &bh->orig_camupvec, &bh->orig_camrightvec);

  bh->camcent = bh->orig_camcent;
  bh->camviewvec = bh->orig_camviewvec;
  bh->camupvec = bh->orig_camupvec;

  trackball(bh->curquat, 0.0, 0.0, 0.0, 0.0);

  return bh;
}


int tachyon_spaceball_update(sbHandle * bh, SceneHandle scene) {
  int tx, ty, tz, rx, ry, rz, buttons;
  float qq[4];
  float xx[3]={1.0, 0.0, 0.0};
  float yy[3]={0.0, 1.0, 0.0};
  float zz[3]={0.0, 0.0, 1.0};

  float m[4][4];
  float t[3];

  static float transdivisor = 5000.0;
  static float angdivisor = 20000.0;

  if (bh->sball == NULL)
    return -1;
 
  if (sball_getstatus(bh->sball, &tx, &ty, &tz, &rx, &ry, &rz, &buttons)) {
    /* negate rotations given by spaceball */
    rx = -rx;
    ry = -ry;
    rz = -rz;

    if (buttons) {
      if (buttons & SBALL_BUTTON_PICK) {
        bh->curtrans[0] = 0.0;
        bh->curtrans[1] = 0.0;
        bh->curtrans[2] = 0.0;
        trackball(bh->curquat, 0.0, 0.0, 0.0, 0.0);
        transdivisor = 5000.0;
        angdivisor = 20000.0; 
  
        bh->camviewvec = bh->orig_camviewvec;
        bh->camupvec = bh->orig_camupvec;
        bh->camcent = bh->orig_camcent;
      }

      if (buttons & SBALL_BUTTON_1) {
        transdivisor /= 1.2;
        angdivisor /= 1.2;
      }
      else if (buttons & SBALL_BUTTON_2) {
        transdivisor *= 1.2; 
        angdivisor *= 1.2;
      }

      if (buttons & SBALL_BUTTON_3)
        transdivisor /= 1.2;
      else if (buttons & SBALL_BUTTON_4)
        transdivisor *= 1.2; 

      if (buttons & SBALL_BUTTON_5) 
        angdivisor *= 1.2;
      else if (buttons & SBALL_BUTTON_6)
        angdivisor /= 1.2;


      if (buttons & SBALL_BUTTON_7) {
        return 1;  /* quit the fly through */
      }
    } /* end of button handling */
      
    t[0] = tx / transdivisor; 
    t[1] = ty / transdivisor;
    t[2] = tz / transdivisor;

    /* 
     * convert rotations and translations from the
     * spaceball's coordinate frame into the camera's frame.
     */
    bh->newtrans[0] = 
      t[0] * bh->orig_camrightvec.x +
      t[1] * bh->orig_camupvec.x +
      t[2] * bh->orig_camviewvec.x;

    bh->newtrans[1] = 
      t[0] * bh->orig_camrightvec.y +
      t[1] * bh->orig_camupvec.y +
      t[2] * bh->orig_camviewvec.y;

    bh->newtrans[2] = 
      t[0] * bh->orig_camrightvec.z +
      t[1] * bh->orig_camupvec.z +
      t[2] * bh->orig_camviewvec.z;

    /* 
     * rotate around camera's coordinate frame
     */ 
    xx[0] = bh->orig_camrightvec.x;
    xx[1] = bh->orig_camrightvec.y;
    xx[2] = bh->orig_camrightvec.z;

    yy[0] = bh->orig_camupvec.x;
    yy[1] = bh->orig_camupvec.y;
    yy[2] = bh->orig_camupvec.z;

    zz[0] = bh->orig_camviewvec.x;
    zz[1] = bh->orig_camviewvec.y;
    zz[2] = bh->orig_camviewvec.z;
 
    /* do rotations */ 
    axis_to_quat(xx, rx / angdivisor, bh->lastquat);

    axis_to_quat(yy, ry / angdivisor, qq);
    add_quats(qq, bh->lastquat, bh->lastquat);

    axis_to_quat(zz, rz / angdivisor, qq);
    add_quats(qq, bh->lastquat, bh->lastquat);

    add_quats(bh->lastquat, bh->curquat, bh->curquat);
  }
  else {
    usleep(5); /* if no movement then sleep for a tiny bit.. */
  }

  build_rotmatrix(m, bh->curquat);

  /*
   * translate along the new axes
   */
  t[0] = m[0][0] * bh->newtrans[0] + m[0][1] * bh->newtrans[1] + m[0][2] * bh->newtrans[2];
  t[1] = m[1][0] * bh->newtrans[0] + m[1][1] * bh->newtrans[1] + m[1][2] * bh->newtrans[2];
  t[2] = m[2][0] * bh->newtrans[0] + m[2][1] * bh->newtrans[1] + m[2][2] * bh->newtrans[2];

  bh->camcent.x += t[0];
  bh->camcent.y += t[1];
  bh->camcent.z += t[2];


  /*
   * rotate view system with spaceball
   */
  bh->camviewvec.x = m[0][0] * bh->orig_camviewvec.x + m[0][1] * bh->orig_camviewvec.y + m[0][2] * bh->orig_camviewvec.z;
  bh->camviewvec.y = m[1][0] * bh->orig_camviewvec.x + m[1][1] * bh->orig_camviewvec.y + m[1][2] * bh->orig_camviewvec.z;
  bh->camviewvec.z = m[2][0] * bh->orig_camviewvec.x + m[2][1] * bh->orig_camviewvec.y + m[2][2] * bh->orig_camviewvec.z;

  bh->camupvec.x = m[0][0] * bh->orig_camupvec.x + m[0][1] * bh->orig_camupvec.y + m[0][2] * bh->orig_camupvec.z;
  bh->camupvec.y = m[1][0] * bh->orig_camupvec.x + m[1][1] * bh->orig_camupvec.y + m[1][2] * bh->orig_camupvec.z;
  bh->camupvec.z = m[2][0] * bh->orig_camupvec.x + m[2][1] * bh->orig_camupvec.y + m[2][2] * bh->orig_camupvec.z;

  /*
   * update camera parameters before we render again
   */
  rt_camera_position(scene, bh->camcent, bh->camviewvec, bh->camupvec);

  return 0;
}

#endif

