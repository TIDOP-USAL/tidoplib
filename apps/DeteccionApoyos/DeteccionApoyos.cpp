#include <windows.h>
#include <memory>
#include <ctime> //For demo
#include <chrono>
#include <thread>

// Cabeceras de OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/optflow.hpp"

// Cabeceras propias
#include "LineDetector.h"
#include "VideoStream.h"
#include "utils.h"
#include "Logger.h"
#include "ImgProcessing.h"
#include "geometric_entities\segment.h"
#include "geometric_entities\window.h"
#include "core/console.h"

using namespace std;
using namespace cv;
using namespace optflow;
using namespace I3D;

const std::string keys =
"{@imput           |       | imagen o video de entrada }"
"{si saveimage     | true  | Guarda imagenes }"
"{dl  DrawLines    | false | Dibuja lineas  }"
"{drl  DrawRegLine | false | Dibuja recta de regresion  }"
;

// Variales globales
int min_threshold = 0;
int max_trackbar = 255;
const char* wname = "Line Detector";
cv::RNG rng(12345);
const char* source_window = "Corner detector";
bool bCanny = true;
bool bSobel = false;
// GaussianBlur
cv::Size gbsize(3, 3);
double gbsigmax = 2.;
double gbsigmay = 2.;
int gbbordertype = 4;
//Detectores de lineas
double angle = 0;
double tol = 0.25;
int thresholdH = 160;
int thresholdHP = 75;
skip_video sf = skip_video::NOT_SKIP;
int SkpFrames = 2;
int SkpSeconds = -1;
bool bDrawRegressionLine = false;
bool bDrawLines = false;
bool bSaveImages = true;
bool bDrawDP = false;

void onTrackbarChange(int _pos, void *up)
{
  LineDetector *ld = (LineDetector *)up;
  int pos = min_threshold + _pos;
  if (ld->getType() == LD_TYPE::HOUGH) {
    static_cast<ldHouh *>(ld)->setThreshold(pos);
  } else if (ld->getType() == LD_TYPE::HOUGHP) {
    static_cast<ldHouhP *>(ld)->setThreshold(pos);
  }
}

void delLinesGroupBySize(vector<ldGroupLines> *vlg, int size)
{
  for (int ilg = 0; ilg < vlg->size(); ilg++) {
    ldGroupLines *lg = &vlg->at(ilg);
    if (lg->getSize() < size) {
      vlg->erase(vlg->begin() + ilg);
      ilg--;
    }
  }
}

int previousProcesses(cv::Mat &matIn, cv::Mat &matOut) 
{
  I3D::ImgProcessingList imgprolist;
  // Canny con media y desviación estandar
  if (bCanny) {
    imgprolist.add(std::make_shared<I3D::Canny>());
  } else if (bSobel) {
    imgprolist.add(std::make_shared<I3D::GaussianBlur>(gbsize, gbsigmax, gbsigmay, gbbordertype));
    imgprolist.add(std::make_shared<I3D::Sobel>(1, 0, 3, 1., 0., CV_16S));
  }
  return imgprolist.execute(matIn, &matOut);
}

cv::Mat prevVideoFrame, prevVideoFrameGray;
vector<ldGroupLines> prevLinesGrops;
double prevFrame;

void onRun(VideoStream *strmVideo, void* userdata)
{
  LineDetector *ld = (LineDetector *)userdata;
  cv::Mat curVideoFrame, curVideoFrameGray;
  cv::Mat_<cv::Point2f> flow;

  cv::Size sz = strmVideo->getFrameSize();
  
  //Ventana en la cual se van a buscar los apoyos
  WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
  ws = expandWindow(ws, -100, 0);

  try {

    if (!strmVideo->nextFrame(curVideoFrame)) {
      strmVideo->stop();
      return;
    }
    double curFrame = strmVideo->getCurrentFrame();
    cvtColor(curVideoFrame, curVideoFrameGray, CV_BGR2GRAY);

    cv::Mat imgout;
    prevVideoFrame.copyTo(imgout);
    cv::Mat mOut;
    int i_error = previousProcesses(curVideoFrameGray, mOut);
    // if (i_error) ... //Error en los procesos previos

    // Se ejecuta el detector de lineas
    ld->run(mOut);

    //... Pinto todas las lineas
    //ld->DrawLines(imgout, cv::Scalar(255, 0, 0), 2);
    //


    // Agrupamos por lineas
    vector<ldGroupLines> curLinesGrops;
    groupLinesByDist(ld->getLines(), &curLinesGrops, 10);

    // Se eliminan los grupos con pocas lineas
    delLinesGroupBySize(&curLinesGrops,10);

    if (curLinesGrops.size() > 0 && prevLinesGrops.size() > 0) {
      flow = cv::Mat(curVideoFrameGray.size[0], curVideoFrameGray.size[1], CV_32FC2);
      Ptr<DenseOpticalFlow> algorithm = createOptFlow_Farneback();
      algorithm->calc(prevVideoFrameGray, curVideoFrameGray, flow);

      cv::Mat flow_split[2];
      cv::Mat magnitude, angle;

      cv::Scalar c;

      for (int iplg = 0; iplg < prevLinesGrops.size(); iplg++) {
        ldGroupLines *lgp = &prevLinesGrops[iplg];
        WindowI wprev = lgp->getBbox();
        for (int iclg = 0; iclg < curLinesGrops.size(); iclg++) {
          ldGroupLines *lgc = &curLinesGrops[iclg];
          WindowI wcur = lgc->getBbox();
          //...
          //for (int il = 0; il < lgc->getSize(); il++) {
          //  cv::Point pt1 = (*lgc)[il].pt1;
          //  cv::Point pt2 = (*lgc)[il].pt2;
          //  line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
          //}
          //...
          // Se comprueba la ventana actual contiene el centro de la ventana previa
          // Se comprueba que las ventanas esten dentro de la zona de busqueda de las torres
          if (wcur.containsPoint(wprev.getCenter()) /*&& ContainsWindow(ws,wcur) && ContainsWindow(ws,wprev)*/) {

            split(flow, flow_split);
            //Magnitud y angulo del desplazamiento
            cartToPolar(flow_split[0], flow_split[1], magnitude, angle, true);

            cv::Mat canditate;
            magnitude.colRange(wprev.pt1.x, wprev.pt2.x).rowRange(wprev.pt1.y, wprev.pt2.y).copyTo(canditate);

            // Normalizamos la magnitud para ver si hay valores mas altos en la zona de la posible torre
            cv::Scalar mean, stdv, meanc, stdvc;
            cv::Mat normMag, candidatenorm;
            cv::normalize(magnitude, normMag, 0, 1, NORM_MINMAX);
            cv::meanStdDev(normMag, mean, stdv);
            normMag.colRange(wprev.pt1.x, wprev.pt2.x).rowRange(wprev.pt1.y, wprev.pt2.y).copyTo(candidatenorm);
            cv::meanStdDev(candidatenorm, meanc, stdvc);

            if (mean[0] < meanc[0] && meanc[0] > 0.5 && stdvc[0] > 0.1 /*&& (mean[0] + stdv[0]) < meanc[0]*/) {

              c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

              // Buscamos los máximos en la zona
              vector<Point> pMax;
              cv::Mat idx;
              cv::sortIdx(candidatenorm, idx, CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);
              for (int ir = 0; ir < candidatenorm.rows; ir++) {
                int id;
                for (int ic = 0; ic < 5; ic++) {
                  id = idx.at<int>(ir, ic);
                  float cmax = candidatenorm.at<float>(ir, id);
                  if (cmax != 0. && cmax > mean[0] + stdv[0]) {
                    cv::Point paux(wprev.pt1.x + id, wprev.pt1.y + ir);
                    pMax.push_back(paux);
                    if (bDrawRegressionLine)
                      line(imgout, paux, paux, c, 1, cv::LINE_8);
                    //line(imgout, cv::Point(cmax*50, ir), cv::Point(cmax*50, ir), c, 1, cv::LINE_8);
                  } else break;
                }
              }


              LogMsg::debugMsg("Frame %i - cols: %i - rows: %i", cvRound(prevFrame), candidatenorm.cols, candidatenorm.rows);
              LogMsg::debugMsg("Frame %i - N points max: %i", cvRound(prevFrame), pMax.size());
              if (pMax.size() > 200 ) {
                // Recta de regresión para los máximos
                double m = 0.;
                double b = 0.;
                regressionLinearXY(pMax, &m, &b);

                cv::Point pt1(cvRound(b), 0);
                cv::Point pt2(cvRound(m * magnitude.rows + b), magnitude.rows);
                
                if (bDrawRegressionLine) {
                  //Se pinta la recta de regresión
                  line(imgout, pt1, pt2, cv::Scalar(0, 0, 150), 1, cv::LINE_8);
                }
                

                //Buscamos sólo cuando la torre este mas vertical
                Line rr(pt1, pt2);
                double ang = rr.angleOY();
                if (ang > CV_PI / 2)  ang = ang - CV_PI;
                else if (ang < -CV_PI / 2) ang = ang + CV_PI;
                // tolerancia de inclinación del eje del apoyo respecto a la vertical -> 0.1
                if (ang <= 0.1 && ang >= -0.1) {
                  LogMsg::debugMsg("Frame %i - Angulo: %f", cvRound(prevFrame), ang);

                  // Busqueda del máximo valor de desplazamiento
                  std::vector<Point> vMagnitudes;
                  cv::Point ptMax;
                  float maxval = I3D_FLOAT_MIN;
                  for (int ir = 0; ir < normMag.rows; ir++) {
                    cv::Point pt(cvRound(m * ir + b), ir);
                    if (pt.x >= 0 && pt.x < normMag.cols) {
                      float mg = magnitude.at<float>(pt);
                      if (mg > maxval){
                        maxval = mg;
                        ptMax = pt;
                      }
                      if (mg > mean[0])
                        vMagnitudes.push_back(cv::Point(static_cast<int>(mg), ir));
                    }
                  }


                  // Aplicamos douglas-Peucker
                  std::vector<Point> lmag;
                  approxPolyDP(vMagnitudes, lmag, 3, false);
                  if (bDrawDP) {
                    if (lmag.size() >= 3) {
                      const cv::Point *cpts2 = (const cv::Point*) Mat(lmag).data;
                      int npts = Mat(lmag).rows;
                      cv::polylines(imgout, &cpts2, &npts, 1, false, Scalar(255, 255, 0), 2);
                    }
                  }

                  //El punto máximo tiene que estar en la parte superior
                  if (abs(wprev.pt1.y - ptMax.y) <= 200) {

                    //Rectangulo envolvente de la torre            
                    cv::rectangle(imgout, wprev.pt1, wprev.pt2, cv::Scalar(0, 255, 0), 1);

                    if (bDrawLines){
                      for (int il = 0; il < lgp->getSize(); il++) {
                        cv::Point pt1 = (*lgp)[il].pt1;
                        cv::Point pt2 = (*lgp)[il].pt2;
                        line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
                      }
                    }
                    

                    // Ajustar el BBOX mejor
                        
                    LogMsg::infoMsg("Torre detectada: Frame %i", cvRound(prevFrame));
                    if (bSaveImages) {
                      char buffer[50];
                      sprintf_s(buffer, "Apoyo_%05i.jpg", cvRound(prevFrame));
                      cv::imwrite(buffer, imgout);
                    }
                    
                    break; // para que no entre otra vez si ha encontrado una torre.
                  }
                  //} else LogMsg::DebugMsg("Frame %i rechazado. lmag.size() < 3", cvRound(prevFrame));
                } else LogMsg::debugMsg("Frame %i rechazado por angulo de recta de regresion mayor al limite. angulo=%f", cvRound(prevFrame), ang);
              }  
            } else {
              LogMsg::infoMsg("Torre rechazada: Frame %i", cvRound(prevFrame) );
            }
          }
        }
      }
    }

    if (!curVideoFrame.empty()) {
      curVideoFrame.copyTo(prevVideoFrame);
      curVideoFrameGray.copyTo(prevVideoFrameGray);
      prevLinesGrops = curLinesGrops;
      prevFrame = curFrame;
    }

    strmVideo->show(imgout);

  } catch (cv::Exception &e) {
    LogMsg::errorMsg(e.what());
    strmVideo->stop();
  }
}

int main(int argc, char *argv[])
{
  int kkint;

  CmdParser kk( argc, argv);
  kk.options = { CmdOption("c"), CmdOption("d") };
  kk.parameters = { CmdParameterInt("value", &kkint) };

  cv::CommandLineParser parser(argc, argv, keys);
  string filename = parser.get<string>(0);
  LD_TYPE ls = LD_TYPE::HOUGHP;//(LD_TYPE)parser.get<int>("ls");
  bSaveImages = parser.get<bool>("si");
  bDrawLines = parser.get<bool>("dl");
  bDrawRegressionLine = parser.get<bool>("drl");

  char logfile[_MAX_PATH];
  int err = changeFileNameAndExtension(getRunfile(), "LogDeteccionApoyos.log", logfile);
  LogMsg log(logfile, LogLevel::LOG_DEBUG);

  //Lectura de xml de configuración.
  //QString xmlfile = QString(path) + "config.xml";
  //QFile file(xmlfile);
  //if (!file.open(QIODevice::ReadOnly)){
  //  log.errorMsg("Error al abrir fichero de configuración");
  //  return 0;
  //}

  //QDomDocument doc;
  //if (!doc.setContent(&file)) {
  //  file.close();
  //  log.errorMsg("Error al leer fichero de configuración");
  //  return 0;
  //}
  //file.close();

  //// Obtención de root
  //QDomElement docElem = doc.documentElement();

  //if (!docElem.isNull() && docElem.hasChildNodes()) {

  //  QDomElement xFilters = docElem.firstChildElement("Filters");
  //  if (!xFilters.isNull() && xFilters.hasChildNodes()){
  //    QDomElement xGaussianBlur = xFilters.firstChildElement("GaussianBlur");
  //    if (!xGaussianBlur.isNull() && xGaussianBlur.hasChildNodes()){
  //      std::string sSize = xGaussianBlur.firstChildElement("Size").text().toStdString();
  //      double n[2];
  //      splitStringToNumbers(sSize.c_str(), n, ',');
  //      gbsize = cv::Size(n[0], n[1]);
  //      gbsigmax = xGaussianBlur.firstChildElement("sigmaX").text().toDouble();
  //      gbsigmay = xGaussianBlur.firstChildElement("sigmaY").text().toDouble();
  //      gbbordertype = xGaussianBlur.firstChildElement("borderType").text().toInt();
  //    }
  //  }

  //  QDomElement xLineDetectors = docElem.firstChildElement("LineDetectors");
  //  if (!xLineDetectors.isNull() && xLineDetectors.hasChildNodes()){
  //    QDomElement xGeneral = xLineDetectors.firstChildElement("General");
  //    if (!xGeneral.isNull() && xGeneral.hasChildNodes()){
  //      angle = xGeneral.firstChildElement("Angle").text().toDouble();
  //      tol = xGeneral.firstChildElement("Tolerance").text().toDouble();
  //    }
  //    QDomElement xHouh = xLineDetectors.firstChildElement("Houh");
  //    if (!xHouh.isNull() && xHouh.hasChildNodes()){
  //      //if (xHouh.attribute("active").toStdString().compare("true") == 0) ls = LD_HOUGH;
  //      thresholdH = xHouh.firstChildElement("threshold").text().toInt();
  //    }
  //    QDomElement xHouhP = xLineDetectors.firstChildElement("HouhP");
  //    if (!xHouhP.isNull() && xHouhP.hasChildNodes()) {
  //      //if (xHouhP.attribute("active").toStdString().compare("true") == 0) ls = LD_HOUGHP;
  //      thresholdHP = xHouhP.firstChildElement("threshold").text().toInt();
  //    }
  //    QDomElement xLSD = xLineDetectors.firstChildElement("LSD");
  //    if (!xLSD.isNull() && xLSD.hasChildNodes()) {
  //      //if (xLSD.attribute("active").toStdString().compare("true") == 0) ls = LD_LSD;
  //    }
  //    QDomElement xLSWMS = xLineDetectors.firstChildElement("LSWMS");
  //    if (!xLSWMS.isNull() && xLSWMS.hasChildNodes()) {
  //      //if (xLSWMS.attribute("active").toStdString().compare("true") == 0) ls = LD_LSWMS;
  //    }
  //  }

  //  QDomElement xVideo = docElem.firstChildElement("Video");
  //  if (!xVideo.isNull() && xVideo.hasChildNodes()){
  //    QDomElement xSkipFrames = xVideo.firstChildElement("SkipFrames");
  //    if (!xSkipFrames.isNull() && xSkipFrames.hasChildNodes()){
  //      QDomElement xFrames = xSkipFrames.firstChildElement("Frames");
  //      QDomElement xMSeconds = xSkipFrames.firstChildElement("Millisecond");
  //      if (!xFrames.isNull() && xFrames.hasChildNodes()){
  //        if (xFrames.attribute("active").toStdString().compare("true") == 0) sf = SKIP_FRAMES;
  //        SkpFrames = xFrames.text().toInt();
  //      }
  //      if (!xMSeconds.isNull() && xMSeconds.hasChildNodes()) {
  //        if (xMSeconds.attribute("active").toStdString().compare("true") == 0) sf = SKIP_MILLISECONDS;
  //        SkpSeconds = xMSeconds.text().toInt();
  //      }
  //    }
  //  }
  //}
  //// Fin lectura config

  VideoStream strmVideo(filename.c_str());
  if (!strmVideo.isOpened()) {
    log.errorMsg("No se ha podido cargar el video: %s", (char *)filename.c_str());
    return 0;
  }

  strmVideo.setFrameSize(cv::Size(800, 720), res_frame::CROP_FRAME, true);
  strmVideo.setSkipBlurryFrames(true);
  if (sf == skip_video::SKIP_FRAMES) strmVideo.setSkipFrames(SkpFrames);
  else if (sf == skip_video::SKIP_MILLISECONDS) strmVideo.setSkipMillisecond(SkpSeconds);

  VideoWindow vc(wname, 1);
  vc.SetVideo(&strmVideo);

  //Se crea el detector
  std::unique_ptr<LineDetector> oLD;
  cv::Scalar ang_tol(angle, tol);
  if (ls == LD_TYPE::HOUGH)            oLD = std::make_unique<ldHouh>(150, ang_tol);
  else if (ls == LD_TYPE::HOUGHP)      oLD = std::make_unique<ldHouhP>(75, ang_tol, 50., 30.);
  else if (ls == LD_TYPE::HOUGH_FAST)  oLD = std::make_unique<ldHouhFast>();
  else if (ls == LD_TYPE::LSD)         oLD = std::make_unique<ldLSD>(ang_tol);
  else {
    log.errorMsg("No se ha seleccionado ningún detector de lineas.");
    return 0;
  }
   
  if (ls == LD_TYPE::HOUGH || ls == LD_TYPE::HOUGHP) {
    int trackbarPos = (ls == LD_TYPE::HOUGH) ? thresholdH : thresholdHP;
    char thresh_label[50];
    sprintf_s(thresh_label, "Thres: %d + input", min_threshold);
    vc.addTrackbar(thresh_label, &trackbarPos, max_trackbar, onTrackbarChange, oLD.get());  
  }

  strmVideo.setReadListener((ReadCallback)onRun, oLD.get());
  strmVideo.run();

  return 0;
}
