/*
 * camera.h - This file contains the defines for camera routines etc.
 *
 *  $Id: camera.h,v 1.18 2003/01/28 05:58:07 johns Exp $
 */

void camera_init(scenedef *);
void camray_init(scenedef *, ray *, unsigned long, unsigned long *);

void camerasetup(scenedef *, flt, vector, vector, vector);
void cameraprojection(camdef *, int);
void cameradof(camdef *, flt, flt);
void camerafrustum(camdef *, flt, flt, flt, flt);

void cameraposition(camdef * camera, vector center, vector viewvec, 
                    vector upvec);
void getcameraposition(camdef * camera, vector * center, vector * viewvec, 
                    vector * upvec, vector *rightvec);

color cam_aa_perspective_ray(ray *, flt, flt);
color cam_perspective_ray(ray *, flt, flt);
color cam_aa_dof_ray(ray *, flt, flt);
color cam_dof_ray(ray *, flt, flt);
color cam_aa_orthographic_ray(ray *, flt, flt);
color cam_orthographic_ray(ray *, flt, flt);
color cam_fisheye_ray(ray *, flt, flt);
color cam_aa_fisheye_ray(ray *, flt, flt);


