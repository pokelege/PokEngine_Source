#pragma once
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
class FirstPersonCameraInput : public Component
{
	GameObject* parent;
	glm::vec2 oldMousePosition;
protected:
	void attatch( GameObject* parent );
	void detatch();
public:
	unsigned int forward , back , left , right , up , down;
	FirstPersonCameraInput();
	FirstPersonCameraInput(GameObject* parent);
	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual ~FirstPersonCameraInput();
};