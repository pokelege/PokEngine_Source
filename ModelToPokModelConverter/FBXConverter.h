#include <fbxsdk.h>
#include <vector>
#include "JointData.h"
#include "VertexData.h"
class FBXConverter
{
	static FbxMesh* findMesh( FbxNode* node );
	static void processPolygons( FbxMesh* theMesh , std::vector<VertexData> &vertices , std::vector<unsigned short> &indices , FbxStringList &uvSets );
	static void processSkeletonHierarchy( FbxNode* node, std::vector<JointData> &skeleton );
	static void skeletonRecurse( FbxNode* node , std::vector<JointData> &skeleton , unsigned int index , unsigned int parentIndex );
public:
	static void convert( const char* input , const char* output = "" );
};