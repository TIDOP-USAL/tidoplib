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

void I3D_EXPORT eulerAngles(const std::array<std::array<double, 3>, 3> &R, double *omega, double *phi, double *kappa);


/* ---------------------------------------------------------------------------------- */
/*                              Operaciones con vectores                              */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Convierte una matriz de OpenCV en un vector
 * \param[in] m Matriz de entrada
 * \param[out] av Vector de salida
 */
template<typename T>
void I3D_EXPORT cvMatToVector(const cv::Mat &m, std::vector<T> *av)
{
  av->resize(m.rows*m.cols);
  if (m.isContinuous()) {
    av->assign((T*)m.datastart, (T*)m.dataend);
  } else {
    for (int i = 0; i < m.rows; ++i) {
      av->insert(av->end(), (T*)m.ptr<uchar>(i), (T*)m.ptr<uchar>(i)+m.cols);
    }
  }
}

/*!
 * \brief Ordena un vector de menor a mayor
 * \param[in] v Vector
 */
template<typename T>
void I3D_EXPORT sortVector(std::vector<T> *v)
{
  std::sort(v->begin(), v->end());
}

/*!
 * \brief Ordena un vector de mayor a menor
 * \param[in] v Vector
 */
template<typename T>
void I3D_EXPORT sortVectorInv(std::vector<T> *v)
{
  std::sort(v->rbegin(),v->rend());
}

/*!
 * \brief Determinar el número de elementos iguales a un número.
 * Sobrecarga del operador == para determinar el número de elementos de un
 * vector que son iguales al valor pasado como parámetro.
 * \param[in] v Vector
 * \param[in] t Valor
 * \return Número de elementos que cumplen la condición
 */
template<typename T>
int operator==(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
  bounds = std::equal_range(v.begin(), v.end(), t);
  int ini = bounds.first - v.begin();
  int end = bounds.second - v.begin();
  return ( end - ini );
}

/*!
 * \brief Determina el número de elementos distintos a un número.
 * Sobrecarga del operador != para determinar el número de elementos de un
 * vector que son distintos al valor pasado como parámetro.
 * \param[in] v Vector
 * \param[in] t Valor
 * \return Número de elementos que cumplen la condición
 */
template<typename T>
int operator!=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
  bounds = std::equal_range(v.begin(), v.end(), t);
  int r1 = bounds.first - v.begin();
  int r2 = v.end() - bounds.second;
  return (r1 + r2 );
}

/*!
 * \brief operator >=
 * \param[in] v
 * \param[in] t
 * \return
 */
template<typename T>
int operator>=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator upOrEq;
  upOrEq = std::lower_bound(v.begin(), v.end(), t);
  return (v.end() - upOrEq);
}

template<typename T>
int operator<=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator lowOrEq;
  lowOrEq = std::upper_bound(v.begin(), v.end(), t);
  return (lowOrEq - v.begin());
}

template<typename T>
int operator> (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator up;
  up = std::upper_bound(v.begin(), v.end(), t);
  return (v.end() - up);
}

template<typename T>
int operator< (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator low;
  low = std::lower_bound(v.begin(), v.end(), t);
  return (low - v.begin());
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por filas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
int I3D_EXPORT sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por columnas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
int I3D_EXPORT sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas 
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T>
std::vector<int> I3D_EXPORT sortIdx(const std::vector<T> &v)
{
  std::vector<int> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
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
