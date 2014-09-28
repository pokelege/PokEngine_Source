#include <Graphics\VertexInfo.h>
#include <Graphics\PokEngineModelDataMap.h>

unsigned int VertexInfo::POSITION_OFFSET = 0;
unsigned int VertexInfo::UV_OFFSET = sizeof(glm::vec3);
unsigned int VertexInfo::NORMAL_OFFSET = sizeof( glm::vec3 ) + sizeof(glm::vec2);
unsigned int VertexInfo::TANGENT_OFFSET = (sizeof( glm::vec3 ) * 2) + sizeof( glm::vec2 );
unsigned int VertexInfo::BITANGENT_OFFSET = ( sizeof( glm::vec3 ) * 3 ) + sizeof( glm::vec2 );
unsigned int VertexInfo::BLENDINGSTART_OFFSET = ( sizeof( glm::vec3 ) * 4 ) + sizeof( glm::vec2 );
unsigned int VertexInfo::BLENDINGEND_OFFSET = ( sizeof( glm::vec3 ) * 4 ) + sizeof( glm::vec2 ) + sizeof(int);
unsigned int VertexInfo::STRIDE = ( sizeof( glm::vec3 ) * 4 ) + sizeof( glm::vec2 ) + (sizeof( int ) * 2);

void VertexInfo::getBlendingInfo( unsigned int index , PokEngineModelDataMap& modelData , unsigned int* blendingIndex , float* blendingWeight )
{
	if ( blendingDataStart > -1 && blendingDataEnd > -1 )
	{
		unsigned int trueIndex = index + (unsigned int) blendingDataStart;
		if ( trueIndex < ( unsigned int ) blendingDataEnd )
		{
			if ( blendingIndex )
			{
				unsigned int blendingIndexDataSize;
				unsigned int* blendingIndexData = modelData.getBlendingIndexData( &blendingIndexDataSize );
				if ( blendingIndexData && trueIndex < blendingIndexDataSize )
					*blendingIndex = blendingIndexData[trueIndex];
			}

			if ( blendingWeight )
			{
				unsigned int blendingWeightDataSize;
				float* blendingWeightData = modelData.getBlendingWeightData( &blendingWeightDataSize );
				if ( blendingWeightData && trueIndex < blendingWeightDataSize )
					*blendingWeight = blendingWeightData[trueIndex];
			}
		}
	}
}

unsigned int VertexInfo::size()
{
	if ( blendingDataStart > -1 && blendingDataEnd > -1 )
	{
		return blendingDataEnd - blendingDataStart;
	}
	else
	{
		return 0;
	}
}