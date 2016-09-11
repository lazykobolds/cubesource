#include "Chunk.h"
#include "..\Utilities\VoxelMesh.h"

Chunk::Chunk(void)
	: BaseComponent()
{
	Initilize(0, 0, 0, 0, 0, 0, 0);
}

Chunk::Chunk(unsigned int sx, unsigned int sy, unsigned int sz, unsigned int w, unsigned int h, unsigned int d)
	: BaseComponent()
{
	Initilize(sx, sy, sz, w, h, d, 0);
}

void Chunk::Initilize( unsigned int sx, unsigned int sy, unsigned int sz, unsigned int w, unsigned int h, unsigned int d, char * data )
{
	xstart = sx;
	ystart = sy;
	zstart = sz;
	width = w;
	height = h;
	depth = d;
	voxel_data = data;
	vert_data = 0;
	num_triangles = 0;

	Activate();
}

Chunk::~Chunk( void )
{
	if( voxel_data ) 
		delete voxel_data;

	if( vert_data ) 
		vert_data->Release();
}

///
void Chunk::SetVoxelData( char * data )
{
	if( voxel_data ) 
		delete[] voxel_data;

	voxel_data = data;
}

/// <summary>
/// Generates the directx vertex buffer and stores it in the chunk
/// </summary>
void Chunk::GenerateVertexBuffer( LPDIRECT3DDEVICE9 d3ddev )
{
	vert_data = OptimizeVoxelTriangles( xstart, ystart, zstart, width, height, depth, voxel_data, d3ddev, num_triangles );
}

bool Chunk::ContainsPoint( D3DXVECTOR2 & point )
{
	if( point.x > xstart && point.x < xstart + width && point.y > zstart && point.y < zstart + depth )
	{
		return true;
	}
	else
	{
		return false;
	}
}
