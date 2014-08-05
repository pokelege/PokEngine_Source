#include "AI\In-Game\Stock\AIRandom.h"
#include "AI\In-Game\GameNodeHolder.h"
#include "AI\In-Game\GameNode.h"
#include "Misc\RandomItemsGenerator.h"
#include "gtx\transform.hpp"
AIRandom::AIRandom( GameNodeHolder* nodeCollection , float moveSpeed , GameNode* from )
{
	this->moveSpeed = moveSpeed;
	this->nodeCollection = nodeCollection;
	if ( from == nullptr )
	{
		while ( star.peek() == nullptr )
		{
			star.generateQueue( nodeCollection->getNode( Engine::RandomItemsGenerator::RandomRangedInt( 0 , nodeCollection->getcurrentMaxNodes() - 1 ) ) , nodeCollection->getNode( Engine::RandomItemsGenerator::RandomRangedInt( 0 , nodeCollection->getcurrentMaxNodes() - 1 ) ) );
			star.poll();
		}
	}

	GameNode* currentFrom = star.poll();
	lastNode = currentFrom;
	position = currentFrom->getPosition();
	while ( star.peek() == nullptr )
	{
		star.generateQueue( currentFrom , nodeCollection->getNode( Engine::RandomItemsGenerator::RandomRangedInt( 0 , nodeCollection->getcurrentMaxNodes() - 1 ) ) );
		star.poll();
	}
	direction = glm::normalize( star.peek()->getPosition() - position );
}

void AIRandom::setMoveSpeed( float moveSpeed )
{
	this->moveSpeed = moveSpeed;
}

void AIRandom::update( float dt, glm::mat4* characterToTransform )
{
	position += (( dt * moveSpeed ) * direction);
	while ( glm::dot(glm::normalize( star.peek()->getPosition() - position ), direction) < 0 )
	{
		GameNode* newFrom = star.poll();
		float oldDistance = glm::length( newFrom->getPosition() - position );
		while ( star.peek() == nullptr )
		{
			star.generateQueue( newFrom , nodeCollection->getNode( Engine::RandomItemsGenerator::RandomRangedInt( 0 , nodeCollection->getcurrentMaxNodes() - 1 ) ) );
			star.poll();
		}
		lastNode = newFrom;
		direction = glm::normalize( star.peek()->getPosition() - newFrom->getPosition() );
		position = newFrom->getPosition() + (oldDistance * direction);
	}

	glm::vec3 zb = glm::normalize( glm::cross( direction , glm::vec3( 0 , -1.0f , 0 ) ) );
	glm::vec3 yb = glm::normalize( glm::cross( direction , zb ) );
	glm::mat4 rot( glm::vec4( direction , 0 ) , glm::vec4( yb , 0 ) , glm::vec4( zb , 0 ) , glm::vec4( 0 , 0 , 0 , 1 ) );
	*characterToTransform = glm::translate( position ) * rot;
}

void AIRandom::forceChangeTarget( GameNode* target )
{
	star.clear();
	direction = glm::normalize( lastNode->getPosition() - position );
	star.generateQueue( lastNode , target );
}

glm::vec3& AIRandom::getPosition()
{
	return position;
}
glm::vec3& AIRandom::getDirection()
{
	return direction;
}