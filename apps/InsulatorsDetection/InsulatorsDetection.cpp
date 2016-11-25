#include <windows.h>
#include <memory>
#include <thread>
#include <mutex>

// Cabeceras de OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/calib3d.hpp"

// Cabeceras propias
#include "LineDetector.h"
#include "VideoStream.h"
#include "core/utils.h"
#include "core/messages.h"
#include "matching.h"
#include "fourier.h"
#include "img_processing.h"
#include "transform.h"
#include "geometric_entities/point.h"
#include "geometric_entities/operations.h"


using namespace I3D;
using namespace cv;
using namespace cv::xfeatures2d;

using namespace std;
using namespace std::placeholders;

LD_TYPE ls = LD_TYPE::HOUGHP;
std::mutex thread_mutex;

void procesado( const cv::Mat &frame, const WindowI &w, const I3D::ImgProcessingList &imgprolist, const Line &lineVertical, LineDetector *oLD) {
  cv::Mat studyArea;
  frame.colRange(w.pt1.x, w.pt2.x).rowRange(w.pt1.y, w.pt2.y).copyTo(studyArea);

  //Determinar angulo con Fourier
  std::vector<int> colFourier;
  colFourier.push_back(studyArea.cols / 2);
  std::vector<std::vector<cv::Point>> ptsFourier;
  double angle = fourierLinesDetection(studyArea, colFourier, &ptsFourier);

  cv::Mat studyAreaProccesed;

  // Procesos imagen
  imgprolist.execute(studyArea, &studyAreaProccesed);

  cv::Mat studyAreaOut;
  cvtColor(studyArea, studyAreaOut, CV_GRAY2BGR);

  std::vector<Line> lines;
  
  thread_mutex.lock();
  oLD->run(studyAreaOut, cv::Scalar(angle, 0.15));
  if (oLD->getLines().empty()) return;
  Translate<cv::Point> trf(w.pt1.x, w.pt1.y);
  trf.transform(oLD->getLines(), &lines);
  thread_mutex.unlock();

  //Comenzamos a agrupar lineas paralelas
  std::vector<ldGroupLines> linesGroups;
  groupParallelLines(lines, &linesGroups, 0.015);
  cv::RNG rng(12345);
  cv::Scalar c;

  int imaxr = 0;
  int maxr = 0;
  for (int ilg = 0; ilg < linesGroups.size(); ilg++) {
    if (maxr < linesGroups[ilg].getSize()){
      maxr = linesGroups[ilg].getSize();
      imaxr = ilg;
    }
  }

  std::vector<ldGroupLines> linesGroups2;
  groupLinesByDist(linesGroups[imaxr].getLines(), &linesGroups2, 10);

  for (size_t ig = 0; ig < linesGroups2.size(); ig++) {
    std::vector<Line> linesJoin;
    joinLinesByDist(linesGroups2[ig].getLines(), &linesJoin, 3);
    linesGroups2[ig] = ldGroupLines(linesJoin);
  }

  for (size_t ig = 0; ig < linesGroups2.size(); ig++) {
    c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    for (int il = 0; il < linesGroups2[ig].getSize(); il++) {
      line(studyAreaOut, linesGroups2[ig][il].pt1, linesGroups2[ig][il].pt2, c, 1, cv::LINE_8);
    }
  }

  //interseccion con lineas
  std::vector<cv::Point> pts;
  for (size_t ig = 0; ig < linesGroups2.size(); ig++) {
    for (int ir = 0; ir < linesGroups2[ig].getSize(); ir++) {
      cv::Point ptAux;
      intersectLines(linesGroups2[ig][ir], lineVertical, &ptAux);
      pts.push_back(ptAux);
    }
  }

  std::vector<int> posVertical(pts.size());
  for (int ir = 0; ir < pts.size(); ir++) {
    posVertical[ir] = pts[ir].y;
  }

  std::vector<cv::Point> ptsSort;
  for (auto i : sortIdx(posVertical)) {
    ptsSort.push_back(pts[i]);
  }

  for (int ir = 0; ir < ptsSort.size(); ir++) {
    cv::line(studyAreaOut, ptsSort[ir], ptsSort[ir], Scalar(255, 0, 0), 3, LINE_AA);
  }
}	


int main(int argc, char *argv[])
{
  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name);
  char dir[I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir);

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  sprintf(logfile, "%s//%s.log", dir, name );
  Message::setMessageLogFile(logfile);
  Message::setMessageLevel(MessageLevel::MSG_INFO);

  std::unique_ptr<LineDetector> oLD;
  cv::Scalar ang_tol(CV_PI / 2, 0.25);
  if (ls == LD_TYPE::HOUGH)            oLD = std::make_unique<ldHouh>(150, ang_tol);
  else if (ls == LD_TYPE::HOUGHP)      oLD = std::make_unique<ldHouhP>(100, ang_tol, 60., 30.);
  else if (ls == LD_TYPE::HOUGH_FAST)  oLD = std::make_unique<ldHouhFast>();
  else if (ls == LD_TYPE::LSD)         oLD = std::make_unique<ldLSD>(ang_tol);
  else {
    logPrintError("No se ha seleccionado ningún detector de lineas.");
    return 0;
  }

  //Procesos que se aplican a las imagenes
  std::shared_ptr<I3D::Normalize> normalize = std::make_shared<I3D::Normalize>(0., 255.);
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(1);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(1);

  std::shared_ptr<FunctionProcess> fProcess1 = std::make_shared<FunctionProcess>(
    [](const cv::Mat &in, cv::Mat *out) {
      in.convertTo(*out, CV_32F);
  });
  int kernel_size = 31;
  double sig = 1., th = I3D_PI / 2., lm = 1.0, gm = 0.02, ps = 0.;
  cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, th, lm, gm);
  std::shared_ptr<I3D::Filter2D> filter2d = std::make_shared<I3D::Filter2D>(CV_32F, kernel);
  std::shared_ptr<FunctionProcess> fProcess2 = std::make_shared<FunctionProcess> (
    [&](const cv::Mat &in, cv::Mat *out) {
      cv::normalize(in, *out, 0, 255, CV_MINMAX); 
      out->convertTo(*out, CV_8U); 
      out->colRange(kernel_size, out->cols - kernel_size).rowRange(31, out->rows - kernel_size).copyTo(*out); 
      
      //cv::Scalar m, stdv;
      //cv::meanStdDev(*out, m, stdv);
      //double th1 = m[0] - stdv[0];
      //double th2 = m[0] + stdv[0];
      //threshold(*out, *out, th1, th2, cv::THRESH_TRUNC);
      //cv::adaptiveThreshold(*out, *out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 7, 2);
  });

  I3D::ImgProcessingList imgprolist { 
    /*fProcess1, 
    filter2d, 
    fProcess2, */
    normalize,
    erotion,
    dilate,
    bilateralFilter/*, 
    std::make_shared<I3D::EqualizeHistogram>()*/ 
  };

  if (ls == LD_TYPE::LSD) {
    imgprolist.add(std::make_shared<I3D::Sobel>(1, 0, 3, 1., 0.));
  } else {
    imgprolist.add(std::make_shared<I3D::Canny>());
  }


  //std::string img1 = "C:\\Desarrollo\\Datos\\Video_Termico_FLIR\\captura2.jpg";
  //WindowI wTower(cv::Point(290, 25), cv::Point(376, 335)); //"C:\\Desarrollo\\Datos\\Video_Termico_FLIR\\captura2.jpg
  //std::string img1 = "C:\\Desarrollo\\Datos\\insulators6_GRAY.png";
  //std::string img1 = "D:\\Desarrollo\\Datos\\FlirLepton_01.PNG";
  //WindowI wTower(cv::Point(250, 0), cv::Point(415, 480)); //"C:\\Desarrollo\\Datos\\FlirLepton_01.PNG"
  std::string img1 = "D:\\Desarrollo\\Datos\\V1_01.PNG";
  WindowI wTower(cv::Point(400, 25), cv::Point(550, 470)); //"C:\\Desarrollo\\Datos\\V1_01.PNG"
  //std::string img1 = "D:\\Desarrollo\\Datos\\V1_02.PNG";
  //WindowI wTower(cv::Point(277, 25), cv::Point(392, 535)); //"C:\\Desarrollo\\Datos\\V1_02.PNG"
  //std::string img1 = "D:\\Desarrollo\\Datos\\paramotor.PNG";
  //WindowI wTower(cv::Point(186, 30), cv::Point(270, 300)); 
  //std::string img1 = "D:\\Desarrollo\\Datos\\captura3.png";
  //WindowI wTower(cv::Point(190, 0), cv::Point(280, 250)); //"C:\\Desarrollo\\Datos\\V1_01.PNG"

  cv::Mat frame = cv::imread(img1, cv::IMREAD_GRAYSCALE);
  WindowI wFrame(cv::Point(0, 0), cv::Point(frame.cols, frame.rows));
  if (frame.empty()) {
    logPrintError("No se puede cargar frame: %s", img1.c_str());
  } else {

    //regiones a derecha e izquierda de la torre donde detectaremos las lineas
    WindowI wRight(cv::Point(wTower.pt2.x, wTower.pt1.y), cv::Point(wTower.pt2.x + 300, wTower.pt2.y));
    WindowI wLeft(cv::Point(wTower.pt1.x - 300, wTower.pt1.y), cv::Point(wTower.pt1.x, wTower.pt2.y));
    wRight = windowIntersection(wRight, wFrame);
    wLeft = windowIntersection(wLeft, wFrame);

    //Line lineL(cv::Point(wTower.pt1.x, 0), cv::Point(wTower.pt1.x, frame.rows));
    //Line lineR(cv::Point(wTower.pt2.x, 0), cv::Point(wTower.pt2.x, frame.rows));
    //kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, angleRight, lm, gm);
    //filter2d->setParameters(CV_32F, kernel);
    //procesado(frame, wLeft, imgprolistL, lineL, oLD.get());
    //procesado(frame, wRight, imgprolistR, lineR, oLD.get());

    cv::Mat right, left;
    frame.colRange(wRight.pt1.x, wRight.pt2.x).rowRange(wRight.pt1.y, wRight.pt2.y).copyTo(right);
    frame.colRange(wLeft.pt1.x, wLeft.pt2.x).rowRange(wLeft.pt1.y, wLeft.pt2.y).copyTo(left);

    //Determinar angulo con Fourier
    std::vector<int> colFourierRight, colFourierLeft;
    colFourierRight.push_back(right.cols / 2);
    colFourierLeft.push_back(left.cols / 2);
    std::vector<std::vector<cv::Point>> ptsFourierRight, ptsFourierLeft;
    double angleRight = fourierLinesDetection(right, colFourierRight, &ptsFourierRight);
    double angleLeft = fourierLinesDetection(left, colFourierLeft, &ptsFourierLeft);

    bool bConcave = angleLeft - angleRight > 0.;

    cv::Mat outRight, outLeft;

    // Procesos imagen derecha
    kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, angleRight, lm, gm);
    filter2d->setParameters(CV_32F, kernel);
    imgprolist.execute(right, &outRight);

    // Procesos imagen izquierda
    kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, angleLeft, lm, gm);
    filter2d->setParameters(CV_32F, kernel);
    imgprolist.execute(left, &outLeft);

    cv::Mat frameout;
    cvtColor(frame, frameout, CV_GRAY2BGR);

    std::vector<Line> linesRight, linesLeft;
    oLD->run(outRight, cv::Scalar(angleRight, 0.15));
    if (oLD->getLines().empty()) return 0;
    Translate<cv::Point> trf(wRight.pt1.x, wRight.pt1.y);
    trf.transform(oLD->getLines(), &linesRight);
    
    oLD->run(outLeft, cv::Scalar(angleLeft, 0.15));
    if (oLD->getLines().empty()) return 0;
    trf.setTranslation(wLeft.pt1.x, wLeft.pt1.y);
    trf.transform(oLD->getLines(), &linesLeft);

    //Comenzamos a agrupar lineas paralelas
    std::vector<ldGroupLines> linesGroupsRight, linesGroupsLeft;
    //groupParallelLines(linesRight, &linesGroupsRight, 0.015);
    //groupParallelLines(linesLeft, &linesGroupsLeft, 0.015)
    std::thread tL(groupParallelLines,linesRight, &linesGroupsRight, 0.015);
    std::thread tR(groupParallelLines,linesLeft, &linesGroupsLeft, 0.015);
	
    cv::RNG rng(12345);
    cv::Scalar c;

    tR.join();
    int imaxr = 0;
    int maxr = 0;
    for (int ilg = 0; ilg < linesGroupsRight.size(); ilg++) {
      if (maxr < linesGroupsRight[ilg].getSize()){
        maxr = linesGroupsRight[ilg].getSize();
        imaxr = ilg;
      }
    }

    tL.join();
    int imaxl = 0;
    int maxl = 0;
    for (int ilg = 0; ilg < linesGroupsLeft.size(); ilg++) {
      if (maxl < linesGroupsLeft[ilg].getSize()){
        maxl = linesGroupsLeft[ilg].getSize();
        imaxl = ilg;
      }
    }

    std::vector<ldGroupLines> linesGroupsRight2, linesGroupsLeft2;
    //groupLinesByDist(linesGroupsRight[imaxr].getLines(), &linesGroupsRight2, 10);
    //groupLinesByDist(linesGroupsLeft[imaxl].getLines(), &linesGroupsLeft2, 10);
    tL = std::thread(groupLinesByDist,linesGroupsRight[imaxr].getLines(), &linesGroupsRight2, 10);
    tR = std::thread(groupLinesByDist,linesGroupsLeft[imaxr].getLines(), &linesGroupsLeft2, 10);

    tL.join();

    for (size_t ig = 0; ig < linesGroupsLeft2.size(); ig++) {
      std::vector<Line> linesJoinLeft;
      joinLinesByDist(linesGroupsLeft2[ig].getLines(), &linesJoinLeft, 3);
      linesGroupsLeft2[ig] = ldGroupLines(linesJoinLeft);
    }

	
	
	
	
	

    for (size_t ig = 0; ig < linesGroupsLeft2.size(); ig++) {
      c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      for (int il = 0; il < linesGroupsLeft2[ig].getSize(); il++) {
        line(frameout, linesGroupsLeft2[ig][il].pt1, linesGroupsLeft2[ig][il].pt2, c, 1, cv::LINE_8);
      }
    }

    tR.join();

    for (size_t ig = 0; ig < linesGroupsRight2.size(); ig++) {
      std::vector<Line> linesJoinRight;
      joinLinesByDist(linesGroupsRight2[ig].getLines(), &linesJoinRight, 3);
      linesGroupsRight2[ig] = ldGroupLines(linesJoinRight);
    }




    for (size_t ig = 0; ig < linesGroupsRight2.size(); ig++) {
      c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      for (int il = 0; il < linesGroupsRight2[ig].getSize(); il++) {
        line(frameout, linesGroupsRight2[ig][il].pt1, linesGroupsRight2[ig][il].pt2, c, 1, cv::LINE_8);
      }
    }

    // Linea vertical
    line(frameout, cv::Point(wTower.pt1.x, 0), cv::Point(wTower.pt1.x, frameout.rows), Scalar(0, 255, 0), 1, LINE_AA);
    line(frameout, cv::Point(wTower.pt2.x, 0), cv::Point(wTower.pt2.x, frameout.rows), Scalar(0, 255, 0), 1, LINE_AA);

    //interseccion con lineas
    std::vector<cv::Point> ptsL;
    for (size_t ig = 0; ig < linesGroupsLeft2.size(); ig++) {
      for (int il = 0; il < linesGroupsLeft2[ig].getSize(); il++) {
        cv::Point ptAux;
        intersectLines(linesGroupsLeft2[ig][il], Line(cv::Point(wTower.pt1.x, 0), cv::Point(wTower.pt1.x, frameout.rows)), &ptAux);
        ptsL.push_back(ptAux);
      }
    }
    std::vector<cv::Point> ptsR;
    for (size_t ig = 0; ig < linesGroupsRight2.size(); ig++) {
      for (int ir = 0; ir < linesGroupsRight2[ig].getSize(); ir++) {
        cv::Point ptAux;
        intersectLines(linesGroupsRight2[ig][ir], Line(cv::Point(wTower.pt2.x, 0), cv::Point(wTower.pt2.x, frameout.rows)), &ptAux);
        ptsR.push_back(ptAux);
      }
    }


    //... Simplificar 


    std::vector<int> posVerticalL(ptsL.size());
    for (int il = 0; il < ptsL.size(); il++) {
      posVerticalL[il] = ptsL[il].y;
    }

    std::vector<cv::Point> ptsLSort;
    for (auto i : sortIdx(posVerticalL)) {
      ptsLSort.push_back(ptsL[i]);
    }

    std::vector<int> posVerticalR(ptsR.size());
    for (int ir = 0; ir < ptsR.size(); ir++) {
      posVerticalR[ir] = ptsR[ir].y;
    }

    std::vector<cv::Point> ptsRSort;
    for (auto i : sortIdx(posVerticalR)) {
      ptsRSort.push_back(ptsR[i]);
    }

    for (int il = 0; il < ptsLSort.size(); il++) {
      cv::line(frameout, ptsLSort[il], ptsLSort[il], Scalar(255, 0, 0), 3, LINE_AA);
    }

    for (int ir = 0; ir < ptsRSort.size(); ir++) {
      cv::line(frameout, ptsRSort[ir], ptsRSort[ir], Scalar(255, 0, 0), 3, LINE_AA);
    }

    std::vector<MultiPointI> groupIntersec;
    for (size_t igl = 0; igl < linesGroupsLeft2.size(); igl++) {
      for (size_t igr = 0; igr < linesGroupsRight2.size(); igr++) {
        //std::vector<cv::Point> groupPoints;
        MultiPointI groupPoints;
        c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        for (int il = 0; il < linesGroupsLeft2[igl].getSize(); il++) {
          for (int ir = 0; ir < linesGroupsRight2[igr].getSize(); ir++) {
            cv::Point pt_intersect;
            intersectLines(linesGroupsLeft2[igl][il], linesGroupsRight2[igr][ir], &pt_intersect);
            if (wTower.containsPoint(pt_intersect)) {
              groupPoints.add(pt_intersect);
              //line(frameout, pt_intersect, pt_intersect, c, 3, cv::LINE_8);
            }

          }
        }

        if (int nPoints = groupPoints.getSize()) {

          cv::Point pt;
          if (nPoints == 1) pt = groupPoints[0];
          else if (nPoints == 2) {
            // el mas centrado... En posiciones centradas del apoyo creo que funcionaria. hay que ver en otras posiciones
            if (abs(groupPoints[0].x - wTower.getCenter().x) < abs(groupPoints[1].x - wTower.getCenter().x))
              pt = groupPoints[0];
            else
              pt = groupPoints[1];
          } else {
            for (size_t i = 0; i < nPoints; i++) {
              pt += groupPoints[i];
            }
            pt.x /= nPoints;
            pt.y /= nPoints;
          }
          line(frameout, pt, pt, cv::Scalar(0,0,255), 3, cv::LINE_8);
          groupIntersec.push_back(groupPoints);

          ////... Comprobación de que se trata de un aislador
          //WindowI wInsulator(pt, 100);
          //cv::Mat aux;
          //frame.colRange(wInsulator.pt1.x, wInsulator.pt2.x).rowRange(wInsulator.pt1.y, wInsulator.pt2.y).copyTo(aux);

        }

      }
    }


    //std::shared_ptr<BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
    //std::shared_ptr<Erotion> erotion = std::make_shared<I3D::Erotion>(2);
    //std::shared_ptr<Dilate> dilate = std::make_shared<I3D::Dilate>(1);
    //std::shared_ptr<I3D::Canny> canny = std::make_shared<I3D::Canny>();

    //int kernel_size = 9;
    //double sig = 1., th = 0., lm = 1.0, gm = 0.02, ps = 0.;
    //cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, th, lm, gm);

    //std::shared_ptr<I3D::Filter2D> filter2d = std::make_shared<I3D::Filter2D>(CV_32F, kernel);

    kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, 0., lm, gm);
    filter2d->setParameters(CV_32F, kernel);

    I3D::ImgProcessingList imgprolist2{ /*bilateralFilter, erotion, dilate,*/ filter2d };

    // Con los puntos encontrados se comprueba que se trate de un aislador.
    //... Prueba a buscarlo con las imagenes visibles... 
    std::string img_rgb = "D:\\Desarrollo\\Datos\\insulators2.png";
    cv::Mat matRgb = cv::imread(img_rgb, cv::IMREAD_GRAYSCALE);
    cv::Mat matRgbF;
    matRgb.convertTo(matRgbF, CV_32F);

    imgprolist2.execute(matRgbF, &matRgbF);

    cv::normalize(matRgbF, matRgbF, 0, 255, CV_MINMAX);
    matRgbF.convertTo(matRgbF, CV_8U);

    //std::vector<cv::Point> l1{ 
    //  cv::Point(486, 74),
    //  cv::Point(487, 119),
    //  cv::Point(480, 197),
    //  cv::Point(480, 213),
    //  cv::Point(472, 338),
    //  cv::Point(477, 425) 
    //};

    //std::vector<cv::Point> l2{ 
    //  cv::Point(323, 109),
    //  cv::Point(330, 147),
    //  cv::Point(325, 224),
    //  cv::Point(308, 239),
    //  cv::Point(313, 368),
    //  cv::Point(307, 459) 
    //};

    //std::vector<cv::Point> l3(6);
    //std::vector<int> distances(6);

    //std::transform(l1.begin(), l1.end(), l2.begin(), distances.begin(), [](cv::Point &pt1, cv::Point &pt2) -> int { return I3D::distance(pt1, pt2); });
    
    //cv::Mat out;
    //MultiPointI intersect_points { cv::Point(555, 250), cv::Point(556, 353),  cv::Point(562, 424) };
    //cv::Mat aux;
    //for (auto point : intersect_points.getPoints()) {
    //  WindowI wInsulator(point, 100);
    //  matRgb.colRange(wInsulator.pt1.x, wInsulator.pt2.x).rowRange(wInsulator.pt1.y, wInsulator.pt2.y).copyTo(aux);
    //  // procesos...
    //  
    //  imgprolist.execute(matRgb, &out);
    //}

  }

  return 0;
}

