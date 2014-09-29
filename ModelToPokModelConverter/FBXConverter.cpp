#include "FBXConverter.h"
#include <iostream>
#include <glm.hpp>
#include <fstream>
#include <string>
#include <algorithm>
#define DATASTRING(data) std::string( reinterpret_cast< const char* >( &data ) , sizeof( data ) );

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
		std::vector<IndexData> indices;

		FbxStringList uvSets;
		theMesh->GetUVSetNames( uvSets );

		processPolygons( theMesh , vertices , indices , uvSets );

		std::vector<JointData> skeleton;
		processSkeletonHierarchy( scene->GetRootNode() , skeleton );
		processAnimations( theMesh->GetNode() , skeleton , vertices , indices );

		std::string modelData;

		std::vector<BlendingIndexWeightPair> blendingData;
		for ( int i = 0; i < vertices.size(); i++ )
		{
			modelData += DATASTRING( vertices[i].position );
			modelData += DATASTRING( vertices[i].uv );
			modelData += DATASTRING( vertices[i].normal );
			modelData += DATASTRING( vertices[i].tangent );
			modelData += DATASTRING( vertices[i].bitangent );
			int blendingDataStart, blendingDataEnd;
			if ( vertices[i].blendingInfo.size() )
			{
				blendingDataStart = blendingData.size();
				for ( unsigned int j = 0; j < vertices[i].blendingInfo.size(); ++j )
				{
					blendingData.push_back( vertices[i].blendingInfo[j] );
				}
				blendingDataEnd = blendingData.size();
			}
			else
			{
				blendingDataStart = -1;
				blendingDataEnd = -1;
			}
			modelData += DATASTRING( blendingDataStart );
			modelData += DATASTRING( blendingDataEnd );
		}

		for ( int i = 0; i < indices.size(); i++ )
		{
			modelData += DATASTRING( indices[i].index );
		}

		std::string boneData;
		std::vector<unsigned int> boneChildren;
		std::vector<AnimationData> boneAnimation;
		for ( int i = 0; i < skeleton.size(); ++i )
		{
			boneData += DATASTRING( skeleton[i].offsetMatrix );
			int childDataStart , childDataEnd , animationDataStart , animationDataEnd;
			if ( skeleton[i].children.size() )
			{
				childDataStart = boneChildren.size();
				for ( int j = 0; j < skeleton[i].children.size(); ++j )
				{
					boneChildren.push_back( skeleton[i].children[j] );
				}
				childDataEnd = boneChildren.size();
			}
			else
			{
				childDataStart = -1;
				childDataEnd = -1;
			}

			if ( skeleton[i].animation.size() )
			{
				animationDataStart = boneAnimation.size();
				for ( int j = 0; j < skeleton[i].animation.size(); ++j )
				{
					boneAnimation.push_back( skeleton[i].animation[j] );
				}
				animationDataEnd = boneAnimation.size();
			}
			else
			{
				animationDataStart = -1;
				animationDataEnd = -1;
			}
			boneData += DATASTRING( childDataStart );
			boneData += DATASTRING( childDataEnd );
			boneData += DATASTRING( animationDataStart );
			boneData += DATASTRING( animationDataEnd );
		}

		std::fstream stream( output , std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );

		unsigned int sizeofVertices = vertices.size();
		stream.write( reinterpret_cast< char* >( &sizeofVertices ) , sizeof( sizeofVertices ));
		unsigned int sizeofIndices = indices.size();
		stream.write( reinterpret_cast< char* >( &sizeofIndices ) , sizeof( sizeofIndices ) );

		unsigned int sizeofBlendingData = blendingData.size();
		stream.write( reinterpret_cast<char*>( &sizeofBlendingData ) , sizeof( sizeofBlendingData ) );

		unsigned int sizeofBoneData = skeleton.size();
		stream.write( reinterpret_cast<char*>( &sizeofBoneData ) , sizeof( sizeofBoneData ) );

		unsigned int sizeofBoneChildData = boneChildren.size();
		stream.write( reinterpret_cast< char* >( &sizeofBoneChildData ) , sizeof( sizeofBoneChildData ));

		unsigned int sizeofBoneAnimationData = boneAnimation.size();
		stream.write( reinterpret_cast< char* >( &sizeofBoneAnimationData ) , sizeof( sizeofBoneAnimationData ) );

		stream.write( modelData.c_str() , modelData.size() );

		for ( unsigned int i = 0; i < blendingData.size(); ++i )
		{
			stream.write( reinterpret_cast< char* >( &blendingData[i].blendingIndex ) , sizeof( blendingData[i].blendingIndex ) );
		}

		for ( unsigned int i = 0; i < blendingData.size(); ++i )
		{
			stream.write( reinterpret_cast< char* >( &blendingData[i].blendingWeight ) , sizeof( blendingData[i].blendingWeight ) );
		}

		stream.write( boneData.c_str() , boneData.size() );

		for ( unsigned int i = 0; i < boneChildren.size(); ++i )
		{
			stream.write( reinterpret_cast< char* >( &boneChildren[i] ) , sizeof( boneChildren[i] ) );
		}

		for ( unsigned int i = 0; i < boneAnimation.size(); ++i )
		{
			stream.write( reinterpret_cast< char* >( &boneAnimation[i] ) , sizeof( boneAnimation[i] ) );
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

void FBXConverter::processPolygons( FbxMesh* theMesh , std::vector<VertexData> &vertices , std::vector<IndexData> &indices , FbxStringList &uvSets )
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
			IndexData data;
			data.index = ( unsigned short ) vertices.size();
			data.oldControlPoint = theMesh->GetPolygonVertex( i , 0 );
			indices.push_back( data );
			theData0.tangent = tangent;
			theData0.bitangent = bitangent;
			vertices.push_back( theData0 );
		}
		else
		{
			IndexData data;
			data.index = ( unsigned short ) index;
			data.oldControlPoint = theMesh->GetPolygonVertex( i , 0 );
			indices.push_back( data );
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
			IndexData data;
			data.index = ( unsigned short ) vertices.size();
			data.oldControlPoint = theMesh->GetPolygonVertex( i , 1 );
			indices.push_back( data );
			theData1.tangent = tangent;
			theData1.bitangent = bitangent;
			vertices.push_back( theData1 );
		}
		else
		{
			IndexData data;
			data.index = ( unsigned short ) index1;
			data.oldControlPoint = theMesh->GetPolygonVertex( i , 1 );
			indices.push_back( data );
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
			IndexData data;
			data.index = ( unsigned short ) vertices.size();
			data.oldControlPoint = theMesh->GetPolygonVertex( i , 2 );
			indices.push_back( data );
			theData2.tangent = tangent;
			theData2.bitangent = bitangent;
			vertices.push_back( theData2 );
		}
		else
		{
			IndexData data;
			data.index = ( unsigned short ) index2;
			data.oldControlPoint = theMesh->GetPolygonVertex( i , 2 );
			indices.push_back( data );
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

void FBXConverter::skeletonRecurse( FbxNode* node , std::vector<JointData> &skeleton , unsigned int index , int parentIndex )
{
	if ( node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton )
	{
		JointData joint;
		joint.name = node->GetName();
		skeleton.push_back( joint );
		if(parentIndex >= 0) skeleton[parentIndex].children.push_back( index );
	}
	for ( int i = 0; i < node->GetChildCount(); i++ )
	{
		skeletonRecurse( node->GetChild(i) , skeleton , skeleton.size() , index );
	}
}

void FBXConverter::processAnimations( FbxNode* node , std::vector<JointData> &skeleton , std::vector<VertexData> &verts , std::vector<IndexData> &indices )
{
	FbxMesh* theMesh = node->GetMesh();

	FbxAnimStack* animationStack = node->GetScene()->GetSrcObject<FbxAnimStack>();
	FbxAnimLayer* animationLayer = animationStack->GetMember<FbxAnimLayer>();
	std::vector<FbxTime> frames;
	for ( unsigned int curveNodeIndex = 0; curveNodeIndex < animationLayer->GetMemberCount(); ++curveNodeIndex )
	{
		FbxAnimCurveNode* currentCurve = animationLayer->GetMember<FbxAnimCurveNode>( curveNodeIndex );

		for ( unsigned int channelIndex = 0; channelIndex < currentCurve->GetChannelsCount(); ++channelIndex )
		{
			for ( unsigned int curve = 0; curve < currentCurve->GetCurveCount( channelIndex ); ++curve )
			{
				FbxAnimCurve* theCurveVictim = currentCurve->GetCurve( channelIndex , curve );
				for ( unsigned int keyIndex = 0; keyIndex < theCurveVictim->KeyGetCount(); ++keyIndex )
				{
					bool alreadyIn = false;
					for ( unsigned int frameIndex = 0; frameIndex < frames.size(); ++frameIndex )
					{
						if ( ( unsigned int ) frames[frameIndex].GetFrameCount() == ( unsigned int ) theCurveVictim->KeyGet( keyIndex ).GetTime().GetFrameCount() )
						{
							alreadyIn = true;
							//std::cout << frames[frameIndex].GetFrameCount() << " " << theCurveVictim->KeyGet( keyIndex ).GetTime().GetFrameCount() << " " << alreadyIn << std::endl;
							break;
						}
					}
					if(!alreadyIn) frames.push_back(theCurveVictim->KeyGet( keyIndex ).GetTime());
				}
			}
		}
	}
	
	std::sort( frames.begin() , frames.end() , frameCompare );

	FbxAMatrix geoTransform( node->GetGeometricTranslation( FbxNode::eSourcePivot ) , node->GetGeometricRotation( FbxNode::eSourcePivot ) , node->GetGeometricScaling( FbxNode::eSourcePivot ) );


	for ( unsigned int i = 0; i < theMesh->GetDeformerCount(); ++i )
	{
		FbxSkin* theSkin = reinterpret_cast< FbxSkin* >( theMesh->GetDeformer( i , FbxDeformer::eSkin ) );
		if ( !theSkin ) continue;

		for ( unsigned int j = 0; j < theSkin->GetClusterCount(); ++j )
		{
			FbxCluster* cluster = theSkin->GetCluster( j );
			std::string jointName = cluster->GetLink()->GetName();
			unsigned int currentJointIndex = 0;
			for ( unsigned int k = 0; k < skeleton.size(); ++k )
			{
				if ( !skeleton[k].name.compare( jointName ) )
				{
					currentJointIndex = k;
					break;
				}
			}


			FbxAMatrix transformMatrix, transformLinkMatrix, offsetMatrix;
			cluster->GetTransformMatrix( transformMatrix );
			cluster->GetTransformLinkMatrix( transformLinkMatrix );
			offsetMatrix = transformLinkMatrix.Inverse() * transformMatrix * geoTransform;
			
			for ( unsigned int row = 0; row < 4; ++row )
			{
				for ( unsigned int column = 0; column < 4; ++column )
				{
					skeleton[currentJointIndex].offsetMatrix[row][column] = offsetMatrix.Get( row , column );
				}
			}

			for ( unsigned int k = 0; k < cluster->GetControlPointIndicesCount(); ++k )
			{
				BlendingIndexWeightPair weightPair;
				weightPair.blendingIndex = currentJointIndex;
				weightPair.blendingWeight = (float)cluster->GetControlPointWeights()[k];
				unsigned int controlPoint = cluster->GetControlPointIndices()[k];
				for ( unsigned int z = 0; z < indices.size(); ++z )
				{
					if ( indices[z].oldControlPoint == controlPoint )
					{
						verts[indices[z].index].blendingInfo.push_back( weightPair );
						break;
					}
				}
			}

			for ( unsigned int frameIndex = 0; frameIndex < frames.size(); ++frameIndex )
			{
				FbxVector4 translation = cluster->GetLink()->EvaluateLocalTranslation( frames[frameIndex] );
				FbxVector4 rotation = cluster->GetLink()->EvaluateLocalRotation( frames[frameIndex] );
				FbxVector4 scale = cluster->GetLink()->EvaluateLocalScaling( frames[frameIndex] );
				AnimationData animData;
				animData.frame = frames[frameIndex].GetFrameCount();
				animData.translation = glm::vec3(translation[0],translation[1],translation[2]);
				animData.rotation = glm::vec3(rotation[0],rotation[1],rotation[2]);
				animData.scale = glm::vec3(scale[0],scale[1],scale[2]);
				skeleton[currentJointIndex].animation.push_back( animData );
			}
		}
	}
}

bool frameCompare( FbxTime i , FbxTime j ) { return ( i.GetFrameCount() < j.GetFrameCount() ); }