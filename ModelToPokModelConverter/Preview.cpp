#include "Preview.h"
#include <Misc\FileReader.h>
#include <Graphics\VertexInfo.h>
#include <iostream>
#include <Misc\Clock.h>
#include <Qt\qtimer.h>
#include <Graphics\CommonGraphicsCommands.h>
#include <Graphics\GraphicsShaderManager.h>
#include <Graphics\GraphicsGeometryManager.h>
#include <Graphics\GraphicsBufferManager.h>
#include <Graphics\GeometryInfo.h>
#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\RenderableInfo.h>
#include <Graphics\GraphicsSharedUniformManager.h>
#include <Core\GameObjectManger.h>
#include <Core\GameObject.h>
#include <Graphics\AnimationRenderingInfo.h>
#include <Graphics\GraphicsCameraManager.h>
#include <Graphics\Camera.h>
#include <Input\FirstPersonCameraInput.h>
#include <Input\MouseInput.h>
#include <Core\WindowInfo.h>
Preview::Preview( std::string fileName ) :fileName( fileName )
{

}

void Preview::initializeGL()
{
	CommonGraphicsCommands::initializeGlobalGraphics();
	GameObjectManager::globalGameObjectManager.initialize();
	std::string err;
	ShaderInfo* shader = GraphicsShaderManager::globalShaderManager.createShaderInfo( FileReader( "Shaders/VertexShader.glsl" ).c_str() , FileReader( "Shaders/FragmentShader.glsl" ).c_str() , &err );
	std::cout << err.c_str() << std::endl;

	GeometryInfo* theModel = GraphicsGeometryManager::globalGeometryManager.addPMDGeometry( fileName.c_str(), GraphicsBufferManager::globalBufferManager);
	theModel->addShaderStreamedParameter( 0 , PT_VEC3 , VertexInfo::STRIDE , VertexInfo::POSITION_OFFSET );
	theModel->addShaderStreamedParameter( 1 , PT_VEC4 , 0 , 0 );
	theModel->addShaderStreamedParameter( 3 , PT_VEC2 , VertexInfo::STRIDE , VertexInfo::UV_OFFSET );
	theModel->addShaderStreamedParameter( 2 , PT_VEC3 , VertexInfo::STRIDE , VertexInfo::NORMAL_OFFSET );
	theModel->addShaderStreamedParameter( 4 , PT_VEC3 , VertexInfo::STRIDE , VertexInfo::TANGENT_OFFSET );
	theModel->addShaderStreamedParameter( 5 , PT_VEC3 , VertexInfo::STRIDE , VertexInfo::BITANGENT_OFFSET );
	theModel->addShaderStreamedParameter( 6 , PT_VEC4 , VertexInfo::STRIDE , VertexInfo::BLENDINGINDEX_OFFSET );
	theModel->addShaderStreamedParameter( 7 , PT_VEC4 , VertexInfo::STRIDE , VertexInfo::BLENDINGWEIGHT_OFFSET );

	Renderable* renderable = GraphicsRenderingManager::globalRenderingManager.addRenderable();
	renderable->geometryInfo = theModel;
	renderable->shaderInfo = shader;
	renderable->culling = CT_NONE;
	renderable->depthTestEnabled = true;
	renderable->sharedUniforms = &GraphicsSharedUniformManager::globalSharedUniformManager;
	renderable->visible = true;
	renderable->initialize( 10 , 0 );

	GameObject* previewModel = GameObjectManager::globalGameObjectManager.addGameObject();
	previewModel->addComponent( renderable );
	
	AnimationRenderingInfo* animation = new AnimationRenderingInfo;
	previewModel->addComponent( animation );

	Camera* camera = GraphicsCameraManager::globalCameraManager.addCamera();
	camera->direction = glm::normalize( glm::vec3( -1 , 0 , -1 ) );
	camera->initializeRenderManagers();
	camera->nearestObject = 0.01f;
	camera->addRenderList( &GraphicsRenderingManager::globalRenderingManager );
	FirstPersonCameraInput* fpsInput = new FirstPersonCameraInput;
	fpsInput->moveSensitivity = 1;
	fpsInput->rotationSensitivity = 0.1f;
	GameObject* cameraMan = GameObjectManager::globalGameObjectManager.addGameObject();
	cameraMan->translate = glm::vec3( 6 , 5 , 15 );
	cameraMan->addComponent( camera );
	cameraMan->addComponent( fpsInput );

	MouseInput::globalMouseInput.updateOldMousePosition = false;

	setMouseTracking( true );
	QCursor c = cursor();
	c.setPos( mapToGlobal( QPoint( width() / 2 , height() / 2 ) ) );
	c.setShape( Qt::BlankCursor );
	setCursor( c );
	MouseInput::globalMouseInput.updateMousePosition( glm::vec2( width() / 2 , height() / 2 ) );
	timer = new QTimer();
	connect( timer , SIGNAL( timeout() ) , this , SLOT( update() ) );
	timer->start( 0 );
}

void Preview::mouseMoveEvent( QMouseEvent* e )
{
	QCursor c = cursor();
	QPoint mapFromGlobal = this->mapFromGlobal( c.pos() );
	if ( GetAsyncKeyState( VK_LBUTTON ) < 0 && mapFromGlobal.x() >= 0 && mapFromGlobal.x() <= width() &&
		 mapFromGlobal.y() >= 0 && mapFromGlobal.y() <= height() && MouseInput::globalMouseInput.oldMousePosition != glm::vec2( c.pos().x() , c.pos().y() ) )
	{
		if ( !MouseInput::globalMouseInput.getDeltaTracking() )MouseInput::globalMouseInput.setDeltaTracking( true );
		glm::vec2 oldPos = MouseInput::globalMouseInput.oldMousePosition;
		MouseInput::globalMouseInput.updateMousePosition( glm::vec2( c.pos().x() , c.pos().y() ) );
		
		c.setPos( QPoint( oldPos.x , oldPos.y ) );
		c.setShape( Qt::BlankCursor );
	}
	else if ( MouseInput::globalMouseInput.oldMousePosition != glm::vec2( c.pos().x() , c.pos().y() ) )
	{
		c.setShape( Qt::ArrowCursor );
		if ( MouseInput::globalMouseInput.getDeltaTracking() )MouseInput::globalMouseInput.setDeltaTracking( false );
		MouseInput::globalMouseInput.oldMousePosition = glm::vec2( c.pos().x() , c.pos().y() );
		MouseInput::globalMouseInput.updateMousePosition( glm::vec2( c.pos().x() , c.pos().y() ) );
	}
	clearFocus();
	setCursor( c );
	setFocus();
}

void Preview::update()
{
	WindowInfo::width = width();
	WindowInfo::height = height();
	Clock::update();
	
	GameObjectManager::globalGameObjectManager.earlyUpdateParents();
	GameObjectManager::globalGameObjectManager.updateParents();
	GameObjectManager::globalGameObjectManager.lateUpdateParents();
	MouseInput::globalMouseInput.mouseDelta = glm::vec2();
	repaint();
}

void Preview::paintGL()
{
	GraphicsCameraManager::globalCameraManager.drawAllCameras();
}