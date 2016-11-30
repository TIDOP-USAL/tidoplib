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
  getFileName(getRunfile(), name);
  char dir[I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir);

  CmdParser cmdParser(name, "Detección de daños en conductores de líneas eléctricas");
  cmdParser.addParameter("img", "Imagen de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
  cmdParser.addParameterOption("l_detect", "HOUGH,HOUGHP,HOUGH_FAST,LSD", "Detector de lineas", true, "HOUGHP");
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

  // Procesos que se aplican a las imagenes
  // Hacemos un remuestreo al 25% para que tarde menos en determinar los buffer de lineas
  std::shared_ptr<I3D::Resize> resize = std::make_shared<I3D::Resize>(25.);
  std::shared_ptr<FunctionProcess> fProcess1 = std::make_shared<FunctionProcess>(
    [](const cv::Mat &in, cv::Mat *out) {
      in.convertTo(*out, CV_32F);
  });
  //std::shared_ptr<I3D::Normalize> normalize = std::make_shared<I3D::Normalize>(0., 255.);
  int kernel_size = 31;
  double sig = 1., th = I3D_PI / 2., lm = 1.0, gm = 0.02/*, ps = 0.*/;
  cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, th, lm, gm);
  // Se inicializa Filter2D. Posteriormente se modifica según el angulo obtenido por fourier
  std::shared_ptr<I3D::Filter2D> filter2d = std::make_shared<I3D::Filter2D>(CV_32F, kernel);
  std::shared_ptr<FunctionProcess> fProcess2 = std::make_shared<FunctionProcess> (
    [&](const cv::Mat &in, cv::Mat *out) {
      cv::normalize(in, *out, 0, 255, CV_MINMAX); 
      out->convertTo(*out, CV_8U); 
      out->colRange(kernel_size, out->cols - kernel_size).rowRange(31, out->rows - kernel_size).copyTo(*out); 
  });
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);

  I3D::ImgProcessingList imgprolist { 
    /*fProcess1, 
    filter2d,
    fProcess2,*/
    resize,
    bilateralFilter
  };

  if (ls == LD_TYPE::LSD) {
    imgprolist.add(std::make_shared<I3D::Sobel>(1, 0, 3, 1., 0.));
  } else {
    imgprolist.add(std::make_shared<I3D::Canny>());
  }

  // Detector de líneas
  std::unique_ptr<LineDetector> pLineDetector;
  cv::Scalar ang_tol(CV_PI / 2, 0.25);
  
  switch ( ls ) {
  case I3D::LD_TYPE::HOUGH:
    pLineDetector = std::make_unique<ldHouh>(150, ang_tol);
    break;
  case I3D::LD_TYPE::HOUGHP:
    pLineDetector = std::make_unique<ldHouhP>(100, ang_tol, 60., 30.);
    break;
  case I3D::LD_TYPE::HOUGH_FAST:
    pLineDetector = std::make_unique<ldHouhFast>();
    break;
  case I3D::LD_TYPE::LSD:
    pLineDetector = std::make_unique<ldLSD>(ang_tol);
    break;
  default:
    printError("No se ha seleccionado ningún detector de lineas.");
    exit(EXIT_FAILURE);
    break;
  }

  printInfo("Leyendo imagen %s", img.c_str());
  cv::Mat image = cv::imread(img.c_str(), cv::IMREAD_GRAYSCALE);

  // Determinar angulo con Fourier
  std::vector<int> colFourier;
  colFourier.push_back(image.cols / 2);
  std::vector<std::vector<cv::Point>> ptsFourier;
  double angleFourier = fourierLinesDetection(image, colFourier, &ptsFourier);

  // Se aplica el procesado previo a la imagen
  kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, angleFourier, lm, gm);
  filter2d->setParameters(CV_32F, kernel);
  cv::Mat image_pro;
  imgprolist.execute(image, &image_pro);


  pLineDetector->run(image_pro, cv::Scalar(angleFourier, 0.15));
  const std::vector<Line> &detect_lines = pLineDetector->getLines();
  if ( detect_lines.empty() ) { 
    printError("No se han detectando lineas");
    exit(EXIT_FAILURE);
  }
  printInfo("Número de lineas detectada %i", detect_lines.size());

  std::vector<ldGroupLines> linesGroups;
  groupLinesByDist(detect_lines, &linesGroups, 20);
  std::vector<Line> linesJoin;
  for (size_t ig = 0; ig < linesGroups.size(); ig++) {
    joinLinesByDist(linesGroups[ig].getLines(), &linesJoin, 40);
  }


  Helmert2D<cv::Point> trf(0, 0, 4, 0.0);
  //std::vector<std::vector<cv::Point>> buffer;
  cv::Scalar m, stdv;
  bilateralFilter->execute(image, &image);
  for (auto &line : linesJoin) {
    line.pt1 = trf.transform(line.pt1, true);
    line.pt2 = trf.transform(line.pt2, true);
    //Para comprobar...
    //cv::line(image, line.pt1, line.pt2, cv::Scalar(255, 0, 0));
    std::vector<cv::Point> buff;
    lineBuffer(line, 80, &buff);
    //buffer.push_back(buff);

    // Se aplica la mascara a la imagen y obtenemos la zona de estudio.
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
    cv::Mat aux(buff);
    const cv::Point *pts = (const cv::Point*) aux.data;
    int npts = aux.rows;
    cv::fillPoly(mask, &pts, &npts, 1, cv::Scalar(1, 1, 1) );
    cv::Mat searchArea;
    cv::bitwise_and(image, image, searchArea, mask);
    //imshow("Fourier", searchArea);
    cv::waitKey();

    // binarización de la imagen
    cv::meanStdDev(searchArea, m, stdv, mask);
    cv::Mat imgBN;
    cv::threshold(searchArea, imgBN, m[0] + stdv[0], 255, cv::THRESH_BINARY);
    //Mirar algoritmos para ver la anchura de la linea.

    cv::LineIterator li(imgBN, line.pt1, line.pt2, 4, false);

    progress_bar.init(0, li.count, "Busqueda de daños en conductor");
    

    cv::Point axis;
    for ( int i = 0; i < li.count; i++, ++li ) {
      axis = li.pos();
      // Tenemos las coordenadas del eje de la línea. A partir de esto buscar la anchura para 
      // cada punto
      // Tendria que estimar la ancchura del aislador antes. Así podría servirme para el
      // buffer y para determinar esta linea auxiliar
      Line laux(axis, line.angleOX(), 60, true);
      // Ahora con un LineIterator recorro la linea y busco el cambio de b/n
      cv::LineIterator li2(imgBN, laux.pt1, laux.pt2, 4, false);
      int width = 0;
      int ini = -1;
      int end = 0;
      for ( int j = 0; j < li2.count; j++, ++li2 ) {
        int value  = (int)li2.ptr[0];
        if ( value == 255 ) {
          if (ini == -1) ini = j;
          end = j;
          width++;
        }
      }
      //if ( width != end - ini ) 
      //  printWarning("Posible daño");
      //printInfo("Ancho línea %i", width);
      progress_bar();
    }
  }

  //cv::Scalar m, stdv;
  //bilateralFilter->execute(image, &image);
  //for ( const auto &buff : buffer) {
  //  // Se aplica la mascara a la imagen y obtenemos la zona de estudio.
  //  cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
  //  cv::Mat aux(buff);
  //  const cv::Point *pts = (const cv::Point*) aux.data;
  //  int npts = aux.rows;
  //  cv::fillPoly(mask, &pts, &npts, 1, cv::Scalar(1, 1, 1) );
  //  cv::Mat searchArea;
  //  cv::bitwise_and(image, image, searchArea, mask);
  //  imshow("Fourier", searchArea);
  //  cv::waitKey();

  //  // binarización de la imagen
  //  cv::meanStdDev(searchArea, m, stdv, mask);
  //  cv::Mat imgBN;
  //  cv::threshold(searchArea, imgBN, m[0] + stdv[0], 255, cv::THRESH_BINARY);
  //  //Mirar algoritmos para ver la anchura de la linea.

  //  //cv::LineIterator li(img, pt1, pt2, connectivity, false);
  //}
  
  
  
  return 0;
}
