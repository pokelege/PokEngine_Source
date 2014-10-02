#pragma once
#include <PokEngineExportHeader.h>
#include <glm.hpp>
struct MouseInput
{
	glm::vec2 mousePosition;
	void updateMousePosition( const glm::vec2& position );
	static MouseInput globalMouseInput;
};