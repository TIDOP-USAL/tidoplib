#include "opencv2/core/core.hpp"

#include "transform.h"
#include "utils.h"

namespace I3D
{

//int svdinverse(double *a, int m, int n, double *b)
//{
//  cv::Mat ma(m, n, CV_64F, a);
//  int iret = cv::invert(ma, ma, cv::DECOMP_SVD) == 0;  // Devuelve 0 si es singula
//  if (iret == 0) {
//    cv::Mat mb(m, 1, CV_64F, b);
//    mb = ma * mb;
//    memcpy(a, ma.data, sizeof(double)*m*n);
//    memcpy(b, mb.data, sizeof(double)*m);
//  }
//  return(iret);
//}

/* ---------------------------------------------------------------------------------- */

void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy)
{
  cv::Point t(dx, dy);
  *lines_out = lines_in;
  for (size_t i = 0; i < lines_out->size(); i++) {
    (*lines_out)[i].pt1 += t;
    (*lines_out)[i].pt2 += t;
  }

  ////... Mismo resultado pero algo mas lento
  //*lines_out = std::vector<Line>(lines_in.size());
  //std::transform(lines_in.begin(), lines_in.end(), lines_out->begin(), [&t](Line ln) -> Line { return Line(ln.pt1 + t, ln.pt2 + t); });
}

void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R) 
{ 
  double sinOmega = sin(omega);
  double cosOmega = cos(omega);
  double sinPhi = sin(phi);
  double cosPhi = cos(phi);
  double sinKappa = sin(kappa);
  double cosKappa = cos(kappa);

  (*R)[0][0] =  cosPhi * cosKappa;
  (*R)[0][1] =  sinOmega * sinPhi * cosKappa + cosOmega * sinKappa;
  (*R)[0][2] = -cosOmega * sinPhi * cosKappa + sinOmega * sinKappa;
  (*R)[1][0] = -cosPhi * sinKappa;
  (*R)[1][1] = -sinOmega * sinPhi * sinKappa + cosOmega * cosKappa;
  (*R)[1][2] =  cosOmega * sinPhi * sinKappa + sinOmega * cosKappa;
  (*R)[2][0] =  sinPhi;
  (*R)[2][1] = -sinOmega * cosPhi;
  (*R)[2][2] = cosOmega * cosPhi;
}


//Rotation<cv::Point2d> rot;
//std::vector<cv::Point2d> pts1{ cv::Point2d(12.3, 34.5), cv::Point2d(10.6, 34.340), cv::Point2d(34.76, 54.26) };
//std::vector<cv::Point2d> pts2{ cv::Point2d(-7.869, 35.578), cv::Point2d(-9.33, 34.71), cv::Point2d(0.499, 64.43) };
//rot.compute(pts1, pts2);
//std::vector<cv::Point2d> pts_out;
//rot.transform(pts1, &pts_out);
//rot.transform(pts2, &pts_out, false);
//
//#ifdef _DEBUG
//double startTick, time;
//startTick = (double)cv::getTickCount(); // measure time
//#endif
//Translate<cv::Point2d> trans(100., 50.);
//for (int i = 0; i < 100; i++) {
//  trans.transform(pts1, &pts_out);
//}
//#ifdef _DEBUG
//time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
//printf("\nTime Rotation [s]: %.6f\n", time);
//#endif
//trans.compute(pts1, pts_out);

} // End namespace I3D