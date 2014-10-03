#include <Graphics\Camera.h>
#include <gtc\matrix_transform.hpp>
#include <Core\GameObject.h>
Camera::Camera() {}

void Camera::attatch( GameObject* parent ) { this->parent = parent; }
void Camera::detatch() { parent = 0; }

glm::mat4 Camera::worldToView() const
{
	if ( !parent ) return glm::mat4();
	return glm::lookAt( parent->translate , parent->translate + direction , up );
}

void Camera::earlyUpdate() {}
void Camera::update() {}
void Camera::lateUpdate() {}
void Camera::earlyDraw() {}
void Camera::draw() {}
void Camera::lateDraw() {}
Camera::~Camera() {}