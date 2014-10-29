#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <string>
#include <Graphics\ParameterType.h>
struct POKENGINE_SHARED UniformInfo
{
	std::string uniformName;
	ParameterType type;
	unsigned int size;
	const void* location;
	UniformInfo() :location( nullptr ) {}
};