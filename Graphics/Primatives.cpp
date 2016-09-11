#include "Primatives.h"
#include "..\DirectX\VertexTypes.h"
#include "..\Utilities\MathLib.h"

#include <stdio.h>

LPDIRECT3DVERTEXBUFFER9 CreateGeoSphere( LPDIRECT3DDEVICE9 dxdevice, unsigned int tesselations, float radius )
{
	unsigned int tri_num = 8 * (unsigned int)pow( 4.0, (int)tesselations );
	unsigned int * tris = new unsigned int[ tri_num * 3 ];
	D3DXVECTOR3 * verts = new D3DXVECTOR3[ tri_num * 3 ];
	unsigned int tri_count = 0;
	unsigned int vert_count = 0;

	verts[0] = D3DXVECTOR3( 0.f, 1.f, 0.f );
	verts[1] = D3DXVECTOR3( -1.f, 0.f, 0.f );
	verts[2] = D3DXVECTOR3( 0.f, 0.f, -1.f );
	verts[3] = D3DXVECTOR3( 1.f, 0.f, 0.f );
	verts[4] = D3DXVECTOR3( 0.f, 0.f, 1.f );
	verts[5] = D3DXVECTOR3( 0.f, -1.f, 0.f );
	vert_count = 6;
	
	tris[0] = 0;
	tris[1] = 1;
	tris[2] = 2;
	tris[3] = 0;
	tris[4] = 2;
	tris[5] = 3;
	tris[6] = 0;
	tris[7] = 3;
	tris[8] = 4;
	tris[9] = 0;
	tris[10] = 4;
	tris[11] = 1;
	tris[12] = 5;
	tris[13] = 1;
	tris[14] = 4;
	tris[15] = 5;
	tris[16] = 2;
	tris[17] = 1;
	tris[18] = 5;
	tris[19] = 3;
	tris[20] = 2;
	tris[21] = 5;
	tris[22] = 4;
	tris[23] = 3;
	tri_count = 8;
	
	for( unsigned int i = 0; i < tesselations; ++i )
	{
		for( unsigned int j = 0; j < tri_count; ++j )
		{
			unsigned int tri_index[6];
			tri_index[0] = tris[j*3];
			tri_index[2] = tris[j*3+1];
			tri_index[4] = tris[j*3+2];
			tri_index[1] = vert_count;
			tri_index[3] = vert_count+1;
			tri_index[5] = vert_count+2;
			
			verts[vert_count] = verts[tris[j*3]]*0.5+verts[tris[j*3+1]]*0.5;
			verts[vert_count+1] = verts[tris[j*3+1]]*0.5+verts[tris[j*3+2]]*0.5;
			verts[vert_count+2] = verts[tris[j*3]]*0.5+verts[tris[j*3+2]]*0.5;
			
			D3DXVec3Normalize( &verts[vert_count], &verts[vert_count] );
			D3DXVec3Normalize( &verts[vert_count+1], &verts[vert_count+1] );
			D3DXVec3Normalize( &verts[vert_count+2], &verts[vert_count+2] );
			vert_count += 3;

			tris[j*3] = tri_index[1];
			tris[j*3+1] = tri_index[3];
			tris[j*3+2] = tri_index[5];
			
			tris[tri_count*3+j*9] = tri_index[0];
			tris[tri_count*3+j*9+1] = tri_index[1];
			tris[tri_count*3+j*9+2] = tri_index[5];

			tris[tri_count*3+j*9+3] = tri_index[2];
			tris[tri_count*3+j*9+4] = tri_index[3];
			tris[tri_count*3+j*9+5] = tri_index[1];

			tris[tri_count*3+j*9+6] = tri_index[4];
			tris[tri_count*3+j*9+7] = tri_index[5];
			tris[tri_count*3+j*9+8] = tri_index[3];
		}
		tri_count *= 4;
	}

	Vertex * vert_data = 0;
	LPDIRECT3DVERTEXBUFFER9 vert_buffer = 0;
	
	dxdevice->CreateVertexBuffer( 
		tri_count * 3 * sizeof( Vertex ), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Vertex::VertexFVF,
		D3DPOOL_DEFAULT,
		&vert_buffer,
		0 );

	HRESULT err = vert_buffer->Lock( 0, tri_count * 3 * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );

	for( unsigned int i = 0; i < tri_count*3; ++i )
	{
		vert_data[i].x = verts[tris[i]].x * radius;
		vert_data[i].y = verts[tris[i]].y * radius;
		vert_data[i].z = verts[tris[i]].z * radius;
		vert_data[i].nx = verts[tris[i]].x;
		vert_data[i].ny = verts[tris[i]].y;
		vert_data[i].nz = verts[tris[i]].z;
		vert_data[i].u = ( verts[tris[i]].x + 0.70710678118654752440084436210485f ) / 1.4142135623730950488016887242097f;
		vert_data[i].v = ( verts[tris[i]].z + 0.70710678118654752440084436210485f ) / 1.4142135623730950488016887242097f;
		vert_data[i].color = D3DCOLOR_XRGB( 100, 100, 10 );
	}

	delete tris;
	delete verts;

	vert_buffer->Unlock();

	return vert_buffer;
}

unsigned int CreateGeoSphereList( Vertex ** verts_out, unsigned int tesselations, float radius, D3DXVECTOR3 & origin )
{
	unsigned int tri_num = 8 * (unsigned int)pow( 4.0, (int)tesselations );
	unsigned int * tris = new unsigned int[ tri_num * 3 ];
	D3DXVECTOR3 * verts = new D3DXVECTOR3[ tri_num * 3 ];
	unsigned int tri_count = 0;
	unsigned int vert_count = 0;

	verts[0] = D3DXVECTOR3( 0.f, 1.f, 0.f );
	verts[1] = D3DXVECTOR3( -1.f, 0.f, 0.f );
	verts[2] = D3DXVECTOR3( 0.f, 0.f, -1.f );
	verts[3] = D3DXVECTOR3( 1.f, 0.f, 0.f );
	verts[4] = D3DXVECTOR3( 0.f, 0.f, 1.f );
	verts[5] = D3DXVECTOR3( 0.f, -1.f, 0.f );
	vert_count = 6;
	
	tris[0] = 0;
	tris[1] = 1;
	tris[2] = 2;
	tris[3] = 0;
	tris[4] = 2;
	tris[5] = 3;
	tris[6] = 0;
	tris[7] = 3;
	tris[8] = 4;
	tris[9] = 0;
	tris[10] = 4;
	tris[11] = 1;
	tris[12] = 5;
	tris[13] = 1;
	tris[14] = 4;
	tris[15] = 5;
	tris[16] = 2;
	tris[17] = 1;
	tris[18] = 5;
	tris[19] = 3;
	tris[20] = 2;
	tris[21] = 5;
	tris[22] = 4;
	tris[23] = 3;
	tri_count = 8;
	
	for( unsigned int i = 0; i < tesselations; ++i )
	{
		for( unsigned int j = 0; j < tri_count; ++j )
		{
			unsigned int tri_index[6];
			tri_index[0] = tris[j*3];
			tri_index[2] = tris[j*3+1];
			tri_index[4] = tris[j*3+2];
			tri_index[1] = vert_count;
			tri_index[3] = vert_count+1;
			tri_index[5] = vert_count+2;

			verts[vert_count] = verts[tris[j*3]]*0.5+verts[tris[j*3+1]]*0.5;
			verts[vert_count+1] = verts[tris[j*3+1]]*0.5+verts[tris[j*3+2]]*0.5;
			verts[vert_count+2] = verts[tris[j*3]]*0.5+verts[tris[j*3+2]]*0.5;
			
			D3DXVec3Normalize( &verts[vert_count], &verts[vert_count] );
			D3DXVec3Normalize( &verts[vert_count+1], &verts[vert_count+1] );
			D3DXVec3Normalize( &verts[vert_count+2], &verts[vert_count+2] );
			vert_count += 3;

			tris[j*3] = tri_index[1];
			tris[j*3+1] = tri_index[3];
			tris[j*3+2] = tri_index[5];
			
			tris[tri_count*3+j*9] = tri_index[0];
			tris[tri_count*3+j*9+1] = tri_index[1];
			tris[tri_count*3+j*9+2] = tri_index[5];

			tris[tri_count*3+j*9+3] = tri_index[2];
			tris[tri_count*3+j*9+4] = tri_index[3];
			tris[tri_count*3+j*9+5] = tri_index[1];

			tris[tri_count*3+j*9+6] = tri_index[4];
			tris[tri_count*3+j*9+7] = tri_index[5];
			tris[tri_count*3+j*9+8] = tri_index[3];
		}
		tri_count *= 4;
	}

	*verts_out = new Vertex[tri_count * 3];

	for( unsigned int i = 0; i < tri_count*3; ++i )
	{
		verts_out[0][i].x = verts[tris[i]].x * radius + origin.x;;
		verts_out[0][i].y = verts[tris[i]].y * radius + origin.y;
		verts_out[0][i].z = verts[tris[i]].z * radius + origin.z;
		verts_out[0][i].nx = verts[tris[i]].x;
		verts_out[0][i].ny = verts[tris[i]].y;
		verts_out[0][i].nz = verts[tris[i]].z;
		verts_out[0][i].u = ( verts[tris[i]].x + 0.70710678118654752440084436210485f ) / 1.4142135623730950488016887242097f;
		verts_out[0][i].v = ( verts[tris[i]].z + 0.70710678118654752440084436210485f ) / 1.4142135623730950488016887242097f;
		verts_out[0][i].color = D3DCOLOR_XRGB((rand()%80)+10,(rand()%155)+100,(rand()%80)+10);
	}
	delete tris;
	delete verts;

	return tri_count*3;
}

void AddRectangle( Vertex * vert_data, D3DXVECTOR3 & p0, D3DXVECTOR3 & p1, D3DXVECTOR3 & p2, D3DXVECTOR3 & p3 )
{
	vert_data[0].x = p0.x;
	vert_data[0].y = p0.y;
	vert_data[0].z = p0.z;

	vert_data[1].x = p1.x;
	vert_data[1].y = p1.y;
	vert_data[1].z = p1.z;

	vert_data[2].x = p3.x;
	vert_data[2].y = p3.y;
	vert_data[2].z = p3.z;

	vert_data[3].x = p1.x;
	vert_data[3].y = p1.y;
	vert_data[3].z = p1.z;

	vert_data[4].x = p2.x;
	vert_data[4].y = p2.y;
	vert_data[4].z = p2.z;

	vert_data[5].x = p3.x;
	vert_data[5].y = p3.y;
	vert_data[5].z = p3.z;
}
