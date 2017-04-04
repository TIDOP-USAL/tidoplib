#include "mathutils.h"

#include "core/messages.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif

#include <cmath> 


namespace I3D
{

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

double laplacianVariance(const cv::Mat& src)
{
  cv::Mat laplacian;
  cv::Laplacian(src, laplacian, CV_64F);
  cv::Scalar mean, stddev;
  cv::meanStdDev(laplacian, mean, stddev);
  return (stddev.val[0] * stddev.val[0]);
}

#endif

double computeMedian(const std::vector<double> &input)
{
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

//https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
void eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa)
{
  if (omega) *omega = atan2(R[1][2], R[2][2]);
  if (phi) *phi = atan2(-R[0][2], sqrt(R[0][0] * R[0][0] + R[0][1] * R[0][1]));
  double s1 = sin(*omega);
  double c1 = cos(*omega);
  if (kappa) *kappa = atan2(s1 * R[2][0] - c1 * R[1][0], c1 * R[1][1] - s1 * R[2][1]);
}

//void eulerAngles2(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa) 
//{ 
//  float sy = sqrt(R[0][0] * R[0][0] + R[0][1] * R[0][1]);
//  if (phi) *phi = atan2(-R[0][2], sy);
//
//  if (sy < 1e-6) {
//    if (omega) *omega = atan2(-R[2][1], R[1][1]);
//    if (kappa) *kappa = 0;
//  } else {
//    if (omega) *omega = atan2(R[1][2], R[2][2]);
//    if (kappa) *kappa = atan2(R[0][1], R[0][0]);
//  }
//}

//https://www.learnopencv.com/rotation-matrix-to-euler-angles/ 

//void eulerAnglesToRotationMatrix(double rX, double rY, double rZ, std::array<std::array<double, 3>, 3> *R)
//{
  //  // Calculate rotation about x axis
  //  Mat R_x = (Mat_<double>(3, 3) <<
  //             1, 0, 0,
  //             0, cos(theta[0]), -sin(theta[0]),
  //             0, sin(theta[0]), cos(theta[0])
  //             );
  //
  //  // Calculate rotation about y axis
  //  Mat R_y = (Mat_<double>(3, 3) <<
  //             cos(theta[1]), 0, sin(theta[1]),
  //             0, 1, 0,
  //             -sin(theta[1]), 0, cos(theta[1])
  //             );
  //
  //  // Calculate rotation about z axis
  //  Mat R_z = (Mat_<double>(3, 3) <<
  //             cos(theta[2]), -sin(theta[2]), 0,
  //             sin(theta[2]), cos(theta[2]), 0,
  //             0, 0, 1);
  //
  //
  //  // Combined rotation matrix
  //  Mat R = R_z * R_y * R_x;
//}

void RotationMatrixAxisX(double rX, std::array<std::array<double, 3>, 3> *RX)
{
  double sinOmega = sin(rX);
  double cosOmega = cos(rX);

  (*RX)[0][0] = 1;
  (*RX)[0][1] = 0;
  (*RX)[0][2] = 0;
  (*RX)[1][0] = 0;
  (*RX)[1][1] = cosOmega;
  (*RX)[1][2] = -sinOmega;
  (*RX)[2][0] = 0;
  (*RX)[2][1] = -sinOmega;
  (*RX)[2][2] = cosOmega;
}

void RotationMatrixAxisY(double rY, std::array<std::array<double, 3>, 3> *RY)
{
  double sinPhi = sin(rY);
  double cosPhi = cos(rY);

  (*RY)[0][0] = cosPhi;
  (*RY)[0][1] = 0;
  (*RY)[0][2] = sinPhi;
  (*RY)[1][0] = 0;
  (*RY)[1][1] = 1;
  (*RY)[1][2] = 0;
  (*RY)[2][0] = -sinPhi;
  (*RY)[2][1] = 0;
  (*RY)[2][2] = cosPhi;
}


void RotationMatrixAxisZ(double rZ, std::array<std::array<double, 3>, 3> *RZ)
{
  double sinKappa = sin(rZ);
  double cosKappa = cos(rZ);

  (*RZ)[0][0] = cosKappa;
  (*RZ)[0][1] = -sinKappa;
  (*RZ)[0][2] = 0;
  (*RZ)[1][0] = sinKappa;
  (*RZ)[1][1] = cosKappa;
  (*RZ)[1][2] = 0;
  (*RZ)[2][0] = 0;
  (*RZ)[2][1] = 0;
  (*RZ)[2][2] = 1;
}

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx)
{
  int iret = 1;
  if (in.empty() || in.channels() != 1) iret = -1;
  try {
    cv::sortIdx(in, *idx, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
    cv::sort(in, *out, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
  } catch (std::exception &e) {
    msgError("%s", e.what());
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
    msgError("%s", e.what());
    iret = -1;
  }
  return iret;
}

#endif

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
