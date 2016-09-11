#pragma once

#include <d3d9.h>
#include <d3dx9.h>

struct Triangle
{
	D3DXVECTOR3 points[3];

	Triangle( const D3DXVECTOR3 & p1, const D3DXVECTOR3 & p2, const D3DXVECTOR3 & p3 )
	{
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
	}
};

bool IntersectPointSphere( const D3DXVECTOR3 & point, const D3DXVECTOR3 & center, const float radius, D3DXVECTOR3 * displacement )
{
	D3DXVECTOR3 diff = center - point;
	float mag = diff.x*diff.x+diff.y*diff.y+diff.z*diff.z;
	
	if( mag > radius * radius ) return false;

	mag = sqrtf( mag );

	*displacement = ( radius - mag ) * diff / mag;

	return true;
}
	
bool IntersectSphereSphere( const D3DXVECTOR3 & center1, const float radius1, const D3DXVECTOR3 & center2, const float radius2, D3DXVECTOR3 * displacement )
{
	D3DXVECTOR3 diff = center2-center1;
	float mag = diff.x*diff.x+diff.y*diff.y+diff.z*diff.z;
	float combined_radius = radius1 + radius2;
	
	if( mag > combined_radius * combined_radius ) return false;

	mag = sqrtf( mag );

	*displacement = ( combined_radius - mag ) * diff / mag;

	return true;
}

bool IntersectSpherePlane( const D3DXVECTOR3 & center, const float radius, const D3DXPLANE & plane, D3DXVECTOR3 * displacement )
{
	float result = center.x*plane.a+center.y*plane.b+center.z*plane.c + plane.d;
	float dist = result - radius;
	if( dist > 0.f ) return false;
	
	displacement->x = plane.a * dist;
	displacement->y = plane.b * dist;
	displacement->z = plane.c * dist;

	return true;
}

bool IntersectSphereAABB( const D3DXVECTOR3 & center, const float radius, const D3DXVECTOR3 & lbound, const D3DXVECTOR3 &ubound, D3DXVECTOR3 * displacement )
{
	float xdist = center.x - lbound.x;
	float ydist = center.y - lbound.y;
	float zdist = center.z - lbound.z;
	unsigned int beforex, beforey, beforez;
	if( center.x < lbound.x )
	{
		if( center.x + radius < lbound.x ) return false;
		beforex = 0;
	}else if( center.x > ubound.x )
	{
		if( center.x - radius > ubound.x ) return false;
		beforex = 2;
	}else
	{
		beforey = 1;
	}
	if( center.y < lbound.y )
	{
		if( center.y + radius < lbound.y ) return false;
		beforex = 0;
	}else if( center.x > ubound.x )
	{
		if( center.y - radius > ubound.y ) return false;
		beforey = 2;
	}else
	{
		beforey = 1;
	}
	if( center.z < lbound.z )
	{
		if( center.z + radius < lbound.z ) return false;
		beforez = 0;
	}else if( center.z > ubound.z )
	{
		if( center.z - radius > ubound.z ) return false;
		beforez = 2;
	}else
	{
		beforez = 1;
	}
	
	if( beforex == 0 )
	{
		if( beforey == 0 )
		{
			if( beforez == 0 )
			{
				return IntersectPointSphere( lbound, center, radius, displacement );
			}else if( beforez == 1 )
			{
				float dx = center.x - lbound.x;
				float dy = center.y - lbound.y;
				float mag = dx*dx+dy*dy;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->y = nmag * dy / mag;
				displacement->z = 0.f;
				return true;
			}else
			{
				return IntersectPointSphere( D3DXVECTOR3( lbound.x, lbound.y, ubound.z ), center, radius, displacement );
			}
		}else if( beforey == 1 )
		{
			if( beforez == 0 )
			{
				//xz z-negative
				float dx = center.x - lbound.x;
				float dz = center.z - lbound.z;
				float mag = dx*dx+dz*dz;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->z = nmag * dz / mag;
				displacement->y = 0.f;
				return true;
			}else if( beforez == 1 )
			{
				//x negative
				displacement->x = center.x - lbound.x + radius;
				displacement->y = 0.f;
				displacement->z = 0.f;
				return true;
			}else
			{
				//xz z-positive
				float dx = center.x - lbound.x;
				float dz = center.z - ubound.z;
				float mag = dx*dx+dz*dz;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->z = nmag * dz / mag;
				displacement->y = 0.f;
				return true;
			}
		}else
		{
			if( beforez == 0 )
			{
				return IntersectPointSphere( D3DXVECTOR3( lbound.x, ubound.y, lbound.z ), center, radius, displacement );
			}else if( beforez == 1 )
			{
				//xy y-positive
				float dx = center.x - lbound.x;
				float dy = center.z - ubound.y;
				float mag = dx*dx+dy*dy;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->y = nmag * dy / mag;
				displacement->z = 0.f;
				return true;
			}else
			{
				return IntersectPointSphere( D3DXVECTOR3( lbound.x, ubound.y, ubound.z ), center, radius, displacement );
			}
		}
	}else if( beforex == 1 )
	{
		if( beforey == 0 )
		{
			if( beforez == 0 )
			{
				float dz = center.z - lbound.z;
				float dy = center.y - lbound.y;
				float mag = dz*dz+dy*dy;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->z = nmag * dz / mag;
				displacement->y = nmag * dy / mag;
				displacement->x = 0.f;
				return true;
			}else if( beforez == 1 )
			{
				//x negative
				displacement->x = 0.f;
				displacement->y = center.y - lbound.x + radius;
				displacement->z = 0.f;
				return true;
			}else
			{
				float dz = center.z - ubound.z;
				float dy = center.y - lbound.y;
				float mag = dz*dz+dy*dy;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->z = nmag * dz / mag;
				displacement->y = nmag * dy / mag;
				displacement->x = 0.f;
				return true;
			}
		}else if( beforey == 1 )
		{
			if( beforez == 0 )
			{
				//z negative
				displacement->x = 0.f;
				displacement->y = 0.f;
				displacement->z = center.z - lbound.x + radius;
				return true;
			}else if( beforez == 1 )
			{
				return false;
			}else
			{
				//z positive
				displacement->x = 0.f;
				displacement->y = 0.f;
				displacement->z = center.z - ubound.x - radius;
				return true;
			}
		}else
		{
			if( beforez == 0 )
			{
				//xz z-negative
				float dy = center.y - ubound.y;
				float dz = center.z - lbound.z;
				float mag = dy*dy+dz*dz;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = 0.f;
				displacement->z = nmag * dz / mag;
				displacement->y = nmag * dy / mag;
				return true;
			}else if( beforez == 1 )
			{
				//x negative
				displacement->x = 0.f;
				displacement->y = center.x - ubound.x - radius;
				displacement->z = 0.f;
				return true;
			}else
			{
				//xz z-negative
				float dy = center.y - ubound.y;
				float dz = center.z - ubound.z;
				float mag = dy*dy+dz*dz;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = 0.f;
				displacement->z = nmag * dz / mag;
				displacement->y = nmag * dy / mag;
				return true;
			}
		}		
	}else
	{
		if( beforey == 0 )
		{
			if( beforez == 0 )
			{
				return IntersectPointSphere( D3DXVECTOR3( ubound.x, lbound.y, lbound.z ), center, radius, displacement );
			}else if( beforez == 1 )
			{
				float dx = center.x - ubound.x;
				float dy = center.y - lbound.y;
				float mag = dx*dx+dy*dy;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->y = nmag * dy / mag;
				displacement->z = 0.f;
				return true;
			}else
			{
				return IntersectPointSphere( D3DXVECTOR3( ubound.x, lbound.y, ubound.z ), center, radius, displacement );
			}
		}else if( beforey == 1 )
		{
			if( beforez == 0 )
			{
				//xz z-negative
				float dx = center.x - ubound.x;
				float dz = center.z - lbound.z;
				float mag = dx*dx+dz*dz;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->z = nmag * dz / mag;
				displacement->y = 0.f;
				return true;
			}else if( beforez == 1 )
			{
				//x negative
				displacement->x = center.x - ubound.x - radius;
				displacement->y = 0.f;
				displacement->z = 0.f;
				return true;
			}else
			{
				//xz z-positive
				float dx = center.x - ubound.x;
				float dz = center.z - ubound.z;
				float mag = dx*dx+dz*dz;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->z = nmag * dz / mag;
				displacement->y = 0.f;
				return true;
			}
		}else
		{
			if( beforez == 0 )
			{
				return IntersectPointSphere( D3DXVECTOR3( ubound.x, ubound.y, lbound.z ), center, radius, displacement );
			}else if( beforez == 1 )
			{
				//xy y-positive
				float dx = center.x - ubound.x;
				float dy = center.z - ubound.y;
				float mag = dx*dx+dy*dy;
				if( mag > radius * radius ) return false;
				float nmag = mag-radius;
				mag = sqrtf( mag );
				displacement->x = nmag * dx / mag;
				displacement->y = nmag * dy / mag;
				displacement->z = 0.f;
				return true;
			}else
			{
				return IntersectPointSphere( ubound, center, radius, displacement );
			}
		}
	}
}

bool IntersectSphereOBB( 
	const D3DXVECTOR3 & scenter, const float radius, 
	const D3DXVECTOR3 & bcenter, const D3DXVECTOR3 & extents, 
	const D3DXMATRIX & matOBB, const D3DXMATRIX & matInvOBB,
	D3DXVECTOR3 * displacement )
{
	D3DXVECTOR3 diff = scenter - bcenter;
	D3DXVec3TransformNormal( &diff, &diff, &matInvOBB );

	bool result = IntersectSphereAABB( diff, radius, bcenter - extents, bcenter + extents, displacement );
	if( result )
	{
		D3DXVec3TransformNormal( displacement, displacement, &matOBB );
		return true;
	}
	return false;
}

bool IntersectRayPlane( const D3DXVECTOR3 & origin, const D3DXVECTOR3 & direction, const D3DXPLANE & plane, D3DXVECTOR3 * intersection, float * dist )
{
	float dot = direction.x * plane.a + direction.y * plane.b + direction.z * plane.c;
	if( dot > -0.00001f && dot < 0.00001f ) return false;
	dot *= -1.f;
	float numerator = origin.x * plane.a + origin.y * plane.b + origin.z * plane.c + plane.d;
	float u = numerator / dot;
	if( u < 0.f ) return false;

	*intersection = origin + direction * u;
	if( dist ) *dist = u;
	return true;
}

bool IntersectRayTriangle( const D3DXVECTOR3 & origin, const D3DXVECTOR3 & direction, const Triangle & triangle, D3DXVECTOR3 * intersection, float * dist )
{
	D3DXVECTOR3 v1 = triangle.points[1] - triangle.points[0];
	D3DXVECTOR3 v2 = triangle.points[2] - triangle.points[0];
	D3DXVECTOR3 norm;
	D3DXVec3Cross( &norm, &v1, &v2 );
	D3DXPLANE plane( norm.x, norm.y, norm.z, -D3DXVec3Dot( &norm, &triangle.points[0] ) );
	bool ptest = IntersectRayPlane( origin, direction, plane, intersection, dist );
	if( !ptest ) return false;

	D3DXVECTOR3 norm1, norm2, norm3;
	D3DXVECTOR3 v3 = triangle.points[1] - triangle.points[2];
	D3DXVec3Cross( &norm1, &norm, &(-v1) );
	if( D3DXVec3Dot( &norm1, intersection ) > 0.f ) return false;
	D3DXVec3Cross( &norm2, &norm, &v2 );
	if( D3DXVec3Dot( &norm2, intersection ) > 0.f ) return false;
	D3DXVec3Cross( &norm3, &norm, &v3 );
	if( D3DXVec3Dot( &norm3, intersection ) > 0.f ) return false;

	return true;
}