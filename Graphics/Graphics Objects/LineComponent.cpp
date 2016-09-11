#include "LineComponent.h"
#include "..\..\DirectX\VertexTypes.h"
#include "..\..\Utilities\MathLib.h"
#include "..\..\Game Objects\GameEntity.h"


LineComponent::LineComponent( void )
	: GraphicsComponent()
{
	Verts = 0;
	NumVerts = 0;
	Shader = 0;
	for( unsigned int i = 0; i < 4; ++i ) TextureList[i] = 0;
}

LineComponent::LineComponent( const LineComponent * comp )
	: GraphicsComponent( comp )
{
	Verts = comp->Verts;
	NumVerts = comp->NumVerts;
	Shader = comp->Shader;
	for( unsigned int i = 0; i < 4; ++i ) TextureList[i] = comp->TextureList[i];
}

void LineComponent::Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity )
{
	//SetObjectSpecificRenderStates( entity );
	dxdevice->SetStreamSource( 0, Verts, 0, sizeof( Vertex ) );
	dxdevice->DrawPrimitive( D3DPT_LINELIST, 0, NumVerts/2 );
}

void LineComponent::SetObjectSpecificRenderStates( const GameEntity * entity )
{
	//D3DXMATRIX matModel;
	//D3DXMatrixIdentity( &matModel );
	//BuildVQSMatrix( matModel, entity->Rotation, entity->Position );
	
	D3DXMATRIX matModel( entity->Rotation );
	matModel._41 = entity->Position.x;
	matModel._42 = entity->Position.y;
	matModel._43 = entity->Position.z;
 	entity->Shader->SetMatrix( "world", &matModel );
	entity->Shader->CommitChanges();
}
