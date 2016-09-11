#pragma once

#include "GraphicsComponent.h"
#include "..\..\FBX Models\Model.h"

class AnimatedModelComponent : public GraphicsComponent
{
public:
	virtual void Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity );

protected:
	virtual void SetObjectSpecificRenderStates( const GameEntity * entity );

	Model Model;
};