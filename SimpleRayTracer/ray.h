#pragma once

#include "vector.h"

typedef struct RAY {
	point3 orig;
	vec3 dir;
}ray;

point3 ray_at(ray r, double t);

ray gen_ray(point3 origin, vec3 direction);