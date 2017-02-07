#ifndef I3D_MATHUTILS_H
#define I3D_MATHUTILS_H

#include <vector>
#include <array>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "geometric_entities/point.h"
#include "geometric_entities/segment.h" 

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

/*!
 * \brief Varianza del laplaciano
 * \param[in] src
 * \return
 */
I3D_EXPORT double laplacianVariance(const cv::Mat &src);

#endif

template<typename Point_t>
I3D_EXPORT inline double module(const Point_t &v)
{
  return sqrt(v.x * v.x + v.y*v.y);
}

/*!
 * \brief Devuelve el ángulo entre dos vectores
 * \param[in] v1 Vector 1
 * \param[in] v2 Vector 2
 * \return Ángulo en radianes
 */
template<typename Point_t>
I3D_EXPORT inline double vectorAngle(const Point_t &v1, const Point_t &v2)
{
  if (v1 == Point_t() || v2 == Point_t())
    return 0.0;
  return acos((v1.x*v2.x + v1.y*v2.y) / (module(v1) * module(v2)));
}

template<typename Point_t>
I3D_EXPORT inline double vectorAngleOX(const Point_t &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.y, v.x);
  return angle; 
}

template<typename Point_t>
I3D_EXPORT inline double vectorAngleOY(const Point_t &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.x, v.y);
  return angle;
}

template<typename Point_t>
I3D_EXPORT inline double azimut(const Point_t &pt1, const Point_t &pt2)
{
  double azimut = 0.;
  Point_t v = pt2 - pt1;
  if (v.x == 0 && v.y == 0) return azimut;
  azimut = atan2(v.x, v.y);
  if (azimut < 0.) azimut += I3D_2PI;
  return azimut;
}

I3D_EXPORT double computeMedian(const std::vector<double> &input);

I3D_EXPORT double computeTempMAD(const std::vector<double> &input, const double median);

I3D_EXPORT bool isOutlier(const double temp, const double median, const double mad);

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
//I3D_EXPORT double regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b);
template<typename Point_t>
I3D_EXPORT double regressionLinearYX(const std::vector<Point_t> &pts, double *m, double *b)
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
      den = sy2 - sy*sy / n;
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
//I3D_EXPORT double regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b);
template<typename Point_t>
I3D_EXPORT double regressionLinearXY(const std::vector<Point_t> &pts, double *m, double *b)
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
      den = sx2 - sx*sx / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sx2 - sx*sx / n));
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
template<typename Point_t>
I3D_EXPORT void expRegression(const std::vector<Point_t> &pts, double *A, double *r)
{
  std::vector<PointD> ptsLog(pts.size());
  std::transform(pts.begin(), pts.end(), ptsLog.begin(), [](Point_t pt) -> PointD { return PointD(pt.x, log10(pt.y)); });

  double m = 0.;
  double b = 0.;
  regressionLinearYX<PointD>(ptsLog, &m, &b);
  *A = pow(10, b);
  *r = pow(10, m);
}

/*!
 * \brief ángulos de Euler
 *
 * A partir de una matriz de rotación obtiene los ángulos de giro.
 * \param[in] R Matriz de rotación
 * \param[out] omega Rotación respecto al eje X en radianes
 * \param[out] phi Rotación respecto al eje Y en radianes
 * \param[out] kappa Rotación respecto al eje Z en radianes
 * \param[in] order Orden de ejes
 * https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
 * http://mathworld.wolfram.com/EulerAngles.html
 */
I3D_EXPORT void eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa);
//Otra posible solución. Realizar un test en condiciones y ver cual es mejor 
//https://www.learnopencv.com/rotation-matrix-to-euler-angles/ 
//I3D_EXPORT void eulerAngles2(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa);

//... Esto hace lo mismo que rotationMatrix (transform.h) para el caso de orden xyz
/*!
 * \brief Cálculo de la matriz de rotación a partir de los angulos de euler
 * \param[in] rX Rotación respecto al eje X en radianes
 * \param[in] rY Rotación respecto al eje Y en radianes
 * \param[in] rZ Rotación respecto al eje Z en radianes
 * \param[out] R Matriz de rotación
 */
//I3D_EXPORT void eulerAnglesToRotationMatrix(double rX, double rY, double rZ, std::array<std::array<double, 3>, 3> *R);

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

#ifdef HAVE_OPENCV

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por filas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
I3D_EXPORT int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por columnas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
I3D_EXPORT int sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

#endif

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
template<typename T>
I3D_EXPORT double threePointsPlane(const std::array<T, 3> &points, std::array<double, 4> &plane, bool normalize = false) 
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
  return( N );
}

/*!
 * \brief Obtiene la distancia de un punto a un plano
 *
 * \param[in] point Punto
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \return Distancia del punto al plano
 */
template<typename Point_t>
I3D_EXPORT double distantePointToPlane(const Point_t &pt, const std::array<double, 4> &plane) 
{
  double num = plane[0] * pt.x + plane[1] * pt.y + plane[2] * pt.z + plane[3];
  double normal = sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
  if ( normal == 0. ) throw std::runtime_errorr( "3 puntos alineados" );
  return(num / normal);
}

/*!
 * \brief Obtiene el ángulo entre una recta y un plano
 *
 * \param[in] line Linea
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \return Ángulo entre una recta y un plano
 */
//template<typename T>
//I3D_EXPORT double linePlaneAngle(const Segment3D<T> &line, const std::array<double, 4> &plane)
//{
//  Point3D v = line.vector();
//  double num = plane[0] * v.x + plane[1] * v.y + plane[2] * v.z;
//  double den = sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]) + line.length();
//  return den ? asin(num / den) : 0.;
//}

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
int isNegative(T t) 
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
I3D_EXPORT double degreesToDecimalDegrees(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a radianes
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return radianes
 */
I3D_EXPORT double degreesToRadians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a grados centesimales
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados centesimales
 */
I3D_EXPORT double degreesToGradians(int degrees, int minutes, int seconds);

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
I3D_EXPORT void decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a radianes
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Radianes
 */
I3D_EXPORT double decimalDegreesToRadians(double decimalDegrees);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados centesimales
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Grados centesimales
 */
I3D_EXPORT double decimalDegreesToGradians(double decimalDegrees);

/*!
 * \brief Conversión de radianes a grados, minutos y segundos
 * \param[in] radians Radianes
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
I3D_EXPORT void radiansToDegrees(double radians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de radianes a grados sexagesimales en notación decimal
 * \param[in] radians Radianes
 * \return Grados sexagesimales en notación decimal
 */
I3D_EXPORT double radiansToDecimalDegrees(double radians);

/*!
 * \brief radiansToGradians
 * \param[in] radians Radianes
 * \return Grados centesimales
 */
I3D_EXPORT double radiansToGradians(double radians);

/*!
 * \brief Conversión de grados centesimales a grados, minutos y segundos
 * \param[in] gradians Grados centesimales
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
I3D_EXPORT void gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados centesimales a grados sexagesimales en notación decimal
 * \param[in] gradians Grados centesimales
 * \return Grados sexagesimales en notación decimal
 */
I3D_EXPORT double gradiansToDecimalDegrees(double gradians);

/*!
 * \brief Conversión de grados centesimales a radianes
 * \param[in] gradians Grados centesimales
 * \return Radianes
 */
I3D_EXPORT double gradiansToRadians(double gradians);

/*! \} */ // end of angleConversion

} // End namespace I3D

#endif // I3D_MATHUTILS_H
