#include "SAT.h"
#include <stdio.h>


bool SATCollideFrustum( 
	D3DXPLANE * planes0, D3DXPLANE * planes1,
	D3DXVECTOR3 * edges0, D3DXVECTOR3 * edges1,
	D3DXVECTOR3 * edge_points0, D3DXVECTOR3 * edge_points1,
	D3DXVECTOR3 * points0, D3DXVECTOR3 * points1 )
{
	float min[2];
	float max[2];
	float res;

	for( unsigned int plane = 0; plane < 6; ++plane )
	{
		min[0] = min[1] = 9000001.f;
		max[0] = max[1] = -9000001.f;
		
		res = planes0[plane].a * points0[0].x + planes0[plane].b * points0[0].y + planes0[plane].c * points0[0].z;// + planes0[plane].d;
		min[0] = max[0] = res;
		res = planes0[plane].a * points1[0].x + planes0[plane].b * points1[0].y + planes0[plane].c * points1[0].z;// + planes0[plane].d;
		min[1] = max[1] = res;
		for( unsigned int p = 1; p < 8; ++p )
		{
			res = planes0[plane].a * points0[p].x + planes0[plane].b * points0[p].y + planes0[plane].c * points0[p].z;// + planes0[plane].d;
			if( res < min[0] ) 
			{
				min[0] = res;
			}
			else if( res > max[0] ) 
			{
				max[0] = res;
			}
			
			res = planes0[plane].a * points1[p].x + planes0[plane].b * points1[p].y + planes0[plane].c * points1[p].z;// + planes0[plane].d;
			if( res < min[1] ) 
			{
				min[1] = res;
			}
			else if( res > max[1] )
			{
				max[1] = res;
			}
		}

		if( min[0] > max[1] || min[1] > max[0] ) 
		{
			if( min[0] > 9000.f || min[1] > 9000.f || max[0] < -9000.f || max[1] < -9000.f )
				printf("%f %f %f %f\n", min[0], min[1], max[0], max[1] );
			return false;
		}
		
		min[0] = min[1] = 9000001.f;
		max[0] = max[1] = -9000001.f;
		res = planes1[plane].a * points0[0].x + planes1[plane].b * points0[0].y + planes1[plane].c * points0[0].z;// + planes1[plane].d;
		min[0] = max[0] = res;
		res = planes1[plane].a * points1[0].x + planes1[plane].b * points1[0].y + planes1[plane].c * points1[0].z;// + planes1[plane].d;
		min[1] = max[1] = res;
		for( unsigned int p = 0; p < 8; ++p )
		{
			res = planes1[plane].a * points0[p].x + planes1[plane].b * points0[p].y + planes1[plane].c * points0[p].z;// + planes1[plane].d;
			if( res < min[0] ) min[0] = res;
			else if( res > max[0] ) max[0] = res;
			
			res = planes1[plane].a * points1[p].x + planes1[plane].b * points1[p].y + planes1[plane].c * points1[p].z;// + planes1[plane].d;
			if( res < min[1] ) min[1] = res;
			else if( res > max[1] ) max[1] = res;
		}

		if( min[0] > max[1] || min[1] > max[0] ) return false;
	}
	return true;

	D3DXVECTOR3 cross;
	float dval;

	for( unsigned int edge0 = 0; edge0 < 12; ++edge0 )
	{
		for( unsigned int edge1 = 0; edge1 < 12; ++edge1 )
		{
			min[0] = min[1] = 9000001.f;
			max[0] = max[1] = -9000001.f;

			D3DXVec3Cross( &cross, &edges0[edge0], &edges1[edge1] );
			dval = cross.x * edge_points0[edge0].x + cross.z * edge_points0[edge0].y + cross.z * edge_points0[edge0].z;
			
			for( unsigned int p = 0; p < 8; ++p )
			{
				res = D3DXVec3Dot( &cross, &points0[p] );
				if( res < min[0] ) res = min[0];
				else if( res > max[0] ) res = max[0];
				
				res = D3DXVec3Dot( &cross, &points1[p] );
				if( res < min[1] ) res = min[0];
				else if( res > max[1] ) res = max[1];
			}

			if( min[0] > max[1] || min[1] > max[0] ) return false;
		}
	}

	return true;
}
