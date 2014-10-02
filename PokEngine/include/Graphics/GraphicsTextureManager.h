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

	TextureInfo* addTexture( const char * file, const unsigned int slot = 0 , const unsigned int wrap = 0x2901 );
	void addCubeTexture( TextureInfo* theTexture , const char* X , const char* x , const char* Y , const char* y , const char* Z , const char* z , unsigned int index , unsigned int wrap = 0x812F );
	void editTexture( TextureInfo* theTexture , const char* file , unsigned int index , unsigned int wrap = 0x2901 );
	void editTexture( TextureInfo* theTexture , const char* data , unsigned int width , unsigned int height , unsigned int index , unsigned int inputFormat = 0x1908 , unsigned int outputFormat = 0x1908 , unsigned int dataType = 0x1401 , unsigned int wrap = 0x2901 );
};