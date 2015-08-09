#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\ParameterType.h>
struct BufferInfo;
struct POKENGINE_SHARED GeometryInfo
{
	BufferInfo* buffer = nullptr;
	unsigned int dataArray = 0;
	unsigned int vertexOffset = 0;
	unsigned int numVertex = 0;
	unsigned int indexOffset = 0;
	unsigned int numIndex = 0;
	unsigned int indexingMode = 0;
	PokEngineModelDataMap* modelData = nullptr;

	void addShaderStreamedParameter(
		unsigned int layoutLocation ,
		ParameterType parameterType ,
		unsigned int bufferStride ,
		unsigned int bufferOffset );
	GeometryInfo();
	~GeometryInfo();
};