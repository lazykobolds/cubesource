#include "GravityTests.h"

D3DXVECTOR3 GravityPoint( D3DXVECTOR3 & point, D3DXVECTOR3 & source )
{
	D3DXVECTOR3 vec( source - point );
	D3DXVec3Normalize( &vec, &vec );
	return vec;
}

D3DXVECTOR3 GravityLineSegment( D3DXVECTOR3 & point, D3DXVECTOR3 & line_start, D3DXVECTOR3 & line_end )
{
	D3DXVECTOR3 dir = line_end - line_start;
	D3DXVECTOR3 point_start = point - line_start;
	float t = D3DXVec3Dot( &dir, &point_start ) / D3DXVec3Dot( &dir, &dir );
	if( t < 0.f )
	{
		D3DXVec3Normalize( &point_start, &point_start );
		return -point_start;		
	}
	if( t > 1.f )
	{
		point_start = point - line_end;
		D3DXVec3Normalize( &point_start, &point_start );
		return -point_start;
	}
	dir = line_start + dir * t;
	dir = dir - point;
	D3DXVec3Normalize( &dir, &dir );
	return dir;
}