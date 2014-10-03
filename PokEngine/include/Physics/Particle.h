#pragma once
#pragma warning (disable : 4201)
#pragma warning (disable : 4251)
#pragma warning (disable: 4239)
#include <glm.hpp>
#include "PokEngineExportHeader.h"
class POKENGINE_SHARED Particle
{
	glm::vec3 position , velocity , acceleration, totalForce;
	float mass , damping;
public:
	Particle( glm::vec3& position = glm::vec3() , glm::vec3& velocity = glm::vec3() , glm::vec3& acceleration = glm::vec3() , const float mass = 1.0f , const float damping = 1.0f );
	glm::vec3& getPosition();
	void setPosition( glm::vec3& position );
	glm::vec3& getVelocity();
	void setVelocity( glm::vec3& velocity );
	glm::vec3& getAcceleration();
	void setAcceleration( glm::vec3& acceleration );
	float& getMass();
	void setMass( float mass );
	float& getDamping();
	void setDamping( float damping );
	glm::vec3& getTotalForce();
	void setTotalForce( glm::vec3& totalForce );
	void addToTotalForce( glm::vec3& force );
	glm::vec3 getAccelerationWithForces();

	virtual void update( const float& dt );
};