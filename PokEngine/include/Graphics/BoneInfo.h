#pragma once
#include <PokEngineExportHeader.h>
#include <glm.hpp>
struct AnimationData
{
	unsigned int frame;
	glm::vec3 scale;
	glm::vec3 translation;
	glm::vec3 rotation;
};
struct POKENGINE_SHARED BoneInfo
{
	glm::mat4 offsetMatrix;
	int childDataStart;
	int childDataEnd;
	int animationDataStart;
	int animationDataEnd;
};

struct POKENGINE_SHARED FullBoneInfo
{
	unsigned int numBones;
	BoneInfo* bones;
	int* children;
	AnimationData* animations;
};