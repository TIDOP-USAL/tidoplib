#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/vect/vectio.h>
#include <tidop/graphic/layer.h>

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

  std::string vect;

  Command cmd("Read vector", "Lectura de un vector");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("vect", 'v', "Lectura de un vector", &vect));

  // Parseo de los argumentos y comprobación de los mismos
  Command::Status status = cmd.parse(argc, argv);

  if (status == Command::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == Command::Status::SHOW_HELP) {
    return 0;
  } else if (status == Command::Status::SHOW_LICENCE) {
    return 0;
  } else if (status == Command::Status::SHOW_VERSION) {
    return 0;
  }


  // Consola
  Console &console = Console::getInstance();
  console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  MessageManager::getInstance().addListener(&console);

  msgInfo("Open file: %s", vect.c_str());
  VectorGraphics vector;
  if (VectorGraphics::Status::OPEN_FAIL == vector.open(vect)) return 1;

  msgInfo("Layers: %i", vector.getLayersCount());

  graph::GLayer layer;
  vector.read(0, &layer);

  return 0;
}
