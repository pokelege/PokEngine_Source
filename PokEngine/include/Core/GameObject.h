#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <glm.hpp>
#include <gtc\quaternion.hpp>
#include <vector>
class Component;
class POKENGINE_SHARED GameObject
{
public:
	glm::vec3 translate;
	glm::quat rotate;
	glm::vec3 scale;
	bool active;
private:
	friend class GameObjectManager;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent;
public:
	GameObject();
	int addComponent(Component* component);
	bool removeComponent( Component* component );
	bool removeComponent( const unsigned int& component );

	int addChild( GameObject* child );
	bool removeChild( GameObject* child );
	bool removeChild( const unsigned int& child );
	int numChildren();
	GameObject* getChild();
	void earlyUpdate();
	void update();
	void lateUpdate();
	void earlyDraw();
	void lateDraw();
	glm::mat4 getWorldTransform();
	glm::mat4 getLocalTransform();
	glm::vec3 getWorldTranslate();
	glm::quat getWorldRotation();
	glm::vec3 getWorldScale();
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

	template<class T> std::vector<T*> getComponents()
	{
		std::vector<T*> toReturn;
		for ( unsigned int i = 0; i < components.size(); ++i )
		{
			if ( T* test = dynamic_cast< T* >( components[i] ) )
			{
				toReturn.push_back( test );
			}
		}
		return toReturn;
	}

	template<class T> T* getComponentInChildren()
	{
		T* toReturn = 0;
		for ( unsigned int i = 0; i < children.size(); ++i )
		{
			toReturn = children.at( i )->getComponent<T>();
			if ( toReturn)
			{
				break;
			}
			else
			{
				toReturn = children.at( i )->getComponentInChildren<T>();
				if ( toReturn )
				{
					break;
				}
			}
		}
		return toReturn;
	}
	//std::vector<Component*> getComponentsInChildren();
};