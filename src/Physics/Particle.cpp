#include <Physics\Particle.h>
#include <Misc\Clock.h>
#include <core\GameObject.h>
#include <Physics\CollisionEvent.h>
#include <Physics\PreCollideEvent.h>
Particle::Particle() : mass( 1 ) , damping( 1 ) , collisionRadius (1), freezeX( false ) , freezeY( false ) , freezeZ( false ) {}

void Particle::addToTotalForce( glm::vec3& force )
{
	totalForce += force;
}
void Particle::earlyUpdate() {}
void Particle::update( )
{
}

void Particle::lateUpdate()
{
	if ( !parent || mass <= 0 ) return;

	glm::vec3 resultAccel = acceleration;
	resultAccel += totalForce / mass;
	velocity += Clock::dt * resultAccel;
	velocity *= powf( damping , Clock::dt );
	if ( !freezeX ) parent->translate.x += Clock::dt * velocity.x;
	if ( !freezeY ) parent->translate.y += Clock::dt * velocity.y;
	if ( !freezeZ ) parent->translate.z += Clock::dt * velocity.z;
	totalForce = glm::vec3();
}
void Particle::earlyDraw() {}
void Particle::draw() {}
void Particle::lateDraw() {}

glm::vec3 Particle::getAccelerationWithForces()
{
	glm::vec3 resultAccel = acceleration;
	resultAccel += totalForce / mass;
	return resultAccel;
}

glm::vec3 Particle::getPosition()
{
	if ( parent ) return parent->translate;
	return glm::vec3();
}
void Particle::setPosition( glm::vec3& position )
{
	if ( parent ) parent->translate = position;
}

void Particle::callCollideEvents(Particle* other)
{
	CollisionEvent* theEvent = 0;

	if ( parent )
	{
		theEvent = parent->getComponent<CollisionEvent>();
		if(theEvent) theEvent->collisionEvent(other);
	}
}

bool Particle::callPreCollideEvents( Particle* other )
{
	PreCollideEvent* theEvent = 0;

	if ( parent )
	{
		theEvent = parent->getComponent<PreCollideEvent>();
		if ( theEvent )
		{
			return theEvent->preCollideEvent( other );
		}
	}
	return true;
}