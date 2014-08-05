#include "AI\In-Game\Astar.h"
#include "AI\In-Game\GameNode.h"
#include "Misc\ArrayList.h"
#include "Misc\PriorityQueue.h"
int Astar::AstarHolder::compareTo( AstarHolder* toCompare )
{
	float thisF = distTraveled + estToEnd;
	float toCompareF = toCompare->distTraveled + toCompare->estToEnd;

	if ( thisF < toCompareF ) return -1;
	else if ( thisF > toCompareF ) return 1;
	else return 0;
}

bool Astar::AstarHolder::equals( AstarHolder* toEqual )
{
	if ( toEqual != nullptr )
	{
		bool test = theNode == toEqual->theNode;
		test;
		return theNode == toEqual->theNode;
	}
	else return false;
}

void Astar::clear()
{
	theStack.cleanUp();
}

Astar::~Astar()
{
	clear();
}

void Astar::generateQueue( GameNode* start , GameNode* end )
{
	theStack.cleanUp();
	ArrayList<AstarHolder> closedList;
	PriorityQueue<AstarHolder> openList;
	AstarHolder* starting = new AstarHolder;
	starting->theNode = start;
	starting->parent = nullptr;
	starting->distTraveled = 0;
	starting->estToEnd = glm::length( end->getPosition() - start->getPosition() );
	openList.offer( starting );
	while ( openList.peek() != nullptr && openList.peek()->theNode != end)
	{
		AstarHolder* toProcess = openList.poll();
		GameNode::GameNodeConnectionHolder* nodesToProcess = nullptr;
		unsigned int numNodesToProcess = 0;
		toProcess->theNode->getConnections( &nodesToProcess , &numNodesToProcess );

		for ( unsigned int i = 0; i < numNodesToProcess; i++ )
		{
			bool skip = false;
			unsigned int j = 0;
			for (j = 0; j < closedList.size() && !skip; j++ )
			{
				if ( closedList.get( j )->theNode == nodesToProcess[i].connectedNode ) skip = true;
			}
			j--;
			if ( skip )
			{
				AstarHolder* newHolder = new AstarHolder;
				newHolder->theNode = nodesToProcess[i].connectedNode;
				newHolder->parent = toProcess;
				newHolder->distTraveled = nodesToProcess[i].weight + toProcess->distTraveled;
				newHolder->estToEnd = glm::length( end->getPosition() - newHolder->theNode->getPosition() );
				AstarHolder* toCompare = closedList.get( j );
				closedList.remove( j );

				if ( newHolder->compareTo( toCompare ) < 0 )
				{
					openList.offer( newHolder );
					delete toCompare;
				}
				else
				{
					closedList.add( toCompare );
					delete newHolder;
				}
			}

			else
			{
				AstarHolder* newHolder = new AstarHolder;
				newHolder->theNode = nodesToProcess[i].connectedNode;
				newHolder->parent = toProcess;
				newHolder->distTraveled = nodesToProcess[i].weight + toProcess->distTraveled;
				newHolder->estToEnd = glm::length( end->getPosition() - newHolder->theNode->getPosition() );

				AstarHolder* toCompare = openList.deleteExisting( newHolder );
				if ( toCompare == nullptr ) openList.offer( newHolder );
				else
				{
					if ( newHolder->compareTo( toCompare ) < 0 )
					{
						openList.offer( newHolder );
						delete  toCompare;
					}
					else
					{
						openList.offer( toCompare );
						delete newHolder;
					}
				}
			}
		}

		closedList.add( toProcess );
	}

	if (openList.peek() != nullptr && openList.peek()->theNode == end )
	{
		AstarHolder* theTest = openList.peek();
		while ( theTest != nullptr)
		{
			theStack.offer( theTest->theNode );
			theTest = theTest->parent;
		}
	}

	while ( openList.peek() != nullptr ) delete openList.poll();
	for ( unsigned int i = 0; i < closedList.size(); i++ ) delete closedList.get( i );
}

GameNode* Astar::poll()
{
	return theStack.poll();
}

GameNode* Astar::peek()
{
	return theStack.peek();
}