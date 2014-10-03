#include <Physics\Particle.h>

Particle::Particle( glm::vec3& position , glm::vec3& velocity , glm::vec3& acceleration , const float mass , const float damping ) : position( position ) , velocity( velocity ) , acceleration( acceleration ) , mass( mass ) , damping(damping) {}

glm::vec3& Particle::getPosition()
{
	return position;
}
void Particle::setPosition( glm::vec3& position )
{
	this->position = position;
}
glm::vec3& Particle::getVelocity()
{
	return velocity;
}
void Particle::setVelocity( glm::vec3& velocity )
{
	this->velocity = velocity;
}
glm::vec3& Particle::getAcceleration()
{
	return acceleration;
}
void Particle::setAcceleration( glm::vec3& acceleration )
{
	this->acceleration = acceleration;
}
float& Particle::getMass()
{
	return mass;
}
void Particle::setMass( float mass )
{
	this->mass = mass;
}
float& Particle::getDamping()
{
	return damping;
}
void Particle::setDamping( float damping )
{
	this->damping = damping;
}
glm::vec3& Particle::getTotalForce()
{
	return totalForce;
}
void Particle::setTotalForce( glm::vec3& totalForce )
{
	this->totalForce = totalForce;
}
void Particle::addToTotalForce( glm::vec3& force )
{
	totalForce += force;
}

void Particle::update( const float& dt )
{
	if ( mass <= 0 ) return;

	glm::vec3 resultAccel = acceleration;
	resultAccel += totalForce / mass;
	velocity += dt * resultAccel;
	velocity *= powf( damping , dt );
	position += dt * velocity;
	totalForce = glm::vec3();
}

glm::vec3 Particle::getAccelerationWithForces()
{
	glm::vec3 resultAccel = acceleration;
	resultAccel += totalForce / mass;
	return resultAccel;
}