
/*
 * apitrigeom.h - header for functions to generate triangle tesselated 
 *                geometry for use with OpenGL, XGL, etc.
 *
 */

void rt_tri_fcylinder(SceneHandle, void *, apivector, apivector, apiflt);
void rt_tri_cylinder(SceneHandle, void *, apivector, apivector, apiflt);
void rt_tri_ring(SceneHandle, void *, apivector, apivector, apiflt, apiflt);
void rt_tri_plane(SceneHandle, void *, apivector, apivector);
void rt_tri_box(SceneHandle, void *, apivector, apivector);

