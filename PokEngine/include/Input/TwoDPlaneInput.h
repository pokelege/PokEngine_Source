#pragma once
#include <Core\CommonWarningDisables.h>
#include <Core\Component.h>
#include <glm.hpp>
#include <PokEngineExportHeader.h>
class POKENGINE_SHARED TwoDPlaneInput : public Component
{
	GameObject* parent;

protected:
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	unsigned int left , right , up , down;
	glm::vec3 x;
	glm::vec3 y;
	float moveSensitivity;
	TwoDPlaneInput();
	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
};