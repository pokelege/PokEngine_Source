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
class GraphicsSharedUniformManager;
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
	bool visible;
	RenderableInfo();
	void initialize( unsigned int numUniformSlots, unsigned int numTextureSlots );
	void destroy();

	UniformInfo* setRenderableUniform(
		const char* name ,
		ParameterType parameterType ,
		const void* dataPointer );
	bool addTexture( TextureInfo* texture );
	void draw( GraphicsSharedUniformManager* sharedUniforms = 0 );
};