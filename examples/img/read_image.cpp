#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img/imgio.h>
//#include "img/metadata.h"

using namespace TL;

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
  console.setTitle("Read Image");
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();
  MessageManager::getInstance().addListener(&console);

  TL::RasterGraphics image;
  if (image.open(img) == TL::RasterGraphics::Status::OPEN_OK) {
    msgInfo("Numero de bandas: %i", image.getBands());
    msgInfo("Profundidad de color: %i", image.getColorDepth());
    msgInfo("Dimensiones de la imagen: %ix%i", image.getCols(), image.getRows());
  } else {
    msgError("Error al abrir la imagen: %s", img.c_str());
  }

  //TL::ImgMetadata *metadata = new TL::JpegMetadata();
  //TL::JpegMetadata *metadata2 = new TL::JpegMetadata();
  //std::string prueba = metadata2->ExifDocumentName;
  return 0;
}
