#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include <QOpenglWidget>

#include "tidop/viewer/renderer/Renderer.h"

namespace tl
{

class ViewerWidget : public QOpenGLWidget
{
protected:

	int button;
	bool mousePressed, first;
	
	Renderer::Ptr renderer;

public:

	ViewerWidget(QWidget* parent = nullptr);
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

	Renderer::Ptr getRenderer() { return renderer; }
};

}