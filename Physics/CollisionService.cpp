#include "CollisionService.h"

void ProcessCollisions(GameEntity ** entities, const unsigned int queueSize)
{
	GameEntity * current;
	GameEntity * target;
	CollisionResult ResultData;
	for (unsigned int i = 0; i < queueSize; ++i)
	{
		ResultData.Initilize();
		current = entities[i];
		for (unsigned int ti = i + 1; ti < queueSize; ++ti)
		{
			target = entities[ti];
			if ((current->CollidableFlags & target->TypeFlags) > 0)
			{
				switch (current->BoundingObject->Type | target->BoundingObject->Type)
				{
				case BoundingObjectTypes::AABB_Frustum:
					break;
				case BoundingObjectTypes::Sphere_AABB:
					break;
				case BoundingObjectTypes::Sphere_Frustum:
					break;
				case BoundingObjectTypes::Sphere:

					break;
				default:
					break;
				}
			}
		}
	}
}

void SphereSphereTest(GameEntity * entityA, GameEntity * entityB, CollisionResult & result)
{
	BoundingSphere * sphere1 = (BoundingSphere*)entityA->BoundingObject;
	BoundingSphere * sphere2 = (BoundingSphere*)entityB->BoundingObject;
	float diff[3] = {	entityA->Position[0] - entityB->Position[0],
						entityA->Position[1] - entityB->Position[1],
						entityA->Position[2] - entityB->Position[2] };

	float radius = sphere1->Radius + sphere2->Radius;
	float radius_sqr = radius * radius;
	float dist = (diff[0] * diff[0]) + (diff[1] * diff[1]) + (diff[2] * diff[2]);

	if (radius_sqr < dist) 
	{ 
		result.Collided = false; 
	}
	else
	{
		dist = sqrtf(dist);
		float mag = radius - dist;

		result.Result[0] = mag * diff[0] / dist;
		result.Result[1] = mag * diff[1] / dist;
		result.Result[2] = mag * diff[2] / dist;
		result.Collided = true;
	}
}

/*

CollisionInfo BoxBoxTest( D3DXVECTOR3 & pos1, BoundingBox * box1, D3DXVECTOR3 & pos2, BoundingBox * box2 )
{
	float xdiff, ydiff, zdiff;
	bool bx, by, bz;
	
	if( pos1.x < pos2.x ) 
	{
		bx = true;
		xdiff = pos1.x + box1->Radius[0] - pos2.x - box2->Radius[0];
	}
	else 
	{	
		bx = false;
		xdiff = pos2.x + box2->Radius[0] - pos1.x - box1->Radius[0];
	}
	if( xdiff <= 0.f ) return CollisionInfo();

	if( pos1.y < pos2.y ) 
	{ 
		by = true;
		ydiff = pos1.y + box1->Radius[1] - pos2.y - box2->Radius[1];
	}
	else  
	{
		by = false;
		ydiff = pos2.y + box2->Radius[1] - pos1.y - box1->Radius[1];
	}
	if( ydiff <= 0.f ) return CollisionInfo();

	if( pos1.z < pos2.x ) 
	{
		bz = true;
		zdiff = pos1.z + box1->Radius[2] - pos2.z - box2->Radius[2];
	}
	else  
	{
		bz = false;
		zdiff = pos2.x + box2->Radius[2] - pos1.z - box1->Radius[2];
	}
	if( zdiff <= 0.f ) return CollisionInfo();

	if( xdiff < ydiff )
	{
		if( xdiff < zdiff )
		{
			return CollisionInfo( D3DXVECTOR3( bx?1.f:-1.f,0.f,0.f ), xdiff );
		}else
		{
			return CollisionInfo( D3DXVECTOR3( 0.f,0.f,bz?1.f:-1.f ), zdiff );
		}
	}else
	{
		if( ydiff < zdiff )
		{
			return CollisionInfo( D3DXVECTOR3( 0.f,by?1.f:-1.f,0.f ), ydiff );
		}else
		{
			return CollisionInfo( D3DXVECTOR3( 0.f,0.f,bz?1.f:-1.f ), zdiff );
		}
	}	
}


CollisionInfo DetectCollision( GameEntity * object1, GameEntity * object2 )
{
	assert( object1->BoundingObject && object2->BoundingObject && "Objects do not have bounding objects!" );

	CollisionInfo cinfo;

	switch( object1->BoundingObject->Shape )
	{
	case BO_SPHERE:
		switch( object2->BoundingObject->Shape )
		{
		case BO_SPHERE:
			cinfo = SphereSphereTest( object1->Position, (BoundingSphere*)object1->BoundingObject, object2->Position, (BoundingSphere*)object2->BoundingObject );
			return cinfo;
			break;
		}
		break;
	}

	return CollisionInfo();
}*/