#include <Graphics\GraphicsShaderManager.h>
#include <Graphics\ShaderInfo.h>
#include <GL\glew.h>
GraphicsShaderManager GraphicsShaderManager::globalShaderManager;

GraphicsShaderManager::GraphicsShaderManager(): shaderInfos(0) {}
void GraphicsShaderManager::initialize( unsigned int numShaderSlots )
{
	if ( shaderInfos ) destroy();
	this->numShaderSlots = numShaderSlots;
	shaderInfos = new ShaderInfo[numShaderSlots];
}
void GraphicsShaderManager::destroy()
{
	if ( !shaderInfos ) return;
	for ( int i = 0; i < MAX_SHADERS; i++ )
	{
		if ( glIsProgram( shaderInfos[i].programID ) ) glDeleteProgram( shaderInfos[i].programID );
	}
	delete[] shaderInfos;
	shaderInfos = 0;
	numShaderSlots = 0;
}
bool GraphicsShaderManager::initialized()
{
	return shaderInfos;
}

ShaderInfo* GraphicsShaderManager::createShaderInfo(
	const char* vertexShaderCode ,
	const char* fragmentShaderCode ,
	std::string* errorLog )
{
	ShaderInfo* shader = 0;

	for ( unsigned int i = 0; i < numShaderSlots; ++i )
	{
		if ( glIsProgram( shaderInfos[i].programID ) == GL_FALSE )
		{
			shader = &shaderInfos[i];
			break;
		}
	}
	if ( !shader ) return 0;

	unsigned int vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vertexShaderID , 1 , &vertexShaderCode , 0 );

	glShaderSource( fragmentShaderID , 1 , &fragmentShaderCode , 0 );

	glCompileShader( vertexShaderID );
	glCompileShader( fragmentShaderID );

	shader->programID = glCreateProgram();
	glAttachShader( shader->programID , vertexShaderID );
	glAttachShader( shader->programID , fragmentShaderID );

	glLinkProgram( shader->programID );

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


		glGetProgramiv( shader->programID , GL_LINK_STATUS , &compileStatus );
		if ( compileStatus != GL_TRUE )
		{
			GLint messageLen;
			glGetProgramiv( shader->programID , GL_INFO_LOG_LENGTH , &messageLen );
			char* buffer = new char[messageLen];
			GLsizei bitbucket;
			glGetProgramInfoLog( shader->programID , messageLen , &bitbucket , buffer );
			*errorLog += buffer;
			*errorLog += '\n';
			delete[] buffer;
		}
	}
	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );

	return shader;
}

void GraphicsShaderManager::updateShaderInfo(
	ShaderInfo* shaderInfoIndex ,
	const char* vertexShaderCode ,
	const char* fragmentShaderCode ,
	std::string* errorLog )
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

