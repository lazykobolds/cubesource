#include "MeshComponent.h"
#include "..\..\DirectX\VertexTypes.h"
#include "..\..\Utilities\MathLib.h"
#include "..\..\Game Objects\GameEntity.h"


MeshComponent::MeshComponent( void )
	: GraphicsComponent()
{
	Vertices = 0;
	NumTriangles = 0;
	Shader = 0;
	for( unsigned int i = 0; i < 4; ++i ) 
		TextureList[i] = 0;
}

MeshComponent::MeshComponent( const MeshComponent * comp )
	: GraphicsComponent( comp )
{
	Vertices = comp->Vertices;
	NumTriangles = comp->NumTriangles;
	Shader = comp->Shader;
	for( unsigned int i = 0; i < 4; ++i ) 
		TextureList[i] = comp->TextureList[i];
}

void MeshComponent::Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity )
{
	SetObjectSpecificRenderStates( entity );
	dxdevice->SetStreamSource(0, Vertices, 0, sizeof(Vertex));
	dxdevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumTriangles );
}

void MeshComponent::SetObjectSpecificRenderStates( const GameEntity * entity )
{
	D3DXMATRIX matModel( entity->Rotation );
	matModel._41 = entity->Position.x;
	matModel._42 = entity->Position.y;
	matModel._43 = entity->Position.z;
	entity->GraphicsComponent->Shader->SetMatrix("world", &matModel);
	entity->GraphicsComponent->Shader->CommitChanges();
}