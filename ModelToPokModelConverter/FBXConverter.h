#include <fbxsdk.h>
#include <vector>
#include "JointData.h"
#include "VertexData.h"
struct IndexData
{
	unsigned short index;
	unsigned int oldControlPoint;
};
class FBXConverter
{
	static FbxMesh* findMesh( FbxNode* node );
	static void processPolygons( FbxMesh* theMesh , std::vector<VertexData> &vertices , std::vector<IndexData> &indices , FbxStringList &uvSets );
	static void processSkeletonHierarchy( FbxNode* node, std::vector<JointData> &skeleton );
	static void skeletonRecurse( FbxNode* node , std::vector<JointData> &skeleton , unsigned int index , int parentIndex );
	static void processAnimations( FbxNode* node , std::vector<JointData> &skeleton, std::vector<VertexData> &verts, std::vector<IndexData> &indices );
public:
	static void convert( const char* input , const char* output = "" );
};