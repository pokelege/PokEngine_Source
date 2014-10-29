#include "Graphics\RawOpenGLManager.h"
#include <GL\glew.h>
#include <SOIL.h>
#include <fstream>
#include <gtc\matrix_transform.hpp>
#include <gtc\quaternion.hpp>
#include <Graphics\VertexInfo.h>
#include <Graphics\BoneInfo.h>
#include <Graphics\AnimationInfo.h>
#include <iostream>
RawOpenGLManager::UniformInfo RawOpenGLManager::globalUniforms[MAX_UNIFORM_PARAMETERS];
RawOpenGLManager::Renderable RawOpenGLManager::renderableInfos[MAX_RENDERABLES];
RawOpenGLManager::FrameBufferInfo RawOpenGLManager::frameBufferInfos[MAX_TEXTURES];

unsigned int RawOpenGLManager::base = 0;

void RawOpenGLManager::initialize()
{
	glewInit();
}

RawOpenGLManager::FrameBufferInfo* RawOpenGLManager::addFrameBuffer()
{
	int i;
	for ( i = 0; i < MAX_TEXTURES; i++ )
	{
		if ( frameBufferInfos[i].bufferID == NULL )
		{
			glGenFramebuffers( 1 , &frameBufferInfos[i].bufferID );
			break;
		}
	}
	return &frameBufferInfos[i];
}

void RawOpenGLManager::setFrameBuffer( FrameBufferInfo* bufferID , unsigned int textureID , unsigned int attatchment )
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , bufferID->bufferID );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER , attatchment , GL_TEXTURE_2D , textureID , 0 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , 0);
}

RawOpenGLManager::Renderable* RawOpenGLManager::addRenderable(
	GeometryInfo* whatGeometry ,
	const char* whereUniform ,
	ShaderInfo* howShaders ,
	const bool& depthTestEnabled,
	TextureInfo* whatTexture,
	CullingType culling,
	const bool& alpha )
{
	int i;

	for ( i = 0; i < MAX_RENDERABLES; i++ )
	{
		if ( renderableInfos[i].whatGeometryIndex == nullptr ) break;
	}

	renderableInfos[i].whatGeometryIndex = whatGeometry;
	renderableInfos[i].howShaderIndex = howShaders;
	renderableInfos[i].textureID = whatTexture;
	renderableInfos[i].visible = true;
	renderableInfos[i].depthTestEnabled = depthTestEnabled;
	renderableInfos[i].whereUniform = std::string( whereUniform );
	renderableInfos[i].culling = culling;
	renderableInfos[i].alpha = alpha;
	return &renderableInfos[i];
}

void RawOpenGLManager::updateAnimation( Renderable& toUpdate , const float& dt )
{


void RawOpenGLManager::updateAnimationMatricesRecurse( unsigned int boneIndex , BoneInfo* bones , Renderable& toUpdate , glm::mat4& parentMatrix )
{

}

void RawOpenGLManager::drawAll()
{
	for ( int i = 0; i < MAX_RENDERABLES && renderableInfos[i].whatGeometryIndex != nullptr; i++ )
	{
		if ( renderableInfos[i].visible )
		{
			glUseProgram( renderableInfos[i].howShaderIndex->programID );
			glBindVertexArray( renderableInfos[i].whatGeometryIndex->dataArray );

			if ( renderableInfos[i].depthTestEnabled ) glEnable( GL_DEPTH_TEST );
			else glDisable( GL_DEPTH_TEST );

			if ( renderableInfos[i].alpha )
			{
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
			}
			else
			{
				glDisable( GL_BLEND );
			}

			if ( renderableInfos[i].culling == CT_NONE ) glDisable( GL_CULL_FACE );
			else
			{
				glEnable( GL_CULL_FACE );
				if ( renderableInfos[i].culling == CT_FRONT ) glCullFace( GL_FRONT );
				else if ( renderableInfos[i].culling == CT_BOTH ) glCullFace( GL_FRONT_AND_BACK );
				else glCullFace( GL_BACK );
			}

			if ( renderableInfos[i].textureID != NULL )
			{
				for ( unsigned int j = 0; j < renderableInfos[i].textureID->size; j++ )
				{
					glActiveTexture( GL_TEXTURE0 + j );
					glBindTexture( renderableInfos[i].textureID->textureIDs[j].type, renderableInfos[i].textureID->textureIDs[j].textureID );
				}
			}

			for ( int j = 0; globalUniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( renderableInfos[i].howShaderIndex , globalUniforms[j].uniformName.c_str() , globalUniforms[j].type , globalUniforms[j].location );
			}

			for ( int j = 0; renderableInfos[i].uniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( renderableInfos[i].howShaderIndex , renderableInfos[i].uniforms[j].uniformName.c_str() , renderableInfos[i].uniforms[j].type , renderableInfos[i].uniforms[j].location );
			}

			const char* st = renderableInfos[i].whereUniform.c_str();
			glm::quat quaternion = glm::rotate( glm::quat() , renderableInfos[i].rotate.x , glm::vec3( 1 , 0 , 0 ) ) *
				glm::rotate( glm::quat() , renderableInfos[i].rotate.y , glm::vec3( 0 , 1 , 0 ) ) *
				glm::rotate( glm::quat() , renderableInfos[i].rotate.z , glm::vec3( 0 , 0 , 1 ) );
			glm::mat4 transform = glm::translate( glm::mat4() , renderableInfos[i].translate ) * glm::mat4_cast( quaternion ) * glm::scale( glm::mat4() , renderableInfos[i].scale );

			setUniformParameter( renderableInfos[i].howShaderIndex , st , ParameterType::PT_MAT4 , &transform[0][0] );

			if ( renderableInfos[i].animationMatrices )
			{
				setUniformParameter( renderableInfos[i].howShaderIndex ,
									 renderableInfos[i].animationMatricesUniform.c_str() ,
									 PT_MAT4 ,
									 reinterpret_cast<const float*>( renderableInfos[i].animationMatrices ) ,
									 renderableInfos[i].sizeofAnimationMatrices );
			}

			glDrawElements( renderableInfos[i].whatGeometryIndex->indexingMode , renderableInfos[i].whatGeometryIndex->numIndex , GL_UNSIGNED_SHORT , ( void* ) renderableInfos[i].whatGeometryIndex->indexOffset );
		}
	}
}

//void RawOpenGLManager::reset()
//{
//	for ( int i = 0; i < MAX_RENDERABLES; i++ )
//	{
//		renderableInfos[i].whatGeometryIndex = nullptr;
//		renderableInfos[i].depthTestEnabled = false;
//		for ( int j = 0; j < MAX_UNIFORM_PARAMETERS; j++ ) renderableInfos[i].uniforms[j].location = nullptr;
//	}
//}