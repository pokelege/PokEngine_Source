#include <Input\FirstPersonCameraInput.h>
#include <Windows.h>
#include <Input\MouseInput.h>
#include <Graphics\Camera.h>
#include <Core\GameObject.h>
#include <gtc\matrix_transform.hpp>
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

	Camera* camera = parent->getComponent<Camera>();
	if ( !camera ) return;
	glm::vec2 delta = MouseInput::globalMouseInput.mousePosition - oldMousePosition;

	glm::vec3 b = glm::cross( camera->direction , camera->up );

	camera->direction = glm::mat3( glm::rotate( glm::mat4() , -delta.x * rotationSensitivity , camera->up ) * glm::rotate( glm::mat4() , -delta.y * rotationSensitivity , b ) ) *camera->direction;

	if ( GetAsyncKeyState( forward ) )
	{
		parent->translate += moveSensitivity * camera->direction;
	}

	if ( GetAsyncKeyState( back ) )
	{
		parent->translate -= moveSensitivity * camera->direction;
	}

	if ( GetAsyncKeyState( left ) )
	{
		parent->translate += moveSensitivity * glm::normalize( glm::cross( camera->up , camera->direction ) );
	}

	if ( GetAsyncKeyState( right) )
	{
		parent->translate += moveSensitivity * glm::normalize( glm::cross( camera->direction , camera->up ) );
	}

	if ( GetAsyncKeyState( up ) )
	{
		parent->translate += moveSensitivity * camera->up;
	}

	if ( GetAsyncKeyState( down ) )
	{
		parent->translate += moveSensitivity * -camera->up;
	}
}
void FirstPersonCameraInput::update() {}
void FirstPersonCameraInput::lateUpdate() {}
FirstPersonCameraInput::~FirstPersonCameraInput() {}