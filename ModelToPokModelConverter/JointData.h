#pragma once
#include <string>
#include <glm.hpp>
#include <vector>
struct AnimationData
{
	unsigned int frame;
	glm::vec3 scale;
	glm::vec3 translation;
	glm::vec3 rotation;
};
struct JointData
{
	std::string name;
	glm::mat4 offsetMatrix;
	std::vector<int> children;
	std::vector<AnimationData> animation;
};