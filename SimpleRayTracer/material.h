#pragma once

#include "vector.h"
#include "ray.h"
#include "objects.h"
#include "texture.h"

typedef struct MATERIAL {
	int(*scatter)(ray r_in, struct HIT_RECORD *rec, color3 *attenuation, ray *scattered);
	color3(*emitted)(texture *tex, double u, double v, point3 p);
	texture tex;
	double fuzziness;
	double ri;
}material;

vec3 random_in_unit_sphere();
vec3 random_unit_vector();

/*
 *		Lambertian material		--------------------------------------------------
 */

material _create_lambertian_material(texture tex);

/*
 *		Metal material		------------------------------------------------------
 */

material _create_metal_material(texture tex, double fuzziness);

/*
 *		Dielectric material		--------------------------------------------------
 */

material _create_dielectric_material(texture tex, double ri);

/*
 *		Diffuse light material   -------------------------------------------------
 */

material _create_diffuse_light_material(texture tex);