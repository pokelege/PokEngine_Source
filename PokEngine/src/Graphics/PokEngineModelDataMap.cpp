#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\VertexInfo.h>
#include <Graphics\BoneInfo.h>
#include <Graphics\AnimationInfo.h>
PokEngineModelDataMap::PokEngineModelDataMap() {}
PokEngineModelDataMap::PokEngineModelDataMap( std::ifstream& stream )
{
	stream.read( reinterpret_cast< char* >( &sizeofVertexData ) , sizeof( unsigned int ) * 6 );
	vertexData = new char[sizeof( VertexInfo ) * sizeofVertexData];
	stream.read( vertexData , sizeof( VertexInfo ) * sizeofVertexData );
	indexData = new char[sizeof( unsigned short ) * sizeofIndexData];
	stream.read( indexData , sizeof( unsigned short ) * sizeofIndexData );
	blendingIndexData = new char[sizeof( unsigned int ) * sizeofBlendingData];
	stream.read( blendingIndexData , sizeof( unsigned int ) * sizeofBlendingData );
	blendingWeightData = new char[sizeof( float ) * sizeofBlendingData];
	stream.read( blendingWeightData , sizeof( float ) * sizeofBlendingData );
	boneData = new char[sizeof( BoneInfo ) * sizeofBoneData];
	stream.read( boneData , sizeof( BoneInfo ) * sizeofBoneData );
	boneChildrenData = new char[sizeof( unsigned int ) * sizeofBoneChildData];
	stream.read( boneChildrenData , sizeof( unsigned int ) * sizeofBoneChildData );
	boneAnimationData = new char[sizeof( AnimationInfo ) * sizeofBoneAnimationData];
	stream.read( boneAnimationData , sizeof( AnimationInfo ) * sizeofBoneAnimationData );
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
unsigned int* PokEngineModelDataMap::getBlendingIndexData( unsigned int* blendingIndexSize )
{
	if(blendingIndexSize) *blendingIndexSize = sizeofBlendingData;
	return reinterpret_cast< unsigned int* >( blendingIndexData );
}
float* PokEngineModelDataMap::getBlendingWeightData( unsigned int* blendingWeightSize )
{
	if ( blendingWeightSize ) *blendingWeightSize = sizeofBlendingData;
	return reinterpret_cast< float* >( blendingWeightData );
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