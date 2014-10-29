#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\VertexInfo.h>
#include <Graphics\BoneInfo.h>
#include <Graphics\AnimationInfo.h>
#include <Graphics\AnimationFrameRangeInfo.h>
#include <fstream>

PokEngineModelDataMap::PokEngineModelDataMap() : vertexData(0), indexData(0), boneData(0), boneChildrenData(0), boneAnimationData(0),animationRangeInfo(0) {}
PokEngineModelDataMap::PokEngineModelDataMap( std::ifstream& stream )
{
	stream.read( reinterpret_cast< char* >( &sizeofVertexData ) , sizeof( unsigned int ) * 6 );
	vertexData = new char[sizeof( VertexInfo ) * sizeofVertexData];
	stream.read( vertexData , sizeof( VertexInfo ) * sizeofVertexData );
	indexData = new char[sizeof( unsigned short ) * sizeofIndexData];
	stream.read( indexData , sizeof( unsigned short ) * sizeofIndexData );
	boneData = new char[sizeof( BoneInfo ) * sizeofBoneData];
	stream.read( boneData , sizeof( BoneInfo ) * sizeofBoneData );
	boneChildrenData = new char[sizeof( unsigned int ) * sizeofBoneChildData];
	stream.read( boneChildrenData , sizeof( unsigned int ) * sizeofBoneChildData );
	boneAnimationData = new char[sizeof( AnimationInfo ) * sizeofBoneAnimationData];
	stream.read( boneAnimationData , sizeof( AnimationInfo ) * sizeofBoneAnimationData );
	animationRangeInfo = new char[sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo];
	stream.read( animationRangeInfo , sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo );
}
PokEngineModelDataMap::PokEngineModelDataMap( const char* rawData )
{
	unsigned int index = 0;
	memcpy( &sizeofVertexData , &rawData[index] , sizeof( unsigned int ) * 6 );
	index += sizeof( unsigned int ) * 6;

	vertexData = new char[sizeof( VertexInfo ) * sizeofVertexData];
	memcpy( vertexData , &rawData[index], sizeof( VertexInfo ) * sizeofVertexData );
	index += sizeof( VertexInfo ) * sizeofVertexData;

	indexData = new char[sizeof( unsigned short ) * sizeofIndexData];
	memcpy( indexData , &rawData[index], sizeof( unsigned short ) * sizeofIndexData );
	index += sizeof( unsigned short ) * sizeofIndexData;

	boneData = new char[sizeof( BoneInfo ) * sizeofBoneData];
	memcpy( boneData, &rawData[index] , sizeof( BoneInfo ) * sizeofBoneData );
	index += sizeof( BoneInfo ) * sizeofBoneData;

	boneChildrenData = new char[sizeof( unsigned int ) * sizeofBoneChildData];
	memcpy( boneChildrenData, &rawData[index] , sizeof( unsigned int ) * sizeofBoneChildData );
	index += sizeof( unsigned int ) * sizeofBoneChildData;

	boneAnimationData = new char[sizeof( AnimationInfo ) * sizeofBoneAnimationData];
	memcpy( boneAnimationData, &rawData[index] , sizeof( AnimationInfo ) * sizeofBoneAnimationData );
	index += sizeof( AnimationInfo ) * sizeofBoneAnimationData;

	animationRangeInfo = new char[sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo];
	memcpy( animationRangeInfo , &rawData[index], sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo );
}
VertexInfo* PokEngineModelDataMap::getVertexData( unsigned int* vertexInfoSize)
{
	if(vertexInfoSize) *vertexInfoSize = sizeofVertexData;
	return reinterpret_cast< VertexInfo* >( vertexData );
}
unsigned short* PokEngineModelDataMap::getIndexData( unsigned int* indexSize  )
{
	if(indexSize) *indexSize = sizeofIndexData;
	return reinterpret_cast<unsigned short*>(indexData);
}
BoneInfo* PokEngineModelDataMap::getBoneData( unsigned int* boneDataSize )
{
	if ( boneDataSize ) *boneDataSize = sizeofBoneData;
	return reinterpret_cast< BoneInfo* >( boneData );
}
unsigned int* PokEngineModelDataMap::getBoneChildren( unsigned int* boneChildrenSize )
{
	if ( boneChildrenSize ) *boneChildrenSize = sizeofBoneChildData;
	return reinterpret_cast< unsigned int* >( boneChildrenData );
}
AnimationInfo* PokEngineModelDataMap::getAnimation( unsigned int* animationInfoSize )
{
	if ( animationInfoSize ) *animationInfoSize = sizeofBoneAnimationData;
	return reinterpret_cast< AnimationInfo* >( boneAnimationData );
}

AnimationFrameRangeInfo* PokEngineModelDataMap::getAnimationFrameRange( unsigned int* animationRangeSize )
{
	if ( animationRangeInfo ) *animationRangeSize = sizeofAnimationRangeInfo;
	return reinterpret_cast< AnimationFrameRangeInfo* >(animationRangeInfo );
}

void PokEngineModelDataMap::setNewAnimationFrameRangeInfo( AnimationFrameRangeInfo* frameRange , unsigned int size )
{
	if ( animationRangeInfo ) destroyAnimationFrameRangeInfo();
	sizeofAnimationRangeInfo = size;
	animationRangeInfo = new char[sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo];
	memcpy( animationRangeInfo , reinterpret_cast< char* >( frameRange ) , sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo );
}

void PokEngineModelDataMap::destroyAnimationFrameRangeInfo()
{
	if ( !animationRangeInfo ) return;
	delete[] animationRangeInfo;
	animationRangeInfo = 0;
	sizeofAnimationRangeInfo = 0;
}

PokEngineModelDataMap::~PokEngineModelDataMap()
{
	if ( vertexData ) delete[] vertexData;
	if ( indexData ) delete[] indexData;
	if ( boneData ) delete[] boneData;
	if ( boneChildrenData ) delete[] boneChildrenData;
	if ( boneAnimationData ) delete[] boneAnimationData;
	if ( animationRangeInfo ) delete[] animationRangeInfo;
}

std::string PokEngineModelDataMap::savePMDData()
{
	std::string data;
	data += std::string( reinterpret_cast< const char* >( &sizeofVertexData ) , sizeof( unsigned int ) * 6 );
	data += std::string( vertexData , sizeof( VertexInfo ) * sizeofVertexData );
	data += std::string( indexData , sizeof( unsigned short ) * sizeofIndexData );
	data += std::string( boneData , sizeof( BoneInfo ) * sizeofBoneData );
	data += std::string( boneChildrenData , sizeof( unsigned int ) * sizeofBoneChildData );
	data += std::string( boneAnimationData , sizeof( AnimationInfo ) * sizeofBoneAnimationData );
	data += std::string( animationRangeInfo , sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo );
	return data;
}
bool PokEngineModelDataMap::savePMDData( std::string& location )
{
	std::fstream stream( location , std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );

	stream.write( reinterpret_cast< char* >( &sizeofVertexData ) , sizeof( unsigned int ) * 6 );

	stream.write( vertexData , sizeof( VertexInfo ) * sizeofVertexData );

	stream.write( indexData , sizeof( unsigned short ) * sizeofIndexData );

	stream.write( boneData , sizeof( BoneInfo ) * sizeofBoneData );

	stream.write( boneChildrenData , sizeof( unsigned int ) * sizeofBoneChildData );

	stream.write( boneAnimationData , sizeof( AnimationInfo ) * sizeofBoneAnimationData );

	stream.write( animationRangeInfo , sizeof( AnimationFrameRangeInfo ) * sizeofAnimationRangeInfo );
	stream.close();
	return true;
}