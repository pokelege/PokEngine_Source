#include <Graphics\GraphicsLightManager.h>
#include <Graphics\Light.h>
#include <Graphics\RenderableInfo.h>
#include <Graphics\GraphicsSharedUniformManager.h>
#include <Graphics\CommonUniformNames.h>
#include <Core\GameObject.h>
#include <Graphics\Camera.h>
#include <Graphics\TextureInfo.h>
#include <GL\glew.h>
GraphicsLightManager GraphicsLightManager::global;

GraphicsLightManager::GraphicsLightManager() : lights(0)
{

}

void GraphicsLightManager::initialize( unsigned int numLightSlots )
{
	if ( lights ) destroy();
	this->numLightSlots = numLightSlots;
	lights = new Light[numLightSlots];
}
void GraphicsLightManager::destroy()
{
	if ( lights )
	{
		numLightSlots = 0;
		delete[] lights;
		lights = 0;
	}
}
bool GraphicsLightManager::initialized()
{
	return lights != 0;
}
Light* GraphicsLightManager::addLight()
{
	if ( !lights ) return 0;
	for ( unsigned int i = 0; i < numLightSlots; ++i )
	{
		if ( !lights[i].parent ) return &lights[i];
	}
	return 0;
}
void GraphicsLightManager::applyLights( GraphicsSharedUniformManager* uniformManager )
{
	if ( !( uniformManager || lights ) ) return;
	lightColor.clear();
	lightPosition.clear();
	lightPerspective.clear();
	lightCameraMatrix.clear();
	depthTextureSlots.clear();
	for ( unsigned int i = 0; i < numLightSlots; ++i )
	{
		if ( !lights[i].parent ) continue;
		lightColor.push_back( lights[i].getColor() );
		lightPosition.push_back( lights[i].parent->getWorldTranslate() );
		if ( TextureInfo* tex = lights[i].getDepthTexture() ) depthTextureSlots.push_back( tex->textureSlot );
		Camera* theCam = lights[i].parent->getComponent<Camera>( );
		if ( !theCam ) continue;
		lightPerspective.push_back( theCam->viewToProjectionFrameBuffer() );
		lightCameraMatrix.push_back( theCam->worldToView() );
	}
	if ( !( lightColor.size() || lightPosition.size() ) ) return;
	uniformManager->setSharedUniform( LIGHTCOLOR , PT_VEC4 , &lightColor[0] , lightColor.size() );
	uniformManager->setSharedUniform( LIGHTPOSITION , PT_VEC3 , &lightPosition[0] , lightPosition.size() );
	uniformManager->setSharedUniform( LIGHTPROJECTION , PT_MAT4 , &lightPerspective[0] , lightPerspective.size() );
	uniformManager->setSharedUniform( LIGHTVIEW , PT_MAT4 , &lightCameraMatrix[0] , lightCameraMatrix.size() );
	uniformManager->setSharedUniform( SHADOWTEXTURE , PT_INT , &depthTextureSlots[0] , depthTextureSlots.size() );
}

void GraphicsLightManager::bindDepthTextures()
{
	for ( unsigned int i = 0; i < numLightSlots; ++i )
	{
		if ( TextureInfo* tex = lights[i].getDepthTexture() )
		{
			glActiveTexture( GL_TEXTURE0 + tex->textureSlot );
			glBindTexture( tex->type , tex->textureID );
		}
	}
}