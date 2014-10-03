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
#define MAX_RENDERABLES 100

public:

	struct POKENGINE_SHARED FrameBufferInfo
	{
		unsigned int bufferID;
		FrameBufferInfo() : bufferID(NULL) {}
	};

	static Renderable renderableInfos[MAX_RENDERABLES];
	static FrameBufferInfo frameBufferInfos[MAX_TEXTURES];
	static unsigned int base;

public:
	static void printString( const char* string , const float& x = -1 , const float& y = 0, glm::vec4 color = glm::vec4(1,1,1,1) );
	static void initializePrinting( HDC hdc , int sizex = 24 , int sizey = 0 , int fontWeight = FW_BOLD );
	static void initialize();
	static void setViewPort( int x , int y , int width , int height );
	static void clear( unsigned int toClear);
	static void enable( unsigned int toEnable );
	static void drawAll();
	static void drawSpecific(Renderable* toDraw);
	static void updateAnimation( Renderable& toUpdate , const float& dt );
	static void updateAnimationMatricesRecurse( unsigned int boneIndex, BoneInfo* bones , Renderable& toUpdate, glm::mat4& parentMatrix );
	static void reset();

	static Renderable* addRenderable(
		GeometryInfo* whatGeometry ,
		const char* whereUniform ,
		ShaderInfo* howShaders ,
		const bool& depthTestEnabled = false,
		TextureInfo* whatTexture = NULL,
		CullingType culling = CullingType::CT_NONE,
		const bool& alpha = false);

	static void addShaderStreamedParameter(
		GeometryInfo* geometryID ,
		unsigned int layoutLocation ,
		ParameterType parameterType ,
		unsigned int bufferStride ,
		unsigned int bufferOffset );

	static void setRenderableUniform(
		Renderable* object ,
		const char* name ,
		ParameterType parameterType ,
		const float* dataPointer );

	static FrameBufferInfo* addFrameBuffer();
	static void setFrameBuffer( FrameBufferInfo* bufferID, unsigned int textureID, unsigned int attatchment );
};