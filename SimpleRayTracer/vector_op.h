#pragma once

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "random.h"

inline vec3 zerosv3()
{
	vec3 v;
	v.e[0] = 0.0;
	v.e[1] = 0.0;
	v.e[2] = 0.0;

	return v;
}
inline vec3 onesv3()
{
	vec3 v;
	v.e[0] = 1.0;
	v.e[1] = 1.0;
	v.e[2] = 1.0;

	return v;
}

inline vec3 initv3(double e1, double e2, double e3)
{
	vec3 v;
	v.e[0] = e1;
	v.e[1] = e2;
	v.e[2] = e3;

	return v;
}

inline vec3 addv(vec3 v1, vec3 v2)
{	
	vec3 v;
	v.e[0] = v1.e[0] + v2.e[0];
	v.e[1] = v1.e[1] + v2.e[1];
	v.e[2] = v1.e[2] + v2.e[2];
	return v;
}

inline vec3 subv(vec3 v1, vec3 v2)
{
	vec3 v;
	v.e[0] = v1.e[0] - v2.e[0];
	v.e[1] = v1.e[1] - v2.e[1];
	v.e[2] = v1.e[2] - v2.e[2];
	return v;
}

inline vec3 subvc(vec3 v, double t)
{
	v.e[0] -= t;
	v.e[1] -= t;
	v.e[2] -= t;
	return v;
}


inline vec3 mulv(double t, vec3 v)
{
	v.e[0] *= t;
	v.e[1] *= t;
	v.e[2] *= t;
	return v;
}

inline vec3 mulvp(vec3 v1, vec3 v2)
{
	vec3 v;
	v.e[0] = v1.e[0] * v2.e[0];
	v.e[1] = v1.e[1] * v2.e[1];
	v.e[2] = v1.e[2] * v2.e[2];;
	return v;
}

inline vec3 divv(double t, vec3 v)
{
	v.e[0] /= t;
	v.e[1] /= t;
	v.e[2] /= t;
	return v;
}

inline double length_squaredv(vec3 v)
{
	return v.e[0] * v.e[0] + v.e[1] * v.e[1] + v.e[2] * v.e[2];
}

inline double lengthv(vec3 v)
{
	return sqrt(length_squaredv(v));
}

inline double dotv(vec3 v1, vec3 v2)
{
	return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline vec3 crossv(vec3 v1, vec3 v2)
{
	vec3 v;
	v.e[0] = v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1];
	v.e[1] = v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2];
	v.e[2] = v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0];
	return v;
}

inline vec3 unitv(vec3 v)
{
	return divv(lengthv(v), v);
}

inline vec3 sqrtv(vec3 v)
{
	return initv3(sqrt(v.e[0]), sqrt(v.e[1]), sqrt(v.e[2]));
}

// Utils
inline void print_vec3(vec3 v)
{
	printf("[ %lf, %lf, %lf ]\n", v.e[0], v.e[1], v.e[2]);
	return;
}

inline vec3 randomv3(double min, double max)
{
	return initv3(random_double_range(min, max), random_double_range(min, max), random_double_range(min, max));
}

