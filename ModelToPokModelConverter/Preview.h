#pragma once
#include <QtOpenGL\QGLWidget>
#include <glm.hpp>
#include <string>
#include <Graphics\RenderableInfo.h>
class Clock;
class Camera;
class QTimer;
struct TextureInfo;
class FirstPersonCameraInput;
struct AnimationRenderingInfo;
class Preview :public QGLWidget
{
	Q_OBJECT;

	QTimer* timer;
	
	TextureInfo* colorTexture;
	FirstPersonCameraInput* fpsInput;
	AnimationRenderingInfo* animation;
	std::string fileName;
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent( QMouseEvent* e );
public:
	Renderable* renderable;
	void setTexture( const char* fileName );
	void setModel( std::string fileName );
	~Preview();
	public slots:
	void update();
};