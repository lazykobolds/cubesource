#include "MathLib.h"

#include <stdio.h>
#include <list>

D3DXMATRIX ROTATION_MATS::R_90_X(	1.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 1.f, 0.f,
									0.f, -1.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_n90_X(	1.f, 0.f, 0.f, 0.f,
									0.f, 0.f, -1.f, 0.f,
									0.f, 1.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_90_Y(	0.f, 0.f, -1.f, 0.f,
									0.f, 1.f, 0.f, 0.f,
									1.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_n90_Y(	0.f, 0.f, 1.f, 0.f,
									0.f, 1.f, 0.f, 0.f,
									-1.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_90_Z(	0.f, 1.f, 0.f, 0.f,
									-1.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 1.f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_n90_Z(	0.f, -1.f, 0.f, 0.f,
									1.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 1.f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_1_Y(	0.99984769515639123915701155881391f, 0.f, 0.01745240643728351281941897851632f, 0.f,
									0.f, 1.f, 0.f, 0.f,
									-0.01745240643728351281941897851632f, 0.f, 0.99984769515639123915701155881391f, 0.f,
									0.f, 0.f, 0.f, 1.f );

D3DXMATRIX ROTATION_MATS::R_n1_Y(	0.99984769515639123915701155881391f, 0.f, -0.01745240643728351281941897851632f, 0.f,
									0.f, 1.f, 0.f, 0.f,
									0.01745240643728351281941897851632f, 0.f, 0.99984769515639123915701155881391f, 0.f,
									0.f, 0.f, 0.f, 1.f );
									
void BuildVQSMatrix( D3DXMATRIX & out, const D3DXQUATERNION & rotation, const D3DXVECTOR3 & translation )
{
	D3DXMatrixRotationQuaternion( &out, &rotation );
	out._41 = translation.x;
	out._42 = translation.y;
	out._43 = translation.z;
}

void SetMatrix( D3DXMATRIX & out, D3DXVECTOR3 & v0, D3DXVECTOR3 & v1, D3DXVECTOR3 & v2 )
{
	out._11 = v0.x;
	out._12 = v0.y;
	out._13 = v0.z;
	out._14 = 0.f;
	
	out._21 = v1.x;
	out._22 = v1.y;
	out._23 = v1.z;
	out._24 = 0.f;
	
	out._31 = v2.x;
	out._32 = v2.y;
	out._33 = v2.z;
	out._34 = 0.f;
	
	out._41 = 0.f;
	out._42 = 0.f;
	out._43 = 0.f;
	out._44 = 1.f;
}

D3DXVECTOR3 GetTranslation( D3DXMATRIX & mat )
{
	return D3DXVECTOR3( mat._41, mat._42, mat._43 );
}


void PrintMatrix( D3DXMATRIX & mat )
{
	for( unsigned int y = 0; y < 4; ++y )
	{
		for( unsigned int x = 0; x < 4; ++x )
		{
			printf("%f ", mat( x, y ) );
		}
		printf("\n");
	}
	printf("\n");
}


void RotateMatrix( D3DXMATRIX & mat, D3DXVECTOR3 & rotation_axix, float radians )
{
	D3DXMATRIX rotation_matrix;
	D3DXMatrixRotationAxis( &rotation_matrix, &rotation_axix, radians );
	D3DXMatrixMultiply( &mat, &mat, &rotation_matrix );
}

// The 'Y' axis is assumed to be changed, and now the 'X' and 'Z' axis are needed to be adjusted to form right angles
void AdjustMatrix( D3DXMATRIX & mat )
{
	mat._11 = mat._22 * mat._33 - mat._23 * mat._32;
	mat._12 = mat._23 * mat._31 - mat._21 * mat._33;
	mat._13 = mat._21 * mat._32 - mat._22 * mat._31;
	float temp = sqrtf( mat._11 * mat._11 + mat._12 * mat._12 + mat._13 * mat._13 );
	mat._11 /= temp;
	mat._12 /= temp;
	mat._13 /= temp;
	mat._31 = mat._12 * mat._23 - mat._13 * mat._22;
	mat._32 = mat._13 * mat._21 - mat._11 * mat._23;
	mat._33 = mat._11 * mat._22 - mat._12 * mat._21;	
}

double return_val( double input )
{
	//return input;
	double mul = ( input > 0.f ) ? 1.f : -1.f;
	return ( cos( 3.14159265 * input * mul / 50.0 ) * -1.0 + 1.0 ) * mul;
}

void MapToSphere( D3DXVECTOR3 & vec )
{
	float height = vec.y;
	vec.x /= 50.f;
	vec.y = 1.f;
	vec.z /= 50.f;

	float sx = vec.x * sqrtf(1.0f - vec.y * vec.y * 0.5f - vec.z * vec.z * 0.5f + vec.y * vec.y * vec.z * vec.z / 3.0f);
	float sy = vec.y * sqrtf(1.0f - vec.z * vec.z * 0.5f - vec.x * vec.x * 0.5f + vec.z * vec.z * vec.x * vec.x / 3.0f);
	float sz = vec.z * sqrtf(1.0f - vec.x * vec.x * 0.5f - vec.y * vec.y * 0.5f + vec.x * vec.x * vec.y * vec.y / 3.0f);
	vec.x = sx;
	vec.y = sy;
	vec.z = sz;
	vec *= height;
}

void MapToSphere( Vertex & vec )
{
	float height = vec.y;
	vec.x /= 50.f;
	vec.y = 1.f;
	vec.z /= 50.f;
	
	float sx = vec.x * sqrtf(1.0f - vec.y * vec.y * 0.5f - vec.z * vec.z * 0.5f + vec.y * vec.y * vec.z * vec.z / 3.0f);
	float sy = vec.y * sqrtf(1.0f - vec.z * vec.z * 0.5f - vec.x * vec.x * 0.5f + vec.z * vec.z * vec.x * vec.x / 3.0f);
	float sz = vec.z * sqrtf(1.0f - vec.x * vec.x * 0.5f - vec.y * vec.y * 0.5f + vec.x * vec.x * vec.y * vec.y / 3.0f);
	vec.x = sx *= height;
	vec.y = sy *= height;
	vec.z = sz *= height;
}

D3DXVECTOR3 MapToSphere( double x, double y, double z )
{
	static double PI = 3.1415926535897932384626433832795;
	static double PI_200 = PI / 200.0;
	static double rad_scale = 1.0 / tanh( 3.14159265 / 4.0 );
	double mag = y;
	x = rad_scale * tanh( PI * x / 200.0 );
	y = 1.0;
	z = rad_scale * tanh( PI * z / 200.0 );
	double norm_mag = sqrtl( x*x + y*y + z*z );
	x = x*mag/norm_mag;
	y = y*mag/norm_mag;
	z = z*mag/norm_mag;
	return D3DXVECTOR3( float(x), float(y), float(z) );
}

void MapToSphere( LPDIRECT3DVERTEXBUFFER9 verts, unsigned int num_verts )
{
	D3DXVECTOR3 vertex;
	Vertex * vert_buff;
	HRESULT err = verts->Lock( 0, num_verts * sizeof( Vertex ), (void**)&vert_buff, 0 );
	for( unsigned int i = 0; i < num_verts; ++i )
	{
		vertex.x = vert_buff[i].x;
		vertex.y = vert_buff[i].y;
		vertex.z = vert_buff[i].z;
		MapToSphere( vertex );
		vert_buff[i].x = vertex.x;
		vert_buff[i].y = vertex.y;
		vert_buff[i].z = vertex.z;
	}
	verts->Unlock();
}

void FixNormals( LPDIRECT3DVERTEXBUFFER9 verts, unsigned int num_verts )
{
	D3DXVECTOR3 vec[2];
	Vertex * vert_buff;
	HRESULT err = verts->Lock( 0, num_verts * sizeof( Vertex ), (void**)&vert_buff, 0 );
	for( unsigned int i = 0; i < num_verts; i += 3 )
	{
		vec[1].x = vert_buff[i+1].x-vert_buff[i].x;
		vec[1].y = vert_buff[i+1].y-vert_buff[i].y;
		vec[1].z = vert_buff[i+1].z-vert_buff[i].z;

		vec[0].x = vert_buff[i+2].x-vert_buff[i].x;
		vec[0].y = vert_buff[i+2].y-vert_buff[i].y;
		vec[0].z = vert_buff[i+2].z-vert_buff[i].z;

		D3DXVec3Cross( &vec[0], &vec[0], &vec[1] );
		for( unsigned int j = 0; j < 3; ++j )
		{
			vert_buff[i+j].nx = vec[0].x;
			vert_buff[i+j].ny = vec[0].y;
			vert_buff[i+j].nz = vec[0].z;
		}
	}
	verts->Unlock();
}

void MapToSphereNormalized( D3DXVECTOR3 & vec )
{
	static double PI = 3.1415926535897932384626433832795;
	static double PI_200 = PI / 200.0;
	static double rad_scale = 1.f / tanh( PI / 4.0 );
	double x = rad_scale * tanh(PI_200 * vec.x);
	double y = 1.0;
	double z = rad_scale * tanh(PI_200 * vec.z);
	double norm_mag = sqrtl(x*x + y*y + z*z);
	vec.x = float(x) / float(norm_mag);
	vec.y = float(y) / float(norm_mag);
	vec.z = float(z) / float(norm_mag);
}

void ThickLine( Vertex * verts, unsigned int & vert_count, D3DXVECTOR3 & p0, D3DXVECTOR3 & p1, float width )
{
	D3DXVECTOR3 norm( p1.y-p0.y, p0.x-p1.x, 0.f );
	float mag = sqrt(norm.x * norm.x + norm.y * norm.y) / width;
	norm.x /= mag;
	norm.y /= mag;

	verts[vert_count].color = 0;
	verts[vert_count].x = p0.x + norm.x;
	verts[vert_count].y = p0.y + norm.y;
	verts[vert_count++].z = 0.f;
	
	verts[vert_count].color = 0;
	verts[vert_count].x = p1.x + norm.x;
	verts[vert_count].y = p1.y + norm.y;
	verts[vert_count++].z = 0.f;
	
	verts[vert_count].color = 0;
	verts[vert_count].x = p0.x - norm.x;
	verts[vert_count].y = p0.y - norm.y;
	verts[vert_count++].z = 0.f;
	
	verts[vert_count].color = 0;
	verts[vert_count++] = verts[vert_count-1];
	
	verts[vert_count].color = 0;
	verts[vert_count++] = verts[vert_count-3];

	verts[vert_count].x = p1.x - norm.x;
	verts[vert_count].y = p1.y - norm.y;
	verts[vert_count].color = 0;
	verts[vert_count++].z = 0.f;

}


void SubDivide( Vertex triangle[3], D3DXPLANE & plane, Vertex outside[6], Vertex inside[6], unsigned int & out_count, unsigned int & in_count )
{
	out_count = 0;
	in_count = 0;
	float num, denom, t;
	D3DXVECTOR3 vec;
	float vx, vy, vz;
	unsigned int plane_count = 0;
	unsigned int points = 0;

	for( unsigned int i = 0; i < 3; ++i )
	{
		vec = D3DXVECTOR3( triangle[(i+1)%3].x - triangle[i].x, triangle[(i+1)%3].y - triangle[i].y, triangle[(i+1)%3].z - triangle[i].z );
		vx = triangle[(i+1)%3].x - triangle[i].x;
		vy = triangle[(i+1)%3].y - triangle[i].y;
		vz = triangle[(i+1)%3].z - triangle[i].z;
		denom = 
			vec.x * plane.a +
			vec.y * plane.b +
			vec.z * plane.c;
		denom = 
			vx * plane.a +
			vy * plane.b +
			vz * plane.c;
		num = 
			triangle[i].x * plane.a +
			triangle[i].y * plane.b +
			triangle[i].z * plane.c + plane.d;
		if( num >= 0.00001 )//0.f )
		{
			outside[out_count++] = triangle[i];
			if( denom >= 0.00001 ) continue;
		}else if( num <= -0.00001 )
		{
			inside[in_count++] = triangle[i];
			if( denom < 0.0 ) continue;
		}else
		{
			outside[out_count++] = triangle[i];
			inside[in_count++] = triangle[i];
			++plane_count;
			continue;
		}
		if( denom > 0.00001 || denom < -0.00001 ) 
		{
			//num = 
			//	triangle[i].x * plane.a +
			//	triangle[i].y * plane.b +
			//	triangle[i].z * plane.c + plane.d;
			t = -num / denom;
			//if( t > 0.00001f && t <= 0.99999f )
			if( t > 0.00001 && t < 0.99999 )
			{
				outside[out_count].x = triangle[i].x + vx * t;
				outside[out_count].y = triangle[i].y + vy * t;
				outside[out_count].z = triangle[i].z + vz * t;
				outside[out_count].color = triangle[i].color;
				inside[in_count] = outside[out_count];
				++in_count;
				++out_count;
				points++;
			}
		}
	}
	if( points == 0 )
	{
		if( ( in_count == 2 || in_count == 3 ) && out_count == 1 )
		{
			out_count = 0;
			in_count = 3;
			inside[0] = triangle[0];
			inside[1] = triangle[1];
			inside[2] = triangle[2];			
		}
		if( in_count == 1 && ( out_count == 2 || out_count == 3 ) )
		{
			out_count = 3;
			in_count = 0;
			outside[0] = triangle[0];
			outside[1] = triangle[1];
			outside[2] = triangle[2];			
		}
	}
	if( points == 1 )
	{
		if( in_count == 3 && out_count == 2 )
		{
			out_count = 0;
			in_count = 3;
			inside[0] = triangle[0];
			inside[1] = triangle[1];
			inside[2] = triangle[2];
		}
		if( out_count == 3 && in_count == 2 )
		{
			out_count = 3;
			in_count = 0;
			outside[0] = triangle[0];
			outside[1] = triangle[1];
			outside[2] = triangle[2];	
		}
	}
	if( plane_count == 2 && points == 0 )
	{
		if( out_count == 3 ) in_count = 0;
		else out_count = 0;
	}
	if( plane_count == 1 && points == 0 )
	{
		if( out_count == 3 && in_count == 1 ) in_count = 0;
		else if( out_count == 1 && in_count == 3 ) out_count = 0;
		else if( out_count == 2 && in_count == 0 )
		{
			out_count = 3;
			outside[0] = triangle[0];
			outside[1] = triangle[1];
			outside[2] = triangle[2];	
		}else if( out_count == 0 && in_count == 2 )
		{
			in_count = 3;
			inside[0] = triangle[0];
			inside[1] = triangle[1];
			inside[2] = triangle[2];	
		}
	}
	if( plane_count == 3 ) in_count = 0;
	if( points == 1 )
	{
		if( in_count == 1 && out_count == 4 )
		{
			in_count = 0;
			out_count = 3;
			outside[0] = triangle[0];
			outside[1] = triangle[1];
			outside[2] = triangle[2];
		}
		if( in_count == 4 && out_count == 1 )
		{
			out_count = 0;
			in_count = 3;
			inside[0] = triangle[0];
			inside[1] = triangle[1];
			inside[2] = triangle[2];
		}
	}
	if( in_count == 4 && out_count == 2 )
	{
		in_count = 3;
		out_count = 0;
		inside[0] = triangle[0];
		inside[1] = triangle[1];
		inside[2] = triangle[2];
	}
	if( in_count == 2 && out_count == 4 )
	{
		in_count = 0;
		out_count = 3;
		outside[0] = triangle[0];
		outside[1] = triangle[1];
		outside[2] = triangle[2];
	}
	if( ( in_count != 4 && in_count % 3 != 0 ) || ( out_count != 4 && out_count % 3 != 0 ) )
	{
		printf("planes: %i\n", plane_count );
		printf("points: %i\n",points );
		printf("in: %i\nout: %i\n\n",in_count,out_count);
	}
	if( in_count == 4 )
	{
		inside[4] = inside[0];
	
		inside[5] = inside[2];
		in_count = 6;
	}
	if( out_count == 4 )
	{
		outside[4] = outside[0];
		outside[5] = outside[2];
		out_count = 6;
	}
}

bool InsidePlanePointTest2D( D3DXVECTOR3 & plane, D3DXVECTOR3 & plane_point, D3DXVECTOR3 & point )
{
	float res = plane.x*point.x+plane.y*point.y-(plane.x*plane_point.x+plane.y*plane_point.y);
	return ( res <= 0.f );
}

bool PointsAreEqual( D3DXVECTOR3 & p0, D3DXVECTOR3 & p1 )
{
	float diff = p0.x-p1.x;
	diff = sqrt(diff*diff);
	if( diff > 0.0001f ) return false;
	
	diff = p0.y-p1.y;
	diff = sqrt(diff*diff);
	if( diff > 0.0001f ) return false;
	
	diff = p0.z-p1.z;
	diff = sqrt(diff*diff);
	if( diff > 0.0001f ) return false;

	return true;
}

bool VertsAreEqual( Vertex & v0, Vertex v1 )
{
	if( PointsAreEqual( D3DXVECTOR3( v0.x, v0.y, v0.z ), D3DXVECTOR3( v1.x, v1.y, v1.z ) ) == false ) return false;
	if( v0.color != v1.color ) return false;
	if( fabs( v0.u - v1.u ) > 0.0001f || fabs( v0.v - v1.v ) > 0.0001f ) return false;
	if( PointsAreEqual( D3DXVECTOR3( v0.nx, v0.ny, v0.nz ), D3DXVECTOR3( v1.nx, v1.ny, v1.nz ) ) == false ) return false;
	return true;
}

void TransformPoints( D3DXVECTOR3 * new_points, D3DXVECTOR3 * points, unsigned int num_points, D3DXMATRIX & old_mat, D3DXMATRIX & new_mat )
{
	D3DXMATRIX tempmat;
	D3DXMATRIX transform;

	D3DXMatrixTranspose( &tempmat, &new_mat );
	transform = old_mat * tempmat;

	for( unsigned int i = 0; i < num_points; ++i )
	{
		D3DXVec3TransformCoord( &new_points[i], &points[i], &transform );
	}
}

void AddQuad( D3DXVECTOR3 & tl, D3DXVECTOR3 & bl, D3DXVECTOR3 & br, D3DXVECTOR3 & tr, Vertex * vert_data, unsigned int & vert_count, D3DXVECTOR3 * quad_data, unsigned int & quad_count )
{
	DWORD color;// = D3DCOLOR_XRGB( rand()%255, 100, 10 );
	unsigned int val = int(tl.y*40);///5.f);
	unsigned int val2 = int(sinf(tl.y)*100.f);
	unsigned int val3 = int(cosf(tl.y)*100.f);
	val = int(255 * tl.y / 10.f);
	color = D3DCOLOR_XRGB( val,val,val);
	if( fabs(tl.y-br.y) < 0.0001f ) color = D3DCOLOR_XRGB( 30, val, 50 );
	if( tl.y < 10.f * 0.1953125 ) color = D3DCOLOR_XRGB( val, val, 250 );
	float diff = tl.y - bl.y;
	float blocksize = 100.f / 256.f;
	float width = 0.5f;
	float bot = width * diff  / blocksize;
	float xoffset = 0.0f;
	float yoffset = -width;
	
	quad_data[quad_count] = tl;
	++quad_count;
	quad_data[quad_count] = bl;
	++quad_count;
	quad_data[quad_count] = br;
	++quad_count;
	quad_data[quad_count] = tr;
	++quad_count;

	vert_data[vert_count].x = tl.x;
	vert_data[vert_count].y = tl.y;
	vert_data[vert_count].z = tl.z;
	vert_data[vert_count].u = 0.f + xoffset;
	vert_data[vert_count].v = 0.f + yoffset;
	vert_data[vert_count].color = color;
	++vert_count;

	vert_data[vert_count].x = br.x;
	vert_data[vert_count].y = br.y;
	vert_data[vert_count].z = br.z;
	vert_data[vert_count].u = width + xoffset;
	vert_data[vert_count].v = bot + yoffset;
	vert_data[vert_count].color = color;
	++vert_count;

	vert_data[vert_count].x = bl.x;
	vert_data[vert_count].y = bl.y;
	vert_data[vert_count].z = bl.z;
	vert_data[vert_count].u = 0.f + xoffset;
	vert_data[vert_count].v = bot + yoffset;
	vert_data[vert_count].color = color;
	++vert_count;

	vert_data[vert_count].x = tl.x;
	vert_data[vert_count].y = tl.y;
	vert_data[vert_count].z = tl.z;
	vert_data[vert_count].u = 0.f + xoffset;
	vert_data[vert_count].v = 0.f + yoffset;
	vert_data[vert_count].color = color;
	++vert_count;

	vert_data[vert_count].x = tr.x;
	vert_data[vert_count].y = tr.y;
	vert_data[vert_count].z = tr.z;
	vert_data[vert_count].u = width + xoffset;
	vert_data[vert_count].v = 0.f + yoffset;
	vert_data[vert_count].color = color;
	++vert_count;

	vert_data[vert_count].x = br.x;
	vert_data[vert_count].y = br.y;
	vert_data[vert_count].z = br.z;
	vert_data[vert_count].u = width + xoffset;
	vert_data[vert_count].v = bot + yoffset;
	vert_data[vert_count].color = color;
	++vert_count;
}

bool IntersectSweptSphereQuad( const D3DXVECTOR3 & pos, const float radius, const D3DXVECTOR3 & velocity, const D3DXVECTOR3 * quad, D3DXVECTOR3 & norm, float & t )
{
	D3DXVECTOR3 edges[4];
	D3DXVECTOR3 face_norm;
	D3DXVECTOR3 edge_norms[4];
	unsigned int result = 0;

	//left
	edges[0] = quad[1]-quad[0];
	//bottom
	edges[1] = quad[2]-quad[1];
	//right
	edges[2] = quad[3]-quad[2];
	//top
	edges[3] = quad[0]-quad[3];
	
	D3DXVec3Cross( &face_norm, &edges[0], &edges[1] );
	D3DXVec3Normalize( &face_norm, &face_norm );
	float face_d = -D3DXVec3Dot( &face_norm, &quad[0] );

	float denom = D3DXVec3Dot( &face_norm, &velocity );
	if( fabs( denom ) < 0.0001f ) return false;
	
	float numerator = radius - face_d - D3DXVec3Dot( &face_norm, &pos );
	t = numerator / denom;

	if( t < -0.1f || t >= 1.f ) return false;

	D3DXVECTOR3 intersect = pos + ( velocity * t );

	for( unsigned int i = 0; i < 4; ++i )
	{
		D3DXVec3Cross( &edge_norms[i], &edges[i], &face_norm );
		D3DXVec3Normalize( &edge_norms[i], &edge_norms[i] );

		denom = D3DXVec3Dot( &intersect, &edge_norms[i] ) - D3DXVec3Dot( &edge_norms[i], &quad[i] );
		if( denom > 0.001f ) return false;
	}

	norm = face_norm;
	return true;
}

bool IntersectSphereQuad( const D3DXVECTOR3 & pos, const float radius, const D3DXVECTOR3 * quad, D3DXVECTOR3 & displacement )
{
	D3DXVECTOR3 edges[4];
	D3DXVECTOR3 face_norm;
	D3DXVECTOR3 edge_norms[4];
	float res[4];
	bool outside[4];
	float face_res;
	float dot;
	float mag;
	unsigned int result = 0;

	//left
	edges[0] = quad[1]-quad[0];
	//bottom
	edges[1] = quad[2]-quad[1];
	//right
	edges[2] = quad[3]-quad[2];
	//top
	edges[3] = quad[0]-quad[3];
	
	D3DXVec3Cross( &face_norm, &edges[0], &edges[1] );
	D3DXVec3Normalize( &face_norm, &face_norm );

	for( unsigned int i = 0; i < 4; ++i )
	{
		D3DXVec3Cross( &edge_norms[i], &edges[i], &face_norm );
		D3DXVec3Normalize( &edge_norms[i], &edge_norms[i] );
	}
	
	face_res = D3DXVec3Dot( &pos, &face_norm ) - D3DXVec3Dot( &quad[0], &face_norm );


	if( face_res > radius ) return false;
	if( face_res < -(radius * 3.f) ) return false;
	
	bool inside = true;
	for( unsigned int i = 0; i < 4; ++i )
	{
		res[i] = D3DXVec3Dot( &pos, &edge_norms[i] ) - D3DXVec3Dot( &quad[i], &edge_norms[i] );
		if( res[i] > 0.f )
		{
			if( res[i] >= 0.01f ) return false;
		}else
		{
			outside[i] = false;
		}
	}
	
	displacement = face_norm * ( radius - face_res );
	return true;

	for( unsigned int i = 0; i < 4; ++i )
	{
		unsigned int next = (i+1)%4;
		unsigned int prev = (i+3)%4;
		if( outside[i] )
		{
			dot = D3DXVec3Dot( &pos, &edges[i] ) - D3DXVec3Dot( &quad[i], &edges[i] );
			if( dot > 0.f )
			{
				dot = D3DXVec3Dot( &pos, &edges[i] ) - D3DXVec3Dot( &quad[next], &edges[i] );
				if( dot < 0.f ) 
				{
					displacement = pos - quad[next];
					D3DXVec3Cross( &displacement, &displacement, &edge_norms[i] );
					D3DXVec3Cross( &displacement, &edge_norms[i], &displacement );
					mag = D3DXVec3Length( &displacement );
					if( mag >= radius ) return false;
					displacement *= ( radius - mag ) / mag;
					return true;
				}
			}
			//dot = D3DXVec3Dot( &pos, &edges[i] ) - D3DXVec3Dot( &quad[next], &edges[i] );
			//if( dot > 0.f )
			//{
			//	dot = D3DXVec3Dot( &pos, &edges[next] ) - D3DXVec3Dot( &quad[next], &edges[next] );
			//	if( dot < 0.f )
			//	{
			//		//next corner
			//		displacement = pos - quad[next];
			//		mag = D3DXVec3Length( &displacement );
			//		if( mag >= radius ) return false;
			//		displacement *= ( radius - mag ) / mag;
			//		return true;
			//	}else
			//	{
			//		//next edge
			//		displacement = pos - quad[next];
			//		D3DXVec3Cross( &displacement, &displacement, &edge_norms[next] );
			//		D3DXVec3Cross( &displacement, &edge_norms[next], &displacement );
			//		mag = D3DXVec3Length( &displacement );
			//		if( mag >= radius ) return false;
			//		displacement *= ( radius - mag  ) / mag;
			//		return true;
			//	}
			//}
			//dot = D3DXVec3Dot( &pos, &edges[i] ) - D3DXVec3Dot( &quad[i], &edges[i] );
			//if( dot < 0.f )
			//{
			//	dot = D3DXVec3Dot( &pos, &edges[prev] ) - D3DXVec3Dot( &quad[i], &edges[prev] ); 
			//	if( dot > 0.f )
			//	{
			//		//prev corner
			//		displacement = pos - quad[i];
			//		mag = sqrt( displacement.x*displacement.x + displacement.y*displacement.y + displacement.z*displacement.z );
			//		if( mag >= radius ) return false;
			//		displacement *= ( radius - mag ) / mag;
			//		return true;
			//	}else
			//	{
			//		//prev edge
			//		displacement = pos - quad[i];
			//		D3DXVec3Cross( &displacement, &displacement, &edge_norms[prev] );
			//		D3DXVec3Cross( &displacement, &edge_norms[next], &displacement );
			//		mag = D3DXVec3Length( &displacement );
			//		if( mag >= radius ) return false;
			//		displacement *= ( radius - mag  ) / mag;
			//		return true;
			//	}
			//}
			////current edge
			//displacement = pos - quad[next];
			//D3DXVec3Cross( &displacement, &displacement, &edge_norms[i] );
			//D3DXVec3Cross( &displacement, &edge_norms[i], &displacement );
			//mag = D3DXVec3Length( &displacement );
			//if( mag >= radius ) return false;
			//displacement *= ( radius - mag ) / mag;
			//return true;			
		}
	}
	return false;

	if( outside[0] || outside[1] || outside[2] || outside[3] ) return false;

	








	return false;
	
	face_res = D3DXVec3Dot( &pos, &face_norm ) - D3DXVec3Dot( &quad[0], &face_norm ); 
	if( face_res > radius || face_res < -radius ) return false;

	for( unsigned int i = 0; i < 4; ++i )
	{
		res[i] = D3DXVec3Dot( &pos, &edge_norms[i] ) - D3DXVec3Dot( &quad[i], &edge_norms[i] );
		if( res[i] > 0.f )
		{
			if( res[i] > radius )
			{
				return false;
			}
			result |= (1<<i);
		}
	}

	switch( result )
	{
	case 0:
		//outside main face
		face_res = D3DXVec3Dot( &pos, &displacement ) - D3DXVec3Dot( &quad[0], &displacement ); 
		displacement *= radius - face_res;
		return true;
		break;
	case 1:
		//outside edge 1
		D3DXVec3Normalize( &edges[0], &edges[0] );
		dot = D3DXVec3Dot( &pos, &edges[0] );
		mag = dot - D3DXVec3Dot( &quad[0], &edges[0] );
		if( mag < 0.f )
		{
			//outside point 0
			displacement = pos - quad[0];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		mag = dot - D3DXVec3Dot( &quad[1], &edges[0] );
		if( mag > 0.f )
		{
			//outside point 1
			displacement = pos - quad[1];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		displacement = edge_norms[0];
		displacement *= radius - res[0];
		return true;
		break;
	case 2:
		//outside edge 2
		D3DXVec3Normalize( &edges[1], &edges[1] );
		dot = D3DXVec3Dot( &pos, &edges[1] );
		mag = dot - D3DXVec3Dot( &quad[1], &edges[1] );
		if( mag < 0.f )
		{
			//outside point 0
			displacement = pos - quad[1];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		mag = dot - D3DXVec3Dot( &quad[2], &edges[1] );
		if( mag > 0.f )
		{
			//outside point 1
			displacement = pos - quad[2];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		displacement = edge_norms[1];
		displacement *= radius - res[1];
		return true;
		break;
	case 3:
		//outside edge 1 and 2
		dot = D3DXVec3Dot( &edge_norms[0], &edge_norms[1] );
		if( dot < 0.f )
		{
			mag = D3DXVec3Dot( &pos, &edges[0] ) - D3DXVec3Dot( &quad[1], &edges[0] );
			if( mag < 0.f )
			{
				//outside edge 1
				displacement = edge_norms[0];
				displacement *= radius - res[0];
				return true;
			}
			mag = D3DXVec3Dot( &pos, &edges[1] ) - D3DXVec3Dot( &quad[1], &edges[1] );
			if( mag > 0.f )
			{
				//outside edge 2
				displacement = edge_norms[1];
				displacement *= radius - res[1];
				return true;
			}
		}
		//outside point 1
		displacement = pos - quad[1];
		mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
		displacement *= ( radius - mag ) / mag;
		return true;
		break;
	case 4:
		//outside edge 3
		D3DXVec3Normalize( &edges[2], &edges[2] );
		dot = D3DXVec3Dot( &pos, &edges[2] );
		mag = dot - D3DXVec3Dot( &quad[2], &edges[2] );
		if( mag < 0.f )
		{
			//outside point 0
			displacement = pos - quad[2];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		mag = dot - D3DXVec3Dot( &quad[3], &edges[2] );
		if( mag > 0.f )
		{
			//outside point 1
			displacement = pos - quad[3];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		displacement = edge_norms[2];
		displacement *= radius - res[2];
		return true;
		break;
	case 6:
		//outside edge 3 and 2
		dot = D3DXVec3Dot( &edge_norms[2], &edge_norms[1] );
		if( dot < 0.f )
		{
			mag = D3DXVec3Dot( &pos, &edges[1] ) - D3DXVec3Dot( &quad[2], &edges[1] );
			if( mag < 0.f )
			{
				//outside edge 1
				displacement = edge_norms[1];
				displacement *= radius - res[1];
				return true;
			}
			mag = D3DXVec3Dot( &pos, &edges[2] ) - D3DXVec3Dot( &quad[2], &edges[2] );
			if( mag > 0.f )
			{
				//outside edge 2
				displacement = edge_norms[2];
				displacement *= radius - res[2];
				return true;
			}
		}
		//outside point 1
		displacement = pos - quad[2];
		mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
		displacement *= ( radius - mag ) / mag;
		return true;
		break;
	case 8:
		//outside edge 4
		D3DXVec3Normalize( &edges[3], &edges[3] );
		dot = D3DXVec3Dot( &pos, &edges[3] );
		mag = dot - D3DXVec3Dot( &quad[3], &edges[3] );
		if( mag < 0.f )
		{
			//outside point 0
			displacement = pos - quad[3];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		mag = dot - D3DXVec3Dot( &quad[0], &edges[3] );
		if( mag > 0.f )
		{
			//outside point 1
			displacement = pos - quad[0];
			mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
			displacement *= ( radius - mag ) / mag;
			return true;
		}
		displacement = edge_norms[3];
		displacement *= radius - res[3];
		return true;
		break;
	case 9:
		//outside edge 4 and 1
		dot = D3DXVec3Dot( &edge_norms[0], &edge_norms[3] );
		if( dot < 0.f )
		{
			mag = D3DXVec3Dot( &pos, &edges[3] ) - D3DXVec3Dot( &quad[0], &edges[3] );
			if( mag < 0.f )
			{
				//outside edge 1
				displacement = edge_norms[3];
				displacement *= radius - res[3];
				return true;
			}
			mag = D3DXVec3Dot( &pos, &edges[0] ) - D3DXVec3Dot( &quad[0], &edges[0] );
			if( mag > 0.f )
			{
				//outside edge 2
				displacement = edge_norms[0];
				displacement *= radius - res[0];
				return true;
			}
		}
		//outside point 1
		displacement = pos - quad[0];
		mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
		displacement *= ( radius - mag ) / mag;
		return true;
		break;
	case 12:
		//outside 4 and 3
		dot = D3DXVec3Dot( &edge_norms[2], &edge_norms[3] );
		if( dot < 0.f )
		{
			mag = D3DXVec3Dot( &pos, &edges[2] ) - D3DXVec3Dot( &quad[3], &edges[2] );
			if( mag < 0.f )
			{
				//outside edge 1
				displacement = edge_norms[2];
				displacement *= radius - res[2];
				return true;
			}
			mag = D3DXVec3Dot( &pos, &edges[3] ) - D3DXVec3Dot( &quad[3], &edges[3] );
			if( mag > 0.f )
			{
				//outside edge 2
				displacement = edge_norms[3];
				displacement *= radius - res[3];
				return true;
			}
		}
		//outside point 1
		displacement = pos - quad[3];
		mag = sqrt( displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z );
		displacement *= ( radius - mag ) / mag;
		return true;
		break;
	default:
		return false;
		break;

	}
}
