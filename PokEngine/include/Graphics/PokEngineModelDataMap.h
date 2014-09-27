#pragma once
#include <fstream>
class PokEngineModelDataMap
{
	unsigned int sizeofVertexData;
	unsigned int sizeofIndexData;
	unsigned int sizeofBlendingData;
	unsigned int sizeofBoneData;
	unsigned int sizeofBoneChildData;
	unsigned int sizeofBoneAnimationData;

	char* vertexData;
	char* indexData;
	char* blendingIndexData;
	char* blendingWeightData;
	char* boneData;
	char* boneChildrenData;
	char* boneAnimationData;
public:
	PokEngineModelDataMap( std::fstream& stream );
	void getVertexData()
};