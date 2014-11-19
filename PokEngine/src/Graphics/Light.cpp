#include <Graphics\Light.h>
unsigned int Light::_id = 0;
Light::Light()
{
	Component::parent = 0;
	id = _id++;
}

glm::vec4& Light::getColor()
{
	return color;
}
void Light::setColor(const glm::vec4& newColor)
{
	color = newColor;
}

void Light::update()
{

}