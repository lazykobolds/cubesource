#pragma once

#include "ChunkReader.h"
#include "Bone.h"
#include "Animation.h"
#include "..\Utilities\SharedResource.h"

class Skeleton : public SharedResource
{
public:
	Skeleton( void );
	~Skeleton( void );

	void LoadFromFile( ChunkReader & file );
	void Initialize( void );

	void SetBindPose( D3DXMATRIX * bone_matrices );
	void SetAnimationPose( D3DXMATRIX * bone_matrices, float time, Animation & animation, unsigned int * last_keys );

	unsigned int NumberOfBones;
	Bone * Bones;

private:
	void AddRootBone( Bone * bone );
	void SetAnimationPoseRecursive(	D3DXMATRIX * bone_matrices, 
									float time, 
									Animation & animation, 
									unsigned int * last_keys,
									Bone & bone, 
									D3DXMATRIX & parent_matrix );

	unsigned int NumberOfRootBones;
	Bone ** RootBones;

};