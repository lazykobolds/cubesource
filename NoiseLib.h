#pragma once

#include <math.h>

float random(int seed, int x, int y, int z);

// cosine interpolation between a and b by a factor of x
float lerp(float a, float b, float x);

void perlin3d( 
	int seed, 
	int xoffset, int yoffset, int zoffset,
	unsigned int width, unsigned int height, unsigned int depth,
	unsigned int octaves, float persistence, int zoom,
	float * data_out );