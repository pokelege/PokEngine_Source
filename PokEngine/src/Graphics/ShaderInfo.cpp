#include <Graphics\ShaderInfo.h>
#include <Graphics\ParameterType.h>
#include <GL\glew.h>
ShaderInfo::ShaderInfo() :shaderCodePersisted( false ) , vertexShader( 0 ) , fragmentShader( 0 ) {}

void ShaderInfo::setUniformParameter(
	const char* name ,
	ParameterType parameterType ,
	const void* value ,
	unsigned int size )
{
	GLint uniformID = glGetUniformLocation( programID , name );
	switch ( parameterType )
	{
		case( PT_INT ) : glUniform1iv( uniformID , size , reinterpret_cast<const int*>( value ) ); break;
		case( PT_IVEC2 ) : glUniform2iv( uniformID , size , reinterpret_cast< const int* >( value ) ); break;
		case( PT_IVEC3 ) : glUniform3iv( uniformID , size , reinterpret_cast< const int* >( value ) );
			break;
		case( PT_IVEC4 ) : glUniform3iv( uniformID , size , reinterpret_cast< const int* >( value ) ); break;
		case ( PT_FLOAT ) : glUniform1fv( uniformID , size , reinterpret_cast< const float* >(value) );
			break;
		case ( PT_VEC2 ) : glUniform2fv( uniformID , size , reinterpret_cast< const float* >( value ) );
			break;
		case( PT_VEC3 ) : glUniform3fv( uniformID , size , reinterpret_cast< const float* >( value ) );
			break;
		case( PT_VEC4 ) : glUniform4fv( uniformID , size , reinterpret_cast< const float* >( value ) );
			break;
		case( PT_MAT3 ) : glUniformMatrix3fv( uniformID , size , GL_FALSE , reinterpret_cast< const float* >( value ) );
			break;
		case( PT_MAT4 ) : glUniformMatrix4fv( uniformID , size , GL_FALSE , reinterpret_cast< const float* >( value ) );
			break;
	}
}