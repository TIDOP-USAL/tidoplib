#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/vect/vectio.h>
#include <tidop/graphic/layer.h>

#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace graph;

/*!
 * read_vect: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  fs::path app_path(argv[0]);
  std::string cmd_name = app_path.stem().string();

  std::string vect;

  Command cmd(cmd_name, "Lectura de un vector");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("vect", 'v', "Lectura de un vector", &vect));

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
  console.setTitle(cmd_name);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::getInstance().addListener(&console);

  msgInfo("Open file: %s", vect.c_str());
  VectorGraphics vector;
  if (VectorGraphics::Status::open_fail == vector.open(vect)) return 1;

  msgInfo("Layers: %i", vector.getLayersCount());

  graph::GLayer layer;
  vector.read(0, &layer);

  return 0;
}
