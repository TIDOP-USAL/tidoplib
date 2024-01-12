#include "MainWindow.h"

#define TL_HAVE_QT

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.test();

    return a.exec();
}
