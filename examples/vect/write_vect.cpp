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

  fs::path vect_in;
  fs::path vect_out;

  Command cmd(cmd_name, "Escritura de un vector");
  cmd.push_back(std::make_shared<ArgumentPathRequired>("in", 'i', "Fichero vectorial de entrada", &vect_in));
  cmd.push_back(std::make_shared<ArgumentPathRequired>("out", 'o', "Fichero vectorial de salida", &vect_out));


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
  console.setLogLevel(MessageLevel::msg_verbose);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  MessageManager::instance().addListener(&console);

  std::string file_in = vect_in.string();
  std::string file_out = vect_out.string();


  //GLayer layer;
  //layer.setName(file_name);
  //std::shared_ptr<GPolygon> polygon/* = std::make_shared<GPolygon>()*/;
  //polygon->push_back(geometry::PointD(641.148132, 720.013672));
  //polygon->push_back(geometry::PointD(643.571106, 719.713989));
  //polygon->push_back(geometry::PointD(645.182739, 719.514648));
  //polygon->push_back(geometry::PointD(638.723450, 720.313599));

  //layer.push_back(polygon);

  VectorGraphics vector;
  vect_in.string();
  if (VectorGraphics::Status::open_fail == vector.open(file_in)) return 1;
  VectorGraphics vector_out;
  if (VectorGraphics::Status::open_fail == vector_out.open(file_out, VectorGraphics::Mode::create)) return 1;

  vector_out.create();  //

  int n = vector.layersCount();

  for (int i = 0; i < n; i++){
    graph::GLayer layer;
    vector.read(0, &layer);


    // Se añade una capa
    vector_out.createLayer(layer.name());
    vector_out.writeLayer(layer.name(), layer);

  }

  vector.close();
  vector_out.close();

  return 0;
}
