#include "DetectTransmissionTower.h"

#include "core/defs.h"
#include "core/messages.h"
#include "core/exception.h"
#include "segment.h"

using namespace I3D;

void DetectTransmissionTower::detectGroupLines(const cv::Mat &img, std::vector<ldGroupLines> *linesGroup)
{
  cv::Mat imageGrayProcces;
  if (img.channels() != 1)
    cvtColor(img, imageGrayProcces, CV_BGR2GRAY);
  
  // Se aplican procesos previos a las imagenes
  if (pImgprolist->execute(img, &imageGrayProcces) == ProcessExit::FAILURE) return;

  if (pLineDetector->run(imageGrayProcces) == LineDetector::Exit::FAILURE) return;
  groupLinesByDist(pLineDetector->getLines(), linesGroup, 10);

  // Se eliminan los grupos con pocas lineas
  delLinesGroupBySize(linesGroup,5);
}

void DetectTransmissionTower::computeOpticalFlow(const cv::Mat &img1, const cv::Mat &img2, cv::Mat_<cv::Point2f> *flow)
{
  *flow = cv::Mat(img1.size[0], img1.size[1], CV_32FC2);
  algorithm->calc(img1, img2, *flow);
}

bool DetectTransmissionTower::run(const cv::Mat &img1, const cv::Mat &img2, cv::Mat *imgout, WindowI *wOut ) 
{ 
  cv::Size sz = img1.size(); 
  img1.copyTo(*imgout);
  
  ////Ventana en la cual se van a buscar los apoyos
  //WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
  //ws = expandWindow(ws, -100, 0);

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

          if (bool bTower = isTower(imgout, linesGroup1[iplg], magnitude)) {
            *wOut = linesGroup1[iplg].getBbox();
            return bTower;
          }
        }
      }
    }
  }
  return false;
}

void DetectTransmissionTower::getMagnitude(const cv::Mat_<cv::Point2f> &flow, cv::Mat *magnitude) 
{
  cv::Mat flow_split[2];
  split(flow, flow_split);
  cv::Mat angle; //... Anque no necesito el angulo tengo que pasarselo
  // Magnitud y angulo del desplazamiento. Sólo interesa la magnitud
  cartToPolar(flow_split[0], flow_split[1], *magnitude, angle, true);

}

bool DetectTransmissionTower::isTower(cv::Mat *imgout, const ldGroupLines &linesGroup1, const cv::Mat &magnitude) {

  WindowI wprev = linesGroup1.getBbox();

  // Normalizamos la magnitud para ver si hay valores mas altos en la zona de la posible torre
  cv::Scalar mean, stdv, meanc, stdvc;
  cv::Mat normMag, candidatenorm;
  cv::normalize(magnitude, normMag, 0, 1, cv::NORM_MINMAX);
  cv::meanStdDev(normMag, mean, stdv);
  normMag.colRange(wprev.pt1.x, wprev.pt2.x).rowRange(wprev.pt1.y, wprev.pt2.y).copyTo(candidatenorm);
  cv::meanStdDev(candidatenorm, meanc, stdvc);

  if (mean[0] < meanc[0] && meanc[0] > 0.5 && stdvc[0] > 0.1 /*&& (mean[0] + stdv[0]) < meanc[0]*/) {

    c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

    // Buscamos los máximos en la zona
    std::vector<cv::Point> pMax;
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
            line(*imgout, paux, paux, c, 1, cv::LINE_8);
        } else break;
      }
    }

    printVerbose("Frame %i - cols: %i - rows: %i", static_cast<int>(prevFrame), candidatenorm.cols, candidatenorm.rows);
    printVerbose("Frame %i - N points max: %i", static_cast<int>(prevFrame), pMax.size());
    if (pMax.size() > 200 ) {
      // Recta de regresión para los máximos
      double m = 0.;
      double b = 0.;
      regressionLinearXY(pMax, &m, &b);
      
      cv::Point pt1(I3D_ROUND_TO_INT(b), 0);
      cv::Point pt2(I3D_ROUND_TO_INT(m * magnitude.rows + b), magnitude.rows);
            
      if (bDrawRegressionLine) {
        //Se pinta la recta de regresión
        line(*imgout, pt1, pt2, cv::Scalar(0, 0, 150), 1, cv::LINE_8);
      }
            

      //Buscamos sólo cuando la torre este mas vertical
      Line rr(pt1, pt2);
      double ang = rr.angleOY();
      if (ang > I3D_PI / 2)  ang = ang - I3D_PI;
      else if (ang < -I3D_PI / 2) ang = ang + I3D_PI;
      // tolerancia de inclinación del eje del apoyo respecto a la vertical -> 0.1
      if (ang <= 0.2 && ang >= -0.2) {
        printVerbose("Frame %i - Angulo: %f", static_cast<int>(prevFrame), ang);

         // Busqueda del máximo valor de desplazamiento
        std::vector<cv::Point> vMagnitudes;
        cv::Point ptMax;
        float maxval = I3D_FLOAT_MIN;
        for (int ir = 0; ir < normMag.rows; ir++) {
          cv::Point pt(I3D_ROUND_TO_INT(m * ir + b), ir);
          if (pt.x >= 0 && pt.x < normMag.cols) {
            float mg = magnitude.at<float>(pt);
            if (mg > maxval){
              maxval = mg;
              ptMax = pt;
            }
            if (mg > mean[0])
              vMagnitudes.push_back(cv::Point(I3D_ROUND_TO_INT(mg), ir));
          }
        }
       
       
        // Aplicamos douglas-Peucker
        std::vector<cv::Point> lmag;
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
          //cv::rectangle(*imgout, wprev.pt1, wprev.pt2, cv::Scalar(0, 255, 0), 1);
       
          //if (bDrawLines){
          //  for (int il = 0; il < linesGroup1.getSize(); il++) {
          //    cv::Point pt1 = linesGroup1[il].pt1;
          //    cv::Point pt2 = linesGroup1[il].pt2;
          //    line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
          //  }
          //}
          
       
          // Ajustar el BBOX mejor  
          printInfo("Torre detectada: Frame %i", static_cast<int>(prevFrame));
          //if (bSaveImages) {
          //  char buffer[50];
          //  sprintf_s(buffer, "Apoyo_%05i.jpg", cvRound(prevFrame));
          //  cv::imwrite(buffer, *imgout);
          //}
          return true; // Devolvemos que hemos encontrado una torre
        }
      } else printVerbose("Frame %i rechazado por angulo de recta de regresion mayor al limite. angulo=%f", I3D_ROUND_TO_INT(prevFrame), ang);
    }
  } else {
    printInfo("Torre rechazada: Frame %i", static_cast<int>(prevFrame) );
    return false;
  }
  return false;
}