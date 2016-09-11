#pragma once

enum BoundingObjectTypes
{
	Sphere			= 0x00000001,
	AABB			= 0x00000002,
	Frustum			= 0x00000008,
	Sphere_AABB		= Sphere | AABB,
	Sphere_Frustum	= Sphere | Frustum,
	AABB_Frustum	= AABB | Frustum
};

class IBoundingObject
{
public:
	IBoundingObject(BoundingObjectTypes type) : Type(type) {}

	const BoundingObjectTypes Type;
};