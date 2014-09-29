#pragma once
#include <glm.hpp>
#include <vector>
struct BlendingIndexWeightPair
{
	unsigned int blendingIndex;
	float blendingWeight;
};
struct VertexData
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	std::vector<BlendingIndexWeightPair> blendingInfo;
};