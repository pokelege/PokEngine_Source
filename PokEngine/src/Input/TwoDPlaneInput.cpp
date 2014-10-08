#include <Input\TwoDPlaneInput.h>
#include <Input\KeyInput.h>
#include <Core\GameObject.h>
#include <Misc\Clock.h>
TwoDPlaneInput::TwoDPlaneInput() : parent(0) , up('W') , down('S') , left('A') , right('D') , moveSensitivity(1) , x(glm::vec3(1 , 0 , 0)) , y(glm::vec3(0,1,0))
{

}

void TwoDPlaneInput::attatch( GameObject* parent )
{
	this->parent = parent;
}
void TwoDPlaneInput::detatch()
{
	parent = 0;
}

void TwoDPlaneInput::earlyUpdate()
{

}
void TwoDPlaneInput::update()
{
	if ( !parent ) return;
	if ( KeyInput::isDown( up ) ) parent->translate += moveSensitivity * Clock::dt * y;
	if ( KeyInput::isDown( down ) ) parent->translate -= moveSensitivity * Clock::dt * y;
	if ( KeyInput::isDown( right ) ) parent->translate += moveSensitivity * Clock::dt * x;
	if ( KeyInput::isDown( left ) ) parent->translate -= moveSensitivity * Clock::dt * x;
}
void TwoDPlaneInput::lateUpdate()
{

}
void TwoDPlaneInput::earlyDraw()
{

}
void TwoDPlaneInput::draw()
{

}
void TwoDPlaneInput::lateDraw()
{

}