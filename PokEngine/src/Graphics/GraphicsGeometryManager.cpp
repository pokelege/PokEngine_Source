#include <Graphics\GraphicsGeometryManager.h>
#include <Graphics\GraphicsBufferManager.h>
#include <Graphics\GeometryInfo.h>
#include <Graphics\VertexInfo.h>
#include <GL\glew.h>
#include <Graphics\BufferInfo.h>
GraphicsGeometryManager globalGeometryManager;
GraphicsGeometryManager::GraphicsGeometryManager() : geometryInfos(0) {}

void GraphicsGeometryManager::initialize( unsigned int numGeometrySlots )
{
	if ( geometryInfos ) destroy();
	this->numGeometrySlots = numGeometrySlots;
	geometryInfos = new GeometryInfo[numGeometrySlots];
}

void GraphicsGeometryManager::destroy()
{
	if ( geometryInfos )
	{
		for ( int i = 0; i < MAX_GEOMETRIES; i++ )
		{
			if ( glIsVertexArray( geometryInfos[i].dataArray ) ) glDeleteVertexArrays( 1 , &geometryInfos[i].dataArray );
		}
		numGeometrySlots = 0;
		delete[] geometryInfos;
		geometryInfos = 0;
	}
}

bool GraphicsGeometryManager::initialized()
{
	return geometryInfos;
}

GeometryInfo* GraphicsGeometryManager::addPMDGeometry( const char* filename , GraphicsBufferManager& bufferManager )
{
	std::ifstream stream( filename , std::ios::ios_base::binary | std::ios::ios_base::in );
	PokEngineModelDataMap data( stream );
	unsigned int numVertices;
	unsigned int numIndices;
	VertexInfo* verts = data.getVertexData( &numVertices );
	unsigned short* indices = data.getIndexData( &numIndices );
	unsigned int dataSize = ( sizeof( VertexInfo ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );

	BufferInfo* buffer = bufferManager.getBuffer( dataSize );
	if ( !buffer ) return 0;

	int j;

	for ( j = 0; j < MAX_GEOMETRIES; j++ )
	{
		if ( glIsVertexArray( geometryInfos[j].dataArray ) == GL_FALSE )
		{
			break;
		}
	}

	geometryInfos[j].buffer = buffer;
	geometryInfos[j].indexingMode = GL_TRIANGLES;

	glGenVertexArrays( 1 , &geometryInfos[j].dataArray );
	glBindVertexArray( geometryInfos[j].dataArray );

	geometryInfos[j].vertexOffset = buffer->offsetAddress;
	geometryInfos[j].numVertex = numVertices;

	geometryInfos[j].indexOffset = buffer->offsetAddress + ( sizeof( VertexInfo )  * numVertices );
	geometryInfos[j].numIndex = numIndices;
	geometryInfos[j].modelData = data;

	glBufferSubData( GL_ARRAY_BUFFER , buffer->offsetAddress , ( sizeof( VertexInfo ) * numVertices ) , ( GLvoid* ) ( verts ) );

	glBufferSubData( GL_ARRAY_BUFFER , buffer->offsetAddress + ( sizeof( VertexInfo ) * numVertices ) , ( sizeof( unsigned short ) * numIndices ) , ( GLvoid* ) ( indices ) );

	buffer->offsetAddress += dataSize;
	buffer->bufferSpace -= dataSize;

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , buffer->bufferID );

	return &geometryInfos[j];
}

GeometryInfo* GraphicsGeometryManager::addRawGeometry( const char* rawData , GraphicsBufferManager& bufferManager )
{
	PokEngineModelDataMap data( rawData );
	unsigned int numVertices;
	unsigned int numIndices;
	VertexInfo* verts = data.getVertexData( &numVertices );
	unsigned short* indices = data.getIndexData( &numIndices );
	unsigned int dataSize = ( sizeof( VertexInfo ) * numVertices ) + ( sizeof( unsigned short ) * numIndices );

	BufferInfo* buffer = bufferManager.getBuffer( dataSize );
	if ( !buffer ) return 0;

	int j;

	for ( j = 0; j < MAX_GEOMETRIES; j++ )
	{
		if ( glIsVertexArray( geometryInfos[j].dataArray ) == GL_FALSE )
		{
			break;
		}
	}

	geometryInfos[j].buffer = buffer;
	geometryInfos[j].indexingMode = GL_TRIANGLES;

	glGenVertexArrays( 1 , &geometryInfos[j].dataArray );
	glBindVertexArray( geometryInfos[j].dataArray );

	geometryInfos[j].vertexOffset = buffer->offsetAddress;
	geometryInfos[j].numVertex = numVertices;

	geometryInfos[j].indexOffset = buffer->offsetAddress + ( sizeof( VertexInfo )  * numVertices );
	geometryInfos[j].numIndex = numIndices;
	geometryInfos[j].modelData = data;

	glBufferSubData( GL_ARRAY_BUFFER , buffer->offsetAddress , ( sizeof( VertexInfo ) * numVertices ) , ( GLvoid* ) ( verts ) );

	glBufferSubData( GL_ARRAY_BUFFER , buffer->offsetAddress + ( sizeof( VertexInfo ) * numVertices ) , ( sizeof( unsigned short ) * numIndices ) , ( GLvoid* ) ( indices ) );

	buffer->offsetAddress += dataSize;
	buffer->bufferSpace -= dataSize;

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , buffer->bufferID );

	return &geometryInfos[j];
}

//implementation later
const char* GraphicsGeometryManager::saveGeometry( GeometryInfo* geo )
{
	return 0;
}