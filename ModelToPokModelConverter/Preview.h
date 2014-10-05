#pragma once
#include <QtOpenGL\QGLWidget>
#include <glm.hpp>
#include <Graphics\RawOpenGLManager.h>
#include <string>
class Clock;
class Light;
class Camera;
class QTimer;
class Preview :public QGLWidget
{
	Q_OBJECT;

	glm::mat4 perp, view;
	float specularity;
	QTimer* timer;
	Clock* clock;
	Light* light;
	Camera* camera;
	RawOpenGLManager::Renderable* renderable;
	std::string fileName;
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent( QMouseEvent* e );
public:
	Preview( std::string fileName );
	public slots:
	void update();
};