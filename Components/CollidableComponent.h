#include "BaseComponent.h"

class CollidableComponent : public BaseComponent
{
public:
	CollidableComponent();

	void Initialize(float radius);

	float _radius;
};