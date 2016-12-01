#include <windows.h>
#include <memory>

// Cabeceras de OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/calib3d.hpp"

// Cabeceras propias
#include "core/utils.h"
#include "core/console.h"
#include "core/messages.h"
#include "LineDetector.h"
#include "VideoStream.h"
#include "matching.h"
#include "fourier.h"
#include "img_processing.h"
#include "transform.h"

using namespace I3D;
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

/*!
 * Detección de daños en conductores de líneas eléctricas:
 *
 * img:         Imagen de los conductores
 * out:         Directorio de salida donde se guarda el log y toda la información generada
 * l_detect:    Detector de lineas. Puede ser HOUGH, HOUGHP, HOUGH_FAST o LSD
 */
int main(int argc, char *argv[])
{
  ProgressBar progress_bar;

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DRIVE + I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DRIVE + I3D_MAX_DIR);

  CmdParser cmdParser(name, "Detección de daños en conductores de líneas eléctricas");
  cmdParser.addParameter("img", "Imagen de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
  if ( cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  std::string img = cmdParser.getValue<std::string>("img");
  std::string out_path = cmdParser.getValue<std::string>("out");
  LD_TYPE ls = cmdParser.getParameterOptionIndex<LD_TYPE>("l_detect");

  if (createDir(out_path.c_str()) == -1) { 
    consolePrintError("No se ha podido crear el directorio: %s", out_path.c_str()); 
    exit(EXIT_FAILURE);
  }

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  sprintf(logfile, "%s\\%s.log", out_path.c_str(), name );
  Message::setMessageLogFile(logfile);
  Message::setMessageLevel(MessageLevel::MSG_INFO);

  cv::Mat image = cv::imread(img.c_str());
  //I3D::Resize res(50.);
  //res.execute(image, &image);
  // Busqueda de la señal de peligro

  cv::Mat channels[3];
  cv::split(image, channels);
  cv::Mat red = channels[2];
  cv::Mat image_cmyk;
  rgbToCmyk(image, &image_cmyk);
  cv::Mat channels_cmyk[4];
  cv::split(image_cmyk, channels_cmyk);
  cv::Mat yellow = channels_cmyk[2];
  
  // binarización de la imagen
  cv::Mat yellow_binary;
  cv::threshold(yellow, yellow_binary, 0.45, 1, cv::THRESH_BINARY);
  cv::Mat red_binary;
  cv::threshold(red, red_binary, 190, 255, cv::THRESH_BINARY);

  // libero memoria
  image.release();
  image_cmyk.release();
  for ( int i = 0; i < 4; i++ )
    channels_cmyk[i].release();
  //for ( int i = 0; i < 3; i++ )
  //  channels[i].release();

  //I3D::ImgProcessingList imgprolist{ 
  //  std::make_shared<I3D::Canny>(), 
  //  std::make_shared<I3D::Dilate>(1) 
  //};

  //std::vector<std::vector<cv::Point>> contours;
  //std::vector<cv::Vec4i> hierarchy;
  //
  //cv::Mat imagePro;
  //imgprolist.execute(image, &imagePro);

  //cv::findContours( imagePro, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

  //std::vector<cv::Point> approx;
  //for( size_t i = 0; i < contours.size(); i++ ) {
  //  approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
  //  if ( approx.size() == 3 ) {
  //  
  //  }
  //  //if( approx.size() == 4 &&
  //  //                fabs(contourArea(Mat(approx))) > 1000 &&
  //  //                isContourConvex(Mat(approx)) ) {
  //  //}
  //}

  return 0;
}
