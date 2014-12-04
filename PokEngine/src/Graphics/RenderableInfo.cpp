#include <Graphics\RenderableInfo.h>
#include <Graphics\UniformInfo.h>
#include <Graphics\ShaderInfo.h>
#include <Graphics\GeometryInfo.h>
#include <Graphics\TextureInfo.h>
#include <Graphics\GraphicsSharedUniformManager.h>
#include <GL\glew.h>
#include <Core\GameObject.h>
#include <Graphics\CommonUniformNames.h>
#include <Graphics\GraphicsLightManager.h>
#include <Graphics\GraphicsLightManager.h>
RenderableInfo::RenderableInfo() :uniforms( 0 ), numUniformSlots(0), textures( 0 ), numTextureSlots(0), sharedUniforms (0), geometryInfo(0), shaderInfo(0), parent(0), slotUsed(false), culling(CT_NONE), frameBufferCulling(CT_FRONT), depthTestEnabled(true), alphaBlendingEnabled(false) {}

RenderableInfo::~RenderableInfo()
{
	if ( uniforms ) delete[] uniforms;
	if ( textures ) delete[] textures;
}

void RenderableInfo::attatch( GameObject* parent )
{
	this->parent = parent;
}
void RenderableInfo::detatch()
{
	parent = 0;
}

void RenderableInfo::initialize( unsigned int numUniformSlots , unsigned int numTextureSlots )
{
	if ( textures || uniforms ) destroy();
	this->numUniformSlots = numUniformSlots;
	uniforms = new UniformInfo[numUniformSlots];
	this->numTextureSlots = numTextureSlots;
	textures = new TextureInfo*[numTextureSlots]();
	for ( unsigned int i = 0; i < numTextureSlots; ++i )
	{
		textures[i] = 0;
	}
}
void RenderableInfo::destroy()
{
	if ( uniforms )
	{
		numUniformSlots = 0;
		delete[] uniforms;
	}
	if ( textures )
	{
		delete[] textures;
		textures = 0;
		numTextureSlots = 0;
	}
}

UniformInfo* RenderableInfo::setRenderableUniform(
	const char* name ,
	ParameterType parameterType ,
	const void* dataPointer,
	unsigned int size)
{
	if ( !strlen( name ) || !uniforms ) return 0;

	UniformInfo* uniform = 0;

	for ( unsigned int i = 0; i < numUniformSlots; ++i )
	{
		if ( !uniforms[i].location )
		{
			uniform = &uniforms[i];
			uniform->uniformName = name;
		}
		else if ( !uniforms[i].uniformName.compare( name ) )
		{
			uniform = &uniforms[i];
		}
	}
	if ( uniform )
	{
		uniform->type = parameterType;
		uniform->location = dataPointer;
		uniform->size = size;
	}

	return uniform;
}

bool RenderableInfo::addTexture( TextureInfo* texture )
{
	bool toReturn = false;
	if ( !texture || !textures ) return toReturn;
	for ( unsigned int i = 0; i < numTextureSlots && !toReturn; ++i )
	{
		if ( !textures[i] )
		{
			textures[i] = texture;
			toReturn = true;
		}
		else if ( textures[i] == texture ) toReturn = true;
	}
	return toReturn;
}

bool RenderableInfo::swapTexture( TextureInfo* texture , const unsigned int& index , TextureInfo** receivingTexture)
{
	bool toReturn = false;
	if ( !texture || !textures || index < 0 || index >= numTextureSlots ) return toReturn;
	if ( receivingTexture ) *receivingTexture = textures[index];
	textures[index] = texture;
	toReturn = true;
	return toReturn;
}

void RenderableInfo::draw( const std::vector<GraphicsLightManager*>& lights, const bool& isFrameBuffer )
{
	if ( !parent ) return;
	if ( shaderInfo ) glUseProgram( shaderInfo->programID );

	if(geometryInfo) glBindVertexArray( geometryInfo->dataArray );

	if ( depthTestEnabled ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	if ( alphaBlendingEnabled )
	{
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	}
	else
	{
		glDisable( GL_BLEND );
	}

	if ( ( culling == CT_NONE && !isFrameBuffer ) || ( frameBufferCulling == CT_NONE && isFrameBuffer ) ) glDisable( GL_CULL_FACE );
	else
	{
		glEnable( GL_CULL_FACE );
		if ( ( culling == CT_FRONT && !isFrameBuffer ) || ( frameBufferCulling == CT_FRONT && isFrameBuffer ) ) glCullFace( GL_FRONT );
		else if ( ( culling == CT_BOTH && !isFrameBuffer ) || ( frameBufferCulling == CT_BOTH && isFrameBuffer ) ) glCullFace( GL_FRONT_AND_BACK );
		else glCullFace( GL_BACK );
	}

	for each ( GraphicsLightManager* i in lights )
	{
		if ( i ) i->bindDepthTextures();
	}

	if ( textures != NULL )
	{
		for ( unsigned int i = 0; i < numTextureSlots; ++i )
		{
			if ( textures[i] )
			{
				glActiveTexture( GL_TEXTURE0 + textures[i]->textureSlot );
				glBindTexture( textures[i]->type , textures[i]->textureID );
			}
		}
	}

	if ( shaderInfo )
	{
		glm::mat4 modelToWorld = parent->getWorldTransform();
		shaderInfo->setUniformParameter( MODELTOWORLD , PT_MAT4 , reinterpret_cast< const void* >( &modelToWorld ) );
		if ( sharedUniforms ) sharedUniforms->applySharedUniforms( shaderInfo );
		else GraphicsSharedUniformManager::globalSharedUniformManager.applySharedUniforms( shaderInfo );


		for ( unsigned int i = 0; i < numUniformSlots; ++i )
		{
			shaderInfo->setUniformParameter( uniforms[i].uniformName.c_str() , uniforms[i].type , uniforms[i].location, uniforms[i].size );
		}
	}
	if(geometryInfo) glDrawElements( geometryInfo->indexingMode , geometryInfo->numIndex , GL_UNSIGNED_SHORT , ( void* ) geometryInfo->indexOffset );
}