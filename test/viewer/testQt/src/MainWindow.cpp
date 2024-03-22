#include "MainWindow.h"

#include <QFileDialog>

#include <tidop/viewer/group/PointCloud.h>
#include <tidop/viewer/io/LASReader.h>

using namespace tl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    viewerWidget(new ViewerWidget(parent))
{
    ui.setupUi(this);

    setCentralWidget(viewerWidget);

    initSignalsAndSlots();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initSignalsAndSlots() {
    connect(ui.actionOpen, &QAction::triggered, this, &MainWindow::open);
}

void MainWindow::loadFromMemory()
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
}

void MainWindow::loadFromFile(const std::string& path) {

    viewerWidget->makeCurrent();

    Path modelPath(path);

    ModelReader::Ptr reader = ModelReaderFactory::create(modelPath);
    reader->open();

    ModelBase::Ptr model = reader->getModelBase();

    PointCloud::Ptr cloud = std::dynamic_pointer_cast<PointCloud>(model);
    cloud->scale(0.1f, 0.1f, 0.1f);

    viewerWidget->getRenderer()->addModel(cloud);
}

void MainWindow::open() {

    QString file = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open Cloud Point File"), "", tr("PLY (*.ply);;PCD (*.pcd);;LAS (*.las);;ASCII (*.txt)"));

    if (!file.isEmpty()) {
        loadFromFile(file.toStdString());
    }
        
}