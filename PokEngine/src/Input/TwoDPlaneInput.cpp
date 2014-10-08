#include <Input\TwoDPlaneInput.h>
#include <Input\KeyInput.h>
#include <Core\GameObject.h>
#include <Misc\Clock.h>
#include <gtc\matrix_transform.hpp>
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
	glm::vec3 totalTranslate;
	float totalRotation = 0;
	int numRotations = 0;
	if ( KeyInput::isDown( up ) )
	{
		++numRotations;
		totalRotation += 90;
		totalTranslate += moveSensitivity * Clock::dt * y;
	}
	if ( KeyInput::isDown( down ) ) 
	{
		++numRotations;
		totalRotation -= 90;
		totalTranslate -= moveSensitivity * Clock::dt * y;
	}
	if ( KeyInput::isDown( right ) )
	{
		++numRotations;
		totalTranslate += moveSensitivity * Clock::dt * x;
	}
	if ( KeyInput::isDown( left ) )
	{
		if ( numRotations )
		{
			if ( !totalRotation ) totalRotation = 180;
			if ( totalRotation < 0 ) totalRotation -= 180;
			if ( totalRotation > 0 ) totalRotation += 180;
		}
		else
		{
			totalRotation = 180;
		}
		++numRotations;
		totalTranslate -= moveSensitivity * Clock::dt * x;
	}
	if ( numRotations )
	{
		totalRotation /= numRotations;
		parent->rotate = totalRotation * glm::cross( x , y );
	}
	parent->translate += totalTranslate;
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