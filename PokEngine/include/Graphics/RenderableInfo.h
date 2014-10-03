#pragma once
#include <PokEngineExportHeader.h>
struct UniformInfo;
struct GeometryInfo;
struct ShaderInfo;
struct TextureInfo;
enum POKENGINE_SHARED CullingType
{
	CT_NONE = 0 ,
	CT_BACK = 1 ,
	CT_FRONT = 2 ,
	CT_BOTH = 3
};
struct POKENGINE_SHARED RenderableInfo
{
private:
	UniformInfo* uniforms;
	unsigned int numUniformSlots;
	TextureInfo** textures;
	unsigned int numTextureSlots;
public:
	ShaderInfo* shaderInfo;
	GeometryInfo* geometryInfo;
	CullingType culling;
	bool depthTestEnabled;
	bool alphaBlendingEnabled;
	RenderableInfo();
	void initialize( unsigned int numUniformSlots, TextureInfo** textures = 0, unsigned int textureArraySize = 0, bool userWillMaintainTextureArrayPointer = false );
	void destroy();
	bool initialized();
};