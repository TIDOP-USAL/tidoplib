#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

#include "geometry/entities/point.h"
#include "geometry/transform.h" 

#include "opencv2/core/core.hpp"

using namespace TL;
using namespace TL::geometry;
using namespace cv;

#ifdef VISUAL_LEAK_DETECTOR
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



  //char name[TL_MAX_FNAME];
  //getFileName(getRunfile(), name, TL_MAX_FNAME);

  //CmdParser cmdParser(name, "");
  //cmdParser.addParameter("img", "Imagen");
  //cmdParser.addParameter("img_out", "Imagen de salida");
  //CmdParser::Status status = cmdParser.parse(argc, argv);
  //if (status == CmdParser::Status::PARSE_ERROR ) {
  //  //exit(EXIT_FAILURE);
  //  return 1;
  //} else if (status == CmdParser::Status::PARSE_HELP) {
  //  //exit(EXIT_SUCCESS);
  //  return 0;
  //}

  //std::string img = cmdParser.getValue<Path>("img").toString();
  //std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  //// Fichero de log
  //Log &log = Log::getInstance();
  ////Configuración de log y mensajes por consola
  //char logfile[TL_MAX_PATH];
  //if (changeFileExtension(getRunfile(), "log", logfile, TL_MAX_PATH) == 0) {
  //  log.setLogFile(logfile);
  //}
  //log.setLogLevel(MessageLevel::MSG_VERBOSE);

  // Consola
  Console console;
  //console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();
  //console.setFontBold(true);
  console.setFontHeight(24);
  
  ////Configuración de mensajes
  //MessageManager &msg_h = MessageManager::getInstance();
  //msg_h.addListener(&log);
  //msg_h.addListener(&console);
 

  //std::vector<cv::Point2f> points_g0 {
  //  cv::Point2f(326.168579, 433.456543),
  //  cv::Point2f(326.168579, 1105.757813),
  //  cv::Point2f(326.168579, 1778.059082),
  //  cv::Point2f(326.168579, 2450.360352),
  //  cv::Point2f(326.168579, 3122.661621),
  //  cv::Point2f(326.168579, 3794.962891),
  //  cv::Point2f(326.168579, 4467.264160),
  //  cv::Point2f(326.168579, 5139.565430)
  //};


  //std::vector<cv::Point2f> points_g5 {
  //  cv::Point2f(8590.993164, 456.778076),
  //  cv::Point2f(8590.993164, 1129.240112),
  //  cv::Point2f(8590.993164, 1801.702271),
  //  cv::Point2f(8590.993164, 2474.164307),
  //  cv::Point2f(8590.993164, 3146.626465),
  //  cv::Point2f(8590.993164, 3819.088379),
  //  cv::Point2f(8590.993164, 4491.550781),
  //  cv::Point2f(8590.993164, 5164.012695)
  //};



  //std::vector<cv::Point2f> points_g0 {
  //  cv::Point2f(329,431),
  //  cv::Point2f(328,1103),
  //  cv::Point2f(328,1776),
  //  cv::Point2f(327,2448),
  //  cv::Point2f(327,3120),
  //  cv::Point2f(326,3792),
  //  cv::Point2f(326,4464),
  //	cv::Point2f(325,5136)
  //};

  //std::vector<cv::Point2f> points_g5 {
  //  cv::Point2f(8594,459),
  //  cv::Point2f(8593,1131),
  //  cv::Point2f(8593,1804),
  //  cv::Point2f(8593,2477),
  //  cv::Point2f(8592,3149),
  //  cv::Point2f(8592,3822),
  //  cv::Point2f(8591,4495),
  //	cv::Point2f(8591,5167)
  //};

  std::vector<cv::Point2f> points_5_h{
    cv::Point2f(1193.909790, 5164.012695),	
    cv::Point2f(1866.371826, 5164.012695),	
    cv::Point2f(2538.833984, 5164.012695),	
    cv::Point2f(3211.296143, 5164.012695),	
    cv::Point2f(3883.758057, 5164.012695),	
    cv::Point2f(4556.220215, 5164.012695),	
    cv::Point2f(5228.682129, 5164.012695),	
    cv::Point2f(5901.144531, 5164.012695),	
    cv::Point2f(6573.606445, 5164.012695),	
    cv::Point2f(7246.068359, 5164.012695),	
    cv::Point2f(7918.530762, 5164.012695),	
    cv::Point2f(8590.993164, 5164.012695)
  };

  std::vector<cv::Point2f> points_4_ht{
    cv::Point2f(1109.279785, 76.174706),	
    cv::Point2f(1781.035522, 76.174706),	
    cv::Point2f(2452.791260, 76.174706),	
    cv::Point2f(3124.546875, 76.174706),	
    cv::Point2f(3796.302490, 76.174706),	
    cv::Point2f(4468.058105, 76.174706),	
    cv::Point2f(5139.813965, 76.174706),	
    cv::Point2f(5811.569824, 76.174706),	
    cv::Point2f(6483.325195, 76.174706),	
    cv::Point2f(7155.081055, 76.174706),	
    cv::Point2f(7826.836914, 76.174706),	
    cv::Point2f(8498.592773, 76.174706)
  };


  std::vector<cv::Point2f> points_4_v_no_orientado{
    cv::Point2f(8498.592773, 76.174706),
    cv::Point2f(8498.592773, 747.930420),
    cv::Point2f(8498.592773, 1419.686035),
    cv::Point2f(8498.592773, 2091.441895),
    cv::Point2f(8498.592773, 2763.197510),
    cv::Point2f(8498.592773, 3434.953125),
    cv::Point2f(8498.592773, 4106.708984),
    cv::Point2f(8498.592773, 4778.464355),
    cv::Point2f(8498.592773, 5450.220215)
  };

  std::vector<cv::Point2f> points_1_v{
    cv::Point2f(244.034363, 223.266144),
    cv::Point2f(244.034363, 894.944336),
    cv::Point2f(244.034363, 1566.622559),
    cv::Point2f(244.034363, 2238.300781),
    cv::Point2f(244.034363, 2909.979004),
    cv::Point2f(244.034363, 3581.657227),
    cv::Point2f(244.034363, 4253.335449),
    cv::Point2f(244.034363, 4925.013672),
    cv::Point2f(244.034363, 5596.691895)
  };

  cv::Mat aux;
  RasterGraphics rg_image;
  if (rg_image.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Rioja\\20180513\\corregidas\\GRID1\\GRID5_c.tif") == RasterGraphics::Status::OPEN_FAIL) return 1;
  rg_image.read(&aux, WindowI());
  int channels = rg_image.getBands();
  rg_image.close();

  if (channels == 2) {
    // Quito la capa de transparencia
    cv::Mat mat_split[2];
    cv::split(aux, mat_split);
    aux = mat_split[0].clone();
  } else if (channels >= 3)
    cv::cvtColor(aux, aux, CV_RGB2GRAY);

  try {
    cornerSubPix(aux, points_5_h, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  } catch (cv::Exception &e) {
    msgError(e.what());
    return 1;
  }

  if (rg_image.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Rioja\\20180513\\corregidas\\GRID1\\GRID4_c.tif") == RasterGraphics::Status::OPEN_FAIL) return 1;
  rg_image.read(&aux, WindowI());
  channels = rg_image.getBands();
  rg_image.close();

  if (channels == 2) {
    // Quito la capa de transparencia
    cv::Mat mat_split[2];
    cv::split(aux, mat_split);
    aux = mat_split[0].clone();
  } else if (channels >= 3)
    cv::cvtColor(aux, aux, CV_RGB2GRAY);

  try {
    cornerSubPix(aux, points_4_ht, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  } catch (cv::Exception &e) {
    msgError(e.what());
    return 1;
  }

  for (int i = 1; i < points_5_h.size(); i++) {
    msgInfo("dist_%i_%i: %f", i-1, i, distance(points_5_h[i - 1], points_5_h[i]));
  }

  for (int i = 1; i < points_4_ht.size(); i++) {
    msgInfo("dist_%i_%i: %f", i-1, i, distance(points_4_ht[i - 1], points_4_ht[i]));
  }

  Helmert2D<cv::Point2f> trf;
  std::vector<double> error;
  double rmse;
  trf.compute(points_4_ht, points_5_h, &error, &rmse);
  
  std::vector<cv::Point2f> points_4_v_orientado;
  trf.transform(points_4_v_no_orientado, &points_4_v_orientado);

  if (rg_image.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Rioja\\20180513\\corregidas\\GRID1\\GRID4_c.tif") == RasterGraphics::Status::OPEN_FAIL) return 1;
  rg_image.read(&aux, WindowI());
  channels = rg_image.getBands();
  rg_image.close();

  if (channels == 2) {
    // Quito la capa de transparencia
    cv::Mat mat_split[2];
    cv::split(aux, mat_split);
    aux = mat_split[0].clone();
  } else if (channels >= 3)
    cv::cvtColor(aux, aux, CV_RGB2GRAY);

  //try {
  //  cornerSubPix(aux, points_4_v_orientado, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  //} catch (cv::Exception &e) {
  //  msgError(e.what());
  //  return 1;
  //}

  if (rg_image.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Rioja\\20180513\\corregidas\\GRID1\\GRID1_c.tif") == RasterGraphics::Status::OPEN_FAIL) return 1;
  rg_image.read(&aux, WindowI());
  //int channels = rg_image.getBands();
  rg_image.close();

  if (channels == 2) {
    // Quito la capa de transparencia
    cv::Mat mat_split[2];
    cv::split(aux, mat_split);
    aux = mat_split[0].clone();
  } else if (channels >= 3)
    cv::cvtColor(aux, aux, CV_RGB2GRAY);

  try {
    cornerSubPix(aux, points_1_v, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  } catch (cv::Exception &e) {
    msgError(e.what());
    return 1;
  }

  trf.compute(points_1_v, points_4_v_orientado, &error, &rmse);

  ////CrsTransform<Point3D> crs("EPSG:25830", "EPSG:4326");
  ////Point3D pt_utm(350000., 4800000., 0.);
  ////Point3D pt_geo;
  ////crs.transform(pt_utm, &pt_geo);

  ////RasterGraphics image;
  ////Helmert2D<PointI> trf;
  ////cv::Mat mat_out;
  ////double scale = 4.;   // 25% de escala de la resolución original 

  ////if (image.open(img.c_str()) == RasterGraphics::Status::OPEN_FAIL) exit(EXIT_FAILURE);

  ////// saveAs() tiene que crear una copia con la libreria adecuada si tienen modo de copia
  ////image.createCopy(img_out.c_str());

  ////try {    
  ////  WindowI w(PointI(-100, -100), PointI(3900, 3900)); // Ventana de 4000x4000                          
  ////  image.read(&mat_out, w, scale, &trf);

  ////} catch (I3D::Exception &e) {
  ////  printError(e.what());
  ////  exit(EXIT_FAILURE);
  ////}

  ////// Imagen que se guarda
  ////RasterGraphics imageOut;
  ////imageOut.open(img_out.c_str(), Mode::Create);
  ////imageOut.create(1000, 1000, 3, 0);
  ////imageOut.write(mat_out, &trf);


  ////GeoRasterGraphics geoRaster;
  ////geoRaster.open("D://Desarrollo//datos//mtn25_epsg25830_0033-3.tif", Mode::Read);
  ////WindowD w_g(PointD(377386.335, 4801010.256), PointD(380324.165, 4803176.748));
  ////geoRaster.read(&mat_out, w_g, scale);

  //TODO: Ojo con salir con exit!!! No llama a las destructoras y con VLD me estaba
  //dando fugas de memoria
  //exit(EXIT_SUCCESS); 
  return 0;
}
