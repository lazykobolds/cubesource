#include <assert.h>
#include <stdio.h>
#include "Voronoi.h"
#include "Utilities\MathLib.h"

void ExpandHull( D3DXVECTOR3 * hull, unsigned int &num_points, D3DXVECTOR3 & new_point )
{
	unsigned int next, next2;
	float res;
	D3DXVECTOR3 vec, norm;
	unsigned int remove[ MAX_POINTS ];
	for( unsigned int i = 0; i < num_points; ++i ) remove[i] = 0;
	for( unsigned int i = 0; i < num_points; ++i )
	{
		next = (i+1)%num_points;
		vec = hull[next]-hull[i];
		norm.x = vec.y;
		norm.y = -vec.x;
		res = ( norm.x*new_point.x+norm.y*new_point.y ) - ( norm.x*hull[i].x+norm.y*hull[i].y );
		if( res >= 0.f )
		{
			remove[i] = 1;
			remove[next] = 1;
		}
	}
	bool found = false;
	for( unsigned int i = 0; i < num_points; ++i )
	{
		if( remove[i] == 1 ) 
		{
			found = true;
			break;
		}
	}
	if( !found ) return;

	unsigned int start = 0;
	for( unsigned int i = 0; i < num_points; ++i )
	{
		if( remove[i] == 0 )
		{
			start = i;
			break;
		}
	}
	unsigned int i = start;
	while( true )
	{
		next = (i+1)%num_points;
		next2 = (next+1)%num_points;
		if( num_points == 3 )
		{
			if( remove[i] == 1 && remove[next] == 1 && remove[next2] == 1 )
			{
				remove[next] = 2;
				break;
			}
		}else if( remove[i] > 0 && remove[next] == 1 && remove[next2] == 1 )
		{
			remove[next] = 2;
		}
		i = (i+1)%num_points;
		if( i == start ) break;
	}
	unsigned int cont = 0;
	for( i = 0; i < num_points; ++i )
	{
		if( remove[i] == 1 ) ++cont;
	}
	if( cont != 2 ) printf("IN TROUBLE\n");
	for( i = 0; i < num_points; ++i )
	{
		if( remove[i] == 2 )
		{
			for( unsigned int j = i; j < num_points-1; ++j )
			{
				remove[j] = remove[j+1];
				hull[j] = hull[j+1];
			}
			--num_points;
			i = -1;
		}
	}
	cont = 0;
	for( i = 0; i < num_points; ++i )
	{
		if( remove[i] == 1 ) ++cont;
		if( remove[i] == 2 ) 
		{
			printf("DANGER WILL ROBINSON! DANGER!!\n");
			printf("num_points: %i\n", num_points );
		}
	}
	if( cont != 2 ) printf("SUPER DANGER MAN RESCUE TIME1\n");

	D3DXVECTOR3 temp,temp2;
	for( i = 0; i < num_points; ++i )
	{
		next = (i+1)%num_points;
		if( remove[i] == 1 && remove[next] == 1 )
		{
			temp = hull[next];
			hull[next] = new_point;
			for( unsigned int j = next+1; j < num_points+1; ++j )
			{
				temp2 = hull[j];
				hull[j] = temp;
				temp = temp2;
			}
			++num_points;
			return;
		}
	}
}

void VTRI::AddPoint( D3DXVECTOR3 & point )
{
	for( unsigned int i = 0; i < num_points; ++i )
	{
		if( PointsAreEqual( point, points[i] ) ) return;
	}

	if( num_points < 3 ) 
	{
		points[ num_points ] = point;
		++num_points;

		if( num_points == 3 )
		{
			D3DXVECTOR3 vecs[2];
			vecs[0] = points[1]-points[0];
			vecs[1] = points[2]-points[0];
			float cross = vecs[0].x*vecs[1].y-vecs[1].x*vecs[0].y;
			if( cross < 0.f )
			{
				vecs[0] = points[1];
				points[1] = points[2];
				points[2] = vecs[0];
			}
		}
	}else
	{
		ExpandHull( points, num_points, point );
	}

}

Voronoi::Voronoi( void )
{
	num_verts = 0;
	vert_buffer = 0;
}

bool IsClosestSite( D3DXVECTOR3 & site, D3DXVECTOR3 * all_sites, unsigned int num_sites, D3DXVECTOR3 & e0, D3DXVECTOR3 & e1 )
{
	D3DXVECTOR3 diff[2];
	float dist[2];
	diff[0] = e0-site;
	dist[0] = (float)sqrt( diff[0].x * diff[0].x + diff[0].y * diff[0].y );
	diff[1] = e1-site;
	dist[1] = (float)sqrt( diff[1].x * diff[1].x + diff[1].y * diff[1].y );

	D3DXVECTOR3 check_diff;
	float check_dist;
	for( unsigned int i = 0; i < num_sites; ++i )
	{
		if( PointsAreEqual( site, all_sites[i] ) ) 
		{
			continue;
		}
		
		check_diff = e0 - all_sites[i];
		check_dist = (float)sqrt( check_diff.x * check_diff.x + check_diff.y * check_diff.y );
		if( check_dist < ( dist[0]-0.0001f ) ) 
		{
			return false;
		}

		check_diff = e1 - all_sites[i];
		check_dist = (float)sqrt( check_diff.x * check_diff.x + check_diff.y * check_diff.y );
		if( check_dist < ( dist[1]-0.0001f ) ) 
		{
			return false;
		}

	}
	return true;
}

unsigned int Voronoi::GetClosestPoint( D3DXVECTOR3 & point, D3DXVECTOR3 * points, unsigned int num_points )
{
	unsigned int closest_point = 0;
	D3DXVECTOR3 diff = points[0]-point;
	float dist = diff.x*diff.x+diff.y*diff.y;
	float test_dist;
	for( unsigned int i = 1; i < num_points; ++i )
	{
		diff = points[i]-point;
		test_dist = diff.x*diff.x+diff.y*diff.y;
		if( test_dist < dist )
		{
			dist = test_dist;
			closest_point = i;
		}
		
	}

	return closest_point;
}

void Voronoi::ComputeVD( LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 * points, unsigned int num_points )
{           
	ComputeVDSub( d3ddev, points, num_points );
	for( unsigned int itterations = 0; itterations < 2; ++itterations )
	{
		for( unsigned int i = 0; i < num_points; ++i )
		{
			D3DXVECTOR3 midpoint(0.f,0.f,0.f);
			for( unsigned int j = 0; j < VSites[i].num_points; ++j )
			{
				midpoint += VSites[i].points[j];
			}
			points[i] = midpoint / float(VSites[i].num_points);
		}	
		ComputeVDSub( d3ddev, points, num_points );
	}

	unsigned int num_tris = 0;
	for( unsigned int i = 0; i < num_points; ++i )
	{
		num_tris += VSites[i].num_points;
	}

	unsigned int add_count = 0;
	D3DXVECTOR3 p0, p1;
	generator.resetIterator();
	while( generator.getNext( p0.x, p0.y, p1.x, p1.y ) ) ++add_count;

	num_verts = num_tris * 3 + add_count * 6;
	HRESULT err = d3ddev->CreateVertexBuffer( 
		num_verts * sizeof( Vertex ), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Vertex::VertexFVF,
		D3DPOOL_DEFAULT,
		&vert_buffer,
		0 );

	printf("VORONOI VBUFFER ERROR: %i\n", err );
	switch( err )
	{
		case D3DERR_OUTOFVIDEOMEMORY:
			printf("out of vid mem\n");
			break;
		case D3DERR_INVALIDCALL:
			printf("invalid call\n");
			break;
	}

	Vertex * vert_data;
	err = vert_buffer->Lock( 0, num_verts * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );
	

	num_tris = 0;
	for( unsigned int i = 0; i < num_points; ++i )
	{
		if( VSites[i].num_points < 3 ) continue;
		for( unsigned int j = 0; j < VSites[i].num_points; ++j )
		{
			vert_data[num_tris*3].x = VSites[i].site.x;
			vert_data[num_tris*3].y = VSites[i].site.y;
			vert_data[num_tris*3].z = VSites[i].site.z;
			vert_data[num_tris*3].color = VSites[i].color;
			
			vert_data[num_tris*3+1].x = VSites[i].points[j].x;
			vert_data[num_tris*3+1].y = VSites[i].points[j].y;
			vert_data[num_tris*3+1].z = VSites[i].points[j].z;
			vert_data[num_tris*3+1].color = 0;//VSites[i].color;
			
			vert_data[num_tris*3+2].x = VSites[i].points[(j+1)%VSites[i].num_points].x;
			vert_data[num_tris*3+2].y = VSites[i].points[(j+1)%VSites[i].num_points].y;
			vert_data[num_tris*3+2].z = VSites[i].points[(j+1)%VSites[i].num_points].z;
			vert_data[num_tris*3+2].color = 0;//VSites[i].color;

			//vert_data[num_tris*3].x = VSites[i].points[0].x;
			//vert_data[num_tris*3].y = VSites[i].points[0].y;
			//vert_data[num_tris*3].z = VSites[i].points[0].z;
			//vert_data[num_tris*3].color = VSites[i].color;
			//
			//vert_data[num_tris*3+1].x = VSites[i].points[j].x;
			//vert_data[num_tris*3+1].y = VSites[i].points[j].y;
			//vert_data[num_tris*3+1].z = VSites[i].points[j].z;
			//vert_data[num_tris*3+1].color = 0.f;//VSites[i].color;
			//
			//vert_data[num_tris*3+2].x = VSites[i].points[j+1].x;
			//vert_data[num_tris*3+2].y = VSites[i].points[j+1].y;
			//vert_data[num_tris*3+2].z = VSites[i].points[j+1].z;
			//vert_data[num_tris*3+2].color = 0.f;//VSites[i].color;

			++num_tris;
		}
	}

	unsigned int vcount = num_tris * 3;
	generator.resetIterator();
	while( generator.getNext( p0.x, p0.y, p1.x, p1.y ) )
	{
		ThickLine( vert_data, vcount, p0, p1, 0.01f );
	}

	vert_buffer->Unlock();
}

void Voronoi::ComputeVDSub( LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 * points, unsigned int num_points )
{
	generator.reset();
	float xvals[MAX_POINTS];
	float yvals[MAX_POINTS];

	for( unsigned int i = 0; i < num_points; ++i )
	{
		xvals[i] = points[i].x;
		yvals[i] = points[i].y;
	}
	
	num_sites = num_points;
	for( unsigned int i = 0; i < num_points; ++i )
	{
		VSites[i].color = D3DCOLOR_XRGB( rand()%255, rand()%255, rand()%255 );
		VSites[i].site = points[i];
		VSites[i].num_points = 0;
	}
	
	//generator.generateVoronoi( xvals, yvals, num_points, -1.f, 1.f, -1.f, 1.f, 0.0f );
	generator.generateVoronoi( xvals, yvals, num_points, -1.f, 1.f, -1.f, 1.f, 0.0f );
	generator.resetIterator();

	D3DXVECTOR3 p0, p1;
	unsigned int num_tris = 0;
	unsigned int add_count = 0;
	while( generator.getNext( p0.x, p0.y, p1.x, p1.y ) )
	{
		++add_count;
		for( unsigned int i = 0; i < num_points; ++i )
		{
			if( IsClosestSite( points[i], points, num_points, p0, p1 ) )
			{
				VSites[i].AddPoint( p0 );
				VSites[i].AddPoint( p1 );
			}
		}
	}

	unsigned int close_site;
	close_site = GetClosestPoint( D3DXVECTOR3( -1.f, 1.f, 0.f ), points, num_points );
	VSites[close_site].AddPoint( D3DXVECTOR3( -1.f, 1.f, 0.f ) );

	close_site = GetClosestPoint( D3DXVECTOR3( 1.f, 1.f, 0.f ), points, num_points );
	VSites[close_site].AddPoint( D3DXVECTOR3( 1.f, 1.f, 0.f ) );

	close_site = GetClosestPoint( D3DXVECTOR3( -1.f, -1.f, 0.f ), points, num_points );
	VSites[close_site].AddPoint( D3DXVECTOR3( -1.f, -1.f, 0.f ) );

	close_site = GetClosestPoint( D3DXVECTOR3( 1.f, -1.f, 0.f ), points, num_points );
	VSites[close_site].AddPoint( D3DXVECTOR3( 1.f, -1.f, 0.f ) );
}


float * Voronoi::GenerateDifferenceTexture( 
		D3DXVECTOR3 & world_min, D3DXVECTOR3 & world_max, 
		unsigned int tex_width, unsigned int tex_height, 
		IMathFunction * noise_function, bool normalized )
{
	float * data = new float [ tex_width * tex_height ];
	float scalex = ( world_max.x - world_min.x ) / tex_width;
	float scaley = ( world_max.y - world_min.y ) / tex_height;
	float max_diff_refernce = (float)pow( (world_max.x-world_min.x)+(world_max.y-world_min.y), 2 );
	float max_diff = 0.f;
	float min_dist[2];
	float diff_mag_squared;
	unsigned int index;
	D3DXVECTOR3 diff;

	NoiseCombiner * NFunc;
	if( noise_function ) NFunc = (NoiseCombiner*)noise_function;

	for( unsigned int x = 0; x < tex_width; ++x )
	{
		for( unsigned int y = 0; y < tex_height; ++y )
		{
			D3DXVECTOR3 world( x * scalex + world_min.x, y * scaley + world_min.y, 0.f );
			min_dist[0] = min_dist[1] = max_diff_refernce;
			if( noise_function )
			{
				world.x += (*NFunc)(x,y,0)*0.3f;
				world.y += (*NFunc)(x,y,1)*0.3f;
			}

			for( unsigned int i = 0; i < num_sites; ++i )
			{
				diff = world - VSites[i].site;
				diff_mag_squared = diff.x*diff.x+diff.y*diff.y;

				if( diff_mag_squared < min_dist[0] )
				{
					min_dist[1] = min_dist[0];
					min_dist[0] = diff_mag_squared;
				}else if( diff_mag_squared < min_dist[1] )
				{
					min_dist[1] = diff_mag_squared;
				}							
			}
			
			index = x * tex_width + y;
			data[index] = float(sqrt( min_dist[1] ) - sqrt( min_dist[0] ) + 0.01f * sinf( 40.f * 3.14159265f * x / 512.f ));
			if( data[index] > max_diff )
			{
				max_diff = data[index];
			}
		}
	} 
	
	if( normalized )
	{
		for( unsigned int x = 0; x < tex_width; ++x )
		{
			for( unsigned int y = 0; y < tex_height; ++y )
			{
				index = x * tex_width + y;
				data[index] /= max_diff;
			}
		}
	}

	return data;
}


float * Voronoi::GenerateBinaryTexture(
	D3DXVECTOR3 & world_min, D3DXVECTOR3 & world_max, 
	unsigned int tex_width, unsigned int tex_height, 
	IMathFunction * noise_function )
{
	float * data = new float[tex_width*tex_height];
	bool * site_on = new bool[ num_sites ];
	for( unsigned int i = 0; i < num_sites; ++i )
	{
		if( rand()%5 == 0 ) site_on[i] = true; else site_on[i] = false;
	}
	//NoiseCombiner * nf = (NoiseCombiner*)noise_function;
	for( unsigned int x = 0; x < tex_width; ++x )
	{
		for( unsigned int y = 0; y < tex_height; ++y )
		{
			float close_dist = 1000000.f;
			unsigned int close_index = 0;
			
			//float xoffset = (*nf)(x, y, 0 ) * 0.1f;
			//float yoffset = (*nf)(x, y, 1 ) * 0.1f;
			D3DXVECTOR3 world_point( 
				( x / float(tex_width) ) * ( world_max.x - world_min.x ) + world_min.x, 
				( y / float(tex_height) ) * ( world_max.y - world_min.y ) + world_min.y, 
				0.f );
			
			for( unsigned int i = 0; i < num_sites; ++i )
			{
				D3DXVECTOR3 diff = VSites[i].site - world_point;
				float tdist = diff.x*diff.x+diff.y*diff.y;
				if( tdist < close_dist ) 
				{
					close_dist = tdist;
					close_index = i;
				}
			}
			if( site_on[close_index] == true )
			{
				data[x*tex_width+y] = 1.f;
			}else
			{
				data[x*tex_width+y] = 0.f;
			}
		}
	}

	delete [] site_on;
	return data;
}
