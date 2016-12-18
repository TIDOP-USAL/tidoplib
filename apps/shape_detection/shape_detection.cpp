//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A7854.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A7854  // Encuentra baliza con medianBlur
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A7973.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A7973  // Encuentra baliza con medianBlur
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A8288.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A8288  // Encuentra baliza con medianBlur
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A8308.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A8308  // Encuentra baliza con medianBlur

//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A0728.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A0728
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A0736.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A0736  // Encuentra baliza con gaussianBlur
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A7949.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A7949
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A7954.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A7954
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A8066.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A8066
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A8187.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A8187


//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A8424.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A8424
//--img=C:\Desarrollo\datos\elementos_a_detectar\2K7A0699.jpg --out=C:\Desarrollo\datos\elementos_a_detectar\2K7A0699  // No hay baliza y saca circulos en zonas que no hay

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
#include "img_processing.h"
#include "transform.h"

using namespace I3D;
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

// Variables globales para pruebas
int higher_threshold = 220;
int max_higher_threshold = 255;
int higher_accum_threshold = 60;
int max_higher_accum_threshold = 255;

cv::Mat image;

std::mutex _mtx;


void houghCircles( cv::Mat &red )
{
  std::vector<Vec3f> circles;
  if ( higher_accum_threshold <= 0 || higher_threshold <= 0 ) return;
  HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/8, higher_threshold, higher_accum_threshold, 30, 80 );
  for (size_t i = 0; i < circles.size(); i++) {
    
    cv::Point center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    cv::Mat aux;
    image.copyTo(aux);
    circle( aux, center, 3, Scalar(0,255,0), -1, 8, 0 );
    circle( aux, center, radius, Scalar(0,0,255), 3, 8, 0 );
    printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);
    WindowI w_aux(center,300);
    w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(red.cols, red.rows)));
    cv::Mat m_aux;
    //... chequear si es una baliza o es un falso positivo
    aux.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
    _mtx.lock();
    cv::imshow("Baliza", m_aux);
    _mtx.unlock();
    cv::waitKey();
  }
}

void change_higher_threshold( int _pos, void *_image )
{
  cv::Mat red = *(cv::Mat *)_image;
  higher_threshold = _pos;
  houghCircles(red);
}

void change_higher_accum_threshold( int _pos, void *_image )
{
  cv::Mat red = *(cv::Mat *)_image;
  higher_accum_threshold = _pos;
  houghCircles(red);
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
   * \brief mCurrentPosition Posición actual del video
   */
  double mCurrentPosition;

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

  cv::Mat out;

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
};

void VideoHelper::onFinish()
{
  VideoStream::Listener::onFinish();
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
}

void VideoHelper::onPause()
{
  VideoStream::Listener::onPause(); 
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
  cv::split(frame, channels);
  cv::Mat red = channels[2];
  image = frame;
  // Se libera memoria
  for (int i = 0; i < 3; i++)
    channels[i].release();

  // Se aplican los procesos previos
  pImgprolist->execute(red, &red);

  //houghCircles(red);
  std::vector<Vec3f> circles;
  if ( higher_accum_threshold <= 0 || higher_threshold <= 0 ) return;
  HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/8, higher_threshold, higher_accum_threshold, 30, 80 );
  for (size_t i = 0; i < circles.size(); i++) {
    cv::Point center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    cv::Mat aux;
    image.copyTo(aux);
    circle( aux, center, 3, Scalar(0,255,0), -1, 8, 0 );
    circle( aux, center, radius, Scalar(0,0,255), 3, 8, 0 );
    printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);
    WindowI w_aux(center,300);
    w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(red.cols, red.rows)));
    //cv::Mat m_aux;
    aux.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(aux);
    if ( i == 0 ) out = aux; // Sólo muestro la primera
    char buffer[I3D_MAX_PATH];
    sprintf_s(buffer, "%s\\frame%05i_%02i.%s", mOutPath.c_str(), cvRound(mCurrentPosition), i, mExtFile.c_str());
    printInfo("Baliza guardada en: %s", buffer);
    cv::imwrite(buffer, aux);

    //_mtx.lock();
    //cv::imshow("Baliza", m_aux);
    //_mtx.unlock();
    //cv::waitKey();
  }


  //WindowI wOut;
  //char buffer[I3D_MAX_PATH];
  //sprintf_s(buffer, "%s\\frame%05i.%s", mOutPath.c_str(), cvRound(mCurrentPosition), mExtFile.c_str());
  //cv::imwrite(buffer, frame);
  if (pProgress) (*pProgress)();
}

void VideoHelper::onResume() 
{ 
  VideoStream::Listener::onResume();
}

void VideoHelper::onShow(cv::Mat &frame) 
{ 
  VideoStream::Listener::onShow(frame);
  cv::imshow("Baliza", out);
  // Mostrar imagen de torre completa
  cv::Mat res;
  cv::resize(image, res, cv::Size(), 0.1, 0.1);
  cv::imshow("Apoyo", res);
}

void VideoHelper::onStop()
{ 
  VideoStream::Listener::onStop(); 
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
  cmdParser.addParameter("in", "Imagen, listado de imagenes o video de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
  cmdParser.addParameterOption("in_type", "image,video", "Tipo de los datos de entrada", true, "image");
  if ( cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  // Se parsean los parámetros del comando
  std::string img = cmdParser.getValue<std::string>("in");
  std::string out_path = cmdParser.getValue<std::string>("out");
  int in_type = cmdParser.getParameterOptionIndex<int>("in_type");


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
    
    //cv::Mat image = cv::imread(img.c_str());
    image = cv::imread(img.c_str());
    if (image.empty()) exit(EXIT_FAILURE);

    cv::Mat channels[3];
    cv::split(image, channels);
    cv::Mat red = channels[2];
    // libero memoria
    for (int i = 0; i < 3; i++)
      channels[i].release();

    //bilateralFilter->execute(red, &red);

    // Busqueda baliza
    //medianBlur->execute(red, &red);
    //gaussianBlur->execute(red, &red);
    //erotion->execute(red, &red);
    //dilate->execute(red, &red);

    imgprolist->execute(red, &red);

    if ( 0 ) {
      std::vector<Vec3f> circles;
      //HoughCircles(red, circles, HOUGH_GRADIENT, 1, red.rows/4, 200, 50, 25, 150 );
      HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/8, 220, 85, 50, 100 );
      for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
        int radius = I3D_ROUND_TO_INT(circles[i][2]);
        circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );
        circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
        printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);
        WindowI w_aux(center,300);
        w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(red.cols, red.rows)));
        cv::Mat m_aux;
        image.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
        cv::imshow("Baliza", m_aux);
        cv::waitKey();
      }
    } else {
      cv::namedWindow( "Baliza", WINDOW_AUTOSIZE );
      cv::createTrackbar( "Threshold: ", "Baliza", &higher_threshold, max_higher_threshold, change_higher_threshold, &red );
      cv::createTrackbar( "Accumulator threshold: ", "Baliza", &higher_accum_threshold, max_higher_accum_threshold, change_higher_accum_threshold, &red );
      //cv::imshow( "Baliza", src );

      houghCircles(red);
    }
    cv::waitKey(0);
  
  } else {     // Busqueda en video
 
    
    // Lectura de video
    //VideoStream strmVideo(img.c_str());
    std::unique_ptr<VideoStream> strmVideo = std::make_unique<ImagesStream>(img.c_str());
    if (!strmVideo->isOpened()) {
      printInfo("No se ha podido cargar el video: %s", img.c_str());
      return 0;
    }
  
    
    // Barra de progreso
    //ProgressBar progress_bar;
    //progress_bar.init(0, strmVideo.getFrameCount());

    VideoHelper videoHelper(out_path, "jpg");
    videoHelper.setImgprolist(imgprolist);
    //videoHelper.setProgressBar(&progress_bar);
    strmVideo->addListener(&videoHelper);

    strmVideo->run(); 

  }

  

  return 0;
}


/*

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

  //std::shared_ptr<I3D::Normalize> normalize = std::make_shared<I3D::Normalize>(255, 0);
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(2);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(3);
  std::shared_ptr<I3D::Canny> canny = std::make_shared<I3D::Canny>();
  std::shared_ptr<I3D::GaussianBlur> gaussianBlur = std::make_shared<I3D::GaussianBlur>(cv::Size(9, 9), 2., 2.);
  cv::Mat image = cv::imread(img.c_str());
  //I3D::Resize res(50.);
  //res.execute(image, &image);
  
  // Ventana de la zona baja de la torre
  //WindowI w(cv::Point(1290, 6190), cv::Point(3402, 7698));
  // Busqueda de la señal de peligro
  cv::Mat image_crop1, image_crop2;
  image.rowRange(500, 3400).colRange(1300, 3400).copyTo(image_crop1);
  image.rowRange(6193, 7698).colRange(1300, 3400).copyTo(image_crop2);
  image.release();

  cv::Mat channels[3];
  cv::split(image_crop1, channels);
  cv::Mat red = channels[2];
  cv::Mat image_cmyk;
  rgbToCmyk(image_crop2, &image_cmyk);
  cv::Mat channels_cmyk[4];
  cv::split(image_cmyk, channels_cmyk);
  cv::Mat yellow = channels_cmyk[2];
  cv::normalize(yellow, yellow, 255, 0, CV_MINMAX);
  yellow.convertTo(yellow, CV_8U);

  bilateralFilter->execute(yellow, &yellow);
  // binarización de la imagen
  cv::Mat yellow_binary;
  cv::threshold(yellow, yellow_binary, 100, 255, cv::THRESH_BINARY);
  cv::Mat red_binary;
  cv::threshold(red, red_binary, 190, 255, cv::THRESH_BINARY);

  //cv::Mat image_gray;
  //cvtColor(image, image_gray, CV_RGB2GRAY);

  // libero memoria
  image_cmyk.release();
  for (int i = 0; i < 4; i++)
    channels_cmyk[i].release();
  for (int i = 0; i < 3; i++)
    channels[i].release();

  I3D::ImgProcessingList imgprolist{ 
    dilate
  };

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  imgprolist.execute(yellow_binary, &yellow_binary);
  cv::Mat yellow_area;
  cv::bitwise_and(yellow, yellow, yellow_area, yellow_binary); 
  cv::findContours( yellow_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  std::vector<cv::Point> approx;
  for (size_t i = 0; i < contours.size(); i++) {
    approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
    if ( approx.size() == 3 ) {
      //if (fabs(contourArea(Mat(approx))) > 50 &&
      //isContourConvex(Mat(approx))) {
        cv::line(image_crop2, approx[0],approx[1],cv::Scalar(255,0,0));
        cv::line(image_crop2, approx[1],approx[2],cv::Scalar(255,0,0));
        cv::line(image_crop2, approx[2],approx[0],cv::Scalar(255,0,0));
      //}
    }
  //  //if( approx.size() == 4 &&
  //  //                fabs(contourArea(Mat(approx))) > 1000 &&
  //  //                isContourConvex(Mat(approx)) ) {
  //  //}
  }


  // Busqueda baliza
  //imgprolist.clear();
  //imgprolist.add(gaussianBlur);
  //imgprolist.execute(red, &red);
  medianBlur(red, red, 5);
  std::vector<Vec3f> circles;
  HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/4, 200, 100, 25, 100 );
  for( size_t i = 0; i < circles.size(); i++ ) {
    cv::Point center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    circle( image_crop1, center, 3, Scalar(0,255,0), -1, 8, 0 );
    circle( image_crop1, center, radius, Scalar(0,0,255), 3, 8, 0 );
  }

  return 0;
}


*/