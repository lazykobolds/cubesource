#pragma once

#include <d3dx9.h>

#include "..\DirectX\VertexTypes.h"

unsigned int GenerateTerrain( 
	int sx, int sy, int sz, 
	unsigned int width, unsigned int height, unsigned int depth,
	unsigned int seed, char * voxel_data );

LPDIRECT3DVERTEXBUFFER9 BuildMesh( 
	int sx, int sy, int sz, 
	unsigned int width, unsigned int height, unsigned int depth,
	char * voxel_data, unsigned int num_tris,
	const LPDIRECT3DDEVICE9 dxdevice );

unsigned int GenerateTerrainWithPadding( 
	int sx, int sy, int sz, 
	unsigned int width, unsigned int height, unsigned int depth,
	unsigned int seed, char * voxel_data );

LPDIRECT3DVERTEXBUFFER9 BuildMeshWithPadding( 
	int sx, int sy, int sz, 
	unsigned int width, unsigned int height, unsigned int depth,
	char * voxel_data, unsigned int num_tris,
	const LPDIRECT3DDEVICE9 dxdevice );

LPDIRECT3DVERTEXBUFFER9 OptimizeVoxelTriangles( 
	int sx, int sy, int sz,
	unsigned int width, unsigned int height, unsigned int depth,
	char * voxel_data, const LPDIRECT3DDEVICE9 dxdevice, unsigned int & num_triangles );