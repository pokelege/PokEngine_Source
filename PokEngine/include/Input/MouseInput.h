#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <glm.hpp>
struct POKENGINE_SHARED MouseInput
{
	glm::vec2 mousePosition;
	void updateMousePosition( const glm::vec2& position );
	static MouseInput globalMouseInput;
};