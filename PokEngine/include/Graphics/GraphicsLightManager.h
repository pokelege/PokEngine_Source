#pragma once
#include <PokEngineExportHeader.h>
#define MAX_LIGHTS 50
class Light;
struct RenderableInfo;
class POKENGINE_SHARED GraphicsLightManager
{
	Light* lights;
	unsigned int numLightSlots;
public:
	static GraphicsLightManager global;
	GraphicsLightManager();
	void initialize( unsigned int numLightSlots = MAX_LIGHTS );
	void destroy();
	bool initialized();
	Light* addLight();
	void applyLights( RenderableInfo* renderable );
};