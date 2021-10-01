#include "texture.h"
#include "vector.h"
#include "vector_op.h"
#include "utils.h"
#include <stdio.h>

/*
 *		Solid color texture		--------------------------------------------------
 */
color3 tex_value_solid_color(texture *tex, double u, double v, point3 p);

texture _create_solid_color_texture(color3 color)
{
	texture tex;
	tex.tex_value = tex_value_solid_color;
	tex.color = color;
	tex.bmp_ptr = NULL;
	tex.bmp_cols = 0;
	tex.bmp_rows = 0;

	return tex;
}

color3 tex_value_solid_color(texture *tex, double u, double v, point3 p)
{
	return tex->color;
}

/*
 *		Bitmap texture		------------------------------------------------------
 */

color3 tex_value_bitmap(texture *tex, double u, double v, point3 p);

texture _create_bitmap_texture(uint8_t * bmp_ptr, int bmp_cols, int bmp_rows)
{
	texture tex;
	tex.tex_value = tex_value_bitmap;
	tex.color = zerosv3();
	tex.bmp_ptr = bmp_ptr;
	tex.bmp_cols = bmp_cols;
	tex.bmp_rows = bmp_rows;

	return tex;
}

color3 tex_value_bitmap(texture *tex, double u, double v, point3 p)
{
	if (tex->bmp_ptr == NULL)
		return initv3(1, 0, 1);

	u = clamp(u, 0.0, 1.0);
	v = 1.0 - clamp(v, 0.0, 1.0);
	
	int j = (int)(u * (double)tex->bmp_cols);
	int i = (int)(v * (double)tex->bmp_rows);

	if (j >= tex->bmp_cols) j = tex->bmp_cols - 1;
	if (i >= tex->bmp_rows) i = tex->bmp_rows - 1;

	return initv3(
		(double)tex->bmp_ptr[(i*tex->bmp_cols + j) * 3 + 0] / 255.0,
		(double)tex->bmp_ptr[(i*tex->bmp_cols + j) * 3 + 1] / 255.0,
		(double)tex->bmp_ptr[(i*tex->bmp_cols + j) * 3 + 2] / 255.0
	);
}