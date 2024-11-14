#include "ViewerWidget.h"

#include <iostream>
#include <vector>

#include <QMouseEvent>
#include <QWheelEvent>

#include "tidop/viewer/group/PointCloud.h"
#include <tidop/viewer/io/ASCIIReader.h>
#include "tidop/viewer/group/Grid.h"
#include "tidop/viewer/group/Mesh.h"
#include "tidop/viewer/texture/Texture.h"


namespace tl
{

bool ViewerWidget::switchAxis = false;

ViewerWidget::ViewerWidget(QWidget* parent)
    : QOpenGLWidget(parent), button(0), mousePressed(false),
    first(false),
    renderer(Renderer::New(width(), height())),
	pickerEnabled(false) {
    pickerListener = [&](const Picker::Result& result) {};
}

void ViewerWidget::initializeGL()
{
    if (glewInit() != GLEW_OK)
        Message::error("Couldn't initialize GLEW");

    renderer->init();

    // Picker
    picker = Picker::New(renderer->getCamera(), width(), height(), 1.0f);
    picker->setListener(pickerListener);

    // Grid
    Grid::Ptr grid = Grid::New(Vector3f::zero(), Size<int>(400, 400), 0.5f);
    grid->scale(10, 10, 10);
    grid->setLineSize(1.5);
    renderer->addModel(grid);

    // Cube test
    std::vector<Vertex> cubeVertices =
    {
		 Vertex(    Vector3f{-0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, 0.0f, -1.0f},   Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f, -1.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f},  Vector3f{  0.0f,  0.0f, -1.0f}, Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f, -1.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f, -1.0f},  Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f, -1.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f,  1.0f},  Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f,  1.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f,  1.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f,  1.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f,  1.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  0.0f,  1.0f},  Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{ - 1.0f,  0.0f, 0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{ - 1.0f,  0.0f, 0.0f},  Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{ - 1.0f,  0.0f, 0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{ - 1.0f,  0.0f, 0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{ - 1.0f,  0.0f, 0.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{ - 1.0f,  0.0f, 0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  1.0f,  0.0f,  0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  1.0f,  0.0f,  0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  1.0f,  0.0f,  0.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  1.0f,  0.0f,  0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  1.0f,  0.0f,  0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  1.0f,  0.0f,  0.0f},  Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, -1.0f,  0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, -1.0f,  0.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, -1.0f,  0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, -1.0f,  0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, -1.0f,  0.0f},  Vector2f{ 0.0f, 0.0f}),
		 Vertex(    Vector3f{-0.5f, -0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f, -1.0f,  0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  1.0f,  0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  1.0f,  0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  1.0f,  0.0f},  Vector2f{ 1.0f, 1.0f}),
		 Vertex(    Vector3f{ 0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  1.0f,  0.0f},  Vector2f{ 1.0f, 0.0f}),
		 Vertex(     Vector3f{-0.5f,  0.5f, -0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  1.0f,  0.0f},  Vector2f{ 0.0f, 1.0f}),
		 Vertex(    Vector3f{-0.5f,  0.5f,  0.5f},  Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{  0.0f,  1.0f,  0.0f},  Vector2f{ 0.0f, 0.0f})
    };

    Mesh::Ptr mesh = Mesh::New(cubeVertices);
    mesh->scale(5.f, 5.f, 5.f);

    Texture::Ptr texture = Texture::New("D:/dev/albedo.png");
    mesh->addTexture(texture);

    renderer->addModel(mesh);
}

void ViewerWidget::resizeGL(int w, int h)
{
    renderer->resize(w, h);
    picker->setCamera(renderer->getCamera());
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

    if(pickerEnabled)
		picker->pick(renderer->getModels(),Vector2i({ event->x(), event->y() }));
}

void ViewerWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {

    }
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