#pragma once

#define MAX_SENTENCE_LENGTH 2048
#define MAX_REPLACE_LENGTH 128 
#define MAX_PROCEDURES 16 
#define MAX_PROCEDURE_CHANCES 16

struct LSProcedure
{
	LSProcedure( void );
	LSProcedure( char target );
	void SetTarget( char target );
	void AddProcedure( float chance, char * procedure );
	char * GetReplacementText( void ) ;

	char Target;
	char TargetString[2];
	char Procedures[ MAX_PROCEDURE_CHANCES ][ MAX_REPLACE_LENGTH ];
	float Chances[ MAX_PROCEDURE_CHANCES ];
	unsigned int ProcedureCount;
};

class LSystem
{
public:
	LSystem( void );
	
	void AddProcedure( char target, char * procedure, float chance );

	void GenerateString( void );

	LSProcedure Procedures[ MAX_PROCEDURES ];
	unsigned int ProcedureCount;

	//working and editing
	char Sentence[2][ MAX_SENTENCE_LENGTH ];
	unsigned int ActiveSentence;
};