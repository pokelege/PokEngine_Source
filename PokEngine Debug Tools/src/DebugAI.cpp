#pragma warning(disable:4201)
#include "DebugAI.h"
#include "DebugMenu.h"
#include "DebugDrawManager.h"
#include "AI\In-Game\Stock\AIRandom.h"
#include "AI\In-Game\GameNode.h"
#include "AI\In-Game\GameNodeHolder.h"
#include "Objects\Camera.h"
#include <gtx\transform.hpp>
AI* DebugAI::linkedAI = nullptr;
Camera* DebugAI::linkedCamera = nullptr;
bool DebugAI::follow = false;
bool DebugAI::drawnodes = false;
bool DebugAI::drawpath = false;
bool DebugAI::enableForcedTargetChange = false;

void DebugAI::linkAI( AI* link , float minSpeed , float maxSpeed )
{
	delinkAI();
	linkedAI = link;
	DebugMenuAddBoolControl( "Draw Nodes: " , &drawnodes );
	DebugMenuAddBoolControl( "Draw Path: " , &drawpath );
	DebugMenuAddFloatControl( "AI Speed: " , &linkedAI->moveSpeed , minSpeed , maxSpeed );
}

void DebugAI::linkCamera( Camera* camera )
{
	if ( linkedAI != nullptr )
	{
		delinkCamera();
		linkedCamera = camera;
		DebugMenuAddBoolControl( "Camera to AI: " , &follow );
		if (typeid( *linkedAI  ) == typeid( AIRandom ))
		DebugMenuAddBoolControl( "Change target with click: " , &enableForcedTargetChange );
	}
}

void DebugAI::delinkAI()
{
	delinkCamera();
	DebugMenuDelete( "Draw Nodes: " );
	DebugMenuDelete( "Draw Path: " );
	DebugMenuDelete( "AI Speed: " );
	linkedAI = nullptr;
}
void DebugAI::delinkCamera()
{
	DebugMenuDelete( "Camera to AI: " );
	if ( linkedAI != nullptr && typeid( *linkedAI ) == typeid( AIRandom ) )
		DebugMenuDelete( "Change target with click: " );
	linkedCamera = nullptr;
	follow = false;
}

void DebugAI::moveCamToAI()
{
	if ( linkedCamera != nullptr && follow )
	{
		if ( drawpath )
		{
			linkedCamera->setPosition( linkedAI->getPosition() + ( 2.0f * linkedAI->getDirection() ) + glm::vec3(0,1, 0) );
		}
		else
		{
			linkedCamera->setPosition( linkedAI->getPosition() + ( 2.0f * linkedAI->getDirection() ) );
		}
		linkedCamera->setDirection(linkedAI->getDirection() );
	}
}

void DebugAI::drawNodes()
{
	if ( linkedAI != nullptr )
	{
		if ( drawpath )
		{
			Stack<GameNode>::SIterator* iterator = linkedAI->star.theStack.iterator();
			if ( iterator->hasNext() )
			{
				GameNode* tempStart = iterator->next();
				GameNode* tempEnd = iterator->next();
				DebugDrawAddVectorD( linkedAI->position, tempStart->getPosition() , glm::vec4( 1 , 1 , 1 , 1 ) , 1.5f , 0.3f , 0 , true , false );

				if ( tempEnd == nullptr )
				{
					DebugDrawAddSphereD( glm::translate( tempStart->getPosition() ) , glm::vec4( 0 , 1 , 0 , 1 ) , 0 , true , false );
				}
				else
				{
					DebugDrawAddSphereD( glm::translate( tempStart->getPosition() ) , glm::vec4( 1 , 1 , 0 , 1 ) , 0 , true , false );
					DebugDrawAddVectorD( tempStart->getPosition() , tempEnd->getPosition() , glm::vec4( 1 , 1 , 1 , 1 ) , 1.5f , 0.3f , 0 , true , false );
					if ( !iterator->hasNext() )
					{
						DebugDrawAddSphereD( glm::translate( tempEnd->getPosition() ) , glm::vec4( 0 , 1 , 0 , 1 ) , 0 , true , false );
					}
					else
					{
						DebugDrawAddSphereD( glm::translate( tempEnd->getPosition() ) , glm::vec4( 1 , 0 , 0 , 1 ) , 0 , true , false );
					}
				}

				while ( iterator->hasNext() )
				{
					tempStart = tempEnd;
					tempEnd = iterator->next();
					if ( !iterator->hasNext() )
					{
						DebugDrawAddSphereD( glm::translate( tempEnd->getPosition() ) , glm::vec4( 0 , 1 , 0 , 1 ) , 0 , true , false );
					}
					else
					{
						DebugDrawAddSphereD( glm::translate( tempEnd->getPosition() ) , glm::vec4( 1 , 0 , 0 , 1 ) , 0 , true , false );
					}
					DebugDrawAddVectorD( tempStart->getPosition() , tempEnd->getPosition() , glm::vec4( 1 , 1 , 1 , 1 ) , 1.5f , 0.3f , 0 , true , false );
				}
			}

			delete iterator;
		}
		if ( drawnodes )
		{
			for ( unsigned int i = 0; i < linkedAI->nodeCollection->getcurrentMaxNodes(); i++ )
			{
				DebugDrawAddSphereD( glm::translate( linkedAI->nodeCollection->getNode( i )->getPosition() ) , glm::vec4( 1 , 1 , 1 , -1 ) , 0 , true , false );
				GameNode::GameNodeConnectionHolder* connections = nullptr;
				unsigned int nodes = 0;
				linkedAI->nodeCollection->getNode( i )->getConnections( &connections , &nodes );
				for ( unsigned int j = 0; j < nodes; j++ )
				{
					DebugDrawAddLineD( linkedAI->nodeCollection->getNode( i )->getPosition() , connections[j].connectedNode->getPosition() , glm::vec4( 1 , 1 , 1 , 1 ) , 0 , true , false );
				}
			}
		}
	}
}

void DebugAI::forceTargetChange( glm::vec3& ray)
{
	if ( enableForcedTargetChange && linkedAI != nullptr && linkedCamera != nullptr && ( typeid( *linkedAI ) == typeid( AIRandom ) ) )
	{
		AIRandom* toChange = ( AIRandom* ) linkedAI;
		float closeTest = FLT_MAX;
		int nodeToSelect = -1;

		for ( unsigned int i = 0; i < linkedAI->nodeCollection->getcurrentMaxNodes(); i++ )
		{
			float nearestTest = FLT_MAX;
			if ( linkedAI->nodeCollection->getNode( i )->intersected( linkedCamera->getPosition() , ray , &nearestTest ) )
			{
				if ( nearestTest < closeTest && nearestTest >= 0 )
				{
					closeTest = nearestTest;
					nodeToSelect = i;
				}
			}
		}

		if ( nodeToSelect >= 0 )
		{
			toChange->forceChangeTarget( linkedAI->nodeCollection->getNode( nodeToSelect ) );
		}
	}
}