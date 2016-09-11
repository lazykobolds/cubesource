#include "AnimationController.h"

AnimationController::AnimationController( void )
{
	SkeletonBones = NULL;;
	BoneMatrices = NULL;
	LastKeyFrame = NULL;
	NumberOfBones = 0;

	CurrentAnimation = NULL;

	AnimationTime = 0.f;
	AnimationSpeed = 1.f;
}

//copy all relevant data
//allocate memory for arrays and copy data over
AnimationController::AnimationController( const AnimationController & copy )
{
	NumberOfBones = copy.NumberOfBones;
	SkeletonBones = copy.SkeletonBones;
	SkeletonBones->AddReference();
	
	Animations = copy.Animations;
	CurrentAnimation = copy.CurrentAnimation;

	BoneMatrices = new D3DXMATRIX[ NumberOfBones ];

	LastKeyFrame = new unsigned int[ NumberOfBones ];
	memcpy( LastKeyFrame, copy.LastKeyFrame, sizeof( unsigned int ) * NumberOfBones );

	AnimationSpeed = copy.AnimationSpeed;
	AnimationTime = copy.AnimationTime;
}

AnimationController::AnimationController( const AnimationController * copy )
{
	NumberOfBones = copy->NumberOfBones;
	SkeletonBones = copy->SkeletonBones;
	SkeletonBones->AddReference();
	
	Animations = copy->Animations;
	CurrentAnimation = copy->CurrentAnimation;

	BoneMatrices = new D3DXMATRIX[ NumberOfBones ];

	LastKeyFrame = new unsigned int[ NumberOfBones ];
	memcpy( LastKeyFrame, copy->LastKeyFrame, sizeof( unsigned int ) * NumberOfBones );

	AnimationSpeed = copy->AnimationSpeed;
	AnimationTime = copy->AnimationTime;
}

void AnimationController::operator =( const AnimationController & copy )
{
	if( SkeletonBones ) SkeletonBones->Release();
	if( BoneMatrices ) delete [] BoneMatrices;
	if( LastKeyFrame ) delete [] LastKeyFrame;

	NumberOfBones = copy.NumberOfBones;
	SkeletonBones = copy.SkeletonBones;
	SkeletonBones->AddReference();
	
	Animations = copy.Animations;
	CurrentAnimation = copy.CurrentAnimation;

	BoneMatrices = new D3DXMATRIX[ NumberOfBones ];

	LastKeyFrame = new unsigned int[ NumberOfBones ];
	memcpy( LastKeyFrame, copy.LastKeyFrame, sizeof( unsigned int ) * NumberOfBones );

	AnimationSpeed = copy.AnimationSpeed;
	AnimationTime = copy.AnimationTime;
}

AnimationController::~AnimationController( void )
{
	if( SkeletonBones ) SkeletonBones->Release();
	if( BoneMatrices ) delete [] BoneMatrices;
	if( LastKeyFrame ) delete [] LastKeyFrame;
}

void AnimationController::Update( float time )
{
	AnimationTime += AnimationSpeed * time;
	//subtract the total animation time from the current timer
	// what is left is an offset of unused animation time, from the beginning of the animation
	if( CurrentAnimation && AnimationTime > CurrentAnimation->Duration )
	{
		AnimationTime -= CurrentAnimation->Duration;

		//sets last keyframe data to zero
		ResetKeyFrames();
	}
	
	//update bone matrices
	SkeletonBones->SetAnimationPose( BoneMatrices, AnimationTime, *CurrentAnimation, LastKeyFrame );

	for( unsigned int i = 0; i < NumberOfBones; ++i )
	{
		//The matrices passed to the shader transform the vertex into bone space and then apply the bones animation
		BoneMatrices[i] = SkeletonBones->Bones[i].ModelToBoneSpace * BoneMatrices[i];
	}
}

void AnimationController::ResetKeyFrames( void )
{
	for( unsigned int i = 0; i < NumberOfBones; ++i )
	{
		LastKeyFrame[i] = 0;
	}
}

//default modeling position of bones
void AnimationController::SetBindPose( void )
{
	SkeletonBones->SetBindPose( BoneMatrices );
}


void AnimationController::ResetAnimation( void )
{
	AnimationTime = 0.f;
	ResetKeyFrames();
}

void AnimationController::SetAnimation( unsigned int index )
{
	if( index < Animations.NumberOfAnimations )
	{
		CurrentAnimation = Animations[ index ];
		ResetAnimation();
	}
}

void AnimationController::AddAnimation( Animation * animation )
{
	Animations.AddAnimation( animation );
}