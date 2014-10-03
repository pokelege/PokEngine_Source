#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\ParameterType.h>
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

	void addShaderStreamedParameter(
		unsigned int layoutLocation ,
		ParameterType parameterType ,
		unsigned int bufferStride ,
		unsigned int bufferOffset );
};