#include <Graphics\GraphicsCameraManager.h>
#include <Graphics\Camera.h>
#include <Core\GameObject.h>
GraphicsCameraManager GraphicsCameraManager::globalCameraManager;

GraphicsCameraManager::GraphicsCameraManager(): cameras(0) {}
void GraphicsCameraManager::initialize( unsigned int numCameraSlots )
{
	if ( cameras ) destroy();
	this->numCameraSlots = numCameraSlots;
	cameras = new Camera[numCameraSlots];
}
void GraphicsCameraManager::destroy()
{
	if ( !cameras ) return;
	delete[] cameras;
	cameras = 0;
	numCameraSlots = 0;
}
bool GraphicsCameraManager::initialized()
{
	return cameras != 0;
}

Camera* GraphicsCameraManager::addCamera()
{
	if ( !cameras ) return 0;
	for ( unsigned int i = 0; i < numCameraSlots; ++i )
	{
		if ( !cameras[i].parent ) return &cameras[i];
	}
	return 0;
}
void GraphicsCameraManager::drawAllCameras()
{
	for ( unsigned int i = 0; i < numCameraSlots; ++i )
	{
		if ( cameras[i].parent && cameras[i].parent->active )
		{
			cameras[i].draw();
		}
	}
}