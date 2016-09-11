#pragma once

#include <d3dx9.h>

D3DXVECTOR3 GravityPoint( D3DXVECTOR3 & point, D3DXVECTOR3 & source );
D3DXVECTOR3 GravityLineSegment( D3DXVECTOR3 & point, D3DXVECTOR3 & line_start, D3DXVECTOR3 & line_end );