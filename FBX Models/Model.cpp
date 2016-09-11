#include "Model.h"
#include "ChunkReader.h"

//fbx file format tags used in saving / loading a mesh
const unsigned int FBX_FILE_TAG			= 'dpm2';
const unsigned int FBX_MESH_TAG			= 'mesh';
const unsigned int FBS_SKELETON_TAG		= 'skel';
const unsigned int FBX_ANIMATION_TAG	= 'anim';

Model::Model( void )
{
	ModelMesh = NULL;
	Controller = NULL; 
	ModelType = NO_MODEL;
}

Model::~Model( void )
{
	if( ModelMesh ) ModelMesh->Release();
	if( Controller ) delete Controller;
}

Model::Model( const Model & copy )
{
	ModelMesh = copy.ModelMesh;
	ModelMesh->AddReference();

	Controller = new AnimationController( copy.Controller );
	
	ModelType = copy.ModelType;
}

Model::Model( const Model * copy )
{
	ModelMesh = copy->ModelMesh;
	ModelMesh->AddReference();

	Controller = new AnimationController( copy->Controller );
	
	ModelType = copy->ModelType;
}

void Model::operator =( const Model & copy )
{
	ModelMesh = copy.ModelMesh;
	ModelMesh->AddReference();

	Controller = new AnimationController( copy.Controller );
	
	ModelType = copy.ModelType;
}

Model * FBX_LoadModel( std::string filename, IDirect3DDevice9 * d3ddev )
{
	ChunkReader file;
	file.Open( filename );

	GChunk file_chunk = file.ReadChunkHeader();

	if( file_chunk.Type != FBX_FILE_TAG )
		return NULL;

	Model * new_model = new Model();
	new_model->ModelType = STATIC_MODEL;
			
	file_chunk = file.PeekChunk();

	while( file_chunk.Type != 0 )
	{
		switch( file_chunk.Type )
		{
			case FBX_MESH_TAG:
			{
				new_model->ModelMesh = new Mesh();
				new_model->ModelMesh->LoadFromFile( file );
				new_model->ModelMesh->Initialize( d3ddev );
			}
			break;
			case FBS_SKELETON_TAG:
			{
				Skeleton * skeleton = new Skeleton();
				skeleton->LoadFromFile( file );
				skeleton->Initialize();

				new_model->ModelType = SKINNED_MODEL;
				new_model->Controller = new AnimationController();
				new_model->Controller->SkeletonBones = skeleton;
				new_model->Controller->NumberOfBones = skeleton->NumberOfBones;
				new_model->Controller->BoneMatrices = new D3DXMATRIX[ skeleton->NumberOfBones ];
				new_model->Controller->LastKeyFrame = new unsigned int[ skeleton->NumberOfBones ];
				for( unsigned int i = 0; i < skeleton->NumberOfBones; ++i )
				{
					new_model->Controller->LastKeyFrame[i] = 0;
				}
			}
			break;
			case FBX_ANIMATION_TAG:
			{
				Animation * animation = new Animation();
				animation->LoadFromFile( file );
				new_model->Controller->AddAnimation( animation );
				new_model->Controller->CurrentAnimation = new_model->Controller->Animations[0];
			}
			break;
			default:
				file.SkipChunk( file_chunk );
			break;
		}

		file_chunk = file.PeekChunk();
	}

	return new_model;
}

void Model::Draw( IDirect3DDevice9 * device, ID3DXEffect * effect )
{
	//Hardware skinning using vertex shaders
	unsigned int passes = 0;
	effect->SetMatrixArray( "WorldMatrixArray", Controller->BoneMatrices, Controller->NumberOfBones );
	effect->SetTechnique( "TechniqueSkin" );
	effect->Begin( &passes, 0 );
	for( UINT uPass = 0; uPass < passes; ++uPass )
	{
		effect->BeginPass( uPass );
		ModelMesh->Render( device );
		effect->EndPass();
	}
	effect->End();
}