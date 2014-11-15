#include <Input\TwoDPlaneInput.h>
#include <Input\KeyInput.h>
#include <Core\GameObject.h>
#include <Misc\Clock.h>
#include <gtc\matrix_transform.hpp>
#include <Physics\Particle.h>
#include <gtx\quaternion.hpp>
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
	if ( KeyInput::isDown( up ) )
	{
		totalTranslate += moveSensitivity * y;
	}
	if ( KeyInput::isDown( down ) ) 
	{
		totalTranslate -= moveSensitivity * y;
	}
	if ( KeyInput::isDown( right ) )
	{
		totalTranslate += moveSensitivity * x;
	}
	if ( KeyInput::isDown( left ) )
	{
		totalTranslate -= moveSensitivity * x;
	}
	if ( glm::length( totalTranslate ) )
	{
		parent->rotate = glm::quat_cast( glm::lookAt( parent->translate , parent->translate + glm::normalize( totalTranslate ) , glm::normalize( glm::vec3(0,0,1) ) ) );
	}
	Particle* theParticle = 0;
	if ( (theParticle = parent->getComponent<Particle>()) != 0 ) theParticle->addToTotalForce( totalTranslate);
	else parent->translate += totalTranslate * Clock::dt;
}
void TwoDPlaneInput::lateUpdate()
{
	Particle* theParticle = 0;
	if ( ( theParticle = parent->getComponent<Particle>() ) != 0 ) theParticle->velocity = glm::vec3();
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