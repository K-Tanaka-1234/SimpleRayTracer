#include "vector.h"
#include "vector_op.h"
#include "random.h"
#include "camera.h"

vec3 random_in_unit_disk() 
{
	while (1) 
	{
		vec3 p = initv3(random_double(-1, 1), random_double(-1, 1), 0);
		if (length_squaredv(p) >= 1) continue;
		return p;
	}
}

void camera_init(camera *cam, renderer_config r_conf, camera_config c_conf)
{
	vec3 vup = initv3(0, 1, 0);
	cam->lookfrom = c_conf.lookfrom;
	cam->lookat = c_conf.lookat;

	cam->lens_radius = c_conf.aperture / 2.0;
	cam->focus_dist = c_conf.focus_dist;

	cam->ray_max_length = DBL_MAX;
	cam->ray_min_length = 0.001;

	double aspect_ratio = (double)r_conf.rendering_width / (double)r_conf.rendering_height;

	double theta = c_conf.vfov * M_PI / 180.0;
	double h = tan(theta / 2.0);
	cam->vfov_r = theta;

	double viewport_height = 2.0 * h;
	double viewport_width = aspect_ratio * viewport_height;

	cam->w = unitv(subv(cam->lookfrom, cam->lookat));
	cam->u = unitv(crossv(vup, cam->w));
	cam->v = crossv(cam->w, cam->u);

	cam->origin = cam->lookfrom;
	cam->horizontal = mulv(cam->focus_dist*viewport_width, cam->u);
	cam->vertical = mulv(cam->focus_dist*viewport_height, cam->v);
	cam->lower_left_corner = subv(subv(subv(cam->origin, divv(2, cam->horizontal)), divv(2, cam->vertical)), mulv(cam->focus_dist, cam->w));

	return;
}

ray camera_get_ray(camera *cam, double s, double t)
{
	vec3 rd = mulv(cam->lens_radius, random_in_unit_disk());
	vec3 offset = addv(mulv(rd.e[0], cam->u), mulv(rd.e[1], cam->v));

	return gen_ray(addv(cam->origin, offset), subv(subv(addv(addv(cam->lower_left_corner, mulv(s, cam->horizontal)), mulv(t, cam->vertical)), cam->origin), offset));
}