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
	int blendingDataStart;
	int blendingDataEnd;
	static unsigned int POSITION_OFFSET;
	static unsigned int UV_OFFSET;
	static unsigned int NORMAL_OFFSET;
	static unsigned int TANGENT_OFFSET;
	static unsigned int BITANGENT_OFFSET;
	static unsigned int BLENDINGSTART_OFFSET;
	static unsigned int BLENDINGEND_OFFSET;
	static unsigned int STRIDE;
	void getBlendingInfo( unsigned int index , PokEngineModelDataMap& modelData , unsigned int* blendingIndex = 0 , float* blendingWeight = 0 );
	unsigned int size();
};