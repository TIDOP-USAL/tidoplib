#include "MainWindow.h"

#include <tidop/viewer/group/PointCloud.h>
#include <tidop/viewer/io/ASCIIReader.h>
#include <tidop/viewer/io/LASReader.h>

#include <thread>

using namespace tl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    viewerWidget(new ViewerWidget(parent))
{
    ui.setupUi(this);

    setCentralWidget(viewerWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::test() 
{

    // Load
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

    PointCloud::Ptr pointCloud = PointCloud::New(vertices);
    pointCloud->setPointSize(4);
    viewerWidget->getRenderer()->addModel(pointCloud);

    /*
    // Load from ASCII file
    Path modelPath("E:/PointClouds/ASCII/torus.txt");

    ModelReader::Ptr reader = ModelReaderFactory::create(modelPath);
    reader->open();

    ModelBase::Ptr model = reader->getModelBase();

    PointCloud::Ptr asciiCloud = std::dynamic_pointer_cast<PointCloud>(model);
    asciiCloud->scale(0.1, 0.1, 0.1);

    viewerWidget->getRenderer()->addModel(asciiCloud);
    */

    // Load from ASCII file
    Path modelPath("E:/PointClouds/MMS.las");

    ModelReader::Ptr reader = ModelReaderFactory::create(modelPath);
    reader->open();

    ModelBase::Ptr model = reader->getModelBase();

    PointCloud::Ptr lasCloud = std::dynamic_pointer_cast<PointCloud>(model);
    lasCloud->scale(0.1, 0.1, 0.1);

    viewerWidget->getRenderer()->addModel(lasCloud);

}
