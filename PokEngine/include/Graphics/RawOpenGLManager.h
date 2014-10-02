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
#define MAX_SHADERS 10
#define MAX_TEXTURES 50
#define MAX_RENDERABLES 100

#define MAX_UNIFORM_PARAMETERS 50

public:

	enum POKENGINE_SHARED ParameterType
	{
		// These values matter:
		PT_INT = ( int )sizeof( int ) * -1 ,
		PT_FLOAT = sizeof( float ) * 1 ,
		PT_IVEC2 = ( int )sizeof( int ) * -2 ,
		PT_VEC2 = sizeof( float ) * 2 ,
		PT_IVEC3 = ( int )sizeof( int ) * -3 ,
		PT_VEC3 = sizeof( float ) * 3 ,
		PT_IVEC4 = ( int )sizeof( int ) * -4 ,
		PT_VEC4 = sizeof( float ) * 4 ,
		//PT_IMAT3 = -sizeof(int) * 9,
		PT_MAT3 = sizeof( float ) * 9 ,
		//PT_IMAT4 = -sizeof(int) * 16,
		PT_MAT4 = sizeof( float ) * 16 ,
	};

	enum POKENGINE_SHARED CullingType
	{
		CT_NONE = 0,
		CT_BACK = 1,
		CT_FRONT = 2,
		CT_BOTH = 3
	};

	struct POKENGINE_SHARED ShaderInfo
	{
		unsigned int programID;
	};

	struct POKENGINE_SHARED TextureInfo
	{
		friend class RawOpenGLManager;
	private:
		struct SubTextureInfo
		{
			unsigned int textureID;
			unsigned int type;
			SubTextureInfo() : textureID( NULL ) , type( 0 ) {}
		};
	public:
		SubTextureInfo* textureIDs;
		unsigned int size;
		TextureInfo() : textureIDs(NULL) , size(0){}
		~TextureInfo() { delete[] textureIDs; }
	};

	struct POKENGINE_SHARED FrameBufferInfo
	{
		unsigned int bufferID;
		FrameBufferInfo() : bufferID(NULL) {}
	};

	struct POKENGINE_SHARED UniformInfo
	{
		std::string uniformName;
		ParameterType type;
		const float* location;
		UniformInfo() :location( nullptr ) {}
	};

	struct POKENGINE_SHARED Renderable
	{
		GeometryInfo* whatGeometryIndex;
		ShaderInfo* howShaderIndex;
		glm::vec3 translate , rotate , scale;
		std::string whereUniform;
		std::string animationMatricesUniform;
		glm::mat4* animationMatrices;
		unsigned int sizeofAnimationMatrices;
		bool visible;
		bool depthTestEnabled;
		bool alpha;
		CullingType culling;
		TextureInfo* textureID;
		unsigned int animationFrameRate;
		float currentFrame;
		UniformInfo uniforms[MAX_UNIFORM_PARAMETERS];
		Renderable() : whatGeometryIndex( nullptr ), depthTestEnabled(false), alpha(false), scale(glm::vec3(1.0f,1.0f,1.0f)), animationMatrices(0) {}
	};

	static ShaderInfo shaderInfos[MAX_SHADERS];
	static TextureInfo textureInfos[MAX_TEXTURES];
	static UniformInfo globalUniforms[MAX_UNIFORM_PARAMETERS];
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

	static ShaderInfo* createShaderInfo(
		const char* vertexShaderCode ,
		const char* fragmentShaderCode,
		std::string* errorLog = nullptr);

	static void updateShaderInfo(
		ShaderInfo* shaderInfoIndex ,
		const char* vertexShaderCode ,
		const char* fragmentShaderCode,
		std::string* errorLog = nullptr);

	static TextureInfo* addTexture( const char ** files , unsigned int number , unsigned int wrap = 0x2901 );
	static void addCubeTexture( TextureInfo* theTexture , const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z , unsigned int index , unsigned int wrap = 0x812F );
	static void editTexture( TextureInfo* theTexture , const char* file , unsigned int index , unsigned int wrap = 0x2901 );
	static void editTexture( TextureInfo* theTexture , const char* data , unsigned int width , unsigned int height , unsigned int index , unsigned int inputFormat = 0x1908 , unsigned int outputFormat = 0x1908 , unsigned int dataType = 0x1401 , unsigned int wrap = 0x2901 );
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

	static void setUniformParameter(
		ShaderInfo* shader ,
		const char* name ,
		ParameterType parameterType ,
		const float* value,
		unsigned int size = 1);

	static void setRenderableUniform(
		Renderable* object ,
		const char* name ,
		ParameterType parameterType ,
		const float* dataPointer );

	static UniformInfo* setGlobalUniform(
		const char* name ,
		ParameterType parameterType ,
		const float* dataPointer );

	static FrameBufferInfo* addFrameBuffer();
	static void setFrameBuffer( FrameBufferInfo* bufferID, unsigned int textureID, unsigned int attatchment );
};