#pragma once

#include <d3dx9.h>
#include "BaseComponent.h"

class Chunk : public BaseComponent
{
public:

	Chunk( void );
	Chunk( unsigned int sx, unsigned int sy, unsigned int sz, unsigned int w, unsigned int h, unsigned int d );
	~Chunk( void );

	void Initilize( unsigned int sx, unsigned int sy, unsigned int sz, unsigned int w, unsigned int h, unsigned int d, char * data );
	void SetVoxelData( char * data );
	void GenerateVertexBuffer( LPDIRECT3DDEVICE9 d3ddev );

	bool ContainsPoint( D3DXVECTOR2 & point );
	
	unsigned int xstart, ystart, zstart;
	unsigned int width, height, depth;

	char * voxel_data;

	LPDIRECT3DVERTEXBUFFER9 vert_data;
	unsigned int num_triangles;
};