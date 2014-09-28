#include <Graphics\VertexInfo.h>
#include <Graphics\PokEngineModelDataMap.h>
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