#include <Graphics\GraphicsTextureManager.h>
#include <Graphics\TextureInfo.h>
#include <GL\glew.h>
#include <fstream>
#include <string>
GraphicsTextureManager GraphicsTextureManager::globalTextureManager;

GraphicsTextureManager::GraphicsTextureManager() : numTextureSlots(0), textureInfos(0) {}
void GraphicsTextureManager::initialize( unsigned int numTextureSlots )
{
	if ( textureInfos ) destroy();

	this->numTextureSlots = numTextureSlots;
	textureInfos = new TextureInfo[numTextureSlots];
}
void GraphicsTextureManager::destroy()
{
	if ( !textureInfos ) return;
	for ( unsigned int i = 0; i < numTextureSlots; i++ )
	{
		if ( glIsTexture( textureInfos[i].textureID ) ) glDeleteTextures(1, &textureInfos[i].textureID);
	}
	delete[] textureInfos;
	textureInfos = 0;
	numTextureSlots = 0;
}
bool GraphicsTextureManager::initialized()
{
	return textureInfos != 0;
}

TextureInfo* GraphicsTextureManager::addTexture( const char * file, unsigned int slot , unsigned int wrap )
{
	if ( !strlen( file ) ) return 0;

	TextureInfo* texture = 0;

	std::ifstream stream( file , std::ios::ios_base::binary | std::ios::ios_base::in );
	int width , height;
	stream.read( ( char* ) &width , sizeof( int ) );
	stream.read( ( char* ) &height , sizeof( int ) );
	unsigned char* theImage = new unsigned char[width * height * 4];
	stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
	texture = addTexture( (char*)theImage , width , height , slot , GL_RGBA , GL_RGBA , GL_UNSIGNED_BYTE , wrap );
	delete[] theImage;
	return texture;
}

TextureInfo* GraphicsTextureManager::addTexture(  const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z, unsigned int slot , unsigned int wrap )
{
	if ( !strlen( X ) || !strlen( x ) || !strlen( y ) || !strlen( Y ) || !strlen( z ) || !strlen( Z ) ) return 0;

	TextureInfo* texture = 0;
	for ( unsigned int i = 0; i < numTextureSlots; ++i )
	{
		if ( !glIsTexture( textureInfos[i].textureID ) )
		{
			texture = &textureInfos[i];
			break;
		}
	}
	if ( !texture ) return texture;
	glGenTextures( 1 , &texture->textureID );

	glBindTexture( GL_TEXTURE_CUBE_MAP , texture->textureID );

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
	texture->type = GL_TEXTURE_CUBE_MAP;
	texture->textureSlot = slot;
	return texture;
}

TextureInfo* GraphicsTextureManager::addTexture(  const char* data , unsigned int width , unsigned int height , unsigned int slot , unsigned int inputFormat , unsigned int outputFormat , unsigned int dataType , unsigned int wrap )
{
	TextureInfo* texture = 0;
	for ( unsigned int i = 0; i < numTextureSlots; ++i )
	{
		if ( !glIsTexture( textureInfos[i].textureID ) )
		{
			texture = &textureInfos[i];
			break;
		}
	}
	if ( !texture ) return texture;
	glGenTextures( 1 , &texture->textureID );
	glBindTexture( GL_TEXTURE_2D , texture->textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	glTexImage2D( GL_TEXTURE_2D , 0 , inputFormat , width , height , 0 , outputFormat , dataType , data );
	texture->type = GL_TEXTURE_2D;
	texture->textureSlot = slot;
	return texture;
}

void GraphicsTextureManager::editTexture( TextureInfo* theTexture , const char* file , unsigned int slot , unsigned int wrap )
{
	if ( !strlen( file ) ) return;
	std::ifstream stream( file , std::ios::ios_base::binary | std::ios::ios_base::in );
	int width , height;
	stream.read( ( char* ) &width , sizeof( int ) );
	stream.read( ( char* ) &height , sizeof( int ) );
	unsigned char* theImage = new unsigned char[width * height * 4];
	stream.read( ( char* ) theImage , sizeof( unsigned char ) * ( width * height * 4 ) );
	editTexture( theTexture, ( char* ) theImage , width , height , slot , GL_RGBA , GL_RGBA , GL_UNSIGNED_BYTE , wrap );
	delete[] theImage;
}

void GraphicsTextureManager::editTexture( TextureInfo* theTexture , const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z, unsigned int slot , unsigned int wrap )
{
	if ( glIsTexture( theTexture->textureID ) ) glDeleteTextures( 1 , &theTexture->textureID );

	glGenTextures( 1 , &theTexture->textureID );

	glBindTexture( GL_TEXTURE_CUBE_MAP , theTexture->textureID );

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
	theTexture->type = GL_TEXTURE_CUBE_MAP;
	theTexture->textureSlot = slot;
}

void GraphicsTextureManager::editTexture( TextureInfo* theTexture , const char* data , unsigned int width , unsigned int height , unsigned int slot , unsigned int inputFormat , unsigned int outputFormat , unsigned int dataType , unsigned int wrap )
{
	if ( glIsTexture( theTexture->textureID ) ) glDeleteTextures( 1 , &theTexture->textureID );
	glGenTextures( 1 , &theTexture->textureID );
	glBindTexture( GL_TEXTURE_2D , theTexture->textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );
	glTexImage2D( GL_TEXTURE_2D , 0 , inputFormat , width , height , 0 , outputFormat , dataType , data );
	theTexture->type = GL_TEXTURE_2D;
	theTexture->textureSlot = slot;
}

void GraphicsTextureManager::deleteTexture( TextureInfo* theTexture )
{
	if ( !theTexture ) return;
	if ( glIsTexture( theTexture->textureID ) ) glDeleteTextures( 1 , &theTexture->textureID );
	*theTexture = TextureInfo();
}

std::string GraphicsTextureManager::saveRawTexture( TextureInfo* theTexture , int* width , int* height)
{
	if ( !glIsTexture( theTexture->textureID ) ) return std::string();
	glBindTexture( GL_TEXTURE_2D , theTexture->textureID );
	int theWidth , theHeight;
	glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_WIDTH , &theWidth );
	glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_HEIGHT , &theHeight );
	char* textureData = new char[theWidth * theHeight * 4];
	glGetTexImage( GL_TEXTURE_2D , 0 , GL_RGBA , GL_UNSIGNED_BYTE , textureData );
	if ( width ) *width = theWidth;
	if ( height ) *height = theHeight;
	std::string toReturn( textureData , theWidth * theHeight * 4 );
	delete[] textureData;
	return toReturn;
}