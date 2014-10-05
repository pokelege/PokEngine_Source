#include "Preview.h"
#include <Misc\FileReader.h>
#include <Graphics\VertexInfo.h>
#include <iostream>
#include <Misc\Clock.h>
#include <Objects\Light.h>
#include <Objects\Camera.h>
#include <gtc\matrix_transform.hpp>
#include <Qt\qtimer.h>
Preview::Preview(std::string fileName) :fileName(fileName)
{
	
}

void Preview::initializeGL()
{
	RawOpenGLManager::initialize();
	std::string err;
	RawOpenGLManager::ShaderInfo* shader = RawOpenGLManager::createShaderInfo( FileReader( "Shaders/VertexShader.glsl" ).c_str() , FileReader( "Shaders/FragmentShader.glsl" ).c_str() , &err );
	std::cout << err.c_str() << std::endl;

	RawOpenGLManager::GeometryInfo* theModel = RawOpenGLManager::addFileGeometry( fileName.c_str() );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 0 , RawOpenGLManager::ParameterType::PT_VEC3 , VertexInfo::STRIDE , VertexInfo::POSITION_OFFSET );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 1 , RawOpenGLManager::ParameterType::PT_VEC4 , 0 , 0 );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 3 , RawOpenGLManager::ParameterType::PT_VEC2 , VertexInfo::STRIDE , VertexInfo::UV_OFFSET );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 2 , RawOpenGLManager::ParameterType::PT_VEC3 , VertexInfo::STRIDE , VertexInfo::NORMAL_OFFSET );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 4 , RawOpenGLManager::ParameterType::PT_VEC3 , VertexInfo::STRIDE , VertexInfo::TANGENT_OFFSET );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 5 , RawOpenGLManager::ParameterType::PT_VEC3 , VertexInfo::STRIDE , VertexInfo::BITANGENT_OFFSET );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 6 , RawOpenGLManager::PT_VEC4 , VertexInfo::STRIDE , VertexInfo::BLENDINGINDEX_OFFSET );
	RawOpenGLManager::addShaderStreamedParameter( theModel , 7 , RawOpenGLManager::PT_VEC4 , VertexInfo::STRIDE , VertexInfo::BLENDINGWEIGHT_OFFSET );

	renderable = RawOpenGLManager::addRenderable( theModel , "modelToWorld" , shader , true );

	renderable->animationMatricesUniform += "animation";
	renderable->animationFrameRate = 30;
	camera = new Camera;
	camera->setPosition( glm::vec3( 6 , 5 , 15 ) );
	camera->setDirection( glm::normalize( glm::vec3( -1 , 0 , -1 ) ) );
	light = new Light( glm::vec3( 0 , 10 , -10 ) , glm::vec4( 1 , 1 , 1 , 1 ) );
	specularity = 20.0f;
	RawOpenGLManager::setGlobalUniform( "diffuseLightColor" , RawOpenGLManager::PT_VEC4 , reinterpret_cast<const float*>( &light->getColor() ) );
	RawOpenGLManager::setGlobalUniform( "diffuseLightPosition" , RawOpenGLManager::PT_VEC3 , reinterpret_cast<const float*>( &light->getPosition() ) );
	RawOpenGLManager::setGlobalUniform( "eyePosition" , RawOpenGLManager::PT_VEC3 , reinterpret_cast<const float*>( &camera->getPosition() ) );
	RawOpenGLManager::setGlobalUniform( "specularIntense" , RawOpenGLManager::PT_FLOAT , reinterpret_cast<const float*>( &specularity ) );
	RawOpenGLManager::setGlobalUniform( "viewToProjection" , RawOpenGLManager::PT_MAT4 , reinterpret_cast< const float* >( &perp ) );
	RawOpenGLManager::setGlobalUniform( "worldToView" , RawOpenGLManager::PT_MAT4 , reinterpret_cast< const float* >( &view ) );

	setMouseTracking( true );
	QCursor c = cursor();
	c.setPos( mapToGlobal( QPoint( width() / 2 , height() / 2 ) ) );
	c.setShape( Qt::BlankCursor );
	setCursor( c );
	camera->setOldMousePosition( glm::vec2( width() / 2 , height() / 2 ) );
	clock = new Clock;
	clock->Start();
	timer = new QTimer();
	connect( timer , SIGNAL( timeout() ) , this , SLOT( update() ) );
	timer->start( 0 );
}

void Preview::mouseMoveEvent( QMouseEvent* e )
{
	QCursor c = cursor();
	QPoint mapFromGlobal = this->mapFromGlobal( c.pos() );
	if ( GetAsyncKeyState( VK_LBUTTON ) < 0 && mapFromGlobal.x() >= 0 && mapFromGlobal.x() <= width() &&
		 mapFromGlobal.y() >= 0 && mapFromGlobal.y() <= height() )
	{
		camera->update( glm::vec2( c.pos().x() , c.pos().y() ) );

		c.setPos( QPoint( camera->getOldMousePosition().x , camera->getOldMousePosition().y ) );
		c.setShape( Qt::BlankCursor );
	}
	else
	{
		c.setShape( Qt::ArrowCursor );
		camera->setOldMousePosition( glm::vec2( c.pos().x() , c.pos().y() ) );
	}
	setCursor( c );
}

void Preview::update()
{
	if ( GetAsyncKeyState( 'W' ) )
	{
		camera->moveForward();
	}

	if ( GetAsyncKeyState( 'S' ) )
	{
		camera->moveBack();
	}

	if ( GetAsyncKeyState( 'A' ) )
	{
		camera->left();
	}

	if ( GetAsyncKeyState( 'D' ) )
	{
		camera->right();
	}

	if ( GetAsyncKeyState( 'R' ) )
	{
		camera->moveUp();
	}

	if ( GetAsyncKeyState( 'F' ) )
	{
		camera->moveDown();
	}

	RawOpenGLManager::setViewPort( 0 , 0 , width() , height() );
	RawOpenGLManager::enable( GL_DEPTH_TEST );
	RawOpenGLManager::clear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	float dt = clock->Interval();
	RawOpenGLManager::updateAnimation( *renderable , dt );
	perp = glm::infinitePerspective( 60.0f , ( float ) width() / height() , 0.001f );
	view = camera->worldToView();
	repaint();
}

void Preview::paintGL()
{
	RawOpenGLManager::drawAll();
}