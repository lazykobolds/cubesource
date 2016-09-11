#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "..\Shader.h"

namespace RenderFlag
{
	const unsigned long long FullscreenLayer		= 0xC000000000000000;
	const unsigned long long Viewport = 0x3800000000000000;
	const unsigned long long ViewportLayer = 0x0700000000000000;
	const unsigned long long TranslucencyType = 0x00C0000000000000;
	const unsigned long long Depth = 0x003FFFFFC0000000;
	const unsigned long long MaterialId = 0x000000003FFFFFFF;
};

namespace RenderFlagOffset
{
	enum RenderFlagOffset
	{
		MaterialId = 0,
		Depth = 30,
		TranslucencyType = 54,
		ViewportLayer = 56,
		Viewport = 59,
		FullscreenLayer = 62
	};
}

namespace RenderFlagSize
{
	enum RenderFlagSize
	{
		MaterialId = 30,
		Depth = 24,
		TranslucencyType = 2,
		ViewportLayer = 3,
		Viewport = 3,
		FullscreenLayer = 2
	};
}

class GameEntity;

class GraphicsComponent
{
public:
	GraphicsComponent( void );
	GraphicsComponent( const GraphicsComponent * comp );

	virtual void Render( const LPDIRECT3DDEVICE9 dxdevice, const GameEntity * entity ) { printf("your in the wrong place dumb dumb\n"); }

	void SetRenderFlag(const unsigned int value, const unsigned short flagOffset);
	void UpdateRenderKey(void);

	LPD3DXEFFECT Shader;
	LPDIRECT3DTEXTURE9 TextureList[4];

	unsigned short TranslucencyType;
	unsigned short ViewportLayer;
	unsigned short Viewport;
	unsigned short FullscreenLayer;
	unsigned int MaterialId;
	unsigned long long RenderKey;

//protected:
	virtual void SetObjectSpecificRenderStates( const GameEntity * entity ) {}

};