#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "..\DirectX\VertexTypes.h"

LPDIRECT3DVERTEXBUFFER9 CreateGeoSphere(LPDIRECT3DDEVICE9 dxdevice, unsigned int tesselations, float radius);
//just create and return list of vertex points
unsigned int CreateGeoSphereList( Vertex ** verts, unsigned int tesselations, float radius, D3DXVECTOR3 & origin );
