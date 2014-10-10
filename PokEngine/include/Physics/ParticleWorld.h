#pragma once
#include "PokEngineExportHeader.h"
#include <Misc\ArrayList.h>
#include <Physics\ParticleForceRegistry.h>
#include <Physics\ParticleContactResolver.h>
class POKENGINE_SHARED ParticleWorld
{
	ParticleForceRegistry forces;
	ArrayList<Particle> particles;
	ParticleContactResolver collisionResolver;
	ParticleCollisionChecker* collisionChecker;
	void processCollisions(const float dt);
	void processParticles( );
public:
	ParticleWorld( ParticleCollisionChecker* collisionChecker = 0, unsigned int numberOfCollsionIterations = 10 );
	void numberOfCollsionIterations( unsigned int numberOfCollsionIterations );
	void addParticleToManage( Particle* particle );
	void removeParticleToManage( Particle* particle , bool deleteHeapData = false );
	void addForceToParticle( Particle* particle , ParticleForceGenerator* force );
	void removeForceFromParticle( Particle* particle , ParticleForceGenerator* force , bool deleteHeapData = false );
	void update( const float dt );
	unsigned int getNumberOfParticles();
	Particle* getParticleFromList( unsigned int i );
};