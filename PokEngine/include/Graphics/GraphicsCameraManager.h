#pragma once
#include <PokEngineExportHeader.h>
#define MAX_CAMERAS 1
class Camera;
class POKENGINE_SHARED GraphicsCameraManager
{
	Camera* cameras;
	unsigned int numCameraSlots;
public:
	static GraphicsCameraManager globalCameraManager;

	GraphicsCameraManager();
	void initialize( unsigned int numCameraSlots = MAX_CAMERAS );
	void destroy();
	bool initialized();

	Camera* addCamera();
	void drawAllCameras();
};