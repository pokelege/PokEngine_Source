#pragma once
#include <PokEngineExportHeader.h>
#include <glm.hpp>
struct POKENGINE_SHARED AnimationRenderingInfo
{
	glm::mat4* animationMatrices;
	unsigned int sizeofAnimationMatrices;
	unsigned int animationFrameRate;
	float currentFrame;
};