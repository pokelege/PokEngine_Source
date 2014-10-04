#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\RenderableInfo.h>
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
		if ( renderables[i].geometryInfo == nullptr )
		{
			renderable = &renderables[i];
			break;
		}
	}
	return renderable;
}
void GraphicsRenderingManager::drawAll()
{
	for ( unsigned int i = 0; i < numRenderableSlots; ++i )
	{
		renderables[i].draw();
	}
}