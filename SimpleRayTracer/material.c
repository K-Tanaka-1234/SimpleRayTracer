#define _USE_MATH_DEFINES
#include <math.h>

#include "vector.h"
#include "vector_op.h"
#include "material.h"
#include "random.h"

vec3 random_in_unit_sphere()
{
	vec3 p = randomv3(-1, 1);

	while (length_squaredv(p) >= 1)
	{
		p = randomv3(-1, 1);
	}

	return p;
}

vec3 random_unit_vector()
{
	double a = random_double_range(0, 2 * M_PI);
	double z = random_double_range(-1, 1);
	double r = sqrt(1 - z * z);

	return initv3(r*cos(a), r*sin(a), z);
}

vec3 reflect(vec3 v, vec3 n)
{
	return subv(v, mulv(2, mulv(dotv(v, n), n)));
}

vec3 refract(vec3 uv, vec3 n, double etai_over_etat)
{
	double cos_theta = dotv(mulv(-1, uv), n);
	vec3 r_out_parallel = mulv(etai_over_etat, addv(uv, mulv(cos_theta, n)));
	vec3 r_out_perp = mulv(-sqrt(1.0 - length_squaredv(r_out_parallel)), n);

	return addv(r_out_parallel, r_out_perp);
}

int no_scatter(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered)
{
	return 0;
}

color3 no_emission(texture *tex, double u, double v, point3 p)
{
	return zerosv3();
}

/*
 *		Lambertian material		--------------------------------------------------
 */

int scatter_lambertian(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered);

material _create_lambertian_material(texture tex)
{
	material mat;
	mat.scatter = scatter_lambertian;
	mat.emitted = no_emission;
	mat.tex = tex;

	return mat;
}

int scatter_lambertian(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered)
{
	vec3 scatter_direction = addv(rec->normal, random_unit_vector());

	double s = 1e-8;
	if ((fabs(scatter_direction.e[0]) < s) && (fabs(scatter_direction.e[1]) < s) && (fabs(scatter_direction.e[2]) < s))
		scatter_direction = rec->normal;

	*scattered = gen_ray(rec->p, scatter_direction);
	*attenuation = rec->mat->tex.tex_value(&rec->mat->tex, rec->u, rec->v, rec->p);
	
	return 1;
}

/*
 *		Metal material		------------------------------------------------------
 */

int scatter_metal(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered);

material _create_metal_material(texture tex, double fuzziness)
{
	material mat;
	mat.scatter = scatter_metal;
	mat.emitted = no_emission;
	mat.tex = tex;
	mat.fuzziness = fuzziness;

	return mat;
}

int scatter_metal(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered)
{
	vec3 reflected = reflect(unitv(r_in.dir), rec->normal);
	*scattered = gen_ray(rec->p, addv(reflected, mulv(rec->mat->fuzziness, random_in_unit_sphere())));
	*attenuation = rec->mat->tex.tex_value(&rec->mat->tex, rec->u, rec->v, rec->p);

	return (dotv(scattered->dir, rec->normal) > 0);
}

/*
 *		Dielectric material		--------------------------------------------------
 */

int scatter_dielectric(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered);

double schlick(double cosine, double ref_idx) 
{
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}


material _create_dielectric_material(texture tex, double ri)
{
	material mat;
	mat.scatter = scatter_dielectric;
	mat.emitted = no_emission;
	mat.tex = tex;
	mat.ri = ri;

	return mat;
}

int scatter_dielectric(ray r_in, hit_record *rec, color3 *attenuation, ray *scattered)
{
	*attenuation = rec->mat->tex.tex_value(&rec->mat->tex, rec->u, rec->v, rec->p);

	double etai_over_etat;
	if (rec->front_face)
	{
		etai_over_etat = 1.0 / rec->mat->ri;
	}
	else
	{
		etai_over_etat = rec->mat->ri;
	}

	vec3 unit_direction = unitv(r_in.dir);
	double cos_theta = fmin(dotv(mulv(-1, unit_direction), rec->normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	
	if (etai_over_etat * sin_theta > 1.0) {
		vec3 reflected = reflect(unit_direction, rec->normal);
		*scattered = gen_ray(rec->p, reflected);
		
		return 1;
	}

	double reflect_prob = schlick(cos_theta, etai_over_etat);
	if (random_double() < reflect_prob) 
	{
		vec3 reflected = reflect(unit_direction, rec->normal);
		*scattered = gen_ray(rec->p, reflected);
		
		return 1;
	}

	vec3 refracted = refract(unit_direction, rec->normal, etai_over_etat);
	*scattered = gen_ray(rec->p, refracted);

	return 1;
}

/*
 *		Diffuse light material   -------------------------------------------------
 */

color3 emitted_diffuse_light(texture *tex, double u, double v, point3 p);

material _create_diffuse_light_material(texture tex)
{
	material mat;
	mat.scatter = no_scatter;
	mat.emitted = emitted_diffuse_light;
	mat.tex = tex;

	return mat;
}

color3 emitted_diffuse_light(texture *tex, double u, double v, point3 p)
{
	return tex->tex_value(tex, u, v, p);
}