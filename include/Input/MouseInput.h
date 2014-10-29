#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <glm.hpp>
struct POKENGINE_SHARED MouseInput
{
	glm::vec2 mousePosition;
	glm::vec2 oldMousePosition;
	glm::vec2 mouseDelta;
	bool updateOldMousePosition;
	MouseInput();
	void setDeltaTracking(bool tracking);
	bool getDeltaTracking();
	void updateMousePosition( const glm::vec2& position );
	static MouseInput globalMouseInput;
private:
	bool deltaTracking;
};