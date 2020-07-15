#include <stdio.h>
#include <memory>

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/crstransf.h>

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  std::string epsg_in;
  std::string epsg_out;
  double coord_x = 0.0;
  double coord_y = 0.0;
  double coord_z = 0.0;

  // Se definen los parámetros y opciones
  Command cmd(cmd_name, "Ejemplo de transformación de coordenadas");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("epsg_in", 'i', "Sistema de referencia de entrada", &epsg_in));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("epsg_out", 'o', "Sistema de referencia de salida", &epsg_out));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("coord_x", 'x', "Coordenadas X", &coord_x));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("coord_y", 'y', "Coordenadas Y", &coord_y));
  cmd.push_back(std::make_shared<ArgumentDoubleOptional>("coord_z", 'z', "Coordenada Z", &coord_z));

  cmd.addExample(cmd_name + " --epsg_in EPSG:25830 --epsg_out EPSG:4258 -x 281815.044 -y 4827675.243");
  cmd.addExample(cmd_name + " -iEPSG:25830 -oEPSG:4258 -x281815.044 -y4827675.243");

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
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  Point3D pt_utm(coord_x, coord_y, coord_z);
  msgInfo("Coordenadas en %s: (%f,%f,%f)", epsg_in.c_str(), pt_utm.x, pt_utm.y, pt_utm.z);

  try {
    std::shared_ptr<Crs> epsgIn(new Crs(epsg_in));
    std::shared_ptr<Crs> epsgOut(new Crs(epsg_out));
    CrsTransform<Point3D> crs(epsgIn, epsgOut);
    
    Point3D pt_geo;
    crs.transform(pt_utm, &pt_geo);

    msgInfo("Coordenadas transformadas en %s: (%f,%f,%f)", epsg_out.c_str(), pt_geo.x, pt_geo.y, pt_geo.z);
  } catch (const std::exception &e) {

  }

  return 0;
}
