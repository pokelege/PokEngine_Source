#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\ParameterType.h>
#include <Core\Component.h>
#include <vector>
class GraphicsLightManager;
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
struct POKENGINE_SHARED RenderableInfo : public Component
{
private:
	UniformInfo* uniforms;
	unsigned int numUniformSlots;
	TextureInfo** textures;
	unsigned int numTextureSlots;
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	GameObject* parent;
	ShaderInfo* shaderInfo;
	GeometryInfo* geometryInfo;
	GraphicsSharedUniformManager* sharedUniforms;
	CullingType culling, frameBufferCulling;
	bool depthTestEnabled;
	bool alphaBlendingEnabled;
	bool slotUsed;
	RenderableInfo();
	~RenderableInfo();
	void initialize( unsigned int numUniformSlots, unsigned int numTextureSlots );
	void destroy();

	virtual void draw(const std::vector<GraphicsLightManager*>& lights = std::vector<GraphicsLightManager*>(), const bool& isFrameBuffer =  false);

	UniformInfo* setRenderableUniform(
		const char* name ,
		ParameterType parameterType ,
		const void* dataPointer,
		unsigned int size = 1 );
	bool addTexture( TextureInfo* texture );
	bool swapTexture( TextureInfo* texture , const unsigned int& index, TextureInfo** receivingTexture = 0 );
};

typedef RenderableInfo Renderable;