#pragma once

#include "vector.h"

typedef struct CAMERA_CONFIG camera_config;

typedef struct RENDERER_CONFIG {
	int rendering_width;
	int rendering_height;
	int num_channels;
	int num_samples;
	int ray_max_depth;
	color3 background;
}renderer_config;

void _renderer_init(int screen_rows, int screen_cols, int channels);
void _run_renderer(renderer_config r_conf, camera_config c_conf);
void _renderer_finish();