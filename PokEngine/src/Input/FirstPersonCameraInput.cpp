#include <Input\FirstPersonCameraInput.h>
#include <Input\MouseInput.h>
#include <Graphics\Camera.h>
#include <Core\GameObject.h>
#include <gtc\matrix_transform.hpp>
#include <Input\KeyInput.h>
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
	glm::vec2 delta = MouseInput::globalMouseInput.mouseDelta;
	glm::vec3 b = glm::normalize(glm::cross( camera->direction , camera->up ));

	camera->direction = glm::normalize(glm::mat3( glm::rotate( glm::mat4() , -delta.x * rotationSensitivity , camera->up ) * glm::rotate( glm::mat4() , -delta.y * rotationSensitivity , b ) ) *camera->direction);

	if ( KeyInput::isDown(forward))
	{
		parent->translate += moveSensitivity * camera->direction;
	}

	if ( KeyInput::isDown( back ) )
	{
		parent->translate -= moveSensitivity * camera->direction;
	}

	if ( KeyInput::isDown( left ) )
	{
		parent->translate += moveSensitivity * glm::normalize( glm::cross( camera->up , camera->direction ) );
	}

	if ( KeyInput::isDown( right ) )
	{
		parent->translate += moveSensitivity * glm::normalize( glm::cross( camera->direction , camera->up ) );
	}

	if ( KeyInput::isDown( up ) )
	{
		parent->translate += moveSensitivity * camera->up;
	}

	if ( KeyInput::isDown( down ) )
	{
		parent->translate += moveSensitivity * -camera->up;
	}
}
void FirstPersonCameraInput::update() {}
void FirstPersonCameraInput::lateUpdate() {}
FirstPersonCameraInput::~FirstPersonCameraInput() {}
void FirstPersonCameraInput::earlyDraw() {}
void FirstPersonCameraInput::draw() {}
void FirstPersonCameraInput::lateDraw() {}