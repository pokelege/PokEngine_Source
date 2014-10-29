#pragma once
#pragma warning(disable: 4251)
#pragma warning(disable:4201)
#include <PokEngineExportHeader.h>
#include <glm.hpp>
class Particle;
class POKENGINE_SHARED ParticleContact
{
	friend class ParticleContactResolver;
	virtual void resolveVelocity( );
	virtual void resolveInterpenetration( );
protected:
	virtual const float calculateSeparatingVelocity();
public:
	Particle* particle[2];

	float restitution;
	glm::vec3 contactNormal;
	float penetration;
	virtual void resolve();
};