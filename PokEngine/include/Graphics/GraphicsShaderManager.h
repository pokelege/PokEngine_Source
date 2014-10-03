#pragma once
#include <PokEngineExportHeader.h>
#include <string>
#include <Graphics\ParameterType.h>
#define MAX_SHADERS 10
struct ShaderInfo;
class POKENGINE_SHARED GraphicsShaderManager
{
	ShaderInfo* shaderInfos;
	unsigned int numShaderSlots;
public:
	static GraphicsShaderManager globalShaderManager;
	
	GraphicsShaderManager();
	void initialize( unsigned int numShaderSlots = MAX_SHADERS );
	void destroy();
	bool initialized();
	ShaderInfo* createShaderInfo(
		const char* vertexShaderCode ,
		const char* fragmentShaderCode ,
		std::string* errorLog = nullptr );
	void updateShaderInfo(
		ShaderInfo* shaderInfoIndex ,
		const char* vertexShaderCode ,
		const char* fragmentShaderCode ,
		std::string* errorLog = nullptr );
};