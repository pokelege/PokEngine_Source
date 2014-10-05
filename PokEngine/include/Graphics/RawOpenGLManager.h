#pragma once
#pragma warning ( disable : 4201)
#pragma warning ( disable : 4127)
#pragma warning ( disable : 4251)
#include <PokEngineExportHeader.h>
#include <glm.hpp>
#include <string>
#include <Windows.h>
#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\GraphicsBufferManager.h>
class POKENGINE_SHARED RawOpenGLManager
{

public:

	struct POKENGINE_SHARED FrameBufferInfo
	{
		unsigned int bufferID;
		FrameBufferInfo() : bufferID(NULL) {}
	};

	//static Renderable renderableInfos[MAX_RENDERABLES];
	static FrameBufferInfo frameBufferInfos[MAX_TEXTURES];
	static unsigned int base;

public:
	//static void drawAll();
	//static void drawSpecific(Renderable* toDraw);
	static void updateAnimation( Renderable& toUpdate , const float& dt );
	static void updateAnimationMatricesRecurse( unsigned int boneIndex, BoneInfo* bones , Renderable& toUpdate, glm::mat4& parentMatrix );

	//static Renderable* addRenderable(
	//	GeometryInfo* whatGeometry ,
	//	const char* whereUniform ,
	//	ShaderInfo* howShaders ,
	//	const bool& depthTestEnabled = false,
	//	TextureInfo* whatTexture = NULL,
	//	CullingType culling = CullingType::CT_NONE,
	//	const bool& alpha = false);

	static FrameBufferInfo* addFrameBuffer();
	static void setFrameBuffer( FrameBufferInfo* bufferID, unsigned int textureID, unsigned int attatchment );
};