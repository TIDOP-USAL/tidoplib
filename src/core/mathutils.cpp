#include "core/mathutils.h"

#include "core/messages.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif

#ifdef HAVE_EIGEN
TL_DISABLE_WARNING(4714)
#include <Eigen/SVD>
#include <Eigen/LU>
#include <Eigen/QR>
TL_ENABLE_WARNING(4714)
#endif

#include <cmath> 

namespace TL
{

/* ---------------------------------------------------------------------------------- */

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


void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R) 
{ 
  double sinOmega = sin(omega);
  double cosOmega = cos(omega);
  double sinPhi = sin(phi);
  double cosPhi = cos(phi);
  double sinKappa = sin(kappa);
  double cosKappa = cos(kappa);

  (*R)[0][0] = cosPhi * cosKappa;
  (*R)[0][1] = sinOmega * sinPhi * cosKappa + cosOmega * sinKappa;
  (*R)[0][2] = sinOmega * sinKappa - cosOmega * sinPhi * cosKappa;
  (*R)[1][0] = -cosPhi * sinKappa;
  (*R)[1][1] = cosOmega * cosKappa - sinOmega * sinPhi * sinKappa;
  (*R)[1][2] = cosOmega * sinPhi * sinKappa + sinOmega * cosKappa;
  (*R)[2][0] = sinPhi;
  (*R)[2][1] = -sinOmega * cosPhi;
  (*R)[2][2] = cosOmega * cosPhi;
}

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

double computeTempMAD(const std::vector<double> &input, double median)
{
  std::vector<double> inp = input;
  for (size_t i = 0; i < inp.size(); ++i) {
    inp[i] = abs(inp[i] - median);
  }
  sort(inp.begin(), inp.end());
  return computeMedian(inp)*1.4826;
}

bool isOutlier(double temp, double median, double mad)
{
  if ((abs(temp - median) / mad)>2) {
    return true;
  } else {
    return false;
  }
}

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
/*                         CONVERSIÓN DE UNIDADES ANGULARES                           */
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
  *seconds = TL_ROUND_TO_INT(fabs(min - *minutes) * 60.);
}

double decimalDegreesToRadians(double decimalDegrees)
{
  return decimalDegrees * TL_DEG_TO_RAD;
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
  return radians * TL_RAD_TO_DEG;
}

double radiansToGradians(double radians)
{
  return radians * TL_RAD_TO_GRAD;
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
  return gradians * TL_GRAD_TO_RAD;
}



/* ---------------------------------------------------------------------------------- */
/*                  RESOLUCIÓN DE SISTEMAS DE ECUACIONES LINEALES                     */
/* ---------------------------------------------------------------------------------- */
//#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
//void svdcmp(float **a, int m, int n, float w[], float **v)
//{
//  float pythag(float a, float b);
//  int flag, i, its, j, jj, k, l, nm;
//  float anorm, c, f, g, h, s, scale, x, y, z/*, *rv1*/;
////  // reserva de memoria.
////  rv1 = vector(1, n);
//  std::vector<float> rv1(n);
//  g = scale = anorm = 0.0; //Householder reduction to bidiagonal form.
//  for (i = 1; i <= n; i++) {
//    l = i + 1;
//    rv1[i] = scale*g;
//    g = s = scale = 0.0;
//    if (i <= m) {
//      for (k = i; k <= m; k++) scale += fabs(a[k][i]);
//      if (scale) {
//        for (k = i; k <= m; k++) {
//          a[k][i] /= scale;
//          s += a[k][i] * a[k][i];
//        }
//        f = a[i][i];
//        g = -SIGN(sqrt(s), f);
//        h = f*g - s;
//        a[i][i] = f - g;
//        for (j = l; j <= n; j++) {
//          for (s = 0.0, k = i; k <= m; k++) s += a[k][i] * a[k][j];
//          f = s / h;
//          for (k = i; k <= m; k++) a[k][j] += f*a[k][i];
//        }
//        for (k = i; k <= m; k++) a[k][i] *= scale;
//      }
//    }
//    w[i] = scale *g;
//    g = s = scale = 0.0;
//    if (i <= m && i != n) {
//      for (k = l; k <= n; k++) scale += fabs(a[i][k]);
//      if (scale) {
//        for (k = l; k <= n; k++) {
//          a[i][k] /= scale;
//          s += a[i][k] * a[i][k];
//        }
//        f = a[i][l];
//        g = -SIGN(sqrt(s), f);
//        h = f*g - s;
//        a[i][l] = f - g;
//        for (k = l; k <= n; k++) rv1[k] = a[i][k] / h;
//        for (j = l; j <= m; j++) {
//          for (s = 0.0, k = l; k <= n; k++) s += a[j][k] * a[i][k];
//          for (k = l; k <= n; k++) a[j][k] += s*rv1[k];
//        }
//        for (k = l; k <= n; k++) a[i][k] *= scale;
//      }
//    }
//    anorm = std::max(anorm, (fabs(w[i]) + fabs(rv1[i])));
//  }
//  for (i = n; i >= 1; i--) { //Accumulation of right-hand transformations.
//    if (i < n) {
//      if (g) {
//        for (j = l; j <= n; j++) //Double division to avoid possible underflow.
//          v[j][i] = (a[i][j] / a[i][l]) / g;
//        for (j = l; j <= n; j++) {
//          for (s = 0.0, k = l; k <= n; k++) s += a[i][k] * v[k][j];
//          for (k = l; k <= n; k++) v[k][j] += s*v[k][i];
//        }
//      }
//      for (j = l; j <= n; j++) v[i][j] = v[j][i] = 0.0;
//    }
//    v[i][i] = 1.0;
//    g = rv1[i];
//    l = i;
//  }
//  for (i = std::min(m, n); i >= 1; i--) { //Accumulation of left-hand transformations.
//    l = i + 1;
//    g = w[i];
//    for (j = l; j <= n; j++) a[i][j] = 0.0;
//    if (g) {
//      g = 1.0 / g;
//      for (j = l; j <= n; j++) {
//        for (s = 0.0, k = l; k <= m; k++) s += a[k][i] * a[k][j];
//        f = (s / a[i][i])*g;
//        for (k = i; k <= m; k++) a[k][j] += f*a[k][i];
//      }
//      for (j = i; j <= m; j++) a[j][i] *= g;
//    } else for (j = i; j <= m; j++) a[j][i] = 0.0;
//    ++a[i][i];
//  }
//  for (k = n; k >= 1; k--) { //Diagonalization of the bidiagonal form: Loop over
//    for (its = 1; its <= 30; its++) { // singular values, and over allowed iterations.
//        flag = 1;
//      for (l = k; l >= 1; l--) { //Test for splitting.
//        nm = l - 1; //Note that rv1[1] is always zero.
//        if ((float)(fabs(rv1[l]) + anorm) == anorm) {
//          flag = 0;
//          break;
//        }
//        if ((float)(fabs(w[nm]) + anorm) == anorm) break;
//      }
//      if (flag) {
//        c = 0.0; //Cancellation of rv1[l], if l > 1.
//        s = 1.0;
//        for (i = l; i <= k; i++) {
//          f = s*rv1[i];
//          rv1[i] = c*rv1[i];
//          if ((float)(fabs(f) + anorm) == anorm) break;
//          g = w[i];
//          h = pythag(f, g);
//          w[i] = h;
//          h = 1.0 / h;
//          c = g*h;
//          s = -f*h;
//          for (j = 1; j <= m; j++) {
//            y = a[j][nm];
//            z = a[j][i];
//            a[j][nm] = y*c + z*s;
//            a[j][i] = z*c - y*s;
//          }
//        }
//      }
//      z = w[k];
//      if (l == k) { //Convergence.
//        if (z < 0.0) { //Singular value is made nonnegative.
//          w[k] = -z;
//          for (j = 1; j <= n; j++) v[j][k] = -v[j][k];
//        }
//        break;
//      }
//      if (its == 30) msgError("no convergence in 30 svdcmp iterations");
//      x = w[l]; //Shift from bottom 2-by-2 minor.
//      nm = k - 1;
//      y = w[nm];
//      g = rv1[nm];
//      h = rv1[k];
//      f = ((y - z)*(y + z) + (g - h)*(g + h)) / (2.0*h*y);
//      g = pythag(f, 1.0);
//      f = ((x - z)*(x + z) + h*((y / (f + SIGN(g, f))) - h)) / x;
//      c = s = 1.0; //Next QR transformation:
//      for (j = l; j <= nm; j++) {
//        i = j + 1;
//        g = rv1[i];
//        y = w[i];
//        h = s*g;
//        g = c*g;
//        z = pythag(f, h);
//        rv1[j] = z;
//        c = f / z;
//        s = h / z;
//        f = x*c + g*s;
//        g = g*c - x*s;
//        h = y*s;
//        y *= c;
//        for (jj = 1; jj <= n; jj++) {
//          x = v[jj][j];
//          z = v[jj][i];
//          v[jj][j] = x*c + z*s;
//          v[jj][i] = z*c - x*s;
//        }
//        z = pythag(f, h);
//        w[j] = z; //Rotation can be arbitrary if z = 0.
//        if (z) {
//          z = 1.0 / z;
//          c = f*z;
//          s = h*z;
//        }
//        f = c*g + s*y;
//        x = c*y - s*g;
//        for (jj = 1; jj <= m; jj++) {
//          y = a[jj][j];
//          z = a[jj][i];
//          a[jj][j] = y*c + z*s;
//          a[jj][i] = z*c - y*s;
//        }
//      }
//      rv1[l] = 0.0;
//      rv1[k] = f;
//      w[k] = x;
//    }
//  }
////  free_vector(rv1, 1, n);
//}

void solveSVD(int nRows, int nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  //Eigen::VectorXd C = A.transpose().jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);
  Eigen::VectorXd C = A.transpose().jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
#elif defined( HAVE_OPENCV)
  cv::Mat A(nRows, nCols, CV_64F, a);
  cv::Mat B(nRows, 1, CV_64F, b);
  cv::Mat C(nCols, 1, CV_64F);
  cv::solve(A, B, C, cv::DECOMP_SVD);
  std::vector<double> v_aux;
  cvMatToVector(C, &v_aux);
  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  //TODO: O implementar método alternativo o devolver error
  // http://www2.units.it/ipl/students_area/imm2/files/Numerical_Recipes.pdf
  //svdcmp(a, nRows, nCols, b, float **v);
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
#elif defined( HAVE_OPENCV)
  cv::Mat A(nRows, nCols, CV_64F, a);
  cv::Mat B(nRows, 1, CV_64F, b);
  cv::Mat C(nCols, 1, CV_64F);
  cv::solve(A, B, C, cv::DECOMP_QR);
  std::vector<double> v_aux;
  cvMatToVector(C, &v_aux);
  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  //TODO: añadir método alternativo
#endif
}

#define TINY 1.0e-20 // A small number.
//Given a matrix a[1..n][1..n], this routine replaces it by the LU decomposition of a rowwise
//permutation of itself. a and n are input. a is output, arranged as in equation (2.3.14) above;
//indx[1..n] is an output vector that records the row permutation effected by the partial
//pivoting; d is output as ±1 depending on whether the number of row interchanges was even
//or odd, respectively. This routine is used in combination with lubksb to solve linear equations
//or invert a matrix.
//void ludcmp(float **a, int n, int *indx, float *d)
//{
//  int i, imax, j, k;
//  float big,dum,sum,temp;
//  float *vv;            // vv stores the implicit scaling of each row.
//  //...vv=vector(1,n);
//  *d=1.0;               // No row interchanges yet.
//  for (i=1;i<=n;i++) {  // Loop over rows to get the implicit scaling information.
//    big=0.0; 
//    for ( j = 1; j <= n; j++ )
//      if ( (temp = fabs(a[i][j])) > big ) big=temp;
//    if ( big == 0.0 ) return; //nrerror("Singular matrix in routine ludcmp");
//    // No nonzero largest element.
//    vv[i]=1.0/big; //Save the scaling.
//  }
//  for (j=1;j<=n;j++) { // This is the loop over columns of Crout’s method.
//  for (i=1;i<j;i++) {  // This is equation (2.3.12) except for i = j.
//    sum=a[i][j];
//    for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
//    a[i][j]=sum;
//  }
//  big=0.0; // Initialize for the search for largest pivot element.
//  for (i=j;i<=n;i++) {   // This is i = j of equation (2.3.12) and i = j+1. . .N
//    sum=a[i][j];         // of equation (2.3.13).
//    for (k=1;k<j;k++)
//      sum -= a[i][k]*a[k][j];
//    a[i][j]=sum;
//    if ( (dum=vv[i]*fabs(sum)) >= big) {
//      // Is the figure of merit for the pivot better than the best so far?
//      big=dum;
//      imax=i;
//    }
//  }
//  if (j != imax) { // Do we need to interchange rows?
//    for (k=1;k<=n;k++) { // Yes, do so...
//      dum=a[imax][k];
//      a[imax][k]=a[j][k];
//      a[j][k]=dum;
//    }
//    *d = -(*d); //...and change the parity of d.
//    vv[imax]=vv[j]; //Also interchange the scale factor.
//  }
//  indx[j]=imax;
//  if (a[j][j] == 0.0) a[j][j]=TINY;
//  // If the pivot element is zero the matrix is singular (at least to the precision of the
//  // algorithm). For some applications on singular matrices, it is desirable to substitute
//  // TINY for zero.
//  if (j != n) { // Now, finally, divide by the pivot element.
//    dum=1.0/(a[j][j]);
//    for (i=j+1;i<=n;i++) a[i][j] *= dum;
//  }
//} // Go back for the next column in the reduction.
////free_vector(vv,1,n);
//}
//
////Solves the set of n linear equations A·X = B. Here a[1..n][1..n] is input, not as the matrix
////A but rather as its LU decomposition, determined by the routine ludcmp. indx[1..n] is input
////as the permutation vector returned by ludcmp. b[1..n] is input as the right-hand side vector
////B, and returns with the solution vector X. a, n, and indx are not modified by this routine
////and can be left in place for successive calls with different right-hand sides b. This routine takes
////into account the possibility that b will begin with many zero elements, so it is efficient for use
////in matrix inversion.
//void lubksb(float **a, int n, int *indx, float b[])
//{
//  int i, ii = 0, ip, j;
//  float sum;
//  for (i=1;i<=n;i++) { //When ii is set to a positive value, it will become the      
//    ip=indx[i];      //index of the first nonvanishing element of b. Wenow
//    sum=b[ip];       //do the forward substitution, equation (2.3.6). The
//    b[ip]=b[i];      //only new wrinkle is to unscramble the permutation
//    if (ii)          //as we go.
//      for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
//    else if (sum) ii=i; // A nonzero element was encountered, so from now on we
//      b[i]=sum;         // will have to do the sums in the loop above.
//  }
//  for (i=n;i>=1;i--) { // Now we do the backsubstitution, equation (2.3.7).
//    sum=b[i];
//    for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
//    b[i]=sum/a[i][i]; //Store a component of the solution vector X.
//  } //All done!
//}

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
#elif defined( HAVE_OPENCV)
  cv::Mat A(nRows, nCols, CV_64F, a);
  cv::Mat B(nRows, 1, CV_64F, b);
  cv::Mat C(nCols, 1, CV_64F);
  cv::solve(A, B, C, cv::DECOMP_LU);
  std::vector<double> v_aux;
  cvMatToVector(C, &v_aux);
  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
#else
  //TODO: añadir método alternativo
#endif
}

// Standard Cholesky decomposition
void solveCholesky(int nRows, int nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  Eigen::LLT<Eigen::MatrixXd> cholesky(A);
  Eigen::VectorXd C = A.llt().solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
#elif defined( HAVE_OPENCV)
  cv::Mat A(nRows, nCols, CV_64F, a);
  cv::Mat B(nRows, 1, CV_64F, b);
  cv::Mat C(nCols, 1, CV_64F);
  cv::solve(A, B, C, cv::DECOMP_CHOLESKY);
  std::vector<double> v_aux;
  cvMatToVector(C, &v_aux);
  std::memcpy(c, v_aux.data(), nCols*sizeof(double));
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
  Eigen::LLT<Eigen::MatrixXd> cholesky(A);
  Eigen::VectorXd C = A.ldlt().solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));

}
#endif



} // End namespace TL
