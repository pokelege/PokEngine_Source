#pragma once
#pragma warning ( disable : 4201)
#pragma warning ( disable : 4127)
#pragma warning ( disable : 4251)
#include "../PokEngineExportHeader.h"
#include <glm.hpp>
#include <string>
#include <Windows.h>

class POKENGINE_SHARED RawOpenGLManager
{
#define MAX_BUFFERS 10
#define MAX_GEOMETRIES 20
#define MAX_SHADERS 10
#define MAX_TEXTURES 50
#define MAX_RENDERABLES 100

#define MAX_UNIFORM_PARAMETERS 50
#define MAX_BUFFER_SIZE sizeof(char) * (int)1.049e+7

public:

	enum POKENGINE_SHARED ParameterType
	{
		// These values matter:
		PT_INT = 1,
		PT_FLOAT = sizeof( float ) * 1 ,
		PT_VEC2 = sizeof( float ) * 2 ,
		PT_VEC3 = sizeof( float ) * 3 ,
		PT_VEC4 = sizeof( float ) * 4 ,
		PT_MAT3 = sizeof( float ) * 9 ,
		PT_MAT4 = sizeof( float ) * 16 ,
	};

	enum POKENGINE_SHARED CullingType
	{
		CT_NONE = 0,
		CT_BACK = 1,
		CT_FRONT = 2,
		CT_BOTH = 3
	};

	struct POKENGINE_SHARED BufferInfo
	{
		unsigned int bufferID;
		unsigned int bufferSpace;
		unsigned int offsetAddress;
	};

	struct POKENGINE_SHARED GeometryInfo
	{
		BufferInfo* buffer;
		unsigned int dataArray;
		unsigned int vertexOffset;
		unsigned int numVertex;
		unsigned int indexOffset;
		unsigned int numIndex;
		unsigned int indexingMode;
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
		bool visible;
		bool depthTestEnabled;
		bool alpha;
		CullingType culling;
		TextureInfo* textureID;
		UniformInfo uniforms[MAX_UNIFORM_PARAMETERS];
		Renderable() : whatGeometryIndex( nullptr ), depthTestEnabled(false), alpha(false), scale(glm::vec3(1.0f,1.0f,1.0f)) {}
	};

	static BufferInfo bufferIds[MAX_BUFFERS];
	static GeometryInfo geometryInfos[MAX_GEOMETRIES];
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
		const float* value );

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

	static GeometryInfo* addFileGeometry( const char* filename );
	static GeometryInfo* addRawGeometry( const char* rawData );
	static std::string saveGeometry( GeometryInfo* geo );

	template <class Tvert>
	static GeometryInfo* addGeometry(
		const Tvert* verts , const unsigned int& numVerts ,
		const unsigned short* indices , const unsigned int& numIndices ,
		const unsigned int& indexingMode )
	{
		unsigned int dataSize = sizeof( *verts ) * numVerts;
		dataSize += sizeof( unsigned short ) * numIndices;
		int i;
		glewInit();
		for ( i = 0; i < MAX_BUFFERS; i++ )
		{
			if ( glIsBuffer( bufferIds[i].bufferID ) == GL_FALSE )
			{
				BufferInfo newBuffer;
				glGenBuffers( 1 , &newBuffer.bufferID );
				glBindBuffer( GL_ARRAY_BUFFER , newBuffer.bufferID );

				glBufferData( GL_ARRAY_BUFFER , MAX_BUFFER_SIZE , 0 , GL_DYNAMIC_DRAW );
				newBuffer.bufferSpace = MAX_BUFFER_SIZE;
				newBuffer.offsetAddress = 0;
				bufferIds[i] = newBuffer;
				break;
			}
			else if ( bufferIds[i].bufferSpace > dataSize )
			{
				glBindBuffer( GL_ARRAY_BUFFER , bufferIds[i].bufferID );
				break;
			}
			else if ( i == MAX_BUFFERS - 1 ) throw std::exception( "No more geo space" );
		}

		int j;

		for ( j = 0; j < MAX_GEOMETRIES; j++ )
		{
			if ( glIsVertexArray( geometryInfos[j].dataArray ) == GL_FALSE )
			{
				break;
			}
		}

		geometryInfos[j].buffer = &bufferIds[i];
		geometryInfos[j].indexingMode = indexingMode;

		glGenVertexArrays( 1 , &geometryInfos[j].dataArray );
		glBindVertexArray( geometryInfos[j].dataArray );

		geometryInfos[j].vertexOffset = bufferIds[i].offsetAddress;
		geometryInfos[j].numVertex = numVerts;
		glBufferSubData( GL_ARRAY_BUFFER , bufferIds[i].offsetAddress , sizeof( *verts )* numVerts , ( GLvoid* ) verts );
		bufferIds[i].offsetAddress += sizeof( *verts )* numVerts;
		bufferIds[i].bufferSpace -= sizeof( *verts )* numVerts;

		geometryInfos[j].indexOffset = bufferIds[i].offsetAddress;
		geometryInfos[j].numIndex = numIndices;
		glBufferSubData( GL_ARRAY_BUFFER , bufferIds[i].offsetAddress , sizeof( *indices )* numIndices , ( GLvoid* ) indices );
		bufferIds[i].offsetAddress += sizeof( *indices )* numIndices;
		bufferIds[i].bufferSpace -= sizeof( *indices )* numIndices;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferIds[i].bufferID );
		return &geometryInfos[j];
	}
};