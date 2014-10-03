#pragma once
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
class POKENGINE_SHARED Camera : public Component
{
	GameObject* parent;
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	glm::vec3 direction;
	glm::vec3 up;

	Camera();
	glm::mat4 worldToView() const;

	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual ~Camera();
};