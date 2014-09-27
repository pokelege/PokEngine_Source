#pragma once
#include <PokEngineExportHeader.h>
#include <glm.hpp>
struct POKENGINE_SHARED VertexInfo
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int blendingDataStart;
	int blendingDataEnd;
};

struct POKENGINE_SHARED FullBlendingInfo
{
	unsigned int numBlendingInfo;
	unsigned int* blendingIndices;
	float* blendingWeights;
};