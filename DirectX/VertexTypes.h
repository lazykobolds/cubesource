#pragma once

#include <d3d9.h>
#include <d3dx9.h>

enum VertexType
{
	VERTEX_TYPE_DEFAULT = 0,
	VERTEX_TYPE_SKIN
};

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	DWORD color;
	float u, v;
	enum FLAG { VertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
};

struct ModelVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;
	const static D3DVERTEXELEMENT9 Decl[6];
};

struct SkinnedModelVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;
	float weight[4];
	byte bone[4];
	const static D3DVERTEXELEMENT9 Decl[6];
};


struct VertexDescription
{
	unsigned int VertexType;
	unsigned int SizeOfVertex;
	const D3DVERTEXELEMENT9 * Declartion;
	const static VertexDescription Desc[2];
};