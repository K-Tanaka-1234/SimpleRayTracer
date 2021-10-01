#pragma once

#include "vector.h"
#include "stdint.h"

typedef struct TEXTURE {
	color3(*tex_value)(struct TEXTURE *tex, double u, double v, point3 p);
	color3 color;
	uint8_t *bmp_ptr;	// 24 bit bmp ptr
	int bmp_cols;
	int bmp_rows;
}texture;

/*
 *		Solid color texture		--------------------------------------------------
 */

texture _create_solid_color_texture(color3 color);

/*
 *		Bitmap texture		------------------------------------------------------
 */

texture _create_bitmap_texture(uint8_t *bmp_ptr, int bmp_cols, int bmp_rows);