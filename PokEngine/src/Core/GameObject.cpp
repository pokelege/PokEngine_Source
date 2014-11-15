#include <Core\GameObject.h>
#include <Core\Component.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\quaternion.hpp>
GameObject::GameObject() : scale(glm::vec3(1,1,1)), parent(0), active(true)  {}
int GameObject::addComponent( Component* component )
{
	int toReturn = components.size();
	component->attatch( this );
	components.push_back( component );
	return toReturn;
}
bool GameObject::removeComponent( Component* component )
{
	bool found = false;
	for ( unsigned int i = 0; i < components.size(); ++i )
	{
		if ( components[i] == component )
		{
			found = true;
			components[i]->detatch();
			components.erase( components.begin() + i);
			break;
		}
	}
	return found;
}
bool GameObject::removeComponent( const unsigned int& component )
{
	bool toReturn = false;
	if ( component < components.size() )
	{
		components[component]->detatch();
		components.erase( components.begin() + component );
		toReturn = true;
	}
	return toReturn;
}
int GameObject::addChild( GameObject* child )
{
	int toReturn = children.size();
	child->parent = this;
	children.push_back( child );
	return toReturn;
}
bool GameObject::removeChild( GameObject* child )
{
	bool found = false;
	for ( unsigned int i = 0; i < children.size(); ++i )
	{
		if ( children[i] == child )
		{
			found = true;
			child->parent = 0;
			children.erase( children.begin() + i );
			break;
		}
	}
	return found;
}
bool GameObject::removeChild( const unsigned int& child )
{
	bool toReturn = false;
	if ( child < children.size() )
	{
		children.erase( children.begin() + child );
		toReturn = true;
	}
	return toReturn;
}

void GameObject::earlyUpdate()
{
	if ( !active ) return;
	for ( unsigned int i = 0; i < components.size(); ++i )
	{
		components[i]->earlyUpdate();
	}
	for ( unsigned int i = 0; i < children.size(); ++i )
	{
		children[i]->earlyUpdate();
	}
}
void GameObject::update()
{
	if ( !active ) return;
	for ( unsigned int i = 0; i < components.size(); ++i )
	{
		components[i]->update();
	}
	for ( unsigned int i = 0; i < children.size(); ++i )
	{
		children[i]->update();
	}
}
void GameObject::lateUpdate()
{
	if ( !active ) return;
	for ( unsigned int i = 0; i < components.size(); ++i )
	{
		components[i]->lateUpdate();
	}
	for ( unsigned int i = 0; i < children.size(); ++i )
	{
		children[i]->lateUpdate();
	}
}

void GameObject::earlyDraw()
{
	if ( !active ) return;
	for ( unsigned int i = 0; i < components.size(); ++i )
	{
		components[i]->earlyDraw();
	}
	for ( unsigned int i = 0; i < children.size(); ++i )
	{
		children[i]->earlyDraw();
	}
}
void GameObject::lateDraw()
{
	if ( !active ) return;
	for ( unsigned int i = 0; i < components.size(); ++i )
	{
		components[i]->lateDraw();
	}
	for ( unsigned int i = 0; i < children.size(); ++i )
	{
		children[i]->lateDraw();
	}
}

glm::mat4 GameObject::getWorldTransform()
{
	if ( parent ) return  parent->getWorldTransform() * getLocalTransform();
	else return getLocalTransform();
}

glm::mat4 GameObject::getLocalTransform()
{
	return glm::translate( glm::mat4() , translate ) * glm::mat4_cast( rotate ) * glm::scale( glm::mat4() , scale );
}