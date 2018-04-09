/*!
 * Ejemplo de transformación de coordenadas
 * 
 */

#include <stdio.h>
#include <memory>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "geometry/entities/point.h"
#include "geospatial/crs.h" 

using namespace TL;
using namespace TL::geometry;

enum class options {
  opt1,
  opt2,
  opt3
};

int main(int argc, char** argv)
{

  char name[TL_MAX_FNAME];
  getFileName(getRunfile(), name, TL_MAX_FNAME);

  // Se definen los parámetros y opciones
  CmdParser cmdParser(name, "Ejemplo de transformación de coordenadas");
  cmdParser.addParameter("epsg_in", "Sistema de referencia de entrada");
  cmdParser.addParameter("epsg_out", "Sistema de referencia de salida");
  cmdParser.addParameter("c", "Coordenadas a transformar");
  
  // Parseo de los argumentos y comprobación de los mismos
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  std::string epsg_in = cmdParser.getValue<std::string>("epsg_in");
  std::string epsg_out = cmdParser.getValue<std::string>("epsg_out");
  std::string coord = cmdParser.getValue<std::string>("c");


  // Consola
  Console console;
  console.setTitle(name);                         // Titulo de la ventana de consola
  console.setConsoleUnicode();
  console.setFontHeight(24);                      // Se establece el tamaño de fuente
  console.setLogLevel(MessageLevel::MSG_VERBOSE); // Se muestran todos los mensajes por consola

  CrsTransform<Point3D> crs(std::make_shared<Crs>(epsg_in), std::make_shared<Crs>(epsg_out));
  Point3D pt_utm(350000., 4800000., 0.);
  Point3D pt_geo;
  crs.transform(pt_utm, &pt_geo);

  return 0;
}
