#include "DebugDrawManager.h"
#include <GL\glew.h>
#include <string>
#include <fstream>
#include <gtx\transform.hpp>
DebugDrawManager::DebugShape** DebugDrawManager::shapes = nullptr;
unsigned int DebugDrawManager::currentArraySize = 0;
unsigned int DebugDrawManager::physicalArraySize = 0;

unsigned int DebugDrawManager::bufferID = NULL;

unsigned int DebugDrawManager::cubeID = NULL;
unsigned int DebugDrawManager::cubeIndex = NULL;
unsigned int DebugDrawManager::cubeNumIndices = NULL;

unsigned int DebugDrawManager::sphereID = NULL;
unsigned int DebugDrawManager::sphereIndex = NULL;
unsigned int DebugDrawManager::sphereNumIndices = NULL;

unsigned int DebugDrawManager::cylID = NULL;
unsigned int DebugDrawManager::cylIndex = NULL;
unsigned int DebugDrawManager::cylNumIndices = NULL;

unsigned int DebugDrawManager::coneID = NULL;
unsigned int DebugDrawManager::coneIndex = NULL;
unsigned int DebugDrawManager::coneNumIndices = NULL;

unsigned int DebugDrawManager::lineID = NULL;
unsigned int DebugDrawManager::lineIndex = NULL;
unsigned int DebugDrawManager::lineNumIndex = NULL;

unsigned int DebugDrawManager::shader = NULL;

void DebugDrawManager::initialize( const char* objectsLocation )
{
	objectsLocation;
	glewInit();

	const char* vertexShader =
		"#version 400\r\n"
		"in layout(location=0) vec3 position;"
		"in layout(location=1) vec2 uv;"
		"uniform bool isAlreadyPerspective;"
		"uniform mat4 perspective;"
		"uniform mat4 view;"
		"uniform mat4 transform;"
		"out vec2 uvs;"

		"void main(){"
		"if (isAlreadyPerspective) gl_Position = transform * vec4(position, 1);"
		"else gl_Position = perspective * view * transform * vec4(position, 1);"
		"uvs = uv;"
		"};";

	const char* fragmentShader =
		"#version 400\r\n"
		"uniform int mode;"
		"uniform vec4 color;"
		"in vec2 uvs;"
		"out vec4 colorRes;"

		"void main(){"
		"if(mode == 0) colorRes = color;"
		"else if(mode == 1) colorRes = uvs.x * color;"
		"else colorRes = vec4(uvs.x, uvs.y, uvs.y, 1);"
		"};";

	unsigned int vertexS = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fragS = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vertexS , 1 , &vertexShader , 0 );
	glShaderSource( fragS , 1 , &fragmentShader , 0 );
	glCompileShader( vertexS );
	glCompileShader( fragS );
	shader = glCreateProgram();
	glAttachShader( shader , vertexS );
	glAttachShader( shader , fragS );
	glLinkProgram( shader );
	
	glDeleteShader( vertexS );
	glDeleteShader( fragS );

	glGenBuffers( 1 , &bufferID );
	glBindBuffer( GL_ARRAY_BUFFER , bufferID );
	glBufferData( GL_ARRAY_BUFFER , MAXDEBUGBUFFERSIZE , 0 , GL_DYNAMIC_DRAW );
	int offsetAddress = 0;
	{
		glGenVertexArrays( 1 , &cubeID );
		glBindVertexArray( cubeID );
		glBindBuffer( GL_ARRAY_BUFFER , bufferID );

		std::ifstream stream( std::string( std::string(objectsLocation) + std::string( "cube.objb" ) ) , std::ios::ios_base::binary | std::ios::ios_base::in );

		std::string buffer( ( std::istreambuf_iterator<char>( stream ) ) ,
							std::istreambuf_iterator<char>() );

		int numVertices = *( int* ) buffer.c_str();
		int numIndices = *( ( int* ) buffer.c_str() + 1 );

		unsigned int dataSize = ( ( sizeof( float ) * 8 ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );


		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress + ( sizeof( float ) * 3 ) ) );
		glBufferSubData( GL_ARRAY_BUFFER , offsetAddress , dataSize , ( GLvoid* ) ( ( int* ) buffer.c_str() + 2 ) );
		cubeIndex = offsetAddress + ( ( sizeof( float ) * 8 ) * numVertices );
		cubeNumIndices = numIndices;
		offsetAddress += dataSize;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferID );
		stream.close();
	}
	{
		glGenVertexArrays( 1 , &sphereID );
		glBindVertexArray( sphereID );
		glBindBuffer( GL_ARRAY_BUFFER , bufferID );

		std::ifstream stream( std::string( objectsLocation + std::string( "sphere.objb" ) ) , std::ios::ios_base::binary | std::ios::ios_base::in );

		std::string buffer( ( std::istreambuf_iterator<char>( stream ) ) ,
							std::istreambuf_iterator<char>() );

		int numVertices = *( int* ) buffer.c_str();
		int numIndices = *( ( int* ) buffer.c_str() + 1 );

		unsigned int dataSize = ( ( sizeof( float ) * 8 ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );


		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress + ( sizeof( float ) * 3 ) ) );
		glBufferSubData( GL_ARRAY_BUFFER , offsetAddress , dataSize , ( GLvoid* ) ( ( int* ) buffer.c_str() + 2 ) );
		sphereIndex = offsetAddress + ( ( sizeof( float ) * 8 ) * numVertices );
		sphereNumIndices = numIndices;
		offsetAddress += dataSize;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferID );
		stream.close();
	}

	{
		glGenVertexArrays( 1 , &cylID );
		glBindVertexArray( cylID );
		glBindBuffer( GL_ARRAY_BUFFER , bufferID );

		std::ifstream stream( std::string( objectsLocation + std::string( "vectorCyl.objb" ) ) , std::ios::ios_base::binary | std::ios::ios_base::in );

		std::string buffer( ( std::istreambuf_iterator<char>( stream ) ) ,
							std::istreambuf_iterator<char>() );

		int numVertices = *( int* ) buffer.c_str();
		int numIndices = *( ( int* ) buffer.c_str() + 1 );

		unsigned int dataSize = ( ( sizeof( float ) * 8 ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );


		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress + ( sizeof( float ) * 3 ) ) );
		glBufferSubData( GL_ARRAY_BUFFER , offsetAddress , dataSize , ( GLvoid* ) ( ( int* ) buffer.c_str() + 2 ) );
		cylIndex = offsetAddress + ( ( sizeof( float ) * 8 ) * numVertices );
		cylNumIndices = numIndices;
		offsetAddress += dataSize;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferID );
		stream.close();
	}
	{
		glGenVertexArrays( 1 , &coneID );
		glBindVertexArray( coneID );
		glBindBuffer( GL_ARRAY_BUFFER , bufferID );

		std::ifstream stream( std::string( objectsLocation + std::string( "vectorCone.objb" ) ) , std::ios::ios_base::binary | std::ios::ios_base::in );

		std::string buffer( ( std::istreambuf_iterator<char>( stream ) ) ,
							std::istreambuf_iterator<char>() );

		int numVertices = *( int* ) buffer.c_str();
		int numIndices = *( ( int* ) buffer.c_str() + 1 );

		unsigned int dataSize = ( ( sizeof( float ) * 8 ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );


		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 8 , ( void* ) ( offsetAddress + ( sizeof( float ) * 3 ) ) );
		glBufferSubData( GL_ARRAY_BUFFER , offsetAddress , dataSize , ( GLvoid* ) ( ( int* ) buffer.c_str() + 2 ) );
		coneIndex = offsetAddress + ( ( sizeof( float ) * 8 ) * numVertices );
		coneNumIndices = numIndices;
		offsetAddress += dataSize;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferID );
		stream.close();
	}
	{
		glGenVertexArrays( 1 , &lineID );
		glBindVertexArray( lineID );
		glBindBuffer( GL_ARRAY_BUFFER , bufferID );

		float lineVerts[] =
		{
			0, 0,0,
			0,0,
			1.0f,0,0,
			1.0f,1.0f
		};

		unsigned short lineInd[] =
		{
			0,1
		};

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof(float) * 5 , ( void* ) ( offsetAddress ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 5 , ( void* ) ( offsetAddress+(sizeof( float ) * 3) ) );
		
		glBufferSubData( GL_ARRAY_BUFFER , offsetAddress , sizeof( lineVerts ) , lineVerts );
		offsetAddress += sizeof( lineVerts );
		lineNumIndex = 2;
		lineIndex = offsetAddress;
		glBufferSubData( GL_ARRAY_BUFFER , offsetAddress , sizeof( lineInd ) , lineInd );
		offsetAddress += sizeof( lineInd );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferID );
	}
	physicalArraySize = 10;
	shapes = new DebugShape*[physicalArraySize];
}

void DebugDrawManager::addDebugShape( DebugShape* shape )
{
	if ( physicalArraySize <= currentArraySize )
	{
		unsigned int lastPhysicalSize = physicalArraySize;
		physicalArraySize += 1;
		physicalArraySize *= 2;
		DebugShape** newArray = new DebugShape*[physicalArraySize];
		if ( shapes != nullptr && lastPhysicalSize > 0 )
		{
			memcpy( newArray , shapes , sizeof( DebugShape* ) *  lastPhysicalSize );
			delete[] shapes;
			shapes = nullptr;
		}
		shapes = newArray;
	}

	shapes[currentArraySize] = shape;
	currentArraySize++;
}

void DebugDrawManager::deleteDebugShape( unsigned int id )
{
	delete shapes[id];
	currentArraySize--;
	shapes[id] = shapes[currentArraySize];

	if ( currentArraySize < physicalArraySize / 3 )
	{
		unsigned int lastPhysicalArray = physicalArraySize;
		physicalArraySize /= 2;
		if ( physicalArraySize > 0 )
		{
			DebugShape** newArray = new DebugShape*[physicalArraySize];
			memcpy( newArray , shapes , sizeof( DebugShape* ) *  currentArraySize );

			if ( shapes != nullptr && lastPhysicalArray > 0 )
			{
				delete[] shapes;
				shapes = nullptr;
			}
			shapes = newArray;
		}
		else
		{
			if ( shapes != nullptr && lastPhysicalArray > 0 )
			{
				delete[] shapes;
				shapes = nullptr;
			}
		}
	}
}

void DebugDrawManager::addPoint( const glm::mat4& centerTransform ,
								 const float duration ,
								 const bool depthEnabled ,
								 const bool isAlreadyPerspective )
{
	addDebugShape( new DebugPoint( centerTransform , duration , depthEnabled , isAlreadyPerspective ) );
}

void DebugDrawManager::addLine( const glm::vec3& fromPosition ,
								const glm::vec3& toPosition ,
								const glm::vec4& color ,
								const float duration ,
								const bool depthEnabled ,
								const bool isAlreadyPerspective
								)
{
	addDebugShape( new DebugLine( fromPosition , toPosition , color , duration , depthEnabled , isAlreadyPerspective ) );
}

void DebugDrawManager::addUnitVector( const glm::vec3& fromPosition ,
									  const glm::vec3& toPosition ,
									  const glm::vec4& color ,
									  const float duration ,
									  const bool depthEnabled ,
									  const bool isAlreadyPerspective )
{
	addDebugShape( new DebugUnitVector( fromPosition , toPosition , color , duration , depthEnabled , isAlreadyPerspective ) );
}

void DebugDrawManager::addSphere( const glm::mat4 centerTransform ,
								  const glm::vec4 color ,
								  const float duration ,
								  const bool depthEnabled ,
								  const bool isAlreadyPerspective )
{
	addDebugShape( new DebugSphere( centerTransform , color , duration , depthEnabled , isAlreadyPerspective ) );
}

void DebugDrawManager::addCube( const glm::mat4 centerTransform ,
								const glm::vec4 color ,
								const float duration ,
								const bool depthEnabled ,
								const bool isAlreadyPerspective )
{
	addDebugShape( new DebugCube( centerTransform , color , duration , depthEnabled , isAlreadyPerspective ) );
}

void DebugDrawManager::addVector( const glm::vec3& fromPosition ,
					   const glm::vec3& toPosition ,
					   const glm::vec4& color ,
					   const float headLocation,
					   const float size ,
					   const float duration ,
					   const bool depthEnabled ,
					   const bool isAlreadyPerspective )
{
	addDebugShape( new DebugVector( fromPosition , toPosition , color , headLocation , size , duration , depthEnabled , isAlreadyPerspective ) );
}

void DebugDrawManager::update( const float dt )
{
	for ( unsigned int i = 0; i < currentArraySize; i++ )
	{
		if ( !shapes[i]->update( dt ) )
		{
			deleteDebugShape( i );
			i--;
		}
	}
}

void DebugDrawManager::draw( glm::mat4& viewMatrix , glm::mat4& perspectiveMatrix )
{
	glUseProgram( shader );
	GLint uniform = glGetUniformLocation( shader , "perspective" );
	glUniformMatrix4fv( uniform , 1 , false , &perspectiveMatrix[0][0] );
	uniform = glGetUniformLocation( shader , "view" );
	glUniformMatrix4fv( uniform , 1 , false , &viewMatrix[0][0] );
	for ( unsigned int i = 0; i < currentArraySize; i++ )
	{
		shapes[i]->draw();
	}
}

DebugDrawManager::DebugPoint::DebugPoint( const glm::mat4& transformation , const float duration , const bool depthEnabled , const bool isAlreadyPerspective ) : transformation( transformation )
{
	this->duration = duration;
	this->depthEnabled = depthEnabled;
	this->isAlreadyPerspective = isAlreadyPerspective;
}

void DebugDrawManager::DebugPoint::draw( )
{
	glBindVertexArray( lineID );

	if (depthEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	GLint colorID = glGetUniformLocation( shader , "color" );


	GLint uniformID = glGetUniformLocation( shader , "mode" );
	glUniform1i( uniformID , 1 );
	uniformID = glGetUniformLocation( shader , "isAlreadyPerspective" );
	glUniform1i( uniformID , isAlreadyPerspective );

	glm::mat4 baseTrans = glm::translate( -1.0f , 0.0f , 0.0f ) * glm::scale( 2.0f , 1.0f , 1.0f );
	glm::mat4 trans = transformation * baseTrans;
	uniformID = glGetUniformLocation( shader , "transform" );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &trans[0][0] );
	glUniform4f( colorID , 1.0f , 0 , 0 , 1.0f );
	glDrawElements( GL_LINES , lineNumIndex , GL_UNSIGNED_SHORT , ( void* ) lineIndex );

	trans = transformation * glm::rotate( 90.0f , glm::vec3( 0 , 0 , 1 ) ) * baseTrans;
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &trans[0][0] );
	glUniform4f( colorID , 0 , 1.0f , 0 , 1.0f );
	glDrawElements( GL_LINES , lineNumIndex , GL_UNSIGNED_SHORT , ( void* ) lineIndex );

	trans = transformation * glm::rotate( -90.0f , glm::vec3( 0 , 1 , 0 ) ) * baseTrans;
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &trans[0][0] );
	glUniform4f( colorID , 0 , 0 , 1.0f , 1.0f );
	glDrawElements( GL_LINES , lineNumIndex , GL_UNSIGNED_SHORT , ( void* ) lineIndex );
}

bool DebugDrawManager::DebugPoint::update( const float dt )
{
	duration -= dt;
	return duration > 0;
}

DebugDrawManager::DebugLine::DebugLine( const glm::vec3& from , const glm::vec3& to , const glm::vec4& color , const float duration , const bool depthEnabled , const bool isAlreadyPerspective )
{
	glm::vec3 theLine = to - from;
	glm::vec3 xBasis = glm::normalize( theLine );
	glm::vec3 zBasis = glm::normalize( glm::cross( xBasis , glm::vec3( 1.0f , 0 , 0 ) ) );
	if ( glm::isnan(zBasis.x))
	{
		transformation = glm::translate( from ) * glm::scale( glm::length( theLine ) , 1.0f , 1.0f );
	}
	else
	{
		glm::vec3 yBasis = glm::normalize( glm::cross( xBasis , zBasis ) );
		glm::mat4 rotation( glm::vec4( xBasis , 0 ) , glm::vec4( yBasis , 0 ) , glm::vec4( zBasis , 0 ) , glm::vec4( 0 , 0 , 0 , 1.0f ) );
		transformation = glm::translate( from ) * rotation * glm::scale( glm::length( theLine ) , 1.0f , 1.0f );
	}
	
	this->color = color;
	this->duration = duration;
	this->depthEnabled = depthEnabled;
	this->isAlreadyPerspective = isAlreadyPerspective;
}

void DebugDrawManager::DebugLine::draw( )
{
	glBindVertexArray( lineID );

	if ( depthEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	GLint uniformID = glGetUniformLocation( shader , "mode" );
	glUniform1i( uniformID , 0 );
	uniformID = glGetUniformLocation( shader , "isAlreadyPerspective" );
	glUniform1i( uniformID , isAlreadyPerspective );

	uniformID = glGetUniformLocation( shader , "color" );
	glUniform4fv( uniformID , 1 , &color[0] );

	uniformID = glGetUniformLocation( shader , "transform" );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &transformation[0][0] );

	glDrawElements( GL_LINES , lineNumIndex , GL_UNSIGNED_SHORT , ( void* ) lineIndex );
}
bool DebugDrawManager::DebugLine::update( const float dt )
{
	duration -= dt;
	return duration > 0;
}

DebugDrawManager::DebugUnitVector::DebugUnitVector( const glm::vec3& from , const glm::vec3& to , const glm::vec4& color , const float duration , const bool depthEnabled , const bool isAlreadyPerspective ) : color( color )
{
	this->duration = duration;
	this->depthEnabled = depthEnabled;
	this->isAlreadyPerspective = isAlreadyPerspective;

	glm::vec3 theLine = to - from;
	glm::vec3 xBasis = glm::normalize( theLine );
	glm::vec3 zBasis = glm::normalize( glm::cross( xBasis , glm::vec3( 1.0f , 0 , 0 ) ) );
	if ( glm::isnan( zBasis.x ) )
	{
		if (from.x <= to.x) transformation = glm::translate( from ) ;
		else transformation = glm::translate( from ) * glm::rotate(180.0f, glm::vec3(0,1,0));
	}
	else
	{
		glm::vec3 yBasis = glm::normalize( glm::cross( xBasis , zBasis ) );
		glm::mat4 rotation( glm::vec4( xBasis , 0 ) , glm::vec4( yBasis , 0 ) , glm::vec4( zBasis , 0 ) , glm::vec4( 0 , 0 , 0 , 1.0f ) );
		transformation = glm::translate( from ) * rotation;
	}
}

void DebugDrawManager::DebugUnitVector::draw()
{
	glBindVertexArray( lineID );

	if ( depthEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	GLint uniformID = glGetUniformLocation( shader , "mode" );
	glUniform1i( uniformID , 1 );
	uniformID = glGetUniformLocation( shader , "isAlreadyPerspective" );
	glUniform1i( uniformID , isAlreadyPerspective );
	uniformID = glGetUniformLocation( shader , "color" );
	glUniform4fv( uniformID , 1 , &color[0] );

	uniformID = glGetUniformLocation( shader , "transform" );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &transformation[0][0] );

	glDrawElements( GL_LINES , lineNumIndex , GL_UNSIGNED_SHORT , ( void* ) lineIndex );
}

bool DebugDrawManager::DebugUnitVector::update( const float dt )
{
	duration -= dt;
	return duration > 0;
}

DebugDrawManager::DebugCube::DebugCube( const glm::mat4& transformation , const glm::vec4& color , const float duration , const bool depthEnabled , const bool isAlreadyPerspective ) : transformation( transformation ) , color( color )
{
	this->duration = duration;
	this->depthEnabled = depthEnabled;
	this->isAlreadyPerspective = isAlreadyPerspective;
}

void DebugDrawManager::DebugCube::draw()
{
	glBindVertexArray( cubeID );
	if ( depthEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	GLint uniformID = glGetUniformLocation( shader , "mode" );
	if ( color.w == -1 ) glUniform1i( uniformID , 2 );
	else glUniform1i( uniformID , 0 );
	uniformID = glGetUniformLocation( shader , "isAlreadyPerspective" );
	glUniform1i( uniformID , isAlreadyPerspective );

	uniformID = glGetUniformLocation( shader , "color" );
	glUniform4fv( uniformID , 1 , &color[0] );

	uniformID = glGetUniformLocation( shader , "transform" );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &transformation[0][0] );
	glDrawElements( GL_TRIANGLES , cubeNumIndices , GL_UNSIGNED_SHORT , ( void* ) cubeIndex );
}

bool DebugDrawManager::DebugCube::update( const float dt )
{
	duration -= dt;
	return duration > 0;
}

DebugDrawManager::DebugSphere::DebugSphere( const glm::mat4& transformation , const glm::vec4& color , const float duration , const bool depthEnabled , const bool isAlreadyPerspective ) : transformation( transformation ) , color( color )
{
	this->duration = duration;
	this->depthEnabled = depthEnabled;
	this->isAlreadyPerspective = isAlreadyPerspective;
}

void DebugDrawManager::DebugSphere::draw()
{
	if ( depthEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );
	glBindVertexArray( sphereID );

	GLint uniformID = glGetUniformLocation( shader , "mode" );
	if ( color.w == -1 ) glUniform1i( uniformID , 2 );
	else glUniform1i( uniformID , 0 );
	uniformID = glGetUniformLocation( shader , "isAlreadyPerspective" );
	glUniform1i( uniformID , isAlreadyPerspective );
	uniformID = glGetUniformLocation( shader , "color" );
	glUniform4fv( uniformID , 1 , &color[0] );

	uniformID = glGetUniformLocation( shader , "transform" );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &transformation[0][0] );

	glDrawElements( GL_TRIANGLES , sphereNumIndices , GL_UNSIGNED_SHORT , ( void* ) sphereIndex );
}

bool DebugDrawManager::DebugSphere::update( const float dt )
{
	duration -= dt;
	return duration > 0;
}

void DebugDrawManager::cleanUp()
{
	if ( glIsProgram( shader ) ) glDeleteProgram( shader );
	if ( glIsVertexArray( cubeID ) ) glDeleteVertexArrays( 1 , &cubeID );
	if ( glIsVertexArray( sphereID ) ) glDeleteVertexArrays( 1 , &sphereID );
	if ( glIsVertexArray( cylID ) ) glDeleteVertexArrays( 1 , &cylID );
	if ( glIsVertexArray( lineID ) ) glDeleteVertexArrays( 1 , &lineID );
	if ( glIsBuffer( bufferID ) ) glDeleteBuffers( 1 , &bufferID );

	if ( shapes != nullptr && physicalArraySize > 0 )
	{
		for ( unsigned int i = 0; i < currentArraySize; i++ )
		{
			delete shapes[i];
		}
		delete[] shapes;
		shapes = nullptr;
	}
}

DebugDrawManager::DebugVector::DebugVector( const glm::vec3& from , const glm::vec3& to , const glm::vec4& color , const float headLocation, const float size , const float duration , const bool depthEnabled , const bool isAlreadyPerspective ) : color( color ) , size(size)
{
	this->duration = duration;
	this->depthEnabled = depthEnabled;
	this->isAlreadyPerspective = isAlreadyPerspective;

	glm::vec3 theLine = to - from;
	glm::vec3 xBasis = glm::normalize( theLine );
	glm::vec3 zBasis = glm::normalize( glm::cross( xBasis , glm::vec3( 1.0f , 0 , 0 ) ) );
	if ( glm::isnan( zBasis.x ) )
	{
		if ( from.x <= to.x )
		{
			cylTransformation = glm::translate( from ) * glm::scale( glm::length( theLine ) - headLocation , size , size );
			coneTransformation = glm::translate( to + ( -headLocation * xBasis ) ) * glm::scale( 1.0f , size , size );
		}
		else
		{
			cylTransformation = glm::translate( from ) * glm::rotate( 180.0f , glm::vec3( 0 , 1 , 0 )) * glm::scale( glm::length( theLine ) - headLocation , size , size );
			coneTransformation = glm::translate( to + ( -headLocation * xBasis ) ) * glm::rotate( 180.0f , glm::vec3( 0 , 1 , 0 )) * glm::scale( 1.0f , size , size );
		}
	}
	else
	{
		glm::vec3 yBasis = glm::normalize( glm::cross( xBasis , zBasis ) );
		glm::mat4 rotation( glm::vec4( xBasis , 0 ) , glm::vec4( yBasis , 0 ) , glm::vec4( zBasis , 0 ) , glm::vec4( 0 , 0 , 0 , 1.0f ) );
		cylTransformation = glm::translate( from ) * rotation * glm::scale( glm::length( theLine ) - headLocation , size , size );
		coneTransformation = glm::translate( to + ( -headLocation * xBasis ) ) * rotation * glm::scale( 1.0f , size , size );
	}
}

void DebugDrawManager::DebugVector::draw()
{
	if ( depthEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	GLint uniformID = glGetUniformLocation( shader , "mode" );
	if ( color.w == -1 ) glUniform1i( uniformID , 2 );
	else glUniform1i( uniformID , 1 );
	uniformID = glGetUniformLocation( shader , "isAlreadyPerspective" );
	glUniform1i( uniformID , isAlreadyPerspective );
	uniformID = glGetUniformLocation( shader , "color" );
	glUniform4fv( uniformID , 1 , &color[0] );

	uniformID = glGetUniformLocation( shader , "transform" );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &cylTransformation[0][0] );
	glBindVertexArray( cylID );
	glDrawElements( GL_TRIANGLES , cylNumIndices , GL_UNSIGNED_SHORT , ( void* ) cylIndex );
	glUniformMatrix4fv( uniformID , 1 , GL_FALSE , &coneTransformation[0][0] );
	glBindVertexArray( coneID );
	glDrawElements( GL_TRIANGLES , coneNumIndices , GL_UNSIGNED_SHORT , ( void* ) coneIndex );
}

bool DebugDrawManager::DebugVector::update( const float dt )
{
	duration -= dt;
	return duration > 0;
}