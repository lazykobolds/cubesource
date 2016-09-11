#include "Camera.h"

Camera::Camera( void )
{
	NearViewPlane = 1.f;
	FarViewPlane = 1000.f;
	D3DXMatrixIdentity( &WorldMatrix );
	CalculateProjectionMatrix();
	xrot = 0;
}

void Camera::CalculateViewMatrix( void )
{
	//D3DXMatrixLookAtLH(
	//				&ViewMatrix,
	//				&Position,							//eye
	//				&( Position + AdjustedDirection ),			//target
	//				&AdjustedUp );	//up vector

	//d3ddev->SetTransform( D3DTS_VIEW, &ViewMatrix );	// set the view transform to matView
}

void Camera::CalculateProjectionMatrix( void )
{
	D3DXMatrixPerspectiveFovLH(
							&ProjectionMatrix,
							D3DXToRadian(45.f),   // the horizontal field of view
							800.f / 600.f,		// aspect ratio
							NearViewPlane,		// the near view-plane
							FarViewPlane );		// the far view-plane

	//d3ddev->SetTransform( D3DTS_PROJECTION, &ProjectionMatrix );    // set the projection
}

void Camera::CalculateFrustumPoints( void )//D3DXVECTOR3 & pos, D3DXMATRIX & mat )
{
	//D3DXMATRIX tview;
	//D3DXMatrixTranspose( &tview, &ViewMatrix );
	float half_near_width = (float)tan( D3DXToRadian(45.f) / 2.f ) * NearViewPlane * 4.f / 3.f; // NearViewPlane
	float half_near_height = half_near_width * 3.f / 4.f;
	float half_far_width = (float)tan( D3DXToRadian(45.f) / 2.f ) * FarViewPlane * 4.f / 3.f;// FarViewPlane;
	float half_far_height = half_far_width * 3.f / 4.f;
	
	right = D3DXVECTOR3( ViewMatrix._11, ViewMatrix._12, ViewMatrix._13 );
	up = D3DXVECTOR3( ViewMatrix._21, ViewMatrix._22, ViewMatrix._23 );
	dir = D3DXVECTOR3( ViewMatrix._31, ViewMatrix._32, ViewMatrix._33 );

	/* point of the camera */
	D3DXVECTOR3 pos2 = right * ViewMatrix._41 + up * ViewMatrix._42 + dir * ViewMatrix._43;
	pos2 *= -1.f;
	//D3DXVECTOR3 pos2( ViewMatrix._41, ViewMatrix._42, ViewMatrix._43 );
	//pos2 *= -1.f;
	
	D3DXVECTOR3 near_point = dir * NearViewPlane + pos2;
	D3DXVECTOR3 far_point = dir * FarViewPlane + pos2;

	FrustumPoints[0] = near_point - right * half_near_width - up * half_near_height;
	FrustumPoints[1] = near_point - right * half_near_width + up * half_near_height;
	FrustumPoints[2] = near_point + right * half_near_width - up * half_near_height;
	FrustumPoints[3] = near_point + right * half_near_width + up * half_near_height;
	FrustumPoints[4] = far_point - right * half_far_width - up * half_far_height;
	FrustumPoints[5] = far_point - right * half_far_width + up * half_far_height;
	FrustumPoints[6] = far_point + right * half_far_width - up * half_far_height;
	FrustumPoints[7] = far_point + right * half_far_width + up * half_far_height;
}

void Camera::CalculateFrustumEdges( void )
{
	FrustumEdges[0] = FrustumPoints[1] - FrustumPoints[0];
	//EdgePoints[0] = FrustumPoints[0];
	FrustumEdges[1] = FrustumPoints[2] - FrustumPoints[0];
	//EdgePoints[1] = FrustumPoints[0];
	FrustumEdges[2] = FrustumPoints[3] - FrustumPoints[2];
	//EdgePoints[2] = FrustumPoints[2];
	FrustumEdges[3] = FrustumPoints[1] - FrustumPoints[3];
	//EdgePoints[3] = FrustumPoints[1];

	FrustumEdges[4] = FrustumPoints[5] - FrustumPoints[4];
	//EdgePoints[4] = FrustumPoints[4];
	FrustumEdges[5] = FrustumPoints[6] - FrustumPoints[4];
	//EdgePoints[5] = FrustumPoints[4];
	FrustumEdges[6] = FrustumPoints[7] - FrustumPoints[6];
	//EdgePoints[6] = FrustumPoints[7];
	FrustumEdges[7] = FrustumPoints[5] - FrustumPoints[7];
	//EdgePoints[7] = FrustumPoints[7];
	
	FrustumEdges[8] = FrustumPoints[4] - FrustumPoints[0];
	//EdgePoints[8] = FrustumPoints[0];
	FrustumEdges[9] = FrustumPoints[5] - FrustumPoints[1];
	//EdgePoints[9] = FrustumPoints[1];
	FrustumEdges[10] = FrustumPoints[6] - FrustumPoints[2];
	//EdgePoints[10] = FrustumPoints[2];
	FrustumEdges[11] = FrustumPoints[7] - FrustumPoints[3];
	//EdgePoints[11] = FrustumPoints[3];
}

void Camera::CalculateFrustumPlanes( void )
{
	D3DXVECTOR3 cross;
	//front 
	D3DXVec3Cross( &cross, &FrustumEdges[0], &FrustumEdges[1] );
	FrustumPlanes[0].a = cross.x;
	FrustumPlanes[0].b = cross.y;
	FrustumPlanes[0].c = cross.z;
	FrustumPlanes[0].d = -cross.x * FrustumPoints[0].x - cross.y * FrustumPoints[0].y - cross.z * FrustumPoints[0].z;
	//back
	D3DXVec3Cross( &cross, &FrustumEdges[4], &FrustumEdges[5] );
	FrustumPlanes[1].a = cross.x;
	FrustumPlanes[1].b = cross.y;
	FrustumPlanes[1].c = cross.z;
	FrustumPlanes[1].d = -cross.x * FrustumPoints[4].x - cross.y * FrustumPoints[4].y - cross.z * FrustumPoints[4].z;
	//left
	D3DXVec3Cross( &cross, &FrustumEdges[0], &FrustumEdges[8] );
	FrustumPlanes[2].a = cross.x;
	FrustumPlanes[2].b = cross.y;
	FrustumPlanes[2].c = cross.z;
	FrustumPlanes[2].d = -cross.x * FrustumPoints[0].x - cross.y * FrustumPoints[0].y - cross.z * FrustumPoints[0].z;
	//bottom
	D3DXVec3Cross( &cross, &FrustumEdges[1], &FrustumEdges[8] );
	FrustumPlanes[3].a = cross.x;
	FrustumPlanes[3].b = cross.y;
	FrustumPlanes[3].c = cross.z;
	FrustumPlanes[3].d = -cross.x * FrustumPoints[0].x - cross.y * FrustumPoints[0].y - cross.z * FrustumPoints[0].z;
	//right
	D3DXVec3Cross( &cross, &FrustumEdges[2], &FrustumEdges[11] );
	FrustumPlanes[4].a = cross.x;
	FrustumPlanes[4].b = cross.y;
	FrustumPlanes[4].c = cross.z;
	FrustumPlanes[4].d = -cross.x * FrustumPoints[2].x - cross.y * FrustumPoints[2].y - cross.z * FrustumPoints[2].z;
	//top
	D3DXVec3Cross( &cross, &FrustumEdges[3], &FrustumEdges[11] );
	FrustumPlanes[5].a = cross.x;
	FrustumPlanes[5].b = cross.y;
	FrustumPlanes[5].c = cross.z;
	FrustumPlanes[5].d = -cross.x * FrustumPoints[1].x - cross.y * FrustumPoints[1].y - cross.z * FrustumPoints[1].z;

}

void Camera::CalculateMatricies( void )
{
	//UpdateOrientationPitch();
	//CalculateViewMatrix();
	//CalculateProjectionMatrix();

	CalculateFrustumPoints();
	CalculateFrustumEdges();
	CalculateFrustumPlanes();


	ViewProjMatrix = ViewMatrix * ProjectionMatrix;
}

/* seperating axis theorem for a 2D "chunk" section against the view frustum */
bool Camera::IntersectRect( float xmin, float xmax, float zmin, float zmax )
{
	float min[2], max[2], val;
	bool ret;
	unsigned int i;

	D3DXVECTOR3 ChunkPoints[8] = 
	{
		D3DXVECTOR3( xmin, 0.f, zmin ),
		D3DXVECTOR3( xmin, 0.f, zmax ),
		D3DXVECTOR3( xmax, 0.f, zmin ),
		D3DXVECTOR3( xmax, 0.f, zmax ),
		D3DXVECTOR3( xmin, 256.f, zmin ),
		D3DXVECTOR3( xmin, 256.f, zmax ),
		D3DXVECTOR3( xmax, 256.f, zmin ),
		D3DXVECTOR3( xmax, 256.f, zmax ),
	};

	/* early-out test */
	/* test if frustum is completely outside x or z values of a chunk's "box" */
	ret = true; for( i = 0; i < 8; ++i ) if( FrustumPoints[i].x < xmax ) { ret = false; break; } if( ret ) return false;
	ret = true; for( i = 0; i < 8; ++i ) if( FrustumPoints[i].x > xmin ) { ret = false; break; } if( ret ) return false;
	ret = true; for( i = 0; i < 8; ++i ) if( FrustumPoints[i].z < zmax ) { ret = false; break; } if( ret ) return false;
	ret = true; for( i = 0; i < 8; ++i ) if( FrustumPoints[i].z > zmin ) { ret = false; break; } if( ret ) return false;

	/* seperating-axis theorem for the view frustum faces as planes, only */
	for( i = 1; i < 6; ++i )
	{
		min[0] = max[0] = FrustumPlanes[i].a * FrustumPoints[0].x + FrustumPlanes[i].b * FrustumPoints[0].y + FrustumPlanes[i].c * FrustumPoints[0].z;
		min[1] = max[1] = FrustumPlanes[i].a * ChunkPoints[0].x + FrustumPlanes[i].b * ChunkPoints[0].y + FrustumPlanes[i].c * ChunkPoints[0].z;
		for( unsigned int fp = 1; fp < 8; ++fp )
		{
			val = FrustumPlanes[i].a * FrustumPoints[fp].x + FrustumPlanes[i].b * FrustumPoints[fp].y + FrustumPlanes[i].c * FrustumPoints[fp].z;
			if( val < min[0] ) min[0] = val;
			else if( val > max[0] ) max[0] = val;

			val = FrustumPlanes[i].a * ChunkPoints[fp].x + FrustumPlanes[i].b * ChunkPoints[fp].y + FrustumPlanes[i].c * ChunkPoints[fp].z;
			if( val < min[1] ) min[1] = val;
			else if( val > max[1] ) max[1] = val;
		}

		if( (min[0] > max[1]) || (max[0] < min[1]) ) return false;
	}
	return true;
}
