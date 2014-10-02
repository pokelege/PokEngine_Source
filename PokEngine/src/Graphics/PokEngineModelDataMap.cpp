#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\VertexInfo.h>
#include <Graphics\BoneInfo.h>
#include <Graphics\AnimationInfo.h>
PokEngineModelDataMap::PokEngineModelDataMap() {}
PokEngineModelDataMap::PokEngineModelDataMap( std::ifstream& stream )
{
	stream.read( reinterpret_cast< char* >( &sizeofVertexData ) , sizeof( unsigned int ) * 5 );
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
}
PokEngineModelDataMap::PokEngineModelDataMap( const char* rawData )
{
	unsigned int index = 0;
	memcpy( &sizeofVertexData , &rawData[index] , sizeof( unsigned int ) * 5 );
	index += sizeof( unsigned int ) * 5;

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