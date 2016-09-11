#include "../Config/ConfigData.h"
#include "../Components/CollidableComponent.h"
#include "../Components/CollisionComponent.h"

class CollisionSystem
{
public:
	CollisionSystem();

	void AddCollisionComponent(unsigned int const& entityId, float const& radius, bool const& canBeHit, bool const& canHit);

	void RemoveCollisionComponent(unsigned int entityId);

private:
	CollisionComponent * _collisionComponents;

	unsigned int _unusedCollisionComponentIndex;
};