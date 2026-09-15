#include <tidop/core/console/Console.h>
#include <tidop/core/app/Logger.h>
#include <tidop/core/app/Message.h>
#include <tidop/GeoTools/GeoTools.h>
#include <tidop/GeoTools/CRSsTools.h>

#include <gdal.h>
#include <cpl_conv.h>
#include <ogr_srs_api.h>

#include "MainWindow.h"

#define TL_HAVE_QT

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);


    tl::Path app_path(argv[0]);

#ifdef TL_OS_WINDOWS

    std::string gdal_data_path(CPLGetConfigOption("TL_GDAL_DATA", ""));
    std::string proj_data_path(CPLGetConfigOption("TL_PROJ_DATA", ""));

    CPLSetConfigOption("GDAL_DATA", gdal_data_path.c_str());
    CPLSetConfigOption("PROJ_LIB", proj_data_path.c_str());

#if GDAL_VERSION_NUM < GDAL_COMPUTE_VERSION(3,7,0)
    const char *proj_data[]{proj_data_path.c_str(), nullptr};
    OSRSetPROJSearchPaths(proj_data);
#endif

#endif // TL_OS_WINDOWS

#ifdef DEBUG
    qInstallMessageHandler(messageHandlerQt);
    CPLSetErrorHandler(messageHandlerGDAL);
#endif // DEBUG
    tl::Console& console = tl::Console::instance();
    console.setMessageLevel(tl::MessageLevel::all);
    console.setTitle("TestQt_Viewer");
    console.setConsoleUnicode();
    tl::Message::addMessageHandler(&console);

    // Log file
    auto &log = tl::Logger::instance();
    log.setMessageLevel(tl::MessageLevel::all);
    tl::Message::addMessageHandler(&log);

    
    try {
        tl::GeoTools* ptrGeoTools = tl::GeoTools::getInstance();
        bool ignoreDeprecatedCRSs = true;
        ptrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);
    }
    catch (std::exception& e) {
        tl::printException(e);
        return(1);
    }
    
    MainWindow w;
    w.show();

    //w.loadFromMemory();
    //w.loadFromFile("E:/PointClouds/ASCII/torus.txt");
    //w.loadFromFile("C:/Users/EquipoTidop/Desktop/output.las");

    return a.exec();
}
