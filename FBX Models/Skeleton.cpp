#include "Skeleton.h"
#include "..\Utilities\MathLib.h"


Skeleton::Skeleton( void )
{
	NumberOfBones = 0;
	NumberOfRootBones = 0;
	Bones = NULL;;
	RootBones = NULL;
}

Skeleton::~Skeleton( void )
{
	if( Bones ) delete [] Bones;
}

/*skeleton structure:

skeleton_header
	number of bones ( uint )

	-- { bones x number of bones } --
		bone_name ( std::string )
		parent_bone ( uint ) -- -1 represents a root bone
		bone_transl ( vec3 )
		bone_rotati ( quat )
		inv_transla ( vec3 )
		inv_rotatio ( quat )
*/
void Skeleton::LoadFromFile( ChunkReader & file )
{	
	file.ReadChunkHeader();

	//get the number of bones from the file
	NumberOfBones = 0;
	file.Read( NumberOfBones );
	Bones = new Bone[ NumberOfBones ];

	//read data for each bone
	for( unsigned int i = 0; i < NumberOfBones; ++i )
	{
		file.Read( Bones[i].Name );
		file.Read( Bones[i].ParentBoneIndex );
		file.Read( Bones[i].BindT );
		file.Read( Bones[i].BindR );
		file.Read( Bones[i].ModelToBoneSpaceT );
		file.Read( Bones[i].ModelToBoneSpaceR );
	}
}

void Skeleton::AddRootBone( Bone * bone )
{
	//reallocate memory for larger set of bones
	Bone ** bones = new Bone*[ NumberOfRootBones+1 ];
	for( unsigned int i = 0; i < NumberOfRootBones; ++i )
	{
		bones[i] = RootBones[i];
	}
	bones[ NumberOfRootBones ] = bone;
	++NumberOfRootBones;
	if( RootBones ) delete [] RootBones;
	RootBones = bones;
}

void Skeleton::Initialize( void )
{
	for( unsigned int i = 0; i < NumberOfBones; ++i )
	{
		//link bone childeren to their parents, and set root bones
		Bones[i].BoneIndex = i;
		if( Bones[i].ParentBoneIndex != -1 )
		{
			Bones[ Bones[i].ParentBoneIndex ].AddChild( &Bones[i] );
		}
		else
		{
			AddRootBone( &Bones[i] );
		}

		//combine rotation and translation into one matrix
		BuildVQSMatrix( Bones[i].BindTransform, Bones[i].BindR, Bones[i].BindT );
		//combine inverse rotation and translation into one matrix
		BuildVQSMatrix( Bones[i].ModelToBoneSpace, Bones[i].ModelToBoneSpaceR, Bones[i].ModelToBoneSpaceT );
	}
}

//set the bone transforms to the default positions
void Skeleton::SetBindPose( D3DXMATRIX * bone_matrices )
{
	for( unsigned int i = 0; i < NumberOfBones; ++i )
	{
		BuildVQSMatrix( bone_matrices[i], Bones[i].BindR, Bones[i].BindT );
	}
}

void Skeleton::SetAnimationPose(	D3DXMATRIX * bone_matrices, 
									float time, 
									Animation & animation, 
									unsigned int * last_keys )
{
	
	VQS vqs_data;
	D3DXMATRIX local_matrix;
	D3DXMATRIX identity_matrix;
	D3DXMatrixIdentity( &identity_matrix );

	//this is the linear method for summing parent-child bone matrices
	// - able to use this method because:
	//	 root bones are stored before child bones, so root matrices are built before children look at them
	// - pass less parameters as function overhead
	for( unsigned int i = 0; i < NumberOfBones; ++i )
	{
		animation.CalculateAnimation( time, i, last_keys[i], vqs_data );
		BuildVQSMatrix( local_matrix, vqs_data.Rotation, vqs_data.Translation );

		//-1 signifies a root bone
		if( Bones[i].ParentBoneIndex == -1 )
		{
			bone_matrices[i] = local_matrix;
		}else
		{
			bone_matrices[i] = local_matrix * bone_matrices[ Bones[i].ParentBoneIndex ];
		}
	} 

	//use for recursive method
	//D3DXMATRIX identity_matrix;
	//D3DXMatrixIdentity( &identity_matrix );
	//for( unsigned int i = 0; i < NumberOfRootBones; ++i )
	//{
	//	SetAnimationPoseRecursive( bone_matrices, time, animation, last_keys, *RootBones[i], identity_matrix );
	//}
}

void Skeleton::SetAnimationPoseRecursive(	D3DXMATRIX * bone_matrices, 
											float time, 
											Animation & animation, 
											unsigned int * last_keys,
											Bone & bone, 
											D3DXMATRIX & parent_matrix )
{	
	VQS vqs_data;
	D3DXMATRIX local_matrix;;
	
	animation.CalculateAnimation( time, bone.BoneIndex, last_keys[ bone.BoneIndex ], vqs_data );
	BuildVQSMatrix( local_matrix, vqs_data.Rotation, vqs_data.Translation );
	bone_matrices[ bone.BoneIndex ] = local_matrix * parent_matrix;

	for( unsigned int i = 0; i < bone.NumChilderen; ++i )
	{
		SetAnimationPoseRecursive( 
							bone_matrices, 
							time, 
							animation, 
							last_keys, 
							*bone.Childeren[i], 
							bone_matrices[ bone.BoneIndex ] );
	}
}
