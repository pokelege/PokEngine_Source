#pragma once
#include <fstream>
#include <PokEngineExportHeader.h>
struct VertexInfo;
struct BoneInfo;
struct AnimationInfo;
class POKENGINE_SHARED PokEngineModelDataMap
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
	PokEngineModelDataMap();
	PokEngineModelDataMap( std::ifstream& stream );
	VertexInfo* getVertexData(unsigned int* vertexInfoSize = 0);
	unsigned short* getIndexData( unsigned int* indexSize = 0 );
	unsigned int* getBlendingIndexData( unsigned int* blendingIndexSize = 0 );
	float* getBlendingWeightData( unsigned int* blendingWeightSize = 0 );
	BoneInfo* getBoneData( unsigned int* boneDataSize = 0 );
	unsigned int* getBoneChildren( unsigned int* boneChildrenSize = 0 );
	AnimationInfo* getAnimation( unsigned int* animationInfoSize = 0 );
};