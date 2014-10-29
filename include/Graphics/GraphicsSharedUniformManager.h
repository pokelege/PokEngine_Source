#pragma once
#include <PokEngineExportHeader.h>
#include <Graphics\ParameterType.h>
#define MAX_UNIFORM_PARAMETERS 50

struct UniformInfo;
struct ShaderInfo;
class POKENGINE_SHARED GraphicsSharedUniformManager
{
	UniformInfo* globalUniforms;
	unsigned int numSharedUniformSlots;
public:
	static GraphicsSharedUniformManager globalSharedUniformManager;

	GraphicsSharedUniformManager();
	void initialize( unsigned int numSharedUniformSlots = MAX_UNIFORM_PARAMETERS );
	void destroy();
	bool initialized();

	UniformInfo* setSharedUniform(
		const char* name ,
		ParameterType parameterType ,
		const void* dataPointer,
		unsigned int size = 1);

	void applySharedUniforms( ShaderInfo* targetShader );
};