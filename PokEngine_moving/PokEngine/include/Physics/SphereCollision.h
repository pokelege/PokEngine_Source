#pragma once
#include <PokEngineExportHeader.h>
#include <Physics\ParticleCollisionChecker.h>
class POKENGINE_SHARED SphereCollision : public ParticleCollisionChecker
{
public:
	virtual bool isCollided( Particle* particle1 = 0 , Particle* particle2 = 0 );
	virtual ParticleContact* generateContact( Particle* particle1 = 0 , Particle* particle2 = 0 );
	virtual void updateContact( ParticleContact* contact );
	static SphereCollision instance;
};