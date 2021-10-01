#include "vector.h"
#include "vector_op.h"
#include "ray.h"

point3 ray_at(ray r, double t)
{
	return addv(r.orig, mulv(t, r.dir));
}

ray gen_ray(point3 origin, vec3 direction)
{
	ray r;
	r.orig = origin;
	r.dir = direction;

	return r;
}