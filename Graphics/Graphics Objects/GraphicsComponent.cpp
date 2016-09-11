#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent( void )
{
	Shader = 0;
	TextureList[0] = TextureList[1] = TextureList[2] = TextureList[3] = 0;
}

GraphicsComponent::GraphicsComponent( const GraphicsComponent * comp )
{
	Shader = comp->Shader;
	TextureList[0] = comp->TextureList[0];
	TextureList[1] = comp->TextureList[1];
	TextureList[2] = comp->TextureList[2];
	TextureList[3] = comp->TextureList[3];
}

void GraphicsComponent::SetRenderFlag(const unsigned int value, const unsigned short flagOffset)
{
	switch (flagOffset)
	{
	case RenderFlagOffset::MaterialId:
		RenderKey = (RenderKey & RenderFlag::MaterialId) | ((unsigned long long)value << flagOffset);
		break;
	case RenderFlagOffset::Depth:
		RenderKey = (RenderKey & RenderFlag::Depth) | ((unsigned long long)value << flagOffset);
		break;
	case RenderFlagOffset::TranslucencyType:
		RenderKey = (RenderKey & RenderFlag::TranslucencyType) | ((unsigned long long)value << flagOffset);
		break;
	case RenderFlagOffset::ViewportLayer:
		RenderKey = (RenderKey & RenderFlag::ViewportLayer) | ((unsigned long long)value << flagOffset);
		break;
	case RenderFlagOffset::Viewport:
		RenderKey = (RenderKey & RenderFlag::Viewport) | ((unsigned long long)value << flagOffset);
		break;
	case RenderFlagOffset::FullscreenLayer:
		RenderKey = (RenderKey & RenderFlag::FullscreenLayer) | ((unsigned long long)value << flagOffset);
		break;
	}
}

/*

MaterialId = 30,
Depth = 24,
TranslucencyType = 2,
ViewportLayer = 3,
Viewport = 3,
FullscreenLayer = 2

*/
void GraphicsComponent::UpdateRenderKey(void)
{
	RenderKey =
		FullscreenLayer <<
		RenderFlagSize::Viewport | Viewport <<
		RenderFlagSize::ViewportLayer | ViewportLayer <<
		RenderFlagSize::TranslucencyType | TranslucencyType <<
		RenderFlagSize::Depth <<
		RenderFlagSize::MaterialId | MaterialId;
}