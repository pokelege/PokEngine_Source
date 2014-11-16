#pragma once
#pragma warning (disable:4251)
#pragma warning (disable:4201)
#include <glm.hpp>
#include <gtc\quaternion.hpp>
#include <PokEngineExportHeader.h>
struct POKENGINE_SHARED AnimationInfo
{
	unsigned int frame;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
};