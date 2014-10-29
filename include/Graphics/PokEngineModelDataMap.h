#pragma once
#include <fstream>
#include <PokEngineExportHeader.h>
struct VertexInfo;
struct BoneInfo;
struct AnimationInfo;
struct AnimationFrameRangeInfo;
class POKENGINE_SHARED PokEngineModelDataMap
{
	unsigned int sizeofVertexData;
	unsigned int sizeofIndexData;
	unsigned int sizeofBoneData;
	unsigned int sizeofBoneChildData;
	unsigned int sizeofBoneAnimationData;
	unsigned int sizeofAnimationRangeInfo;

	char* vertexData;
	char* indexData;
	char* boneData;
	char* boneChildrenData;
	char* boneAnimationData;
	char* animationRangeInfo;
public:
	PokEngineModelDataMap();
	PokEngineModelDataMap( std::ifstream& stream );
	PokEngineModelDataMap( const char* rawData );
	~PokEngineModelDataMap();
	VertexInfo* getVertexData(unsigned int* vertexInfoSize = 0);
	unsigned short* getIndexData( unsigned int* indexSize = 0 );
	BoneInfo* getBoneData( unsigned int* boneDataSize = 0 );
	unsigned int* getBoneChildren( unsigned int* boneChildrenSize = 0 );
	AnimationInfo* getAnimation( unsigned int* animationInfoSize = 0 );
	AnimationFrameRangeInfo* getAnimationFrameRange( unsigned int* animationRangeSize = 0 );
	void setNewAnimationFrameRangeInfo( AnimationFrameRangeInfo* frameRange , unsigned int size );
	void destroyAnimationFrameRangeInfo();
	std::string savePMDData();
	bool savePMDData( std::string& location );
};