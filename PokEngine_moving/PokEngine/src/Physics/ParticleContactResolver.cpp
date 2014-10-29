#include <Physics\ParticleContactResolver.h>
#include <Physics\ParticleContact.h>
#include <Physics\ParticleCollisionChecker.h>
ParticleContactResolver::ParticleContactResolver() :iterations(0) , collisionChecker(0) {}
void ParticleContactResolver::initialize( unsigned int iterations , ParticleCollisionChecker* penetrationUpdater )
{
	this->iterations =  iterations ;
	collisionChecker = penetrationUpdater;
}

void ParticleContactResolver::setIterations( unsigned int iterations ) { this->iterations = iterations; }

void ParticleContactResolver::resolveContacts( ArrayList<ParticleContact>* contactArray)
{
	unsigned int i;

	iterationsUsed = 0;

	while ( iterationsUsed < iterations )
	{
		float max = FLT_MAX;
		unsigned int maxIndex = contactArray->size();

		for ( i = 0; i < contactArray->size(); i++ )
		{
			float sepVelocity = contactArray->get( i )->calculateSeparatingVelocity();
			if ( sepVelocity < max && ( sepVelocity < 0 || contactArray->get( i )->penetration > 0 ) )
			{
				max = sepVelocity;
				maxIndex = i;
			}
		}

		if ( maxIndex == contactArray->size() )break;

		contactArray->get( maxIndex )->resolve();
		if ( collisionChecker ) collisionChecker->updateContact( contactArray->get( maxIndex ) );
		iterationsUsed++;
	}
}