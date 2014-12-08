#pragma once
#include <PokEngineExportHeader.h>
#include <glm.hpp>
class POKENGINE_SHARED XBox360XInput
{
public:
	static bool isDown( unsigned short key, unsigned int player = 0 );
	static float leftTrigger( unsigned int player = 0 );
	static float rightTrigger( unsigned int player = 0 );
	static glm::vec2 leftStick( unsigned int player = 0 );
	static glm::vec2 rightStick( unsigned int player = 0 );
	static void setVibration( unsigned short left , unsigned short right , unsigned int player = 0 );
	static bool exists( unsigned int player = 0 );
};