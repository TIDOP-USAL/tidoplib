#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include <QOpenglWidget>

#include "tidop/viewer/renderer/Renderer.h"
#include "tidop/viewer/raycasting/Picker.h"

namespace tl
{

class ViewerWidget : public QOpenGLWidget
{
protected:

	int button;
	bool mousePressed, first;
	
	Renderer::Ptr renderer;
	Picker::Listener pickerListener;
	Picker::Ptr picker;
	bool pickerEnabled;

	PointCloud::Ptr rayModelBase;

public:

	ViewerWidget(QWidget* parent = nullptr);
	~ViewerWidget() = default;

private:

	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

public:

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void setPickerListener(const Picker::Listener& listener) { pickerListener = listener; }
	void enablePicker(bool enable) { pickerEnabled = enable; }

	Renderer::Ptr getRenderer() { return renderer; }

	PointCloud::Ptr getRayModelBase() { return rayModelBase; }
};

}