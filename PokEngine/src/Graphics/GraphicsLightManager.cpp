#include <Graphics\GraphicsLightManager.h>
#include <Graphics\Light.h>
#include <Graphics\RenderableInfo.h>
#include <Graphics\GraphicsSharedUniformManager.h>
#include <Graphics\CommonUniformNames.h>
#include <Core\GameObject.h>
GraphicsLightManager GraphicsLightManager::global;

GraphicsLightManager::GraphicsLightManager() : lights(0)
{

}

void GraphicsLightManager::initialize( unsigned int numLightSlots )
{
	if ( lights ) destroy();
	this->numLightSlots = numLightSlots;
	lights = new Light[numLightSlots];
}
void GraphicsLightManager::destroy()
{
	if ( lights )
	{
		numLightSlots = 0;
		delete[] lights;
		lights = 0;
	}
}
bool GraphicsLightManager::initialized()
{
	return lights != 0;
}
Light* GraphicsLightManager::addLight()
{
	if ( !lights ) return 0;
	for ( unsigned int i = 0; i < numLightSlots; ++i )
	{
		if ( !lights[i].parent ) return &lights[i];
	}
	return 0;
}
void GraphicsLightManager::applyLights( GraphicsSharedUniformManager* uniformManager )
{
	if ( !( uniformManager || lights ) ) return;
	lightColor.clear();
	lightPosition.clear();
	for ( unsigned int i = 0; i < numLightSlots; ++i )
	{
		if ( !lights[i].parent ) continue;
		lightColor.push_back( lights[i].getColor() );
		lightPosition.push_back( lights[i].parent->translate );
	}
	if ( !( lightColor.size() || lightPosition.size() ) ) return;
	uniformManager->setSharedUniform( LIGHTCOLOR , PT_VEC4 , &lightColor[0] , lightColor.size() );
	uniformManager->setSharedUniform( LIGHTPOSITION , PT_VEC3 , &lightPosition[0] , lightPosition.size() );
}