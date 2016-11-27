#include "mathutils.h"

#include "core/messages.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cmath> 

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

double laplacianVariance(const cv::Mat& src)
{
  cv::Mat laplacian;
  cv::Laplacian(src, laplacian, CV_64F);
  cv::Scalar mean, stddev;
  cv::meanStdDev(laplacian, mean, stddev);
  return (stddev.val[0] * stddev.val[0]);
}

double computeMedian(const std::vector<double> &input) {
  size_t size = input.size();
  if (size % 2 == 0)
    return (input[size / 2 - 1] + input[size / 2]) / 2;
  else
    return input[size / 2];
}

double computeTempMAD(const std::vector<double> &input, const double median)
{
  std::vector<double> inp = input;
  for (size_t i = 0; i < inp.size(); ++i) {
    inp[i] = abs(inp[i] - median);
  }
  sort(inp.begin(), inp.end());
  return computeMedian(inp)*1.4826;
}

bool isOutlier(const double temp, const double median, const double mad)
{
  if ((abs(temp - median) / mad)>2) {
    return true;
  } else {
    return false;
  }
}

//double regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b)
//{
//  double corr = 0.0;
//  double sx = 0., sy = 0., sx2 = 0., sy2 = 0., sxy = 0.;
//  size_t n = pts.size();
//  if (n >= 2) {
//    for (size_t i = 0; i < n; i++) {
//      sx += pts[i].x;
//      sy += pts[i].y;
//      sx2 += pts[i].x * pts[i].x;
//      sy2 += pts[i].y * pts[i].y;
//      sxy += pts[i].x * pts[i].y;
//    }
//    double den = (n*sx2 - sx*sx);
//    if (den) {
//      // Línea no vertical
//      *m = (n*sxy - sy*sx) / (n*sx2 - sx*sx);
//      *b = (sy - *m*sx) / n;
//      den = sy2 - sy*sy / n;
//      corr = sqrt(*m * (sxy - sx*sy / n) / (sy2 - sy*sy / n));
//    }
//  }
//  return(corr);
//}

//double regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b)
//{
//  double corr = 0.0;
//  double sx = 0., sy = 0., sx2 = 0., sy2 = 0., sxy = 0.;
//  size_t n = pts.size();
//  if (n >= 2) {
//    for (size_t i = 0; i < n; i++) {
//      sx += pts[i].x;
//      sy += pts[i].y;
//      sx2 += pts[i].x * pts[i].x;
//      sy2 += pts[i].y * pts[i].y;
//      sxy += pts[i].x * pts[i].y;
//    }
//    double den = (n*sy2 - sy*sy);
//    if (den) {
//      *m = (n*sxy - sy*sx) / (n*sy2 - sy*sy);
//      *b = (sx - *m*sy) / n;
//      den = sx2 - sx*sx / n;
//      corr = sqrt(*m * (sxy - sx*sy / n) / (sx2 - sx*sx / n));
//    }
//  }
//  return(corr);
//}

//https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
void eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa) 
{ 
  if (omega) *omega = atan2(R[1][2], R[2][2]);
  if (phi) *phi = atan2(-R[0][2], sqrt(R[0][0] * R[0][0] + R[0][1] * R[0][1]));
  double s1 = sin(*omega);
  double c1 = cos(*omega);
  if (kappa) *kappa = atan2(s1 * R[2][0] - c1 * R[1][0], c1 * R[1][1] - s1 * R[2][1]);
}

/* ---------------------------------------------------------------------------------- */

int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx)
{
  int iret = 1;
  if (in.empty() || in.channels() != 1) iret = -1;
  try {
    cv::sortIdx(in, *idx, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
    cv::sort(in, *out, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
  } catch (std::exception &e) {
    printError("%s", e.what());
    iret = -1;
  }
  return iret;
}

int sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx )
{
  int iret = 1;
  if (in.empty() || in.channels() != 1) iret = -1;
  try {
    cv::sortIdx(in, *idx, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
    cv::sort(in, *out, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
  } catch (std::exception &e) {
    printError("%s", e.what());
    iret = -1;
  }
  return iret;
}



/* ---------------------------------------------------------------------------------- */



double degreesToDecimalDegrees(int degrees, int minutes, int seconds)
{
  return isNegative(degrees) * (abs(degrees) + minutes / 60. + seconds / 3600.);
}

double degreesToRadians(int degrees, int minutes, int seconds)
{
  return decimalDegreesToRadians(degreesToDecimalDegrees(degrees, minutes, seconds));
}

double degreesToGradians(int degrees, int minutes, int seconds)
{
  return decimalDegreesToGradians(degreesToDecimalDegrees(degrees, minutes, seconds));
}

void decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds)
{
  *degrees = static_cast<int>(decimalDegrees);
  double min = fabs(decimalDegrees - *degrees) * 60.;
  *minutes = static_cast<int>(min);
  *seconds = I3D_ROUND_TO_INT(fabs(min - *minutes) * 60.);
}

double decimalDegreesToRadians(double decimalDegrees)
{
  return decimalDegrees * I3D_DEG_TO_RAD;
}

double decimalDegreesToGradians(double decimalDegrees)
{
  return decimalDegrees * 200. / 180.;
}

void radiansToDegrees(double rad, int *degrees, int *minutes, int *seconds)
{
  decimalDegreesToDegrees(radiansToDecimalDegrees(rad), degrees, minutes, seconds);
}

double radiansToDecimalDegrees(double radians)
{ 
  return radians * I3D_RAD_TO_DEG;
}

double radiansToGradians(double radians)
{
  return radians * I3D_RAD_TO_GRAD;
}

void gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds)
{
  decimalDegreesToDegrees(gradiansToDecimalDegrees(gradians), degrees, minutes, seconds);
}

double gradiansToDecimalDegrees(double gradians)
{
  return gradians * 180. / 200.;
}

double gradiansToRadians(double gradians)
{
  return gradians * I3D_GRAD_TO_RAD;
}

} // End namespace I3D
