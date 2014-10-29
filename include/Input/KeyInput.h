#pragma once
#include <PokEngineExportHeader.h>
class POKENGINE_SHARED KeyInput
{
public:
	static bool isDown( unsigned int key );
	static bool wasPressed( unsigned int key );
};