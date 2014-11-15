#pragma once
#pragma warning (disable : 4251)
#pragma warning (disable : 4201)
#include <PokEngineExportHeader.h>
#include <glm.hpp>
#include <Core\Component.h>
class POKENGINE_SHARED Light : public Component
{
	static unsigned int _id;
	unsigned int id;
protected:
	glm::vec4 color;
public:
	Light();
	glm::vec4& getColor();
	void setColor(const glm::vec4& newColor);
	virtual void update();
};