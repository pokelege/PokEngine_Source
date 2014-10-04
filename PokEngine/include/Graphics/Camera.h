#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
#define MAXRENDERS 1
class GraphicsRenderingManager;
class POKENGINE_SHARED Camera : public Component
{
	GraphicsRenderingManager** toRender;
	unsigned int numRenders;
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	GameObject* parent;
	glm::vec3 direction;
	glm::vec3 up;
	float x , y , width , height;
	float FOV;
	float nearestObject;
	Camera();
	void initializeRenderManagers( unsigned int numRenders = MAXRENDERS );
	void destroyRenderManagers();
	bool addRenderList( GraphicsRenderingManager* list );
	glm::mat4 worldToView() const;
	glm::mat4 viewToProjection() const;

	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
	virtual ~Camera();
};