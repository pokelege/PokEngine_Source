#pragma once
#pragma warning (disable:4251)
#pragma warning (disable:4201)
#include <PokEngineExportHeader.h>
#include <glm.hpp>
class PokEngineModelDataMap;
struct AnimationInfo;
struct POKENGINE_SHARED BoneInfo
{
	glm::mat4 offsetMatrix;
	int childDataStart;
	int childDataEnd;
	int animationDataStart;
	int animationDataEnd;

	const BoneInfo* getChild( unsigned int index , PokEngineModelDataMap& modelData );
	unsigned int childrenSize();

	AnimationInfo* getAnimation( unsigned int index , PokEngineModelDataMap& modelData );
	unsigned int animationSize();
};