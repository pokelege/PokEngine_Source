#include "AI\In-Game\Decision Tree\DecisionNode.h"

DecisionNode::DecisionNode( DecisionNode* trueNode , DecisionNode* falseNode , fastdelegate::FastDelegate1<AI* , bool> compareLogic ) : trueNode( trueNode ) , falseNode( falseNode ) , compareLogic( compareLogic ) , toReturn(nullptr){}
DecisionNode::DecisionNode( State* toReturn ) : trueNode( nullptr ) , falseNode( nullptr ) , compareLogic( nullptr ) , toReturn( toReturn ) {}
State* DecisionNode::toDecide( AI* toCompare )
{
	if ( toReturn != nullptr ) return toReturn;
	else if ( compareLogic.empty() ) return nullptr;
	else return compareLogic( toCompare ) ? trueNode->toDecide( toCompare ) : falseNode->toDecide( toCompare );
}