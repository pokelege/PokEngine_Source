#pragma once
#pragma warning(disable : 4201)
#pragma warning(disable:4239)
#pragma warning(disable:4251)
#include <PokEngineExportHeader.h>
#include <glm.hpp>

class POKENGINE_SHARED GameNode
{
public:
	struct POKENGINE_SHARED GameNodeConnectionHolder
	{
		float weight;
		GameNode* connectedNode;
	};
private:
	unsigned int numConnections;
	glm::vec3 position;
	GameNodeConnectionHolder* connections;
public:
	void updateNodePointers( GameNode* oldStartPointer , GameNode* newStartPointer );
	glm::vec3 getPosition();
	void setPosition( glm::vec3& position );
	void getConnections( GameNodeConnectionHolder** arrayHolder , unsigned int* arrayMaxHolder );
	void updateConnection( GameNode* firstPointer , GameNode* lastPointer );
	bool intersected( glm::vec3& cameraPos , glm::vec3& ray , float* closestDistanceToReturn = nullptr , float* otherDistanceToReturn = nullptr );
	void fixPointers( GameNode* startPointer, GameNodeConnectionHolder* startConnections );
};