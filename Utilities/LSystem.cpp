#include "LSystem.h"
#include <string.h>
#include <stdlib.h>

LSProcedure::LSProcedure( void ) 
{
	ProcedureCount = 0; 
}

LSProcedure::LSProcedure( char target ) 
{ 
	Target = target; 
	ProcedureCount = 0; 
	TargetString[0] = target; 
	TargetString[1] = 0; 
}

void LSProcedure::SetTarget( char target )
{
	Target = target; 
	ProcedureCount = 0; 
	TargetString[0] = target; 
	TargetString[1] = 0; 
}

void LSProcedure::AddProcedure( float chance, char * procedure ) 
{ 
	Chances[ProcedureCount] = chance; 
	strcpy(Procedures[ProcedureCount], procedure); ++ProcedureCount;
}

char * LSProcedure::GetReplacementText( void ) 
{ 
	float r = rand()/float(RAND_MAX);
	float c = 0.f;
	for( unsigned int i = 0; i < ProcedureCount; ++i )
	{
		c += Chances[i];
		if( r < c ) return Procedures[i];
	}
	return TargetString;
}

LSystem::LSystem( void ) 
{ 
	ActiveSentence = 0; 
	ProcedureCount = 0; 
}

void LSystem::AddProcedure( char target, char * procedure, float chance )
{
	for( unsigned int i = 0; i < ProcedureCount; ++i )
	{
		if( Procedures[i].Target == target )
		{
			Procedures[i].AddProcedure( chance, procedure );
			return;
		}
	}
	Procedures[ ProcedureCount ].SetTarget( target );
	Procedures[ ProcedureCount ].AddProcedure( chance, procedure );
	++ProcedureCount;
}

void LSystem::GenerateString( void )
{
	unsigned int active_i = 0;
	unsigned int mod = (ActiveSentence+1)%2;
	Sentence[mod][0] = 0;
	while( Sentence[ActiveSentence][active_i] != 0 )
	{
		for( unsigned int i = 0; i < ProcedureCount; ++i )
		{
			if( Procedures[i].Target == Sentence[ActiveSentence][active_i] )
			{
				char rt[MAX_REPLACE_LENGTH];
				strcpy( rt, Procedures[i].GetReplacementText() );
				if( strlen( rt ) + strlen( Sentence[mod] ) > MAX_SENTENCE_LENGTH ) 
				{
					ActiveSentence = mod;
					return;
				}

				strcat( Sentence[mod], Procedures[i].GetReplacementText() );
				break;
			}
		}
		++active_i;
	}
	ActiveSentence = mod;
}