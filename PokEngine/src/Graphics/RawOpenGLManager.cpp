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

void RawOpenGLManager::addShaderStreamedParameter(
	GeometryInfo* geometryID ,
	unsigned int layoutLocation ,
	ParameterType parameterType ,
	unsigned int bufferStride ,
	unsigned int bufferOffset )
{
	glBindVertexArray( geometryID->dataArray );

	glEnableVertexAttribArray( layoutLocation );

	if ( parameterType < 0 )
	{
		int pt = -parameterType / sizeof( int );
		glVertexAttribPointer( layoutLocation , pt , GL_INT , GL_FALSE , bufferStride , ( void* ) ( geometryID->vertexOffset + bufferOffset ) );
	}
	else
	{
		glVertexAttribPointer( layoutLocation , parameterType / sizeof( float ) , GL_FLOAT , GL_FALSE , bufferStride , ( void* ) ( geometryID->vertexOffset + bufferOffset ) );
	}
}

void RawOpenGLManager::setRenderableUniform(
	Renderable* object ,
	const char* name ,
	ParameterType parameterType ,
	const float* dataPointer )
{
	if ( object != nullptr )
	{
		int i = 0;

		while ( i < MAX_UNIFORM_PARAMETERS && object->uniforms[i].uniformName.compare( name ) && object->uniforms[i].location != nullptr )
		{
			i++;
		}

		UniformInfo uni;
		uni.uniformName = name;
		uni.type = parameterType;
		uni.location = dataPointer;

		object->uniforms[i] = uni;
	}
}

void RawOpenGLManager::setViewPort( int x , int y , int width , int height )
{
	glViewport( x , y , width , height );
}

void RawOpenGLManager::clear( unsigned int toClear )
{
	glClear( toClear );
}

void RawOpenGLManager::drawSpecific( Renderable* toDraw )
{
	if ( toDraw != nullptr )
	{
		if ( toDraw->visible )
		{
			glUseProgram( toDraw->howShaderIndex->programID );
			glBindVertexArray( toDraw->whatGeometryIndex->dataArray );

			if ( toDraw->depthTestEnabled ) glEnable( GL_DEPTH_TEST );
			else glDisable( GL_DEPTH_TEST );

			if ( toDraw->alpha )
			{
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
			}
			else
			{
				glDisable(GL_BLEND);
			}

			if ( toDraw->culling == CT_NONE ) glDisable( GL_CULL_FACE );
			else
			{
				glEnable( GL_CULL_FACE );
				if ( toDraw->culling == CT_FRONT ) glCullFace( GL_FRONT );
				else if ( toDraw->culling == CT_BOTH ) glCullFace( GL_FRONT_AND_BACK );
				else glCullFace( GL_BACK );
			}

			if ( toDraw->textureID != NULL )
			{
				for ( unsigned int i = 0; i < toDraw->textureID->size; i++ )
				{
					glActiveTexture( GL_TEXTURE0 + i );
					glBindTexture( toDraw->textureID->textureIDs[i].type , toDraw->textureID->textureIDs[i].textureID );
				}
			}

			for ( int j = 0; globalUniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( toDraw->howShaderIndex , globalUniforms[j].uniformName.c_str() , globalUniforms[j].type , globalUniforms[j].location );
			}

			for ( int j = 0; toDraw->uniforms[j].location != nullptr; j++ )
			{
				setUniformParameter( toDraw->howShaderIndex , toDraw->uniforms[j].uniformName.c_str() , toDraw->uniforms[j].type , toDraw->uniforms[j].location );
			}

			const char* st = toDraw->whereUniform.c_str();
			glm::quat quaternion = glm::rotate(glm::quat(), toDraw->rotate.x, glm::vec3(1,0,0)) *
				glm::rotate( glm::quat() , toDraw->rotate.y , glm::vec3( 0 , 1 , 0 ) ) *
				glm::rotate( glm::quat() , toDraw->rotate.z , glm::vec3( 0 , 0 , 1 ) );
			glm::mat4 transform = glm::translate(glm::mat4() , toDraw->translate) * glm::mat4_cast(quaternion) * glm::scale(glm::mat4(), toDraw->scale);
			setUniformParameter( toDraw->howShaderIndex , st , ParameterType::PT_MAT4 , &transform[0][0] );
			if ( toDraw->animationMatrices )
			{
				setUniformParameter( toDraw->howShaderIndex ,
									 toDraw->animationMatricesUniform.c_str() ,
									 PT_MAT4 ,
									 reinterpret_cast<const float*>(toDraw->animationMatrices),
									 toDraw->sizeofAnimationMatrices);
			}
			glDrawElements( toDraw->whatGeometryIndex->indexingMode , toDraw->whatGeometryIndex->numIndex , GL_UNSIGNED_SHORT , ( void* ) toDraw->whatGeometryIndex->indexOffset );
		}
	}
}

void RawOpenGLManager::updateAnimation( Renderable& toUpdate , const float& dt )
{
	unsigned int boneDataSize;
	BoneInfo* bones = toUpdate.whatGeometryIndex->modelData.getBoneData( &boneDataSize );
	if ( bones )
	{
		if ( !toUpdate.animationMatrices )
		{
			toUpdate.animationMatrices = new glm::mat4[boneDataSize];
			toUpdate.sizeofAnimationMatrices = boneDataSize;
		}
		toUpdate.currentFrame += toUpdate.animationFrameRate * dt;
		glm::mat4 parent;
		updateAnimationMatricesRecurse( 0, bones , toUpdate , parent );
	}
}

void RawOpenGLManager::updateAnimationMatricesRecurse( unsigned int boneIndex , BoneInfo* bones , Renderable& toUpdate , glm::mat4& parentMatrix )
{
	glm::mat4 animateTransform;
	if ( bones[boneIndex].animationSize() > 0 )
	{
		AnimationInfo* start = 0;
		AnimationInfo* end = 0;

		unsigned int animationEndTime = bones[boneIndex].getAnimation( bones[boneIndex].animationSize() - 1 , toUpdate.whatGeometryIndex->modelData )->frame;
		while ( toUpdate.currentFrame > animationEndTime ) toUpdate.currentFrame -= animationEndTime;
		for ( unsigned int i = 0; i < bones[boneIndex].animationSize(); ++i )
		{
			AnimationInfo* test = bones[boneIndex].getAnimation( i , toUpdate.whatGeometryIndex->modelData );
			if ( test->frame <= ( unsigned int ) toUpdate.currentFrame )
			{
				start = test;
			}
			if ( test->frame >= toUpdate.currentFrame )
			{
				end = test;
				break;
			}
		}
		float interpolation = toUpdate.currentFrame;
		glm::vec3 lerpedTranslate;
		glm::vec3 lerpedScale;
		glm::vec3 lerpedRotation;
		if ( start )
		{
			interpolation = ( interpolation - start->frame ) / ( end->frame - start->frame );
			lerpedTranslate = ( ( 1 - interpolation ) * start->translation ) + ( interpolation * end->translation );
			lerpedScale = ( ( 1 - interpolation ) * start->scale ) + ( interpolation * end->scale );
			lerpedRotation = ( ( 1 - interpolation ) * start->rotation ) + ( interpolation * end->rotation );
		}
		else
		{
			interpolation /= end->frame;
			lerpedTranslate = ( interpolation * end->translation );
			lerpedScale = ( ( 1 - interpolation ) * glm::vec3(1,1,1) ) + ( interpolation * end->scale );
			lerpedRotation = ( interpolation * end->rotation );
		}

		glm::quat quaternion = glm::rotate( glm::quat() , lerpedRotation.x , glm::vec3( 1 , 0 , 0 ) ) *
			glm::rotate( glm::quat() , lerpedRotation.y , glm::vec3( 0 , 1 , 0 ) ) *
			glm::rotate( glm::quat() , lerpedRotation.z , glm::vec3( 0 , 0 , 1 ) );

		animateTransform = parentMatrix * ( glm::translate( glm::mat4(), lerpedTranslate ) * glm::mat4_cast(quaternion) * glm::scale( glm::mat4(), lerpedScale ) );
		toUpdate.animationMatrices[boneIndex] = animateTransform * bones[boneIndex].offsetMatrix;
	}
	else
	{
		animateTransform = parentMatrix;
		toUpdate.animationMatrices[boneIndex] = animateTransform * bones[boneIndex].offsetMatrix;
	}

	for ( unsigned int i = 0; i < bones[boneIndex].childrenSize(); ++i )
	{
		updateAnimationMatricesRecurse( toUpdate.whatGeometryIndex->modelData.getBoneChildren()[bones[boneIndex].childDataStart + i], bones, toUpdate, animateTransform );
	}
}

void RawOpenGLManager::enable( unsigned int toEnable)
{
	glEnable( toEnable );
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

void RawOpenGLManager::reset()
{
	for ( int i = 0; i < MAX_RENDERABLES; i++ )
	{
		renderableInfos[i].whatGeometryIndex = nullptr;
		renderableInfos[i].depthTestEnabled = false;
		for ( int j = 0; j < MAX_UNIFORM_PARAMETERS; j++ ) renderableInfos[i].uniforms[j].location = nullptr;
	}
}

void RawOpenGLManager::initializePrinting( HDC hdc, int sizex, int sizey, int fontWeight )
{
	if ( glIsList( base ) ) glDeleteLists( base , 96 );
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists( 96 );								// Storage For 96 Characters

	font = CreateFont( -sizex ,							// Height Of Font
					   sizey ,								// Width Of Font
					   0 ,								// Angle Of Escapement
					   0 ,								// Orientation Angle
					   fontWeight ,						// Font Weight
					   FALSE ,							// Italic
					   FALSE ,							// Underline
					   FALSE ,							// Strikeout
					   ANSI_CHARSET ,					// Character Set Identifier
					   OUT_TT_PRECIS ,					// Output Precision
					   CLIP_DEFAULT_PRECIS ,			// Clipping Precision
					   ANTIALIASED_QUALITY ,			// Output Quality
					   FF_DONTCARE | DEFAULT_PITCH ,		// Family And Pitch
					   "Courier New" );					// Font Name

	oldfont = ( HFONT ) SelectObject( hdc , font );           // Selects The Font We Want
	wglUseFontBitmaps( hdc , 32 , 96 , base );				// Builds 96 Characters Starting At Character 32
	SelectObject( hdc , oldfont );							// Selects The Font We Want
	DeleteObject( font );
}

void RawOpenGLManager::printString( const char* string , const float& x , const float& y , glm::vec4 color )
{
	if ( glIsList( base ) )
	{
		glUseProgram( 0 );
		glRasterPos2f( x , y );
		glColor4f( color.x , color.y , color.z , color.w );
		glPushAttrib( GL_LIST_BIT );
		glListBase( base - 32 );
		glCallLists( strlen( string ) , GL_UNSIGNED_BYTE , string );
		glPopAttrib();
	}
}