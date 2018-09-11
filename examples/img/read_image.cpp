#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"
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

  CmdParser cmdParser("Read Image", "Lectura de una imagen");
  cmdParser.addParameter("img", "Imagen");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  std::string img = cmdParser.getValue<std::string>("img");


  // Consola
  Console console;
  console.setTitle("Read Image");
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();

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
