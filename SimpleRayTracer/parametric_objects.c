#include "parametric_objects.h"
#include "vector.h"
#include "vector_op.h"
#include "objects.h"

#define _USE_MATH_DEFINES
#include <math.h>

/*
 *		Sphere object		----------------------------------------------------------------------------
 */

int sphere_intersection_test(ray r, double t_min, double t_max, hit_record *rec, object_data *data);
int sphere_bounding_box(double t0, double t1, aabb *bb, object_data *data);

void calc_sphere_uv(vec3 p, double *u, double *v) 
{
	double theta = acos(-p.e[1]);
	double phi = atan2(-p.e[2], p.e[0]) + M_PI;

	*u = phi / (2 * M_PI);
	*v = theta / M_PI;
}

object _create_sphere_object(point3 center, double r)
{
	object_data data;
	data.type = PARAMETRIC;
	data.center = center;
	data.data[0] = r;	

	return create_object(data, sphere_intersection_test, sphere_bounding_box);
}

int sphere_intersection_test(ray r, double t_min, double t_max, hit_record *rec, object_data *data)
{
	vec3 oc = subv(r.orig, data->center);
	double a = length_squaredv(r.dir);
	double half_b = dotv(oc, r.dir);
	double c = length_squaredv(oc) - data->data[0] * data->data[0];
	double discriminant = half_b * half_b - a * c;

	if (discriminant > 0)
	{
		double root = sqrt(discriminant);
		double temp = (-half_b - root) / a;

		if (temp < t_max && temp > t_min) {
			rec->t = temp;
			rec->p = ray_at(r, rec->t);
			calc_sphere_uv(divv(data->data[0], subv(rec->p, data->center)), &rec->u, &rec->v);
			rec->mat = data->mat;
			set_face_normal(r, divv(data->data[0], subv(rec->p, data->center)), rec);

			return 1;
		}

		temp = (-half_b + root) / a;

		if (temp < t_max && temp > t_min) {
			rec->t = temp;
			rec->p = ray_at(r, rec->t);
			calc_sphere_uv(divv(data->data[0], subv(rec->p, data->center)), &rec->u, &rec->v);
			rec->mat = data->mat;
			set_face_normal(r, divv(data->data[0], subv(rec->p, data->center)), rec);

			return 1;
		}
	}

	return 0;
}

int sphere_bounding_box(double t0, double t1, aabb *bb, object_data *data)
{
	bb->min = subv(data->center, initv3(data->data[0], data->data[0], data->data[0]));
	bb->max = addv(data->center, initv3(data->data[0], data->data[0], data->data[0]));

	return 1;
}