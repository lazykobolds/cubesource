#pragma once

#include <d3d9.h>
#include <string>

#include "Mesh.h"
#include "AnimationController.h"

enum ModelType
{
	STATIC_MODEL = 0,
	SKINNED_MODEL,
	NO_MODEL
};

class Model
{
public:
	Model( void );
	~Model( void );

	Model( const Model & copy );
	Model( const Model * copy );
	void operator =( const Model & copy );

	void Draw( IDirect3DDevice9 * device, ID3DXEffect * pEffect );

	Mesh * ModelMesh;
	AnimationController * Controller;

	unsigned int ModelType;

private:
};


Model * FBX_LoadModel( std::string filename, IDirect3DDevice9 * d3ddev );