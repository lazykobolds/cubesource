#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <string.h>

struct Bone
{
	Bone( void )
	{
		Childeren = NULL;
		NumChilderen = 0;
	}

	void AddChild( Bone * child )
	{
		Bone ** bones = new Bone*[ NumChilderen+1 ];
		for( unsigned int i = 0; i < NumChilderen; ++i )
		{
			bones[i] = Childeren[i];
		}
		bones[ NumChilderen ] = child;
		++NumChilderen;
		if( Childeren ) delete [] Childeren;
		Childeren = bones;
	}

	std::string Name;
	int BoneIndex;
	int ParentBoneIndex;

	//Not all the following data is needed but is
	//helpful to debug and understand how skinning works

	//Translation of bone in bind position
	D3DXVECTOR3 BindT;
	//Rotation of bone in bind position
	D3DXQUATERNION BindR;
	//Transform of the bone in bind position
	D3DXMATRIX BindTransform;

	//Transform from model space to bone space
	//this is the inverse of the BindTransform
	D3DXMATRIX ModelToBoneSpace;

	//Inverse Translation of bone in bind position
	D3DXVECTOR3 ModelToBoneSpaceT;
	//Inverse Rotation of bone in bind position
	D3DXQUATERNION ModelToBoneSpaceR;

    Bone ** Childeren;
	unsigned int NumChilderen;
};