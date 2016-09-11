#pragma once

#include <d3dx9.h>
#include "DirectX\VertexTypes.h"
#include "Utilities\MathLib.h"
#include "VoronoiDiagramGenerator.h"
#include "Utilities\MathFunctions.h"

#define MAX_POINTS 1024
#define MAX_TRIANGLES 1024

struct VTRI
{
	DWORD color;
	D3DXVECTOR3 site;
	D3DXVECTOR3 * points;
	unsigned int num_points;

	VTRI( void ) 
	{
		color = 0;
		num_points = 0;
		points = new D3DXVECTOR3[ MAX_POINTS ];
	}

	~VTRI( void )
	{
		delete [] points;
	}

	void AddPoint( D3DXVECTOR3 & point );
};

class NoiseCombiner : public IMathFunction
{
public:
	NoiseCombiner( float * d0, float * d1 ) { mix_data[0] = d0; mix_data[1] = d1; }

	float operator()( float input0, float input1 )
	{
		return 0;
	}

	float operator() ( unsigned int x, unsigned int y, unsigned int pass )
	{
		return mix_data[pass%2][x*512+y];
	}

	float * mix_data[2];
};

void ExpandHull( D3DXVECTOR3 * hull, unsigned int &num_points, D3DXVECTOR3 & new_point );

class Voronoi
{
public:
	Voronoi( void );
	void ComputeVD( LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 * points, unsigned int num_points );
	void ComputeVDSub( LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 * points, unsigned int num_points );
	unsigned int GetClosestPoint( D3DXVECTOR3 & point, D3DXVECTOR3 * points, unsigned int num_points );

	unsigned int GetClosestRegion( D3DXVECTOR3 & point, unsigned int exclude_region );
	bool IsClosestPoint( D3DXVECTOR3 & point, D3DXVECTOR3 & target, D3DXVECTOR3 * points, unsigned int num_points );
	bool PointInList( D3DXVECTOR3 & point, D3DXVECTOR3 * points, unsigned int num_points );

	float * GenerateDifferenceTexture( 
		D3DXVECTOR3 & world_min, D3DXVECTOR3 & world_max, 
		unsigned int tex_width, unsigned int tex_height, 
		IMathFunction * noise_function = 0, bool normalized = true );

	float * GenerateBinaryTexture(
		D3DXVECTOR3 & world_min, D3DXVECTOR3 & world_max, 
		unsigned int tex_width, unsigned int tex_height, 
		IMathFunction * noise_function = 0 );

	LPDIRECT3DVERTEXBUFFER9 vert_buffer;
	unsigned int num_verts;

	VTRI VSites[MAX_POINTS];
	unsigned int num_sites;
	VoronoiDiagramGenerator generator;
};
