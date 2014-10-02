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
	unsigned int sizeofBoneData;
	unsigned int sizeofBoneChildData;
	unsigned int sizeofBoneAnimationData;

	char* vertexData;
	char* indexData;
	char* boneData;
	char* boneChildrenData;
	char* boneAnimationData;
public:
	PokEngineModelDataMap();
	PokEngineModelDataMap( std::ifstream& stream );
	PokEngineModelDataMap( const char* rawData );
	VertexInfo* getVertexData(unsigned int* vertexInfoSize = 0);
	unsigned short* getIndexData( unsigned int* indexSize = 0 );
	BoneInfo* getBoneData( unsigned int* boneDataSize = 0 );
	unsigned int* getBoneChildren( unsigned int* boneChildrenSize = 0 );
	AnimationInfo* getAnimation( unsigned int* animationInfoSize = 0 );
};