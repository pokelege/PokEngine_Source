#include "AI/Editor/NodeHolder.h"
#include "AI\Editor\Node.h"
NodeHolder::NodeHolder( unsigned int physicalMaxNodes ) : physicalMaxNodes( physicalMaxNodes ) , currentMaxNodes( 0 )
{
	nodes = nullptr;
	if ( physicalMaxNodes > 0 )
	{
		nodes = new Node[physicalMaxNodes];
	}
}

NodeHolder::~NodeHolder()
{
	if ( nodes != nullptr && physicalMaxNodes > 0 )
	{
		for ( unsigned int i = 0; i < physicalMaxNodes; i++ )
		{
			nodes[i].cleanUp();
		}
		delete[] nodes;
		nodes = nullptr;
	}
}

void NodeHolder::connectDisconnectNode( unsigned int& id , unsigned int id2 , const float* weight )
{
	Node::NodeConnectionHolder* connections = nullptr;
	unsigned int numberOfConnections = 0;

	nodes[id].getConnections( &connections , &numberOfConnections );
	bool found = false;
	for ( unsigned int i = 0; i < numberOfConnections && !found; i++ )
	{
		found = ( connections[i].connectedNode == &nodes[id2] );
	}

	if ( found )
	{
		nodes[id].disconnect( &nodes[id2] );
	}
	else
	{
		if ( weight == nullptr ) nodes[id].connect( &nodes[id2] , std::abs( glm::length( nodes[id].getPosition() - nodes[id2].getPosition() ) ) );
		else nodes[id].connect( &nodes[id2] , *weight );
	}
}

void NodeHolder::addNode( glm::vec3& node )
{
	if ( physicalMaxNodes <= currentMaxNodes )
	{
		unsigned int lastMaxNodes = physicalMaxNodes;
		physicalMaxNodes += 1;
		physicalMaxNodes *= 2;
		glm::vec3 pos;
		Node* newNodes = new Node[physicalMaxNodes];
		memcpy( newNodes , nodes , sizeof( Node ) * currentMaxNodes );
		for ( unsigned int i = 0; i < currentMaxNodes; i++ ) newNodes[i].updateNodePointers( nodes , newNodes );
		if ( nodes != nullptr && lastMaxNodes > 0 )
		{
			delete[] nodes;
			nodes = nullptr;
		}
		nodes = newNodes;
	}
	nodes[currentMaxNodes].resetConnections();
	nodes[currentMaxNodes].setPosition( node );
	currentMaxNodes++;
}

int NodeHolder::getcurrentMaxNodes()
{
	return currentMaxNodes;
}

Node* NodeHolder::getNode( unsigned int id )
{
	return &nodes[id];
}

void NodeHolder::deleteNode( unsigned int id )
{
	if ( id < currentMaxNodes )
	{
		for ( unsigned int i = 0; i < currentMaxNodes; i++ )
		{
			nodes[i].disconnect( &nodes[id] );
		}
		currentMaxNodes--;


		nodes[id].swapNodes( &nodes[currentMaxNodes] );
		nodes[currentMaxNodes].cleanUp();
		for ( unsigned int i = 0; i < currentMaxNodes; i++ )
		{
			nodes[i].updateConnection( &nodes[currentMaxNodes] , &nodes[id] );
		}


	}
}

std::string NodeHolder::getNodeData()
{
	std::string string( reinterpret_cast< const char* >( &currentMaxNodes ) , sizeof( currentMaxNodes ) );
	unsigned int connectionPointer = 0;

	std::string nodeData;
	std::string connectionData;
	for ( unsigned int i = 0; i < currentMaxNodes; i++ )
	{
		nodes[i].exportNode( nodes, &nodeData, &connectionData );
		nodeData += std::string( reinterpret_cast< const char* >( &connectionPointer ) , sizeof( connectionPointer ) );
		connectionPointer += nodes[i].getNumMaxConnections();
	}
	string += nodeData;
	string += connectionData;
	return string;
}

class NodeIfGame
{
	unsigned int size;
	glm::vec3 pos;
	void* connections;
};

void NodeHolder::setNodeData( const char* toSet )
{
	unsigned int lastPhysicalMaxNodes = physicalMaxNodes;
	memcpy( &currentMaxNodes , toSet , sizeof( currentMaxNodes ) );
	physicalMaxNodes = currentMaxNodes;

	if ( nodes != nullptr && lastPhysicalMaxNodes > 0 )
	{
		unsigned int toClear = lastPhysicalMaxNodes;
		if ( lastPhysicalMaxNodes > physicalMaxNodes ) toClear = physicalMaxNodes;
		for ( unsigned int i = 0; i < toClear; i++ )
		{
			nodes[i].cleanUp();
		}
		delete[] nodes;
		nodes = nullptr;
	}

	if ( physicalMaxNodes > 0 )
	{
		nodes = new Node[physicalMaxNodes];

		unsigned int currentNodePointer = sizeof( currentMaxNodes );
		unsigned int currentConnectionPointer = sizeof( currentMaxNodes ) +( sizeof( NodeIfGame ) * currentMaxNodes );
		for ( unsigned int i = 0; i < currentMaxNodes; i++ )
		{
			unsigned int connectionIndex = *reinterpret_cast< const unsigned int* >( toSet + currentNodePointer + sizeof( unsigned int ) +sizeof( glm::vec3 ) );
			nodes[i].importNode(toSet + currentNodePointer, toSet + currentConnectionPointer + (connectionIndex * sizeof(Node::NodeConnectionHolder)),  nodes);
			currentNodePointer += sizeof(NodeIfGame);
		}
	}
}