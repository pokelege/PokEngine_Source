#include <Input\MouseInput.h>
MouseInput MouseInput::globalMouseInput;

MouseInput::MouseInput() : deltaTracking(false) {}

void MouseInput::updateMousePosition( const glm::vec2& position )
{
	mousePosition = position;
	if ( deltaTracking )
	{
		mouseDelta = mousePosition - oldMousePosition;
		if(updateOldMousePosition) oldMousePosition = mousePosition;
	}
	else
	{
		mouseDelta = glm::vec2();
	}
}

void MouseInput::setDeltaTracking( bool tracking )
{
	deltaTracking = tracking;
	if ( deltaTracking )
	{
		if ( updateOldMousePosition ) oldMousePosition = mousePosition;
	}
}

bool MouseInput::getDeltaTracking()
{
	return deltaTracking;
}