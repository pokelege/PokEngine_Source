#include <Graphics\BoneInfo.h>
#include <Graphics\AnimationInfo.h>
#include <Graphics\PokEngineModelDataMap.h>
const BoneInfo* BoneInfo::getChild( unsigned int index , PokEngineModelDataMap& modelData )
{
	BoneInfo* bone = 0;
	if ( childDataStart > -1 && childDataEnd > -1 )
	{
		unsigned int trueIndex = childDataStart + index;
		if ( trueIndex < (unsigned int)childDataEnd )
		{
			unsigned int childDataCount;
			unsigned int* childData = modelData.getBoneChildren( &childDataCount );
			if ( childDataCount && trueIndex < childDataCount )
			{
				unsigned int boneIndex = childData[trueIndex];
				unsigned int boneCount;
				BoneInfo* bones = modelData.getBoneData( &boneCount );
				if ( boneCount && boneIndex < boneCount )
				{
					bone = &bones[boneIndex];
				}
			}
		}
	}
	return bone;
}

unsigned int BoneInfo::childrenSize()
{
	return childDataEnd - childDataStart;
}

AnimationInfo* BoneInfo::getAnimation( unsigned int index , PokEngineModelDataMap& modelData )
{
	AnimationInfo* animation = 0;
	if ( animationDataStart > -1 && animationDataEnd > -1 )
	{
		unsigned int trueIndex = animationDataStart + index;
		if ( trueIndex < (unsigned int)animationDataEnd )
		{
			unsigned int animationDataCount;
			AnimationInfo* animations = modelData.getAnimation( &animationDataCount );
			if ( animationDataCount && trueIndex < animationDataCount )
			{
				animation = &animations[trueIndex];
			}
		}
	}
	return animation;
}

unsigned int BoneInfo::animationSize()
{
	return animationDataEnd - animationDataStart;
}