#include "CollisionSystem.h"

#include <assert.h>

CollisionSystem::CollisionSystem()
{
	_unusedCollisionComponentIndex = 0;
}

void CollisionSystem::AddCollisionComponent(unsigned int const& entityId, float const& radius, bool const& canBeHit, bool const& canHit)
{
	_collisionComponents[_unusedCollisionComponentIndex].Initialize(entityId, radius, canBeHit, canHit);

	while (_collisionComponents[++_unusedCollisionComponentIndex]._active) { assert(_unusedCollisionComponentIndex < MAX_COMPONENTS - 1); }
}

void CollisionSystem::RemoveCollisionComponent(unsigned int entityId)
{
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i)
	{
		if (_collisionComponents[i]._entityId == entityId)
		{
			_collisionComponents[i].Deactivate();
			if (i < _unusedCollisionComponentIndex)
			{
				_unusedCollisionComponentIndex = i;
			}
			return;
		}
	}
}