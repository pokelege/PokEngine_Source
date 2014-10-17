#pragma once
#pragma warning (disable : 4201)
#pragma warning (disable : 4251)
#pragma warning (disable: 4239)
#include <glm.hpp>
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
class POKENGINE_SHARED Particle : public Component
{
public:
	glm::vec3 velocity , acceleration , totalForce;
	float mass , damping;
	bool freezeX , freezeY , freezeZ;
	Particle();
	void addToTotalForce( glm::vec3& force );
	glm::vec3 getAccelerationWithForces();

	glm::vec3 getPosition();
	void setPosition(glm::vec3& position);

	void callCollideEvents(Particle* other = 0);

	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
};