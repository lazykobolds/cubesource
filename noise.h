//#pragma once
//
//#include <d3dx9.h>
//#include "DirectX\VertexTypes.h"
//
//#define NOISE_POWER 9
//#define NOISE_WIDTH (1<<NOISE_POWER)
//
//struct TexColor
//{
//	char r;
//	char g;
//	char b;
//	char a;
//};
//
//float random(int seed, int x, int y, int z);
//// cosine interpolation between a and b by a factor of x
//float lerp(float a, float b, float x);
//float * PerlinNoise(int seed, int width, int height, int octaves, float persistence, int zoom, int x_offset = 0, int y_offset = 0 );
//
//class Noise
//{
//public:
//	
//	Noise( void ) 
//	{ 
//		Initilize();
//	}		
//
//	~Noise( void )
//	{
//		if( texture ) texture->Release();
//		if( verts ) verts->Release();
//		if( block_data ) delete [] block_data;
//		if( data ) delete [] data;
//		if( data_extra ) delete [] data_extra;
//	}
//
//	void Initilize( void )
//	{
//		texture = 0; 
//		verts = 0; 
//		block_data = 0; 
//		data = new float[(NOISE_WIDTH+1)*(NOISE_WIDTH+1)];
//		data_extra = new bool[(NOISE_WIDTH+1)*(NOISE_WIDTH+1)];
//		blocks_across = NOISE_WIDTH;
//		block_size = 1;
//	}
//
//	void GenerateNoise( LPDIRECT3DDEVICE9 d3ddev );
//	void GeneratePerlinNoise(int seed, int width, int height, int octaves, float persistence, int zoom, int x_offset = 0, int y_offset = 0 );
//
//	void Pixelate( float block_width );
//	void GenerateTexture( LPDIRECT3DDEVICE9 d3ddev );
//
//	void NoiseLevel( unsigned int level, float offset );
//	void CalculateMinMax( void );
//	void Scale( float scale_min, float scale_max );
//	void DirectScale( float scale );
//	void Truncate( float min_cap, float max_cap );
//	void GenerateSteps( unsigned int num_steps );
//	void CalculateBlockData( void );
//
//    void AddQuad( D3DXVECTOR3 & tl, D3DXVECTOR3 & bl, D3DXVECTOR3 & br, D3DXVECTOR3 & tr, Vertex * vert_data, unsigned int & vert_count );
//
//	LPDIRECT3DVERTEXBUFFER9 GenerateVertexBuffer( 
//		LPDIRECT3DDEVICE9 d3ddev, 
//		unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height,
//		float min_width, float min_height, float min_depth, 
//		float max_width, float max_height, float max_depth, 
//		unsigned int & total_vert_count );
//
//	float * data;
//	bool * data_extra;
//	float * block_data;
//
//	float block_size;
//	unsigned int blocks_across;
//	float max_value;
//	float min_value;
//
//	LPDIRECT3DTEXTURE9 texture;
//	LPDIRECT3DVERTEXBUFFER9 verts;
//};
//
//unsigned char * GenerateStandardNoise( int seed, unsigned int xoffset, unsigned int yoffset );