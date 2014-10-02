#include <Input\FirstPersonCameraInput.h>
#include <Windows.h>
#include <Input\MouseInput.h>
FirstPersonCameraInput::FirstPersonCameraInput() : parent( 0 ) , forward( 'W' ) , back( 'S'), left('A'), right('D'), up('R'), down('F') {}
FirstPersonCameraInput::FirstPersonCameraInput( GameObject* parent ) : parent( parent ) , forward( 'W' ) , back( 'S' ) , left( 'A' ) , right( 'D' ) , up( 'R' ) , down( 'F' ) {}
void FirstPersonCameraInput::attatch( GameObject* parent )
{
	this->parent = parent;
}
void FirstPersonCameraInput::detatch()
{
	parent = 0;
}
void FirstPersonCameraInput::earlyUpdate()
{
	if ( !parent ) return;

	glm::vec2 delta = MouseInput::globalMouseInput.mousePosition - oldMousePosition;

	glm::vec3 b = glm::cross( direction , up );

	direction = glm::mat3( glm::rotate( -delta.x * mouseSpeed , up ) * glm::rotate( -delta.y * mouseSpeed , b ) ) * direction;
}
void FirstPersonCameraInput::update() {}
void FirstPersonCameraInput::lateUpdate() {}
FirstPersonCameraInput::~FirstPersonCameraInput() {}