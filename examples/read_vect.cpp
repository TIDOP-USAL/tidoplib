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
  cmdParser.addParameter("vect", "Vector");
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
  
  msgInfo("Open file: %s", vect.c_str());
  VectorGraphics vector;
  vector.open(vect);
  msgInfo("Layers: %i", vector.getLayersCount());
  vector.read();
  return 0;
}
