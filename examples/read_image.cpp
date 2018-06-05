#include <stdio.h>

/* Cabeceras tidopLib */

#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

using namespace TL;

/*!
 * read_image: Lee una imagen y muestra sus propiedades
 *
 * uso: --img=[ruta imagen]
 */
int main(int argc, char** argv)
{

  char name[TL_MAX_FNAME];
  getFileName(getRunfile(), name, TL_MAX_FNAME);

  CmdParser cmdParser(name, "Lectura de una imagen");
  cmdParser.addParameter("img", "Imagen");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  std::string img = cmdParser.getValue<std::string>("img");
  msgInfo("Image: %s", img.c_str());

  /* Configuración de la consola */

  Console console;
  console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();

  /* Lectura de la imagen */

  TL::RasterGraphics image;
  if(image.open(img) == TL::RasterGraphics::Status::OPEN_OK){
    msgInfo("Número de bandas: %i", image.getBands());
    msgInfo("Profundidad de color: %i", image.getColorDepth());
    msgInfo("Dimensiones: %ix%i", image.getCols(), image.getRows());
  } else {
    msgError("No se ha podido cargar la imagen");
  }

  return 0;
}
