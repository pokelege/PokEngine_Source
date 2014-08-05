#pragma once
#include <PokEngineExportHeader.h>
#include <Misc\Stack.h>
#pragma warning(disable:4251)
class DebugAI;
class GameNode;
class POKENGINE_SHARED Astar
{
	friend class DebugAI;

	struct AstarHolder
	{
		GameNode* theNode;
		float distTraveled;
		float estToEnd;
		AstarHolder* parent;

		int compareTo( AstarHolder* toCompare );
		bool equals( AstarHolder* toEqual );
	};

	Stack<GameNode> theStack;

public:
	~Astar();
	void clear();
	void generateQueue( GameNode* start , GameNode* end );
	GameNode* poll();
	GameNode* peek();
};