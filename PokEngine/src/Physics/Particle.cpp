#include <Physics\Particle.h>
#include <Misc\Clock.h>
#include <core\GameObject.h>
Particle::Particle() : parent(0), mass(1), damping(1), freezeX(false), freezeY(false), freezeZ(false) {}

void Particle::attatch( GameObject* parent )
{
	this->parent = parent;
}
void Particle::detatch()
{
	parent = 0;
}

void Particle::addToTotalForce( glm::vec3& force )
{
	totalForce += force;
}
void Particle::earlyUpdate() {}
void Particle::update( )
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

void Particle::lateUpdate() {}
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