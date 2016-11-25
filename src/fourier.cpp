#include "fourier.h"

#include "core/utils.h"
#include "core/mathutils.h"

namespace I3D
{

void DiscreteFourierTrf::forward(const cv::Mat &source, cv::Mat *out) const
{
  cv::dft(source, *out);
}

void DiscreteFourierTrf::inverse(const cv::Mat &source, cv::Mat *out) const
{
  cv::dft(source, *out, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
}

void DiscreteFourierTrf::shift(cv::Mat &_image) const
{
  _image = _image(cv::Rect(0, 0, _image.cols & -2, _image.rows & -2));

  int cx = _image.cols / 2;
  int cy = _image.rows / 2;

  // Se crea un ROI por cuadrante
  cv::Mat q0(_image, cv::Rect(0, 0, cx, cy));   // Top-Left
  cv::Mat q1(_image, cv::Rect(cx, 0, cx, cy));  // Top-Right
  cv::Mat q2(_image, cv::Rect(0, cy, cx, cy));  // Bottom-Left
  cv::Mat q3(_image, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

  // Se intercambian los cuadrantes
  cv::Mat tmp;
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);
  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);
}

// log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
void logScale(const cv::Mat &in, cv::Mat *out)
{
  in.copyTo(*out);
  *out += cv::Scalar::all(1);
  cv::log(*out, *out);
}

double fourierLinesDetection(cv::Mat &source, std::vector<int> &cols, std::vector<std::vector<cv::Point>> *pts, double *angle )
{
  if (source.empty()) return 0;

  //cv::Size size = source.size();

  // Se expande la imagen al tamaño optimo
  cv::Mat OptSizeImg;
  int r = cv::getOptimalDFTSize(source.rows);
  int c = cv::getOptimalDFTSize(source.cols);

  cv::Size size(c, r);

  // Se añaden ceros en los bordes
  copyMakeBorder(source, OptSizeImg, 0, r - source.rows, 0, c - source.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

  // A la imagen expandida se añade otra matriz con ceros
  cv::Mat planes[] = { cv::Mat_<float>(OptSizeImg), cv::Mat::zeros(OptSizeImg.size(), CV_32F) };
  cv::Mat complexI;
  cv::merge(planes, 2, complexI);

  // Generación de la transformada de Fourier.
  I3D::DiscreteFourierTrf ft;
  ft.forward(complexI, &complexI);
  ft.shift(complexI);

  // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
  
  cv::split(complexI, planes);
  size = planes->size();
  cv::Mat magI;
  cv::magnitude(planes[0], planes[1], magI);

  logScale(magI, &magI);

  cv::normalize(magI, magI, 0, 1, CV_MINMAX);
  double r_angle = 0.0;
  // Buscamos los máximos
  // Linea por angulo
  cv::Point pt1, pt2;
  if (angle) {
    int x0 = size.width / 2, y0 = size.height / 2;
    int dx = I3D_ROUND_TO_INT(tan(*angle) * 70.);
    pt1.x = x0 + dx;
    pt1.y = y0 - 70;
    pt2.x = x0 - dx;
    pt2.y = y0 + 70;
  } else {
    // Recortamos la imagen directamente en lugar de usar una mascara.
    cv::Mat magIcrop;
    //... Lo de cortar por un tamaño fijo es peligroso. Comprobar el tamaño del mat pasado para evitar excepciones
    cv::Size sz_crop;
    sz_crop.width = size.width > 140 ? 140 : size.width;
    sz_crop.height = size.height > 140 ? 140 : size.height;
    cv::Rect rcrop(size.width / 2 - sz_crop.width / 2, size.height / 2 - sz_crop.height / 2, sz_crop.width, sz_crop.height);
    magI.colRange(rcrop.x, rcrop.x + rcrop.width).rowRange(rcrop.y, rcrop.height + rcrop.y).copyTo(magIcrop);


    // Eliminamos los valores con variaciones pequeñas
    cv::Scalar med, stdv;
    cv::meanStdDev(magIcrop, med, stdv);
    cv::Mat mask2 = cv::Mat::zeros(rcrop.size(), CV_8U);
    mask2.setTo(1, magIcrop > med[0] + stdv[0]);

    std::vector<cv::Point> pMax;
    cv::Mat idx;
    cv::sortIdx(magIcrop, idx, CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);
    for (int ir = 0; ir < magIcrop.rows; ir++) {
      int id;
      for (int ic = 0; ic < 2; ic++) {
        id = idx.at<int>(ir, ic);
        float cmax = magIcrop.at<float>(ir, id);
        if (cmax != 0. && cmax > med[0] + stdv[0]) {
          cv::Point paux(/*rcrop.x +*/ id, /*rcrop.y +*/ ir);
          pMax.push_back(paux);
        }
        else break;
      }
    }
    // Recta de regresión para los máximos
    double m = 0.;
    double b = 0.;
    /*double corr = */regressionLinearXY(pMax, &m, &b);
    
    pt1 = cv::Point(I3D_ROUND_TO_INT(b + rcrop.x), rcrop.y);
    pt2 = cv::Point(I3D_ROUND_TO_INT(m * rcrop.height + b + rcrop.x), rcrop.height + rcrop.y);
    r_angle = atan(m) + CV_PI / 2.;
  }

  cv::Mat maskLine = cv::Mat::zeros(size, CV_8U);
  cv::line(maskLine, pt1, pt2, cv::Scalar(1, 1, 1), 3);
  cv::Mat mat5;
  cv::Mat mat6;
  cv::bitwise_and(planes[0], planes[0], mat5, maskLine);
  cv::bitwise_and(planes[1], planes[1], mat6, maskLine);

  cv::Mat planesi[] = { mat5, mat6 };
  cv::merge(planesi, 2, complexI);

  //Inversa de la transformada de fourier para pasar al dominio del espació
  cv::Mat imageOut;
  ft.shift(complexI);
  ft.inverse(complexI, &imageOut);

  normalize(imageOut, imageOut, 0, 1, CV_MINMAX);

  float prevVar, curVar;
  int prevBinVar, curBinVar;

  pts->clear();
  pts->resize(cols.size());
  for (size_t j = 0; j < cols.size(); j++) {

    for (int i = 0; i < size.height; i++) {
      curVar = imageOut.at<float>(cv::Point(cols[j], i)) - imageOut.at<float>(cv::Point(cols[j], 0));
      curBinVar = 0;
      if (i + 2 < size.height) {
        if (imageOut.at<int>(cv::Point(cols[j], i + 1)) - imageOut.at<int>(cv::Point(cols[j], i + 2)) > 0) {
          curBinVar = 1;
        }
      }

      if (i > 0 && i < size.height){
        int aux = curBinVar - prevBinVar;
        if (aux != 0) {
          if (prevVar - curVar > 0.04) {
            int y = i + 1;
            //cv::line(source, cv::Point(size.width/2, y - 5), cv::Point(size.width/2, y + 5), cv::Scalar(255, 255, 255));
            //cv::line(source, cv::Point(size.width/2 - 5, y), cv::Point(size.width/2 + 5, y), cv::Scalar(255, 255, 255));
            (*pts)[j].push_back(cv::Point(cols[j], y));
          }
        }
      }

      prevVar = curVar;
      prevBinVar = curBinVar;
    }
  }
  return r_angle;
}

//int fourierLinesDetection(cv::Mat &source, std::vector<int> &cols, std::vector<std::vector<cv::Point>> *pts, double *angle)
//{
//  double startTick, time;
//  startTick = (double)cv::getTickCount(); // measure time
//  if (source.empty()) return 0;
//
//  //cv::Size size = source.size();
//
//  // Se expande la imagen al tamaño optimo
//  cv::Mat OptSizeImg;
//  int r = cv::getOptimalDFTSize(source.rows);
//  int c = cv::getOptimalDFTSize(source.cols);
//
//  cv::Size size(c, r);
//
//  // Se añaden ceros en los bordes
//  copyMakeBorder(source, OptSizeImg, 0, r - source.rows, 0, c - source.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
//
//  // A la imagen expandida se añade otra matriz con ceros
//  cv::Mat planes[] = { cv::Mat_<float>(OptSizeImg), cv::Mat::zeros(OptSizeImg.size(), CV_32F) };
//  cv::Mat complexI;
//  cv::merge(planes, 2, complexI);
//
//  // Generación de la transformada de Fourier.
//  I3D::DiscreteFourierTrf ft;
//  ft.forward(complexI, &complexI);
//  ft.shift(complexI);
//
//  // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
//
//  cv::split(complexI, planes);
//  cv::Mat magI;
//  cv::magnitude(planes[0], planes[1], magI);
//
//  // log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
//  logScale(magI, &magI);
//
//  cv::normalize(magI, magI, 0, 1, CV_MINMAX);
//
//  // Recortamos la imagen directamente en lugar de usar una mascara.
//  cv::Mat magIcrop;
//  //... Lo de cortar por un tamaño fijo es peligroso. Comprobar el tamaño del mat pasado para evitar excepciones
//  cv::Size sz_crop;
//  sz_crop.width = size.width > 140 ? 140 : size.width;
//  sz_crop.height = size.height > 140 ? 140 : size.height;
//  cv::Rect rcrop(size.width / 2 - sz_crop.width / 2, size.height / 2 - sz_crop.height / 2, sz_crop.width, sz_crop.height);
//  magI.colRange(rcrop.x, rcrop.x + rcrop.width).rowRange(rcrop.y, rcrop.height + rcrop.y).copyTo(magIcrop);
//
//  // Eliminamos los valores con variaciones pequeñas
//  cv::Scalar m, stdv;
//  cv::meanStdDev(magIcrop, m, stdv);
//  cv::Mat mask2 = cv::Mat::zeros(rcrop.size(), CV_8U);
//  //mask2.setTo(1, magIcrop > 0.6);
//  mask2.setTo(1, magIcrop > m[0] + stdv[0]);
//
//  // Buscamos los máximos
//  // Linea por angulo
//  cv::Point pt1, pt2;
//  if (angle) {
//    double a = cos(*angle), b = sin(*angle);
//    double x0 = size.width / 2, y0 = size.height / 2;
//    pt1.x = cvRound(x0 + 100 * (-b));
//    pt1.y = cvRound(y0 + 100 * (a));
//    pt2.x = cvRound(x0 - 100 * (-b));
//    pt2.y = cvRound(y0 - 100 * (a));
//  } else {
//    cv::Mat nonZeroCoordinates;
//    cv::findNonZero(mask2, nonZeroCoordinates);    // Buscamos en la mascara los elementos distintos de 0
//    cv::Mat rc[] = { cv::Mat::zeros(cv::Size(nonZeroCoordinates.rows, 1), CV_32S), cv::Mat::zeros(cv::Size(nonZeroCoordinates.rows, 1), CV_32S) };
//    cv::split(nonZeroCoordinates, rc);
//    double rmax, rmin;                      // Máximo y mínimo en filas
//    cv::Point pmin, pmax;                   // Posición de máximo y mínimo en filas
//    cv::minMaxLoc(rc[1], &rmin, &rmax, &pmin, &pmax);
//    pt1.x = rcrop.x + rc[0].at<int>(pmin);
//    pt1.y = rcrop.y + static_cast<int>(rmin);
//    pt2.x = rcrop.x + rc[0].at<int>(pmax);
//    pt2.y = rcrop.y + static_cast<int>(rmax);
//  }
//
//  cv::Mat maskLine = cv::Mat::zeros(size, CV_8U);
//  cv::line(maskLine, pt1, pt2, cv::Scalar(1, 1, 1), 3);
//  cv::Mat mat5;
//  cv::Mat mat6;
//  cv::bitwise_and(planes[0], planes[0], mat5, maskLine);
//  cv::bitwise_and(planes[1], planes[1], mat6, maskLine);
//
//  cv::Mat planesi[] = { mat5, mat6 };
//  cv::merge(planesi, 2, complexI);
//
//  //Inversa de la transformada de fourier para pasar al dominio del espació
//  cv::Mat imageOut;
//  ft.shift(complexI);
//  ft.inverse(complexI, &imageOut);
//
//  normalize(imageOut, imageOut, 0, 1, CV_MINMAX);
//
//  //cv::Mat cc2 = cv::Mat::zeros(cv::Size(1, size.height), CV_32F);
//  //imageOut.col(size.width / 2).rowRange(2, size.height).copyTo(cc2);
//
//  float prevVar, curVar;
//  int prevBinVar, curBinVar;
//
//  pts->clear();
//  pts->resize(cols.size());
//  for (int j = 0; j < cols.size(); j++) {
//
//    for (int i = 0; i < size.height; i++) {
//      curVar = imageOut.at<float>(cv::Point(cols[j], i)) - imageOut.at<float>(cv::Point(cols[j], 0));
//      curBinVar = 0;
//      if (i + 2 < size.height) {
//        if (imageOut.at<int>(cv::Point(cols[j], i + 1)) - imageOut.at<int>(cv::Point(cols[j], i + 2)) > 0) {
//          curBinVar = 1;
//        }
//      }
//
//      if (i > 0 && i < size.height){
//        int aux = curBinVar - prevBinVar;
//        if (aux != 0) {
//          if (prevVar - curVar > 0.04) {
//            int y = i + 1;
//            //cv::line(source, cv::Point(size.width/2, y - 5), cv::Point(size.width/2, y + 5), cv::Scalar(255, 255, 255));
//            //cv::line(source, cv::Point(size.width/2 - 5, y), cv::Point(size.width/2 + 5, y), cv::Scalar(255, 255, 255));
//            (*pts)[j].push_back(cv::Point(cols[j], y));
//          }
//        }
//      }
//
//      prevVar = curVar;
//      prevBinVar = curBinVar;
//    }
//  }
//
//  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
//  printf("\nTime [s]: %.3f\n", time);
//  return static_cast<int>(pts->size());
//}


} // End namespace I3D
