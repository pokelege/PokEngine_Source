#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\ParameterType.h>
struct POKENGINE_SHARED ShaderInfo
{
	unsigned int programID;
	const char* vertexShader;
	const char* fragmentShader;
	bool shaderCodePersisted;
	ShaderInfo();
	void setUniformParameter(
		const char* name ,
		ParameterType parameterType ,
		const void* value ,
		unsigned int size = 1 );
};