#include "AnimatedComponent.h"
#include "..\..\Utilities\MathLib.h"
#include "..\..\Game Objects\GameEntity.h"


void AnimatedModelComponent::Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity )
{
	SetObjectSpecificRenderStates( entity );
	Model.ModelMesh->Render( dxdevice );

}

void AnimatedModelComponent::SetObjectSpecificRenderStates( const GameEntity * entity )
{
	D3DXMATRIX matModel( entity->Rotation );
	matModel._41 = entity->Position.x;
	matModel._42 = entity->Position.y;
	matModel._43 = entity->Position.z;
	//BuildVQSMatrix( matModel, entity->Rotation, entity->Position );
	entity->Shader->SetMatrixArray( "WorldMatrixArray", Model.Controller->BoneMatrices, Model.Controller->NumberOfBones );
	entity->Shader->SetMatrix( "world", &matModel );
}
