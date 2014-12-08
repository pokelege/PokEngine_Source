#pragma once
#include <PokEngineExportHeader.h>
class POKENGINE_SHARED KeyInput
{
public:
	static bool isDown( unsigned int key, unsigned int player = 0 );
	static bool wasPressed( unsigned int key , unsigned int player = 0 );
};