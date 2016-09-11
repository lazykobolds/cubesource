#pragma once

#include <d3d9.h>

#include "ChunkReader.h"
#include "..\Utilities\SharedResource.h"

class Mesh : public SharedResource
{
public:
	Mesh( void );
	~Mesh( void );

	void Initialize( IDirect3DDevice9 * d3ddev );
	void LoadFromFile( ChunkReader & file );
	void Render( LPDIRECT3DDEVICE9 dxdevice );

private:

	unsigned int VertexType;
	unsigned int VertexSize;
	unsigned int NumIndices;
	unsigned int * IndexBufferData;
	unsigned int NumVertices;
	unsigned char * VertexBufferData;
	unsigned int NumPrimitives;

	IDirect3DVertexBuffer9 * VertexBuffer;
	IDirect3DIndexBuffer9 * IndexBuffer;
	IDirect3DVertexDeclaration9* VertexDeclaration;
};