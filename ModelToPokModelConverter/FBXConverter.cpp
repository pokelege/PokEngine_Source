#include "FBXConverter.h"
#include <iostream>
#include <glm.hpp>
#include <fstream>

void FBXConverter::convert( const char* input , const char* output )
{
	FbxManager* fbxManger = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create( fbxManger , IOSROOT );
	fbxManger->SetIOSettings( ios );

	FbxImporter* importer = FbxImporter::Create( fbxManger , "" );

	bool status = importer->Initialize( input , -1 , fbxManger->GetIOSettings() );
	if ( !status )
	{
		std::cout << importer->GetStatus().GetErrorString() << std::endl;
	}

	FbxScene* scene = FbxScene::Create( fbxManger , "theScene" );
	importer->Import( scene );
	importer->Destroy();

	FbxMesh* theMesh = findMesh( scene->GetRootNode() );
	

	if ( theMesh )
	{
		std::vector<VertexData> vertices;
		std::vector<unsigned short> indices;

		FbxStringList uvSets;
		theMesh->GetUVSetNames( uvSets );

		processPolygons( theMesh , vertices , indices , uvSets );

		std::fstream stream( output , std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );

		int sizeofVertices = vertices.size();
		int sizeofIndices = indices.size();
		stream.write( reinterpret_cast< const char* >( &sizeofVertices ) , sizeof( int ) );
		stream.write( reinterpret_cast< const char* >( &sizeofIndices ) , sizeof( int ) );

		for ( int i = 0; i < vertices.size(); i++ )
		{
			stream.write( reinterpret_cast< const char* >( &vertices[i] ) , sizeof( VertexData ) );
		}

		for ( int i = 0; i < indices.size(); i++ )
		{
			stream.write( reinterpret_cast< const char* >( &indices[i] ) , sizeof( unsigned short ) );
		}
		stream.close();
	}
}

FbxMesh* FBXConverter::findMesh( FbxNode* node )
{
	if ( node->GetMesh() ) { std::cout << node->GetName() << std::endl; return node->GetMesh(); }
	for ( int i = 0; i < node->GetChildCount(); i++ )
	{
		FbxMesh* theMesh = findMesh( node->GetChild( i ) );
		if ( theMesh )
		{
			return theMesh;
		}
	};
	return NULL;
}

void FBXConverter::processPolygons( FbxMesh* theMesh , std::vector<VertexData> &vertices , std::vector<unsigned short> &indices , FbxStringList &uvSets )
{
	for ( int i = 0; i < theMesh->GetPolygonCount(); i++ )
	{
		if ( theMesh->GetPolygonSize( i ) != 3 )
		{
			std::cout << "Mesh not triangulated" << std::endl;
			break;
		}

		//0
		FbxVector4 vertexResult = theMesh->GetControlPointAt( theMesh->GetPolygonVertex( i , 0 ) );
		FbxVector4 normalResult;
		theMesh->GetPolygonVertexNormal( i , 0 , normalResult );
		FbxVector2 uvResult;
		bool uvCheck;
		theMesh->GetPolygonVertexUV( i , 0 , uvSets[0] , uvResult , uvCheck );

		VertexData theData0;
		theData0.position = glm::vec3( vertexResult[0] , vertexResult[1] , vertexResult[2] );
		theData0.uv = glm::vec2( uvResult[0] , uvResult[1] );
		theData0.normal = glm::vec3( normalResult[0] , normalResult[1] , normalResult[2] );

		vertexResult = theMesh->GetControlPointAt( theMesh->GetPolygonVertex( i , 1 ) );
		theMesh->GetPolygonVertexNormal( i , 1 , normalResult );
		theMesh->GetPolygonVertexUV( i , 1 , uvSets[0] , uvResult , uvCheck );

		//1
		VertexData theData1;
		theData1.position = glm::vec3( vertexResult[0] , vertexResult[1] , vertexResult[2] );
		theData1.uv = glm::vec2( uvResult[0] , uvResult[1] );
		theData1.normal = glm::vec3( normalResult[0] , normalResult[1] , normalResult[2] );

		vertexResult = theMesh->GetControlPointAt( theMesh->GetPolygonVertex( i , 2 ) );
		theMesh->GetPolygonVertexNormal( i , 2 , normalResult );
		theMesh->GetPolygonVertexUV( i , 2 , uvSets[0] , uvResult , uvCheck );

		//2
		VertexData theData2;
		theData2.position = glm::vec3( vertexResult[0] , vertexResult[1] , vertexResult[2] );
		theData2.uv = glm::vec2( uvResult[0] , uvResult[1] );
		theData2.normal = glm::vec3( normalResult[0] , normalResult[1] , normalResult[2] );

		//tangent/bitangent
		glm::vec3 deltaPos1 = theData1.position - theData0.position;
		glm::vec3 deltaPos2 = theData2.position - theData0.position;

		glm::vec2 deltaUV1 = theData1.uv - theData0.uv;
		glm::vec2 deltaUV2 = theData2.uv - theData0.uv;

		float r = 1.0f / ( ( deltaUV1.x * deltaUV2.y ) - ( deltaUV1.y * deltaUV2.x ) );
		glm::vec3 tangent = ( ( deltaPos1 * deltaUV2.y ) - ( deltaPos2 * deltaUV1.y ) ) * r;
		glm::vec3 bitangent = ( ( deltaPos2 * deltaUV1.x ) - ( deltaPos1 * deltaUV2.x ) ) * r;

		int index = -1;
		for ( int j = 0; j < vertices.size(); j++ )
		{
			if ( vertices[j].position == theData0.position &&
				 vertices[j].uv == theData0.uv &&
				 vertices[j].normal == theData0.normal )
			{
				index = j;
				break;
			}
		}

		if ( index == -1 )
		{
			indices.push_back( ( unsigned short ) vertices.size() );
			theData0.tangent = tangent;
			theData0.bitangent = bitangent;
			vertices.push_back( theData0 );
		}
		else
		{
			indices.push_back( ( unsigned short ) index );
			vertices[index].tangent += tangent;
			vertices[index].bitangent += bitangent;
		}

		int index1 = -1;
		for ( int j = 0; j < vertices.size(); j++ )
		{
			if ( vertices[j].position == theData1.position &&
				 vertices[j].uv == theData1.uv &&
				 vertices[j].normal == theData1.normal )
			{
				index1 = j;
				break;
			}
		}

		if ( index1 == -1 )
		{
			indices.push_back( ( unsigned short ) vertices.size() );
			theData1.tangent = tangent;
			theData1.bitangent = bitangent;
			vertices.push_back( theData1 );
		}
		else
		{
			indices.push_back( ( unsigned short ) index1 );
			vertices[index1].tangent += tangent;
			vertices[index1].bitangent += bitangent;
		}

		int index2 = -1;
		for ( int j = 0; j < vertices.size(); j++ )
		{
			if ( vertices[j].position == theData2.position &&
				 vertices[j].uv == theData2.uv &&
				 vertices[j].normal == theData2.normal )
			{
				index2 = j;
				break;
			}
		}

		if ( index2 == -1 )
		{
			indices.push_back( ( unsigned short ) vertices.size() );
			theData2.tangent = tangent;
			theData2.bitangent = bitangent;
			vertices.push_back( theData2 );
		}
		else
		{
			indices.push_back( ( unsigned short ) index2 );
			vertices[index2].tangent += tangent;
			vertices[index2].bitangent += bitangent;
		}
	}
}

void FBXConverter::processSkeletonHierarchy( FbxNode* node , std::vector<JointData> &skeleton )
{
	for ( int i = 0; i < node->GetChildCount(); i++ )
	{
		FbxNode* currentNode = node->GetChild( i );
		skeletonRecurse( currentNode , skeleton , 0 , -1 );
	}
}

void FBXConverter::skeletonRecurse( FbxNode* node , std::vector<JointData> &skeleton , unsigned int index , unsigned int parentIndex )
{
	if ( node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton )
	{
		JointData joint;
		joint.parentIndex = parentIndex;
		joint.name = node->GetName();
		skeleton.push_back( joint );
	}
	//might be in if statement
	for ( int i = 0; i < node->GetChildCount(); i++ )
	{
		//skeleton size changes so parent can change
		skeletonRecurse( node->GetChild(i) , skeleton , skeleton.size() , index );
	}
}