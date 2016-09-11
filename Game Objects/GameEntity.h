#pragma once

#include <d3dx9.h>

#include "..\Graphics\Graphics Objects\GraphicsComponent.h"
#include "..\Physics\BoundingObjects.h"
#include "..\Utilities\Coordinate.h"
#include "..\DirectX\VertexTypes.h"

enum EntityType
{
	None		= 0x00000000,
	Terrain		= 0x00000001,
	Entity		= 0x00000002,
	Projectile	= 0x00000004,
	All			= 0xFFFFFFFF
};

class GameEntity
{
public:
	GameEntity( void );
	
	D3DXVECTOR3 Position;
	D3DXVECTOR3 PreviousPosition;
	D3DXMATRIX Rotation;
	D3DXVECTOR3 Velocity;
	bool Colored;

	GraphicsComponent * GraphicsComponent;
	LPD3DXEFFECT Shader;
	IBoundingObject * BoundingObject;

	bool Active;
	bool Gravity;
	bool Moves;
	bool Collidable;
	unsigned int CollidableFlags;
	unsigned int TypeFlags;
};