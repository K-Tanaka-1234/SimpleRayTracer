#pragma once

#ifdef _MSC_VER
	#ifndef SIMPLE_RAY_TRACER_IMPORT_DLL
		#define DECLSPEC __declspec(dllexport)
		#else
		#define DECLSPEC __declspec(dllimport)
	#endif
#elif #ifdef __GNUC__
	#ifndef SIMPLE_RAY_TRACER_IMPORT_DLL
		#define DECLSPEC __attribute__((visibility("default")))
		#else	
		#define DECLSPEC
	#endif
#endif

#include <stdio.h>
#include "renderer.h"
#include "camera.h"
#include "objects.h"
#include "material.h"
#include "texture.h"

#include "parametric_objects.h"

extern uint8_t *g_frame_buffer;
extern obj_list objects;

DECLSPEC void init(int screen_rows, int screen_cols);
DECLSPEC void enable_direct_frame_buffer(uint8_t *buffer);
DECLSPEC void exchange_frame_buffer(uint8_t *buffer);
DECLSPEC void clear_frame_buffer();
DECLSPEC void free_frame_buffer();
DECLSPEC void run_renderer(renderer_config r_conf, camera_config c_conf);
DECLSPEC void renderer_finish();

/*
 *	Materials
 */

DECLSPEC void create_lambertian_material(texture tex, material *mat);
DECLSPEC void create_metal_material(texture tex, double fuzziness, material *mat);
DECLSPEC void create_dielectric_material(texture tex, double ri, material *mat);
DECLSPEC void create_diffuse_light_material(texture tex, material *mat);

/*
 *	Texture
 */

DECLSPEC void create_solid_color_texture(color3 color, texture *tex);
DECLSPEC void create_bitmap_texture(uint8_t *bmp_ptr, int bmp_cols, int bmp_rows, texture *tex);

/*
 *	Objects
 */

DECLSPEC void add_object(object obj, material mat);

// Sphere object

DECLSPEC void create_sphere_object(point3 center, double r, object *obj);