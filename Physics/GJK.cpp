#include "GJK.h"

#include <assert.h>

void GetSupport( D3DXVECTOR3 * verts, unsigned int num_verts, D3DXVECTOR3 & direction, D3DXVECTOR3 & out_support )
{
	float max_dist = verts[0].x * direction.x + verts[0].y * direction.y  + verts[0].z * direction.z;
	unsigned int max_support = 0;
	for( unsigned int i = 1; i < num_verts; ++i )
	{
		float dist = verts[i].x * direction.x + verts[i].y * direction.y  + verts[i].z * direction.z;
		if( dist > max_dist ) 
		{
			max_dist = dist;
			max_support = i;
		}
	}
	out_support = verts[max_support];
}

void SimplexLine( D3DXVECTOR3 * supports, unsigned int &num_supports, D3DXVECTOR3 & search_direction )
{
	D3DXVECTOR3 diff = supports[1]-supports[0];
	D3DXVec3Cross( &search_direction, &diff, &supports[0] );
	D3DXVec3Cross( &search_direction, &diff, &search_direction );
}

void SimplexTriangle( D3DXVECTOR3 * supports, unsigned int & num_supports, D3DXVECTOR3 & search_direction )
{
	D3DXVECTOR3 BC = supports[2]-supports[1];
	D3DXVECTOR3 AC = supports[2]-supports[0];
	D3DXVECTOR3 ABC;
	D3DXVec3Cross( &ABC, &AC, &BC );
	D3DXVECTOR3 ACnorm;
	D3DXVec3Cross( &ACnorm, &ABC, &AC );
	D3DXVECTOR3 BCnorm;
	D3DXVec3Cross( &BCnorm, &BC, &ABC );
	//ACnorm * -A
	float res;
	res = ACnorm.x * supports[0].x + ACnorm.y * supports[0].y + ACnorm.z * supports[0].z;
	if( res < -0.0001f )
	{
		//outside AC
		res = AC.x * supports[0].x + AC.y * supports[0].y + AC.z * supports[0].z;
		if( res < -0.0001f )
		{
			//region A
			num_supports = 1;
			search_direction = -supports[0];
			return;
		}else
		{
			//region AC
			D3DXVec3Cross( &search_direction, &supports[0], &AC );
			D3DXVec3Cross( &search_direction, &search_direction, &AC );
			supports[1] = supports[2];
			num_supports = 2;
			return;
		}
	}

	res = BCnorm.x * supports[1].x + BCnorm.y * supports[1].y + BCnorm.z * supports[1].z;
	if( res < -0.0001f )
	{
		//outside BC
		res = BC.x * supports[1].x + BC.y * supports[1].y + BC.z * supports[1].z;
		if( res < -0.0001f )
		{
			//region B
			num_supports = 1;
			search_direction = -supports[1];
			return;
		}else
		{
			//region BC
			D3DXVec3Cross( &search_direction, &BC, &supports[1] );
			D3DXVec3Cross( &search_direction, &BC, &search_direction );
			supports[0] = supports[1];
			supports[1] = supports[2];
			num_supports = 2;
			return;
		}
	}

	res = ABC.x * supports[0].x + ABC.y * supports[0].y + ABC.z * supports[0].z;
	if( res < -0.0001f )
	{
		search_direction = ABC;
		return;
	}else
	{
		search_direction = supports[0];
		supports[0] = supports[1];
		supports[1] = search_direction;
		search_direction = -ABC;
		return;
	}
}

void SimplexTetrahedron( D3DXVECTOR3 * supports, unsigned int & num_supports, D3DXVECTOR3 & search_direction )
{
	D3DXVECTOR3 ABD, BCD, CAD;
	D3DXVECTOR3 DA, DB, DC;
	DA = supports[0]-supports[3];
	DB = supports[1]-supports[3];
	DC = supports[2]-supports[3];

	D3DXVec3Cross( &ABD, &DA, &DB );
	D3DXVec3Cross( &BCD, &DB, &DC );
	D3DXVec3Cross( &CAD, &DC, &DA );

	float res;
	unsigned int outside = 0;

	res = ABD.x * supports[0].x + ABD.y * supports[0].y + ABD.z * supports[0].z;
	if( res < -0.001f ) outside = 1;
	res = CAD.x * supports[0].x + CAD.y * supports[0].y + CAD.z * supports[0].z;
	if( res < -0.001f ) outside |= 2;
	res = BCD.x * supports[1].x + BCD.y * supports[1].y + BCD.z * supports[1].z;
	if( res < -0.001f ) outside |= 4;

	switch( outside )
	{
	case 0:
		return;
	case 1:
		//outside ABD
		supports[2] = supports[3];
		num_supports = 3;
		SimplexTriangle( supports, num_supports, search_direction );
		return;
	case 2:
		//outside CAD
		supports[1] = supports[0];
		supports[0] = supports[2];
		supports[2] = supports[3];
		num_supports = 3;
		SimplexTriangle( supports, num_supports, search_direction );
		return;
	case 3:
	{
		//outside ABD and CAD
		D3DXVECTOR3 AD = supports[3] - supports[0];
		D3DXVECTOR3 AD_C, AD_B;
		D3DXVec3Cross( &AD_C, &AD, &CAD );
		D3DXVec3Cross( &AD_B, &ABD, &AD );
		res = AD_C.x * supports[0].x + AD_C.y * supports[0].y + AD_C.z * supports[0].z;
		outside = 0;
		if( res < 0.0001f ) outside = 1;
		res = AD_B.x * supports[0].x + AD_B.y * supports[0].y + AD_B.z * supports[0].z;
		if( res < 0.0001f ) outside |= 2;
		switch( outside )
		{
		case 0:
			assert( "ERROR! INVALID CASE!\n" && false );
			break;
		case 1:
			//triangle ABD
			supports[2] = supports[1];
			supports[1] = supports[0];
			supports[0] = supports[3];
		num_supports = 3;
			SimplexTriangle( supports, num_supports, search_direction );
			return;
		case 2:
			//triangle CAD
			supports[1] = supports[3];
		num_supports = 3;
			SimplexTriangle( supports, num_supports, search_direction );
			return;
		case 3:
			//outside AD edge testing
			res = AD.x * supports[3].x + AD.y * supports[3].y + AD.z * supports[3].z;
			if( res < 0.0001f )
			{
				//outside D
				supports[0] = supports[3];
				search_direction = -supports[3];
				num_supports = 1;
				return;
			}
			res = AD.x * supports[0].x + AD.y * supports[0].y + AD.z * supports[0].z;
			if( res > -0.0001f )
			{
				//outside A
				num_supports = 1;
				search_direction = -supports[0];
				return;
			}
			//actually outside AD edge
			D3DXVec3Cross( &search_direction, &supports[0], &AD );
			D3DXVec3Cross( &search_direction, &search_direction, &AD );
			supports[1] = supports[3];
			num_supports = 2;
			return;
		}
		return;
	}
	case 4:
		{
		//outside BCD
		supports[0] = supports[1];
		supports[1] = supports[2];
		supports[2] = supports[3];
		num_supports = 3;
		SimplexTriangle( supports, num_supports, search_direction );
		return;
		}
	case 5:
	{
		//outside ABD and BCD
		D3DXVECTOR3 BD = supports[3] - supports[1];
		D3DXVECTOR3 BD_C, BD_A;
		D3DXVec3Cross( &BD_C, &BCD, &BD );
		D3DXVec3Cross( &BD_A, &BD, &ABD );
		res = BD_C.x * supports[1].x + BD_C.y * supports[1].y + BD_C.z * supports[1].z;
		outside = 0;
		if( res < 0.0001f ) outside = 1;
		res = BD_A.x * supports[1].x + BD_A.y * supports[1].y + BD_A.z * supports[1].z;
		if( res < 0.0001f ) outside |= 2;
		switch( outside )
		{
		case 0:
			assert( "ERROR! INVALID CASE!\n" && false );
			break;
		case 1:
			//triangle ABD
			supports[2] = supports[0];
			supports[0] = supports[1];
			supports[1] = supports[3];
		num_supports = 3;
			SimplexTriangle( supports, num_supports, search_direction );
			return;
		case 2:
			//triangle BCD
			supports[0] = supports[3];
		num_supports = 3;
			SimplexTriangle( supports, num_supports, search_direction );
			return;
		case 3:
			//outside BD edge testing
			res = BD.x * supports[3].x + BD.y * supports[3].y + BD.z * supports[3].z;
			if( res < 0.0001f )
			{
				//outside D
				supports[0] = supports[3];
				search_direction = -supports[3];
				num_supports = 1;
				return;
			}
			res = BD.x * supports[1].x + BD.y * supports[1].y + BD.z * supports[1].z;
			if( res > -0.0001f )
			{
				//outside B
				num_supports = 1;
				supports[0] = supports[1];
				search_direction = -supports[1];
				return;
			}
			//actually outside BD edge
			D3DXVec3Cross( &search_direction, &supports[1], &BD );
			D3DXVec3Cross( &search_direction, &search_direction, &BD );
			supports[0] = supports[1];
			supports[1] = supports[3];
			num_supports = 2;
			return;
		}
		return;
	}
	case 6:
	{
		//outside CAD and BCD
		D3DXVECTOR3 CD = supports[3] - supports[2];
		D3DXVECTOR3 CD_B, CD_A;
		D3DXVec3Cross( &CD_B, &CD, &BCD );
		D3DXVec3Cross( &CD_A, &CAD, &CD );
		res = CD_B.x * supports[3].x + CD_B.y * supports[3].y + CD_B.z * supports[3].z;
		outside = 0;
		if( res < 0.0001f ) outside = 1;
		res = CD_A.x * supports[3].x + CD_A.y * supports[3].y + CD_A.z * supports[3].z;
		if( res < 0.0001f ) outside |= 2;
		switch( outside )
		{
		case 0:
			assert( "ERROR! INVALID CASE!\n" && false );
			break;
		case 1:
			//triangle CDB
			supports[0] = supports[2];
			supports[2] = supports[1];
			supports[1] = supports[3];
		num_supports = 3;
			SimplexTriangle( supports, num_supports, search_direction );
			return;
		case 2:
			//triangle CDA
			supports[1] = supports[2];
			supports[2] = supports[0];
			supports[0] = supports[3];
		num_supports = 3;
			SimplexTriangle( supports, num_supports, search_direction );
			return;
		case 3:
			//outside CD edge testing
			res = CD.x * supports[3].x + CD.y * supports[3].y + CD.z * supports[3].z;
			if( res < 0.0001f )
			{
				//outside D
				supports[0] = supports[3];
				search_direction = -supports[3];
				num_supports = 1;
				return;
			}
			res = CD.x * supports[2].x + CD.y * supports[2].y + CD.z * supports[2].z;
			if( res > -0.0001f )
			{
				//outside C
				num_supports = 1;
				supports[0] = supports[2];
				search_direction = -supports[2];
				return;
			}
			//actually outside CD edge
			D3DXVec3Cross( &search_direction, &supports[3], &CD );
			D3DXVec3Cross( &search_direction, &search_direction, &CD );
			supports[0] = supports[2];
			supports[1] = supports[3];
			num_supports = 2;
			return;
		}
		return;
	}
	case 7:
		//outside D
		supports[0] = supports[3];
		search_direction = -supports[3];
		num_supports = 1;
		return;
	}
}

bool GJKCollide( D3DXVECTOR3 * verts0, unsigned int num_verts0, D3DXVECTOR3 * verts1, unsigned int num_verts1 )
{
	D3DXVECTOR3 supports[4];
	D3DXVECTOR3 support0;
	D3DXVECTOR3 support1;
	D3DXVECTOR3 search_direction;
	unsigned int num_supports = 1;

	supports[0] = verts0[0] - verts1[0];
	search_direction = -supports[0];

	unsigned int count = 0;

	while( count < 10 )
	{
		GetSupport( verts0, num_verts0, search_direction, support0 );
		GetSupport( verts1, num_verts1, -search_direction, support1 );
		//supports[num_supports] = support1 - support0;
		supports[num_supports] = support0 - support1;
		
		float res = D3DXVec3Dot( &supports[num_supports], &search_direction );
		if( res < -0.0000001f )
			return false;
		++num_supports;

		switch( num_supports )
		{
		case 2:
			SimplexLine( supports, num_supports, search_direction );
			break;
		case 3:
			SimplexTriangle( supports, num_supports, search_direction );
			break;
		case 4:
			SimplexTetrahedron( supports, num_supports, search_direction );
			if( num_supports == 4 ) return true;
			break;
		}

		++count;
	}

	return true;
}