#include "MainWindow.h"

#include <QAction>
#include <QFileDialog>

#include <tidop/viewer/group/PointCloud.h>
#include <tidop/viewer/io/LASReader.h>
#include <tidop/viewer/raycasting/Picker.h>

using namespace tl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    viewerWidget(new ViewerWidget(parent))
{
    ui.setupUi(this);

    ViewerWidget::enableSwitchAxis();
    setCentralWidget(viewerWidget);

    initSignalsAndSlots();


    QAction* actionPicker = new QAction("Picker", this);
    actionPicker->setCheckable(true);
    ui.mainToolBar->addAction(actionPicker);
    connect(actionPicker, &QAction::toggled, this, &MainWindow::togglePicker);


    Picker::Listener listener = [&](const Picker::Result& result) -> void
    {
        static Vector3f previousPoint{ 0.0, 0.0, 0.0 };

        if(result.intersects)
        {
            std::cout << "Selected point:\n" << result.point << "\nOffset:\n" << result.modelBase->getOffset() << std::endl;
            std::cout << "Color: " << result.vertices[0].color << std::endl;
            rayModelBase->translate(-previousPoint.x(), -previousPoint.y(), -previousPoint.z());
            rayModelBase->translate(result.point.x(), result.point.y(), result.point.z());
            previousPoint = result.point;
        }
    };

    viewerWidget->setPickerListener(listener);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initSignalsAndSlots() {
    connect(ui.actionOpen, &QAction::triggered, this, &MainWindow::open);
}

void MainWindow::loadFromFile(const std::string& path) {

    viewerWidget->makeCurrent();

    Path modelPath(path);

    ModelReader::Ptr reader = ModelReaderFactory::create(modelPath);
    reader->open();

    ModelBase::Ptr model = reader->getModelBase();

    PointCloud::Ptr cloud = std::dynamic_pointer_cast<PointCloud>(model);
    cloud->scale(0.01f, 0.01f, 0.01f);

    viewerWidget->getRenderer()->addModel(cloud);




    // Ray model base
    std::vector<Vertex> points = {
    Vertex(Vector3f{0.0, 0.0, 0.0}, Vector4f{1.0, 0.0, 0.0, 1.0})
    };

    rayModelBase = PointCloud::New(points);
    rayModelBase->setPointSize(5.0f);
    viewerWidget->getRenderer()->addModel(rayModelBase);
}

void MainWindow::open() {

    QString file = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open Cloud Point File"), "", tr("PLY (*.ply);;PCD (*.pcd);;LAS (*.las);;ASCII (*.txt)"));

    if (!file.isEmpty()) {
        loadFromFile(file.toStdString());
    }
        
}

void MainWindow::togglePicker(bool enable)
{
    std::cout << "picker enabled: " << enable <<std::endl;
    viewerWidget->enablePicker(enable);
}