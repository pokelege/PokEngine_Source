#pragma once
#include <PokEngineExportHeader.h>
#define MAX_TEXTURES 50

struct TextureInfo;
class POKENGINE_SHARED GraphicsTextureManager
{
	TextureInfo* textureInfos;
	unsigned int numTextureSlots;
public:
	static GraphicsTextureManager globalTextureManager;

	GraphicsTextureManager();
	void initialize( unsigned int numTextureSlots = MAX_TEXTURES);
	void destroy();
	bool initialized();

	//File texture
	TextureInfo* addTexture( const char * file, const unsigned int slot = 0 , const unsigned int wrap = 0x2901 );
	
	//Cube file texture
	TextureInfo* addTexture( const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z, unsigned int slot = 0 , unsigned int wrap = 0x812F );

	//Raw data texture
	TextureInfo* addTexture( const char* data , unsigned int width , unsigned int height , unsigned int slot = 0 , unsigned int inputFormat = 0x1908 , unsigned int outputFormat = 0x1908 , unsigned int dataType = 0x1401 , unsigned int wrap = 0x2901 );

	//File texture
	void editTexture( TextureInfo* theTexture , const char* file , unsigned int slot = 0 , unsigned int wrap = 0x2901 );

	//Cube file texture
	void editTexture( TextureInfo* theTexture , const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z, unsigned int slot = 0 , unsigned int wrap = 0x812F );
	//Raw data texture
	void editTexture( TextureInfo* theTexture , const char* data , unsigned int width , unsigned int height , unsigned int slot = 0 , unsigned int inputFormat = 0x1908 , unsigned int outputFormat = 0x1908 , unsigned int dataType = 0x1401, unsigned int wrap = 0x2901 );
};