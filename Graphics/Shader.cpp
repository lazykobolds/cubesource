#include "Shader.h"
#include <stdio.h>

LPD3DXEFFECT LoadEffect( const char * filename, LPDIRECT3DDEVICE9 d3ddev )
{
	LPD3DXEFFECT effect = 0;
	HRESULT hr;
	LPD3DXBUFFER buffer_errors = NULL;

	hr = D3DXCreateEffectFromFile(	d3ddev, 
									filename,
									NULL, 
									NULL, 
									0, 
									NULL, 
									&effect, 
									&buffer_errors );

	if( FAILED(hr) &&  buffer_errors )
	{
		char* compile_errors = (char*)buffer_errors->GetBufferPointer();
		MessageBox(NULL, (const char*)compile_errors, "Fx Compile Error",
			MB_OK|MB_ICONEXCLAMATION);
		printf("SHADER ERROR:\n");
		printf( "%s\n", compile_errors );
	}else if( FAILED(hr) )
	{
		printf("Error number: %i\n", hr );
		printf("Failed to create shader: %s\n", filename );
	}

	return effect;
}