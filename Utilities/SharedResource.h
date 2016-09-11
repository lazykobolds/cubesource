#pragma once

class SharedResource
{
public:
	SharedResource( void ) : ReferenceCount( 1 ) {}
	virtual ~SharedResource( void ) {}

	virtual void AddReference( void );
	virtual void Release( void );
	

private:
	unsigned int ReferenceCount;
};

inline void SharedResource::AddReference( void )
{
	++ReferenceCount;
}

inline void SharedResource::Release( void )
{
	if( --ReferenceCount == 0 ) 
		delete this; 
}