#pragma once
#define D3D_DEBUG_INFO

#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <list>

#include "Camera.h"
#include "../FBX Models/Model.h"
#include "Shader.h"
#include "../Game Objects/GameEntity.h"
#include "../Systems/TerrainSystem.h"

typedef void(*RenderFcn)( const LPDIRECT3DDEVICE9, const LPD3DXEFFECT, Camera*, GameEntity *, LPD3DXMATRIX );

struct TransformationNode
{
	TransformationNode( D3DXMATRIX * m ) { transform = m; }
	D3DXMATRIX * transform;
	std::list< GameEntity* > EntityList;
};

struct RenderNode
{
	RenderFcn Render;
	std::map< D3DXMATRIX*, std::list< GameEntity* > > Transforms;
};

#define RenderQueueType std::multimap< unsigned long long, GameEntity* >
#define RenderQueuePair std::pair<unsigned long long, GameEntity *>

class GraphicsEngine
{
public:
	GraphicsEngine( void );
	~GraphicsEngine( void );

	void Initilize( HWND hwnd );
	void RenderScene( void );
	
	void BeginFrame( void );

	void RegisterShader( LPD3DXEFFECT shader, RenderFcn func );
	void QueueObject( GameEntity * entity );
	void SetTransformation( D3DXMATRIX * matrix );
	void SetIdenityTransformation( void );

	void SetTerrainSystem(TerrainSystem * terrainSystem);

	Camera Camera;
	LPD3DXEFFECT shader;
	LPDIRECT3DDEVICE9 d3ddev;
	LPDIRECT3D9 d3d;

private:
	void InitilizeDirectX( HWND hwnd );
	void SetRenderStates( void );

	void RenderTerrain(void);
	
	unsigned int ScreenWidth;
	unsigned int ScreenHeight;
	
	LPDIRECT3DVERTEXBUFFER9 sphere;
	
	D3DXMATRIX IdentityMatrix;
	D3DXMATRIX * CurrentTransformation;

	std::map< LPD3DXEFFECT, RenderFcn > RenderFunctions;

	/*
		Rendering sort order (unsigned long x64 bit)
		Fullscreen Layer | ViewPort | ViewPort Layer | Translucency Type | Depth | Material ID
		------ 2 -------   --- 3 --   ------ 3 -----   ------ 2 --------   -24 -   --- 30 ----
	*/
	std::multimap< unsigned long long, GameEntity* > RenderQueue;

	TerrainSystem * _terrainSystem;

	LPD3DXEFFECT _basicShader;
};