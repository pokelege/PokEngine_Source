#pragma once
#ifdef DEBUGAI
#include <PokEngineDebugExportHeader.h>
#include <glm.hpp>
class Camera;
class AI;
class POKENGINEDEBUGTOOLS_SHARED DebugAI
{
	static AI* linkedAI;
	static Camera* linkedCamera;
	static bool follow;
	static bool drawnodes;
	static bool drawpath;
	static bool enableForcedTargetChange;
public:
	static void linkAI( AI* link, float minSpeed, float maxSpeed );
	static void linkCamera( Camera* camera );
	static void delinkAI();
	static void delinkCamera();
	static void moveCamToAI();
	static void drawNodes();
	static void forceTargetChange( glm::vec3& ray);
};

#define DebugAILinkAI(ai, min, max) DebugAI::linkAI(ai, min, max)
#define DebugAILinkCamera(cam) DebugAI::linkCamera(cam)
#define DebugAIDelinkCamera DebugAI::delinkCamera()
#define DebugAIDelinkAI DebugAI::delinkAI()
#define DebugAIMoveCamToAI DebugAI::moveCamToAI()
#define DebugAIDrawNodes DebugAI::drawNodes()
#define DebugAIForceTargetChange(ray) DebugAI::forceTargetChange(ray)
#else
#define DebugAILinkAI(ai, min, max)
#define DebugAILinkCamera(cam)
#define DebugAIDelinkCamera
#define DebugAIDelinkAI
#define DebugAIMoveCamToAI
#define DebugAIDrawNodes
#define DebugAIForceTargetChange(ray)
#endif