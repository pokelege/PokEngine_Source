#pragma once
#include <PokEngineExportHeader.h>
struct POKENGINE_SHARED ShaderInfo
{
	unsigned int programID;
	const char* vertexShader;
	const char* fragmentShader;
	bool shaderCodePersisted;
	ShaderInfo() :shaderCodePersisted(false) , vertexShader(0) , fragmentShader(0) {}
};