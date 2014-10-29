#include <Physics\SphereCollision.h>
#include <Physics\Particle.h>
#include <Physics\ParticleContact.h>
SphereCollision SphereCollision::instance;

bool SphereCollision::isCollided( Particle* particle1 , Particle* particle2 )
{
	if ( particle2 && particle2 != particle1 )
	{
		if ( ( ( particle1->collisionRadius / 2 ) + ( particle2->collisionRadius / 2 ) ) - glm::length( particle1->getPosition() - particle2->getPosition() ) >= 0 )
		{
			if ( ( particle2 && particle2 != particle1 ) && !particle1->callPreCollideEvents( particle2 ) ) return false;
			if ( ( particle2 && particle2 != particle1 ) && !particle2->callPreCollideEvents( particle1 ) ) return false;
			return true;
		}
	}
	return false;
}

ParticleContact* SphereCollision::generateContact( Particle* particle1 , Particle* particle2 )
{
	ParticleContact* contacted = new ParticleContact();
	contacted->particle[0] = particle1;
	if ( particle2 && particle2 != particle1 ) contacted->particle[1] = particle2;
	else contacted->particle[1] = NULL;
	contacted->restitution = 0;
	if ( particle2 && particle2 != particle1 )
	{
		contacted->contactNormal = glm::normalize( particle1->getPosition() - particle2->getPosition() );
		if ( glm::isnan( contacted->contactNormal.x ) )
		{
			contacted->contactNormal = glm::normalize( glm::vec3( 0 , 1 , 0 ) );
		}
	}
	return contacted;
}

void SphereCollision::updateContact( ParticleContact* contact )
{
	if ( contact->particle[1] )
	{
		contact->penetration = ( ( contact->particle[0]->collisionRadius / 2 ) + ( contact->particle[1]->collisionRadius / 2 ) ) - glm::length( contact->particle[0]->getPosition() - contact->particle[1]->getPosition() );
		contact->particle[1]->callCollideEvents( contact->particle[0] );
	}
	contact->particle[0]->callCollideEvents( contact->particle[1] );
}