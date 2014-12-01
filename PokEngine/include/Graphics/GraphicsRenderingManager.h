#pragma once
#include <PokEngineExportHeader.h>

#define MAX_RENDERABLES 100
struct RenderableInfo;
class Camera;
class POKENGINE_SHARED GraphicsRenderingManager
{
	RenderableInfo* renderables;
	unsigned int numRenderableSlots;
public:
	static GraphicsRenderingManager globalRenderingManager;

	GraphicsRenderingManager();
	void initialize( unsigned int numRenderableSlots = MAX_RENDERABLES );
	void destroy();
	bool initialized();

	RenderableInfo* addRenderable();
	void drawAll(const Camera& camera, const bool& isFrameBuffer = false);
};