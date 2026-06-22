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
    //mesh->rotate(AxisAngle<float>(consts::pi<float>, { 1.0, 1.0, 0.0 }));

    Texture::Ptr texture = Texture::New("C:/Users/EquipoTidop/Documents/Texturas/albedo.png");
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPointF pos = event->position();
#else
    QPoint pos = event->pos();
#endif
    renderer->setPreviousMouse(Vector2i({ roundToInteger(pos.x()),
                                          roundToInteger(pos.y())}));
    button = event->button();

    if (pickerEnabled) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QPointF pickPos = event->position();
#else
        QPoint pickPos = event->pos();
#endif
        picker->pick(renderer->getModels(), Vector2i({roundToInteger(pickPos.x()),
                                                      roundToInteger(pickPos.y())}));

    }
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPointF pos = event->position();
#else
    QPoint pos = event->pos();
#endif

    if (mousePressed && button == Qt::LeftButton)
        renderer->rotate(roundToInteger(pos.x()),
                         roundToInteger(pos.y()));
    else if (mousePressed && button == Qt::RightButton)
        renderer->pan(roundToInteger(pos.x()),
                      roundToInteger(pos.y()));
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