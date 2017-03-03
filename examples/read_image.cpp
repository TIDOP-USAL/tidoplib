#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

#include "experimental/experimental.h"

using namespace I3D;


/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  CmdParser cmdParser("read_image", "");
  cmdParser.addParameter("img", "Imagen");
  cmdParser.addParameter("img_out", "Imagen de salida");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    exit(EXIT_FAILURE);
  } else if (status == CmdParser::Status::PARSE_HELP) {
    exit(EXIT_SUCCESS);
  }

  std::string img = cmdParser.getValue<Path>("img").toString();
  std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  // Fichero de log
  Log &log = Log::getInstance();
  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    log.setLogFile(logfile);
  }
  log.setLogLevel(MessageLevel::MSG_INFO);

  // Consola
  Console console;
  console.setLogLevel(MessageLevel::MSG_INFO);
  console.setConsoleUnicode();

  //Configuración de mensajes
  MessageManager &msg_h = MessageManager::getInstance();
  msg_h.addListener(&log);
  msg_h.addListener(&console);


  RasterGraphics image;
  Helmert2D<PointI> trf;
  cv::Mat mat_out;
  double scale = 4.;   // 25% de escala de la resolución original 

  if (image.open(img.c_str()) == Status::OPEN_FAIL) exit(EXIT_FAILURE);

  try {    
    WindowI w(PointI(-100, -100), PointI(3900, 3900)); // Ventana de 4000x4000                          
    image.read(&mat_out, w, scale, &trf);

  } catch (I3D::Exception &e) {
    printError(e.what());
    exit(EXIT_FAILURE);
  }

  // Imagen que se guarda
  RasterGraphics imageOut;
  imageOut.open(img_out.c_str(), Mode::Create);
  imageOut.create(1000, 1000, 3, 0);
  imageOut.write(mat_out, &trf);


  //GeoRasterGraphics geoRaster;
  //geoRaster.open("D://Desarrollo//datos//mtn25_epsg25830_0033-3.tif", Mode::Read);
  //WindowD w_g(PointD(377386.335, 4801010.256), PointD(380324.165, 4803176.748));
  //geoRaster.read(&mat_out, w_g, scale);

  exit(EXIT_SUCCESS);
}
