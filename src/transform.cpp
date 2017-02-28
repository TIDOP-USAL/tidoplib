#include "transform.h"

#include "core/utils.h"

#ifdef HAVE_EIGEN
#include <Eigen/SVD>
#include <Eigen/LU>
#include <Eigen/QR>
#endif

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy)
{
  PointI t(dx, dy);
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
  // O implementar método alternativo o devolver error
  // http://www2.units.it/ipl/students_area/imm2/files/Numerical_Recipes.pdf
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

#endif
}

// Robust Cholesky decomposition
void solveRobustCholesky(int nRows, int nCols, double *a, double *b, double *c)
{
#ifdef HAVE_EIGEN
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  Eigen::LLT<Eigen::MatrixXd> cholesky(A);
  Eigen::VectorXd C = A.ldlt().solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
#endif
}

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL

Crs::Crs(const char *epsg, const char *grid, const char *geoid) 
  : mEpsg(epsg), mGrid(grid), mGeoid(geoid)
{
  pCrs->importFromEPSG(atoi(epsg+5));
  if ( grid ) {
    char *cprj = NULL;
    pCrs->exportToProj4(&cprj);
    std::string crs_prj4 = std::string(cprj) + "+nadgrids=" + mGrid;
    pCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(cprj);
  }
  if (geoid) {
    char *prjin = NULL;
    pCrs->exportToProj4(&prjin);
    std::string crs_prj4 = std::string(prjin) + "+geoidgrids=" + mGeoid;
    pCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(prjin);
  }
}

Crs::~Crs()
{
  OGRSpatialReference::DestroySpatialReference(pCrs);
}

const char *Crs::getEPSG() 
{ 
  return mEpsg.c_str(); 
};

bool Crs::isGeocentric()
{
  return pCrs->IsGeocentric() != 0;
}

bool Crs::isGeographic()
{
  return pCrs->IsGeographic()!= 0;
}


std::unique_ptr<CrsCache> CrsCache::sCrsCache;

void CrsCache::get()
{
  if (sCrsCache.get() == 0) {
    sCrsCache.reset(new CrsCache());
  }
}

#endif // HAVE_GDAL

} // End namespace I3D