//#include "NoiseComponent.h"
//#include "..\..\DirectX\VertexTypes.h"
//#include "..\..\Utility\MathLib.h"
//#include "..\..\Game Objects\GameEntity.h"
//
//
//NoiseComponent::NoiseComponent( void )
//	: GraphicsComponent()
//{
//	Verts = 0;
//	NumTriangles = 0;
//	Shader = 0;
//	for( unsigned int i = 0; i < 4; ++i ) TextureList[i] = 0;
//}
//
//NoiseComponent::NoiseComponent( const NoiseComponent * comp )
//	: GraphicsComponent( comp )
//{
//	Verts = comp->Verts;
//	NumTriangles = comp->NumTriangles;
//	Shader = comp->Shader;
//	for( unsigned int i = 0; i < 4; ++i ) TextureList[i] = comp->TextureList[i];
//}
//
//void NoiseComponent::Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity )
//{
//	//dxdevice->SetStreamSource( 0, Verts, 0, sizeof( Vertex ) );
//	//dxdevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumTriangles );
//
//	dxdevice->SetStreamSource( 0, noise.verts, 0, sizeof( Vertex ) );
//	entity->Shader->SetTexture( "noise_texture", noise.texture );
//	dxdevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
//	
//	//dxdevice->SetStreamSource( 0, voronoi.vert_buffer, 0, sizeof( Vertex ) );
//	//dxdevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, voronoi.num_verts/3 );
//	
//}
//
//void NoiseComponent::SetObjectSpecificRenderStates( const GameEntity * entity )
//{
//	//D3DXMATRIX matModel;
//	//D3DXMatrixIdentity( &matModel );
//	//BuildVQSMatrix( matModel, entity->Rotation, entity->Position );
//	
//	D3DXMATRIX matModel( entity->Rotation );
//	matModel._11 *= entity->Size;
//	matModel._12 *= entity->Size;
//	matModel._13 *= entity->Size;
//	matModel._21 *= entity->Size;
//	matModel._22 *= entity->Size;
//	matModel._23 *= entity->Size;
//	matModel._31 *= entity->Size;
//	matModel._32 *= entity->Size;
//	matModel._33 *= entity->Size;
//	matModel._41 = entity->Position.x;
//	matModel._42 = entity->Position.y;
//	matModel._43 = entity->Position.z;
//	entity->Shader->SetMatrix( "world", &matModel );
//	entity->Shader->CommitChanges();
//	//Shader->SetMatrix( "world", &matModel );
//	//Shader->CommitChanges();
//}
