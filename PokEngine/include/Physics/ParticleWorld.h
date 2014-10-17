#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <Misc\ArrayList.h>
#include <Physics\ParticleContactResolver.h>
#include <Physics\Particle.h>
#include <Physics\SphereCollision.h>
#include <FastDelegate.h>
class POKENGINE_SHARED ParticleWorld
{
	ArrayList<Particle> particles;
	ParticleContactResolver collisionResolver;
	ParticleCollisionChecker* collisionChecker;
	void processCollisions();
public:
	ParticleWorld();
	void initialize( ParticleCollisionChecker* collisionChecker = &SphereCollision::instance , unsigned int numberOfCollsionIterations = 10 );
	void numberOfCollsionIterations( unsigned int numberOfCollsionIterations );
	void addParticleToManage( Particle* particle );
	void removeParticleToManage( Particle* particle , bool deleteHeapData = false );
	void update();
	unsigned int getNumberOfParticles();
	Particle* getParticleFromList( unsigned int i );
	static ParticleWorld global;
};