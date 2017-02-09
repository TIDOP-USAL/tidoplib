#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

using namespace I3D;


/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{
  Message::initExternalHandlers();

  CmdParser cmdParser("read_image", "");
  cmdParser.addParameter("img", "Imagen");
  cmdParser.parse(argc, argv);
  if (cmdParser.parse(argc, argv) == CmdParser::Status::PARSE_ERROR ) {
    return 0;
  }
  std::string img = cmdParser.getValue<std::string>("img");

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

  RasterGraphics image;
  image.open(img.c_str());
  cv::Mat img_out;
  WindowI w(PointI(-100, -100), PointI(2000, 2000));
  image.read(&img_out, w, 2);

  return 0;
}
