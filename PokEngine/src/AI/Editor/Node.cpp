#include "AI/Editor/Node.h"

Node::NodeConnectionHolder::NodeConnectionHolder()
{
	connectedNode = nullptr;
	weight = FLT_MAX;
}

Node::Node( glm::vec3& position , unsigned int physicalMaxConnections ) : physicalMaxConnections( physicalMaxConnections ) , position( position )
{
	connectDraw = false;
	currentMaxConnections = 0;
	connections = nullptr;
	if ( physicalMaxConnections > 0 )
	{
		connections = new NodeConnectionHolder[this->physicalMaxConnections];
	}
}

void Node::cleanUp( unsigned int* test )
{
	if ( test == nullptr )
	{
		if ( connections != nullptr && physicalMaxConnections > 0 )
		{
			delete[] connections;
			connections = nullptr;
		}
	}
	else
	{
		if ( connections != nullptr && test > 0 )
		{
			delete[] connections;
			connections = nullptr;
		}
	}
}

void Node::updateNodePointers( Node* oldStartPointer , Node* newStartPointer )
{
	unsigned int oldP = ( unsigned int ) oldStartPointer;
	unsigned int newP = ( unsigned int ) newStartPointer;

	for ( unsigned  int i = 0; i < currentMaxConnections; i++ )
	{
		unsigned int temp = ( unsigned int ) connections[i].connectedNode;
		temp += newP - oldP;
		connections[i].connectedNode = ( Node* ) temp;
	}
}

void Node::swapNodes( Node* toSwap )
{
	NodeConnectionHolder* thisConnection = connections;
	unsigned int thisPhyMax = physicalMaxConnections;
	unsigned int thisCurMax = currentMaxConnections;
	glm::vec3 thisPos = position;
	bool thisConnectDraw = connectDraw;

	connections = toSwap->connections;
	physicalMaxConnections = toSwap->physicalMaxConnections;
	currentMaxConnections = toSwap->currentMaxConnections;
	position = toSwap->position;
	connectDraw = toSwap->connectDraw;

	toSwap->connections = thisConnection;
	toSwap->physicalMaxConnections = thisPhyMax;
	toSwap->currentMaxConnections = thisCurMax;
	toSwap->position = thisPos;
	toSwap->connectDraw = thisConnectDraw;
}

unsigned int Node::getNumMaxConnections()
{
	return currentMaxConnections;
}

void Node::resetConnections()
{
	cleanUp();

	physicalMaxConnections = 10;

	connections = new NodeConnectionHolder[physicalMaxConnections];
	currentMaxConnections = 0;
}

void Node::setPosition(glm::vec3& position)
{
	this->position = position;
}
glm::vec3 Node::getPosition()
{
	return position;
}
void Node::connect( Node* child , float weight )
{
	if ( physicalMaxConnections <= currentMaxConnections )
	{
		unsigned int lastMaxConnections = physicalMaxConnections;
		physicalMaxConnections += 1;
		physicalMaxConnections *= 2;
		NodeConnectionHolder* newConnections = new NodeConnectionHolder[physicalMaxConnections];

		if ( connections != nullptr && lastMaxConnections > 0 )
		{
			memcpy( newConnections , connections , sizeof( NodeConnectionHolder ) * currentMaxConnections );

			cleanUp(&lastMaxConnections);
		}
		connections = newConnections;
	}

	connections[currentMaxConnections].connectedNode = child;
	connections[currentMaxConnections].weight = weight;
	currentMaxConnections++;
}

void Node::disconnect( Node* child )
{
	for ( unsigned int i = 0; i < currentMaxConnections; i++ )
	{
		if ( connections[i].connectedNode == child )
		{
			currentMaxConnections--;
			connections[i] = connections[currentMaxConnections];
			break;
		}
	}
	if ( currentMaxConnections < physicalMaxConnections / 3)
	{
		unsigned int lastMaxConnections = physicalMaxConnections;
		physicalMaxConnections /= 2;
		if ( physicalMaxConnections > 0 )
		{
			NodeConnectionHolder* newConnections = new NodeConnectionHolder[physicalMaxConnections];

			memcpy( newConnections , connections , sizeof( NodeConnectionHolder ) * currentMaxConnections );
			cleanUp(&lastMaxConnections);
			connections = newConnections;
		}
		else
		{
			cleanUp( &lastMaxConnections );
		}
	}
}


void Node::getConnections( NodeConnectionHolder** arrayHolder , unsigned int* arrayMaxHolder )
{
	*arrayHolder = connections;
	*arrayMaxHolder = currentMaxConnections;

}

void Node::updateConnection( Node* firstPointer , Node* lastPointer )
{
	for ( unsigned int i = 0; i < currentMaxConnections; i++ )
	{
		if ( connections[i].connectedNode == firstPointer )
		{
			connections[i].connectedNode = lastPointer;
			break;
		}
	}
}

bool Node::intersected( glm::vec3& cameraPos , glm::vec3& ray , float* closestDistanceToReturn , float* otherDistanceToReturn )
{
	float b = glm::dot( ray , ( cameraPos - position ) );
	float c = glm::dot( ( cameraPos - position ) , ( cameraPos - position ) ) - 1;

	float squareRtTest = ( b*b ) - c;

	if ( squareRtTest > 0 )
	{
		if ( closestDistanceToReturn != nullptr || otherDistanceToReturn != nullptr )
		{
			float positiveTest = -b + glm::sqrt(squareRtTest);
			float negativeTest = -b - glm::sqrt( squareRtTest);

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

void Node::exportNode( Node* startPointer , std::string* nodeDataHolder , std::string* connectionDataHolder )
{
	std::string maxConnections( reinterpret_cast< const char* >( &currentMaxConnections ) , sizeof( currentMaxConnections ) );

	std::string thePosition( reinterpret_cast< const char* >( &position ) , sizeof( position ) );
	nodeDataHolder->operator+=(maxConnections + thePosition);

	for ( unsigned int i = 0; i < currentMaxConnections; i++ )
	{
		std::string test( reinterpret_cast< const char* >( &connections[i].weight ) , sizeof( connections[i].weight ) );
		float test2 = *reinterpret_cast< const float* >( test.c_str() );
		test2;
		connectionDataHolder->operator+=(std::string( reinterpret_cast< const char* >( &connections[i].weight ) , sizeof( connections[i].weight ) ));
		
		unsigned int pointer = ( unsigned int ) ( connections[i].connectedNode - startPointer );
		connectionDataHolder->operator+=( std::string( reinterpret_cast< const char* >( &pointer ) , sizeof( pointer ) ));
	}
}

unsigned int Node::importNode( const char* nodeData , const char* connectionData , Node* startPointer )
{
	memcpy( &currentMaxConnections , nodeData , sizeof( currentMaxConnections ) );
	physicalMaxConnections = currentMaxConnections;

	memcpy( &position , nodeData + sizeof( currentMaxConnections ) , sizeof( position ) );

	cleanUp();

	if ( physicalMaxConnections > 0 )
	{
		connections = new NodeConnectionHolder[physicalMaxConnections];
		memcpy( connections , connectionData, sizeof( NodeConnectionHolder ) * currentMaxConnections );

		for ( unsigned int i = 0; i < currentMaxConnections; i++ )
		{
			connections[i].connectedNode = startPointer + ( unsigned int ) connections[i].connectedNode;
		}
	}
	return currentMaxConnections;
}