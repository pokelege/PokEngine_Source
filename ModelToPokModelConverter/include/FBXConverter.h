#pragma once
#include <fbxsdk.h>
#include <vector>

#include <JointData.h>
#include <VertexData.h>
#include <FBXConverterExportHeader.h>
struct FBXCONVERTER_SHARED IndexData
{
	unsigned short index;
	unsigned int oldControlPoint;
};
class FBXCONVERTER_SHARED FBXConverter
{
	bool converting;
	FbxMesh* findMesh( FbxNode* node );
	void processPolygons( FbxMesh* theMesh , std::vector<VertexData> &vertices , std::vector<IndexData> &indices , FbxStringList &uvSets );
	void processSkeletonHierarchy( FbxNode* node, std::vector<JointData> &skeleton );
	void skeletonRecurse( FbxNode* node , std::vector<JointData> &skeleton , unsigned int index , int parentIndex );
	void processAnimations( FbxNode* node , std::vector<JointData> &skeleton, std::vector<VertexData> &verts, std::vector<IndexData> &indices );
public:
	FBXConverter();
	void convert( const char* input , const char* output = "" );
};

bool frameCompare( FbxTime i , FbxTime j );