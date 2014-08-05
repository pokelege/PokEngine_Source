#include "AI/In-Game/GameNodeHolder.h"
#include "AI\In-Game\GameNode.h"
unsigned int GameNodeHolder::getcurrentMaxNodes()
{
	return currentMaxNodes;
}

GameNode* GameNodeHolder::getNode( unsigned int id )
{
	GameNode* nodeArray = ( GameNode* ) ( this + 1 );
	return &nodeArray[id];
}

void GameNodeHolder::setNodePointers()
{
	if ( currentMaxNodes > 0 )
	{
		GameNode* nodeArray = ( GameNode* ) ( this + 1 );
		GameNode::GameNodeConnectionHolder* startConnection = ( GameNode::GameNodeConnectionHolder* )( nodeArray + currentMaxNodes );
		for ( unsigned int i = 0; i < currentMaxNodes; i++ )
		{
			nodeArray[i].fixPointers( nodeArray , startConnection );
		}
	}
}