#pragma once
#include <PokEngineExportHeader.h>
class GraphicsBufferManager;
struct POKENGINE_SHARED BufferInfo
{
	int bufferID;
	unsigned int bufferSpace;
	unsigned int offsetAddress;
	BufferInfo();
};