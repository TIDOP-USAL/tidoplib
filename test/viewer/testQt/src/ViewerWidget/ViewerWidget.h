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

public:

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
};