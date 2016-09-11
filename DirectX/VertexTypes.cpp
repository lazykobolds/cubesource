///////////////////////////////////////////////////////////////////////////////////////
//
//	VertexTypes.cpp
//	Authors: Chris Peters
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include <stddef.h>

#include "VertexTypes.h"

const D3DVERTEXELEMENT9 ModelVertex::Decl[] =
{
{ 0, offsetof(ModelVertex,pos),	D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, offsetof(ModelVertex,norm),D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL  , 0 },
{ 0, offsetof(ModelVertex,tex),	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
D3DDECL_END()
};
//const D3DVERTEXELEMENT9 ModelVertex::Decl[] =
//{
//{ 0, offsetof(ModelVertex,pos),	D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//{ 0, offsetof(ModelVertex,norm),D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL  , 0 },
//{ 0, offsetof(ModelVertex,tex),	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
//D3DDECL_END()
//};

const D3DVERTEXELEMENT9 SkinnedModelVertex::Decl[] =
{
{ 0, offsetof(SkinnedModelVertex,pos), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, offsetof(SkinnedModelVertex,norm),	D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL  , 0 },
{ 0, offsetof(SkinnedModelVertex,tex), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
{ 0, offsetof(SkinnedModelVertex,weight), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
{ 0, offsetof(SkinnedModelVertex,bone),	D3DDECLTYPE_UBYTE4 , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
D3DDECL_END()
};

const VertexDescription VertexDescription::Desc[] =
{
	{ VERTEX_TYPE_DEFAULT, sizeof(ModelVertex) , ModelVertex::Decl },
	{ VERTEX_TYPE_SKIN, sizeof(SkinnedModelVertex) , SkinnedModelVertex::Decl },
};