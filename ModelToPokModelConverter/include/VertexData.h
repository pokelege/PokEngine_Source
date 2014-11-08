#pragma once
#include <glm.hpp>
#include <vector>
#include <FBXConverterExportHeader.h>
struct FBXCONVERTER_SHARED BlendingIndexWeightPair
{
	unsigned int blendingIndex;
	float blendingWeight;
};
struct FBXCONVERTER_SHARED VertexData
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	std::vector<BlendingIndexWeightPair> blendingInfo;
};