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
#include "VideoStream.h"
#include "matching.h"
#include "fourier.h"
#include "img_process/img_processing.h"
#include "img_process/filters.h"
#include "transform.h"
#include "experimental/experimental.h"

using namespace I3D;
using namespace I3D::EXPERIMENTAL;
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;


// - Clasificar puntos en la imagen como daño, separador, antivibrador, etc.
// - Se saca la anchura y se hace un primer estudio de 'picos' para ver si hay algún elemento que destaque mucho como los separadores.
// - Se parte la linea por ese punto (una ventana) y se vuelve a calcular la anchura para quitar la influencia de esos valores altos 
//   que alterarian la media.

/*!
 * Puntos candidatos con un posible daño.
 * Se guarda el punto, una zona de influencia y tipo de daño.
 */
//class DamageCandidates
//{
//public:
//  /*!
//   * \brief Tipo de daño
//   */
//  enum class Type
//  {
//    unknown,
//    Type1, // ver que nombres poner
//    Type2,
//    Type3,
//    Type4
//  };
//
//  /*!
//   * \brief Punto sobre el centro del conductor donde esta el posible daño
//   */
//  cv::Point mPt;
//
//  /*!
//   * \brief Ventana con la zona del posible daño
//   */
//  WindowI mWindow;
//
//public:
//
//  /*!
//   *
//   */
//  DamageCandidates(cv::Point pt, WindowI w) : mPt(pt), mWindow(w) {}
//  
//  /*!
//   *
//   */
//  ~DamageCandidates();
//
//private:
//
//};
//
//DamageCandidates::~DamageCandidates()
//{
//}
//
//std::vector<DamageCandidates> damages;


/*!
 * Detección de daños en conductores de líneas eléctricas:
 *
 * img:         Imagen de los conductores
 * out:         Directorio de salida donde se guarda el log y toda la información generada
 * l_detect:    Detector de lineas. Puede ser HOUGH, HOUGHP, HOUGH_FAST o LSD
 */
int main(int argc, char *argv[])
{

  // Barra de progreso
  ProgressBar progress_bar;

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DIR);

  // Parser del comando 
  CmdParser cmdParser(name, "Detección de daños en conductores de líneas eléctricas");
  cmdParser.addParameter("img", "Imagen de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true, dir);
  cmdParser.addParameterOption("l_detect", "HOUGH,HOUGHP,HOUGH_FAST,LSD", "Detector de lineas", true, "HOUGHP");
  if ( cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  std::string img = cmdParser.getValue<std::string>("img");
  std::string out_path = cmdParser.getValue<std::string>("out");
  LD_TYPE ls = cmdParser.getParameterOptionIndex<LD_TYPE>("l_detect");
  bool bDrawHoles = false;

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
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(1);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(1);

  // Listado de procesos
  I3D::ImgProcessingList imgprolist { 
    resize,
    bilateralFilter,
    erotion,
    dilate
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

  cv::Mat full_image = cv::imread(img.c_str());
  if (full_image.empty()) exit(EXIT_FAILURE);
  
  // Se limita la zona de estudio para minimizar los efectos de la distorsión de la camara
  cv::Mat image;
  full_image.colRange(1000, full_image.cols - 1000).copyTo(image);
  full_image.release();
  cv::Mat image_gray;
  cvtColor(image, image_gray, CV_BGR2GRAY);

  // Determinar ángulo con Fourier
  std::vector<int> colFourier;
  colFourier.push_back(image_gray.cols / 2);
  std::vector<std::vector<cv::Point>> ptsFourier;
  double angleFourier = fourierLinesDetection(image_gray, colFourier, &ptsFourier);

  // Se aplica el procesado previo a la imagen
  cv::Mat image_pro;
  if ( imgprolist.execute(image_gray, &image_pro) == ProcessExit::FAILURE) 
    exit(EXIT_FAILURE);

  // Detección de lineas
  if (pLineDetector->run(image_pro, cv::Scalar(angleFourier, 0.15)) == LineDetector::Exit::FAILURE) 
    exit(EXIT_FAILURE);

  //pLineDetector->drawLines(image_gray);
  const std::vector<Line> &detect_lines = pLineDetector->getLines();
  if ( detect_lines.empty() ) { 
    printError("No se han detectando lineas");
    exit(EXIT_FAILURE);
  }
  //printInfo("Número de lineas detectada %i", detect_lines.size());

  std::vector<ldGroupLines> linesGroups;
  groupLinesByDist(detect_lines, &linesGroups, 20);
  std::vector<Line> linesJoin;
  for (size_t ig = 0; ig < linesGroups.size(); ig++) {
    joinLinesByDist(linesGroups[ig].getLines(), &linesJoin, 40);
  }

  cv::Mat gap = cv::Mat::zeros(image_gray.size(), CV_8U); // Para los huecos en conductores

  // Transformación al sistema origen
  Helmert2D<cv::Point> trf(0, 0, 4., 0.0);

  //std::vector<std::vector<cv::Point>> buffer;
  cv::Scalar m, stdv;

  bilateralFilter->execute(image_gray, &image_gray);

  int widthBuffer = 80;

  // Se estudian uno a uno los conductores
  for (auto &line : linesJoin) {
    cv::Mat gap = cv::Mat::zeros(image_gray.size(), CV_8U);
    //trf.transformEntity(line, &line, transform_order::DIRECT);
    transform(line, &line, &trf, transform_order::DIRECT);
    std::vector<cv::Point> buff;
    lineBuffer(line, widthBuffer, &buff);

    // Se aplica la mascara a la imagen y obtenemos la zona de estudio.
    cv::Mat mask = cv::Mat::zeros(image_gray.size(), CV_8U);
    cv::Mat aux(buff);
    const cv::Point *pts = (const cv::Point*) aux.data;
    int npts = aux.rows;
    cv::fillPoly(mask, &pts, &npts, 1, cv::Scalar(1, 1, 1) );
    cv::Mat searchArea;
    cv::bitwise_and(image_gray, image_gray, searchArea, mask);

    // binarización de la imagen
    cv::meanStdDev(image_gray, m, stdv, mask);
    cv::Mat imgBN;
    double thresh = m[0] + stdv[0];
    double maxval = 255.;
    cv::threshold(searchArea, imgBN, thresh, maxval, cv::THRESH_BINARY);

    cv::LineIterator li(imgBN, line.pt1, line.pt2, 8, false);
    std::vector<int> v_width(li.count, 0);
    std::vector<cv::Point> vPosWidth(li.count); // Almacenamos un vector de puntos cuya abscisa representa la posición en la linea y sus ordenadas la anchura en ese punto
    cv::Point axis;

    BresenhamLine lineIter(line.pt1, line.pt2);
    if (0) {
      // Pruebas para clasificar los daños o elementos
      v_width.resize(li.count-20);
      vPosWidth.resize(li.count-20);
      lineIter.position(10);
      for ( int i = 0; i < lineIter.size()-20; i++, lineIter++ ) {
        axis = lineIter.position();
        
        // Tenemos las coordenadas del eje de la línea. A partir de esto buscar la anchura para 
        // cada punto
        // Tendria que estimar la anchura del aislador antes. Así podría servirme para el
        // buffer y para determinar esta linea auxiliar
        Line laux(axis, line.angleOX(), widthBuffer, true);
        // Ahora con un LineIterator recorro la linea y busco el cambio de b/n
        cv::LineIterator li2(imgBN, laux.pt1, laux.pt2, 8, false);
        int width = 0;  //Anchura a lo largo de la línea
        int ini = -1;
        int end = 0;
        std::vector<int> v_aux(li2.count, 0);
        std::vector<cv::Point> v_pts_aux(li2.count);
        int value_prev = -1;
        for ( int j = 0; j < li2.count; j++, ++li2 ) {
          cv::Point pt_aux = li2.pos();
          int value = (int)li2.ptr[0];
          if ( value_prev == -1 ) {
            if ( value == 255 )
              v_aux[j] = 1;
          } else {
            int dif = value - value_prev;
            if ( dif < 0 ) {
              v_aux[j] = -1;
            } else if ( dif > 0 ) {
              v_aux[j] = 1;
            } else {
              v_aux[j] = v_aux[j - 1];
            }
          }
          value_prev = value;

          if ( value == 255 ) {
            if (ini == -1) ini = j;
            end = j;
            width++;
          }
          v_pts_aux[j] = li2.pos();
        }

        // Anchura teniendo en cuenta los huecos
        v_width[i] = end - ini + 1;
        vPosWidth[i] = cv::Point(i+10, v_width[i]);
      }

      // Hay que mirar si el máximo representa un pico muy claro. En ese caso podria ser un separador.
      // Aplicamos douglas-Peucker para simplificar la linea
      std::vector<cv::Point> vPosWidthDP;
      approxPolyDP(vPosWidth, vPosWidthDP, 5, false);  // Ver que valor se introduce como distancia máxima. ¿En función de la desviaciòn tipica?

      double slopePrev = 0.;
      for (size_t idp = 1; idp < vPosWidthDP.size(); idp++) {
        // Pendiente
        double slope = SegmentI(vPosWidthDP[idp-1], vPosWidthDP[idp]).angleOX();
        if (idp > 1 && abs(slopePrev - slope) > 0.15) {
          cv::Point pt1 = lineIter.position(vPosWidthDP[idp-1].x);
          cv::Point pt2 = lineIter.position(vPosWidthDP[idp].x);
          // Se añade un posible daño
          //damages.push_back(DamageCandidates(pt1,WindowI(pt1,pt2)));
        }
        slopePrev = slope;
      }


    } else {


      for ( int i = 0; i < li.count; i++, ++li ) {
        axis = li.pos();
        // Tenemos las coordenadas del eje de la línea. A partir de esto buscar la anchura para 
        // cada punto
        // Tendria que estimar la anchura del aislador antes. Así podría servirme para el
        // buffer y para determinar esta linea auxiliar
        Line laux(axis, line.angleOX(), widthBuffer, true);
        // Ahora con un LineIterator recorro la linea y busco el cambio de b/n
        cv::LineIterator li2(imgBN, laux.pt1, laux.pt2, 8, false);
        int width = 0;  //Anchura a lo largo de la línea
        int ini = -1;
        int end = 0;
        std::vector<int> v_aux(li2.count, 0);
        std::vector<cv::Point> v_pts_aux(li2.count);
        int value_prev = -1;
        for ( int j = 0; j < li2.count; j++, ++li2 ) {
          cv::Point pt_aux = li2.pos();
          int value = (int)li2.ptr[0];
          if ( value_prev == -1 ) {
            if ( value == 255 )
              v_aux[j] = 1;
          } else {
            int dif = value - value_prev;
            if ( dif < 0 ) {
              v_aux[j] = -1;
            } else if ( dif > 0 ) {
              v_aux[j] = 1;
            } else {
              v_aux[j] = v_aux[j - 1];
            }
          }
          value_prev = value;

          if ( value == 255 ) {
            if (ini == -1) ini = j;
            end = j;
            width++;
          }
          v_pts_aux[j] = li2.pos();
        }

        // Anchura teniendo en cuenta los huecos
        v_width[i] = end - ini + 1; // width;
        vPosWidth[i] = cv::Point(i, v_width[i]);

        if ( ini != -1 ) {
          std::vector<int> v_(li2.count);
          for ( int k = ini; k < end; k++ ) {
            if ( v_aux[k] == -1 ) {
              cv::line(gap, v_pts_aux[k], v_pts_aux[k], cv::Scalar(255));
            }
          }
        }

      }    
    }







    // Buscar máxima y mínima anchura
    cv::Mat mat_aux(v_width);
    cv::Scalar m, stdv;
    cv::meanStdDev(mat_aux, m, stdv);
    double th1 = m[0] - stdv[0] - 2; // Un pixel mas de margen
    double th2 = m[0] + stdv[0] + 2;


    cv::LineIterator li3(imgBN, line.pt1, line.pt2, 8, false);
    // Ahora buscar si quedan pixeles fuera
    
    // Arrastramos una media de 25 posiciones para ver la variación

    cv::Point iniDamage = cv::Point(0, 0);
    cv::Point endDamage = cv::Point(0, 0);
    std::vector<SegmentI> segmentDamage;
    printInfo("Ancho línea: Mínimo: %f. Máximo: %f", th1, th2);
    progress_bar.init(0., static_cast<double>(v_width.size()-50), "Busqueda de zonas dañadas");
    for (int is = 25; is < v_width.size() - 25; is++, ++li3) {
      double sum = std::accumulate(v_width.begin() + is - 25, v_width.begin() + is + 25, 0);
      int accumul = I3D_ROUND_TO_INT(sum / 50.);
      if ( accumul < I3D_ROUND_TO_INT(th1) || accumul > I3D_ROUND_TO_INT(th2) ) {
        // Pixel fuera de rango.
        axis = li3.pos();
        if (iniDamage == cv::Point(0, 0)) iniDamage = axis;
        endDamage = axis; 
        logPrintWarning("Posible daño (%i, %i). Ancho línea: %i. Mínimo: %f. Máximo: %f", axis.x, axis.y, accumul, th1, th2);
        // Ir acumulando la zona total del daño
      } else {
        if (iniDamage != cv::Point(0, 0)) {
          LineIterator it3 = li3;
          for ( int k = 0; k < 24; k++ ) ++it3; // Un poco ñapas pero para salir del paso. LineIterator esta poco documentada. 
          endDamage = it3.pos();
          segmentDamage.push_back(SegmentI(iniDamage, endDamage));
          
          iniDamage = cv::Point(0, 0);
          endDamage = cv::Point(0, 0);
        }
      }

      progress_bar();
    }

    // Se muestran las zonas encontradas
    for ( int isd = 0; isd < segmentDamage.size(); isd++ ) {
      SegmentI line = segmentDamage[isd];
      if ( line.length() > 10. ) {
        WindowI w_aux = expandWindow(segmentDamage[isd].getWindow(), 100);
        w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(image.cols, image.rows)));
        
        cv::Mat m_aux;
        image.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
        
        std::vector<cv::Point> buff;
        I3D::Translate<cv::Point> trf(w_aux.pt1.x, w_aux.pt1.y);
        //trf.transformEntity(line, &line, transform_order::INVERSE);
        transform(line, &line, &trf, transform_order::INVERSE);
        lineBuffer(line, static_cast<int>(th2) + 10, &buff);
        cv::Mat aux(buff);
        const cv::Point *pts = (const cv::Point*) aux.data;
        int npts = aux.rows;
        cv::polylines(m_aux, &pts, &npts, 1, true, Color::randomColor().get<cv::Scalar>() );
        
        cv::imshow("Warning", m_aux);
        cv::Mat res_out, _aux;
        image.copyTo(_aux);
        cv::rectangle(_aux, windowToCvRect(w_aux), cv::Scalar(0, 255, 0), 4);
        cv::resize(_aux, _aux, cv::Size(), 0.2, 0.2);
        cv::imshow("Vista general", _aux);
        cv::waitKey();
      }

    }

    cv::Mat canny_output;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( gap, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    if ( bDrawHoles) {
      for( size_t i = 0; i< contours.size(); i++ ) {
        if (contours[i].size() < 3) continue;
        cv::Scalar color = Color::randomColor().get<cv::Scalar>();
        WindowI w_aux = cvRectToWindow(boundingRect( contours[i] ));
        w_aux = expandWindow(w_aux, 10);
        cv::Mat _aux;
        image.copyTo(_aux);
        cv::rectangle(_aux, w_aux.pt1, w_aux.pt2, color, 2, 8, 0);
        w_aux = expandWindow(w_aux, 100);
        w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(image.cols, image.rows)));
        cv::Mat m_aux;
        _aux.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
        cv::imshow("Warning", m_aux);
        cv::rectangle(_aux, windowToCvRect(w_aux), cv::Scalar(0, 255, 0), 4);
        cv::resize(_aux, _aux, cv::Size(), 0.2, 0.2);
        cv::imshow("Vista general", _aux);
        cv::waitKey();
      }
    }

  }
  
  
  
  return 0;
}
