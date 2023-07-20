#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "ViewerWidget/ViewerWidget.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindowClass ui;

    ViewerWidget* viewerWidget;
};
