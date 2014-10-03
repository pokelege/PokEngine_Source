#include <Graphics\GraphicsSharedUniformManager.h>
#include <Graphics\UniformInfo.h>
#include <Graphics\ShaderInfo.h>
GraphicsSharedUniformManager GraphicsSharedUniformManager::globalSharedUniformManager;

GraphicsSharedUniformManager::GraphicsSharedUniformManager(): globalUniforms(0) {}
void GraphicsSharedUniformManager::initialize( unsigned int numSharedUniformSlots = MAX_UNIFORM_PARAMETERS )
{
	if ( globalUniforms ) destroy();
	this->numSharedUniformSlots = numSharedUniformSlots;
	globalUniforms = new UniformInfo[numSharedUniformSlots];
}
void GraphicsSharedUniformManager::destroy()
{
	if ( !globalUniforms ) return;
	numSharedUniformSlots = 0;
	delete[] globalUniforms;
}
bool GraphicsSharedUniformManager::initialized()
{
	return globalUniforms;
}

UniformInfo* GraphicsSharedUniformManager::setSharedUniform(
	const char* name ,
	ParameterType parameterType ,
	const void* dataPointer )
{
	if ( !strlen( name ) ) return 0;

	UniformInfo* uniform = 0;

	for ( unsigned int i = 0; i < numSharedUniformSlots; ++i )
	{
		if ( !globalUniforms[i].location )
		{
			uniform = &globalUniforms[i];
			uniform->uniformName = name;
		}
		else if ( !globalUniforms[i].uniformName.compare( name ) )
		{
			uniform = &globalUniforms[i];
		}
	}
	if ( uniform )
	{
		uniform->type = parameterType;
		uniform->location = dataPointer;
	}
	
	return uniform;
}

void GraphicsSharedUniformManager::applySharedUniforms( ShaderInfo* targetShader )
{
	for ( unsigned int i = 0; i < numSharedUniformSlots; ++i )
	{
		UniformInfo* target = &globalUniforms[i];
		if (target->uniformName.size() )
		{
			targetShader->setUniformParameter( target->uniformName.c_str() , target->type , target->location );
		}
	}
}