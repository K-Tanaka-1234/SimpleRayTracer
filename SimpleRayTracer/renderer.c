#define _USE_MATH_DEFINES
#include <math.h>

#include "simple_ray_tracer.h"

#include "renderer.h"
#include "vector.h"
#include "vector_op.h"
#include "ray.h"
#include "objects.h"
#include "parametric_objects.h"
#include "camera.h"
#include "random.h"
#include "material.h"
#include "texture.h"

static camera cam;
obj_list objects;

void _renderer_init(int screen_rows, int screen_cols, int channels)
{
	init_object_list(&objects);

	return;
}

color3 calc_color(camera *cam, ray r, obj_list *list, int depth, color3 background)
{
	hit_record rec;

	if (depth <= 0) return initv3(0, 0, 0);
	
	if (!objects_intersection_test(r, cam->ray_min_length, cam->ray_max_length, list, &rec))
		return background;
		
	ray scattered;
	color3 attenuation;
	color3 emitted = rec.mat->emitted(&rec.mat->tex, rec.u, rec.v, rec.p);

	if (rec.mat->scatter(r, &rec, &attenuation, &scattered))
		return addv(emitted, mulvp(attenuation, calc_color(cam, scattered, list, depth - 1, background)));
	
	return emitted;
}

void _run_renderer(renderer_config r_conf, camera_config c_conf)
{
	camera_init(&cam, r_conf, c_conf);

	for (int i = 0; i < r_conf.rendering_height; i++)
	{
		for (int j = 0; j < r_conf.rendering_width; j++)
		{
			color3 c = zerosv3();

			for (int k = 0; k < r_conf.num_samples; k++)
			{
				double v = ((double)i + random_double()) / (r_conf.rendering_height - 1);
				double u = ((double)j + random_double()) / (r_conf.rendering_width - 1);

				ray r = camera_get_ray(&cam, u, v);
				
				c = addv(c, calc_color(&cam, r, &objects, r_conf.ray_max_depth, r_conf.background));
			}
			c = sqrtv(divv(r_conf.num_samples, c));

			g_frame_buffer[((r_conf.rendering_height - 1 - i)*r_conf.rendering_width + j)*r_conf.num_channels + 0] = (unsigned char)(255.0*c.e[2]);
			g_frame_buffer[((r_conf.rendering_height - 1 - i)*r_conf.rendering_width + j)*r_conf.num_channels + 1] = (unsigned char)(255.0*c.e[1]);
			g_frame_buffer[((r_conf.rendering_height - 1 - i)*r_conf.rendering_width + j)*r_conf.num_channels + 2] = (unsigned char)(255.0*c.e[0]);
		}
		//printf("%lf\n", (double)i / (double)(r_conf.rendering_height - 1));
	}

	return;
}

void _renderer_finish()
{
	clear_object_list(&objects);
}