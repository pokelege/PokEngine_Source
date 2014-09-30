#pragma once
#pragma warning (disable:4201)
#pragma warning (disable:4251)
#include <PokEngineExportHeader.h>
#include <glm.hpp>
class PokEngineModelDataMap;
struct POKENGINE_SHARED VertexInfo
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::ivec4 blendingIndex;
	glm::vec4 blendingWeight;
	int blendingDataEnd;
	static unsigned int POSITION_OFFSET;
	static unsigned int UV_OFFSET;
	static unsigned int NORMAL_OFFSET;
	static unsigned int TANGENT_OFFSET;
	static unsigned int BITANGENT_OFFSET;
	static unsigned int BLENDINGINDEX_OFFSET;
	static unsigned int BLENDINGWEIGHT_OFFSET;
	static unsigned int STRIDE;
};