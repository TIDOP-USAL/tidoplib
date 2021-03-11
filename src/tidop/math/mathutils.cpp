/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "mathutils.h"

#include "tidop/core/messages.h"

#include "tidop/math/algebra/qr.h"
#include "tidop/math/algebra/lu.h"


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
/*                  RESOLUCIÓN DE SISTEMAS DE ECUACIONES LINEALES                     */
/* ---------------------------------------------------------------------------------- */

void solveSVD(size_t nRows, size_t nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, static_cast<long>(nCols), static_cast<long>(nRows));
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, static_cast<long>(nRows));
  //Eigen::VectorXd C = A.transpose().jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);
  //Eigen::VectorXd C = A.transpose().jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(B);
  Eigen::VectorXd C = A.transpose().bdcSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
//#elif defined( HAVE_OPENCV)
//  cv::Mat A(static_cast<int>(nRows), static_cast<int>(nCols), CV_64F, a);
//  cv::Mat B(static_cast<int>(nRows), 1, CV_64F, b);
//  cv::Mat C(static_cast<int>(nCols), 1, CV_64F);
//  cv::solve(A, B, C, cv::DECOMP_SVD);
//  std::vector<double> v_aux;
//  cvMatToVector(C, &v_aux);
//  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  math::Matrix<double> A(a, nRows, nCols);
  math::Vector<double> B(b, nRows);
  math::SingularValueDecomposition<math::Matrix<double>> svd(A);
  math::Vector<double> C = svd.solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
#endif
}

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
