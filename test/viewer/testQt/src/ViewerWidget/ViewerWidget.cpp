#include "ViewerWidget.h"

#include <iostream>
#include <vector>

#include <QMouseEvent>
#include <QWheelEvent>

#include "tidop/viewer/group/PointCloud.h"
#include <tidop/viewer/io/ASCIIReader.h>

// Draw mesh
/*
vertexArray->bind();

if (!vertexBuffer->hasIndexBuffer()) glDrawArrays(GL_TRIANGLES, 0, vertices.size());
else    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

vertexArray->unbind();
*/

namespace tl
{

ViewerWidget::ViewerWidget(QWidget* parent)
    : QOpenGLWidget(parent), button(0), mousePressed(false),
    first(false),
    renderer(Renderer::New(width(), height())) {
}

void ViewerWidget::initializeGL()
{
    if (glewInit() != GLEW_OK)
        Message::error("Couldn't initialize GLEW");

    renderer->init();
}

void ViewerWidget::resizeGL(int w, int h)
{
    renderer->resize(w, h);
}

void ViewerWidget::paintGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer->render();

    // Update -> WARNING: No FPS limit
    update();
}

void ViewerWidget::mousePressEvent(QMouseEvent* event)
{
    mousePressed = true;
    renderer->setPreviousMouse(Vector2i({ event->x(), event->y() }));
    button = event->button();
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    mousePressed = false;
}

void ViewerWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (mousePressed && button == Qt::LeftButton)
        renderer->rotate(event->x(), event->y());
    else if (mousePressed && button == Qt::RightButton)
        renderer->pan(event->x(), event->y());
}

void ViewerWidget::wheelEvent(QWheelEvent* event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        renderer->zoom(numPixels.y());
    }
    else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        renderer->zoom(numSteps.y());
    }

    event->accept();
}

}