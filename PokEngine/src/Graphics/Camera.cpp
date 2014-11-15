#define GLM_FORCE_RADIANS
#include <Graphics\Camera.h>
#include <gtc\matrix_transform.hpp>
#include <Core\GameObject.h>
#include <Core\WindowInfo.h>
#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\CommonGraphicsCommands.h>
Camera::Camera() : toRender(0), parent(0), up(glm::vec3(0,1,0)), direction(glm::vec3(0,0,-1)), x(0), y(0), width(1), height(1), FOV(60), nearestObject(0.01f) {}
void Camera::attatch( GameObject* parent ) { this->parent = parent; }
void Camera::detatch() { parent = 0; }

glm::mat4 Camera::worldToView() const
{
	if ( !parent ) return glm::mat4();
	return glm::lookAt( parent->translate , parent->translate + direction , up );
}

glm::mat4 Camera::viewToProjection() const
{
	float apect = ( width * WindowInfo::width ) / ( height * WindowInfo::height );
	return glm::infinitePerspective( glm::radians(FOV) ,apect, nearestObject );
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
	CommonGraphicsCommands::setViewPort( ( int ) ( WindowInfo::width * x ) , ( int ) ( WindowInfo::height * y ) , ( int ) ( WindowInfo::width * width ) , ( int ) ( WindowInfo::height * height ) );
	CommonGraphicsCommands::clearCommon();
	for ( unsigned int i = 0; i < numRenders; ++i )
	{
		if ( toRender[i] )
		{
			toRender[i]->drawAll(*this);
		}
	}
}
void Camera::lateDraw() {}
Camera::~Camera() { destroyRenderManagers(); }