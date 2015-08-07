#pragma once
#include <PokEngineExportHeader.h>

struct POKENGINE_SHARED TextureParams
{
	unsigned int width, height;
};

struct POKENGINE_SHARED TextureInfo
{
	unsigned int textureID;
	unsigned int type;
	unsigned int textureSlot;
	TextureInfo() : textureID( 0 ) , type( 0 ) {}
	//File texture
	void editTexture( const char* file , unsigned int slot = 0 , unsigned int wrap = 0x2901 );

	//Cube file texture
	void editTexture( const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z , unsigned int slot = 0 , unsigned int wrap = 0x812F );
	//Raw data texture
	void editTexture( const char* data , unsigned int width , unsigned int height , unsigned int slot = 0 , unsigned int inputFormat = 0x1908 , unsigned int outputFormat = 0x1908 , unsigned int dataType = 0x1401 , unsigned int wrap = 0x2901 );
	TextureParams getParams();
};