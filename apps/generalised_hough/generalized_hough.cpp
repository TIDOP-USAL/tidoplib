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
#include "feature_detection/linedetector.h"
#include "videostream.h"
#include "matching.h"
#include "fourier.h"
#include "img_process/img_processing.h"
#include "img_process/filters.h"
#include "transform.h"

//#include "generalized_hough.h"
#include "feature_detection/shapedetection.h"

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
  cmdParser.addParameter("img", "Imagen");
  cmdParser.addParameter("template", "Template");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
  if ( cmdParser.parse(argc, argv) == CmdParser::Status::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  // Se parsean los parámetros del comando
  std::string img = cmdParser.getValue<std::string>("img");
  std::string templ = cmdParser.getValue<std::string>("template");
  std::string out_path = cmdParser.getValue<std::string>("out");

  if (createDir(out_path.c_str()) == -1) { 
    consolePrintError("No se ha podido crear el directorio: %s", out_path.c_str()); 
    exit(EXIT_FAILURE);
  }

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  sprintf(logfile, "%s\\%s.log", out_path.c_str(), name );
  Message::setMessageLogFile(logfile);
  Message::setMessageLevel(MessageLevel::MSG_INFO);

  //std::shared_ptr<I3D::Normalize> normalize = std::make_shared<I3D::Normalize>(255, 0);
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(1);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(1);
  std::shared_ptr<I3D::Canny> canny = std::make_shared<I3D::Canny>();
  std::shared_ptr<I3D::GaussianBlur> gaussianBlur = std::make_shared<I3D::GaussianBlur>(cv::Size(9, 9), 2., 2.);
  std::shared_ptr<I3D::MedianBlur> medianBlur = std::make_shared<I3D::MedianBlur>(5);
  std::shared_ptr<I3D::EqualizeHistogram> equalizeHistogram = std::make_shared<I3D::EqualizeHistogram>();

  std::shared_ptr<I3D::ImgProcessingList> imgprolist = std::make_shared<I3D::ImgProcessingList>(); 
  imgprolist->add(bilateralFilter);
  imgprolist->add(medianBlur);

  cv::Mat image = cv::imread(img.c_str(), 1);
  cv::Mat imageGray;
  cvtColor(image, imageGray, CV_BGR2GRAY);
  cv::Mat imgtempl = cv::imread(templ.c_str(), 0);
  //runGHT('r');

 I3D::GeneralizedHough ght;
 //ght.createTemplate(templ.c_str(), "D:\\Desarrollo\\datos\\Vuelo2_2-2_Conductores\\Recortes\\contour_rough.bmp");

  ght.createRtable("D:\\Desarrollo\\datos\\Vuelo2_2-2_Conductores\\Recortes\\2K7A2606.png", "D:\\Desarrollo\\datos\\Vuelo2_2-2_Conductores\\Recortes\\contour.bmp");
	//	//cv::Mat detect_img = imread("files\\Img_01.png", 1);
  cv::Mat detect_img = imread(img.c_str(), 1);
	//	//cv::Mat detect_img = imread("files\\Img_03.png", 1);
	ght.accumulate(detect_img);
	ght.bestCandidate();

  ////// create template
  //cv::Ptr<cv::GeneralizedHoughBallard> ght = createGeneralizedHoughBallard();
  //ght->setTemplate(imgtempl, cv::Point(30, 126));
  //ght->setCannyHighThresh(255);
  //ght->setCannyLowThresh(150);

  //cv::Mat_<Vec4f> pos; //an array of (x,y,1,0) tuples
  //ght->detect(imageGray, pos);
  //cerr << pos << endl;
  //for (int i=0; i<pos.total(); i++)
  //{
  //    Vec4f p = pos.at<Vec4f>(i);
  //    circle(image, Point(p[0], p[1]), 200, Scalar(255), 2);
  //}

  return 0;
}
