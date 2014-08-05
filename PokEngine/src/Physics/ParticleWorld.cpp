#include <Physics\ParticleWorld.h>
#include <Physics\Particle.h>
#include <Physics\ParticleCollisionChecker.h>
#include <Physics\ParticleForceGenerator.h>
#include <Physics\ParticleContact.h>
ParticleWorld::ParticleWorld(ParticleCollisionChecker* collisionChecker , unsigned int numberOfCollsionIterations) : collisionResolver(numberOfCollsionIterations, collisionChecker)
{
	this->collisionChecker = collisionChecker;
}

void ParticleWorld::numberOfCollsionIterations( unsigned int numberOfCollsionIterations )
{
	collisionResolver.setIterations( numberOfCollsionIterations );
}

void ParticleWorld::addParticleToManage( Particle* particle )
{
	particles.add( particle );
}

void ParticleWorld::removeParticleToManage( Particle* particle , bool deleteHeapData )
{
	for ( unsigned int i = 0; i < particles.size(); i++ )
	{
		if ( particles.get( i ) == particle )
		{
			if ( deleteHeapData ) delete particles.get( i );
			particles.remove( i );
			break;
		}
	}
}

void ParticleWorld::addForceToParticle( Particle* particle , ParticleForceGenerator* force )
{
	forces.add( particle , force );
}

void ParticleWorld::removeForceFromParticle( Particle* particle , ParticleForceGenerator* force , bool deleteHeapData )
{
	forces.remove( particle , force );
	if ( deleteHeapData ) delete force;
}

void ParticleWorld::update( const float dt )
{
	forces.update( dt );
	if(collisionChecker != NULL) processCollisions( dt );
	processParticles( dt );
}

void ParticleWorld::processCollisions( const float dt )
{
	ArrayList<ParticleContact> contacts;
	for ( unsigned int i = 0; i < particles.size(); i++ )
	{
		for ( unsigned int j = i; j < particles.size(); j++ )
		{
			if ( collisionChecker->isCollided( particles.get( i ) , particles.get( j ) ) )
			{
				ParticleContact* contacted = collisionChecker->generateContact( particles.get( i ) , particles.get( j ) );
				contacts.add( contacted );
			}
		}
	}
	if ( contacts.size() > 0 )
	{
		collisionResolver.resolveContacts( &contacts , dt );
		for ( unsigned int i = 0; i < contacts.size(); i++ )
		{
			delete contacts.get( i );
		}
	}
}

void ParticleWorld::processParticles( const float dt )
{
	for ( unsigned int i = 0; i < particles.size(); i++ )
	{
		particles.get( i )->update(dt);
	}
}

unsigned int ParticleWorld::getNumberOfParticles()
{
	return particles.size();
}
Particle* ParticleWorld::getParticleFromList( unsigned int i )
{
	return particles.get( i );
}