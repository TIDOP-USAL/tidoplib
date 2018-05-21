#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "vect/vectio.h"


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



  VectorGraphics vector;
  if (VectorGraphics::Status::OPEN_OK == vector.open(vect, VectorGraphics::Mode::Create)) {
    msgInfo("Create file: %s", vect.c_str());

    // Se añade una capa
    graph::GLayer *layer;
    vector.createLayer(file_name);

    vector.close();
  }

  return 0;
}
