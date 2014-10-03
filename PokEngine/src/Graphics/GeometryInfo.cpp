#include <Graphics\GeometryInfo.h>
#include <GL\glew.h>
void GeometryInfo::addShaderStreamedParameter(
	unsigned int layoutLocation ,
	ParameterType parameterType ,
	unsigned int bufferStride ,
	unsigned int bufferOffset )
{
	glBindVertexArray( dataArray );

	glEnableVertexAttribArray( layoutLocation );

	if ( parameterType < 0 )
	{
		int pt = -parameterType / sizeof( int );
		glVertexAttribPointer( layoutLocation , pt , GL_INT , GL_FALSE , bufferStride , ( void* ) ( vertexOffset + bufferOffset ) );
	}
	else
	{
		glVertexAttribPointer( layoutLocation , parameterType / sizeof( float ) , GL_FLOAT , GL_FALSE , bufferStride , ( void* ) ( vertexOffset + bufferOffset ) );
	}
}