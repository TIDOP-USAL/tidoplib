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
  cmdParser.addParameter("img_out", "Imagen de salida");
  cmdParser.parse(argc, argv);
  if (cmdParser.parse(argc, argv) == CmdParser::Status::PARSE_ERROR ) {
    return 0;
  }
  std::string img = cmdParser.getValue<std::string>("img");
  std::string img_out = cmdParser.getValue<std::string>("img_out");

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

  RasterGraphics image;
  image.open(img.c_str());
  cv::Mat mat_out;
  WindowI w(PointI(-100, -100), PointI(3900, 3900)); // Ventana de 4000x4000
  double scale = 4.;                                 // 25% de escala de la resulución original  
  
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

  return 0;
}
