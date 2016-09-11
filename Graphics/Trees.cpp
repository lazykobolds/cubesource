#include "Trees.h"
#include "..\DirectX\VertexTypes.h"
#include "..\Utilities\LSystem.h"
#include "Primatives.h"

#include <math.h>
#include <stdio.h>

/*
Tree Symbols:

' - Increase Width
F - Move Forward
[ - Push Stack
] - Pop Stack
X - rot x
x - rot -x
Y - rot y
y - rot -y
Z - rot z
z - rot -z

L - leaf node


*/

D3DXMATRIX R_X(		1.f, 0.f, 0.f, 0.f,
					0.f, 0.9659258262890682867497431997289f, 0.25881904510252076234889883762405f, 0.f,
					0.f, -0.25881904510252076234889883762405f, 0.9659258262890682867497431997289f, 0.f,
					0.f, 0.f, 0.f, 1.f );

D3DXMATRIX R_nX(	1.f, 0.f, 0.f, 0.f,
					0.f, 0.9659258262890682867497431997289f, -0.25881904510252076234889883762405f, 0.f,
					0.f, 0.25881904510252076234889883762405f, 0.9659258262890682867497431997289f, 0.f,
					0.f, 0.f, 0.f, 1.f );

D3DXMATRIX R_Y(		0.9659258262890682867497431997289f, 0.f, 0.25881904510252076234889883762405f, 0.f,
					0.f, 1.f, 0.f, 0.f,
					-0.25881904510252076234889883762405f, 0.f, 0.9659258262890682867497431997289f, 0.f,
					0.f, 0.f, 0.f, 1.f );

D3DXMATRIX R_nY(	0.9659258262890682867497431997289f, 0.f, -0.25881904510252076234889883762405f, 0.f,
					0.f, 1.f, 0.f, 0.f,
					0.25881904510252076234889883762405f, 0.f, 0.9659258262890682867497431997289f, 0.f,
					0.f, 0.f, 0.f, 1.f );

D3DXMATRIX R_Z(		0.9659258262890682867497431997289f, -0.25881904510252076234889883762405f, 0.f, 0.f,
					0.25881904510252076234889883762405f, 0.9659258262890682867497431997289f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f );

D3DXMATRIX R_nZ(	0.9659258262890682867497431997289f, 0.25881904510252076234889883762405f, 0.f, 0.f,
					-0.25881904510252076234889883762405f, 0.9659258262890682867497431997289f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f );

void ProcessTree( Vertex * verts, unsigned int & vert_index, D3DXVECTOR3 origin, D3DXMATRIX matrix, char * sentence, unsigned int & s_index, float radius, unsigned int f_depth )
{
	float c,c2;
	float s,s2;
	float rad, new_radius;
	Vertex bot[8];
	Vertex top[8];
	
	while( sentence[s_index] )
	{
		switch( sentence[s_index] )
		{
		case 'F':
			++s_index;
			rad = 0.f;
			new_radius = radius - 0.03f;
			if( new_radius < 0.05f ) new_radius = 0.05f;
			for( unsigned int i = 0; i < 8; ++i )
			{
				c = cosf( rad );
				c2 = c * new_radius;
				c *= radius;
				s = sinf( rad );
				s2 = s * new_radius;
				s *= radius;
				bot[i].nx = matrix._11 * c - matrix._31 * s;
				bot[i].ny = matrix._12 * c - matrix._32 * s;
				bot[i].nz = matrix._13 * c - matrix._33 * s;
				bot[i].x = bot[i].nx + origin.x;
				bot[i].y = bot[i].ny + origin.y;
				bot[i].z = bot[i].nz + origin.z;

				top[i].nx = matrix._11 * c2 - matrix._31 * s2;
				top[i].ny = matrix._12 * c2 - matrix._32 * s2;
				top[i].nz = matrix._13 * c2 - matrix._33 * s2;
				top[i].x = top[i].nx + matrix._21 + origin.x;
				top[i].y = top[i].ny + matrix._22 + origin.y;
				top[i].z = top[i].nz + matrix._23 + origin.z;
				rad += 0.78539816339744830961566084581988f;
			}

			for( unsigned int i = 0; i < 7; ++i )
			{
				verts[vert_index++] = bot[i];
				verts[vert_index++] = top[i+1];
				verts[vert_index++] = bot[i+1];
				verts[vert_index++] = top[i+1];
				verts[vert_index++] = bot[i];
				verts[vert_index++] = top[i];
			}
			verts[vert_index++] = bot[7];
			verts[vert_index++] = top[0];
			verts[vert_index++] = bot[0];
			verts[vert_index++] = top[0];
			verts[vert_index++] = bot[7];
			verts[vert_index++] = top[7];
			origin.x += matrix._21;
			origin.y += matrix._22;
			origin.z += matrix._23;
			radius = new_radius;
			break;
		case '`':
			++s_index;
			radius += 0.01f;
			break;
		case 'X':
			++s_index;
			D3DXMatrixMultiply( &matrix, &matrix, &R_X );
			break;
		case 'x':
			++s_index;
			D3DXMatrixMultiply( &matrix, &matrix, &R_nX );
			break;
		case 'Y':
			++s_index;
			matrix = R_Y * matrix;
			break;
			D3DXMatrixMultiply( &matrix, &matrix, &R_Y );
			//D3DXMatrixMultiply( &matrix, &R_Y, &matrix );
			break;
		case 'y':
			++s_index;
			matrix = R_nY * matrix;
			break;
			D3DXMatrixMultiply( &matrix, &matrix, &R_nY );
			//D3DXMatrixMultiply( &matrix, &R_nY, &matrix );
			break;
		case 'Z':
			++s_index;
			matrix = R_Z * matrix;
			break;
			D3DXMatrixMultiply( &matrix, &matrix, &R_Z );
			//D3DXMatrixMultiply( &matrix, &R_Z, &matrix );
			break;
		case 'z':
			++s_index;
			matrix = R_nZ * matrix;
			break;
			D3DXMatrixMultiply( &matrix, &matrix, &R_nZ );
			//D3DXMatrixMultiply( &matrix, &R_nZ, &matrix );
			break;
		case '[':
			++s_index;
			ProcessTree( verts, vert_index, origin, matrix, sentence, s_index, radius, f_depth+1 );
			break;
		case ']':
			++s_index;
			return;
			break;
		case 'l':
			{
				++s_index;
				//if( f_depth < 1 ) 
				{
				Vertex * sphere_verts;
				unsigned int svcount = CreateGeoSphereList( &sphere_verts, 2, 4.f, origin );
				for( unsigned int i = 0; i < svcount; ++i )
				{
					verts[vert_index++] = sphere_verts[i];
				}
				printf("sphereing\n");
				delete sphere_verts;
				}
				break;
			}
		default:
			++s_index;
			break;
		}
	}
}

LPDIRECT3DVERTEXBUFFER9 CreateTree( LPDIRECT3DDEVICE9 dxdevice, float radius, unsigned int & vcount )
{
	LSystem lsys;
	strcpy_s(lsys.Sentence[lsys.ActiveSentence], sizeof("gFFFFFFl"), "gFFFFFFl");
	lsys.AddProcedure( 'l', "l", 1.f );
	//lsys.AddProcedure( 'l', "[l]l", .05f );
	//lsys.AddProcedure( 'l', "FFl", 0.13571428571428571428571428571429f );
	//lsys.AddProcedure( 'l', "zl", 0.13571428571428571428571428571429f );
	//lsys.AddProcedure( 'l', "yl", 0.13571428571428571428571428571429f );
	//lsys.AddProcedure( 'l', "xl", 0.13571428571428571428571428571429f );
	//lsys.AddProcedure( 'l', "zFl", 0.13571428571428571428571428571429f );
	//lsys.AddProcedure( 'l', "yFl", 0.13571428571428571428571428571429f );
	//lsys.AddProcedure( 'l', "xFl", 0.13571428571428571428571428571429f );
	////lsys.AddProcedure( 'l', "FFl", .6f );
	////lsys.AddProcedure( 'l', "zFl", 0.125f );
	////lsys.AddProcedure( 'l', "ZFl", 0.125f );
	////lsys.AddProcedure( 'l', "[ZZFFFFl][YYYYYYZZFFFFl][YYYYYYYYYYZZFFFFl][YYYYYYYYYYYYYYZZFFFFl]Fl", 0.15f );
	lsys.AddProcedure( 'g', "`g", 1.f );
	lsys.AddProcedure( 'F', "F", 1.f );
	lsys.AddProcedure( '[', "[", 1.f );
	lsys.AddProcedure( ']', "]", 1.f );
	lsys.AddProcedure( 'y', "y", 1.f );
	lsys.AddProcedure( 'Y', "Y", 1.f );
	lsys.AddProcedure( 'z', "z", 1.f );
	lsys.AddProcedure( 'Z', "Z", 1.f );
	lsys.AddProcedure( 'x', "x", 1.f );
	lsys.AddProcedure( 'X', "X", 1.f );
	lsys.AddProcedure( '`', "`", 1.f );
	for( unsigned int i = 0; i < 10; ++i )
	{
		lsys.GenerateString();
	}
	
	printf( "%s\n",lsys.Sentence[lsys.ActiveSentence] );

	unsigned int chunks = 0;
	unsigned int leaves = 0;
	unsigned int i = 0;
	while( lsys.Sentence[lsys.ActiveSentence][i] != 0 )
	{
		if( lsys.Sentence[lsys.ActiveSentence][i] == 'F' ) ++chunks;
		if( lsys.Sentence[lsys.ActiveSentence][i] == 'l' ) ++leaves;
		++i;
	}


	Vertex * verts = new Vertex[ chunks * 6 * 8 + leaves * 24 * 4 * 4 ];
	D3DXMATRIX ident;
	D3DXMatrixIdentity( &ident );
	unsigned int vert_index = 0;
	i = 0;
	ProcessTree( verts, vert_index, D3DXVECTOR3( 0.f, 0.f, 0.f), ident, lsys.Sentence[lsys.ActiveSentence], i, 0.5f, 0 );
	if( vert_index > (4096<<1) ) (vert_index=4096<<1);
	vcount = vert_index;

	Vertex * vert_data = 0;
	LPDIRECT3DVERTEXBUFFER9 vert_buffer = 0;
	
	if( FAILED(dxdevice->CreateVertexBuffer( 
		vert_index * sizeof( Vertex ), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Vertex::VertexFVF,
		D3DPOOL_DEFAULT,
		&vert_buffer,
		0 ) ) )
	{
		printf("CreateVertexBuffer failed.\n%s\n%i\n",__FILE__,__LINE__);
	}

	HRESULT err = vert_buffer->Lock( 0, vert_index * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );
	unsigned int c = 0;
	DWORD color = D3DCOLOR_XRGB((rand()%80)+10,(rand()%155)+100,(rand()%80)+10);
	for( unsigned int i = 0; i < vert_index; ++i )
	{
		//vert_data[i].x = verts[i].x;
		//vert_data[i].y = verts[i].y;
		//vert_data[i].z = verts[i].z;
		//vert_data[i].nx = verts[i].nx;
		//vert_data[i].ny = verts[i].ny;
		//vert_data[i].nz = verts[i].nz;
		vert_data[i] = verts[i];
		//vert_data[i].u = verts[i]
		//vert_data[i].v = ( verts[tris[i]].z + 0.70710678118654752440084436210485f ) / 1.4142135623730950488016887242097f;
		vert_data[i].color = color;
		++c;
		if( c == 3 ) { color = D3DCOLOR_XRGB((rand()%80)+10,(rand()%155)+100,(rand()%80)+10); c = 0; }
		//vert_data[i].color = D3DCOLOR_XRGB((rand()%80)+10,(rand()%155)+100,(rand()%80)+10);
		//vert_data[i].color = D3DCOLOR_XRGB( 10, 150, 10 );
	}

	delete verts;

	vert_buffer->Unlock();

	return vert_buffer;
}