#ifndef TL_CORE_MATH_UTILS_H
#define TL_CORE_MATH_UTILS_H

#include "config_tl.h"

#include <vector>
#include <array>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "core/messages.h"

namespace TL
{

/*!
 * \defgroup mathUtils Utilidades matemáticas
 *
 * Utilidades para operaciones entre vectores (tanto en el plano como en espacio),
 * funciones estadísticas, ajuste de nubes de puntos a diversas geometrias, resolución
 * de sistemas de ecuaciones lineales.
 *
 * Se utilizan contenedores y un tipo de punto generico (con métodos miembro x, y, z).
 *
 * \{
 */


/* ---------------------------------------------------------------------------------- */
/*                                 ALGEBRA                                            */
/* ---------------------------------------------------------------------------------- */
/* Operaciones entre vectores en el plano y en el espacio                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Vector entre dos puntos
 * \param[in] pt1 punto 1
 * \param[in] pt2 punto 2
 * \return Vector
 */
template<typename Point_t> inline
Point_t vector2D(const Point_t &pt1, const Point_t &pt2)
{
  Point_t v;
  v.x = pt2.x - pt1.x;
  v.y = pt2.y - pt1.y;
  return v;
}

template<typename Point_t> inline
Point_t vector3D(const Point_t &pt1, const Point_t &pt2)
{
  Point_t v;
  v.x = pt2.x - pt1.x;
  v.y = pt2.y - pt1.y;
  v.z = pt2.z - pt1.z;
  return v;
}

/*!
 * \brief Producto vectorial de dos vectores en el plano
 * \param[in] pt1 Vector o punto 1
 * \param[in] pt2 Vector o punto 2
 * \return Producto vectorial de los dos vectores
 */
template<typename Point_t> inline
double crossProduct(const Point_t &pt1, const Point_t &pt2)
{
  return static_cast<double>(pt1.x*pt2.y - pt1.y*pt2.x);
}

/*!
 * \brief Producto vectorial de dos vectores en 3 dimensiones
 * \param[in] pt1 Vector o punto 1
 * \param[in] pt2 Vector o punto 2
 * \return Producto vectorial de los dos vectores
 */
template<typename Point3_t> inline 
Point3_t crossProduct3D(const Point3_t &pt1, const Point3_t &pt2)
{
  return Point3_t(pt1.y*pt2.z - pt1.z*pt2.y, pt1.z*pt2.x - pt1.x*pt2.z, pt1.x*pt2.y - pt1.y*pt2.x);
}

/*!
 * \brief Producto escalar de dos vectores en el plano
 * \param[in] pt1 Vector o punto 1
 * \param[in] pt2 Vector o punto 2
 * \return Producto escalar de los dos vectores
 */
template<typename Point_t> inline 
size_t dotProduct(const Point_t &pt1, const Point_t &pt2)
{
  return static_cast<double>(pt1.x)*static_cast<double>(pt2.x) + 
         static_cast<double>(pt1.y)*static_cast<double>(pt2.y);
}

/*!
 * \brief Producto escalar de dos vectores en 3 dimensiones
 * \param[in] pt1 Vector o punto 1
 * \param[in] pt2 Vector o punto 2
 * \return Producto escalar de los dos vectores
 */
template<typename Point3_t> inline 
size_t dotProduct3D(const Point3_t &pt1, const Point3_t &pt2)
{
  return static_cast<double>(pt1.x)*static_cast<double>(pt2.x) + 
    static_cast<double>(pt1.y)*static_cast<double>(pt2.y) + 
    static_cast<double>(pt1.z)*static_cast<double>(pt2.z);
}

/*!
 * \brief Módulo de un vector 2D
 * \param[in] v Vector 
 */
template<typename Point_t> inline 
size_t module(const Point_t &v)
{
  return sqrt(dotProduct(v, v));
}

/*!
 * \brief Módulo de un vector 3D
 * \param[in] v Vector
 */
template<typename Point3_t> inline 
size_t module3D(const Point3_t &v)
{
  return sqrt(dotProduct3D(v, v));
}

/*!
 * \brief Devuelve el ángulo entre dos vectores
 * \param[in] v1 Vector 1
 * \param[in] v2 Vector 2
 * \return Ángulo en radianes
 */
template<typename Point_t> inline 
double vectorAngle(const Point_t &v1, const Point_t &v2)
{
  if (v1 == Point_t() || v2 == Point_t())
    return 0.0;
  return acos((v1.x*v2.x + v1.y*v2.y) / (module(v1) * module(v2)));
}

template<typename Point_t> inline 
double vectorAngleOX(const Point_t &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.y, v.x);
  return angle; 
}

template<typename Point_t> inline 
double vectorAngleOY(const Point_t &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.x, v.y);
  return angle;
}

template<typename Point_t> inline 
double azimut(const Point_t &pt1, const Point_t &pt2)
{
  double azimut = 0.;
  Point_t v = vector2D(pt1, pt2);
  if (v.x == 0 && v.y == 0) return azimut;
  azimut = atan2(v.x, v.y);
  if (azimut < 0.) azimut += TL_2PI;
  return azimut;
}

/*!
 * \brief ángulos de Euler
 *
 * A partir de una matriz de rotación obtiene los ángulos de giro.
 *
 * Referencias:
 * https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
 * http://mathworld.wolfram.com/EulerAngles.html
 *
 * \param[in] R Matriz de rotación
 * \param[out] omega Rotación respecto al eje X en radianes
 * \param[out] phi Rotación respecto al eje Y en radianes
 * \param[out] kappa Rotación respecto al eje Z en radianes
 */
TL_EXPORT void eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa);

//Otra posible solución. Realizar un test en condiciones y ver cual es mejor 
//https://www.learnopencv.com/rotation-matrix-to-euler-angles/ 
//TL_EXPORT void eulerAngles2(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa);

//... Esto hace lo mismo que rotationMatrix (transform.h) para el caso de orden xyz

/*!
 * \brief Cálculo de la matriz de rotación a partir de los angulos de euler
 * \param[in] rX Rotación respecto al eje X en radianes
 * \param[in] rY Rotación respecto al eje Y en radianes
 * \param[in] rZ Rotación respecto al eje Z en radianes
 * \param[out] R Matriz de rotación
 */
//TL_EXPORT void eulerAnglesToRotationMatrix(double rX, double rY, double rZ, std::array<std::array<double, 3>, 3> *R);

/*!
 * \brief Cálculo de la matriz de rotación respecto al eje X
 * \param[in] rX Rotación respecto al eje X en radianes
 * \param[out] RX Matriz de rotación
 */
void RotationMatrixAxisX(double rX, std::array<std::array<double, 3>, 3> *RX);

/*!
 * \brief Cálculo de la matriz de rotación respecto al eje Y
 * \param[in] rY Rotación respecto al eje Y en radianes
 * \param[out] RY Matriz de rotación
 */
void RotationMatrixAxisY(double rY, std::array<std::array<double, 3>, 3> *RY);

/*!
 * \brief Cálculo de la matriz de rotación respecto al eje Z
 * \param[in] rZ Rotación respecto al eje Z en radianes
 * \param[out] RZ Matriz de rotación
 */
void RotationMatrixAxisZ(double rZ, std::array<std::array<double, 3>, 3> *RZ);

/*!
 * \brief Cálculo de la matriz de rotación
 * \param[in] omega Rotación respecto al eje X en radianes
 * \param[in] phi Rotación respecto al eje Y en radianes
 * \param[in] kappa Rotación respecto al eje Z en radianes
 * \param[out] R Matriz de rotación
 */
TL_EXPORT void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R);



/* ---------------------------------------------------------------------------------- */
/*                  RESOLUCIÓN DE SISTEMAS DE ECUACIONES LINEALES                     */
/* ---------------------------------------------------------------------------------- */

// La resolución de sistemas lineales con OpenCV es muy lenta:
// http://www.patrikhuber.ch/blog/5-solving-linear-systems-with-eigen
// Se usa Eigen en caso de que este presente la libreria.

/*!
 * \brief Resolución de Sistemas Lineales mediante SVD (Singular value decomposition)
 *
 * En álgebra lineal, la descomposición en valores singulares de una matriz real
 * o compleja es una factorización de la misma con muchas aplicaciones, entre ellas
 * la resolución de sistemas lineales.
 *
 * Dada una matriz real A de m×n, existen matrices ortogonales U (de orden m) y V (de orden n)
 * y una matriz diagonal Σ (de tamaño m×n) tales que:
 *
 * \f[ A = UΣV^T  \f]
 *
 * Esta factorización de A se llama descomposición en valores singulares de A.
 *
 *
 * http://www.ehu.eus/izaballa/Cursos/valores_singulares.pdf
 * https://www.researchgate.net/publication/263583897_La_descomposicion_en_valores_singulares_SVD_y_algunas_de_sus_aplicaciones
 * \param[in] nRows Número de filas
 * \param[in] nCols Número de columnas
 * \param[in] a
 * \param[in] b
 * \param[out] c
 */
TL_EXPORT void solveSVD(int nRows, int nCols, double *a, double *b, double *c);

/*!
 * \brief Factorización QR
 *
 * La descomposición o factorización QR de una matriz es una descomposición de
 * la misma como producto de una matriz ortogonal por una triangular superior.
 *
 * La descomposición QR de una matriz cuadrada real A es:
 *
 * \f[ A = QR \f]
 *
 * donde Q es una matriz ortogonal:
 *
 * \f[ Q^TQ = I \f]
 *
 * y R es una matriz triangular superior.
 * \param[in] nRows Número de filas
 * \param[in] nCols Número de columnas
 * \param[in] a
 * \param[in] b
 * \param[out] c
 */
TL_EXPORT void solveQR(int nRows, int nCols, double *a, double *b, double *c);

/*!
 * \brief Factorización o descomposición LU
 *
 * Sea A una matriz no singular (si lo fuera, entonces la descomposición podría no ser única)
 *
 * \f[ A=LU \f]
 *
 * donde L y U son matrices inferiores y superiores triangulares respectivamente.
 *
 * \param[in] nRows Número de filas
 * \param[in] nCols Número de columnas
 * \param[in] a
 * \param[in] b
 * \param[out] c
 */
TL_EXPORT void solveLU(int nRows, int nCols, double *a, double *b, double *c);

/*!
 * \brief Resolución de sistemas de ecuaciones lineales mediante la Factorización Cholesky
 *
 * Cualquier matriz cuadrada A con pivotes no nulos puede ser escrita como el producto
 * de una matriz triangular inferior L y una matriz triangular superior U; esto recibe
 * el nombre de factorización LU. Sin embargo, si A es simétrica y definida positiva,
 * se pueden escoger los factores tales que U es la transpuesta de L, y esto se llama
 * la descomposición o factorización de Cholesky.
 *
 * La matriz A tiene que ser simétrica y definida positiva.
 *
 * \param[in] nRows Número de filas
 * \param[in] nCols Número de columnas
 * \param[in] a
 * \param[in] b
 * \param[out] c
 */
TL_EXPORT void solveCholesky(int nRows, int nCols, double *a, double *b, double *c);

#ifdef HAVE_EIGEN

/*!
 * \brief Resolución de sistemas de ecuaciones lineales mediante la Factorización Cholesky
 *
 * Resolución por Cholesky para el caso de matrices semidefinidas positivas o negativas
 *
 * \param[in] nRows Número de filas
 * \param[in] nCols Número de columnas
 * \param[in] a
 * \param[in] b
 * \param[out] c
 */
TL_EXPORT void solveRobustCholesky(int nRows, int nCols, double *a, double *b, double *c);

#endif




/* ---------------------------------------------------------------------------------- */
/*                                 ESTADISTICA                                        */
/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

/*!
 * \brief Varianza del laplaciano
 * \param[in] src
 * \return
 */
TL_EXPORT double laplacianVariance(const cv::Mat &src);

#endif

template<typename T> inline
double mean(const T &data)
{
  size_t n = data.size();
  if (n <= 1) return 0.0; //TODO: Mensaje de error
  //TODO: Comprobar que es un contenedor con tipos numéricos
  typename T::value_type sum = 0.0;
  for (auto &t : data) {
    sum += t;
  }
  return static_cast<double>(sum / n);
}

template<typename T> inline 
double variance(const T &data)
{
  size_t n = data.size();
  if (n <= 1) return 0.0; //TODO: Mensaje de error
  double _mean = mean(data);
  double sum = 0.0;
  double ep = 0.0;
  double aux = 0.0;
  for (auto &t : data) {
    aux = t - _mean;
    ep += aux;
    sum += aux*aux;
  }
  //return sum / (n-1);
  // corrected two-pass algorithm
  return (sum - ep*ep / n) / (n - 1);
}

template<typename T> inline 
double standarDeviation(const T &data)
{
  return sqrt(variance(data));
}

//average deviation or mean absolute deviation
// Estimador mas robusto que la desviacion estandar y la varianza
template<typename T> inline 
double averageAbsoluteDeviation(const T &data)
{
  size_t n = data.size();
  if (n <= 1) return 0.0; // Mensaje de error
  double _mean = mean(data);
  double sum = 0.0;
  for (auto &t : data) {
    sum += std::abs(t - _mean);
  }
  return sum / n;
}

template<typename T> inline 
double skewness(const T &data)
{
  // Para que sea lo mas rapido posible no llamo a las funciones previas
  // Hago todo el desarrollo aqui para no repetir pasos
  size_t n = data.size();
// Varianza
  if (n <= 1) return 0.; // Mensaje de error
  double _mean = mean(data);
  double ep = 0.;
  double aux = 0.;
  double aux2 = 0.;
  double skew = 0.;
  double sum = 0.;
  for (auto &t : data) {
    aux = t - _mean;
    ep += aux;
    sum += (aux2 = aux*aux);
    skew += aux2 * aux; 
  }

  double variance =  (sum - ep*ep / n) / (n - 1);  // Varianza Corrected two-pass formula

  if (variance) {
    double stdDev = sqrt(variance); // Desviación tipica
    return skew / (n*variance*stdDev);
  } else return 0.;// "No skew when variance = 0 (in moment)")
}

template<typename T> inline 
double kurtosis(const T &data)
{
  size_t n = data.size();
// Varianza
  if (n <= 1) return 0.; // Mensaje de error
  double _mean = mean(data);
  double ep = 0.;
  double aux = 0.;
  double _kurtosis = 0.;
  double aux2 = 0.;
  double sum = 0.;

  for (auto &t : data) {
    aux = t - _mean;
    ep += aux;
    sum += (aux2 = aux*aux);
    _kurtosis += aux2;
  }

  double variance = (sum - ep*ep / n) / (n - 1);  // Varianza Corrected two-pass formula

  if (variance) {
    _kurtosis /= (n*variance*variance);
    return _kurtosis - 3.;
  } else return 0.;// "No kurtosis when variance = 0 (in moment)")
}

TL_EXPORT double computeMedian(const std::vector<double> &input);

TL_EXPORT double computeTempMAD(const std::vector<double> &input, double median);

TL_EXPORT bool isOutlier(double temp, double median, double mad);

////TODO: deberian generalizarse para cualquier tipo de contenedor
//template<typename T> inline 
//T computeMedian(const std::vector<T> &input)
//{
//  size_t size = input.size();
//  if (size % 2 == 0)
//    return (input[size / 2 - 1] + input[size / 2]) / 2;
//  else
//    return input[size / 2];
//}
//
//template<typename T> inline 
//T computeTempMAD(const std::vector<T> &input, T median)
//{
//  std::vector<T> inp = input;
//  for (size_t i = 0; i < inp.size(); ++i) {
//    inp[i] = abs(inp[i] - median);
//  }
//  sort(inp.begin(), inp.end());
//  return computeMedian(inp)*1.4826;
//}

/* ---------------------------------------------------------------------------------- */
/*           AJUSTES DE PUNTOS A GEOMETRIAS (LINEAS, PLANOS, ...)                     */
/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Recta de regresión de Y sobre X
 * La recta de regresión de Y sobre X se utiliza para estimar los valores de la Y a partir de los de la X.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable X.
 *
 * \f$ y = m * x + b\f$<BR>
 *
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return Coeficiente de correlacción. Valor entre -1 (pendiente negativa) y 1 (pendiente positiva). Valores próximos a cero suponen un mal ajuste
 *
 * <h4>Ejemplo</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearYX(pts, &m, &b);
 * \endcode
 */
//TL_EXPORT double regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b);
template<typename Point_t> inline 
double regressionLinearYX(const std::vector<Point_t> &pts, double *m, double *b)
{
  double corr = 0.0;
  double sx = 0., sy = 0., sx2 = 0., sy2 = 0., sxy = 0.;
  size_t n = pts.size();
  if (n >= 2) {
    for (size_t i = 0; i < n; i++) {
      sx += pts[i].x;
      sy += pts[i].y;
      sx2 += pts[i].x * pts[i].x;
      sy2 += pts[i].y * pts[i].y;
      sxy += pts[i].x * pts[i].y;
    }
    double den = (n*sx2 - sx*sx);
    if (den) {
      // Línea no vertical
      *m = (n*sxy - sy*sx) / (n*sx2 - sx*sx);
      *b = (sy - *m*sx) / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sy2 - sy*sy / n));
    }
  }
  return(corr);
}

/*!
 * \brief Recta de regresión de X sobre Y
 * La recta de regresión de X sobre Y se utiliza para estimar los valores de la X a partir de los de la Y.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable Y.
 *
 * \f$ x = m*y + b\f$<BR>
 *
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return Coeficiente de correlacción. Valor entre -1 (pendiente negativa) y 1 (pendiente positiva). Valores próximos a cero suponen un mal ajuste
 *
 * <h4>Ejemplo</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearXY(pts, &m, &b);
 * \endcode
 */
//TL_EXPORT double regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b);
template<typename Point_t> inline 
double regressionLinearXY(const std::vector<Point_t> &pts, double *m, double *b)
{
  double corr = 0.0;
  double sx = 0., sy = 0., sx2 = 0., sy2 = 0., sxy = 0.;
  size_t n = pts.size();
  if (n >= 2) {
    for (size_t i = 0; i < n; i++) {
      sx += pts[i].x;
      sy += pts[i].y;
      sx2 += pts[i].x * pts[i].x;
      sy2 += pts[i].y * pts[i].y;
      sxy += pts[i].x * pts[i].y;
    }
    double den = (n*sy2 - sy*sy);
    if (den) {
      *m = (n*sxy - sy*sx) / (n*sy2 - sy*sy);
      *b = (sx - *m*sy) / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sx2 - sx*sx / n));
    }
  }
  return(corr);
}



/*!
 * \brief Ajusta una recta a un conjunto de puntos por mínimos cuadrados
 *
 * La recta de regresión de Y sobre X se utiliza para estimar los valores de la Y a partir de los de la X.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable X.
 *
 * \f$ y = m * x + b\f$<BR>
 *
 * Si se calcula la recta de regresión de Y sobre X la expresión utilizada es:
 *
 * \f$ x = m*y + b\f$<BR>
 * https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf
 *
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \param[int] YX Si es true (valor por defecto) se cálcula la recta de regresión de Y sobre X. En caso contrario se calcula la recta de X sobre Y.
 * \return Coeficiente de correlacción. Valor entre -1 (pendiente negativa) y 1 (pendiente positiva). Valores próximos a cero suponen un mal ajuste
 *
 * <h4>Ejemplo de determinación de la recta de mejor ajuste</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = linearFittingLS(pts, &m, &b);
 *
 * // Para el caso de la recta de X sobre Y
 * corr = linearFittingLS(pts, &m, &b, false); 
 *
 * \endcode
 */
template<typename Point_t> inline 
double linearFittingLS(const std::vector<Point_t> &pts, double *m, double *b, bool YX = true)
{
  double corr = 0.0;
  size_t size = pts.size();
  if (size >= 2) {
    int _m = static_cast<int>(size) * 2, _n = 2;
    double *_a = new double[_m*_n], *pa = _a, *_b = new double[_m], *pb = _b;
    double *_c = new double[_n];
    double sum_sx = 0., sum_sy = 0., sum_sx2 = 0., sum_sy2 = 0., sum_sxy = 0.;
    double sx = 0., sy = 0., sx2 = 0., sy2 = 0., sxy = 0.;
    for (size_t i = 0; i < size; i++) {
      sum_sx += (sx = pts[i].x);
      sum_sy += (sy = pts[i].y);
      sum_sx2 += (sx2 = pts[i].x * pts[i].x);
      sum_sy2 += (sy2 = pts[i].y * pts[i].y);
      sum_sxy += (sxy = pts[i].x * pts[i].y );
      *pa++ = YX ? sx2 : sy2;
      *pa++ = YX ? sx : sy;
      *pb++ = sxy;
      *pa++ = YX ? sx : sy;
      *pa++ = 1;
      *pb++ = YX ? sy : sx;
    }

    solveSVD(_m, _n, _a, _b, _c);
    *m = _c[0];
    *b = _c[1];
    double div = YX ? (sum_sy2 - sum_sy*sum_sy / size) : (sum_sx2 - sum_sx*sum_sx / size);
    if (div != 0) {
      corr = sqrt(*m * (sum_sxy - sum_sx*sum_sy / size) / div);
    }
  }
  return(corr);
}

/*!
 * \brief Regresión exponencial
 * Ajusta una curva exponencial de regresión a un conjunto de datos según la formula:
 * 
 * \f$ y = A*r^x \f$<BR>
 *
 * A partir de esta expresión tomando logaritmos en ambos lados tenemos que:
 *
 * \f$ log10(y) = log10(A*r^x) \f$<BR>
 * \f$ log10(y) = log10(A) + log10(r^x) \f$<BR>
 * \f$ log10(y) = log10(A) + x * log10(r) \f$<BR>
 *
 * El logaritmo de <b>y</B> se puede expresar ahora como una función lineal y resolver mediante 
 * la recta de regresión donde:
 *
 * \f$ m = log10(r) \f$<BR>
 * \f$ b = log10(A) \f$<BR>
 *
 * y por tanto:
 *
 * \f$ r = 10^m \f$<BR>
 * \f$ A = 10^b \f$<BR>
 *
 * \param[in] pts Puntos
 * \param[out] A 
 * \param[out] r 
 */
//template<typename Point_t> inline 
//void expRegression(const std::vector<Point_t> &pts, double *A, double *r)
//{
//  std::vector<geometry::PointD> ptsLog(pts.size());
//  std::transform(pts.begin(), pts.end(), ptsLog.begin(), 
//                 [](Point_t pt) -> geometry::PointD {
//                    return geometry::PointD(pt.x, log10(pt.y)); 
//                  });
//
//  double m = 0.;
//  double b = 0.;
//  regressionLinearYX<geometry::PointD>(ptsLog, &m, &b);
//  *A = pow(10, b);
//  *r = pow(10, m);
//}


/*!
 * \brief Obtiene la ecuación de un plano que pasa por tres puntos
 *
 * \f$ A*x + B*y + C*z + D = 0\f$
 *
 * \param[in] points Puntos que definen el plano
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[int] normalize Si es verdadero normaliza la ecuación del plano
 * \return Normal al plano
 */
template<typename T> inline 
double threePointsPlane(const std::array<T, 3> &points, std::array<double, 4> &plane, bool normalize = false) 
{
  T v1 = points[1] - points[0];
  T v2 = points[2] - points[0];
  plane[0] = v1.y*v2.z - v1.z*v2.y;
  plane[1] = v1.z*v2.x - v1.x*v2.z;
  plane[2] = v1.x*v2.y - v2.x*v1.y;
  plane[3] = -plane[0] *points[0].x - plane[1]*points[0].y - plane[2]*points[0].z;
  double N = sqrt(plane[0]*plane[0] + plane[1]*plane[1] + plane[2]*plane[2]);
  if ( N && normalize ) { 
    plane[0] /= N;
    plane[1] /= N;
    plane[2] /= N;
    plane[3] /= N;
  }
  return N ;
}

/*!
 * \brief Obtiene la ecuación del plano de mejor ajuste de un conjunto de puntos resolviendo el sistema de ecuaciones por minimos cuadrados
 *
 * \f$ A*x + B*y + C*z + D = 0\f$
 *
 * Referencias:
 *
 * https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf
 * http://casanchi.com/mat/ajustesuperficies01.pdf
 *
 * \param[in] points Puntos que definen el plano
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[int] normalize Si es verdadero normaliza la ecuación del plano
 * \return Normal al plano
 */
template<typename Point_t> inline 
double nPointsPlaneLS(const std::vector<Point_t> &points, std::array<double, 4> &plane, bool normalize = false)
{
  double N = 0.;
  size_t size = points.size();
  if (size < 3) {
    return 0.; // O devolver error
  } else if (size == 3) {
    std::array<Point_t, 3> _points;
    std::copy_n(points.begin(), 3, _points.begin());
    threePointsPlane(_points, plane, normalize);
  } else {
    int m = static_cast<int>(size) * 3, n = 3;
    double *_a = new double[m*n], *pa = _a, *_b = new double[m], *pb = _b;
    double *_c = new double[n];
    try {
      for (int i = 0; i < size; i++) {
        *pa++ = points[i].x*points[i].x;
        *pa++ = points[i].x*points[i].y;
        *pa++ = points[i].x;
        *pb++ = points[i].x*points[i].z;
        *pa++ = points[i].x*points[i].y;
        *pa++ = points[i].y*points[i].y;
        *pa++ = points[i].y;
        *pb++ = points[i].z*points[i].y;
        *pa++ = points[i].x;
        *pa++ = points[i].y;
        *pa++ = 1;
        *pb++ = points[i].z;
      }

      solveSVD(m, n, _a, _b, _c);
      
      plane[0] = _c[0];
      plane[1] = _c[1];
      plane[2] = -1;
      plane[3] = _c[2];

      // Por si me interesase devolver el error
      double dz = 0.;
      double sumErr = 0.;
      for (int i = 0; i < size; i++) {
        dz = points[i].z - (plane[0]*points[i].x + plane[1]*points[i].y + plane[3]);
        sumErr += dz*dz;
      }

      N = sqrt(plane[0]*plane[0] + plane[1]*plane[1] + 1);

      if ( N && normalize ) { 
        plane[0] /= N;
        plane[1] /= N;
        plane[2] /= N;
        plane[3] /= N;
      }
      
    } catch (std::exception &e) {
      msgError(e.what());
    }

  }
  return N ;
}

//Lo mismo pero con contenedores
template<typename it> inline 
double nPointsPlaneLS(it it_begin, it it_end, std::array<double, 4> &plane, bool normalize = false)
{
  double N = 0.;
  typename std::iterator_traits<it>::difference_type size = std::distance(it_begin, it_end);
  typedef typename std::iterator_traits<it>::value_type Point3_t;
  if (size < 3) {
    return 0.; // O devolver error
  } else if (size == 3) {
    std::array<Point3_t, 3> _points;
    std::copy_n(it_begin, 3, _points.begin());
    threePointsPlane(_points, plane, normalize);
  } else {
    int m = static_cast<int>(size) * 3, n = 3;
    double *_a = new double[m*n], *pa = _a, *_b = new double[m], *pb = _b;
    double *_c = new double[n];
    try {
      for (int i = 0; i < size; i++, it_begin++) {
        *pa++ = it_begin->x*it_begin->x;
        *pa++ = it_begin->x*it_begin->y;
        *pa++ = it_begin->x;
        *pb++ = it_begin->x*it_begin->z;
        *pa++ = it_begin->x*it_begin->y;
        *pa++ = it_begin->y*it_begin->y;
        *pa++ = it_begin->y;
        *pb++ = it_begin->z*it_begin->y;
        *pa++ = it_begin->x;
        *pa++ = it_begin->y;
        *pa++ = 1;
        *pb++ = it_begin->z;

      }

      solveSVD(m, n, _a, _b, _c);
      
      plane[0] = _c[0];
      plane[1] = _c[1];
      plane[2] = -1;
      plane[3] = _c[2];

      //// Por si me interesase devolver el error
      //double dz = 0.;
      //double sumErr = 0.;
      //for (int i = 0; i < size; i++) {
      //  dz = points[i].z - (plane[0]*points[i].x + plane[1]*points[i].y + plane[3]);
      //  sumErr += dz*dz;
      //}

      N = sqrt(plane[0]*plane[0] + plane[1]*plane[1] + 1);

      if ( N && normalize ) { 
        plane[0] /= N;
        plane[1] /= N;
        plane[2] /= N;
        plane[3] /= N;
      }
      
    } catch (std::exception &e) {
      msgError(e.what());
    }

  }
  return N ;
}

/* ---------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------- */


#ifdef HAVE_OPENCV

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por filas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
TL_EXPORT int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por columnas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
TL_EXPORT int sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

#endif



/* ---------------------------------------------------------------------------------- */
/*                               Conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup angleConversion Conversión de ángulos
 *  
 * Conversiones entre los diferentes formatos ángulares. 
 * - grados sexagesimales.
 * - grados centesimales
 * - radianes
 * \{
 */

template<typename T>
inline int isNegative(T t) 
{
  return t < 0 ? -1 : 1;
}

/*!
 * \brief Conversión de grados sexagesimales a grados sexagesimales en notación decimal
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados sexagesimales en notación decimal
 */
TL_EXPORT double degreesToDecimalDegrees(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a radianes
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return radianes
 */
TL_EXPORT double degreesToRadians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a grados centesimales
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados centesimales
 */
TL_EXPORT double degreesToGradians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados, minutos y segundos
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 *
 *
 * <h4>Ejemplo</h4>
 * \code
 * int degrees, minutes, seconds;
 * decimalDegreesToDegrees(55.666, &degrees, &minutes, &seconds);
 * \endcode
 */
TL_EXPORT void decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a radianes
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Radianes
 */
TL_EXPORT double decimalDegreesToRadians(double decimalDegrees);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados centesimales
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Grados centesimales
 */
TL_EXPORT double decimalDegreesToGradians(double decimalDegrees);

/*!
 * \brief Conversión de radianes a grados, minutos y segundos
 * \param[in] radians Radianes
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
TL_EXPORT void radiansToDegrees(double radians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de radianes a grados sexagesimales en notación decimal
 * \param[in] radians Radianes
 * \return Grados sexagesimales en notación decimal
 */
TL_EXPORT double radiansToDecimalDegrees(double radians);

/*!
 * \brief radiansToGradians
 * \param[in] radians Radianes
 * \return Grados centesimales
 */
TL_EXPORT double radiansToGradians(double radians);

/*!
 * \brief Conversión de grados centesimales a grados, minutos y segundos
 * \param[in] gradians Grados centesimales
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
TL_EXPORT void gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados centesimales a grados sexagesimales en notación decimal
 * \param[in] gradians Grados centesimales
 * \return Grados sexagesimales en notación decimal
 */
TL_EXPORT double gradiansToDecimalDegrees(double gradians);

/*!
 * \brief Conversión de grados centesimales a radianes
 * \param[in] gradians Grados centesimales
 * \return Radianes
 */
TL_EXPORT double gradiansToRadians(double gradians);

/*! \} */ // end of angleConversion


/*! \} */ // end of mathUtils

} // End namespace TL

#endif // TL_CORE_MATH_UTILS_H
