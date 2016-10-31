#include "cloud_points.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/optflow.hpp"

// Cabeceras tidopLib
#include "core\console.h"
#include "core\messages.h"
#include "videostream.h"
#include "geometric_entities\segment.h"
#include "linedetector.h"
#include "img_processing.h"

using namespace cv;
using namespace optflow;
using namespace I3D;

namespace I3D
{

void DetectTransmissionTower::detectGroupLines(const cv::Mat &img, std::vector<ldGroupLines> *linesGroup)
{
  cv::Mat imageGrayProcces;
  if (img.channels() != 1)
    cvtColor(img, imageGrayProcces, CV_BGR2GRAY);
  
  // Se aplican procesos previos a las imagenes
  imgprolist.execute(img, &imageGrayProcces);

  pLineDetector->run(imageGrayProcces);
  groupLinesByDist(pLineDetector->getLines(), linesGroup, 10);

  // Se eliminan los grupos con pocas lineas
  delLinesGroupBySize(linesGroup,10);
}

void DetectTransmissionTower::computeOpticalFlow(const cv::Mat &img1, const cv::Mat &img2, cv::Mat_<cv::Point2f> *flow)
{
  *flow = cv::Mat(img1.size[0], img1.size[1], CV_32FC2);
  algorithm->calc(img1, img2, *flow);
}

void DetectTransmissionTower::run(const cv::Mat &img1, const cv::Mat &img2, cv::Mat imgout ) 
{ 
  cv::Size sz = img1.size(); 

  //Ventana en la cual se van a buscar los apoyos
  WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
  ws = expandWindow(ws, -100, 0);

  cv::Mat image1, image2;

  // Conversión a escala de grises
  cvtColor(img1, image1, CV_BGR2GRAY);
  cvtColor(img2, image2, CV_BGR2GRAY);
  
  std::vector<ldGroupLines> linesGroup1, linesGroup2;
  detectGroupLines(image1, &linesGroup1);
  detectGroupLines(image2, &linesGroup2);

  if (linesGroup1.size() > 0 && linesGroup2.size() > 0) {
    //determinación del flujo óptico
    cv::Mat_<cv::Point2f> flow;
    computeOpticalFlow(image1, image2, &flow);
    image1.release();
    image2.release();

    // Se comparan todos los grupos de lineas dos a dos
    for (int iplg = 0; iplg < linesGroup1.size(); iplg++) {
      WindowI wprev = linesGroup1[iplg].getBbox();
      for (int iclg = 0; iclg < linesGroup2.size(); iclg++) {
        WindowI wcur = linesGroup2[iclg].getBbox();

        // Se comprueba la ventana actual contiene el centro de la ventana previa
        if (wcur.containsPoint(wprev.getCenter())) {

          // Se obtiene la magnitud a partir del flujo óptico.
          cv::Mat magnitude;
          getMagnitude(flow, &magnitude);

          // Comprueba si se trata de una torre
          /*bool bTower = */isTower(imgout, linesGroup1[iplg], magnitude);


        }
      }
    }
  }
}

void DetectTransmissionTower::getMagnitude(const cv::Mat_<cv::Point2f> &flow, cv::Mat *magnitude) 
{
  cv::Mat flow_split[2];
  split(flow, flow_split);
  cv::Mat angle; //... Anque no necesito el angulo tengo que pasarselo
  // Magnitud y angulo del desplazamiento. Sólo interesa la magnitud
  cartToPolar(flow_split[0], flow_split[1], *magnitude, angle, true);

}

bool DetectTransmissionTower::isTower(cv::Mat imgout, const ldGroupLines &linesGroup1, const cv::Mat &magnitude) {

  WindowI wprev = linesGroup1.getBbox();

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
    std::vector<Point> pMax;
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
          //if (bDrawRegressionLine)
            //line(imgout, paux, paux, c, 1, cv::LINE_8);
        } else break;
      }
    }

    printVerbose("Frame %i - cols: %i - rows: %i", cvRound(prevFrame), candidatenorm.cols, candidatenorm.rows);
    printVerbose("Frame %i - N points max: %i", cvRound(prevFrame), pMax.size());
    if (pMax.size() > 200 ) {
      // Recta de regresión para los máximos
      double m = 0.;
      double b = 0.;
      regressionLinearXY(pMax, &m, &b);
      
      cv::Point pt1(cvRound(b), 0);
      cv::Point pt2(cvRound(m * magnitude.rows + b), magnitude.rows);
            
      //if (bDrawRegressionLine) {
      //  //Se pinta la recta de regresión
      //  line(imgout, pt1, pt2, cv::Scalar(0, 0, 150), 1, cv::LINE_8);
      //}
            

      //Buscamos sólo cuando la torre este mas vertical
      Line rr(pt1, pt2);
      double ang = rr.angleOY();
      if (ang > CV_PI / 2)  ang = ang - CV_PI;
      else if (ang < -CV_PI / 2) ang = ang + CV_PI;
      // tolerancia de inclinación del eje del apoyo respecto a la vertical -> 0.1
      if (ang <= 0.1 && ang >= -0.1) {
        printVerbose("Frame %i - Angulo: %f", cvRound(prevFrame), ang);

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
        //if (bDrawDP) {
        //  if (lmag.size() >= 3) {
        //    const cv::Point *cpts2 = (const cv::Point*) Mat(lmag).data;
        //    int npts = Mat(lmag).rows;
        //    cv::polylines(imgout, &cpts2, &npts, 1, false, Scalar(255, 255, 0), 2);
        //  }
        //}
       
        //El punto máximo tiene que estar en la parte superior
        if (abs(wprev.pt1.y - ptMax.y) <= 200) {
       
          //Rectangulo envolvente de la torre            
          //cv::rectangle(imgout, wprev.pt1, wprev.pt2, cv::Scalar(0, 255, 0), 1);
       
          //if (bDrawLines){
          //  for (int il = 0; il < linesGroup1.getSize(); il++) {
          //    cv::Point pt1 = linesGroup1[il].pt1;
          //    cv::Point pt2 = linesGroup1[il].pt2;
          //    line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
          //  }
          //}
          
       
          // Ajustar el BBOX mejor
              
          printInfo("Torre detectada: Frame %i", cvRound(prevFrame));
          if (bSaveImages) {
            char buffer[50];
            sprintf_s(buffer, "Apoyo_%05i.jpg", cvRound(prevFrame));
            cv::imwrite(buffer, imgout);
          }
          return true; // Devolvemos que hemos encontrado una torre
        }
      } else printVerbose("Frame %i rechazado por angulo de recta de regresion mayor al limite. angulo=%f", cvRound(prevFrame), ang);
    }
  } else {
    printInfo("Torre rechazada: Frame %i", cvRound(prevFrame) );
    return false;
  }
  return false;
}

} // End namespace I3D


/*!
 * Clase auxiliar para manejar los frames de video. Heredada 
 * de VideoStream::Listener e implementa los métodos que controlan
 * la ejecución de los eventos
 */
class VideoHelper : public VideoStream::Listener
{
public:

  double mCurrentPosition;

  cv::Mat mFramePrev;
  cv::Mat out;

  DetectTransmissionTower *mDetectTower;

public:

  VideoHelper(DetectTransmissionTower *detectTower) 
  {
    mDetectTower = detectTower;
    mCurrentPosition = 0.;
  }

  ~VideoHelper() {}

  void onPause();

  void onPositionChange(double position);

  void onRead(cv::Mat &frame);

  void onResume();

  void onShow(cv::Mat &frame);

  void onStop();

};

void VideoHelper::onPause()
{
  VideoStream::Listener::onPause(); 
}

void VideoHelper::onPositionChange(double position) 
{ 
  VideoStream::Listener::onPositionChange(position);
  mDetectTower->prevFrame = position;
}

void VideoHelper::onRead(cv::Mat &frame) 
{
  VideoStream::Listener::onRead(frame);
  if (!mFramePrev.empty()) {
    mDetectTower->run(mFramePrev, frame, out);
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



int main(int argc, char** argv)
{

  CmdParser cmdParser("cloud_points", "Calculo de nubes de puntos a partir de imagenes de video");
  cmdParser.addParameter("video", "Video en el que vamos a buscar las torres");
  cmdParser.addParameter("out_path", "Directorio de salida");
  cmdParser.parse(argc, argv);
  if (cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp(); // Esto habria que hacerlo directamente en cmdParser.parse y mostrar un mensaje de error por consola
    return 0;
  }
  std::string video = cmdParser.getValue<std::string>("video");
  std::string out_path = cmdParser.getValue<std::string>("out_path");

  // Barra de progreso
  ProgressBar progress_bar;

  //Configuración de log y mensajes por consola
  char logfile[_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

  // Lectura de video
  VideoStream strmVideo(video.c_str());
  if (!strmVideo.isOpened()) {
    printInfo("No se ha podido cargar el video: %s", video.c_str());
    return 0;
  }

  strmVideo.setSkipFrames(1);

  //VideoWindow vc("Cloud Points", WINDOW_AUTOSIZE);
  //vc.setVideo(&strmVideo);

  // Se crea el detector
  double angle = 0;
  double tol = 0.25;
  LD_TYPE ls = LD_TYPE::HOUGHP;
  std::unique_ptr<LineDetector> oLD;
  cv::Scalar ang_tol(angle, tol);
  if (ls == LD_TYPE::HOUGH)            oLD = std::make_unique<ldHouh>(150, ang_tol);
  else if (ls == LD_TYPE::HOUGHP)      oLD = std::make_unique<ldHouhP>(75, ang_tol, 50., 30.);
  else if (ls == LD_TYPE::HOUGH_FAST)  oLD = std::make_unique<ldHouhFast>();
  else if (ls == LD_TYPE::LSD)         oLD = std::make_unique<ldLSD>(ang_tol);
  else {
    printError("No se ha seleccionado ningún detector de lineas.");
    return 0;
  }

  // Detección de apoyos
  DetectTransmissionTower detectTower(oLD.get());
  VideoHelper videoHelper(&detectTower);
  strmVideo.addListener(&videoHelper);


  // No se necesita salida de video
  // VideoWindow vc("", 1);
  // vc.SetVideo(&strmVideo);

  //strmVideo.setReadListener((ReadCallback)onRun, oLD.get());
  //strmVideo.addObserver("",);
  

  //std::shared_ptr<OnPositionChange> onPositionChange(new OnPositionChange);
  //strmVideo.addOnPositionChangeListener( std::make_shared<OnPositionChange>() );

  strmVideo.run();



  return 0;
}
