#pragma once

#include <Windows.h>

#include "../Graphics/GraphicsEngine.h"
#include "../Graphics/Graphics Objects/MeshComponent.h"
#include "../Graphics/Primatives.h"
#include "GameEntity.h"
#include "../Graphics/RenderFunctions.h"
#include "../DirectX/Input.h"
#include "../Utilities/MathLib.h"
#include "../NoiseLib.h"

class GameController
{
public:

	InputBuffer IB;

	GraphicsEngine G;
	GameEntity obj;
	LPD3DXEFFECT shader;
	
	float * noise;

	void Initilize( HWND hWnd, HINSTANCE hInstance );
	void Update( unsigned int i );
	void CollisionStep( void );
	void DetectObjectCollision( GameEntity & ge );
};