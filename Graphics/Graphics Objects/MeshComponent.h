#pragma once

#include "GraphicsComponent.h"

class MeshComponent : public GraphicsComponent
{
public:
	MeshComponent( void );
	MeshComponent( const MeshComponent * comp );

	virtual void Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity );

	LPDIRECT3DVERTEXBUFFER9 Vertices;	
	unsigned int NumTriangles;

protected:
	virtual void SetObjectSpecificRenderStates( const GameEntity * entity );

};