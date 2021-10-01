#include "simple_ray_tracer.h"
#include "renderer.h"
#include "random.h"
#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "objects.h"
#include "material.h"
#include "texture.h"

#include "parametric_objects.h"

int g_enable_direct_fb = 0;
uint8_t *g_frame_buffer = NULL;
int g_screen_rows = 0;
int g_screen_cols = 0;
int g_num_channels = 3;

DECLSPEC void init(int screen_rows, int screen_cols)
{
	g_screen_rows = screen_rows;
	g_screen_cols = screen_cols;

	g_frame_buffer = (uint8_t *)malloc(sizeof(uint8_t) * g_screen_rows * g_screen_cols * g_num_channels);

	for (int i = 0; i < g_screen_rows * g_screen_cols * g_num_channels; i++)g_frame_buffer[i] = 255;

	pcg32_init();
	_renderer_init(g_screen_rows, g_screen_cols, g_num_channels);

	return;
}

DECLSPEC void enable_direct_frame_buffer(uint8_t * buffer)
{
	if (g_frame_buffer != NULL) free(g_frame_buffer);
	g_frame_buffer = buffer;
	g_enable_direct_fb = 1;

	return;
}

DECLSPEC void exchange_frame_buffer(uint8_t *buffer)
{
	if (!g_enable_direct_fb)
		for (int i = 0; i < g_screen_rows * g_screen_cols * g_num_channels; i++) buffer[i] = g_frame_buffer[i];

	return;
}

DECLSPEC void clear_frame_buffer()
{
	for (int i = 0; i < g_screen_rows * g_screen_cols * g_num_channels; i++) g_frame_buffer[i] = 0;

	return;
}

DECLSPEC void free_frame_buffer()
{
	if (!g_enable_direct_fb) free(g_frame_buffer);
	return;
}



DECLSPEC void run_renderer(renderer_config r_conf, camera_config c_conf)
{
	_run_renderer(r_conf, c_conf);
	return;
}

DECLSPEC void renderer_finish()
{
	_renderer_finish();
	return;
}



DECLSPEC void create_lambertian_material(texture tex, material *mat)
{
	*mat = _create_lambertian_material(tex);
}

DECLSPEC void create_metal_material(texture tex, double fuzziness, material *mat)
{
	*mat = _create_metal_material(tex, fuzziness);
}

DECLSPEC void create_dielectric_material(texture tex, double ri, material *mat)
{
	*mat = _create_dielectric_material(tex, ri);
}

DECLSPEC void create_diffuse_light_material(texture tex, material *mat)
{
	*mat = _create_diffuse_light_material(tex);
}



DECLSPEC void create_solid_color_texture(color3 color, texture *tex)
{
	*tex= _create_solid_color_texture(color);
}

DECLSPEC void create_bitmap_texture(uint8_t *bmp_ptr, int bmp_cols, int bmp_rows, texture *tex)
{
	*tex = _create_bitmap_texture(bmp_ptr, bmp_cols, bmp_rows);
}



DECLSPEC void add_object(object obj, material mat)
{
	object_list_append(&objects, obj, mat);
}



DECLSPEC void create_sphere_object(point3 center, double r, object *obj)
{
	*obj = _create_sphere_object(center, r);
}