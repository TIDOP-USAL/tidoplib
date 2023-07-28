#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include <QOpenglWidget>

class ViewerWidget : public QOpenGLWidget 
{

public:

	ViewerWidget(QWidget* parent = nullptr) : QOpenGLWidget(parent) { }
	~ViewerWidget() = default;

private:

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

public:

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

};