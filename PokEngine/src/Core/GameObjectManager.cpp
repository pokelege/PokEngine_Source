#include <Core\GameObjectManger.h>
#include <Core\GameObject.h>
GameObjectManager GameObjectManager::globalGameObjectManager;

GameObjectManager::GameObjectManager() : gameObjects(0) {}
void GameObjectManager::initialize( unsigned int numGameObjectSlots )
{
	if ( gameObjects ) destroy();
	this->numGameObjectSlots = numGameObjectSlots;
	gameObjects = new GameObject[numGameObjectSlots];
	usedGameObjects = 0;
}
void GameObjectManager::destroy()
{
	if ( !gameObjects ) return;
	delete[] gameObjects;
	gameObjects = 0;
	numGameObjectSlots = 0;
	usedGameObjects = 0;
}
bool GameObjectManager::initialized()
{
	return gameObjects != 0;
}

GameObject* GameObjectManager::addGameObject()
{
	if ( !gameObjects ) return 0;
	return &gameObjects[usedGameObjects++];
}
void GameObjectManager::earlyUpdateParents()
{
	for ( unsigned int i = 0; i < usedGameObjects; ++i )
	{
		if(!gameObjects[i].parent) gameObjects[i].earlyUpdate();
	}
}
void GameObjectManager::updateParents()
{
	for ( unsigned int i = 0; i < usedGameObjects; ++i )
	{
		if ( !gameObjects[i].parent ) gameObjects[i].update();
	}
}
void GameObjectManager::lateUpdateParents()
{
	for ( unsigned int i = 0; i < usedGameObjects; ++i )
	{
		if ( !gameObjects[i].parent ) gameObjects[i].lateUpdate();
	}
}