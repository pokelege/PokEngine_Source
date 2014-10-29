#pragma once
#pragma warning (disable : 4251)
#pragma warning( disable : 4512)
#pragma warning (disable : 4201)
#include <PokEngineExportHeader.h>
#include <glm.hpp>

class POKENGINE_SHARED Camera
{
	const float mouseSpeed;
	const float moveSpeed;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec2 oldMousePosition;
	float x;
	float y;
public:
	Camera();
	glm::mat4 worldToView() const;
	void setPosition( const glm::vec3& position );
	void setDirection( const glm::vec3& direction );
	void reset();
	void update(const glm::vec2& mousePosition);
	void setOldMousePosition(const glm::vec2& mousePosition);
	glm::vec3& getPosition();
	glm::vec3& getDirection();
	glm::vec2& getOldMousePosition();
	void moveForward();
	void moveBack();
	void left();
	void right();
	void moveUp();
	void moveDown();
};

