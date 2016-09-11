#include "GameEntity.h"


GameEntity::GameEntity( void )
{
	//BoundingObject = 0;
	GraphicsComponent = 0;
	Active = false;
	Gravity = false;
	Velocity *= 0;
	Position *= 0;
	PreviousPosition *= 0;
	Moves = false;
	Collidable = false;
	Shader = 0;
}