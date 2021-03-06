#include <Physics\ParticleContact.h>
#include <Physics\Particle.h>
#include <Misc\Clock.h>
void ParticleContact::resolve()
{
	resolveVelocity();
	resolveInterpenetration();
}

const float ParticleContact::calculateSeparatingVelocity()
{
	if ( particle[1] )
	{
		penetration = ( ( particle[0]->collisionRadius / 2 ) + ( particle[1]->collisionRadius / 2 ) ) - glm::length( particle[0]->getPosition() - particle[1]->getPosition() );
	}
	else
	{
		penetration = ( particle[0]->collisionRadius / 2 ) - particle[0]->getPosition().y;
	}
	glm::vec3 relativeVelocity = particle[0]->velocity;
	if ( particle[1] ) relativeVelocity -= particle[1]->velocity;
	return glm::dot(relativeVelocity, contactNormal);
}

void ParticleContact::resolveVelocity()
{
	float separatingVelocity = calculateSeparatingVelocity();

	if ( separatingVelocity > 0 ) return;

	float newSepVelocity = -separatingVelocity * restitution;

	glm::vec3 accelCausedVelocity = particle[0]->getAccelerationWithForces();
	if ( particle[1] ) accelCausedVelocity -= particle[1]->getAccelerationWithForces();
	float accelCausedSepVelocity = glm::dot(accelCausedVelocity, contactNormal) * Clock::dt;

	if ( accelCausedSepVelocity < 0 )
	{
		newSepVelocity += restitution * accelCausedSepVelocity;
		if ( newSepVelocity < 0 ) newSepVelocity = 0;
	}

	float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = 1 / particle[0]->mass;
	if ( particle[1] ) totalInverseMass += 1/particle[1]->mass;

	if ( totalInverseMass <= 0 ) return;

	float impulse = deltaVelocity / totalInverseMass;

	glm::vec3 impulsePerIMass = contactNormal * impulse;

	particle[0]->velocity = 
		particle[0]->velocity +
		impulsePerIMass * (1/particle[0]->mass);

	if ( particle[1] )
	{
		particle[1]->velocity =
			particle[1]->velocity +
			impulsePerIMass * -( 1 / particle[1]->mass )
			;
	}
}

#pragma warning(disable : 4100)

void ParticleContact::resolveInterpenetration()
{
	if ( penetration <= 0 ) return;

	float totalInverseMass = 1 / particle[0]->mass;
	if ( particle[1] ) totalInverseMass += 1/particle[1]->mass;

	if ( totalInverseMass <= 0 ) return;

	glm::vec3 movePerIMass = contactNormal * ( penetration / totalInverseMass );
	glm::vec3 movePerIMass1 = movePerIMass;
	if ( particle[0]->freezeX ) movePerIMass1.x = 0;
	if ( particle[0]->freezeY ) movePerIMass1.y = 0;
	if ( particle[0]->freezeZ ) movePerIMass1.z = 0;
	particle[0]->setPosition( particle[0]->getPosition() + ( movePerIMass1 * ( 1 / particle[0]->mass ) ) );
	if ( particle[1] )
	{
		glm::vec3 movePerIMass2 = movePerIMass;
		if ( particle[1]->freezeX ) movePerIMass2.x = 0;
		if ( particle[1]->freezeY ) movePerIMass2.y = 0;
		if ( particle[1]->freezeZ ) movePerIMass2.z = 0;
		particle[1]->setPosition( particle[1]->getPosition() + ( -movePerIMass2 * ( 1 / particle[1]->mass ) ) );
	}
}