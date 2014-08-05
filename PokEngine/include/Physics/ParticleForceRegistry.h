#pragma once
#pragma warning(disable: 4251)
#pragma warning(disable:4201)
#include <PokEngineExportHeader.h>
#include <Misc\ArrayList.h>
class ParticleForceGenerator;
class Particle;
class POKENGINE_SHARED ParticleForceRegistry
{
protected:
	struct POKENGINE_SHARED ForceHolder
	{
		Particle* particle;
		ParticleForceGenerator* forceGenerator;
	};

	ArrayList<ForceHolder> registry;

public:
	virtual ~ParticleForceRegistry();
	void add( Particle* particle , ParticleForceGenerator* forceGenerator );

	void remove( Particle* particle , ParticleForceGenerator* forceGenerator );

	void clear();

	void update( const float dt );
};