#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
#define MAXRENDERS 1
class GraphicsRenderingManager;
struct TextureInfo;
class POKENGINE_SHARED Camera : public Component
{
	GraphicsRenderingManager** toRender;
	unsigned int numRenders;
	int frameBufferID;
	int frameBufferWidth , frameBufferHeight;
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
	bool drawCamera, drawFrameBuffer;
	Camera();
	void initializeRenderManagers( unsigned int numRenders = MAXRENDERS );
	void destroyRenderManagers();
	bool addRenderList( GraphicsRenderingManager* list );
	glm::mat4 worldToView() const;
	glm::mat4 viewToProjection() const;
	glm::mat4 viewToProjectionFrameBuffer() const;
	void attatchFrameBuffer( TextureInfo* color = 0 , TextureInfo* depth = 0 );

	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();
	virtual ~Camera();
};