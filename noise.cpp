//#include <stdlib.h>
//#include <math.h>
//#include "noise.h"
//#include "DirectX\VertexTypes.h"
//#include <stdio.h>
//
//#define FRAND(min,max) ( ( ( rand()/float(RAND_MAX) ) *((max)-(min)) ) + (min) )
//
//void Noise::GeneratePerlinNoise(int seed, int width, int height, int octaves, float persistence, int zoom, int x_offset, int y_offset )
//{
//	if( data != 0 ) 
//	{
//		delete data;
//		data = 0;
//	}
//	data = PerlinNoise( seed, width, height, octaves, persistence, zoom, x_offset, y_offset );
//	min_value = -1.f;
//	max_value = 1.f;
//}
//
//void Noise::GenerateNoise( LPDIRECT3DDEVICE9 d3ddev )
//{
//	srand( GetTickCount() );
//	//float initial_noise = 1.f;
//	//data[0] = FRAND(-initial_noise,initial_noise);
//	//data[NOISE_WIDTH] = FRAND(-initial_noise,initial_noise);
//	//data[NOISE_WIDTH*NOISE_WIDTH] = FRAND(-initial_noise,initial_noise);
//	//data[NOISE_WIDTH*NOISE_WIDTH+NOISE_WIDTH] = FRAND(-initial_noise,initial_noise);
//	//
//	//NoiseLevel( NOISE_POWER, initial_noise );
//}
//
//void Noise::DirectScale( float scale)
//{
//	//min_value *= scale;
//	//max_value *= scale;
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned index = x * NOISE_WIDTH + y;
//			data[index] *= scale;
//		}
//	}
//}
//
//void Noise::Scale( float scale_min, float scale_max )
//{
//	CalculateMinMax();
//	float scalar = scale_max - scale_min;
//	float min_max_range = max_value - min_value;
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned index = x * NOISE_WIDTH + y;
//			data[index] = ( ( data[index] - min_value ) * scalar / min_max_range ) + scale_min;
//		}
//	}
//	min_value = scale_min;
//	max_value = scale_max;
//}
//
//void Noise::CalculateMinMax( void )
//{
//	min_value = max_value = data[0];
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned index = x * NOISE_WIDTH + y;
//			if( data[index] < min_value ) min_value = data[index];
//			if( data[index] > max_value ) max_value = data[index];
//		}
//	}
//}
//
//void Noise::Truncate( float min_cap, float max_cap )
//{
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned index = x * NOISE_WIDTH + y;
//			data[index] = min( max( data[index], min_cap ), max_cap );
//		}
//	}
//
//	min_value = min_cap;
//	max_value = max_cap;
//}
//
//void Noise::GenerateSteps( unsigned int num_steps )
//{
//	float step_size = (max_value-min_value) / float( num_steps );
//	float half_step = step_size / 2.f;
//
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned index = x * NOISE_WIDTH + y;
//			data[index] = int( ( data[index]  ) / step_size ) * step_size;
//		}
//	}
//}
//
//void Noise::CalculateBlockData( void )
//{
//	if( block_data != 0 ) delete [] block_data;
//	block_data = new float[ blocks_across * blocks_across ];
//
//	unsigned int iblock_size = unsigned int( block_size );
//
//	for( unsigned int x = 0; x < blocks_across; ++x )
//	{
//		for( unsigned int y = 0; y < blocks_across; ++y )
//		{
//			block_data[x*blocks_across+y] = data[x*iblock_size*NOISE_WIDTH+y*iblock_size];
//		}
//	}
//}
//
//
//void Noise::AddQuad( D3DXVECTOR3 & tl, D3DXVECTOR3 & bl, D3DXVECTOR3 & br, D3DXVECTOR3 & tr, Vertex * vert_data, unsigned int & vert_count )
//{
//	DWORD color;// = D3DCOLOR_XRGB( rand()%255, 100, 10 );
//	unsigned int val = int(tl.y*40);///5.f);
//	unsigned int val2 = sinf(tl.y)*100.f;
//	unsigned int val3 = cosf(tl.y)*100.f;
//	val = 255 * tl.y / 10.f;
//	color = D3DCOLOR_XRGB( val,val,val);
//	if( fabs(tl.y-br.y) < 0.0001f ) color = D3DCOLOR_XRGB( 30, val, 50 );
//	if( tl.y < 10.f * 0.1953125 ) color = D3DCOLOR_XRGB( val, val, 250 );
//
//
//	vert_data[vert_count].x = tl.x;
//	vert_data[vert_count].y = tl.y;
//	vert_data[vert_count].z = tl.z;
//	vert_data[vert_count].color = color;
//	++vert_count;
//
//	vert_data[vert_count].x = br.x;
//	vert_data[vert_count].y = br.y;
//	vert_data[vert_count].z = br.z;
//	vert_data[vert_count].color = color;
//	++vert_count;
//
//	vert_data[vert_count].x = bl.x;
//	vert_data[vert_count].y = bl.y;
//	vert_data[vert_count].z = bl.z;
//	vert_data[vert_count].color = color;
//	++vert_count;
//
//	vert_data[vert_count].x = tl.x;
//	vert_data[vert_count].y = tl.y;
//	vert_data[vert_count].z = tl.z;
//	vert_data[vert_count].color = color;
//	++vert_count;
//
//	vert_data[vert_count].x = tr.x;
//	vert_data[vert_count].y = tr.y;
//	vert_data[vert_count].z = tr.z;
//	vert_data[vert_count].color = color;
//	++vert_count;
//
//	vert_data[vert_count].x = br.x;
//	vert_data[vert_count].y = br.y;
//	vert_data[vert_count].z = br.z;
//	vert_data[vert_count].color = color;
//	++vert_count;
//}
//
////////////////////////////////////////////////////////
//// TRADITIONAL HEIGHTMAP APPROACH
////////////////////////////////////////////////////////
////LPDIRECT3DVERTEXBUFFER9 Noise::GenerateVertexBuffer(  
////		LPDIRECT3DDEVICE9 d3ddev, 
////		float min_width, float min_height, float min_depth, 
////		float max_width, float max_height, float max_depth, 
////		unsigned int & total_vert_count )
////{
////	if( block_data == 0 ) CalculateBlockData();
////	total_vert_count = (blocks_across-1)*(blocks_across-1)*6;
////
////	LPDIRECT3DVERTEXBUFFER9 out_verts;
////	d3ddev->CreateVertexBuffer( 
////		total_vert_count * sizeof( Vertex ), 
////		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
////		Vertex::VertexFVF,
////		D3DPOOL_DEFAULT,
////		&out_verts,
////		0 );
////
////	Vertex * vert_data;
////	HRESULT err = out_verts->Lock( 0, total_vert_count * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );
////
////	unsigned int count = 0;
////
////	for( unsigned int x = 0; x < blocks_across-1; ++x )
////	{
////		for( unsigned int y = 0; y < blocks_across-1; ++y )
////		{
////			//DWORD color = D3DCOLOR_XRGB( sin(block_data[x*blocks_across+y])*255, cos(block_data[x*blocks_across+y])*255, sin(block_data[x*blocks_across+y])*255 );
////			
////			vert_data[count].x = (max_width-min_width) * x / blocks_across + min_width;
////			vert_data[count].y = (max_height-min_height) * block_data[x*blocks_across+y] / max_value + min_height;
////			vert_data[count].z = (max_depth-min_depth) * y / blocks_across + min_depth;
////			++count;
////
////			vert_data[count].x = (max_width-min_width) * (x+1) / blocks_across + min_width;
////			vert_data[count].y = (max_height-min_height) * block_data[(x+1)*blocks_across+y+1] / max_value + min_height;
////			vert_data[count].z = (max_depth-min_depth) * (y+1) / blocks_across + min_depth;
////			++count;
////
////			vert_data[count].x = (max_width-min_width) * x / blocks_across + min_width;
////			vert_data[count].y = (max_height-min_height) * block_data[x*blocks_across+y+1] / max_value + min_height;
////			vert_data[count].z = (max_depth-min_depth) * (y+1) / blocks_across + min_depth;
////			++count;
////
////			
////			vert_data[count].x = (max_width-min_width) * x / blocks_across + min_width;
////			vert_data[count].y = (max_height-min_height) * block_data[x*blocks_across+y] / max_value + min_height;
////			vert_data[count].z = (max_depth-min_depth) * y / blocks_across + min_depth;
////			++count;
////
////			vert_data[count].x = (max_width-min_width) * (x+1) / blocks_across + min_width;
////			vert_data[count].y = (max_height-min_height) * block_data[(x+1)*blocks_across+y] / max_value + min_height;
////			vert_data[count].z = (max_depth-min_depth) * (y) / blocks_across + min_depth;
////			++count;
////
////			vert_data[count].x = (max_width-min_width) * (x+1) / blocks_across + min_width;
////			vert_data[count].y = (max_height-min_height) * block_data[(x+1)*blocks_across+y+1] / max_value + min_height;
////			vert_data[count].z = (max_depth-min_depth) * (y+1) / blocks_across + min_depth;
////			++count;
////		}
////	}
////
////	for( unsigned int i = 0; i < count; ++i )
////	{
////		float pival = (vert_data[i].y-min_height)/(max_height-min_height)*2.f*3.14159265f;
////		vert_data[i].color = D3DCOLOR_XRGB( int(sin(pival)*255), int(cos(pival)*255), int( ( cos(pival)*sin(pival)/2.f+0.5)*255 ) );
////	}
////
////	out_verts->Unlock();
////	
////	return out_verts;
////}
//
//
////////////////////////////////////////////////////////
//// CUBIC HEIGHTMAP APPROACH
////////////////////////////////////////////////////////
//LPDIRECT3DVERTEXBUFFER9 Noise::GenerateVertexBuffer(  
//		LPDIRECT3DDEVICE9 d3ddev, 
//		unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height,
//		float min_width, float min_height, float min_depth, 
//		float max_width, float max_height, float max_depth, 
//		unsigned int & total_vert_count )
//{
//	if( block_data == 0 ) CalculateBlockData();
//	
//	float xscale = ( max_width - min_width ) / blocks_across;
//	float yscale = ( max_height - min_height ) / max_value;
//	float zscale = ( max_depth - min_depth ) / blocks_across;
//	printf("blocks_across: %i\nmax_value: %f\n",blocks_across, max_value );
//	
//	unsigned int quad_count = 0;
//	for( unsigned int x = start_x; x < start_x+width; ++x )
//	{
//		for( unsigned int y = start_y; y < start_y+height; ++y )
//		{
//			unsigned int index = x * blocks_across + y;
//			
//			++quad_count;
//			if( x == 0 )
//			{
//				if( block_data[index] > 0.f ) ++quad_count;
//				if( block_data[index+blocks_across] < block_data[index] ) ++quad_count;
//			}else if( x == blocks_across-1 )
//			{
//				if( block_data[index] > 0.f ) ++quad_count;
//				if( block_data[index-blocks_across] < block_data[index] ) ++quad_count;
//			}else
//			{
//				if( block_data[index+blocks_across] < block_data[index] ) ++quad_count;
//				if( block_data[index-blocks_across] < block_data[index] ) ++quad_count;
//			}
//			
//			if( y == 0 )
//			{
//				if( block_data[index] > 0.f ) ++quad_count;
//				if( block_data[index+1] < block_data[index] ) ++quad_count;
//			}else if( y == blocks_across-1 )
//			{
//				if( block_data[index] > 0.f ) ++quad_count;
//				if( block_data[index-1] < block_data[index] ) ++quad_count;
//			}else
//			{
//				if( block_data[index+1] < block_data[index] ) ++quad_count;
//				if( block_data[index-1] < block_data[index] ) ++quad_count;
//			}
//		}
//	}
//
//	unsigned int tri_count = quad_count * 2;
//	unsigned int vert_count = tri_count * 3;
//	unsigned int count = 0;
//	
//	LPDIRECT3DVERTEXBUFFER9 out_verts;
//	d3ddev->CreateVertexBuffer( 
//		vert_count * sizeof( Vertex ), 
//		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
//		Vertex::VertexFVF,
//		D3DPOOL_DEFAULT,
//		&out_verts,
//		0 );
//
//	Vertex * tri_data;
//	HRESULT err = out_verts->Lock( 0, vert_count * sizeof( Vertex ), (void**)&tri_data, D3DLOCK_DISCARD );
//
//	for( unsigned int x = start_x; x < start_x + width; ++x )
//	{
//		for( unsigned int y = start_y; y < start_y + height; ++y )
//		{
//			unsigned int index = x * blocks_across + y;
//			
//			AddQuad( 
//				D3DXVECTOR3( x*xscale, block_data[index]*yscale, y*zscale ),
//				D3DXVECTOR3( x*xscale, block_data[index]*yscale, (y+1)*zscale ),
//				D3DXVECTOR3( (x+1)*xscale, block_data[index]*yscale, (y+1)*zscale ),
//				D3DXVECTOR3( (x+1)*xscale, block_data[index]*yscale, y*zscale ), 
//				tri_data, count );
//
//			if( x == 0 )
//			{
//				if( block_data[index] > 0.f ) 
//				{
//					AddQuad( 
//						D3DXVECTOR3( 0.f, block_data[index]*yscale, y*zscale ),
//						D3DXVECTOR3( 0.f, 0.f, y*zscale ),
//						D3DXVECTOR3( 0.f, 0.f, (y+1)*zscale ),
//						D3DXVECTOR3( 0.f, block_data[index]*yscale, (y+1)*zscale ), 
//						tri_data, count );
//				}
//				if( block_data[index+blocks_across] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( xscale, block_data[index]*yscale, (y+1)*zscale ),
//						D3DXVECTOR3( xscale, ( block_data[index+blocks_across] ) * yscale, (y+1)*zscale ), 
//						D3DXVECTOR3( xscale, ( block_data[index+blocks_across] ) * yscale, y*zscale ),
//						D3DXVECTOR3( xscale, block_data[index]*yscale, y*zscale ),
//						tri_data, count );
//				}
//			}else if( x == blocks_across-1 )
//			{
//				if( block_data[index] > 0.f )
//				{
//					AddQuad( 
//						D3DXVECTOR3( blocks_across*xscale, block_data[index]*yscale, (y+1)*zscale ),
//						D3DXVECTOR3( blocks_across*xscale, 0.f, (y+1)*zscale ), 
//						D3DXVECTOR3( blocks_across*xscale, 0.f, y*zscale ),
//						D3DXVECTOR3( blocks_across*xscale, block_data[index]*yscale, y*zscale ),
//						tri_data, count );
//				}
//				if( block_data[index-blocks_across] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( x*xscale, block_data[index]*yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index-blocks_across] ) * yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index-blocks_across] ) * yscale, (y+1)*zscale ),
//						D3DXVECTOR3( x*xscale, block_data[index]*yscale, (y+1)*zscale ), 
//						tri_data, count );
//				}
//			}else
//			{
//				if( block_data[index+blocks_across] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( (x+1)*xscale, block_data[index]*yscale, (y+1)*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, ( block_data[index+blocks_across] ) * yscale, (y+1)*zscale ), 
//						D3DXVECTOR3( (x+1)*xscale, ( block_data[index+blocks_across] ) * yscale, y*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, block_data[index]*yscale, y*zscale ),
//						tri_data, count );
//				}
//				if( block_data[index-blocks_across] < block_data[index] ) 
//				{
//					AddQuad( 
//						D3DXVECTOR3( x*xscale, block_data[index]*yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index-blocks_across] ) * yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index-blocks_across] ) * yscale, (y+1)*zscale ),
//						D3DXVECTOR3( x*xscale, block_data[index]*yscale, (y+1)*zscale ), 
//						tri_data, count );
//				}
//			}
//			
//			if( y == 0 )
//			{
//				if( block_data[index] > 0.f )
//				{
//					AddQuad( 
//						D3DXVECTOR3( (x+1)*xscale, block_data[index] * yscale, 0.f ),
//						D3DXVECTOR3( (x+1)*xscale, 0.f, 0.f ),
//						D3DXVECTOR3( x*xscale, 0.f, 0.f ),
//						D3DXVECTOR3( x*xscale, block_data[index] * yscale, 0.f ), 
//						tri_data, count );
//				}
//				if( block_data[index+1] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( x*xscale, block_data[index] * yscale, zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index+1] ) * yscale, zscale ),
//						D3DXVECTOR3( (x+1)*xscale, ( block_data[index+1] ) * yscale, zscale ),
//						D3DXVECTOR3( (x+1)*xscale, block_data[index] * yscale, zscale ), 
//						tri_data, count );
//				}
//			}else if( y == blocks_across-1 )
//			{
//				if( block_data[index] > 0.f )
//				{
//					AddQuad( 
//						D3DXVECTOR3( x*xscale, block_data[index] * yscale, blocks_across*zscale ),
//						D3DXVECTOR3( x*xscale, 0.f, blocks_across*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, 0.f, blocks_across*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, block_data[index] * yscale, blocks_across*zscale ), 
//						tri_data, count );
//				}
//				if( block_data[index-1] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( (x+1)*xscale, block_data[index] * yscale, y*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, ( block_data[index-1] ) * yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index-1] ) * yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, block_data[index] * yscale, y*zscale ), 
//						tri_data, count );
//				}
//			}else
//			{
//				if( block_data[index+1] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( x*xscale, block_data[index] * yscale, (y+1)*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index+1] ) * yscale, (y+1)*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, ( block_data[index+1] ) * yscale, (y+1)*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, block_data[index] * yscale, (y+1)*zscale ), 
//						tri_data, count );
//				}
//				if( block_data[index-1] < block_data[index] )
//				{
//					AddQuad( 
//						D3DXVECTOR3( (x+1)*xscale, block_data[index] * yscale, y*zscale ),
//						D3DXVECTOR3( (x+1)*xscale, ( block_data[index-1] ) * yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, ( block_data[index-1] ) * yscale, y*zscale ),
//						D3DXVECTOR3( x*xscale, block_data[index] * yscale, y*zscale ), 
//						tri_data, count );
//				}
//			}
//		}
//	}
//	
//	for( unsigned int i = 0; i < count; ++i )
//	{
//		tri_data[i].x += min_width;
//		tri_data[i].y += min_height;//50.0f;
//		tri_data[i].z += min_depth;
//	}
//
//
//	out_verts->Unlock();
//
//	if( vert_count != count ) printf("ERROR: VERT_COUNT (%i) != COUNT (%i)\n", vert_count, count );
//	printf("NOISE VERT COUNT: %i\n", vert_count );
//
//	total_vert_count = count;
//	return out_verts;
//}
//
//void Noise::Pixelate( float block_width )
//{
//	block_size = block_width;
//	blocks_across = NOISE_WIDTH / block_size;
//
//	unsigned int iblock_size = int(block_size);
//	unsigned int iblock_size_squared = iblock_size * iblock_size;
//	if( NOISE_WIDTH % iblock_size != 0 ) return;
//
//	for( unsigned int x = 0; x < NOISE_WIDTH; x += iblock_size )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; y += iblock_size )
//		{
//			float average = 0;
//			for( unsigned int xo = x; xo < x + iblock_size; ++xo )
//			{
//				for( unsigned int yo = y; yo < y + iblock_size; ++yo )
//				{
//					average += data[xo*NOISE_WIDTH+yo];
//				}
//			}
//			average /= iblock_size_squared;
//			for( unsigned int xo = x; xo < x + iblock_size; ++xo )
//			{
//				for( unsigned int yo = y; yo < y + iblock_size; ++yo )
//				{
//					unsigned int index = xo*NOISE_WIDTH+yo;
//					data[xo*NOISE_WIDTH+yo] = average;
//				}
//			}
//		}
//	}
//}
//
//void Noise::GenerateTexture( LPDIRECT3DDEVICE9 d3ddev )
//{
//	if( verts != 0 )
//	{
//		verts->Release();
//		verts = 0;
//	}else
//	{
//		d3ddev->CreateVertexBuffer( 
//			6 * sizeof( Vertex ), 
//			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
//			Vertex::VertexFVF,
//			D3DPOOL_DEFAULT,
//			&verts,
//			0 );
//
//		Vertex * vert_data;
//		HRESULT err = verts->Lock( 0, 6 * sizeof( Vertex ), (void**)&vert_data, D3DLOCK_DISCARD );
//
//		vert_data[0].x = -1.f;
//		vert_data[0].y = 1.f;
//		vert_data[0].z = 0.f;
//		vert_data[0].u = 0.f;
//		vert_data[0].v = 1.f;
//
//		vert_data[1].x = -1.f;
//		vert_data[1].y = -1.f;
//		vert_data[1].z = 0.f;
//		vert_data[1].u = 0.f;
//		vert_data[1].v = 0.f;
//
//		vert_data[2].x = 1.f;
//		vert_data[2].y = 1.f;
//		vert_data[2].z = 0.f;
//		vert_data[2].u = 1.f;
//		vert_data[2].v = 1.f;
//
//		vert_data[3].x = 1.f;
//		vert_data[3].y = 1.f;
//		vert_data[3].z = 0.f;
//		vert_data[3].u = 1.f;
//		vert_data[3].v = 1.f;
//
//		vert_data[4].x = -1.f;
//		vert_data[4].y = -1.f;
//		vert_data[4].z = 0.f;
//		vert_data[4].u = 0.f;
//		vert_data[4].v = 0.f;
//
//		vert_data[5].x = 1.f;
//		vert_data[5].y = -1.f;
//		vert_data[5].z = 0.f;
//		vert_data[5].u = 1.f;
//		vert_data[5].v = 0.f;
//
//		verts->Unlock();
//	}
//
//	float max_val = data[0];
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned int i = x * NOISE_WIDTH + y;
//			if( data[i] > max_val ) max_val = data[i];
//		}
//	}
//
//	printf("tex: %x\n", texture );
//	if( texture != 0 ) 
//	{
//		texture->Release();
//		texture = 0;
//	}
//	if( texture == 0 ) D3DXCreateTexture( d3ddev, NOISE_WIDTH, NOISE_WIDTH, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture );
//	printf("tex: %x\n", texture );
//	D3DLOCKED_RECT tex_rect;
//	texture->LockRect( 0, &tex_rect, 0, D3DLOCK_DISCARD );
//	
//	TexColor * color_data = (TexColor*)tex_rect.pBits;
//	for( unsigned int x = 0; x < NOISE_WIDTH; ++x )
//	{
//		for( unsigned int y = 0; y < NOISE_WIDTH; ++y )
//		{
//			unsigned int i = x * NOISE_WIDTH + y;
//			color_data[i].r = color_data[i].g = color_data[i].b = 255.f * data[i] / max_val;
//			if( data_extra[i] ) 
//			{
//				color_data[i].r = 255;
//				color_data[i].g = 10;
//			}
//		}
//	}
//
//	texture->UnlockRect( 0 );
//	//texture->GenerateMipSubLevels();
//}
//
//
//void Noise::NoiseLevel( unsigned int level, float offset )
//{
//	if( level == 0 ) return;
//
//	int cell_size = 1<<level;
//	int half_size = cell_size >> 1;
//	int num_cells = 1<<(NOISE_POWER-level);
//	
//	//midpoint
//	for( unsigned int x = 0; x < num_cells; ++x )
//	{
//		for( unsigned int y = 0; y < num_cells; ++y )
//		{
//			int sx = x * cell_size;
//			int ex = sx + cell_size;
//			int mx = sx + half_size;
//			int sy = y * cell_size;
//			int ey = sy + cell_size;
//			int my = sy + half_size;
//
//			data[mx*NOISE_WIDTH+my] = ( 
//				data[sx*NOISE_WIDTH+sy] + 
//				data[sx*NOISE_WIDTH+ey] + 
//				data[ex*NOISE_WIDTH+sy] + 
//				data[ex*NOISE_WIDTH+ey] ) / 4.f + FRAND(-offset,offset);
//		}
//	}
//
//	//diamond (edge midpoints)
//	for( unsigned int x = 0; x < num_cells; ++x )
//	{
//		for( unsigned int y = 0; y < num_cells; ++y )
//		{
//			int sx = x * cell_size;
//			int ex = sx + cell_size;
//			int mx = sx + half_size;
//			int sy = y * cell_size;
//			int ey = sy + cell_size;
//			int my = sy + half_size;
//
//			data[mx*NOISE_WIDTH+sy] = data[sx*NOISE_WIDTH+sy];
//			data[mx*NOISE_WIDTH+sy] += data[ex*NOISE_WIDTH+sy];
//			data[mx*NOISE_WIDTH+sy] += data[mx*NOISE_WIDTH+my];
//			if( y == 0 )
//			{
//				data[mx*NOISE_WIDTH+sy] /= 3.f;
//				data[mx*NOISE_WIDTH+sy] += FRAND(-offset,offset);
//			}else
//			{
//				data[mx*NOISE_WIDTH+sy] += data[mx*NOISE_WIDTH+sy-half_size];
//				data[mx*NOISE_WIDTH+sy] /= 4.f;
//				data[mx*NOISE_WIDTH+sy] += FRAND(-offset,offset);
//			}
//
//			data[sx*NOISE_WIDTH+my] = data[sx*NOISE_WIDTH+sy];
//			data[sx*NOISE_WIDTH+my] += data[sx*NOISE_WIDTH+ey];
//			data[sx*NOISE_WIDTH+my] += data[mx*NOISE_WIDTH+my];
//			if( x == 0 )
//			{
//				data[sx*NOISE_WIDTH+my] /= 3.f;
//				data[sx*NOISE_WIDTH+my] += FRAND(-offset,offset);
//			}else
//			{
//				data[sx*NOISE_WIDTH+my] += data[(sx-half_size)*NOISE_WIDTH+my];
//				data[sx*NOISE_WIDTH+my] /= 4.f;
//				data[sx*NOISE_WIDTH+my] += FRAND(-offset,offset);
//			}
//
//			data[mx*NOISE_WIDTH+ey] = data[sx*NOISE_WIDTH+ey];
//			data[mx*NOISE_WIDTH+ey] += data[ex*NOISE_WIDTH+ey];
//			data[mx*NOISE_WIDTH+ey] += data[mx*NOISE_WIDTH+my];
//			if( ey == NOISE_WIDTH )
//			{
//				data[mx*NOISE_WIDTH+ey] /= 3.f;
//				data[mx*NOISE_WIDTH+ey] += FRAND(-offset,offset);
//			}else
//			{
//				data[mx*NOISE_WIDTH+ey] += data[mx*NOISE_WIDTH+sy+half_size];
//				data[mx*NOISE_WIDTH+ey] /= 4.f;
//				data[mx*NOISE_WIDTH+ey] += FRAND(-offset,offset);
//			}
//
//			data[ex*NOISE_WIDTH+my] = data[ex*NOISE_WIDTH+sy];
//			data[ex*NOISE_WIDTH+my] += data[ex*NOISE_WIDTH+ey];
//			data[ex*NOISE_WIDTH+my] += data[mx*NOISE_WIDTH+my];
//			if( ex == NOISE_WIDTH )
//			{
//				data[ex*NOISE_WIDTH+my] /= 3.f;
//				data[ex*NOISE_WIDTH+my] += FRAND(-offset,offset);
//			}else
//			{
//				data[ex*NOISE_WIDTH+my] += data[(sx+half_size)*NOISE_WIDTH+my];
//				data[ex*NOISE_WIDTH+my] /= 4.f;
//				data[ex*NOISE_WIDTH+my] += FRAND(-offset,offset);
//			}
//		}
//	}
//
//	NoiseLevel( level-1, offset * 0.7f );
//}
//
//// "random" number between -1.0f and 1.0f
//float random(int seed, int x, int y, int z)
//{
//	int n = seed + x*73 + y*179 + z*283;
//	n = (n << 13) ^ n;
//	int m = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
//	return 1.0f - ((float)m / 1073741824.0f);
//}
//	 
//// cosine interpolation between a and b by a factor of x
//float lerp(float a, float b, float x)
//{
//	float ft = x * 3.1415927f;
//	float f = (1.0f - cosf(ft)) * 0.5f;
//	return a * (1.0f - f) + b * f;
//}
//
//float * PerlinNoise(int seed, int width, int height, int octaves, float persistence, int zoom, int x_offset, int y_offset )
//{
//	float x, y;
//	int floorx, floory;
//	float na, nb, nc, nd;
//	float la, lb, lc;
//	float frequency, amplitude;
//	 
//	float * density = new float[ width * height ];
//	 
//	for (int i = 0; i < width; ++i )
//	{
//	    for (int j = 0; j < height; ++j )
//	    {
//	        for (int o = 0; o <= octaves; o++)
//	        {
//				int xused = i + x_offset;
//				int yused = j + y_offset;
//
//	            frequency = powf( 2, o );
//	            amplitude = powf( persistence, o );
//	 
//	            x = ((float)(xused)) * frequency / zoom;
//	            y = ((float)(yused)) * frequency / zoom;
//	            floorx = (int)x;
//	            floory = (int)y;
//
//	            na = random(seed, floorx, floory, 0);
//	            nb = random(seed, floorx + 1, floory, 0);
//	            nc = random(seed, floorx, floory + 1, 0);
//	            nd = random(seed, floorx + 1, floory + 1, 0);
//	            la = lerp(na, nb, x - floorx);
//	            lb = lerp(nc, nd, x - floorx);
//	            lc = lerp(la, lb, y - floory);
//	 
//	            density[ i * width + j ] += lc * amplitude;
//	        }
//	    }
//	}
//	 
//	return density;
//}
//
///*
//	// 2d perlin noise function
//02	public float[,] perlinnoise(int seed, int width, int height, int octaves, float persistence, int zoom)
//03	{
//04	    float x, y;
//05	    int floorx, floory;
//06	    float na, nb, nc, nd;
//07	    float la, lb, lc;
//08	    float frequency, amplitude;
//09	 
//10	    float[,] density = new float[width, height];
//11	 
//12	    for (int j = 0; j < height; j++)
//13	    {
//14	        for (int i = 0; i < width; i++)
//15	        {
//16	            for (int o = 0; o <= octaves; o++)
//17	            {
//18	                frequency = (float)Math.Pow(2, o);
//19	                amplitude = (float)Math.Pow(persistence, o);
//20	 
//21	                x = ((float)i) * frequency / zoom;
//22	                y = ((float)j) / zoom * frequency;
//23	                floorx = (int)x;
//24	                floory = (int)y;
//25	                na = random(seed, floorx, floory, 0);
//26	                nb = random(seed, floorx + 1, floory, 0);
//27	                nc = random(seed, floorx, floory + 1, 0);
//28	                nd = random(seed, floorx + 1, floory + 1, 0);
//29	                la = lerp(na, nb, x - floorx);
//30	                lb = lerp(nc, nd, x - floorx);
//31	                lc = lerp(la, lb, y - floory);
//32	 
//33	                density[i, j] += lc * amplitude;
//34	            }
//35	        }
//36	    }
//37	 
//38	    return density;
//39	}
//40	 
//41	// "random" number between -1.0f and 1.0f
//42	public float random(int seed, int x, int y, int z)
//43	{
//44	    int n = seed + x*73 + y*179 + z*283;
//45	    n = (n << 13) ^ n;
//46	    int m = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
//47	    return 1.0f - ((float)m / 1073741824.0f);
//48	}
//49	 
//50	// cosine interpolation between a and b by a factor of x
//51	public float lerp(float a, float b, float x)
//52	{
//53	    float ft = x * 3.1415927f;
//54	    float f = (1.0f - (float)Math.Cos(ft)) * 0.5f;
//55	    return a * (1.0f - f) + b * f;
//56	}
//*/
//
//
//unsigned char * GenerateStandardNoise( int seed, unsigned int xoffset, unsigned int yoffset )
//{
//	Noise noise[2];
//	
//	noise[0].Initilize();
//	noise[0].GeneratePerlinNoise( seed, 512, 512, 8, 0.5f, 128, xoffset, yoffset );
//
//	for( unsigned int x = 0; x < 512; ++x )
//	{
//		for( unsigned int y = 0; y < 512; ++y )
//		{
//			unsigned int index = x*512+y;
//			
//			//HILLS
//			noise[0].data[index] = abs( noise[0].data[index] );
//			noise[0].data[index] = max( 0.f, noise[0].data[index] - 0.6f );
//			noise[0].data[index] *= 0.01f;
//		}
//	}
//
//	noise[0].min_value = 0.f;
//	noise[0].max_value = 512.f;
//	noise[0].DirectScale( 256.f );
//
//	noise[1].Initilize();
//	noise[1].GeneratePerlinNoise( seed, 512, 512, 8, 0.5f, 128, xoffset, yoffset );
//	noise[1].Scale( 0.f, 512.f );
//	noise[1].DirectScale( 0.05f );
//
//	for( unsigned int x = 0; x < 512; ++x )
//	{
//		for( unsigned int y = 0; y < 512; ++y )
//		{
//			unsigned int index = x*512+y;
//			noise[0].data[index] += noise[1].data[index];
//		}
//	}
//	
//	noise[0].Pixelate( 2.f );
//	noise[0].Pixelate( 2.f );
//	noise[0].GenerateSteps( 256 );
//	noise[0].CalculateBlockData();
//
//	unsigned char * out = new unsigned char[256*256];
//	for( unsigned int i = 0; i < 256*256; ++i )
//	{
//		out[i] = noise[0].block_data[i];
//	}
//	return out;
//}
