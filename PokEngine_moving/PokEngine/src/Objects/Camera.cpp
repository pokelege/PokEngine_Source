#include "Objects\Camera.h"
#include <gtx\transform.hpp>
Camera::Camera() : direction(0, 0, -1.0f), up(0, 1.0f, 0), mouseSpeed(0.3f), moveSpeed(0.3f), position(0,0,0)
{

}

glm::mat4 Camera::worldToView() const
{
	return glm::lookAt( position , position + direction , up );
}

void Camera::setPosition( const glm::vec3& position )
{
	this->position = position;
}

void Camera::setDirection( const glm::vec3& direction )
{
	this->direction = direction;
}

glm::vec3& Camera::getPosition()
{
	return position;
}

glm::vec3& Camera::getDirection()
{
	return direction;
}

void Camera::reset()
{
	position = glm::vec3( 0 , 0 , 0 );
	direction = glm::vec3( 0 , 0 , -1.0f );
	up = glm::vec3( 0 , 1.0f , 0 );
}

void Camera::update( const glm::vec2& mousePosition )
{
	glm::vec2 delta = mousePosition - oldMousePosition;

	glm::vec3 b = glm::cross( direction , up );

	direction = glm::mat3( glm::rotate( -delta.x * mouseSpeed , up ) * glm::rotate( -delta.y * mouseSpeed , b ) ) * direction;
}

void Camera::setOldMousePosition( const glm::vec2& mousePosition )
{
	oldMousePosition = mousePosition;
}

glm::vec2& Camera::getOldMousePosition()
{
	return oldMousePosition;
}

void Camera::moveForward()
{
	position += moveSpeed * direction;
}
void Camera::moveBack()
{
	position -= moveSpeed * direction;
}
void Camera::left()
{
	position += -moveSpeed * glm::normalize( glm::cross( direction , up ) );
}
void Camera::right()
{
	position += moveSpeed * glm::normalize( glm::cross( direction , up ) );
}
void Camera::moveUp()
{
	position += moveSpeed * up;
}
void Camera::moveDown()
{
	position += -moveSpeed * up;
}