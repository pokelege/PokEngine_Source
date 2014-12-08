#pragma once
#include <PokEngineExportHeader.h>

class POKENGINE_SHARED XBox360Input
{
public:
	static bool isDown( unsigned int key );
	static bool wasPressed( unsigned int key );
};