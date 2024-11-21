#include <tidop/core/console.h>
#include <tidop/core/log.h>
#include <tidop/core/msg/message.h>
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
    tl::Path graphos_path = app_path.parentPath().parentPath();
    tl::Path gdal_data_path(graphos_path);
    gdal_data_path.append("gdal\\data");
    tl::Path proj_data_path(graphos_path);
    proj_data_path.append("proj");
#   if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
    CPLSetConfigOption("PROJ_DATA", proj_data_path.toString().c_str());
    CPLSetConfigOption("PROJ_LIB", proj_data_path.toString().c_str());
#   else
    std::string s_proj = proj_data_path.toString();
    const char* proj_data[]{ s_proj.c_str(), nullptr };
    OSRSetPROJSearchPaths(proj_data);
#   endif
    CPLSetConfigOption("GDAL_DATA", gdal_data_path.toString().c_str());
#endif // TL_OS_WINDOWS

#ifdef DEBUG
    qInstallMessageHandler(messageHandlerQt);
    CPLSetErrorHandler(messageHandlerGDAL);
#endif // DEBUG

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
