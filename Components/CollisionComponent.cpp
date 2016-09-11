#include "CollisionComponent.h"

CollisionComponent::CollisionComponent()
	: BaseComponent()
{
}

void CollisionComponent::Initialize(unsigned int const& entityId, float const& radius, bool const& canHit, bool const& canBeHit)
{
	_entityId = entityId;
	_radius = radius;
	_canHit = canHit;
	_canBeHit = canBeHit;

	Activate();
}