#include "RenderFunctions.h"
#include "..\Utilities\MathLib.h"
#include "..\DirectX\VertexTypes.h"
#include "..\Graphics\Graphics Objects\MeshComponent.h"

void RenderFunctions::SampleRender(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entity, LPD3DXMATRIX local_transform)
{
	dxdevice->SetFVF( Vertex::VertexFVF );
	effect->SetTechnique( 0 );
	effect->SetMatrix( "worldViewProj", &((*local_transform)*camera->ViewProjMatrix) );
	effect->SetMatrix( "viewMat", &(camera->ViewMatrix) );
	
	
	unsigned int passes = 0;
	effect->Begin( &passes, 0 );

	entity->Colored = false;
	for( unsigned int i = 0; i < passes; ++i )
	{
		effect->BeginPass( i );
		entity->GraphicsComponent->Render( dxdevice, entity );

		effect->EndPass();
	}
	
	effect->End();
}

void RenderFunctions::SampleRender2(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entity, LPD3DXMATRIX local_transform)
{
	dxdevice->SetFVF( Vertex::VertexFVF );
	effect->SetTechnique( "outlines" );
	effect->SetMatrix( "worldViewProj", &((*local_transform)*camera->ViewProjMatrix) );
	effect->Begin( 0, 0 );
	effect->BeginPass( 0 );

	entity->GraphicsComponent->Render( dxdevice, entity );
	
	effect->EndPass();
	effect->End();
}
	
void RenderFunctions::FlatScreenRender(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entity, LPD3DXMATRIX local_transform)
{
	dxdevice->SetFVF( Vertex::VertexFVF );
	effect->SetTechnique( "Flat" );
	effect->Begin( 0, 0 );
	effect->BeginPass( 0 );

	entity->GraphicsComponent->Render( dxdevice, entity );
	
	effect->EndPass();
	effect->End();
}
	
void RenderFunctions::NoiseRender(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entity, LPD3DXMATRIX local_transform)
{
	dxdevice->SetFVF( Vertex::VertexFVF );
	effect->SetTechnique( "Noise" );
	//effect->SetMatrix( "worldViewProj", &((*local_transform)*camera->ViewProjMatrix) );
	effect->Begin( 0, 0 );
	effect->BeginPass( 0 );

	entity->GraphicsComponent->Render( dxdevice, entity );
	
	effect->EndPass();
	effect->End();
}