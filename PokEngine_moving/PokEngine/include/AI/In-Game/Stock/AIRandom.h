#pragma once
#include <AI\In-Game\AI.h>
class POKENGINE_SHARED AIRandom: public AI
{
	GameNode* lastNode;
public:
	AIRandom() {}
	AIRandom( GameNodeHolder* nodeCollection, float moveSpeed = 10 , GameNode* from = nullptr );
	glm::vec3& getPosition();
	glm::vec3& getDirection();
	void setMoveSpeed( float moveSpeed );
	void update( float dt , glm::mat4* characterToTransform );
	void forceChangeTarget( GameNode* target );
	void forceChangeTarget( glm::vec3& target ) { target; };
};