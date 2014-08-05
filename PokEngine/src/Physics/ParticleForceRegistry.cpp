#include <Physics\ParticleForceRegistry.h>
#include <Physics\Particle.h>
#include <Physics\ParticleForceGenerator.h>
void ParticleForceRegistry::add( Particle* particle , ParticleForceGenerator* forceGenerator )
{
	ForceHolder* newForce = new ForceHolder;
	newForce->particle = particle;
	newForce->forceGenerator = forceGenerator;

	registry.add( newForce );
}

void ParticleForceRegistry::remove( Particle* particle , ParticleForceGenerator* forceGenerator )
{
	for ( unsigned int i = 0; i < registry.size(); i++ )
	{
		if ( registry.get( i )->particle == particle && registry.get( i )->forceGenerator == forceGenerator )
		{
			delete registry.get( i );
			registry.remove( i );
			break;
		}
	}
}

void ParticleForceRegistry::clear()
{
	while ( registry.size() > 0 )
	{
		delete registry.get( 0 );
		registry.remove( 0 );
	}
}

void ParticleForceRegistry::update( const float dt )
{
	for ( unsigned int i = 0; i < registry.size(); i++ )
	{
		registry.get( i )->forceGenerator->update( registry.get( i )->particle , dt );
	}
}

ParticleForceRegistry::~ParticleForceRegistry()
{
	clear();
}