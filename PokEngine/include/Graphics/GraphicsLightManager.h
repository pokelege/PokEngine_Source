#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <vector>
#include <glm.hpp>
#define MAX_LIGHTS 50
class Light;
class GraphicsSharedUniformManager;
class POKENGINE_SHARED GraphicsLightManager
{
	Light* lights;
	unsigned int numLightSlots;
	std::vector<glm::vec4> lightColor;
	std::vector<glm::vec3> lightPosition;
public:
	static GraphicsLightManager global;
	GraphicsLightManager();
	void initialize( unsigned int numLightSlots = MAX_LIGHTS );
	void destroy();
	bool initialized();
	Light* addLight();
	void applyLights( GraphicsSharedUniformManager* renderable );
};