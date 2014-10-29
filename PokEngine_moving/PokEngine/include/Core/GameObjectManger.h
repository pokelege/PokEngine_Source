#pragma once
#include <PokEngineExportHeader.h>
#define MAX_GAMEOBJECTS 100

class GameObject;
class POKENGINE_SHARED GameObjectManager
{
	GameObject* gameObjects;
	unsigned int numGameObjectSlots;
	unsigned int usedGameObjects;
public:
	static GameObjectManager globalGameObjectManager;

	GameObjectManager();
	void initialize( unsigned int numGameObjectSlots = MAX_GAMEOBJECTS );
	void destroy();
	bool initialized();

	GameObject* addGameObject();
	void earlyUpdateParents();
	void updateParents();
	void lateUpdateParents();
	void earlyDrawParents();
	void lateDrawParents();
};