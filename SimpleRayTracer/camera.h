#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "vector.h"
#include "ray.h"
#include "renderer.h"
#include <float.h>

typedef struct CAMERA {
	double vfov_r;
	point3 lookfrom;
	point3 lookat;

	double lens_radius;
	double focus_dist;
	point3 origin;
	vec3 horizontal;
	vec3 vertical;
	point3 lower_left_corner;

	vec3 u;
	vec3 v;
	vec3 w;
	
	double ray_max_length;
	double ray_min_length;
}camera;

typedef struct CAMERA_CONFIG {
	double vfov;
	point3 lookfrom;
	point3 lookat;
	double aperture;
	double focus_dist;
}camera_config;

vec3 random_in_unit_disk();

void camera_init(camera *cam, renderer_config r_conf, camera_config c_conf);
ray camera_get_ray(camera *cam, double s, double t);