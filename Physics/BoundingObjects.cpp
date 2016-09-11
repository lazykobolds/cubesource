#include "BoundingObjects.h"

BoundingSphere::BoundingSphere(	const float radius )
	: IBoundingObject( BoundingObjectTypes::Sphere ) 
{
	Radius = radius;
}


BoundingAABB::BoundingAABB(	const float ex, const float ey, const float ez )
							: IBoundingObject( BoundingObjectTypes::AABB )
{
	Extents[0] = ex;
	Extents[1] = ey;
	Extents[2] = ez;
}