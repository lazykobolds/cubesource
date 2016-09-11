#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "ChunkReader.h"
#include "..\Utilities\SharedResource.h"


struct KeyFrame
{
	float Time;
	D3DXVECTOR3 Translation;
	D3DXQUATERNION Rotation;
}; 

struct VQS
{
	D3DXVECTOR3 Translation;
	D3DXQUATERNION Rotation;	
};

struct Track
{
	unsigned int NumberOfFrames;
	KeyFrame * Frames;

	//constant quaternion term for iSlerp interpolation
	D3DXQUATERNION * Qc;
	D3DXQUATERNION Qk;
	D3DXVECTOR3 * Vc;
	D3DXVECTOR3 Vk;
	unsigned int CurrentFrame;
};

class Animation : public SharedResource
{
public:
	Animation( void );
	~Animation( void );

	void LoadFromFile( ChunkReader & file );
	
	void CalculateAnimation( float time, unsigned int track_index, unsigned int & previous_frame, VQS & transform );

	float Duration;
	unsigned int NumberOfTracks;
	Track * Tracks;
private:
};

class AnimationSet
{
public:
	AnimationSet( void );
	~AnimationSet( void );

	AnimationSet( const AnimationSet & copy );
	void operator =( const AnimationSet & copy );

	void AddAnimation( Animation * animation );

	Animation * operator []( const unsigned int & index ) const;
 
	unsigned int NumberOfAnimations;
	Animation ** Animations;

private:
};
