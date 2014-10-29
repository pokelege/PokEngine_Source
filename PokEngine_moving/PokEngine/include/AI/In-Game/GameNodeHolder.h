#pragma once
#include <PokEngineExportHeader.h>
class GameNode;
class POKENGINE_SHARED GameNodeHolder
{
	unsigned int currentMaxNodes;

public:
	unsigned int getcurrentMaxNodes();
	GameNode* getNode( unsigned int id );
	void setNodePointers();
};