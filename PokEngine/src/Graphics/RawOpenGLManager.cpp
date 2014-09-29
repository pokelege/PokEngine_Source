#include "Graphics\RawOpenGLManager.h"
#include <GL\glew.h>
#include <SOIL.h>
#include <fstream>
#include <gtx\transform.hpp>
#include <Graphics\VertexInfo.h>
RawOpenGLManager::BufferInfo RawOpenGLManager::bufferIds[MAX_BUFFERS];
RawOpenGLManager::GeometryInfo RawOpenGLManager::geometryInfos[MAX_GEOMETRIES];
RawOpenGLManager::ShaderInfo RawOpenGLManager::shaderInfos[MAX_SHADERS];
RawOpenGLManager::TextureInfo RawOpenGLManager::textureInfos[MAX_TEXTURES];
RawOpenGLManager::UniformInfo RawOpenGLManager::globalUniforms[MAX_UNIFORM_PARAMETERS];
RawOpenGLManager::Renderable RawOpenGLManager::renderableInfos[MAX_RENDERABLES];
RawOpenGLManager::FrameBufferInfo RawOpenGLManager::frameBufferInfos[MAX_TEXTURES];

unsigned int RawOpenGLManager::base = 0;

void RawOpenGLManager::initialize()
{
	glewInit();
}

RawOpenGLManager::ShaderInfo* RawOpenGLManager::createShaderInfo(
	const char* vertexShaderCode ,
	const char* fragmentShaderCode,
	std::string* errorLog)
{
	unsigned int vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vertexShaderID , 1 , &vertexShaderCode , 0 );

	glShaderSource( fragmentShaderID , 1 , &fragmentShaderCode , 0 );

	glCompileShader( vertexShaderID );
	glCompileShader( fragmentShaderID );

	int i;

	for ( i = 0; i < MAX_SHADERS; i++ )
	{
		if ( glIsProgram( shaderInfos[i].programID ) == GL_FALSE ) break;
	}

	shaderInfos[i].programID = glCreateProgram();
	glAttachShader( shaderInfos[i].programID , vertexShaderID );
	glAttachShader( shaderInfos[i].programID , fragmentShaderID );

	glLinkProgram( shaderInfos[i].programID );

	if ( errorLog != nullptr )
	{
		GLint compileStatus;
		glGetShaderiv( vertexShaderID , GL_COMPILE_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetShaderiv( vertexShaderID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetShaderInfoLog( vertexShaderID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}

		glGetShaderiv( fragmentShaderID , GL_COMPILE_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetShaderiv( fragmentShaderID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetShaderInfoLog( fragmentShaderID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}


		glGetProgramiv( shaderInfos[i].programID , GL_LINK_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetProgramiv( shaderInfos[i].programID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetProgramInfoLog( shaderInfos[i].programID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}
	}
	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );

	return &shaderInfos[i];
}

void RawOpenGLManager::updateShaderInfo(
	ShaderInfo* shaderInfoIndex ,
	const char* vertexShaderCode ,
	const char* fragmentShaderCode,
	std::string* errorLog)
{
	unsigned int vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vertexShaderID , 1 , &vertexShaderCode , 0 );

	glShaderSource( fragmentShaderID , 1 , &fragmentShaderCode , 0 );

	glCompileShader( vertexShaderID );
	glCompileShader( fragmentShaderID );

	glDeleteProgram( shaderInfoIndex->programID );
	shaderInfoIndex->programID = glCreateProgram();
	glAttachShader( shaderInfoIndex->programID , vertexShaderID );
	glAttachShader( shaderInfoIndex->programID , fragmentShaderID );

	glLinkProgram( shaderInfoIndex->programID );

	if ( errorLog != nullptr )
	{
		GLint compileStatus;
		glGetShaderiv( vertexShaderID , GL_COMPILE_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetShaderiv( vertexShaderID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetShaderInfoLog( vertexShaderID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}

		glGetShaderiv( fragmentShaderID , GL_COMPILE_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetShaderiv( fragmentShaderID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetShaderInfoLog( fragmentShaderID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}


		glGetProgramiv( shaderInfoIndex->programID , GL_LINK_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetProgramiv( shaderInfoIndex->programID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetProgramInfoLog( shaderInfoIndex->programID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}
	}


	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );
}

RawOpenGLManager::TextureInfo* RawOpenGLManager::addTexture( const char ** files , unsigned int number, unsigned int wrap )
{
	TextureInfo::SubTextureInfo* textureIDs = new TextureInfo::SubTextureInfo[number];

	for ( unsigned int i = 0; i < number; i++ )
	{
		if ( strlen( files[i] ) == 0 ) textureIDs[i].textureID = NULL;
		else
		{
			//glActiveTexture( GL_TEXTURE0 + i );
			textureIDs[i].textureID = SOIL_load_OGL_texture( files[i] , SOIL_LOAD_RGBA , SOIL_CREATE_NEW_ID , SOIL_FLAG_INVERT_Y );

			glBindTexture( GL_TEXTURE_2D , textureIDs[i].textureID );
			glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
			glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
		}
		textureIDs[i].type = GL_TEXTURE_2D;
	}

	int i;
	for ( i = 0; i < MAX_TEXTURES; i++ )
	{
		if ( textureInfos[i].textureIDs == NULL )
		{
			textureInfos[i].textureIDs = textureIDs;
			textureInfos[i].size = number;
			break;
		}
	}

	return &textureInfos[i];
}

void RawOpenGLManager::addCubeTexture( TextureInfo* theTexture , const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z , unsigned int index , unsigned int wrap )
{
	if ( index >= theTexture->size )
	{
		index = theTexture->size;
		TextureInfo::SubTextureInfo* textureIDs = new TextureInfo::SubTextureInfo[theTexture->size + 1];
		memcpy( textureIDs , theTexture->textureIDs , sizeof( TextureInfo::SubTextureInfo ) *  theTexture->size );
		++theTexture->size;
		delete[] theTexture->textureIDs;
		theTexture->textureIDs = textureIDs;
	}


	//glActiveTexture( GL_TEXTURE0 + index );

	if ( glIsTexture( theTexture->textureIDs[index].textureID ) ) glDeleteTextures( 1 , &theTexture->textureIDs[index].textureID );

	theTexture->textureIDs[index].textureID = SOIL_load_OGL_cubemap( X , x , Y , y , Z , z , SOIL_LOAD_RGBA , SOIL_CREATE_NEW_ID , 0 );

	glBindTexture( GL_TEXTURE_CUBE_MAP , theTexture->textureIDs[index].textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	theTexture->textureIDs[index].type = GL_TEXTURE_CUBE_MAP;
}

void RawOpenGLManager::editTexture( TextureInfo* theTexture , const char* file , unsigned int index , unsigned int wrap )
{
	if ( index >= theTexture->size )
	{
		index = theTexture->size;
		TextureInfo::SubTextureInfo* textureIDs = new TextureInfo::SubTextureInfo[theTexture->size + 1];
		memcpy( textureIDs , theTexture->textureIDs , sizeof( TextureInfo::SubTextureInfo ) *  theTexture->size );
		++theTexture->size;
		delete[] theTexture->textureIDs;
		theTexture->textureIDs = textureIDs;
	}


	//glActiveTexture( GL_TEXTURE0 + index );

	if ( glIsTexture( theTexture->textureIDs[index].textureID ) ) glDeleteTextures( 1 , &theTexture->textureIDs[index].textureID );

	theTexture->textureIDs[index].textureID = SOIL_load_OGL_texture( file , SOIL_LOAD_RGBA , SOIL_CREATE_NEW_ID , SOIL_FLAG_INVERT_Y );

	glBindTexture( GL_TEXTURE_2D , theTexture->textureIDs[index].textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	theTexture->textureIDs[index].type = GL_TEXTURE_2D;
}

void RawOpenGLManager::editTexture( TextureInfo* theTexture , const char* data , unsigned int width , unsigned int height , unsigned int index , unsigned int inputFormat , unsigned int outputFormat , unsigned int dataType , unsigned int wrap )
{
	if ( index >= theTexture->size )
	{
		index = theTexture->size;
		TextureInfo::SubTextureInfo* textureIDs = new TextureInfo::SubTextureInfo[theTexture->size + 1];
		memcpy( textureIDs , theTexture->textureIDs , sizeof( TextureInfo::SubTextureInfo ) *  theTexture->size );
		++theTexture->size;
		delete[] theTexture->textureIDs;
		theTexture->textureIDs = textureIDs;
	}


	//glActiveTexture( GL_TEXTURE0 + index );
	if ( glIsTexture( theTexture->textureIDs[index].textureID ) ) glDeleteTextures( 1 , &theTexture->textureIDs[index].textureID );
	glGenTextures( 1 , &theTexture->textureIDs[index].textureID );
	glBindTexture( GL_TEXTURE_2D , theTexture->textureIDs[index].textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	glTexImage2D( GL_TEXTURE_2D , 0 , inputFormat , width , height , 0 , outputFormat , dataType , data );
	theTexture->textureIDs[index].type = GL_TEXTURE_2D;
}

RawOpenGLManager::FrameBufferInfo* RawOpenGLManager::addFrameBuffer()
{
	int i;
	for ( i = 0; i < MAX_TEXTURES; i++ )
	{
		if ( frameBufferInfos[i].bufferID == NULL )
		{
			glGenFramebuffers( 1 , &frameBufferInfos[i].bufferID );
			break;
		}
	}
	return &frameBufferInfos[i];
}

void RawOpenGLManager::setFrameBuffer( FrameBufferInfo* bufferID , unsigned int textureID , unsigned int attatchment )
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , bufferID->bufferID );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER , attatchment , GL_TEXTURE_2D , textureID , 0 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , 0);
}

RawOpenGLManager::GeometryInfo* RawOpenGLManager::addFileGeometry( const char* filename )
{
	std::ifstream stream( filename , std::ios::ios_base::binary | std::ios::ios_base::in );
	PokEngineModelDataMap data( stream );
	unsigned int numVertices;
	unsigned int numIndices;
	VertexInfo* verts = data.getVertexData( &numVertices );
	unsigned short* indices = data.getIndexData( &numIndices );
	unsigned int dataSize = (sizeof(VertexInfo) * numVertices) + ( sizeof( unsigned short ) * numIndices );

	int i;

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
	geometryInfos[j].indexingMode = GL_TRIANGLES;

	glGenVertexArrays( 1 , &geometryInfos[j].dataArray );
	glBindVertexArray( geometryInfos[j].dataArray );

	geometryInfos[j].vertexOffset = bufferIds[i].offsetAddress;
	geometryInfos[j].numVertex = numVertices;

	geometryInfos[j].indexOffset = bufferIds[i].offsetAddress + ( sizeof( VertexInfo )  * numVertices );
	geometryInfos[j].numIndex = numIndices;
	geometryInfos[j].modelData = data;
	/*glBufferSubData( GL_ARRAY_BUFFER , bufferIds[i].offsetAddress , dataSize , ( GLvoid* ) ( ( int* ) rawData + 2 ) );*/

	glBufferSubData( GL_ARRAY_BUFFER , bufferIds[i].offsetAddress , ( sizeof( VertexInfo ) * numVertices ) , ( GLvoid* ) ( verts ) );

	glBufferSubData( GL_ARRAY_BUFFER , bufferIds[i].offsetAddress + ( sizeof( VertexInfo ) * numVertices ) , ( sizeof( unsigned short ) * numIndices ) , ( GLvoid* ) ( indices ) );

	bufferIds[i].offsetAddress += dataSize;
	bufferIds[i].bufferSpace -= dataSize;
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferIds[i].bufferID );

	return &geometryInfos[j];

	//
	//std::string buffer( ( std::istreambuf_iterator<char>( stream ) ) ,
	//					std::istreambuf_iterator<char>() );
	//return addRawGeometry( buffer.c_str() );
}

RawOpenGLManager::GeometryInfo* RawOpenGLManager::addRawGeometry( const char* rawData )
{
	int numVertices = *( int* ) rawData;
	int numIndices = *( ( int* ) rawData + 1 );

	unsigned int dataSize = ( ( sizeof( float ) * 14 ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );

	int i;

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
	geometryInfos[j].indexingMode = GL_TRIANGLES;

	glGenVertexArrays( 1 , &geometryInfos[j].dataArray );
	glBindVertexArray( geometryInfos[j].dataArray );

	geometryInfos[j].vertexOffset = bufferIds[i].offsetAddress;
	geometryInfos[j].numVertex = numVertices;

	geometryInfos[j].indexOffset = bufferIds[i].offsetAddress + ( ( sizeof( float ) * 14 ) * numVertices );
	geometryInfos[j].numIndex = numIndices;

	glBufferSubData( GL_ARRAY_BUFFER , bufferIds[i].offsetAddress , dataSize , ( GLvoid* ) ( ( int* ) rawData + 2 ) );

	bufferIds[i].offsetAddress += dataSize;
	bufferIds[i].bufferSpace -= dataSize;

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferIds[i].bufferID );

	return &geometryInfos[j];
}

RawOpenGLManager::Renderable* RawOpenGLManager::addRenderable(
	GeometryInfo* whatGeometry ,
	const char* whereUniform ,
	ShaderInfo* howShaders ,
	const bool& depthTestEnabled,
	TextureInfo* whatTexture,
	CullingType culling,
	const bool& alpha )
{
	int i;

	for ( i = 0; i < MAX_RENDERABLES; i++ )
	{
		if ( renderableInfos[i].whatGeometryIndex == nullptr ) break;
	}

	renderableInfos[i].whatGeometryIndex = whatGeometry;
	renderableInfos[i].howShaderIndex = howShaders;
	renderableInfos[i].textureID = whatTexture;
	renderableInfos[i].visible = true;
	renderableInfos[i].depthTestEnabled = depthTestEnabled;
	renderableInfos[i].whereUniform = std::string( whereUniform );
	renderableInfos[i].culling = culling;
	renderableInfos[i].alpha = alpha;
	return &renderableInfos[i];
}

void RawOpenGLManager::addShaderStreamedParameter(
	GeometryInfo* geometryID ,
	unsigned int layoutLocation ,
	ParameterType parameterType ,
	unsigned int bufferStride ,
	unsigned int bufferOffset )
{
	glBindVertexArray( geometryID->dataArray );

	glEnableVertexAttribArray( layoutLocation );

	if ( parameterType == PT_INT )
	{
		glVertexAttribPointer( layoutLocation , 1 , GL_INT , GL_FALSE , bufferStride , ( void* ) ( geometryID->vertexOffset + bufferOffset ) );
	}
	else
	{
		glVertexAttribPointer( layoutLocation , parameterType / sizeof( float ) , GL_FLOAT , GL_FALSE , bufferStride , ( void* ) ( geometryID->vertexOffset + bufferOffset ) );
	}
}

void RawOpenGLManager::setUniformParameter(
	ShaderInfo* shader ,
	const char* name ,
	ParameterType parameterType ,
	const float* value ,
	unsigned int size )
{
	GLint uniformID = glGetUniformLocation( shader->programID , name );
	switch ( parameterType )
	{
		case( PT_INT ) : glUniform1iv( uniformID , size , reinterpret_cast<const int*>(value)); break;
		case ( PT_FLOAT ) : glUniform1fv( uniformID , size , value );
			break;
		case ( PT_VEC2 ) : glUniform2fv( uniformID , size , value );
			break;
		case( PT_VEC3 ) : glUniform3fv( uniformID , size , value );
			break;
		case( PT_VEC4 ) : glUniform4fv( uniformID , size , value );
			break;
		case( PT_MAT3 ) : glUniformMatrix3fv( uniformID , size , GL_FALSE , value );
			break;
		case( PT_MAT4 ) : glUniformMatrix4fv( uniformID , size , GL_FALSE , value );
			break;
	}
}

void RawOpenGLManager::setRenderableUniform(
	Renderable* object ,
	const char* name ,
	ParameterType parameterType ,
	const float* dataPointer )
{
	if ( object != nullptr )
	{
		int i = 0;

		while ( i < MAX_UNIFORM_PARAMETERS && object->uniforms[i].uniformName.compare( name ) && object->uniforms[i].location != nullptr )
		{
			i++;
		}

		UniformInfo uni;
		uni.uniformName = name;
		uni.type = parameterType;
		uni.location = dataPointer;

		object->uniforms[i] = uni;
	}
}

RawOpenGLManager::UniformInfo* RawOpenGLManager::setGlobalUniform(
	const char* name ,
	ParameterType parameterType ,
	const float* dataPointer )
{
	int i = 0;

	while ( i < MAX_UNIFORM_PARAMETERS && globalUniforms[i].uniformName.compare( name ) && globalUniforms[i].location != nullptr )
	{
		i++;
	}

	UniformInfo uni;
	uni.uniformName = name;
	uni.type = parameterType;
	uni.location = dataPointer;

	globalUniforms[i] = uni;
	return &globalUniforms[i];
}

void RawOpenGLManager::setViewPort( int x , int y , int width , int height )
{
	glViewport( x , y , width , height );
}

void RawOpenGLManager::clear( unsigned int toClear )
{
	glClear( toClear );
}

void RawOpenGLManager::drawSpecific( Renderable* toDraw )
{
	if ( toDraw != nullptr )
	{
		if ( toDraw->visible )
		{
			glUseProgram( toDraw->howShaderIndex->programID );
			glBindVertexArray( toDraw->whatGeometryIndex->dataArray );

			if ( toDraw->depthTestEnabled ) glEnable( GL_DEPTH_TEST );
			else glDisable( GL_DEPTH_TEST );

			if ( toDraw->alpha )
			{
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
			}
			else
			{
				glDisable(GL_BLEND);
			}

			if ( toDraw->culling == CT_NONE ) glDisable( GL_CULL_FACE );
			else
			{
				glEnable( GL_CULL_FACE );
				if ( toDraw->culling == CT_FRONT ) glCullFace( GL_FRONT );
				else if ( toDraw->culling == CT_BOTH ) glCullFace( GL_FRONT_AND_BACK );
				else glCullFace( GL_BACK );
			}

			if ( toDraw->textureID != NULL )
			{
				for ( unsigned int i = 0; i < toDraw->textureID->size; i++ )
				{
					glActiveTexture( GL_TEXTURE0 + i );
					glBindTexture( toDraw->textureID->textureIDs[i].type , toDraw->textureID->textureIDs[i].textureID );
				}
			}

			for ( int j = 0; globalUniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( toDraw->howShaderIndex , globalUniforms[j].uniformName.c_str() , globalUniforms[j].type , globalUniforms[j].location );
			}

			for ( int j = 0; toDraw->uniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( toDraw->howShaderIndex , toDraw->uniforms[j].uniformName.c_str() , toDraw->uniforms[j].type , toDraw->uniforms[j].location );
			}

			const char* st = toDraw->whereUniform.c_str();
			glm::mat4 transform = glm::translate( toDraw->translate ) * glm::rotate( toDraw->rotate.x , glm::vec3( 1 , 0 , 0 ) ) * glm::rotate( toDraw->rotate.y , glm::vec3( 0 , 1 , 0 ) )* glm::rotate( toDraw->rotate.z , glm::vec3( 0 , 0 , 1 ) ) * glm::scale( toDraw->scale );
			setUniformParameter( toDraw->howShaderIndex , st , ParameterType::PT_MAT4 , &transform[0][0] );
			if ( toDraw->animationMatrices )
			{
				setUniformParameter( toDraw->howShaderIndex ,
									 toDraw->animationMatricesUniform.c_str() ,
									 PT_MAT4 ,
									 reinterpret_cast<const float*>(toDraw->animationMatrices),
									 toDraw->sizeofAnimationMatrices);
				unsigned int sizeofBlendingIndexData;
				unsigned int* blendingIndex = toDraw->whatGeometryIndex->modelData.getBlendingIndexData( &sizeofBlendingIndexData );
				setUniformParameter( toDraw->howShaderIndex ,
									 toDraw->animationIndexUniform.c_str() ,
									 PT_INT ,
									 reinterpret_cast< float* >( blendingIndex ) ,
									 sizeofBlendingIndexData );
				unsigned int sizeofBlendingWeightData;
				float* blendingWeight = toDraw->whatGeometryIndex->modelData.getBlendingWeightData( &sizeofBlendingWeightData );
				setUniformParameter( toDraw->howShaderIndex ,
									 toDraw->animationWeightUniform.c_str() ,
									 PT_FLOAT ,
									 blendingWeight ,
									 sizeofBlendingWeightData );
			}
			glDrawElements( toDraw->whatGeometryIndex->indexingMode , toDraw->whatGeometryIndex->numIndex , GL_UNSIGNED_SHORT , ( void* ) toDraw->whatGeometryIndex->indexOffset );
		}
	}
}


void RawOpenGLManager::enable( unsigned int toEnable)
{
	glEnable( toEnable );
}

void RawOpenGLManager::drawAll()
{
	for ( int i = 0; i < MAX_RENDERABLES && renderableInfos[i].whatGeometryIndex != nullptr; i++ )
	{
		if ( renderableInfos[i].visible )
		{
			glUseProgram( renderableInfos[i].howShaderIndex->programID );
			glBindVertexArray( renderableInfos[i].whatGeometryIndex->dataArray );

			if ( renderableInfos[i].depthTestEnabled ) glEnable( GL_DEPTH_TEST );
			else glDisable( GL_DEPTH_TEST );

			if ( renderableInfos[i].alpha )
			{
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
			}
			else
			{
				glDisable( GL_BLEND );
			}

			if ( renderableInfos[i].culling == CT_NONE ) glDisable( GL_CULL_FACE );
			else
			{
				glEnable( GL_CULL_FACE );
				if ( renderableInfos[i].culling == CT_FRONT ) glCullFace( GL_FRONT );
				else if ( renderableInfos[i].culling == CT_BOTH ) glCullFace( GL_FRONT_AND_BACK );
				else glCullFace( GL_BACK );
			}

			if ( renderableInfos[i].textureID != NULL )
			{
				for ( unsigned int j = 0; j < renderableInfos[i].textureID->size; j++ )
				{
					glActiveTexture( GL_TEXTURE0 + j );
					glBindTexture( renderableInfos[i].textureID->textureIDs[j].type, renderableInfos[i].textureID->textureIDs[j].textureID );
				}
			}

			for ( int j = 0; globalUniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( renderableInfos[i].howShaderIndex , globalUniforms[j].uniformName.c_str() , globalUniforms[j].type , globalUniforms[j].location );
			}

			for ( int j = 0; renderableInfos[i].uniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( renderableInfos[i].howShaderIndex , renderableInfos[i].uniforms[j].uniformName.c_str() , renderableInfos[i].uniforms[j].type , renderableInfos[i].uniforms[j].location );
			}

			const char* st = renderableInfos[i].whereUniform.c_str();
			glm::mat4 transform = glm::translate( renderableInfos[i].translate ) * glm::rotate( renderableInfos[i].rotate.x , glm::vec3( 1 , 0 , 0 ) ) * glm::rotate( renderableInfos[i].rotate.y , glm::vec3( 0 , 1 , 0 ) )* glm::rotate( renderableInfos[i].rotate.z , glm::vec3( 0 , 0 , 1 ) ) * glm::scale( renderableInfos[i].scale );
			setUniformParameter( renderableInfos[i].howShaderIndex , st , ParameterType::PT_MAT4 , &transform[0][0] );

			if ( renderableInfos[i].animationMatrices )
			{
				setUniformParameter( renderableInfos[i].howShaderIndex ,
									 renderableInfos[i].animationMatricesUniform.c_str() ,
									 PT_MAT4 ,
									 reinterpret_cast<const float*>( renderableInfos[i].animationMatrices ) ,
									 renderableInfos[i].sizeofAnimationMatrices );
				unsigned int sizeofBlendingIndexData;
				unsigned int* blendingIndex = renderableInfos[i].whatGeometryIndex->modelData.getBlendingIndexData( &sizeofBlendingIndexData );
				setUniformParameter( renderableInfos[i].howShaderIndex ,
									 renderableInfos[i].animationIndexUniform.c_str() ,
									 PT_INT ,
									 reinterpret_cast< float* >( blendingIndex ) ,
									 sizeofBlendingIndexData );
				unsigned int sizeofBlendingWeightData;
				float* blendingWeight = renderableInfos[i].whatGeometryIndex->modelData.getBlendingWeightData( &sizeofBlendingWeightData );
				setUniformParameter( renderableInfos[i].howShaderIndex ,
									 renderableInfos[i].animationWeightUniform.c_str() ,
									 PT_FLOAT ,
									 blendingWeight ,
									 sizeofBlendingWeightData );
			}

			glDrawElements( renderableInfos[i].whatGeometryIndex->indexingMode , renderableInfos[i].whatGeometryIndex->numIndex , GL_UNSIGNED_SHORT , ( void* ) renderableInfos[i].whatGeometryIndex->indexOffset );
		}
	}
}

std::string RawOpenGLManager::saveGeometry( GeometryInfo* geo )
{
	std::string toSend( reinterpret_cast< const char* >( &geo->numVertex ) , sizeof( geo->numVertex ) );
	toSend += std::string( reinterpret_cast< const char* >( &geo->numIndex) , sizeof( geo->numIndex ) );
	glBindVertexArray( geo->dataArray );
	glBindBuffer( GL_ARRAY_BUFFER , geo->buffer->bufferID );
	unsigned int dataSize = ( ( sizeof( float ) * 8 ) * geo->numVertex ) + ( sizeof( unsigned short ) * geo->numIndex );
	char* pointer = new char[dataSize];
	glGetBufferSubData( GL_ARRAY_BUFFER , geo->vertexOffset , dataSize , pointer );
	std::string test( pointer , dataSize );
	toSend += test;
	delete[] pointer;
	return toSend;
}

void RawOpenGLManager::reset()
{
	for ( int i = 0; i < MAX_BUFFERS; i++ )
	{
		if ( glIsBuffer( bufferIds[i].bufferID ) ) glDeleteBuffers( 1 , &bufferIds[i].bufferID );
	}

	for ( int i = 0; i < MAX_GEOMETRIES; i++ )
	{
		if ( glIsVertexArray( geometryInfos[i].dataArray ) ) glDeleteVertexArrays( 1 , &geometryInfos[i].dataArray );
	}

	for ( int i = 0; i < MAX_SHADERS; i++ )
	{
		if ( glIsProgram( shaderInfos[i].programID ) ) glDeleteProgram( shaderInfos[i].programID );
	}

	for ( int i = 0; i < MAX_TEXTURES; i++ )
	{
		for ( unsigned int j = 0; j < textureInfos[j].size; j++ )
		{
			if ( glIsTexture( textureInfos[i].textureIDs[j].textureID ) ) glDeleteTextures( 1 , &textureInfos[i].textureIDs[j].textureID );
		}
	}
	for ( int i = 0; i < MAX_UNIFORM_PARAMETERS; i++ ) globalUniforms[i].location = nullptr;

	for ( int i = 0; i < MAX_RENDERABLES; i++ )
	{
		renderableInfos[i].whatGeometryIndex = nullptr;
		renderableInfos[i].depthTestEnabled = false;
		for ( int j = 0; j < MAX_UNIFORM_PARAMETERS; j++ ) renderableInfos[i].uniforms[j].location = nullptr;
	}
}

void RawOpenGLManager::initializePrinting( HDC hdc, int sizex, int sizey, int fontWeight )
{
	if ( glIsList( base ) ) glDeleteLists( base , 96 );
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists( 96 );								// Storage For 96 Characters

	font = CreateFont( -sizex ,							// Height Of Font
					   sizey ,								// Width Of Font
					   0 ,								// Angle Of Escapement
					   0 ,								// Orientation Angle
					   fontWeight ,						// Font Weight
					   FALSE ,							// Italic
					   FALSE ,							// Underline
					   FALSE ,							// Strikeout
					   ANSI_CHARSET ,					// Character Set Identifier
					   OUT_TT_PRECIS ,					// Output Precision
					   CLIP_DEFAULT_PRECIS ,			// Clipping Precision
					   ANTIALIASED_QUALITY ,			// Output Quality
					   FF_DONTCARE | DEFAULT_PITCH ,		// Family And Pitch
					   "Courier New" );					// Font Name

	oldfont = ( HFONT ) SelectObject( hdc , font );           // Selects The Font We Want
	wglUseFontBitmaps( hdc , 32 , 96 , base );				// Builds 96 Characters Starting At Character 32
	SelectObject( hdc , oldfont );							// Selects The Font We Want
	DeleteObject( font );
}

void RawOpenGLManager::printString( const char* string , const float& x , const float& y , glm::vec4 color )
{
	if ( glIsList( base ) )
	{
		glUseProgram( 0 );
		glRasterPos2f( x , y );
		glColor4f( color.x , color.y , color.z , color.w );
		glPushAttrib( GL_LIST_BIT );
		glListBase( base - 32 );
		glCallLists( strlen( string ) , GL_UNSIGNED_BYTE , string );
		glPopAttrib();
	}
}