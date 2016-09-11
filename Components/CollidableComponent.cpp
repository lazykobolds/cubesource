#include "CollidableComponent.h"

CollidableComponent::CollidableComponent()
	: BaseComponent()
{
}

void CollidableComponent::Initialize(float radius)
{
	_radius = radius;
	Activate();
}