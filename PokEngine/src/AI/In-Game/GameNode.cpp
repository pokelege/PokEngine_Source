#include "AI/In-Game/GameNode.h"

void GameNode::updateNodePointers( GameNode* oldStartPointer , GameNode* newStartPointer )
{
	unsigned int oldP = ( unsigned int ) oldStartPointer;
	unsigned int newP = ( unsigned int ) newStartPointer;

	for ( unsigned int i = 0; i < numConnections; i++ )
	{
		GameNodeConnectionHolder* theConnections = ( GameNodeConnectionHolder* ) ( this + 1 );
		unsigned int temp = ( unsigned int ) theConnections[i].connectedNode;
		temp += newP - oldP;
		theConnections[i].connectedNode = ( GameNode* ) temp;
	}
}

void GameNode::setPosition( glm::vec3& position )
{
	this->position = position;
}

glm::vec3 GameNode::getPosition()
{
	return position;
}


void GameNode::getConnections( GameNodeConnectionHolder** arrayHolder , unsigned int* arrayMaxHolder )
{
	*arrayHolder = connections;
	*arrayMaxHolder = numConnections;

}

void GameNode::updateConnection( GameNode* firstPointer , GameNode* lastPointer )
{
	for ( unsigned int i = 0; i < numConnections; i++ )
	{
		if ( connections[i].connectedNode == firstPointer )
		{
			connections[i].connectedNode = lastPointer;
			break;
		}
	}
}

bool GameNode::intersected( glm::vec3& cameraPos , glm::vec3& ray , float* closestDistanceToReturn , float* otherDistanceToReturn )
{
	float b = glm::dot( ray , ( cameraPos - position ) );
	float c = glm::dot( ( cameraPos - position ) , ( cameraPos - position ) ) - 1;

	float squareRtTest = ( b*b ) - c;

	if ( squareRtTest > 0 )
	{
		if ( closestDistanceToReturn != nullptr || otherDistanceToReturn != nullptr )
		{
			float positiveTest = -b + glm::sqrt( squareRtTest );
			float negativeTest = -b - glm::sqrt( squareRtTest );

			if ( positiveTest >= negativeTest )
			{
				if ( closestDistanceToReturn != nullptr ) *closestDistanceToReturn = positiveTest;
				if ( otherDistanceToReturn != nullptr ) * otherDistanceToReturn = negativeTest;
			}
			else if ( negativeTest > positiveTest )
			{
				if ( closestDistanceToReturn != nullptr ) *closestDistanceToReturn = negativeTest;
				if ( otherDistanceToReturn != nullptr ) * otherDistanceToReturn = positiveTest;
			}
		}
		return true;
	}
	else return false;
}

void GameNode::fixPointers( GameNode* startPointer , GameNodeConnectionHolder* startConnections )
{
	connections = startConnections + ( ( unsigned int ) connections );
	if ( numConnections > 0 )
	{
		for ( unsigned int i = 0; i < numConnections; i++ )
		{
			connections[i].connectedNode = startPointer + (( unsigned int ) connections[i].connectedNode);
		}
	}
}