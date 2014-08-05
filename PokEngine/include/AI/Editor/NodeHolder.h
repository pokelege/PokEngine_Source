#pragma once
#pragma warning(disable : 4201)
#pragma warning(disable:4239)
#pragma warning(disable:4251)
#include <PokEngineExportHeader.h>
#include <string>
#include <glm.hpp>
class Node;
class POKENGINE_SHARED NodeHolder
{
	unsigned int physicalMaxNodes;
	unsigned int currentMaxNodes;
	Node* nodes;

public:
	NodeHolder( unsigned int physicalMaxNodes = 10 );
	~NodeHolder();
	int getcurrentMaxNodes();
	void addNode( glm::vec3& node );
	void deleteNode( unsigned int id );
	Node* getNode( unsigned int id );
	std::string getNodeData();
	void connectDisconnectNode( unsigned int& id , unsigned int id2 , const float* weight = nullptr );
	void setNodeData( const char* toSet );
};