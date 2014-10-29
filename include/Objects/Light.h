#pragma once
#pragma warning (disable : 4251)
#pragma warning (disable : 4201)
#include <PokEngineExportHeader.h>
#include <glm.hpp>

class POKENGINE_SHARED Light
{
protected:
	glm::vec3 position;
	glm::vec4 color;
public:

	Light();
	Light(const glm::vec3& initPosition, const glm::vec4& initColor);

	glm::vec3& getPosition();
	void setPosition(const glm::vec3& newPosition);

	glm::vec4& getColor();
	void setColor(const glm::vec4& newColor);
};