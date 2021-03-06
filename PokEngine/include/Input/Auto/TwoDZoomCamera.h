#pragma once 
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
class POKENGINE_SHARED TwoDZoomCamera : public Component
{
	GameObject* parent;
	GameObject** tracking;
	unsigned int numTracking;
	glm::vec3 targetPos;
protected:
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	float minDistance;
	float maxDistance;
	float zoomScale;
	void initialize(unsigned int numObjectsToTrack);
	void destroy();
	bool initialized();
	bool addGameObjectToTrack( GameObject* object );
	TwoDZoomCamera();
	~TwoDZoomCamera();
	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
};