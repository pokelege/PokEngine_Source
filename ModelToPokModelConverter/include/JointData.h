#pragma once
#include <string>
#include <glm.hpp>
#include <vector>
#include <FBXConverterExportHeader.h>
struct FBXCONVERTER_SHARED AnimationData
{
	int frame;
	glm::vec3 scale;
	glm::vec3 translation;
	glm::vec3 rotation;
};
struct FBXCONVERTER_SHARED JointData
{
	bool isParent;
	std::string name;
	glm::mat4 offsetMatrix;
	std::vector<int> children;
	std::vector<AnimationData> animation;
};