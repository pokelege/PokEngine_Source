#pragma once
#include "PokEngineExportHeader.h"
class Particle;
class ParticleContact;
class POKENGINE_SHARED ParticleCollisionChecker
{
public:
	virtual bool isCollided( Particle* particle1 = 0 , Particle* particle2 = 0 ) = 0;
	virtual ParticleContact* generateContact( Particle* particle1 = 0 , Particle* particle2 = 0 ) = 0;
	virtual void updateContact( ParticleContact* contact ) = 0;
};