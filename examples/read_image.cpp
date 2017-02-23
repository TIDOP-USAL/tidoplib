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

  //EXPERIMENTAL::MessageManager::message("Prueba de Mensaje %i", 1).print(EXPERIMENTAL::MessageLevel::MSG_DEBUG);



  // Para redirección de errores de otras librerias
  Message::initExternalHandlers();

  CmdParser cmdParser("read_image", "");
  cmdParser.addParameter("img", "Imagen");
  cmdParser.addParameter("img_out", "Imagen de salida");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    exit(EXIT_FAILURE);
  } else if (status == CmdParser::Status::PARSE_HELP) {
    exit(EXIT_SUCCESS);
  }

  //std::string img = cmdParser.getValue<std::string>("img");
  //std::string img_out = cmdParser.getValue<std::string>("img_out");

  std::string img = cmdParser.getValue<Path>("img").toString();
  std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

///////////////
  //Fichero de log
  EXPERIMENTAL::Log &log = EXPERIMENTAL::Log::getInstance();
  log.setLogFile(logfile);
  log.setLogLevel(EXPERIMENTAL::MessageLevel::MSG_INFO);

  //Configuración de mensajes
  EXPERIMENTAL::MessageManager &msg_h = EXPERIMENTAL::MessageManager::getInstance();
  msg_h.addListener(&log);
  msg_h.release("Mensaje de prueba", EXPERIMENTAL::MessageLevel::MSG_ERROR);
  msg_h.release(EXPERIMENTAL::MessageManager::Message("Mensaje de prueba %i", 1).getMessage(), EXPERIMENTAL::MessageLevel::MSG_ERROR);
/////////

  RasterGraphics image;
  image.open(img.c_str());
  cv::Mat mat_out;
  WindowI w(PointI(-100, -100), PointI(3900, 3900)); // Ventana de 4000x4000
  double scale = 4.;                                 // 25% de escala de la resolución original  
  
  Helmert2D<PointI> trf;
  image.read(&mat_out, w, scale, &trf);                         

  // Imagen que se guarda
  RasterGraphics imageOut;
  imageOut.open(img_out.c_str(), Mode::Create);
  imageOut.create(1000, 1000, 3, 0);
  imageOut.write(mat_out, &trf);

  //GdalGeoRaster geo;
  //geo.open("D://Desarrollo//datos//mtn25_epsg25830_0033-3.tif", Mode::Read);
  
  Affine<PointD> TrfAffine;
  TrfAffine.setParameters(0.67646181611214418, 0.0013147558867338115, -0.0013312380792860341, 0.67646180945385193, 848.026479127, 19.43015351242);

  GeoRasterGraphics geoRaster;
  geoRaster.open("D://Desarrollo//datos//mtn25_epsg25830_0033-3.tif", Mode::Read);
  WindowD w_g(PointD(377386.335, 4801010.256), PointD(380324.165, 4803176.748));
  geoRaster.read(&mat_out, w_g, scale);

  exit(EXIT_SUCCESS);
}
