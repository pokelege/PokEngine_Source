#pragma once
#include <PokEngineExportHeader.h>
#include <Misc\ArrayList.h>
class ParticleCollisionChecker;
class ParticleContact;
class POKENGINE_SHARED ParticleContactResolver
{
protected:
	unsigned int iterations;
	unsigned int iterationsUsed;
	ParticleCollisionChecker* collisionChecker;
public:
	ParticleContactResolver( unsigned int iterations, ParticleCollisionChecker* penetrationUpdater );
	void setIterations( unsigned int iterations );
	void resolveContacts( ArrayList<ParticleContact>* contactArray , const float dt );
};