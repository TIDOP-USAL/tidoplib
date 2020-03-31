#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
//#include <tidop/img/imgio.h>
#include <tidop/img/imghandler.h>
#include <tidop/img/metadata.h>

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif

/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  std::string img;

  Command cmd("Read Image", "Lectura de una imagen");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("img", 'i', "Lectura de una imagen", &img));

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
  console.setTitle("Read Image");
  console.setLogLevel(MessageLevel::msg_verbose);
  console.setConsoleUnicode();
  MessageManager::instance().addListener(&console);

  ImageHandler image;
  if (image.open(img) == ImageHandler::Status::open_ok) {
    msgInfo("Numero de bandas: %i", image.channels());
    msgInfo("Profundidad de color: %i", image.depth());
    msgInfo("Dimensiones de la imagen: %ix%i", image.cols(), image.rows());
  } else {
    msgError("Error al abrir la imagen: %s", img.c_str());
  }
  cv::Mat bmp;
  image.read(&bmp, geometry::WindowI());

  JpegMetadata metadata(&image);
  std::string document_name = metadata.exifDocumentName();
  std::string description = metadata.exifImageDescription();
  std::string make = metadata.exifMake();
  std::string model = metadata.exifModel();

  msgInfo("Document Name: %s", document_name.c_str());
  msgInfo("Image Description: %s", description.c_str());
  msgInfo("Make: %s", make.c_str());
  msgInfo("Model: %s", model.c_str());

  return 0;
}
