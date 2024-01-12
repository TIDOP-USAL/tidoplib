#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "ViewerWidget/ViewerWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::MainWindowClass ui;
    tl::ViewerWidget* viewerWidget;

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:

    void test();

};
