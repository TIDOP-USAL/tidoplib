#include "ViewerWidget.h"

#include <iostream>
#include <vector>
#include <array>

#include <tidop/viewer/Vertex.h>
#include <tidop/viewer/opengl/buffer/VertexBuffer.h>
#include <tidop/viewer/opengl/buffer/VertexArray.h>
#include <tidop/viewer/opengl/shader/Shader.h>
#include <tidop/viewer/renderer/TrackballCamera.h>

#include <tidop/math/algebra/matrices.h>

#include <QMouseEvent>
#include <QWheelEvent>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define TO_DEGREES (180 / M_PI) * 
#define TO_RADIANS (M_PI / 180) * 

using namespace tl;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
    "color = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 color;"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

std::vector<Vertex> vertices = {
    Vertex(Vector3f{-0.5, -0.5,  0.5}, Vector4f{0.0f, 0.0f, 1.0f, 1.0f}),
    Vertex(Vector3f{ 0.5, -0.5,  0.5}, Vector4f{1.0f, 0.0f, 1.0f, 1.0f}),
    Vertex(Vector3f{ 0.5,  0.5,  0.5}, Vector4f{0.0f, 1.0f, 1.0f, 1.0f}),
    Vertex(Vector3f{-0.5,  0.5,  0.5}, Vector4f{0.0f, 1.0f, 0.5f, 1.0f}),
    Vertex(Vector3f{-0.5, -0.5, -0.5}, Vector4f{0.0f, 0.0f, 1.0f, 1.0f}),
    Vertex(Vector3f{ 0.5, -0.5, -0.5}, Vector4f{1.0f, 0.0f, 1.0f, 1.0f}),
    Vertex(Vector3f{ 0.5,  0.5, -0.5}, Vector4f{0.0f, 1.0f, 1.0f, 1.0f}),
    Vertex(Vector3f{-0.5,  0.5, -0.5}, Vector4f{0.0f, 1.0f, 0.5f, 1.0f})
};

std::vector<unsigned int> indices {
    0, 1, 2, 1, 5, 6, 7, 6, 5,
    2, 3, 0, 6, 2, 1, 5, 4, 7,
    4, 0, 3, 4, 5, 1, 3, 2, 6,
    3, 7, 4, 1, 0, 4, 6, 7, 3
};

std::vector<Vertex> vertices2 = {
    Vertex(Vector3f{ -0.6f, -0.4f, 3.0}, Vector4f{ 1.f, 0.f, 0.f, 1.0f }),
    Vertex(Vector3f{  0.6f, -0.4f, 3.0}, Vector4f{ 0.f, 1.f, 0.f, 1.0f }),
    Vertex(Vector3f{   0.f,  0.6f, 3.0}, Vector4f{ 0.f, 0.f, 1.f, 1.0f })
};

VertexArray::Ptr vertexArray;
VertexBuffer::Ptr vertexBuffer;
ShaderProgram::Ptr shaderProgram;

Matrix4x4f model = Matrices::scale(0.5f, 0.5f, 0.5f);

double aspectRatio = static_cast<double>(WINDOW_WIDTH) / WINDOW_HEIGHT;
TrackballCamera camera = TrackballCamera::perspectiveCamera(TO_RADIANS 45.0, aspectRatio, 0.1, 1000);

Vector2i previousMouse({0, 0});
bool mousePressed = false, first = false;
int button = 0;

void drawScene() 
{
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    shaderProgram->useProgram();

    shaderProgram->uniformMat4("model", model);
    shaderProgram->uniformMat4("view", camera.getViewMatrix());
    shaderProgram->uniformMat4("projection", camera.getProjectionMatrix());

    vertexArray->bind();

    if (!vertexBuffer->hasIndexBuffer()) glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    else    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    vertexArray->unbind();
}

void ViewerWidget::initializeGL()
{
    // Init GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Error: " << std::endl;
    }

    // Shaders
    Shader vertexShader = Shader::fromCode(vertexShaderSource, Shader::ShaderType::Vertex);
    Shader fragmentShader = Shader::fromCode(fragmentShaderSource, Shader::ShaderType::Fragment);
    shaderProgram = ShaderProgram::New(vertexShader, fragmentShader);

    // Buffers
    vertexArray = VertexArray::New();
    vertexBuffer = VertexBuffer::New(vertices, indices);
    vertexBuffer->bind();
    vertexArray->unbind();

    // Camera
    camera.zoom(-2.0f);
}

void ViewerWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    float theta = camera.getTheta();
    float phi = camera.getPhi();
    float radius = camera.getRadius();

    double aspectRatio = static_cast<double>(w) / h;
    camera = TrackballCamera::perspectiveCamera(TO_RADIANS 45.0, aspectRatio, 0.1, 1000);

    camera.setTheta(theta);
    camera.setPhi(phi);
    camera.setRadius(radius);
}

void ViewerWidget::paintGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //model = model * Matrices::rotationX(TO_RADIANS 1.0f);
    //camera.rotate(0.0f, 0.01f);

    // Draw scene
    drawScene();

    // Update -> WARNING: No FPS limit
    update();
}

void ViewerWidget::mousePressEvent(QMouseEvent* event) {
    mousePressed = true;
    previousMouse[0] = event->x();
    previousMouse[1] = event->y();
    button = event->button();
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent* event) {
    mousePressed = false;
}

void ViewerWidget::mouseMoveEvent(QMouseEvent* event) {


    if (mousePressed && button == Qt::LeftButton) {

        float dTheta = (event->x() - previousMouse.x()) / static_cast<float>(WINDOW_WIDTH);
        float dPhi = (event->y() - previousMouse.y()) / static_cast<float>(WINDOW_HEIGHT);
        std::cout << event->x() << " " << event->y() << std::endl;

        const float sensitivity = 1.0f;
        previousMouse = Vector2i({ event->x(), event->y() });

        camera.rotate(-dTheta * sensitivity, -dPhi * sensitivity);
    }
    else if (mousePressed && button == Qt::RightButton) {

        float dx = (event->x() - previousMouse.x()) / (static_cast<float>(WINDOW_WIDTH) / 2);
        float dy = (event->y() - previousMouse.y()) / (static_cast<float>(WINDOW_HEIGHT) / 2);

        const float panSensitivity = 1.0f;
        previousMouse = Vector2i({ event->x(), event->y() });

        camera.pan(dx * panSensitivity, dy * panSensitivity);
    }
}

void ViewerWidget::wheelEvent(QWheelEvent* event) {
    float dr = 0.1f;
    if (event->angleDelta().y() < 0) dr *= -1.0f;
    camera.zoom(dr);
}