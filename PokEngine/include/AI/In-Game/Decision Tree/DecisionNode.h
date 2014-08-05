#pragma once
#pragma warning (disable: 4100)
#pragma warning (disable:4251)
#include "PokEngineExportHeader.h"
#include <FastDelegate.h>
class AI;
class State;
class POKENGINE_SHARED DecisionNode
{
	fastdelegate::FastDelegate1<AI* , bool> compareLogic;
	DecisionNode* trueNode , *falseNode;
	State* toReturn;
public:
	DecisionNode( DecisionNode* trueNode , DecisionNode* falseNode , fastdelegate::FastDelegate1<AI* , bool> compareLogic );
	DecisionNode( State* toReturn = nullptr );
	State* toDecide(AI* toCompare);
};