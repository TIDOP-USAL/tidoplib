#ifndef I3D_MATHUTILS_H
#define I3D_MATHUTILS_H

#include <vector>
#include <array>

#include "opencv2/core/core.hpp"

#include "core/defs.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Varianza del laplaciano
 * \param[in] src
 * \return
 */
double I3D_EXPORT laplacianVariance(const cv::Mat &src);

template<typename T>
inline double I3D_EXPORT module(const cv::Point_<T> &v)
{
  return sqrt(v.x * v.x + v.y*v.y);
}

/*!
 * \brief Devuelve el ángulo entre dos vectores
 * \param[in] v1 Vector 1
 * \param[in] v2 Vector 2
 * \return Ángulo en radianes
 */
template<typename T>
inline double I3D_EXPORT vectorAngle(const cv::Point_<T> &v1, const cv::Point_<T> &v2)
{
  if (v1 == cv::Point_<T>() || v2 == cv::Point_<T>())
    return 0.0;
  return acos((v1.x*v2.x + v1.y*v2.y) / (module(v1) * module(v2)));
}

template<typename T>
inline double I3D_EXPORT vectorAngleOX(const cv::Point_<T> &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.y, v.x);
  return angle; 
}

template<typename T>
inline double I3D_EXPORT vectorAngleOY(const cv::Point_<T> &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.x, v.y);
  return angle;
}

template<typename T>
inline double I3D_EXPORT azimut(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2)
{
  double azimut = 0.;
  cv::Point v = pt2 - pt1;
  if (v.x == 0 && v.y == 0) return azimut;
  azimut = atan2(v.x, v.y);
  if (azimut < 0.) azimut += CV_PI * 2.;
  return azimut;
}

double I3D_EXPORT computeMedian(const std::vector<double> &input);

double I3D_EXPORT computeTempMAD(const std::vector<double> &input, const double median);

bool I3D_EXPORT isOutlier(const double temp, const double median, const double mad);

/*!
 * \brief Recta de regresión de Y sobre X
 * La recta de regresión de Y sobre X se utiliza para estimar los valores de la Y a partir de los de la X.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable X.
 * \f$ y = m * x + b\f$<BR>
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return coeficiente de correlacción
 *
 * <h4>Ejemplo</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearYX(pts, &m, &b);
 * \endcode
 */
double I3D_EXPORT regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b);

/*!
 * \brief Recta de regresión de X sobre Y
 * La recta de regresión de X sobre Y se utiliza para estimar los valores de la X a partir de los de la Y.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable Y.
 * \f$ x = m*y + b\f$<BR>
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return coeficiente de correlacción
 *
 * <h4>Ejemplo</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearXY(pts, &m, &b);
 * \endcode
 */
double I3D_EXPORT regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b);

/*!
 * \brief ángulos de Euler
 *
 * A partir de una matriz de rotación obtiene los ángulos de giro.
 * \param[in] R Matriz de rotación
 * \param[out] omega Rotación respecto al eje X
 * \param[out] phi Rotación respecto al eje Y
 * \param[out] kappa Rotación respecto al eje Z
 * https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
 */
void I3D_EXPORT eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa);

/*!
 * \brief Obtiene la ecuación de un plano que pasa por tres puntos
 *
 * \f$ A*x + B*y + C*z + D = 0\f$
 *
 * param[in] points Puntos que definen el plano
 * param[out] plane Parametros de la ecuación general del plano normalizados (A, B, C, D)
 */
template<typename T>
double threePointsPlane(const std::array<T, 3> &points, std::array<double, 4> &plane) 
{
  T v1 = points[1] - points[0];
  T v2 = points[2] - points[0];
  double A = v1.y*v2.z - v1.z*v2.y;
  double B = v1.z*v2.x - v1.x*v2.z;
  double C = v1.x*v2.y - v2.x*v1.y;
  double D = -A *points[0].x - B*points[0].y - C*points[0].z;
  double N = sqrt(A*A + B*B + C*C);
  if ( N ) { 
    plane[0] = A / N;
    plane[1] = B / N;
    plane[2] = C / N;
    plane[3] = D / N;
  }
  return( N );
}

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
double I3D_EXPORT degreesToDecimalDegrees(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a radianes
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return radianes
 */
double I3D_EXPORT degreesToRadians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a grados centesimales
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados centesimales
 */
double I3D_EXPORT degreesToGradians(int degrees, int minutes, int seconds);

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
void I3D_EXPORT decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a radianes
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Radianes
 */
double I3D_EXPORT decimalDegreesToRadians(double decimalDegrees);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados centesimales
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Grados centesimales
 */
double I3D_EXPORT decimalDegreesToGradians(double decimalDegrees);

/*!
 * \brief Conversión de radianes a grados, minutos y segundos
 * \param[in] radians Radianes
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
void I3D_EXPORT radiansToDegrees(double radians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de radianes a grados sexagesimales en notación decimal
 * \param[in] radians Radianes
 * \return Grados sexagesimales en notación decimal
 */
double I3D_EXPORT radiansToDecimalDegrees(double radians);

/*!
 * \brief radiansToGradians
 * \param[in] radians Radianes
 * \return Grados centesimales
 */
double I3D_EXPORT radiansToGradians(double radians);

/*!
 * \brief Conversión de grados centesimales a grados, minutos y segundos
 * \param[in] gradians Grados centesimales
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
void I3D_EXPORT gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados centesimales a grados sexagesimales en notación decimal
 * \param[in] gradians Grados centesimales
 * \return Grados sexagesimales en notación decimal
 */
double I3D_EXPORT gradiansToDecimalDegrees(double gradians);

/*!
 * \brief Conversión de grados centesimales a radianes
 * \param[in] gradians Grados centesimales
 * \return Radianes
 */
double I3D_EXPORT gradiansToRadians(double gradians);

/*! \} */ // end of angleConversion

} // End namespace I3D

#endif // I3D_MATHUTILS_H
