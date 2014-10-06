#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\ParameterType.h>
#include <Core\Component.h>
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
	GameObject* parent;
public:
	ShaderInfo* shaderInfo;
	GeometryInfo* geometryInfo;
	GraphicsSharedUniformManager* sharedUniforms;
	CullingType culling;
	bool depthTestEnabled;
	bool alphaBlendingEnabled;
	bool visible;
	RenderableInfo();
	void initialize( unsigned int numUniformSlots, unsigned int numTextureSlots );
	void destroy();

	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();

	UniformInfo* setRenderableUniform(
		const char* name ,
		ParameterType parameterType ,
		const void* dataPointer,
		unsigned int size = 1 );
	bool addTexture( TextureInfo* texture );
};

typedef RenderableInfo Renderable;