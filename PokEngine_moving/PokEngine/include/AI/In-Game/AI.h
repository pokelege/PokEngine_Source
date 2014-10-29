#pragma once
#include <PokEngineExportHeader.h>
#include <AI\In-Game\Astar.h>
#pragma warning(disable:4201)
#pragma warning(disable:4251)
#include <glm.hpp>
class DebugAI;
class GameNodeHolder;
class POKENGINE_SHARED AI
{
	friend DebugAI;
protected:
	Astar star;
	glm::vec3 position;
	glm::vec3 direction;
	float moveSpeed;
	GameNodeHolder* nodeCollection;
public:
	virtual glm::vec3& getPosition() = 0;
	virtual glm::vec3& getDirection() = 0;
	virtual void setMoveSpeed( float moveSpeed ) = 0;
	virtual void update( float dt , glm::mat4* characterToTransform ) = 0;
};