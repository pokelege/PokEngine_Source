#pragma once
#include <PokEngineExportHeader.h>

struct POKENGINE_SHARED TextureInfo
{
	unsigned int textureID;
	unsigned int type;
	unsigned int textureSlot;
	TextureInfo() : textureID( 0 ) , type( 0 ) {}
};