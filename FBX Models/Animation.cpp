#include "Animation.h"

#define CONSTANT_FRAME_TIME 0.01f

Animation::Animation( void )
{
	Duration = 0.f;
	NumberOfTracks = 0;
	Tracks = NULL;
}

Animation::~Animation( void )
{
	for( unsigned int i = 0; i < NumberOfTracks; ++i )
	{
		if( Tracks[i].Frames ) delete Tracks[i].Frames;
	}
	if( Tracks ) delete [] Tracks;
}

/*
animation_header
	animation_duration { float }
	number_of_tracks { unsigned int }

	{for each track}
	number_of_frames { unsigned int }
	
	--frames-- { frame array for num frames }
		time { float }
		translation { vec3 }
		rotation { quaternion }		
*/
void Animation::LoadFromFile( ChunkReader & file )
{
	file.ReadChunkHeader();

	NumberOfTracks = 0;
	file.Read( Duration );
	file.Read( NumberOfTracks );

	Tracks = new Track[ NumberOfTracks ];
	for( unsigned int i = 0; i < NumberOfTracks; ++i )
	{
		Tracks[i].NumberOfFrames = 0;
		file.Read( Tracks[i].NumberOfFrames );
		Tracks[i].Frames = new KeyFrame[ Tracks[i].NumberOfFrames ];
		file.ReadArray( Tracks[i].Frames, Tracks[i].NumberOfFrames );
	}
}


void Animation::CalculateAnimation( float time, 
									unsigned int track_index, 
									unsigned int & previous_frame, 
									VQS & transform )
{
	//determine keyframe interval
	unsigned int frame = previous_frame;
	while(	frame != Tracks[ track_index ].NumberOfFrames-1 && 
			Tracks[ track_index ].Frames[ frame+1 ].Time < time )
	{
		++frame;
	}
	while(	frame != 0 && 
			Tracks[ track_index ].Frames[ frame ].Time > time )
	{
		--frame;
	}

	if( frame == Tracks[ track_index ].NumberOfFrames - 1 )
	{
		//use the last keyframe data by itself
		transform.Rotation = Tracks[ track_index ].Frames[ frame ].Rotation;
		transform.Translation = Tracks[ track_index ].Frames[ frame ].Translation;
	}else
	{
		//interpolate between keyframes
		float diff = Tracks[ track_index ].Frames[ frame+1 ].Time - Tracks[ track_index ].Frames[ frame ].Time;
		float t = time - Tracks[ track_index ].Frames[ frame ].Time;
		t /= diff;

		//perform lerp algorithm on two quaternions
		float dot = 
			Tracks[track_index].Frames[frame].Rotation.x * Tracks[track_index].Frames[frame+1].Rotation.x +
			Tracks[track_index].Frames[frame].Rotation.y * Tracks[track_index].Frames[frame+1].Rotation.y +
			Tracks[track_index].Frames[frame].Rotation.z * Tracks[track_index].Frames[frame+1].Rotation.z +
			Tracks[track_index].Frames[frame].Rotation.w * Tracks[track_index].Frames[frame+1].Rotation.w;
		if( dot < 0.f )
		{
			transform.Rotation = 
				Tracks[ track_index ].Frames[ frame ].Rotation * (1-t) +
				Tracks[ track_index ].Frames[ frame+1 ].Rotation * -t;
		}else
		{
			transform.Rotation = 
				Tracks[ track_index ].Frames[ frame ].Rotation * (1-t) +
				Tracks[ track_index ].Frames[ frame+1 ].Rotation * t;
		}
		//D3DXQuaternionSlerp( 
		//	&transform.Rotation , 
		//	&Tracks[ track_index ].Frames[ frame ].Rotation , 
		//	&Tracks[ track_index ].Frames[ frame+1 ].Rotation,
		//	t  );

		transform.Translation = 
			Tracks[ track_index ].Frames[ frame ].Translation * (1-t) +
			Tracks[ track_index ].Frames[ frame+1 ].Translation * t;
		
	}

	previous_frame = frame;
}



void AnimationSet::AddAnimation( Animation * animation )
{
	//reallocate memory for larger set of animations
	Animation ** animations = new Animation*[ NumberOfAnimations+1 ];
	for( unsigned int i = 0; i < NumberOfAnimations; ++i )
	{
		animations[i] = Animations[i];
	}
	animations[ NumberOfAnimations ] = animation;
	++NumberOfAnimations;
	if( Animations ) delete [] Animations;
	Animations = animations;
}


AnimationSet::AnimationSet( void )
{
	NumberOfAnimations = 0;
	Animations = NULL;
}

AnimationSet::~AnimationSet( void )
{
	for( unsigned int i = 0; i < NumberOfAnimations; ++i )
	{
		Animations[i]->Release();
	}
}

AnimationSet::AnimationSet( const AnimationSet & copy )
{
	NumberOfAnimations = copy.NumberOfAnimations;
	if( NumberOfAnimations > 0 ) Animations = new Animation*[ NumberOfAnimations ];
	for( unsigned int i = 0; i < NumberOfAnimations; ++i )
	{
		Animations[i] = copy.Animations[i];
		Animations[i]->AddReference();
	}
}

void AnimationSet::operator =( const AnimationSet & copy )
{
	if( NumberOfAnimations > 0 )
	{
		for( unsigned int i = 0; i < NumberOfAnimations; ++i )
		{
			Animations[i]->Release();
		}
	}

	NumberOfAnimations = copy.NumberOfAnimations;
	if( NumberOfAnimations > 0 ) Animations = new Animation*[ NumberOfAnimations ];
	for( unsigned int i = 0; i < NumberOfAnimations; ++i )
	{
		Animations[i] = copy.Animations[i];
		Animations[i]->AddReference();
	}
}

Animation * AnimationSet::operator []( const unsigned int & index ) const
{
	return Animations[ index ];
}

