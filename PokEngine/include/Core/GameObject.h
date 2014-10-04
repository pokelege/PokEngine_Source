#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <glm.hpp>
#include <vector>
class Component;
class POKENGINE_SHARED GameObject
{
public:
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
private:
	friend class GameObjectManager;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent;
public:
	GameObject();
	int addComponent(Component& component);
	bool removeComponent( Component& component );
	bool removeComponent( const unsigned int& component );
	int addChild( GameObject& child );
	bool removeChild( GameObject& child );
	bool removeChild( const unsigned int& child );
	void earlyUpdate();
	void update();
	void lateUpdate();
	glm::mat4 getWorldTransform();
	glm::mat4 getLocalTransform();
	template<class T> T* getComponent()
	{
		T* toReturn = 0;
		for ( unsigned int i = 0; i < components.size(); ++i )
		{
			if (T* test = dynamic_cast< T* >( components[i] ) )
			{
				toReturn = test;
			}
		}
		return toReturn;
	}
};