#pragma once
#pragma warning (disable : 4201)
#pragma warning (disable : 4251)
#pragma warning (disable: 4239)
#include <glm.hpp>
#include "PokEngineExportHeader.h"
#include <Core\Component.h>
class POKENGINE_SHARED Particle : public Component
{
	GameObject* parent;
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	glm::vec3 velocity , acceleration , totalForce;
	float mass , damping;
	bool freezeX , freezeY , freezeZ;
	Particle();
	void addToTotalForce( glm::vec3& force );
	glm::vec3 getAccelerationWithForces();

	glm::vec3 getPosition();
	void setPosition(glm::vec3& position);

	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
};