#include <Graphics\Light.h>
unsigned int Light::_id = 0;
Light::Light() : depthTexture(0)
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

TextureInfo* Light::setDepthTexture( TextureInfo* depthTexture )
{
	TextureInfo* toReturn = this->depthTexture;
	this->depthTexture = depthTexture;
	return toReturn;
}
TextureInfo* Light::getDepthTexture()
{
	return depthTexture;
}

void Light::update()
{

}