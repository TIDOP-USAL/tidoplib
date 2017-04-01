#include "core/defs.h"

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
#include "img_process/white_balance.h"
#include "transform.h"
#include "experimental/experimental.h"
#include "geometric_entities/point.h"

using namespace I3D;
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

// Variables globales para pruebas
int higher_threshold = 220;
int max_higher_threshold = 255;
int higher_accum_threshold = 60;
int max_higher_accum_threshold = 255;
int minRadius = 30;
int maxRadius = 80;
bool bDraw = false;
bool bShowImg = true;
bool bSaveImages = true;
bool bSaveAll = false;

cv::Mat image;

std::mutex _mtx;


void houghCircles( cv::Mat &red )
{
  std::vector<Vec3f> circles;
  if ( higher_accum_threshold <= 0 || higher_threshold <= 0 ) return;
  HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/8, higher_threshold, higher_accum_threshold, minRadius, maxRadius );
  for (size_t i = 0; i < circles.size(); i++) {
    
    PointI center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    cv::Mat aux;
    image.copyTo(aux);
    if (bDraw) {
      circle( aux, center, 3, Scalar(0,255,0), -1, 8, 0 );
      circle( aux, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);
    WindowI w_aux(center,300);
    w_aux = windowIntersection(w_aux, WindowI(PointI(0, 0), PointI(red.cols, red.rows)));
    if (bShowImg) {
      cv::Mat m_aux;
      //... chequear si es una baliza o es un falso positivo
      aux.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
      _mtx.lock();
      cv::imshow("Baliza", m_aux);
      _mtx.unlock();
      cv::waitKey();
    }
    if (bSaveAll == false) return;
  }
}

void change_higher_threshold( int _pos, void *_image )
{
  higher_threshold = _pos;
  if (_image) {
    cv::Mat red = *(cv::Mat *)_image;
    houghCircles(red);
  }
}

void change_higher_accum_threshold( int _pos, void *_image )
{
  higher_accum_threshold = _pos;
  if (_image) {
    cv::Mat red = *(cv::Mat *)_image;
    houghCircles(red);
  }
}





/*!
 * Clase auxiliar para manejar los frames de video. Heredada 
 * de VideoStream::Listener e implementa los métodos que controlan
 * la ejecución de los eventos
 */
class VideoHelper : public VideoStream::Listener
{
public:

  /*!
   * \brief mOutPath Ruta donde se guardan las imagenes de salida
   */
  std::string mOutPath;

  /*!
   * \brief mExtFile Extensión de las imagenes de salida
   */
  std::string mExtFile;

  /*!
   * \brief pProgress Barra de progreso
   */
  ProgressBar *pProgress;

  /*!
   * Procesos que se aplican a la imagen
   */
  std::shared_ptr<I3D::ImgProcessingList> pImgprolist;

  /*!
   * \brief mCurrentPosition Posición actual del video
   */
  double mCurrentPosition;

  cv::Mat out;

  int nFound;

  /*!
   * \brief guarda la ventana de la baliza
   */
  bool bSaveImage;

public:

  VideoHelper(const std::string &path, const std::string &ext, ProgressBar *progress = NULL) 
    : mOutPath(path), mExtFile(ext), pProgress(progress), mCurrentPosition(0) { }

  ~VideoHelper() {}

  /*!
   * \brief onFinish
   */
  void onFinish() override;

  /*!
   * \brief onInitialize
   */
  void onInitialize() override;

  /*!
   * \brief onPause
   */
  void onPause() override;

  /*!
   * \brief onPositionChange
   * \param position Posición del video
   */
  void onPositionChange(double position) override;

  /*!
   * \brief onRead
   * \param frame Frame leido
   */
  void onRead(cv::Mat &frame) override;

  /*!
   * \brief onResume
   */
  void onResume() override;

  /*!
   * \brief onShow
   * \param frame Frame que se muestra
   */
  void onShow(cv::Mat &frame) override;

  /*!
   * \brief onStop
   */
  void onStop() override;

  /*!
   * \brief Establece una función de progreso
   * \param progress
   */
  void setProgressBar( ProgressBar *progress);

  void setImgprolist(std::shared_ptr<I3D::ImgProcessingList> imgprolist);

  void setSaveImage(bool saveImg) { bSaveImage = saveImg; }

};

void VideoHelper::onFinish()
{
  VideoStream::Listener::onFinish();
  printInfo("Número de balizas encontradas: %i", nFound);
}

void VideoHelper::onInitialize()
{
  VideoStream::Listener::onInitialize();

  if (mOutPath.empty()) {
    char path[I3D_MAX_DRIVE + I3D_MAX_DIR];
    getFileDriveDir(getRunfile(),path,I3D_MAX_DRIVE + I3D_MAX_DIR);
    mOutPath = path;
  } else {
    createDir(mOutPath.c_str());
  }
  nFound = 0;
  if (bShowImg) {
    cv::namedWindow("Baliza", WINDOW_AUTOSIZE);
    cv::createTrackbar("Threshold: ", "Baliza", &higher_threshold, max_higher_threshold, change_higher_threshold);
    cv::createTrackbar("Accumulator threshold: ", "Baliza", &higher_accum_threshold, max_higher_accum_threshold, change_higher_accum_threshold);
  }
}

void VideoHelper::onPause()
{
  VideoStream::Listener::onPause(); 
  consolePrintInfo("Proceso en pausa. Tabulación para continuar");
}

void VideoHelper::onPositionChange(double position) 
{ 
  VideoStream::Listener::onPositionChange(position);
  mCurrentPosition = position;
}

void VideoHelper::onRead(cv::Mat &frame) 
{
  VideoStream::Listener::onRead(frame);

  cv::Mat channels[3];
  cv::Mat red;
  image = frame;

  if (1) {
    cv::split(frame, channels);
    red = channels[2];
  } else {
    //... prueba
    cv::Mat imageChromaticity;
    chromaticityCoordinates(image, &imageChromaticity);
    cv::Mat channels[3];
    cv::split(imageChromaticity, channels);
    cv::Mat red;
    cv::normalize(channels[2], red, 0, 255, CV_MINMAX); 
    red.convertTo(red, CV_8U); 
  }

  // Se libera memoria
  for (int i = 0; i < 3; i++)
    channels[i].release();

  // Se aplican los procesos previos
  pImgprolist->execute(red, &red);

  std::vector<Vec3f> circles;
  if ( higher_accum_threshold <= 0 || higher_threshold <= 0 ) return;
  HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/8, higher_threshold, higher_accum_threshold, minRadius, maxRadius );
  for (size_t i = 0; i < circles.size(); i++) {
    PointI center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    cv::Mat aux;
    image.copyTo(aux);
    WindowI w_aux(center,300);
    w_aux = windowIntersection(w_aux, WindowI(PointI(0, 0), PointI(red.cols, red.rows)));
    aux.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(aux);
    
    if ( 0 ) { // Prueba para diferenciar los dos tipos de balizas.

/*

Color Detection:
http://opencv-srf.blogspot.com.es/2010/09/object-detection-using-color-seperation.html

Para eliminar falsos positivos se comprobará el color de la baliza. Si no es naranja o blanco se
pasará al siguiente elemento. Para el naranja se pasará de RGB a HSV y se tomara el valor naranja
como H=0-20.

*/

      cv::Point c_moments;
      cv::Mat img_hsv;
      cvtColor(aux, img_hsv, COLOR_BGR2HSV);
      PointI pt_c = center - w_aux.pt1;
      WindowI w_aux2(pt_c, radius*2 + 20);
      img_hsv.rowRange(w_aux2.pt1.y, w_aux2.pt2.y).colRange(w_aux2.pt1.x, w_aux2.pt2.x).copyTo(img_hsv);
      
      cv::Mat img_thresh;

      // Umbral por rango de color naranja
      cv::inRange(img_hsv, cv::Scalar(0, 0, 110), cv::Scalar(20, 255, 255), img_thresh);

      // Operación morfológica de apertura para eliminar elementos pequeños del fondo
      Opening opening(2, cv::MORPH_ELLIPSE);
      opening.execute(img_thresh, &img_thresh);

      // Operación morfológica de cierre para eliminar agujeros en el fondo
      Closing closing(2, cv::MORPH_ELLIPSE);
      closing.execute(img_thresh, &img_thresh);

      //Calculate the moments of the thresholded image
      //cv::Moments _moments = moments(img_thresh);

      //double dM01 = _moments.m01;
      //double dM10 = _moments.m10;
      //double dArea = _moments.m00;

      //if ( dArea > 1000000 ) {
      //  c_moments.x = dM10 / dArea;
      //  c_moments.y = dM01 / dArea;
      //} else continue;

      int count_white = cv::countNonZero(img_thresh != 0);
      int area_max = static_cast<int>((radius+5) * (radius+5) * I3D_PI);
      int area_min = static_cast<int>((radius) * (radius) * I3D_PI / 2); // Margen hasta media esfera.

      cv::Scalar color;
      //if ( abs(area - count_white) > (area * 0.3) ) continue;
      if (count_white < area_min || count_white > area_max) {
        // Ver si es una baliza blanca y naranja
        //std::shared_ptr<I3D::WhitePatch> whiteBalance = std::make_shared<I3D::WhitePatch>();
        //cv::Mat img_wb;
        //whiteBalance->execute(aux, &img_wb);
        cv::Mat aux_gray;        
        cvtColor(aux, aux_gray, CV_BGR2GRAY);
        I3D::EqualizeHistogram eh;
        eh.execute(aux_gray, &aux_gray);
        cv::Mat aux_bn;
        cv::threshold(aux_gray, aux_bn, 240, 255, cv::THRESH_BINARY);

        // Si hay mas elementos que la baliza los etiquetamos y los estudiamos por separado
        cv::Mat labels;
        int n_labels = cv::connectedComponents(aux_bn, labels);
        bool bFound = false;
        for (int i = 0; i < n_labels; i++) {
          int count_white = cv::countNonZero(img_thresh != 0);
          int area_max = static_cast<int>((radius+5) * (radius+5) * I3D_PI);
          int area_min = static_cast<int>((radius) * (radius) * I3D_PI / 2);     
          if (count_white >= area_min && count_white <= area_max) {
            color = cv::Scalar(255, 0, 0);
            bFound = true;
            break;
          }
        }
        if (!bFound) continue;
      } else {
        color = cv::Scalar(0, 0, 255);
      }

      cv::Point c = center - w_aux.pt1;
      if (bDraw) {
        circle( aux, c, 3, cv::Scalar(0,255,0), -1, 8, 0 );
        circle( aux, c, radius, color, 3, 8, 0 );
      } else {
        cv::Mat mask = cv::Mat::zeros(aux.size(), CV_8U);
        cv::circle(mask, c, radius, cv::Scalar(1,1,1), -1);
        cv::bitwise_and(aux, aux, aux, mask);
      }

      printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);

      if ( i == 0 ) out = aux; // Sólo muestro la primera

      if (bSaveImage) {
        char buffer[I3D_MAX_PATH];
        sprintf_s(buffer, I3D_MAX_PATH, "%s\\frame%05i_%02i.%s", mOutPath.c_str(), cvRound(mCurrentPosition), static_cast<int>(i), mExtFile.c_str());
        printInfo("Baliza guardada en: %s", buffer);
        cv::imwrite(buffer, aux);
      }

      nFound++;
      if (!bSaveAll) break;

    } else {
      //if (bDraw) {
      //  circle(aux, center - w_aux.pt1, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
      //  circle(aux, center - w_aux.pt1, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
      //}
      cv::Point c = center - w_aux.pt1;
      if (bDraw) {
        circle( aux, c, 3, cv::Scalar(0,255,0), -1, 8, 0 );
        circle( aux, c, radius, cv::Scalar(0, 0, 255), 3, 8, 0 );
      } else {
        cv::Mat mask = cv::Mat::zeros(aux.size(), CV_8UC3);
        cv::circle(mask, c, radius+1, cv::Scalar(255,255,255), -1);
        cv::Mat _aux;
        aux.copyTo(_aux, mask);
        aux = _aux;
      }
      printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);

      if ( i == 0 ) out = aux; // Sólo muestro la primera

      if (bSaveImage) {
        char buffer[I3D_MAX_PATH];
        sprintf_s(buffer, I3D_MAX_PATH, "%s\\frame%05i_%02i.%s", mOutPath.c_str(), cvRound(mCurrentPosition), static_cast<int>(i), mExtFile.c_str());
        printInfo("Baliza guardada en: %s", buffer);
        cv::imwrite(buffer, aux);
      }

      nFound++;
      if (!bSaveAll) break;
    }
  }

  if (pProgress) (*pProgress)();
}

void VideoHelper::onResume() 
{ 
  VideoStream::Listener::onResume();
}

void VideoHelper::onShow(cv::Mat &frame) 
{ 
  VideoStream::Listener::onShow(frame);
  if (bShowImg) {
    if (!out.empty())
      cv::imshow("Baliza", out);
    // Mostrar imagen de torre completa
    cv::Mat res;
    cv::resize(image, res, cv::Size(), 0.1, 0.1);
    cv::imshow("Apoyo", res);
  }
}

void VideoHelper::onStop()
{ 
  VideoStream::Listener::onStop(); 
  printInfo("Proceso detenido por el usuario");
}

void VideoHelper::setProgressBar(ProgressBar *progress)
{
  pProgress = progress;
}

void VideoHelper::setImgprolist(std::shared_ptr<I3D::ImgProcessingList> imgprolist)
{
  pImgprolist = imgprolist;
}


/*!
 * Detección de daños en conductores de líneas eléctricas:
 *
 * img:         Imagen o listado de imagenes de los conductores
 * out:         Directorio de salida donde se guarda el log y toda la información generada
 * in_type:     Tipo de los datos de entrada. Puede ser imagen o video. Como video tambien soporta un txt con un listado de imagenes
 * th:          Umbral más alto del detector de bordes de Canny
 * th_acu:      Umbral del acumulador para los centros de los círculos
 * r_min:       Radio mínimo para los círculos
 * r_max:       Radio máximo para los círculos
 * draw:        Dibuja el circulo y su centro
 * showImg:     Muestra las imagenes
 * all:         Guarda todos los resultados de la detección en cada imagen. Por defecto sólo guarda el primero
 */
int main(int argc, char *argv[])
{

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DRIVE + I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DRIVE + I3D_MAX_DIR);

  CmdParser cmdParser(name, "Detección de balizas de líneas eléctricas");
  cmdParser.addParameter("in", "Imagen o listado de imagenes de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true, dir);
  cmdParser.addParameterOption("in_type", "image,stream", "Tipo de los datos de entrada", true, "image");
  cmdParser.addParameter("th", "Umbral más alto del detector de bordes de Canny", true, std::to_string(higher_threshold).c_str());
  cmdParser.addParameter("th_acu", "Umbral del acumulador para los centros de los círculos", true, std::to_string(higher_accum_threshold).c_str());
  cmdParser.addParameter("r_min", "Radio mínimo para los círculos", true, std::to_string(minRadius).c_str());
  cmdParser.addParameter("r_max", "Radio máximo para los círculos", true, std::to_string(maxRadius).c_str());
  cmdParser.addOption("draw", "Dibuja el circulo y su centro");
  cmdParser.addOption("show_img", "Muestra las imagenes");
  cmdParser.addOption("save_img", "Salva las imagenes");
  cmdParser.addOption("all", "Guarda todos los resultados de la detección en cada imagen. Por defecto sólo guarda el primero");

  if ( cmdParser.parse(argc, argv) == CmdParser::Status::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  // Se parsean los parámetros del comando
  std::string img = cmdParser.getValue<std::string>("in");
  std::string out_path = cmdParser.getValue<std::string>("out");
  int in_type = cmdParser.getParameterOptionIndex<int>("in_type");
  higher_threshold = cmdParser.getValue<int>("th");
  higher_accum_threshold = cmdParser.getValue<int>("th_acu");
  minRadius = cmdParser.getValue<int>("r_min");
  maxRadius = cmdParser.getValue<int>("r_max");
  bDraw = cmdParser.hasOption("draw");
  bShowImg = cmdParser.hasOption("show_img");
  bSaveImages = cmdParser.hasOption("save_img");
  bSaveAll = cmdParser.hasOption("all");

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

  if ( in_type == 0 ) { // Busqueda en una imagen
    
    image = cv::imread(img.c_str());
    if (image.empty()) exit(EXIT_FAILURE);
    std::shared_ptr<I3D::WhitePatch> whiteBalance = std::make_shared<I3D::WhitePatch>();
    whiteBalance->execute(image, &image);

    cv::Mat channels[3];
    cv::Mat red;
    if (1) {
      cv::split(image, channels);
      red = channels[2];
    } else {
      cv::Mat imageChromaticity;
      chromaticityCoordinates(image, &imageChromaticity);
      cv::split(imageChromaticity, channels);
      cv::normalize(channels[2], red, 0, 255, CV_MINMAX); 
      red.convertTo(red, CV_8U);
    }


    // Se libera memoria
    for (int i = 0; i < 3; i++)
      channels[i].release();

    imgprolist->execute(red, &red);

    if ( 0 ) {
      std::vector<Vec3f> circles;
      HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/8, higher_threshold, higher_accum_threshold, minRadius, maxRadius );
      for (size_t i = 0; i < circles.size(); i++) {
        PointI center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
        int radius = I3D_ROUND_TO_INT(circles[i][2]);
        if (bDraw) {
          circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );
          circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }
        printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);
        WindowI w_aux(center,300);
        w_aux = windowIntersection(w_aux, WindowI(PointI(0, 0), PointI(red.cols, red.rows)));
        cv::Mat m_aux;
        image.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
        cv::imshow("Baliza", m_aux);
        cv::waitKey();
      }
    } else {
      if (bShowImg) {
        cv::namedWindow( "Baliza", WINDOW_AUTOSIZE );
        cv::createTrackbar( "Threshold: ", "Baliza", &higher_threshold, max_higher_threshold, change_higher_threshold, &red );
        cv::createTrackbar( "Accumulator threshold: ", "Baliza", &higher_accum_threshold, max_higher_accum_threshold, change_higher_accum_threshold, &red );
      }

      houghCircles(red);
    }
    cv::waitKey(0);
  
  } else {     // Busqueda en video
 
    
    // Lectura de listado de imagenes
    std::unique_ptr<VideoStream> strmVideo = std::make_unique<ImagesStream>(img.c_str());
    if (!strmVideo->isOpened()) {
      printInfo("No se ha podido cargar el listado de imagenes o esta mal formado: %s", img.c_str());
      return 0;
    }

    VideoHelper videoHelper(out_path, "jpg");
    videoHelper.setImgprolist(imgprolist);
    videoHelper.setSaveImage(bSaveImages);
    strmVideo->addListener(&videoHelper);

    strmVideo->run(); 

  }

  return 0;
}
