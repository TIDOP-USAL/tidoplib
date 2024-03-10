#include "MainWindow.h"

#define TL_HAVE_QT

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //w.loadFromMemory();
    //w.loadFromFile("E:/PointClouds/ASCII/torus.txt");
    w.loadFromFile("C:/Users/EquipoTidop/Desktop/output.las");

    return a.exec();
}
