#pragma once

#include <d3dx9.h>
#include <stdio.h>

void SimplexLine( D3DXVECTOR3 * supports, unsigned int & num_supports, D3DXVECTOR3 & search_direction );
void SimplexTriangle( D3DXVECTOR3 * supports, unsigned int & num_supports, D3DXVECTOR3 & search_direction );
void SimplexTetrahedron( D3DXVECTOR3 * supports, unsigned int & num_supports, D3DXVECTOR3 & search_direction );

void GetSupport( D3DXVECTOR3 * verts, unsigned int num_verts, D3DXVECTOR3 & direction, D3DXVECTOR3 & out_support );

bool GJKCollide( D3DXVECTOR3 * verts0, unsigned int num_verts0, D3DXVECTOR3 * verts1, unsigned int num_verts1 );