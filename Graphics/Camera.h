#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
public:
	Camera( void );

	void CalculateMatricies( void );
	void CalculateViewMatrix( void );
	void CalculateProjectionMatrix( void );
	void CalculateFrustumPlanes( void );
	void CalculateFrustumPoints( void );// D3DXVECTOR3 & pos, D3DXMATRIX & mat );
	void CalculateFrustumEdges( void );

	bool IntersectRect( float xmin, float xmax, float zmin, float zmax );

	D3DXVECTOR3 Position;
	
	float NearViewPlane;
	float FarViewPlane;

	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
	D3DXMATRIX ViewProjMatrix;

	D3DXMATRIX WorldMatrix;
	float xrot;

	/* unused ? */
	D3DXMATRIX RelativeViewMatrix;


	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 dir;

	D3DXVECTOR3 FrustumPoints[8];

	/* unused */
	D3DXVECTOR3 EdgePoints[12];
	D3DXVECTOR3 FrustumEdges[12];
	
	//near - far - bottom - top - left - right
	D3DXPLANE FrustumPlanes[6];

private:
};