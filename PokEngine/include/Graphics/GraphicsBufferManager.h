#pragma once
#include <PokEngineExportHeader.h>
#define MAX_BUFFERS 10
#define MAX_BUFFER_SIZE sizeof(char) * (unsigned int)1.049e+7

struct BufferInfo;
class POKENGINE_SHARED GraphicsBufferManager
{
private:
	unsigned int numBufferSlots;
	unsigned int numSizePerBufferSlot;
	BufferInfo* bufferIds;
public:
	static GraphicsBufferManager globalBufferManager;
	GraphicsBufferManager();
	void initialize( unsigned int numBufferSlots = MAX_BUFFERS , unsigned int numSizePerBufferSlot = MAX_BUFFER_SIZE);
	void destroy();
	bool initialized();
	BufferInfo* getBuffer( const unsigned int& requiredBufferSpace );
};