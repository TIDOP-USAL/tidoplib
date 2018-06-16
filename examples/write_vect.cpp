#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "vect/vectio.h"
#include "graphic/layer.h"
#include "graphic/entities/polygon.h"

using namespace TL;
using namespace TL::graph;

/*!
 * read_vect: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  char name[TL_MAX_FNAME];
  getFileName(getRunfile(), name, TL_MAX_FNAME);

  CmdParser cmdParser(name, "");
  cmdParser.addParameter("vect", "Fichero vectorial de salida");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  std::string vect = cmdParser.getValue<std::string>("vect");

  // Consola
  Console console;
  console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  
  char file_name[TL_MAX_FNAME];
  getFileName(vect.c_str(), file_name, TL_MAX_FNAME);

  GLayer layer;
  layer.setName(file_name);
  std::shared_ptr<GPolygon> polygon/* = std::make_shared<GPolygon>()*/;
  polygon->push_back(geometry::PointD(641.148132, 720.013672));
  polygon->push_back(geometry::PointD(643.571106, 719.713989));
  polygon->push_back(geometry::PointD(645.182739, 719.514648));
  polygon->push_back(geometry::PointD(638.723450, 720.313599)); 

  layer.push_back(polygon);

  VectorGraphics vector;
  if (VectorGraphics::Status::OPEN_OK == vector.open(vect, VectorGraphics::Mode::Create)) {
    msgInfo("Create file: %s", vect.c_str());
    vector.create(); ///TODO: Create tiene que tener las propiedades del fichero. Crear un objeto de propiedades de formato como en las imagenes
    // Se añade una capa
    vector.createLayer(file_name);
    vector.writeLayer(std::string(file_name), layer);

    vector.close();
  }

  return 0;
}
