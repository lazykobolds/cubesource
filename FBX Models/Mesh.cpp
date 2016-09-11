#include "Mesh.h"
#include "..\DirectX\VertexTypes.h"

Mesh::Mesh( void )
{
	VertexType = 0;
	VertexSize = 0;
	NumIndices = 0;
	IndexBufferData = NULL;
	NumVertices = 0;
	VertexBufferData = NULL;
	NumPrimitives = 0;

	VertexBuffer = NULL;
	IndexBuffer = NULL;
	VertexDeclaration = NULL;
}

Mesh::~Mesh( void )
{
	if( IndexBufferData ) delete [] IndexBufferData;
	if( VertexBufferData ) delete [] VertexBufferData;

	if( VertexBuffer ) VertexBuffer->Release();
	if( IndexBuffer ) IndexBuffer->Release();
	if( VertexDeclaration ) VertexDeclaration->Release();
}

void Mesh::Initialize( IDirect3DDevice9 * d3ddev )
{
	unsigned int vert_size = NumVertices * VertexSize;
	unsigned int index_size = NumIndices * sizeof( unsigned int );
	void * vert_buffer;

	d3ddev->CreateVertexBuffer(
		vert_size,					// buffer size
		D3DUSAGE_WRITEONLY,			// rw status
		0,							// FVF format
		D3DPOOL_MANAGED,			// memory management style
		&VertexBuffer,				// buffer pointer
		NULL );						// shared handle

	VertexBuffer->Lock( 0, vert_size, &vert_buffer, 0 );
	memcpy( vert_buffer, VertexBufferData, vert_size );
	VertexBuffer->Unlock();

	d3ddev->CreateVertexDeclaration( VertexDescription::Desc[ VertexType ].Declartion, &VertexDeclaration );
	
	d3ddev->CreateIndexBuffer( 
		index_size,				// size of index buffer
		D3DUSAGE_WRITEONLY,		// rw status
		D3DFMT_INDEX32,			// index buffer data format
		D3DPOOL_MANAGED,		// memory management style
		&IndexBuffer,			// buffer pointer
		NULL );					// shared handle

	IndexBuffer->Lock( 0, index_size, &vert_buffer, 0 );
	memcpy( vert_buffer, IndexBufferData, index_size );
	IndexBuffer->Unlock();
}

/*
mesh format:
mesh_header { mesh tag, mesh chunk size } { 2 * uint }
	vertex type { 0, 1 } { uint }
	
	number of indices	{ uint }
	index data			{ uint * }
	
	number of verts		{ uint }
	vert data			{ uint * }
*/
void Mesh::LoadFromFile( ChunkReader & file )
{
	//read past the header of the mesh section
	file.ReadChunkHeader();

	//vertex type info
	file.Read( VertexType );
	VertexSize = VertexDescription::Desc[ VertexType ].SizeOfVertex;

	//index buffer
	file.Read( NumIndices );
	IndexBufferData = new unsigned int[ NumIndices ];
	file.ReadArray( IndexBufferData , NumIndices );

	//vertex buffer
	file.Read( NumVertices );
	VertexBufferData = new unsigned char[ NumVertices * VertexSize ];
	file.ReadArraySize( VertexBufferData , NumVertices * VertexSize );

	//calculate primative count
	NumPrimitives = NumIndices/3;
}

void Mesh::Render( LPDIRECT3DDEVICE9 dxdevice )
{
	dxdevice->SetVertexDeclaration( VertexDeclaration );
	//DXDevice->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );
	dxdevice->SetStreamSource( 0, VertexBuffer, 0, VertexSize );
	dxdevice->SetIndices( IndexBuffer );
	dxdevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, NumVertices, 0, NumPrimitives );
}