#include <Graphics\TextureInfo.h>
#include <fstream>
#include <GL\glew.h>
void TextureInfo::editTexture( const char* file , unsigned int slot , unsigned int wrap )
{
	if ( !strlen( file ) ) return;
	std::ifstream stream( file , std::ios::ios_base::binary | std::ios::ios_base::in );
	int width , height;
	stream.read( ( char* ) &width , sizeof( int ) );
	stream.read( ( char* ) &height , sizeof( int ) );
	unsigned char* theImage = new unsigned char[width * height * 4];
	stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
	editTexture( ( char* ) theImage , width , height , slot , GL_RGBA , GL_RGBA , GL_UNSIGNED_BYTE , wrap );
	delete[] theImage;
}

void TextureInfo::editTexture( const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z , unsigned int slot , unsigned int wrap )
{
	if ( glIsTexture( textureID ) ) glDeleteTextures( 1 , &textureID );

	glGenTextures( 1 , &textureID );

	glBindTexture( GL_TEXTURE_CUBE_MAP , textureID );

	{
		std::ifstream stream( X , std::ios::ios_base::binary | std::ios::ios_base::in );
		int width , height;
		stream.read( ( char* ) &width , sizeof( int ) );
		stream.read( ( char* ) &height , sizeof( int ) );
		unsigned char* theImage = new unsigned char[width * height * 4];
		stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , theImage );
		delete[] theImage;
	}

		{
			std::ifstream stream( x , std::ios::ios_base::binary | std::ios::ios_base::in );
			int width , height;
			stream.read( ( char* ) &width , sizeof( int ) );
			stream.read( ( char* ) &height , sizeof( int ) );
			unsigned char* theImage = new unsigned char[width * height * 4];
			stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
			glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , theImage );
			delete[] theImage;

		}

		{
			std::ifstream stream( Y , std::ios::ios_base::binary | std::ios::ios_base::in );
			int width , height;
			stream.read( ( char* ) &width , sizeof( int ) );
			stream.read( ( char* ) &height , sizeof( int ) );
			unsigned char* theImage = new unsigned char[width * height * 4];
			stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , theImage );
			delete[] theImage;

		}

		{
			std::ifstream stream( y , std::ios::ios_base::binary | std::ios::ios_base::in );
			int width , height;
			stream.read( ( char* ) &width , sizeof( int ) );
			stream.read( ( char* ) &height , sizeof( int ) );
			unsigned char* theImage = new unsigned char[width * height * 4];
			stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
			glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , theImage );
			delete[] theImage;

		}

		{
			std::ifstream stream( Z , std::ios::ios_base::binary | std::ios::ios_base::in );
			int width , height;
			stream.read( ( char* ) &width , sizeof( int ) );
			stream.read( ( char* ) &height , sizeof( int ) );
			unsigned char* theImage = new unsigned char[width * height * 4];
			stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , theImage );
			delete[] theImage;

		}

		{
			std::ifstream stream( z , std::ios::ios_base::binary | std::ios::ios_base::in );
			int width , height;
			stream.read( ( char* ) &width , sizeof( int ) );
			stream.read( ( char* ) &height , sizeof( int ) );
			unsigned char* theImage = new unsigned char[width * height * 4];
			stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
			glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , theImage );
			delete[] theImage;

		}


	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	type = GL_TEXTURE_CUBE_MAP;
	textureSlot = slot;
}

void TextureInfo::editTexture( const char* data , unsigned int width , unsigned int height , unsigned int slot , unsigned int inputFormat , unsigned int outputFormat , unsigned int dataType , unsigned int wrap )
{
	if ( glIsTexture( textureID ) ) glDeleteTextures( 1 , &textureID );
	glGenTextures( 1 , &textureID );
	glBindTexture( GL_TEXTURE_2D , textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	glTexImage2D( GL_TEXTURE_2D , 0 , inputFormat , width , height , 0 , outputFormat , dataType , data );
	type = GL_TEXTURE_2D;
	textureSlot = slot;
}

TextureParams TextureInfo::getParams( )
{
	if ( !glIsTexture( textureID ) ) return TextureParams();
	glBindTexture( GL_TEXTURE_2D , textureID );
	int theWidth , theHeight;
	glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_WIDTH , &theWidth );
	glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_HEIGHT , &theHeight );

	TextureParams params;
	params.width = theWidth;
	params.height = theHeight;
	return params;
}