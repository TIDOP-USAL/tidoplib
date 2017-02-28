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