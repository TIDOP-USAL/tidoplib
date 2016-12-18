#include "DetectTransmissionTower.h"

#include <cstdio>

#include "opencv2/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/optflow.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "core/utils.h"
#include "videostream.h"
#include "geometric_entities/segment.h"
#include "feature_detection/linedetector.h"
#include "img_processing.h"
#include "matching.h"
#include "transform.h"

using namespace cv;
using namespace optflow;
using namespace cv::ximgproc;
using namespace cv::xfeatures2d;
using namespace I3D;

int getdir(const std::string _filename, std::vector<std::string> &files)
{
  std::ifstream myfile(_filename.c_str());
  if (!myfile.is_open()) {
    std::cout << "Unable to read file: " << _filename << std::endl;
    exit(0);
  } else {
    size_t found = _filename.find_last_of("/\\");
    std::string line_str, path_to_file = _filename.substr(0, found);
    while ( getline(myfile, line_str) )
      files.push_back(path_to_file+std::string("/")+line_str);
  }
  return 1;
}


void getTowerImageAndWindow(const char * file, std::vector<std::string> &images_paths, std::vector<WindowI> &windows) 
{
  std::ifstream read_handler(file, std::ifstream::in);
  if (!read_handler.is_open()) {
    consolePrintInfo("Unable to read file: %s", file);
  } else {
    std::vector<std::string> out;
    while (!read_handler.eof()) {
      std::string line;
      read_handler >> line;
      if ( !line.empty() && split(line, out, "|") == 0) {
        images_paths.push_back(out[0]);
        std::vector<int> coord;
        if (splitToNumbers(out[1], coord, ";") == 0) {
          windows.push_back(WindowI(cv::Point(coord[0], coord[1]), cv::Point(coord[2], coord[3])));
        }
      }
    }
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
  DetectTransmissionTower *mDetectTower;
  std::string outPath;
  bool bSaveImages;
  double mCurrentPosition;

private:
  cv::Mat mFramePrev;
  cv::Mat out;
  std::string outFile;
  
  std::vector<std::string> framesSaved;
  std::vector<WindowI> windowsSaved;

  //DOMImplementation *impl;

public:

  VideoHelper(DetectTransmissionTower *detectTower, const std::string &out_path, bool save_frames) 
    : mDetectTower(detectTower), outPath(out_path), bSaveImages(save_frames), mCurrentPosition(0.)
  {
    outFile = "TowerDetected.txt";
  }

  ~VideoHelper() {}

  void onFinish() override;

  void onInitialize() override;

  void onPause() override;

  void onPositionChange(double position) override;

  void onRead(cv::Mat &frame) override;

  void onResume() override;

  void onShow(cv::Mat &frame) override;

  void onStop() override;

  void write();
};

void VideoHelper::onFinish()
{
  VideoStream::Listener::onFinish();

  write();
}

void VideoHelper::onInitialize()
{
  VideoStream::Listener::onInitialize();

  if (outPath.empty()) {
    char path[I3D_MAX_DRIVE + I3D_MAX_DIR];
    getFileDriveDir(getRunfile(), path, I3D_MAX_DRIVE + I3D_MAX_DIR);
    outPath = path;
  } else {
    createDir(outPath.c_str());
  }
}

void VideoHelper::onPause()
{
  VideoStream::Listener::onPause(); 
}

void VideoHelper::onPositionChange(double position) 
{ 
  VideoStream::Listener::onPositionChange(position);
  mDetectTower->prevFrame = position;
  mCurrentPosition = position;
}

//void VideoHelper::onRead(cv::Mat &frame) 
//{
//  VideoStream::Listener::onRead(frame);
//  if (!mFramePrev.empty()) {
//    WindowI wOut;
//    //bool bTower = mDetectTower->run(mFramePrev, frame, &out, &wOut);
//    bool bTower = false;
//    cv::Size sz = mFramePrev.size(); 
//    mFramePrev.copyTo(out);
//  
//    ////Ventana en la cual se van a buscar los apoyos
//    //WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
//    //ws = expandWindow(ws, -100, 0);
//
//    cv::Mat image1, image2;
//
//    // Conversión a escala de grises
//    cvtColor(mFramePrev, image1, CV_BGR2GRAY);
//    cvtColor(frame, image2, CV_BGR2GRAY);
//  
//    std::vector<ldGroupLines> linesGroup1, linesGroup2;
//    //mDetectTower->detectGroupLines(image1, &linesGroup1);
//    mDetectTower->detectGroupLines(image2, &linesGroup2);
//
//    if (linesGroup1.size() > 0 && linesGroup2.size() > 0) {
//      //determinación del flujo óptico
//      cv::Mat_<cv::Point2f> flow;
//      mDetectTower->computeOpticalFlow(image1, image2, &flow);
//      image1.release();
//      image2.release();
//
//      // Se comparan todos los grupos de lineas dos a dos
//      for (int iplg = 0; iplg < linesGroup1.size(); iplg++) {
//        WindowI wprev = linesGroup1[iplg].getBbox();
//        for (int iclg = 0; iclg < linesGroup2.size(); iclg++) {
//          WindowI wcur = linesGroup2[iclg].getBbox();
//
//          // Se comprueba la ventana actual contiene el centro de la ventana previa
//          if (wcur.containsPoint(wprev.getCenter())) {
//
//            // Se obtiene la magnitud a partir del flujo óptico.
//            cv::Mat magnitude;
//             mDetectTower->getMagnitude(flow, &magnitude);
//
//            if (bTower =  mDetectTower->isTower(&out, linesGroup1[iplg], magnitude)) {
//              wOut = linesGroup1[iplg].getBbox();
//            }
//          }
//        }
//      }
//    }
//
//
//
//    frame.copyTo(mFramePrev);
//    out.copyTo(frame);
//    if (bTower) {
//      char buffer[I3D_MAX_PATH];
//      sprintf_s(buffer, "%s\\Apoyo_%05i.png", outPath.c_str(), I3D_ROUND_TO_INT(mCurrentPosition));
//      if (bSaveImages) {
//        cv::imwrite(buffer, out);
//        framesSaved.push_back(std::string(buffer));
//        windowsSaved.push_back(wOut);      
//      }
//    }
//  } else {
//    frame.copyTo(mFramePrev);
//  }
//}

void VideoHelper::onRead(cv::Mat &frame) 
{
  VideoStream::Listener::onRead(frame);
  if (!mFramePrev.empty()) {
    WindowI wOut;
    bool bTower = mDetectTower->run(mFramePrev, frame, &out, &wOut);
    frame.copyTo(mFramePrev);
    out.copyTo(frame);
    if (bTower) {
      char buffer[I3D_MAX_PATH];
      sprintf_s(buffer, "%s\\Apoyo_%05i.png", outPath.c_str(), I3D_ROUND_TO_INT(mCurrentPosition));
      if (bSaveImages) {
        cv::imwrite(buffer, out);
        framesSaved.push_back(std::string(buffer));
        windowsSaved.push_back(wOut);      
      }
    }
  } else {
    frame.copyTo(mFramePrev);
  }
}

void VideoHelper::onResume() 
{ 
  VideoStream::Listener::onResume();
}

void VideoHelper::onShow(cv::Mat &frame) 
{ 
  VideoStream::Listener::onShow(frame);
}

void VideoHelper::onStop()
{ 
  VideoStream::Listener::onStop(); 
}

void VideoHelper::write()
{ 
  char buffer[I3D_MAX_PATH];
  sprintf_s(buffer, "%s\\%s", outPath.c_str(), outFile.c_str());
  std::string name = outPath + "\\TowerDetected.txt";
  std::remove(buffer);
  std::ofstream hWrite(buffer);
  for (size_t i = 0; i < framesSaved.size(); ++i) {
    
    if (hWrite.is_open()) {
      hWrite << framesSaved[i].c_str() << "|" << windowsSaved[i].pt1.x <<  ";" << windowsSaved[i].pt1.y <<  ";" << windowsSaved[i].pt2.x <<  ";" << windowsSaved[i].pt2.y << "\n";
    }
  }
  hWrite.close();
}




/*!
 * Detección de apoyos:
 *
 * video:       Video en el que vamos a buscar las torres
 * out:         Directorio de salida donde se guardan las imagenes y otra información de salida
 * si:          Guarda imagenes
 * dl:          Dibuja lineas
 * show_video:  Muestra el video mientras corre el programa
 * skip_frames: Salto de frames que se aplica al video
 * l_detect:    Detector de lineas. Puede ser HOUGH, HOUGHP, HOUGH_FAST o LSD
 */
int main(int argc, char** argv)
{

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DIR);

  CmdParser cmdParser(name, "Detección de apoyos");
  cmdParser.addParameter("video", "Video en el que vamos a buscar las torres");
  cmdParser.addParameter("out", "Directorio de salida donde se guardan las imagenes y otra información de salida", true, dir);
  cmdParser.addOption("si", "Guarda imagenes", true);
  cmdParser.addOption("dl", "Dibuja lineas", true);
  cmdParser.addOption("drl", "Dibuja recta de regresión", true);
  cmdParser.addOption("show_video", "Muestra el video mientras corre el programa", true);
  cmdParser.addParameter("skip_frames", "Salto de frames", true, "1");
  cmdParser.addParameterOption("l_detect", "HOUGH,HOUGHP,HOUGH_FAST,LSD", "Detector de lineas", true, "HOUGHP");
  if (cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp(); 
    exit(EXIT_FAILURE);
  }

  std::string video = cmdParser.getValue<std::string>("video");
  std::string out_path = cmdParser.getValue<std::string>("out");
  bool bSaveImages = cmdParser.hasOption("si");
  bool bDrawLines = cmdParser.hasOption("dl");
  bool bDrawRegressionLine = cmdParser.hasOption("drl");
  bool bShowVideo = cmdParser.hasOption("show_video");
  int skip_frames = cmdParser.getValue<int>("skip_frames");
  //std::string l_detect = cmdParser.getValue<std::string>("l_detect");
  LD_TYPE ls = cmdParser.getParameterOptionIndex<LD_TYPE>("l_detect");

  // Barra de progreso
  //ProgressBar progress_bar;

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  sprintf(logfile, "%s//%s.log", out_path.c_str(), name );
  Message::setMessageLogFile(logfile);
  Message::setMessageLevel(MessageLevel::MSG_INFO);

  // Lectura de video
  VideoStream strmVideo(video.c_str());
  if (!strmVideo.isOpened()) {
    printError("No se ha podido cargar el video: %s", video.c_str());
    exit(EXIT_FAILURE);
  }

  // Salto de frames
  strmVideo.setSkipFrames(skip_frames);

  VideoWindow vc("Cloud Points", WINDOW_AUTOSIZE);
  if (bShowVideo) {
    vc.setVideo(&strmVideo);
  }
  
  // Se crea el detector de líneas
  std::shared_ptr<I3D::LineDetector> pLineDetector;
  cv::Scalar ang_tol(0., 0.3);
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

  // Procesos previos que se aplican a las imagenes
  std::shared_ptr<I3D::ImgProcessingList> imgprolist = std::make_shared<I3D::ImgProcessingList>();
  imgprolist->add(std::make_shared<I3D::Canny>());

  // Detección de apoyos
  DetectTransmissionTower detectTower(pLineDetector, imgprolist, bDrawRegressionLine, bDrawLines);
  VideoHelper videoHelper(&detectTower, out_path, bSaveImages);
  strmVideo.addListener(&videoHelper);
  videoHelper.outPath = out_path;

  strmVideo.run();

  return 0;
}














//#include <windows.h>
//#include <memory>
//#include <ctime> //For demo
//#include <chrono>
//#include <thread>
//
//// Cabeceras de OpenCV
//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/optflow.hpp"
//
//// Cabeceras propias
//#include "linedetector.h"
//#include "videostream.h"
//#include "core/utils.h"
//#include "Logger.h"
//#include "img_processing.h"
//#include "geometric_entities\segment.h"
//#include "geometric_entities\window.h"
//#include "core/console.h"
//
//using namespace std;
//using namespace cv;
//using namespace optflow;
//using namespace I3D;
//
//
//// Variales globales
//int min_threshold = 0;
//int max_trackbar = 255;
//const char* wname = "Line Detector";
//cv::RNG rng(12345);
//const char* source_window = "Corner detector";
//bool bCanny = true;
//bool bSobel = false;
//// GaussianBlur
//cv::Size gbsize(3, 3);
//double gbsigmax = 2.;
//double gbsigmay = 2.;
//int gbbordertype = 4;
////Detectores de lineas
//double angle = 0;
//double tol = 0.25;
//int thresholdH = 160;
//int thresholdHP = 75;
//VideoStream::Skip sf = VideoStream::Skip::NOT_SKIP;
//
//int SkpFrames = 2;
//int SkpSeconds = -1;
//bool bDrawRegressionLine = false;
//bool bDrawLines = false;
//bool bSaveImages = true;
//bool bDrawDP = false;
//
//void onTrackbarChange(int _pos, void *up)
//{
//  LineDetector *ld = (LineDetector *)up;
//  int pos = min_threshold + _pos;
//  if (ld->getType() == LD_TYPE::HOUGH) {
//    static_cast<ldHouh *>(ld)->setThreshold(pos);
//  } else if (ld->getType() == LD_TYPE::HOUGHP) {
//    static_cast<ldHouhP *>(ld)->setThreshold(pos);
//  }
//}
//
//int previousProcesses(cv::Mat &matIn, cv::Mat &matOut) 
//{
//  I3D::ImgProcessingList imgprolist;
//  // Canny con media y desviación estandar
//  if (bCanny) {
//    imgprolist.add(std::make_shared<I3D::Canny>());
//  } else if (bSobel) {
//    imgprolist.add(std::make_shared<I3D::GaussianBlur>(gbsize, gbsigmax, gbsigmay, gbbordertype));
//    imgprolist.add(std::make_shared<I3D::Sobel>(1, 0, 3, 1., 0., CV_16S));
//  }
//  return imgprolist.execute(matIn, &matOut);
//}
//
//cv::Mat prevVideoFrame, prevVideoFrameGray;
//vector<ldGroupLines> prevLinesGrops;
//double prevFrame;
//
//void onRun(VideoStream *strmVideo, void* userdata)
//{
//  LineDetector *ld = (LineDetector *)userdata;
//  cv::Mat curVideoFrame, curVideoFrameGray;
//  cv::Mat_<cv::Point2f> flow;
//
//  cv::Size sz = strmVideo->getFrameSize();
//  
//  ////Ventana en la cual se van a buscar los apoyos
//  //WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
//  //ws = expandWindow(ws, -100, 0);
//
//  try {
//
//    if (!strmVideo->nextFrame(&curVideoFrame)) {
//      strmVideo->stop();
//      return;
//    }
//    double curFrame = strmVideo->getCurrentFrame();
//    cvtColor(curVideoFrame, curVideoFrameGray, CV_BGR2GRAY);
//
//    cv::Mat imgout;
//    prevVideoFrame.copyTo(imgout);
//    cv::Mat mOut;
//    int i_error = previousProcesses(curVideoFrameGray, mOut);
//    // if (i_error) ... //Error en los procesos previos
//
//    // Se ejecuta el detector de lineas
//    ld->run(mOut);
//
//    //... Pinto todas las lineas
//    //ld->DrawLines(imgout, cv::Scalar(255, 0, 0), 2);
//    //
//
//
//    // Agrupamos por lineas
//    vector<ldGroupLines> curLinesGrops;
//    groupLinesByDist(ld->getLines(), &curLinesGrops, 10);
//
//    // Se eliminan los grupos con pocas lineas
//    delLinesGroupBySize(&curLinesGrops,10);
//
//    if (curLinesGrops.size() > 0 && prevLinesGrops.size() > 0) {
//      flow = cv::Mat(curVideoFrameGray.size[0], curVideoFrameGray.size[1], CV_32FC2);
//      Ptr<DenseOpticalFlow> algorithm = createOptFlow_Farneback();
//      algorithm->calc(prevVideoFrameGray, curVideoFrameGray, flow);
//
//      cv::Mat flow_split[2];
//      cv::Mat magnitude, angle;
//
//      cv::Scalar c;
//
//      for (int iplg = 0; iplg < prevLinesGrops.size(); iplg++) {
//        ldGroupLines *lgp = &prevLinesGrops[iplg];
//        WindowI wprev = lgp->getBbox();
//        for (int iclg = 0; iclg < curLinesGrops.size(); iclg++) {
//          ldGroupLines *lgc = &curLinesGrops[iclg];
//          WindowI wcur = lgc->getBbox();
//          //...
//          //for (int il = 0; il < lgc->getSize(); il++) {
//          //  cv::Point pt1 = (*lgc)[il].pt1;
//          //  cv::Point pt2 = (*lgc)[il].pt2;
//          //  line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
//          //}
//          //...
//          // Se comprueba la ventana actual contiene el centro de la ventana previa
//          // Se comprueba que las ventanas esten dentro de la zona de busqueda de las torres
//          if (wcur.containsPoint(wprev.getCenter()) /*&& ContainsWindow(ws,wcur) && ContainsWindow(ws,wprev)*/) {
//
//            split(flow, flow_split);
//            //Magnitud y angulo del desplazamiento
//            cartToPolar(flow_split[0], flow_split[1], magnitude, angle, true);
//
//            cv::Mat canditate;
//            magnitude.colRange(wprev.pt1.x, wprev.pt2.x).rowRange(wprev.pt1.y, wprev.pt2.y).copyTo(canditate);
//
//            // Normalizamos la magnitud para ver si hay valores mas altos en la zona de la posible torre
//            cv::Scalar mean, stdv, meanc, stdvc;
//            cv::Mat normMag, candidatenorm;
//            cv::normalize(magnitude, normMag, 0, 1, NORM_MINMAX);
//            cv::meanStdDev(normMag, mean, stdv);
//            normMag.colRange(wprev.pt1.x, wprev.pt2.x).rowRange(wprev.pt1.y, wprev.pt2.y).copyTo(candidatenorm);
//            cv::meanStdDev(candidatenorm, meanc, stdvc);
//
//            if (mean[0] < meanc[0] && meanc[0] > 0.5 && stdvc[0] > 0.1 /*&& (mean[0] + stdv[0]) < meanc[0]*/) {
//
//              c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//
//              // Buscamos los máximos en la zona
//              vector<Point> pMax;
//              cv::Mat idx;
//              cv::sortIdx(candidatenorm, idx, CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);
//              for (int ir = 0; ir < candidatenorm.rows; ir++) {
//                int id;
//                for (int ic = 0; ic < 5; ic++) {
//                  id = idx.at<int>(ir, ic);
//                  float cmax = candidatenorm.at<float>(ir, id);
//                  if (cmax != 0. && cmax > mean[0] + stdv[0]) {
//                    cv::Point paux(wprev.pt1.x + id, wprev.pt1.y + ir);
//                    pMax.push_back(paux);
//                    if (bDrawRegressionLine)
//                      line(imgout, paux, paux, c, 1, cv::LINE_8);
//                    //line(imgout, cv::Point(cmax*50, ir), cv::Point(cmax*50, ir), c, 1, cv::LINE_8);
//                  } else break;
//                }
//              }
//
//
//              LogMsg::debugMsg("Frame %i - cols: %i - rows: %i", I3D_ROUND_TO_INT(prevFrame), candidatenorm.cols, candidatenorm.rows);
//              LogMsg::debugMsg("Frame %i - N points max: %i", I3D_ROUND_TO_INT(prevFrame), pMax.size());
//              if (pMax.size() > 200 ) {
//                // Recta de regresión para los máximos
//                double m = 0.;
//                double b = 0.;
//                regressionLinearXY(pMax, &m, &b);
//
//                cv::Point pt1(I3D_ROUND_TO_INT(b), 0);
//                cv::Point pt2(I3D_ROUND_TO_INT(m * magnitude.rows + b), magnitude.rows);
//                
//                if (bDrawRegressionLine) {
//                  //Se pinta la recta de regresión
//                  line(imgout, pt1, pt2, cv::Scalar(0, 0, 150), 1, cv::LINE_8);
//                }
//                
//
//                //Buscamos sólo cuando la torre este mas vertical
//                Line rr(pt1, pt2);
//                double ang = rr.angleOY();
//                if (ang > CV_PI / 2)  ang = ang - CV_PI;
//                else if (ang < -CV_PI / 2) ang = ang + CV_PI;
//                // tolerancia de inclinación del eje del apoyo respecto a la vertical -> 0.1
//                if (ang <= 0.1 && ang >= -0.1) {
//                  LogMsg::debugMsg("Frame %i - Angulo: %f", I3D_ROUND_TO_INT(prevFrame), ang);
//
//                  // Busqueda del máximo valor de desplazamiento
//                  std::vector<Point> vMagnitudes;
//                  cv::Point ptMax;
//                  float maxval = I3D_FLOAT_MIN;
//                  for (int ir = 0; ir < normMag.rows; ir++) {
//                    cv::Point pt(I3D_ROUND_TO_INT(m * ir + b), ir);
//                    if (pt.x >= 0 && pt.x < normMag.cols) {
//                      float mg = magnitude.at<float>(pt);
//                      if (mg > maxval){
//                        maxval = mg;
//                        ptMax = pt;
//                      }
//                      if (mg > mean[0])
//                        vMagnitudes.push_back(cv::Point(static_cast<int>(mg), ir));
//                    }
//                  }
//
//
//                  // Aplicamos douglas-Peucker
//                  std::vector<Point> lmag;
//                  approxPolyDP(vMagnitudes, lmag, 3, false);
//                  if (bDrawDP) {
//                    if (lmag.size() >= 3) {
//                      const cv::Point *cpts2 = (const cv::Point*) Mat(lmag).data;
//                      int npts = Mat(lmag).rows;
//                      cv::polylines(imgout, &cpts2, &npts, 1, false, Scalar(255, 255, 0), 2);
//                    }
//                  }
//
//                  //El punto máximo tiene que estar en la parte superior
//                  if (abs(wprev.pt1.y - ptMax.y) <= 200) {
//
//                    //Rectangulo envolvente de la torre            
//                    cv::rectangle(imgout, wprev.pt1, wprev.pt2, cv::Scalar(0, 255, 0), 1);
//
//                    if (bDrawLines){
//                      for (int il = 0; il < lgp->getSize(); il++) {
//                        cv::Point pt1 = (*lgp)[il].pt1;
//                        cv::Point pt2 = (*lgp)[il].pt2;
//                        line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
//                      }
//                    }
//                    
//
//                    // Ajustar el BBOX mejor
//                        
//                    LogMsg::infoMsg("Torre detectada: Frame %i", I3D_ROUND_TO_INT(prevFrame));
//                    if (bSaveImages) {
//                      char buffer[50];
//                      sprintf_s(buffer, "Apoyo_%05i.jpg", I3D_ROUND_TO_INT(prevFrame));
//                      cv::imwrite(buffer, imgout);
//                    }
//                    
//                    break; // para que no entre otra vez si ha encontrado una torre.
//                  }
//                  //} else LogMsg::DebugMsg("Frame %i rechazado. lmag.size() < 3", cvRound(prevFrame));
//                } else LogMsg::debugMsg("Frame %i rechazado por angulo de recta de regresion mayor al limite. angulo=%f", I3D_ROUND_TO_INT(prevFrame), ang);
//              }  
//            } else {
//              LogMsg::infoMsg("Torre rechazada: Frame %i", I3D_ROUND_TO_INT(prevFrame) );
//            }
//          }
//        }
//      }
//    }
//
//    if (!curVideoFrame.empty()) {
//      curVideoFrame.copyTo(prevVideoFrame);
//      curVideoFrameGray.copyTo(prevVideoFrameGray);
//      prevLinesGrops = curLinesGrops;
//      prevFrame = curFrame;
//    }
//
//    strmVideo->show(imgout);
//
//  } catch (cv::Exception &e) {
//    LogMsg::errorMsg(e.what());
//    strmVideo->stop();
//  }
//}
//
//const std::string keys =
//"{@imput           |       | imagen o video de entrada }"
//"{si saveimage     | true  | Guarda imagenes }"
//"{dl  DrawLines    | false | Dibuja lineas  }"
//"{drl  DrawRegLine | false | Dibuja recta de regresion  }"
//;
//
//int main(int argc, char *argv[])
//{
//  cv::CommandLineParser parser(argc, argv, keys);
//  string filename = parser.get<string>(0);
//  LD_TYPE ls = LD_TYPE::HOUGHP;//(LD_TYPE)parser.get<int>("ls");
//  bSaveImages = parser.get<bool>("si");
//  bDrawLines = parser.get<bool>("dl");
//  bDrawRegressionLine = parser.get<bool>("drl");
//
//  char logfile[I3D_MAX_PATH];
//  int err = changeFileNameAndExtension(getRunfile(), "LogDeteccionApoyos.log", logfile);
//  LogMsg log(logfile, LogLevel::LOG_DEBUG);
//
//  VideoStream strmVideo(filename.c_str());
//  if (!strmVideo.isOpened()) {
//    log.errorMsg("No se ha podido cargar el video: %s", (char *)filename.c_str());
//    return 0;
//  }
//
//  strmVideo.setFrameSize(cv::Size(800, 720), VideoStream::Resolution::CROP_FRAME, true);
//  strmVideo.setSkipBlurryFrames(true);
//  if (sf == VideoStream::Skip::SKIP_FRAMES) strmVideo.setSkipFrames(SkpFrames);
//  else if (sf == VideoStream::Skip::SKIP_MILLISECONDS) strmVideo.setSkipMillisecond(SkpSeconds);
//
//  VideoWindow vc(wname, WINDOW_AUTOSIZE);
//  vc.SetVideo(&strmVideo);
//
//  //Se crea el detector
//  std::unique_ptr<LineDetector> oLD;
//  cv::Scalar ang_tol(angle, tol);
//  if (ls == LD_TYPE::HOUGH)            oLD = std::make_unique<ldHouh>(150, ang_tol);
//  else if (ls == LD_TYPE::HOUGHP)      oLD = std::make_unique<ldHouhP>(75, ang_tol, 50., 30.);
//  else if (ls == LD_TYPE::HOUGH_FAST)  oLD = std::make_unique<ldHouhFast>();
//  else if (ls == LD_TYPE::LSD)         oLD = std::make_unique<ldLSD>(ang_tol);
//  else {
//    log.errorMsg("No se ha seleccionado ningún detector de lineas.");
//    return 0;
//  }
//   
//  if (ls == LD_TYPE::HOUGH || ls == LD_TYPE::HOUGHP) {
//    int trackbarPos = (ls == LD_TYPE::HOUGH) ? thresholdH : thresholdHP;
//    char thresh_label[50];
//    sprintf_s(thresh_label, "Thres: %d + input", min_threshold);
//    vc.addTrackbar(thresh_label, &trackbarPos, max_trackbar, onTrackbarChange, oLD.get());  
//  }
//
//  strmVideo.setReadListener((ReadCallback)onRun, oLD.get());
//  strmVideo.run();
//
//  return 0;
//}
