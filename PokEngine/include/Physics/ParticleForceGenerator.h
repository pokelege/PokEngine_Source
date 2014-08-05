#pragma once
#include "PokEngineExportHeader.h"
class Particle;
class POKENGINE_SHARED ParticleForceGenerator
{
public:
	virtual void update( Particle* particle , const float& dt ) = 0;
};