#include <Graphics\Camera.h>
#include <gtc\matrix_transform.hpp>
#include <Core\GameObject.h>
#include <Core\WindowInfo.h>
#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\CommonGraphicsCommands.h>
Camera::Camera() : toRender(0) {}

void Camera::attatch( GameObject* parent ) { this->parent = parent; }
void Camera::detatch() { parent = 0; }

glm::mat4 Camera::worldToView() const
{
	if ( !parent ) return glm::mat4();
	return glm::lookAt( parent->translate , parent->translate + direction , up );
}

glm::mat4 Camera::viewToProjection() const
{
	return glm::infinitePerspective( FOV , (width * WindowInfo::width ) / (height / WindowInfo::height), nearestObject );
}

void Camera::initializeRenderManagers( unsigned int numRenders )
{
	if ( toRender ) destroyRenderManagers();
	this->numRenders = numRenders;
	toRender = new GraphicsRenderingManager*[numRenders];
	for ( unsigned int i = 0; i < numRenders; ++i ) toRender[i] = 0;
}

void Camera::destroyRenderManagers()
{
	if ( !toRender ) return;
	delete[] toRender;
	toRender = 0;
	numRenders = 0;
}

bool Camera::addRenderList( GraphicsRenderingManager* list )
{
	for ( unsigned int i = 0; i < numRenders; ++i )
	{
		if ( !toRender[i] )
		{
			toRender[i] = list;
			return true;
		}
	}
	return false;
}

void Camera::earlyUpdate() {}
void Camera::update() {}
void Camera::lateUpdate() {}
void Camera::earlyDraw() {}
void Camera::draw()
{
	CommonGraphicsCommands::clearCommon();
	CommonGraphicsCommands::setViewPort( (int)(WindowInfo::width * x),  (int)(WindowInfo::height * y) , (int)(WindowInfo::width * width ), (int)(WindowInfo::height * height) );
	for ( unsigned int i = 0; i < numRenders; ++i )
	{
		if ( toRender[i] )
		{
			toRender[i]->drawAll(*this);
		}
	}
}
void Camera::lateDraw() {}
Camera::~Camera() {}