//#pragma once
//
//#include "GraphicsComponent.h"
//#include "../../noise.h"
//#include "../../Voronoi.h"
//
//class NoiseComponent : public GraphicsComponent
//{
//public:
//	NoiseComponent( void );
//	NoiseComponent( const NoiseComponent * comp );
//
//	virtual void Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity );
//
//	LPDIRECT3DVERTEXBUFFER9 Verts;	
//	unsigned int NumTriangles;
//
//	Noise noise;
//	Voronoi voronoi;
//
//protected:
//	virtual void SetObjectSpecificRenderStates( const GameEntity * entity );
//
//};