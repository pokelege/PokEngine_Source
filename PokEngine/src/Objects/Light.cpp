#include <Objects\Light.h>

Light::Light()
{}

Light::Light(const glm::vec3& initPosition, const glm::vec4& initColor) : position(initPosition), color(initColor)
{}

glm::vec3& Light::getPosition()
{
	return position;
}

void Light::setPosition(const glm::vec3& newPosition)
{
	position = newPosition;
}

glm::vec4& Light::getColor()
{
	return color;
}
void Light::setColor(const glm::vec4& newColor)
{
	color = newColor;
}