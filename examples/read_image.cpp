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

  std::vector<PointD> ptsIn{
    PointD(4157222.543, 664789.307),
    PointD(4149043.336, 688836.443),
    PointD(4172803.511, 690340.078),
    PointD(4177148.376, 642997.635),
    PointD(4137012.190, 671808.029),
    PointD(4146292.729, 666952.887),
    PointD(4138759.902, 702670.738) };

  std::vector<PointD> ptsOut(7);
  Helmert2D<PointD> _trf(100., 100., 1., 0.0);
  PointD ptout = ptsIn[0] * _trf;

  //std::transform(ptsIn.begin(), ptsIn.end(), ptsOut.begin(), [&ptsIn](PointD pt) -> PointD { return pt; });
  //I3D::transform(ptsIn.begin(), ptsIn.end(), ptsOut.begin(), &_trf);

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


  std::string img = cmdParser.getValue<Path>("img").toString();
  std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

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
