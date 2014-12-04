#define GLM_FORCE_RADIANS
#include <Graphics\Camera.h>
#include <gtc\matrix_transform.hpp>
#include <Core\GameObject.h>
#include <Core\WindowInfo.h>
#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\CommonGraphicsCommands.h>
#include <GL\glew.h>
#include <Graphics\TextureInfo.h>
Camera::Camera() : toRender(0), numRenders(0) , parent(0) , up(glm::vec3(0 , 1 , 0)) , direction(glm::vec3(0 , 0 , -1)) , x(0) , y(0) , width(1) , height(1) , FOV(60) , nearestObject(0.01f) , frameBufferID(0) , drawCamera(true) , drawFrameBuffer(true) {}
void Camera::attatch( GameObject* parent ) { this->parent = parent; }
void Camera::detatch() { parent = 0; }

glm::mat4 Camera::worldToView() const
{
	if ( !parent ) return glm::mat4();
	return glm::lookAt( parent->getWorldTranslate() , parent->getWorldTranslate() + direction , up );
}

glm::mat4 Camera::viewToProjection() const
{
	float apect = ( width * WindowInfo::width ) / ( height * WindowInfo::height );
	return glm::infinitePerspective( glm::radians(FOV) ,apect, nearestObject );
}

glm::mat4 Camera::viewToProjectionFrameBuffer() const
{
	float apect = ((float)frameBufferWidth ) / ( (float)frameBufferHeight );
	return glm::infinitePerspective( glm::radians( FOV ) , apect , nearestObject );
}

void Camera::initializeRenderManagers( unsigned int numRenders )
{
	if ( toRender ) destroyRenderManagers();
	this->numRenders = numRenders;
	toRender = new GraphicsRenderingManager*[numRenders];
	for ( unsigned int i = 0; i < numRenders; ++i ) toRender[i] = 0;
}

void Camera::destroyRenderManagers()
{
	if ( !toRender ) return;
	delete[] toRender;
	toRender = 0;
	numRenders = 0;
}

bool Camera::addRenderList( GraphicsRenderingManager* list )
{
	for ( unsigned int i = 0; i < numRenders; ++i )
	{
		if ( !toRender[i] )
		{
			toRender[i] = list;
			return true;
		}
	}
	return false;
}

bool Camera::addLights( GraphicsLightManager* light )
{
	toLight.push_back( light ); return true;
}

void Camera::attatchFrameBuffer( TextureInfo* color , TextureInfo* depth )
{
	if ( glIsFramebuffer( frameBufferID ) ) glDeleteFramebuffers (1, (GLuint*)&frameBufferID );
	glGenFramebuffers( 1 , (GLuint*)&frameBufferID );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , frameBufferID );
	if ( color ) glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D , color->textureID, 0 );
	if ( depth ) glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_TEXTURE_2D , depth->textureID , 0 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , 0 );
}
void Camera::earlyUpdate() {}
void Camera::update() {}
void Camera::lateUpdate() {}
void Camera::earlyDraw() {}
void Camera::draw()
{
	if ( drawFrameBuffer && glIsFramebuffer( frameBufferID ))
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER , frameBufferID );
		GLint params;
		glGetFramebufferAttachmentParameteriv( GL_DRAW_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME , &params );
		if ( params <= 0 )
		{
			glGetFramebufferAttachmentParameteriv( GL_DRAW_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME , &params );
		}
		if ( params > 0 )
		{
			glBindTexture( GL_TEXTURE_2D , params );
			glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_WIDTH , &frameBufferWidth );
			glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_WIDTH , &frameBufferHeight );
			CommonGraphicsCommands::setViewPort( 0 , 0 , frameBufferWidth ,frameBufferHeight );
			CommonGraphicsCommands::clearCommon();
			for ( unsigned int i = 0; i < numRenders; ++i )
			{
				if ( toRender[i] )
				{
					toRender[i]->drawAll( *this, std::vector<GraphicsLightManager*>() , true );
				}
			}
		}

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER , 0 );
	}
	if ( drawCamera )
	{
		CommonGraphicsCommands::setViewPort( ( int ) ( WindowInfo::width * x ) , ( int ) ( WindowInfo::height * y ) , ( int ) ( WindowInfo::width * width ) , ( int ) ( WindowInfo::height * height ) );
		CommonGraphicsCommands::clearCommon();
		for ( unsigned int i = 0; i < numRenders; ++i )
		{
			if ( toRender[i] )
			{
				toRender[i]->drawAll( *this, toLight );
			}
		}
	}
}
void Camera::lateDraw() {}
Camera::~Camera() { destroyRenderManagers(); }