#include <Physics\ParticleWorld.h>
#include <Physics\Particle.h>
#include <Physics\ParticleCollisionChecker.h>

#include <Physics\ParticleContact.h>

ParticleWorld ParticleWorld::global;

ParticleWorld::ParticleWorld() : collisionChecker(0) {}
void ParticleWorld::initialize( ParticleCollisionChecker* collisionChecker , unsigned int numberOfCollsionIterations )
{
	collisionResolver.initialize( numberOfCollsionIterations , collisionChecker );
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

void ParticleWorld::update( )
{
	if(collisionChecker != NULL) processCollisions(  );
}

void ParticleWorld::processCollisions( )
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
		collisionResolver.resolveContacts( &contacts );
		for ( unsigned int i = 0; i < contacts.size(); i++ )
		{
			delete contacts.get( i );
		}
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