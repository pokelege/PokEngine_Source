#pragma once
#include <Core\Component.h>
class Particle;
class CollisionEvent : public Component
{
public:
	virtual void collisionEvent(Particle* other = 0) = 0;
};