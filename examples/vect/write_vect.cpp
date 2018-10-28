#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/vect/vectio.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/polygon.h>

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

  Command cmd(cmd_name, "Escritura de un vector");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("vect", 'v', "Fichero vectorial de salida", &vect));

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
  console.setConsoleUnicode();
  console.setFontHeight(14);
  MessageManager::getInstance().addListener(&console);

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
  if (VectorGraphics::Status::open_ok == vector.open(vect, VectorGraphics::Mode::create)) {
    msgInfo("Create file: %s", vect.c_str());
    vector.create(); ///TODO: Create tiene que tener las propiedades del fichero. Crear un objeto de propiedades de formato como en las imagenes
    // Se añade una capa
    vector.createLayer(file_name);
    vector.writeLayer(std::string(file_name), layer);

    vector.close();
  }

  return 0;
}
