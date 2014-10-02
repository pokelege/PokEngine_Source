#include <Graphics\GraphicsBufferManager.h>
#include <GL\glew.h>
#include <Graphics\BufferInfo.h>

GraphicsBufferManager GraphicsBufferManager::globalBufferManager;
GraphicsBufferManager::GraphicsBufferManager(): bufferIds(0) {}
void GraphicsBufferManager::initialize( unsigned int numBufferSlots , unsigned int numSizePerBufferSlot )
{
	if ( bufferIds )
	{
		destroy();
	}
	this->numBufferSlots = numBufferSlots;
	this->numSizePerBufferSlot = numSizePerBufferSlot;
	bufferIds = new BufferInfo[numBufferSlots];
}

BufferInfo* GraphicsBufferManager::getBuffer( const unsigned int& requiredBufferSpace )
{	
	BufferInfo* bufferInfo = 0;
	if ( bufferIds && requiredBufferSpace < numSizePerBufferSlot )
	{
		for ( int i = 0; i < numBufferSlots; ++i )
		{
			if ( glIsBuffer( bufferIds[i].bufferID ) == GL_FALSE )
			{
				unsigned int newBufferID = 0;
				glGenBuffers( 1 , &newBufferID );
				bufferIds[i].bufferID = newBufferID;
				glBindBuffer( GL_ARRAY_BUFFER , bufferIds[i].bufferID );

				glBufferData( GL_ARRAY_BUFFER , numSizePerBufferSlot , 0 , GL_DYNAMIC_DRAW );
				bufferIds[i].bufferSpace = numSizePerBufferSlot;
				bufferIds[i].offsetAddress = 0;
				bufferInfo = &bufferIds[i];
			}
			else if ( bufferIds[i].bufferSpace >= requiredBufferSpace )
			{
				glBindBuffer( GL_ARRAY_BUFFER , bufferIds[i].bufferID );
				bufferInfo = &bufferIds[i];
				break;
			}
		}
	}
	return bufferInfo;
}

void GraphicsBufferManager::destroy()
{
	if ( bufferIds )
	{
		for ( int i = 0; i < numBufferSlots; i++ )
		{
			unsigned int bufferid = bufferIds[i].bufferID;
			if ( glIsBuffer( bufferid ) ) glDeleteBuffers( 1 , &bufferid );
		}
		numBufferSlots = 0;
		numSizePerBufferSlot = 0;
		delete[] bufferIds;
		bufferIds = 0;
	}
}

bool GraphicsBufferManager::initialized()
{
	return bufferIds;
}