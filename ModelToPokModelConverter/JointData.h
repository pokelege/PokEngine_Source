#pragma once
#include <string>
#include <fbxsdk.h>
#include <glm.hpp>
#include <vector>
struct AnimationData
{
	unsigned int frame;
	FbxVector4 translation;
	FbxVector4 rotation;
	FbxVector4 scale;
};
struct JointData
{
	std::string name;
	std::vector<AnimationData> animation;
	std::vector<int> children;
};