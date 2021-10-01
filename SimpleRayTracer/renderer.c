#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
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

#define NUM_THREADS 12

static camera cam;
static renderer_config r_conf_t;
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

void rendering_line(void *arg)
{
	int i = *(int *)arg;

	for (int j = 0; j < r_conf_t.rendering_width; j++)
	{
		color3 c = zerosv3();

		for (int k = 0; k < r_conf_t.num_samples; k++)
		{
			double v = ((double)i + random_double()) / (r_conf_t.rendering_height - 1);
			double u = ((double)j + random_double()) / (r_conf_t.rendering_width - 1);

			ray r = camera_get_ray(&cam, u, v);

			c = addv(c, calc_color(&cam, r, &objects, r_conf_t.ray_max_depth, r_conf_t.background));
		}
		c = sqrtv(divv(r_conf_t.num_samples, c));

		g_frame_buffer[((r_conf_t.rendering_height - 1 - i)*r_conf_t.rendering_width + j)*r_conf_t.num_channels + 0] = (unsigned char)(255.0*c.e[2]);
		g_frame_buffer[((r_conf_t.rendering_height - 1 - i)*r_conf_t.rendering_width + j)*r_conf_t.num_channels + 1] = (unsigned char)(255.0*c.e[1]);
		g_frame_buffer[((r_conf_t.rendering_height - 1 - i)*r_conf_t.rendering_width + j)*r_conf_t.num_channels + 2] = (unsigned char)(255.0*c.e[0]);
	}
}

void _run_renderer(renderer_config r_conf, camera_config c_conf)
{
	r_conf_t = r_conf;

	HANDLE handle[NUM_THREADS];
	int i[NUM_THREADS];

	camera_init(&cam, r_conf, c_conf);

	for (int t = 0; t < r_conf.rendering_height / NUM_THREADS + 1; t++)
	{
		int tmp = ((t + 1)*NUM_THREADS > r_conf.rendering_height) ? r_conf.rendering_height - t*NUM_THREADS : NUM_THREADS;
		
		for (int s = 0; s < tmp; s++) 
		{
			i[s] = t * NUM_THREADS + s;
			handle[s] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)rendering_line, &i[s], 0, NULL);
		}

		WaitForMultipleObjects(NUM_THREADS,	handle, TRUE, INFINITE);
		
		for(int s = 0;s < tmp;s++) CloseHandle(handle[s]);
	}

	return;
}

void _renderer_finish()
{
	clear_object_list(&objects);
}