#include "BaseComponent.h"

class CollisionComponent : public BaseComponent
{
public:
	CollisionComponent();

	void Initialize(unsigned int const& entityId, float const& radius, bool const& canHit, bool const& canBeHit);

	float _radius;
	bool _canHit;
	bool _canBeHit;
};