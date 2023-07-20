#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    viewerWidget(new ViewerWidget(parent))
{
    ui.setupUi(this);
    
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    setCentralWidget(viewerWidget);
}

MainWindow::~MainWindow()
{

}
