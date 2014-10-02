#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\PokEngineModelDataMap.h>
struct BufferInfo;
struct POKENGINE_SHARED GeometryInfo
{
	BufferInfo* buffer;
	unsigned int dataArray;
	unsigned int vertexOffset;
	unsigned int numVertex;
	unsigned int indexOffset;
	unsigned int numIndex;
	unsigned int indexingMode;
	PokEngineModelDataMap modelData;
};