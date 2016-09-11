#pragma once

#include "Skeleton.h"
#include "Animation.h"

class AnimationController
{
public:
	AnimationController( void );
	~AnimationController( void );

	AnimationController( const AnimationController & copy );
	AnimationController( const AnimationController * copy );
	void operator =( const AnimationController & copy );

	void AddAnimation( Animation * animation );

	void Update( float time );
	void SetBindPose( void );
	void SetAnimation( unsigned int index );

	//shared data
	unsigned int NumberOfBones;	
	Skeleton * SkeletonBones;
	AnimationSet Animations;

	//unique data
	Animation * CurrentAnimation;
	D3DXMATRIX * BoneMatrices;
	unsigned int * LastKeyFrame;
	
private:
	void ResetKeyFrames( void );
	void ResetAnimation( void );
	
	//unique data
	float AnimationTime;
	float AnimationSpeed;
};