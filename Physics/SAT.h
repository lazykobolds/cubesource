#pragma once

#include <d3dx9.h>

bool SATCollideFrustum( 
	D3DXPLANE * planes0, D3DXPLANE * planes1,
	D3DXVECTOR3 * edges0, D3DXVECTOR3 * edges1,
	D3DXVECTOR3 * edge_points0, D3DXVECTOR3 * edge_points1,
	D3DXVECTOR3 * points0, D3DXVECTOR3 * points1 );