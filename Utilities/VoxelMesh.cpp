#include "VoxelMesh.h"

#include "..\NoiseLib.h"
#include "..\DirectX\VertexTypes.h"

#include <stdio.h>
#include <assert.h>

const unsigned int OCTAVES = 4;
const float PERSISTANCE = 0.25f;
const unsigned int ZOOM = 20;
const float THRESHOLD = 1.f;

const float BLOCK_WIDTH = 1.f;
const float BLOCK_HALF_WIDTH = BLOCK_WIDTH / 2.f;

#define INDEX(a,b,c) ((c)*width*height+(b)*width+(a))
#define _SUBX (sx+x*BLOCK_WIDTH-BLOCK_HALF_WIDTH)
#define _ADDX (sx+x*BLOCK_WIDTH+BLOCK_HALF_WIDTH)
#define _SUBY (sy+y*BLOCK_WIDTH-BLOCK_HALF_WIDTH)
#define _ADDY (sy+y*BLOCK_WIDTH+BLOCK_HALF_WIDTH)
#define _SUBZ (sz+z*BLOCK_WIDTH-BLOCK_HALF_WIDTH)
#define _ADDZ (sz+z*BLOCK_WIDTH+BLOCK_HALF_WIDTH)

unsigned int GenerateTerrain( 
	int sx, int sy, int sz, /* world coord offsets for building vertex buffer */
	unsigned int width, unsigned int height, unsigned int depth, /* size of array to extract, and dimensions of mesh */
	unsigned int seed, char * voxel_data )
{
	float * raw_data = new float[ width*height*depth ];

	perlin3d( seed, sx, sy, sz, width, height, depth, OCTAVES, PERSISTANCE, ZOOM, raw_data );
	
	/* return value */
	unsigned int num_tris = 0;
	
	int index;
	unsigned int ncubes = 0;
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				int yy = ( height - 1 ) - y;
				index = INDEX(x,y,z);
				raw_data[index] = ( raw_data[index] + 1.75f ) * float(( height - 1 ) - y) / float(height);
				if( raw_data[index] > THRESHOLD ) voxel_data[index] = 1; else voxel_data[index] = 0;
			}
		}
	}

	for( unsigned int z = 0; z < depth; ++z )
	{
		for (unsigned int y = 0; y < height; ++y)
		{
			for (unsigned int x = 0; x < width; ++x)
			{
				int yy = ( height - 1 ) - y;
				index = INDEX(x,y,z);

				if( voxel_data[ index ] > 0 )
				{
					++ncubes;			
					
					//if( x == 0			|| ( (voxel_data[INDEX(x-1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 2; }
					//if( x == (width-1)	|| ( (voxel_data[INDEX(x+1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 4; }
					//if( y == 0			|| ( (voxel_data[INDEX(x  ,y-1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 8; }
					//if( y == (height-1) || ( (voxel_data[INDEX(x  ,y+1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 16; }
					//if( z == 0			|| ( (voxel_data[INDEX(x  ,y  ,z-1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 32; }
					//if( z == (depth-1)  || ( (voxel_data[INDEX(x  ,y  ,z+1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 64; }
					
					if( x > 0			&& ( (voxel_data[INDEX(x-1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 2; }
					if( x < (width-1)	&& ( (voxel_data[INDEX(x+1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 4; }
					if( y > 0			&& ( (voxel_data[INDEX(x  ,y-1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 8; }
					if( y < (height-1)  && ( (voxel_data[INDEX(x  ,y+1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 16; }
					if( z > 0			&& ( (voxel_data[INDEX(x  ,y  ,z-1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 32; }
					if( z < (depth-1)   && ( (voxel_data[INDEX(x  ,y  ,z+1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 64; }

					//	
					//if( voxel_data[index] & 4 == 0 )
					//{
					//printf("%i - ", voxel_data[index] );
					//for( unsigned int bdd = 0; bdd < 8; ++bdd )
					//{
					//	int bd = 8 - 1 - bdd;
					//	if( ((1<<bd)&voxel_data[index]) == (1<<bd) ) printf("1"); else printf("0");
					//}
					//printf(" - bits\n");
					//}
				}
			}
		}
	}
	printf("num_tris inside: %i\n", num_tris );
	printf("num_cubes insid: %i\n", ncubes );
	return num_tris;
}

LPDIRECT3DVERTEXBUFFER9 BuildMesh( 
	int sx, int sy, int sz, 
	unsigned int width, unsigned int height, unsigned int depth,
	char * voxel_data, unsigned int num_tris,
	const LPDIRECT3DDEVICE9 dxdevice )
{
	LPDIRECT3DVERTEXBUFFER9 vert_buffer;
	Vertex * vert_data;
	unsigned int num_verts = num_tris * 3;

	dxdevice->CreateVertexBuffer( 
		num_verts * sizeof( Vertex ), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Vertex::VertexFVF,
		D3DPOOL_DEFAULT,
		&vert_buffer,
		0 );

	HRESULT err = vert_buffer->Lock( 0, num_verts * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );

	unsigned int index;
	unsigned int c = 0;
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = INDEX(x,y,z);
				
				if( ( voxel_data[index] & 2 ) == 2 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = -1.f; vert_data[i].ny = vert_data[i].nz = 0.f;
						vert_data[i].color = D3DCOLOR_XRGB( 200, 100, 100 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
				}
				if( ( voxel_data[index] & 4 ) == 4 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = 1.f; vert_data[i].ny = vert_data[i].nz = 0.f;
						vert_data[i].color = D3DCOLOR_XRGB( 200, 100, 100 );
					}
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
				}
				if( ( voxel_data[index] & 8 ) == 8 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].nz = 0.f; vert_data[i].ny = -1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 150, 100 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
				}
				if( ( voxel_data[index] & 16 ) == 16 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].nz = 0.f; vert_data[i].ny = 1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 150, 100 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
				}
				if( ( voxel_data[index] & 32 ) == 32 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].ny = 0.f; vert_data[i].nz = -1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 100, 200 );
					}
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
				}
				if( ( voxel_data[index] & 64 ) == 64 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].ny = 0.f; vert_data[i].nz = 1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 100, 200 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
				}
			}
		}
	}

	vert_buffer->Unlock();
	return vert_buffer;
}

/* voxel_array is padded with one extra cell on all sides to calculate correct chunk edges */
unsigned int GenerateTerrainWithPadding( 
	int sx, int sy, int sz, /* world coord offsets for building vertex buffer */
	unsigned int width, unsigned int height, unsigned int depth, /* size of array to extract, and dimensions of mesh */
	unsigned int seed, char * voxel_data )
{
	width += 2;
	height += 2;
	depth += 2;
	sx -= 1;
	sy -= 1;
	sz -= 1;

	float * raw_data = new float[ width*height*depth ];

	perlin3d( seed, sx, sy, sz, width, height, depth, OCTAVES, PERSISTANCE, ZOOM, raw_data );
	
	/* return value */
	unsigned int num_tris = 0;
	
	/* turn on / off voxel_data (3-D terrain block grid) 
	   voxel_data only stores binary 1/0 values for exist/not exist
	*/
	int index;
	for (unsigned int z = 0; z < depth; ++z)
	{
		for (unsigned int y = 0; y < height; ++y)
		{
			for (unsigned int x = 0; x < width; ++x)
			{
				int yy = ( height - 1 ) - y;
				index = INDEX(x,y,z);
				raw_data[index] = ( raw_data[index] + 1.75f ) * float(( height - 1 ) - y) / float(height);
				if( raw_data[index] > THRESHOLD ) voxel_data[index] = 1; else voxel_data[index] = 0;
			}
		}
	}

	delete [] raw_data;
	
	/* recompute voxel_data values into a bit-string representing if the cube DOES NOT have a neighbor in that cardinal direction
	   00123456
	   01 - 00000001 - Left
	   02 - 00000010 - Right
	   04 - 00000100 - below
	   08 - 00001000 - Above
	   16 - 00010000 - Back  (-Z)
	   32 - 00100000 - Front (+Z)
	   */
	for (unsigned int z = 1; z < depth - 1; ++z)
	{
		for (unsigned int y = 1; y < height - 1; ++y)
		{
			for (unsigned int x = 1; x < width - 1; ++x)
			{
				index = INDEX(x,y,z);

				if( voxel_data[ index ] > 0 )
				{
					
					//if( x == 0			|| ( (voxel_data[INDEX(x-1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 2; }
					//if( x == (width-1)	|| ( (voxel_data[INDEX(x+1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 4; }
					//if( y == 0			|| ( (voxel_data[INDEX(x  ,y-1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 8; }
					//if( y == (height-1) || ( (voxel_data[INDEX(x  ,y+1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 16; }
					//if( z == 0			|| ( (voxel_data[INDEX(x  ,y  ,z-1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 32; }
					//if( z == (depth-1)  || ( (voxel_data[INDEX(x  ,y  ,z+1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 64; }
					
					if( x > 0			&& ( (voxel_data[INDEX(x-1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 2; }
					if( x < (width-1)	&& ( (voxel_data[INDEX(x+1,y  ,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 4; }
					if( y > 0			&& ( (voxel_data[INDEX(x  ,y-1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 8; }
					if( y < (height-1)  && ( (voxel_data[INDEX(x  ,y+1,z  )] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 16; }
					if( z > 0			&& ( (voxel_data[INDEX(x  ,y  ,z-1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 32; }
					if( z < (depth-1)   && ( (voxel_data[INDEX(x  ,y  ,z+1)] & 1) == 0 ) ) { num_tris += 2; voxel_data[index] |= 64; }

				}
			}
		}
	}
	printf("num_tris inside: %i\n", num_tris );
	return num_tris;
}

int FindRectX( 
	int sx, int sy, int sz,
	int width, int height, int depth,
	char * voxel_data, int * cdata,
	int ccount, unsigned int flag )
{
	int index;
	for( int x = sx; x < width; ++x )
	{
		index = sz*width*height+sy*width+x;
		if( cdata[index] != 0 || ((voxel_data[index]&127)==127) || ( (voxel_data[index] & flag) != flag ) )
		{
			return x-1;
		}
		cdata[index] = ccount;
	}

	return width-1;
}

int FindRectZ( 
	int sx, int sy, int sz,
	int width, int height, int depth,
	char * voxel_data, int * cdata,
	int ccount, unsigned int flag )
{
	int index;
	for( int z = sz; z < depth; ++z )
	{
		index = z*width*height+sy*width+sx;
		if( cdata[index] != 0 || ((voxel_data[index]&127)==127) || ( (voxel_data[index] & flag) != flag ) )
		{
			return z-1;
		}
		cdata[index] = ccount;
	}

	return depth-1;
}

int FindRectXY( 
	int sx, int sy, int sz, int ex,
	int width, int height, int depth,
	char * voxel_data, int * cdata,
	int ccount, unsigned int flag )
{
	int index;
	bool bad_line;
	for( int y = sy; y < height; ++y )
	{
		bad_line = false;
		for( int x = sx; x <= ex; ++x )
		{
			index = sz*width*height+y*width+x;
			if( cdata[index] != 0 || ((voxel_data[index]&127)==127) || ( (voxel_data[index] & flag) != flag ) )
			{
				bad_line = true;
				for( int _x = sx; _x < x; ++_x ) cdata[sz*width*height+y*width+_x] = 0;
				return y-1;
			}else
			{
				cdata[index] = ccount;
			}
		}
	}

	return height-1;
}

int FindRectXZ( 
	int sx, int sy, int sz, int ex,
	int width, int height, int depth,
	char * voxel_data, int * cdata,
	int ccount, unsigned int flag )
{
	int index;
	bool bad_line;
	for( int z = sz; z < depth; ++z )
	{
		bad_line = false;
		for( int x = sx; x <= ex; ++x )
		{
			index = z*width*height+sy*width+x;
			if( cdata[index] != 0 || ((voxel_data[index]&127)==127) || ( (voxel_data[index] & flag) != flag ) )
			{
				bad_line = true;
				for( int _x = sx; _x < x; ++_x ) cdata[z*width*height+sy*width+_x] = 0;
				return z-1;
			}else
			{
				cdata[index] = ccount;
			}
		}
	}

	return depth-1;
}

int FindRectZY( 
	int sx, int sy, int sz, int ez,
	int width, int height, int depth,
	char * voxel_data, int * cdata,
	int ccount, unsigned int flag )
{
	int index;
	bool bad_line;
	for( int y = sy; y < height; ++y )
	{
		bad_line = false;
		for( int z = sz; z <= ez; ++z )
		{
			index = z*width*height+y*width+sx;
			if( cdata[index] != 0 || ((voxel_data[index]&127)==127) || ( (voxel_data[index] & flag) != flag ) )
			{
				bad_line = true;
				for( int _z = sz; _z < z; ++_z ) cdata[_z*width*height+y*width+sx] = 0;
				return y-1;
			}else
			{
				cdata[index] = ccount;
			}
		}
	}

	return height-1;
}

LPDIRECT3DVERTEXBUFFER9 OptimizeVoxelTriangles( 
	int sx, int sy, int sz,
	unsigned int width, unsigned int height, unsigned int depth,
	char * voxel_data, const LPDIRECT3DDEVICE9 dxdevice, unsigned int & num_triangles )
{
	unsigned int size = (width)*(height)*(depth);
	
	/* color data to identify processed ( found ) quads */
	int * cdata = new int[ size ];
	/* unique id to represent a single quad */
	int ccount = 1;

	/* array of quad start-end points */
	//VPoint * vpoints = new VPoint[ size * 6 ];
	Vertex * vpoints = new Vertex[ size * 36 ];
	unsigned int num_points = 0;
	
	unsigned int index;
	unsigned int e1, e2;
	memset( cdata, 0, sizeof(int) * size );
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = z*height*width+y*width+x;
				/* x-y plane negative side */
				if( cdata[index] == 0 && ( (voxel_data[index]&33) == 33 ) && ( (voxel_data[index]&127) != 127 ) )
				{
					e1 = FindRectX( x, y, z, width, height, depth, voxel_data, cdata, ccount, 33 );
					e2 = FindRectXY( x, y+1, z, e1, width, height, depth, voxel_data, cdata, ccount, 33 );
					++ccount;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = 1.f;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = 1.f;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = 1.f;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = 1.f;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = 1.f;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = 1.f;
					++num_points;
				}
			}
		}
	}
	ccount = 1;
	memset( cdata, 0, sizeof(int) * size );
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = z*height*width+y*width+x;
				/* x-y plane positive side */
				if( cdata[index] == 0 && ( (voxel_data[index]&65) == 65 ) && ( (voxel_data[index]&127) != 127 ) )
				{
					e1 = FindRectX( x, y, z, width, height, depth, voxel_data, cdata, ccount, 65 );
					e2 = FindRectXY( x, y+1, z, e1, width, height, depth, voxel_data, cdata, ccount, 65 );
					++ccount;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = -1.f;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = -1.f;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = -1.f;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = -1.f;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = -1.f;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 0;vpoints[num_points].nz = -1.f;
					++num_points;
				}
			}
		}
	}
	
	ccount = 1;
	memset( cdata, 0, sizeof(int) * size );
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = z*height*width+y*width+x;
				/* x-z plane negative side */
				if( cdata[index] == 0 && ( (voxel_data[index]&9) == 9 ) && ( (voxel_data[index]&127) != 127 ) )
				{
					e1 = FindRectX( x, y, z, width, height, depth, voxel_data, cdata, ccount, 9 );
					e2 = FindRectXZ( x, y, z+1, e1, width, height, depth, voxel_data, cdata, ccount, 9 );
					++ccount;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = -1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = -1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = -1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = -1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = -1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = -1.f;vpoints[num_points].nz = 0;
					++num_points;
				}
			}
		}
	}
	
	ccount = 1;
	memset( cdata, 0, sizeof(int) * size );
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = z*height*width+y*width+x;
				/* x-z plane positive side */
				if( cdata[index] == 0 && ( (voxel_data[index]&16) == 16 ) && ( (voxel_data[index]&127) != 127 ) )
				{
					e1 = FindRectX( x, y, z, width, height, depth, voxel_data, cdata, ccount, 16 );
					e2 = FindRectXZ( x, y, z+1, e1, width, height, depth, voxel_data, cdata, ccount, 16 );
					++ccount;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 1.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 0;vpoints[num_points].ny = 1.f;vpoints[num_points].nz = 0;
					++num_points;
				}
			}
		}
	}
	
	ccount = 1;
	memset( cdata, 0, sizeof(int) * size );
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = z*height*width+y*width+x;
				/* z-y plane negative side */
				if( cdata[index] == 0 && ( (voxel_data[index]&3) == 3 ) && ( (voxel_data[index]&127) != 127 ) )
				{
					e1 = FindRectZ( x, y, z, width, height, depth, voxel_data, cdata, ccount, 3 );
					e2 = FindRectZY( x, y+1, z, e1, width, height, depth, voxel_data, cdata, ccount, 3 );
					++ccount;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = -1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = -1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = -1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = -1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = -1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = -1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
				}
			}
		}
	}
	
	ccount = 1;
	memset( cdata, 0, sizeof(int) * size );
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = z*height*width+y*width+x;
				/* z-y plane positive side */
				if( cdata[index] == 0 && ( (voxel_data[index]&5) == 5 ) && ( (voxel_data[index]&127) != 127 ) )
				{
					e1 = FindRectZ( x, y, z, width, height, depth, voxel_data, cdata, ccount, 5 );
					e2 = FindRectZY( x, y+1, z, e1, width, height, depth, voxel_data, cdata, ccount, 5 );
					++ccount;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + e1) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + y) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
					vpoints[num_points].x = (sx + x) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].y = (sy + e2) * BLOCK_WIDTH + BLOCK_HALF_WIDTH;
					vpoints[num_points].z = (sz + z) * BLOCK_WIDTH - BLOCK_HALF_WIDTH;
					vpoints[num_points].nx = 1.f;vpoints[num_points].ny = 0.f;vpoints[num_points].nz = 0;
					++num_points;
				}
			}
		}
	}

	LPDIRECT3DVERTEXBUFFER9 vert_buffer;
	Vertex * vert_data;

	dxdevice->CreateVertexBuffer( 
		num_points * sizeof( Vertex ), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Vertex::VertexFVF,
		D3DPOOL_DEFAULT,
		&vert_buffer,
		0 );

	HRESULT err = vert_buffer->Lock( 0, num_points * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );
	index = 0;
	DWORD co = D3DCOLOR_XRGB( rand()%100+100,rand()%100+100,rand()%100+100 );
	for( unsigned int i = 0; i < num_points; ++i )
	{
		if( i%6 == 0 ) co = D3DCOLOR_XRGB( rand()%100+100,rand()%100+100,rand()%100+100 );
		vert_data[i] = vpoints[i];
		vert_data[i].color = co;//D3DCOLOR_XRGB( 100, 150, 100 );
	}
	
	
	delete [] vpoints;
	delete [] cdata;

	
	vert_buffer->Unlock();
	num_triangles = num_points / 3;

	return vert_buffer;
}

/* voxel_array is padded with one extra cell on all sides to calculate correct chunk edges */
LPDIRECT3DVERTEXBUFFER9 BuildMeshWithPadding( 
	int sx, int sy, int sz, 
	unsigned int width, unsigned int height, unsigned int depth,
	char * voxel_data, unsigned int num_tris,
	const LPDIRECT3DDEVICE9 dxdevice )
{
	LPDIRECT3DVERTEXBUFFER9 vert_buffer;
	Vertex * vert_data;
	unsigned int num_verts = num_tris * 3;

	dxdevice->CreateVertexBuffer( 
		num_verts * sizeof( Vertex ), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Vertex::VertexFVF,
		D3DPOOL_DEFAULT,
		&vert_buffer,
		0 );

	HRESULT err = vert_buffer->Lock( 0, num_verts * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );

	unsigned int index;
	unsigned int c = 0;
	for( unsigned int z = 0; z < depth; ++z )
	{
		for( unsigned int y = 0; y < height; ++y )
		{
			for( unsigned int x = 0; x < width; ++x )
			{
				index = (z+1)*(width+2)*(height+2)+(y+1)*(width+2)+x+1;
				
				if( ( voxel_data[index] & 2 ) == 2 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = -1.f; vert_data[i].ny = vert_data[i].nz = 0.f;
						vert_data[i].color = D3DCOLOR_XRGB( 200, 100, 100 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
				}
				if( ( voxel_data[index] & 4 ) == 4 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = 1.f; vert_data[i].ny = vert_data[i].nz = 0.f;
						vert_data[i].color = D3DCOLOR_XRGB( 200, 100, 100 );
					}
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
				}
				if( ( voxel_data[index] & 8 ) == 8 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].nz = 0.f; vert_data[i].ny = -1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 150, 100 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
				}
				if( ( voxel_data[index] & 16 ) == 16 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].nz = 0.f; vert_data[i].ny = 1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 150, 100 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
				}
				if( ( voxel_data[index] & 32 ) == 32 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].ny = 0.f; vert_data[i].nz = -1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 100, 200 );
					}
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _SUBZ; ++c;
				}
				if( ( voxel_data[index] & 64 ) == 64 )
				{
					for( unsigned int i = c; i < c+6; ++i )
					{
						vert_data[i].nx = vert_data[i].ny = 0.f; vert_data[i].nz = 1.f;
						vert_data[i].color = D3DCOLOR_XRGB( 100, 100, 200 );
					}
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _SUBY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _ADDX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
					vert_data[c].x = _SUBX; vert_data[c].y = _ADDY; vert_data[c].z = _ADDZ; ++c;
				}
			}
		}
	}

	vert_buffer->Unlock();
	return vert_buffer;
}
