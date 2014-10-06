#pragma once
#include <QtOpenGL\QGLWidget>
#include <glm.hpp>
#include <string>
class Clock;
class Camera;
class QTimer;
class Preview :public QGLWidget
{
	Q_OBJECT;

	QTimer* timer;
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