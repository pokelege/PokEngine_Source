#include <Physics\ParticleContact.h>
#include <Physics\Particle.h>
void ParticleContact::resolve( const float& dt )
{
	resolveVelocity( dt );
	resolveInterpenetration( dt );
}

const float ParticleContact::calculateSeparatingVelocity()
{
	if ( particle[1] )
	{
		penetration = ( ( particle[0]->getMass() / 2 ) + ( particle[1]->getMass() / 2 ) ) - glm::length( particle[0]->getPosition() - particle[1]->getPosition() );
	}
	else
	{
		penetration = ( particle[0]->getMass() / 2 ) - particle[0]->getPosition().y;
	}
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	if ( particle[1] ) relativeVelocity -= particle[1]->getVelocity();
	return glm::dot(relativeVelocity, contactNormal);
}

void ParticleContact::resolveVelocity( const float& dt )
{
	float separatingVelocity = calculateSeparatingVelocity();

	if ( separatingVelocity > 0 ) return;

	float newSepVelocity = -separatingVelocity * restitution;

	glm::vec3 accelCausedVelocity = particle[0]->getAccelerationWithForces();
	if ( particle[1] ) accelCausedVelocity -= particle[1]->getAccelerationWithForces();
	float accelCausedSepVelocity = glm::dot(accelCausedVelocity, contactNormal) * dt;

	if ( accelCausedSepVelocity < 0 )
	{
		newSepVelocity += restitution * accelCausedSepVelocity;
		if ( newSepVelocity < 0 ) newSepVelocity = 0;
	}

	float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = 1 / particle[0]->getMass();
	if ( particle[1] ) totalInverseMass += 1/particle[1]->getMass();

	if ( totalInverseMass <= 0 ) return;

	float impulse = deltaVelocity / totalInverseMass;

	glm::vec3 impulsePerIMass = contactNormal * impulse;

	particle[0]->setVelocity(
		particle[0]->getVelocity() +
		impulsePerIMass * (1/particle[0]->getMass())
		);

	if ( particle[1] )
	{
		particle[1]->setVelocity(
			particle[1]->getVelocity() +
			impulsePerIMass * -( 1 / particle[1]->getMass() )
			);
	}
}

#pragma warning(disable : 4100)

void ParticleContact::resolveInterpenetration( const float& dt )
{
	if ( penetration <= 0 ) return;

	float totalInverseMass = 1 / particle[0]->getMass();
	if ( particle[1] ) totalInverseMass += 1/particle[1]->getMass();

	if ( totalInverseMass <= 0 ) return;

	glm::vec3 movePerIMass = contactNormal * ( penetration / totalInverseMass );

	particle[0]->setPosition( particle[0]->getPosition() + ( movePerIMass * ( 1 / particle[0]->getMass() ) ) );
	if(particle[1]) particle[1]->setPosition( particle[1]->getPosition() + (- movePerIMass * ( 1 / particle[1]->getMass() ) ) );
}