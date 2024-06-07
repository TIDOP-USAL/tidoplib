#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "ViewerWidget/ViewerWidget.h"

#include <tidop/viewer/group/PointCloud.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::MainWindowClass ui;
    tl::ViewerWidget* viewerWidget;

    tl::PointCloud::Ptr rayModelBase;

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:

    void initSignalsAndSlots();

    void loadFromMemory();
    void loadFromFile(const std::string& path);
   
private slots:

    void open();
    void togglePicker(bool enable);
};
