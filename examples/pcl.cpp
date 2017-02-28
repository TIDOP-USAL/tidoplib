#include <stdio.h>

#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

#include "experimental/experimental.h"

using namespace I3D;


/*!
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  //// Para redirección de errores de otras librerias
  //Message::initExternalHandlers();

  CmdParser cmdParser("PCL", "");
  cmdParser.addParameter("file", "Fichero que se abre");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    exit(EXIT_FAILURE);
  } else if (status == CmdParser::Status::PARSE_HELP) {
    exit(EXIT_SUCCESS);
  }

  std::string file = cmdParser.getValue<std::string>("file");

  //Fichero de log
  EXPERIMENTAL::Log &log = EXPERIMENTAL::Log::getInstance();
  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    log.setLogFile(logfile);
  }
  log.setLogLevel(EXPERIMENTAL::MessageLevel::MSG_INFO);

  //Configuración de mensajes
  EXPERIMENTAL::MessageManager &msg_h = EXPERIMENTAL::MessageManager::getInstance();
  msg_h.addListener(&log);

  msgInfo("Abriendo fichero: %s", file.c_str());
  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
  if (pcl::io::loadPCDFile(file.c_str(), *cloud) == -1) {
    msgError("Couldn't read file test_pcd.pcd \n");
    exit(EXIT_FAILURE);
  }
  msgInfo("Lectura de nube de puntos");
  msgInfo("Ancho: %i", cloud->width);
  msgInfo("Alto: %i", cloud->height);
  msgInfo("Número de puntos: %i", cloud->size());

  // Lectura de 10 puntos
  for (size_t i = 0; i < 10; ++i) {
    std::cout << "    " << cloud->points[i].x
          << " "    << cloud->points[i].y
          << " "    << cloud->points[i].z 
          << std::endl;
  }

  

  exit(EXIT_SUCCESS);
}
