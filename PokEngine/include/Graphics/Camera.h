#pragma once
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
#include <glm.hpp>
class POKENGINE_SHARED Camera : public Component
{
public:
	glm::vec3 direction;
	glm::vec3 up;

	Camera();
	glm::mat4 worldToView() const;
};