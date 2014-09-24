#pragma once
#include <string>
#include <fbxsdk.h>
struct JointData
{
	int parentIndex;
	std::string name;
	FbxAMatrix globalBindposeInverse;
	FbxNode* node;
};