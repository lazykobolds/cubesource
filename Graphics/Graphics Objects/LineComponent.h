#pragma once

#include "GraphicsComponent.h"

class LineComponent : public GraphicsComponent
{
public:
	LineComponent( void );
	LineComponent( const LineComponent * comp );

	virtual void Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity );

	LPDIRECT3DVERTEXBUFFER9 Verts;	
	unsigned int NumVerts;

protected:
	virtual void SetObjectSpecificRenderStates( const GameEntity * entity );

};