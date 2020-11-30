#include "mathutils.h"

#include "tidop/core/messages.h"

#include "tidop/math/algebra/qr.h"
#include "tidop/math/algebra/lu.h"

//#ifdef HAVE_OPENCV
//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#endif

#ifdef HAVE_EIGEN
TL_SUPPRESS_WARNINGS
#include <Eigen/SVD>
#include <Eigen/LU>
#include <Eigen/QR>
TL_DEFAULT_WARNINGS
#endif

#include <cmath> 

namespace tl
{

/* ---------------------------------------------------------------------------------- */
#ifdef TL_ENABLE_DEPRECATED_METHODS
//https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
void eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa)
{
  //-sinOmega * cosPhi, cosOmega * cosPhi
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

void rotationMatrixAxisX(double rX, std::array<std::array<double, 3>, 3> *RX)
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

void rotationMatrixAxisY(double rY, std::array<std::array<double, 3>, 3> *RY)
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


void rotationMatrixAxisZ(double rZ, std::array<std::array<double, 3>, 3> *RZ)
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


void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R) 
{ 
  double sinOmega = sin(omega);
  double cosOmega = cos(omega);
  double sinPhi = sin(phi);
  double cosPhi = cos(phi);
  double sinKappa = sin(kappa);
  double cosKappa = cos(kappa);

  (*R)[0][0] = cosPhi * cosKappa;
  (*R)[0][1] = -cosPhi * sinKappa;
  (*R)[0][2] = sinPhi;
  (*R)[1][0] = sinOmega * sinPhi * cosKappa + cosOmega * sinKappa;
  (*R)[1][1] = cosOmega * cosKappa - sinOmega * sinPhi * sinKappa;
  (*R)[1][2] = -sinOmega * cosPhi;
  (*R)[2][0] = sinOmega * sinKappa - cosOmega * sinPhi * cosKappa;
  (*R)[2][1] = cosOmega * sinPhi * sinKappa + sinOmega * cosKappa;
  (*R)[2][2] = cosOmega * cosPhi;
}
#endif TL_ENABLE_DEPRECATED_METHODS
/* ---------------------------------------------------------------------------------- */


//#ifdef HAVE_OPENCV
//
//int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx)
//{
//  int iret = 1;
//  if (in.empty() || in.channels() != 1) iret = -1;
//  try {
//    cv::sortIdx(in, *idx, cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING);
//    cv::sort(in, *out, cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING);
//  } catch (std::exception &e) {
//    msgError("%s", e.what());
//    iret = -1;
//  }
//  return iret;
//}
//
//int sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx )
//{
//  int iret = 1;
//  if (in.empty() || in.channels() != 1) iret = -1;
//  try {
//    cv::sortIdx(in, *idx, cv::SortFlags::SORT_EVERY_COLUMN + cv::SortFlags::SORT_ASCENDING);
//    cv::sort(in, *out, cv::SortFlags::SORT_EVERY_COLUMN + cv::SortFlags::SORT_ASCENDING);
//  } catch (std::exception &e) {
//    msgError("%s", e.what());
//    iret = -1;
//  }
//  return iret;
//}
//
//#endif

/* ---------------------------------------------------------------------------------- */
/*                  RESOLUCIÓN DE SISTEMAS DE ECUACIONES LINEALES                     */
/* ---------------------------------------------------------------------------------- */


void solveQR(int nRows, int nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  // Existen 3 métodos:
  // - HouseholderQR (no pivoting, so fast but unstable)
  // - ColPivHouseholderQR (column pivoting, thus a bit slower but more accurate) 
  // - FullPivHouseholderQR (full pivoting, so slowest and most stable)
  Eigen::VectorXd C = A.colPivHouseholderQr().solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
//#elif defined( HAVE_OPENCV)
//  cv::Mat A(nRows, nCols, CV_64F, a);
//  cv::Mat B(nRows, 1, CV_64F, b);
//  cv::Mat C(nCols, 1, CV_64F);
//  cv::solve(A, B, C, cv::DECOMP_QR);
//  std::vector<double> v_aux;
//  cvMatToVector(C, &v_aux);
//  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  math::Matrix<double> A(a, nRows, nCols);
  math::Vector<double> B(b, nRows);
  math::QRDecomposition<math::Matrix<double>> qr(A);
  math::Vector<double> C = qr.solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
#endif
}

void solveLU(int nRows, int nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  // PartialPivLU es mas rapido que FullPivLU pero requiere que la matriz 
  // sea invertible. Se hace la comprobación y se elige el método mas 
  // adecuado.
  Eigen::VectorXd C;
  Eigen::FullPivLU<Eigen::MatrixXd> full_lu(A);
  if (full_lu.isInvertible()) {
    C = A.partialPivLu().solve(B);
  } else {
    C = A.fullPivLu().solve(B);
  }
  std::memcpy(c, C.data(), nCols*sizeof(double));
//#elif defined( HAVE_OPENCV)
//  cv::Mat A(nRows, nCols, CV_64F, a);
//  cv::Mat B(nRows, 1, CV_64F, b);
//  cv::Mat C(nCols, 1, CV_64F);
//  cv::solve(A, B, C, cv::DECOMP_LU);
//  std::vector<double> v_aux;
//  cvMatToVector(C, &v_aux);
//  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  math::Matrix<double> A(a, nRows, nCols);
  math::Vector<double> B(b, nRows);
  math::LuDecomposition<math::Matrix<double>> lu(A);
  math::Vector<double> C = lu.solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
#endif
}

// Standard Cholesky decomposition
void solveCholesky(int nRows, int nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  //Eigen::LLT<Eigen::MatrixXd> cholesky(A);
  Eigen::VectorXd C = A.llt().solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
//#elif defined( HAVE_OPENCV)
//  cv::Mat A(nRows, nCols, CV_64F, a);
//  cv::Mat B(nRows, 1, CV_64F, b);
//  cv::Mat C(nCols, 1, CV_64F);
//  cv::solve(A, B, C, cv::DECOMP_CHOLESKY);
//  std::vector<double> v_aux;
//  cvMatToVector(C, &v_aux);
//  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  //TODO: añadir método alternativo
#endif
}

#ifdef HAVE_EIGEN
// Robust Cholesky decomposition
void solveRobustCholesky(int nRows, int nCols, double *a, double *b, double *c)
{

  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  //Eigen::LDLT<Eigen::MatrixXd> cholesky(A);
  Eigen::VectorXd C = A.ldlt().solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));

}
#endif



} // End namespace TL
