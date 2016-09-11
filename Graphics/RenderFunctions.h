#include <d3d9.h>
#include <d3dx9.h>

#include "Camera.h"
#include "..\Game Objects\GameEntity.h"

//typedef void(*RenderFcn)( LPD3DXEFFECT, Camera*, List< GameEntity * >& );

namespace RenderFunctions
{
	
	void SampleRender(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entities, LPD3DXMATRIX local_transform);
	void SampleRender2(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entities, LPD3DXMATRIX local_transform);
	void FlatScreenRender(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entities, LPD3DXMATRIX local_transform);
	void NoiseRender(const LPDIRECT3DDEVICE9 dxdevice, const LPD3DXEFFECT effect, Camera * camera, GameEntity * entities, LPD3DXMATRIX local_transform);

};