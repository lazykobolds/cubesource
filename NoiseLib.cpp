#include "NoiseLib.h"

float random(int seed, int x, int y, int z)
{
	int n = seed + x*73 + y*179 + z*283;
	n = (n << 13) ^ n;
	int m = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ((float)m / 1073741824.0f);
}

// cosine interpolation between a and b by a factor of x
float lerp(float a, float b, float x)
{
	float ft = x * 3.1415927f;
	float f = (1.0f - cosf(ft)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

void perlin3d( 
	int seed, 
	int xoffset, int yoffset, int zoffset,
	unsigned int width, unsigned int height, unsigned int depth,
	unsigned int octaves, float persistence, int zoom, 
	float * data_out )
{
	float frequency, amplitude, freq_zoom;
	float xf, yf, zf;
	int xi, yi, zi;
	float a, b, c, d, e, f, g, h;
	float lxa, lxb, lxc, lxd, lya, lyb, lz;
	float density;

	for( unsigned int x = 0; x < width; ++x )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int z = 0; z < depth; ++z )
			{
				density = 0.f;
				for( unsigned int oct = 0; oct < octaves; ++oct )
				{
					frequency = powf( 2.f, float(oct) );
					amplitude = powf( persistence, float(oct) );
					freq_zoom = frequency / (float)zoom;

					/*
					var xFloor = ~~x;
					var yFloor = ~~y;
					var zFloor = ~~z;

					var iX = xFloor & 255;
					var iY = yFloor & 255;
					var iZ = zFloor & 255;
					*/

					xf = float( x+xoffset+0xffff ) * freq_zoom;
					yf = float( y+yoffset+0xffff ) * freq_zoom;
					zf = float( z+zoffset+0xffff ) * freq_zoom;

					xi = int(xf);
					yi = int(yf);
					zi = int(zf);

					xf -= xi;
					yf -= yi;
					zf -= zi;

					a = random( seed, xi, yi, zi );
					b = random( seed, xi+1, yi, zi );
					c = random( seed, xi, yi+1, zi );
					d = random( seed, xi, yi, zi+1 );
					e = random( seed, xi+1, yi+1, zi );
					f = random( seed, xi+1, yi, zi+1 );
					g = random( seed, xi, yi+1, zi+1 );
					h = random( seed, xi+1, yi+1, zi+1 );

					lxa = lerp( a, b, xf );
					lxb = lerp( c, e, xf );
					lxc = lerp( d, f, xf );
					lxd = lerp( g, h, xf );

					lya = lerp( lxa, lxb, yf );
					lyb = lerp( lxc, lxd, yf );

					lz = lerp( lya, lyb, zf );

					density += lz * amplitude;
				}

				data_out[ z*width*height + y*width + x ] = density;// octaves;
			}
		}
	}
}