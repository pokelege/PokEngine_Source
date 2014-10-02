#include <Graphics\GraphicsTextureManager.h>
#include <Graphics\TextureInfo.h>
#include <GL\glew.h>
#include <SOIL.h>
#include <string>
GraphicsTextureManager GraphicsTextureManager::globalTextureManager;

GraphicsTextureManager::GraphicsTextureManager() : numTextureSlots(0) {}
void GraphicsTextureManager::initialize( unsigned int numTextureSlots = MAX_TEXTURES )
{
	if ( textureInfos ) destroy();

	this->numTextureSlots = numTextureSlots;
	textureInfos = new TextureInfo[numTextureSlots];
}
void GraphicsTextureManager::destroy()
{
	if ( textureInfos ) return;
	for ( int i = 0; i < numTextureSlots; i++ )
	{
		if ( glIsTexture( textureInfos[i].textureID ) ) glDeleteTextures(1, &textureInfos[i].textureID);
	}
	delete[] textureInfos;
	numTextureSlots = 0;
}
bool GraphicsTextureManager::initialized()
{
	return textureInfos;
}

TextureInfo* GraphicsTextureManager::addTexture( const char * file, unsigned int slot , unsigned int wrap )
{
	if ( !strlen( file ) ) return 0;

	TextureInfo* texture;
	for (int i = 0; i < numTextureSlots; ++i )
	{
		if ( !glIsTexture(textureInfos[i].textureID) )
		{
			texture = &textureInfos[i];
			break;
		}
	}

	
	texture->textureID = SOIL_load_OGL_texture( file , SOIL_LOAD_RGBA , SOIL_CREATE_NEW_ID , SOIL_FLAG_INVERT_Y );

	glBindTexture( GL_TEXTURE_2D , texture->textureID );
	glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , wrap );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE );

	texture->type = GL_TEXTURE_2D;

	return texture;
}

void GraphicsTextureManager::addCubeTexture( TextureInfo* theTexture , const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z , unsigned int index , unsigned int wrap )
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

void GraphicsTextureManager::editTexture( TextureInfo* theTexture , const char* file , unsigned int index , unsigned int wrap )
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

void GraphicsTextureManager::editTexture( TextureInfo* theTexture , const char* data , unsigned int width , unsigned int height , unsigned int index , unsigned int inputFormat , unsigned int outputFormat , unsigned int dataType , unsigned int wrap )
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