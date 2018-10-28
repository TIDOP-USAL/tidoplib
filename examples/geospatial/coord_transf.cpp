/*!
 * Ejemplo de transformación de coordenadas
 * 
 */

#include <stdio.h>
#include <memory>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace geometry;
using namespace geospatial;

enum class options {
  opt1,
  opt2,
  opt3
};

int main(int argc, char** argv)
{
#ifdef HAVE_GDAL

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  std::string epsg_in;
  std::string epsg_out;
  std::string coord;

  // Se definen los parámetros y opciones
  Command cmd(cmd_name, "Ejemplo de transformación de coordenadas");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("epsg_in", 'i', "Sistema de referencia de entrada", &epsg_in));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("epsg_out", 'o', "Sistema de referencia de salida", &epsg_out));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("coord", 'c', "Coordenadas a transformar", &coord));

  // Parseo de los argumentos y comprobación de los mismos
  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::parse_error ) {
    return 1;
  } else if (status == Command::Status::show_help) {
    return 0;
  } else if (status == Command::Status::show_licence) {
    return 0;
  } else if (status == Command::Status::show_version) {
    return 0;
  }

  // Consola
  Console &console = Console::getInstance();
  console.setTitle(cmd_name);                         // Titulo de la ventana de consola
  console.setConsoleUnicode();
  console.setFontHeight(14);                      // Se establece el tamaño de fuente
  console.setLogLevel(MessageLevel::msg_verbose); // Se muestran todos los mensajes por consola
  MessageManager::getInstance().addListener(&console);

  CrsTransform<Point3D> crs(std::make_shared<Crs>(epsg_in), std::make_shared<Crs>(epsg_out));
  Point3D pt_utm(350000., 4800000., 0.);
  Point3D pt_geo;
  crs.transform(pt_utm, &pt_geo);
#endif

  return 0;
}


