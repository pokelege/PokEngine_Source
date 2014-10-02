#include <Input\MouseInput.h>
MouseInput MouseInput::globalMouseInput;
void MouseInput::updateMousePosition( const glm::vec2& position )
{
	mousePosition = position;
}