/* 
 * shade.h - This file contains declarations and definitions for the shader.
 *
 *  $Id: shade.h,v 1.17 2003/02/16 06:24:21 johns Exp $
 */

#ifndef SHADE_H
#define SHADE_H

color lowest_shader(ray *);
color low_shader(ray *);
color medium_shader(ray *);
color full_shader(ray *);
color shade_reflection(ray *, const shadedata *, flt);
color shade_transmission(ray *, const shadedata *, flt);



flt shade_phong(const ray * incident, const shadedata * shadevars, flt specpower);
flt shade_nullphong(const ray * incident, const shadedata * shadevars, flt specpower);
flt shade_blinn(const ray * incident, const shadedata * shadevars, flt specpower);
flt shade_blinn_fast(const ray * incident, const shadedata * shadevars, flt specpower);


color fog_color_linear(struct fogdata_t *, color col, flt z);
color fog_color_exp(struct fogdata_t *, color col, flt z);
color fog_color_exp2(struct fogdata_t *, color col, flt z);

#endif
