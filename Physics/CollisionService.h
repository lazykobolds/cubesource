#pragma once

#include "../Game Objects/GameEntity.h"

struct CollisionResult
{
	bool Collided;
	float Result[3];
	
	void Initilize()
	{
		Collided = false;
		Result[0] = Result[1] = Result[2] = 0.0f;
	}
};

void ProcessCollisions(GameEntity ** entities, const unsigned int queueSize);
void SphereSphereTest(GameEntity * entityA, GameEntity * entityB, CollisionResult & result);
