#pragma once
#pragma warning(disable : 4201)
#pragma warning(disable:4239)
#pragma warning(disable:4251)
#include <PokEngineExportHeader.h>
#include <glm.hpp>
#include <string>

class POKENGINE_SHARED Node
{
public:
	struct POKENGINE_SHARED NodeConnectionHolder
	{
		float weight;
		Node* connectedNode;
		NodeConnectionHolder();
	};
private:
	unsigned int physicalMaxConnections;
	unsigned int currentMaxConnections;
	glm::vec3 position;
	NodeConnectionHolder* connections;
public:
	Node( glm::vec3& position = glm::vec3() , unsigned int physicalMaxConnections = 0 );
	void cleanUp( unsigned int* test = nullptr );
	void updateNodePointers( Node* oldStartPointer , Node* newStartPointer );
	bool connectDraw;
	void resetConnections();
	glm::vec3 getPosition();
	unsigned int getNumMaxConnections();
	void setPosition( glm::vec3& position );
	void connect( Node* child, float weight );
	void disconnect( Node* child );
	void swapNodes( Node* toSwap );
	void getConnections( NodeConnectionHolder** arrayHolder , unsigned int* arrayMaxHolder );
	void updateConnection( Node* firstPointer , Node* lastPointer );
	bool intersected( glm::vec3& cameraPos , glm::vec3& ray , float* closestDistanceToReturn = nullptr, float* otherDistanceToReturn = nullptr );
	void exportNode( Node* startPointer, std::string* nodeDataHolder, std::string* connectionDataHolder );
	unsigned int importNode( const char* nodeData , const char* connectionData , Node* startPointer );
};