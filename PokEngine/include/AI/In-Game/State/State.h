#pragma once
#include "PokEngineExportHeader.h"
class AI;
class State
{
public:
	virtual void update(AI* toPass) = 0;
};