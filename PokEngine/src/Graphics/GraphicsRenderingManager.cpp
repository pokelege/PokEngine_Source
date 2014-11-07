#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\RenderableInfo.h>
#include <Graphics\Camera.h>
#include <Graphics\GraphicsSharedUniformManager.h>
#include <Graphics\CommonUniformNames.h>
#include <glm.hpp>
#include <Core\GameObject.h>
GraphicsRenderingManager GraphicsRenderingManager::globalRenderingManager;

GraphicsRenderingManager::GraphicsRenderingManager(): renderables(0) {}
void GraphicsRenderingManager::initialize( unsigned int numRenderableSlots )
{
	if ( renderables ) destroy();
	this->numRenderableSlots = numRenderableSlots;
	renderables = new RenderableInfo[numRenderableSlots];
}
void GraphicsRenderingManager::destroy()
{
	if ( !renderables ) return;
	delete[] renderables;
	renderables = 0;
	numRenderableSlots = 0;
}
bool GraphicsRenderingManager::initialized()
{
	return renderables != 0;
}

RenderableInfo* GraphicsRenderingManager::addRenderable()
{
	RenderableInfo* renderable = 0;
	for ( unsigned i = 0; i < numRenderableSlots; i++ )
	{
		if ( !renderables[i].slotUsed )
		{
			renderable = &renderables[i];
			renderable->slotUsed = true;
			break;
		}
	}
	return renderable;
}
void GraphicsRenderingManager::drawAll( const Camera& camera )
{
	glm::mat4 projection = camera.viewToProjection();
	glm::mat4 view = camera.worldToView();
	for ( unsigned int i = 0; i < numRenderableSlots; ++i )
	{
		if ( renderables[i].parent && renderables[i].parent->active )
		{
			if ( renderables[i].sharedUniforms )
			{
				renderables[i].sharedUniforms->setSharedUniform( VIEWTOPROJECTION , PT_MAT4 , reinterpret_cast<const void*>( &projection ) );
				renderables[i].sharedUniforms->setSharedUniform( WORLDTOVIEW , PT_MAT4 , reinterpret_cast<const void*>( &view ) );
			}
			else
			{
				renderables[i].setRenderableUniform( VIEWTOPROJECTION , PT_MAT4 , reinterpret_cast< const void* >( &projection ) );
				renderables[i].setRenderableUniform( WORLDTOVIEW , PT_MAT4 , reinterpret_cast< const void* >( &view ) );
			}
			renderables[i].draw();
		}
	}
}