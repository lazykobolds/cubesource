#pragma once

#include "IBoundingObject.h"

class BoundingSphere : public IBoundingObject
{
public:
	float Radius;

	BoundingSphere( const float radius );
};

class BoundingAABB : public IBoundingObject
{
public:
	float Extents[3];

	BoundingAABB( const float ex, const float ey, const float ez );
};