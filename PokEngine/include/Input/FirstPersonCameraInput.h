#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
class POKENGINE_SHARED FirstPersonCameraInput : public Component
{
	GameObject* parent;
	glm::vec2 oldMousePosition;
protected:
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	unsigned int forward , back , left , right , up , down;
	float rotationSensitivity;
	float moveSensitivity;
	FirstPersonCameraInput();
	FirstPersonCameraInput(GameObject* parent);
	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual ~FirstPersonCameraInput();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
};