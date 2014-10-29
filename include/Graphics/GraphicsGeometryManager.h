#pragma once
#include <PokEngineExportHeader.h>
#include <string>
#define MAX_GEOMETRIES 20

struct GeometryInfo;
class GraphicsBufferManager;
class POKENGINE_SHARED GraphicsGeometryManager
{
private:
	GeometryInfo* geometryInfos;
	unsigned int numGeometrySlots;
public:
	static GraphicsGeometryManager globalGeometryManager;

	GraphicsGeometryManager();
	void initialize( unsigned int numGeometrySlots = MAX_GEOMETRIES );
	void destroy();
	bool initialized();

	GeometryInfo* addPMDGeometry( const char* filename , GraphicsBufferManager& bufferManager );
	GeometryInfo* addRawGeometry( const char* rawData , GraphicsBufferManager& bufferManager );

	//full implementation later
	std::string saveGeometry( GeometryInfo* geo );
	bool saveGeometry( GeometryInfo* geo , std::string fileName );

	template <class Tvert>
	GeometryInfo* addGeometry(
		const Tvert* verts , const unsigned int& numVerts ,
		const unsigned short* indices , const unsigned int& numIndices ,
		const unsigned int& indexingMode, GraphicsBufferManager& bufferManager )
	{
		unsigned int dataSize = sizeof( *verts ) * numVerts;
		dataSize += sizeof( unsigned short ) * numIndices;
		
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
		geometryInfos[j].indexingMode = indexingMode;
	
		glGenVertexArrays( 1 , &geometryInfos[j].dataArray );
		glBindVertexArray( geometryInfos[j].dataArray );
	
		geometryInfos[j].vertexOffset = buffer->offsetAddress;
		geometryInfos[j].numVertex = numVerts;
		glBufferSubData( GL_ARRAY_BUFFER , buffer->offsetAddress , sizeof( *verts )* numVerts , ( GLvoid* ) verts );
		buffer->offsetAddress += sizeof( *verts )* numVerts;
		buffer->bufferSpace -= sizeof( *verts )* numVerts;
	
		geometryInfos[j].indexOffset = buffer->offsetAddress;
		geometryInfos[j].numIndex = numIndices;
		glBufferSubData( GL_ARRAY_BUFFER , buffer->offsetAddress , sizeof( *indices )* numIndices , ( GLvoid* ) indices );
		buffer->offsetAddress += sizeof( *indices )* numIndices;
		buffer->bufferSpace -= sizeof( *indices )* numIndices;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , buffer->bufferID );
		return &geometryInfos[j];
	}
};