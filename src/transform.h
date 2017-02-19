#ifndef I3D_TRANSFORM_H
#define I3D_TRANSFORM_H

#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <array>
#include <thread>

#include "core/config.h"

#ifdef HAVE_EIGEN
#include <Eigen/SVD>
#endif

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#endif

#ifdef HAVE_GDAL
#include "ogr_spatialref.h"
#include "ogr_p.h"
#include "ogr_api.h"
#endif

#include "core/defs.h"
#include "core/messages.h"
#include "core/exception.h"
#include "geometric_entities/entity.h"
#include "geometric_entities/segment.h"
#include "geometric_entities/bbox.h"

namespace I3D
{

/*! \defgroup trfGroup Transformaciones
 *  Transformaciones geométricas
 *  \{
 */


/*!
 * \brief Cálculo de la matriz de rotación
 * \param[in] omega Rotación respecto al eje X en radianes
 * \param[in] phi Rotación respecto al eje Y en radianes
 * \param[in] kappa Rotación respecto al eje Z en radianes
 * \param[out] R Matriz de rotación
 */
I3D_EXPORT void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R);



/*!
 * \brief Tipos de transformaciones
 */
enum class transform_type {
  TRANSLATE,       /*!< Desplazamiento. */
  ROTATION,        /*!< Giro. */
  HELMERT_2D,      /*!< Helmert 2D */
  AFFINE,          /*!< Afin */
  PERSPECTIVE,     /*!< Perspectiva */
  PROJECTIVE,      /*!< Projectiva */
  HELMERT_3D,      /*!< Helmert 3D */
  POLYNOMIAL,      /*!< Transformación polinómica*/
  // tipos especiales
  MULTIPLE,
  CRS
};

enum class transform_order {
  DIRECT,   /*!< Transformación directa. */
  INVERSE   /*!< Transformación inversa. */
};

enum class transform_status {
  SUCCESS,
  FAILURE
};

/*!
 * \brief Clase base para transformaciones
 */
template<typename Point_t>
class I3D_EXPORT Transform
{

protected:

  /*!
   * \brief Tipo de transformación
   * \see transform_type
   */
  transform_type mTrfType;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   */
  int mMinPoint;

  /*!
   * \brief Dimensiones de la transformación. 2D o 3D
   */
  int mDimensions;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor
   * \param[in] trfType Tipo de transformación
   * \param[in] n_min Número mínimo de puntos necesario para la transformación
   */
  Transform(transform_type trfType, int nMin = 0)
    : mTrfType(trfType), mMinPoint(nMin) {}

  /*!
   * \brief Destructora
   */
  virtual ~Transform() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  virtual double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) = 0;

  /*!
   * \brief Determina si el numero de puntos son suficientes para calcular la transformación
   * \param[in] npoints Número de puntos para calcular la transformación
   * \return Verdadero si son puntos suficientes
   */
  virtual bool isNumberOfPointsValid(int npoints) const { return npoints >= mMinPoint; }

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos aplicando paralelismo
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transformParallel(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  virtual Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   * \return Número mínimo de puntos
   */
  int minNumberOfPoints() { return mMinPoint; }

  /*!
   * \brief Tipo de transformación
   * \return Tipo de transformación
   * \see transform_type
   */
  transform_type getTransformType() { return mTrfType; }

  /*!
   * \brief root-mean-square error (Raiz cuadrada de error cuadratico medio)
   * \param ptsIn Puntos en el sistema de entrada
   * \param ptsOut Puntos en el sistema de salida
   * \param error Vector con los errores para cada punto
   * \return RMSE
   */
  double rootMeanSquareError(const std::vector<Point_t> &ptsIn, const std::vector<Point_t> &ptsOut, std::vector<double> *error = NULL);

protected:

  /*!
   * \brief Formatea de forma adecuada el vector de salida
   *
   * Prepara el vector de puntos de salida redimensionandolo al 
   * tamaño adecuado y limpiando su contenido si ya tenia datos.
   * En caso de que se utilice el mismo vector de entrada y de salida no
   * hace nada.
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   */
  template<typename T2> 
  void formatVectorOut(const std::vector<T2> &ptsIn, std::vector<T2> *ptsOut) const;

  /*!
   * \brief root-mean-square error
   * \param ptsIn Puntos en el sistema de entrada
   * \param ptsOut Puntos en el sistema de salida
   * \param error Vector con los errores para cada punto
   * \return RMSE
   */
  double _rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                              const std::vector<Point_t> &ptsOut, 
                              std::vector<double> *error = NULL);
};


template<typename Point_t> inline
transform_status Transform<Point_t>::transformParallel( const std::vector<Point_t> &ptsIn, 
                                                        std::vector<Point_t> *ptsOut, 
                                                        transform_order trfOrder = transform_order::DIRECT) const
{
  formatVectorOut(ptsIn, ptsOut);
  transform_status r_status;
  parallel_for(0, static_cast<int>(ptsIn.size()), [&](int i) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) return;
  });
  return r_status;
}

template<typename Point_t> inline
double Transform<Point_t>::rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                                               const std::vector<Point_t> &ptsOut, 
                                               std::vector<double> *error = NULL)
{
  size_t n = ptsIn.size();
  std::vector<Point_t> pts_out(n);
  std::vector<double> err(n);
  double sumErr = 0.;

  //... Sería mejor añadirlo en el propio calculo de los parámetros?
  if (compute(ptsIn, ptsOut)) {
    for (size_t i = 0; i < n; i++) {
      transform(ptsIn[i], &pts_out[i]);
      pts_out[i] -= ptsOut[i];
      err[i] = static_cast<double>(pts_out[i].x * pts_out[i].x + pts_out[i].y * pts_out[i].y);
      sumErr += err[i];
    }
    if (error) *error = err;
    return sqrt(sumErr/(mDimensions * (n - mMinPoint)));
  }
  return 0.;
}

template<typename Point_t> template<typename T2> inline
void Transform<Point_t>::formatVectorOut(const std::vector<T2> &ptsIn, std::vector<T2> *ptsOut) const {
  if (&ptsIn != ptsOut && ptsIn.size() != ptsOut->size()) {
    ptsOut->clear();
    ptsOut->resize(ptsIn.size());
  }
}

template<typename Point_t> inline
double Transform<Point_t>::_rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                                                const std::vector<Point_t> &ptsOut, 
                                                std::vector<double> *error = NULL)
{
  size_t n = ptsIn.size();
  std::vector<Point_t> pts_out(n);
  std::vector<double> err(n);
  double sumErr = 0.;

  for (int i = 0; i < n; i++) {
    transform(ptsIn[i], &pts_out[i]);
    pts_out[i] -= ptsOut[i];
    err[i] = static_cast<double>(pts_out[i].x * pts_out[i].x + pts_out[i].y * pts_out[i].y);
    sumErr += err[i];
  }

  if (error) *error = err;

  return sqrt(sumErr/(mDimensions * (n - mMinPoint)));
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación multiple.
 *
 * Una transformación multiple permite agrupar varias transformaciones de 
 * forma que se ejecutan a la vez (consecutivamente).
 */
template<typename Point_t>
class I3D_EXPORT TrfMultiple : public Transform<Point_t>
{

private:

  /*!
   * \brief Lista de transformaciones
   */
  std::list<std::shared_ptr<Transform<Point_t>>> mTransf;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<Point_t> o CV::Point_<Point_t>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructora
   */
  TrfMultiple() : Transform<Point_t>(transform_type::MULTIPLE) {}

  /*!
   * \brief Constructor de lista
   * \param[in] transfList listado de transformaciones
   */
  TrfMultiple(std::initializer_list<std::shared_ptr<Transform<Point_t>>> transfList)
    : Transform<Point_t>(transform_type::MULTIPLE), mTransf(transfList) {}

  /*!
   * \brief Destructora
   */
  ~TrfMultiple() {}

public:

  /*!
   * \brief Añade una nueva transformación a la lista
   * \param[in] trf Transformación que se añade
   */
  void add(std::shared_ptr<Transform<Point_t>> trf) { mTransf.push_back(trf); }

  /*!
   * \brief Borra las transformaciones
   */
  void clear() { mTransf.clear(); }

  bool isNumberOfPointsValid(int npoints) const override;

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename Point_t> inline
bool TrfMultiple<Point_t>::isNumberOfPointsValid(int npoints) const
{ 
  printError("'isNumberOfPointsValid' no esta soportado para TrfMultiple");
  I3D_COMPILER_WARNING("'isNumberOfPointsValid' no esta soportado para TrfMultiple");
  return true;
}

template<typename Point_t> inline
double TrfMultiple<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  printError("'compute' no esta soportado para TrfMultiple");
  I3D_COMPILER_WARNING("'compute' no esta soportado para TrfMultiple");
  return -1.;
}

template<typename Point_t> inline
transform_status TrfMultiple<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  *ptsOut = ptsIn;
  transform_status r_status;
  for (auto trf : mTransf) {
    r_status = trf->transform(*ptsOut, ptsOut, trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status TrfMultiple<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  *ptOut = ptIn;
  transform_status r_status;
  for (auto trf : mTransf) {
    r_status = trf->transform(*ptOut, ptOut, trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t TrfMultiple<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t out = ptIn;
  for (auto trf : mTransf) {
    out = trf->transform(out, trfOrder);
  }
  return out;
}


/* ---------------------------------------------------------------------------------- */


/*! \defgroup trf2DGroup Transformaciones 2D
*  Transformaciones geométricas en el plano
*  \{
*/

/*!
 * \brief Clase base para transformaciones 2D
 */
template<typename Point_t>
class I3D_EXPORT Transform2D : public Transform<Point_t>
{

private:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<Point_t> o CV::Point_<Point_t>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Transform2D
   * \param n_min Número mínimo de puntos necesario para la transformación
   */
  Transform2D(transform_type trfType, int n_min = 0)
    : Transform<Point_t>(trfType, n_min)
  {
    this->mDimensions = 2;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform2D() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  virtual double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override = 0;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  virtual Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override = 0;

};

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

/*!
 * \brief Transformación perspectiva
 */
template<typename Point_t>
class I3D_EXPORT TrfPerspective : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Parámetros de la transformación
   */
  cv::Mat H;

private:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<Point_t> o CV::Point_<Point_t>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor
   * \param[in] trfType Tipo de transformación
   * \param[in] n_min Número mínimo de puntos necesario para la transformación
   */
  TrfPerspective()
    : Transform2D<Point_t>(transform_type::PERSPECTIVE, 1) {}

  /*!
   * \brief Destructora
   */
  ~TrfPerspective() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &in, transform_order trfOrder = transform_order::DIRECT) const override;
};

template<typename Point_t> inline
transform_status TrfPerspective<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  try {
    if (trfOrder == transform_order::DIRECT)
      cv::perspectiveTransform(ptsIn, *ptsOut, H);
    else {
      cv::perspectiveTransform(ptsIn, *ptsOut, H.inv());
    }
  } catch ( cv::Exception &e ) {
    printError("Error en transformación perspectiva: %s",e.what());
    return transform_status::FAILURE; 
  }
  return transform_status::SUCCESS; 
}

template<typename Point_t> inline
transform_status TrfPerspective<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  std::vector<Point_t> vIn, vOut;
  vIn.push_back(ptIn);
  try {
    if (trfOrder == transform_order::DIRECT) {
      cv::perspectiveTransform(vIn, vOut, H);
    } else {
      cv::perspectiveTransform(vIn, vOut, H.inv());
    }
    *ptOut = vOut[0];
  } catch ( cv::Exception &e ) {
    printError("Error en transformación perspectiva: %s", e.what());
    return transform_status::FAILURE; 
  }
  return transform_status::SUCCESS; 
}

template<typename Point_t> inline
Point_t TrfPerspective<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t out = ptIn;
  transform(ptIn, &out, trfOrder);
  return out;
}

template<typename Point_t> inline
double TrfPerspective<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  if (error) error = NULL; // Habria que poder calcular el error
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printError("Número de puntos distinto en cada conjunto de puntos"); 
  } else {
    // Controlar estos errores a nivel general
    if (this->isNumberOfPointsValid(n1)) {

      H = cv::findHomography(pts1, pts2, cv::RANSAC);
      //cv::Mat H0 = cv::findHomography(pts1, pts2, cv::RANSAC);
      //cv::Mat H1 = cv::findHomography(pts1, pts2, cv::LMEDS);
      //cv::Mat H2 = cv::findHomography(pts1, pts2);
      //... determinar error
    } else
      printError("Número de puntos no valido. Se necesitan %i puntos para poder calcular los parámetros de la transformación", this->mMinPoint);
  }
  return H.empty() ? -1. : 1.;
}

#endif // HAVE_OPENCV

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Traslación
 *
 * Transformación que aplica una traslación en el plano a un conjunto de puntos
 */
template<typename Point_t>
class I3D_EXPORT Translate : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Traslación en X
   */
  double tx;

  /*!
   * \brief Traslación en Y
   */
  double ty;

private:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Translate()
    : Transform2D<Point_t>(transform_type::TRANSLATE, 1), tx(0.), ty(0.) {}

  /*!
   * \brief Constructora
   * \param[in] tx Traslación en el eje X
   * \param[in] ty Traslación en el eje Y
   */
  Translate(double tx, double ty)
    : Transform2D<Point_t>(transform_type::TRANSLATE, 1), tx(tx), ty(ty) { }

  /*!
   * \brief Calculo de la traslación
   *
   * Calcula la traslación entre dos sistemas diferentes a partir
   * de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una traslación
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  ///*!
  // * \brief Transforma un conjunto de segmentos en otro aplicando una traslación
  // * \param[in] in Puntos de entrada
  // * \param[out] out Puntos de salida
  // * \param[in] bDirect Transformación directa
  // */
  //void transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out, transform_order trfOrder = transform_order::DIRECT) const;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename Point_t> inline
double Translate<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double ret = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  //if (n1 != n2) I3D_THROW_ERROR("Número de puntos distinto en cada conjunto de puntos");
  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 4;
      double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat A(m, n, CV_64F, a);
        cv::Mat B(m, 1, CV_64F, b);
        cv::solve(A, B, C, cv::DECOMP_SVD);
        tx = static_cast<sub_type>(C.at<double>(2));
        ty = static_cast<sub_type>(C.at<double>(3));

        ret = this->_rootMeanSquareError(pts1, pts2, error);

      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] a;
      delete[] b;
    }
  }
  return ret;
}

template<typename Point_t> inline
transform_status Translate<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  this->formatVectorOut(ptsIn, ptsOut);
  transform_status r_status;
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

//template<typename Point_t> inline
//void Translate<Point_t>::transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out, transform_order trfOrder) const
//{
//  formatVectorOut(in, out);
//  for (int i = 0; i < in.size(); i++) {
//    transform(in[i].pt1, &(*out)[i].pt1, trfOrder);
//    transform(in[i].pt2, &(*out)[i].pt2, trfOrder);
//  }
//}

template<typename Point_t> inline
transform_status Translate<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  try {
    Point_t pt_aux;
    if (typeid(Point_t) == typeid(int)) {
      pt_aux.x = I3D_ROUND_TO_INT(tx);
      pt_aux.y = I3D_ROUND_TO_INT(ty);
    } else {
      pt_aux.x = static_cast<sub_type>(tx);
      pt_aux.y = static_cast<sub_type>(ty);
    }
    *ptOut = (trfOrder == transform_order::DIRECT) ? ptIn + pt_aux : ptIn - pt_aux;
  } catch ( std::exception &e ) {
    printError("Error en traslación: %s", e.what());
    r_status = transform_status::FAILURE; 
  }
  return r_status;
}

template<typename Point_t> inline
Point_t Translate<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t pt_aux;
  if (typeid(Point_t) == typeid(int)) {
    pt_aux.x = I3D_ROUND_TO_INT(tx);
    pt_aux.y = I3D_ROUND_TO_INT(ty);
  } else {
    pt_aux.x = static_cast<sub_type>(tx);
    pt_aux.y = static_cast<sub_type>(ty);
  }
  return (trfOrder == transform_order::DIRECT) ? ptIn + pt_aux : ptIn - pt_aux;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Rotación
 *
 * Transformación que aplica una rotación en el plano a un conjunto de puntos 
 */
template<typename Point_t> 
class I3D_EXPORT Rotation : public Transform2D<Point_t>
{
private:

  /*!
   * \brief Ángulo de rotación
   */
  double angle;

  /*!
   * \brief r1 = cos(angle);
   */
  double r1;

  /*!
   * \brief r2 = sin(angle);
   */
  double r2;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o cv::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Rotation() 
    : Transform2D<Point_t>(transform_type::ROTATION, 1), angle(0.)
  {
    update();
  }

  /*!
   * \brief Constructora
   * \param[in] angle Ángulo en radianes
   */
  Rotation(double angle) 
    : Transform2D<Point_t>(transform_type::ROTATION, 1), angle(angle)
  {
    update();
  }
  
  /*!
   * \brief Calculo del ángulo de rotación
   *
   * Calcula la rotación entre dos sistemas de coordenadas a partir de dos 
   * conjuntos de puntos
   * 
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot;
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out{ cv::Point2d(-7.869, 35.578), cv::Point2d(-9.33, 34.71), cv::Point2d(0.499, 64.43) };
   * rot.compute(pts_in, pts_out);
   * \endcode
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Devuelve el ángulo de la rotación
   * \return Ángulo en radianes
   */
  double getAngle() const { return angle; }

  /*!
   * \brief Establece en ángulo de la rotación
   * \param[in] ang Ángulo en radianes
   */
  void setAngle(double ang);

  /*!
   * \brief Aplica una rotación a un conjunto de puntos
   *
   * Transforma un conjunto de puntos en un sistema cartesiano a otro 
   * sistema aplicando un giro.
   *
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot(0.562);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out;
   * rot.transform(pts_in, &pts_out);
   * \endcode
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

private:

  /*!
   * \brief Actualiza los parámetros r1 y r2
   */
  void update();
};

template<typename Point_t> inline
double Rotation<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 2;
      double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
      cv::Mat C(2, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = -pts1[i].y;
          *pb++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = pts1[i].x;
          *pb++ = pts2[i].y;
        }
        
        cv::Mat A(m, n, CV_64F, a);
        cv::Mat B(m, 1, CV_64F, b);
        cv::solve(A, B, C, cv::DECOMP_SVD);
        r1 = static_cast<sub_type>(C.at<double>(0));
        r2 = static_cast<sub_type>(C.at<double>(1));
        angle = acos(r1);

        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] a;
      delete[] b;
    }
  }
  return rmse;
}

template<typename Point_t> inline
void Rotation<Point_t>::setAngle(double ang)
{ 
  angle = ang; 
  update(); 
}

template<typename Point_t> inline
transform_status Rotation<Point_t>::transform(const std::vector<Point_t> &in, std::vector<Point_t> *out, transform_order trfOrder) const
{
  this->formatVectorOut(in, out);
  transform_status r_status;
  for (int i = 0; i < in.size(); i++) {
    r_status = transform(in[i], &(*out)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status Rotation<Point_t>::transform(const Point_t &ptsIn, Point_t *ptsOut, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  sub_type x_aux = ptsIn.x;
  try {
    if (trfOrder == transform_order::DIRECT) {
      ptsOut->x = static_cast<sub_type>(x_aux*r1 - ptsIn.y*r2);
      ptsOut->y = static_cast<sub_type>(x_aux*r2 + ptsIn.y*r1);
    } else {
      ptsOut->x = static_cast<sub_type>(x_aux*r1 + ptsIn.y*r2);
      ptsOut->y = static_cast<sub_type>(ptsIn.y*r1 - x_aux*r2);
    }
  } catch (std::exception &e ) {
    printError("Error al aplicar la rotación: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Rotation<Point_t>::transform(const Point_t &ptsIn, transform_order trfOrder) const
{
  Point_t out;
  if (trfOrder == transform_order::DIRECT) {
    out.x = ptsIn.x*r1 - ptsIn.y*r2;
    out.y = ptsIn.x*r2 + ptsIn.y*r1;
  } else {
    out.x = ptsIn.x*r1 + ptsIn.y*r2;
    out.y = ptsIn.y*r1 - ptsIn.x*r2;
  }
  return out;
}

template<typename Point_t> inline
void Rotation<Point_t>::update()
{
  r1 = cos(angle);
  r2 = sin(angle);
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Tranformación Helmert 2D o transformación de semejanza
 *
 * Esta transformación expresa la relación que existe (o transformación que es 
 * preciso realizar)  entre dos sistemas cartesianos que discrepan en la situación 
 * del origen, en la orientación de los ejes y en la unidad de medida a lo largo 
 * de los mismos pero de manera que dicha variación en unidad de medida es constante 
 * a lo largo de cada eje y entre los dos ejes  
 *
 * \f$ a = scale * cos(rotation)\f$<BR>
 * \f$ b = scale * sin(rotation)\f$<BR>
 *
 * \f$ x' = a * x + b * y + X0\f$<BR>
 * \f$ y' = a * x - b * x + Y0\f$
 */
template<typename Point_t>
class I3D_EXPORT Helmert2D : public Transform2D<Point_t>
{
private:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Traslación en x
   */
  double tx;

  /*!
   * \brief Traslación en y
   */
  double ty;

private:

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * \brief Ángulo de rotation
   */
  double mRotation;

  /*!
   * \brief a = scale * cos(rotation);
   */
  double a;

  /*!
   * \brief b = scale * sin(rotation);
   */
  double b;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert2D()
    : Transform2D<Point_t>(transform_type::HELMERT_2D, 2), tx(0), ty(0), mScale(1.), mRotation(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  Helmert2D(double tx, double ty, double scale, double rotation)
    : Transform2D<Point_t>(transform_type::HELMERT_2D, 2), tx(tx), ty(ty), mScale(scale), mRotation(rotation)
  {
    update();
  }

  //~Helmert2D();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   *  
   * <H3>Ejemplo:</H3>
   * \code
   * Helmert2D<cv::Point2d> h2d(x0,y0, scale, rotation);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(4157222.543, 664789.307),
   *     cv::Point2d(4149043.336, 688836.443), cv::Point2d(4172803.511, 690340.078),
   *     cv::Point2d(4177148.376, 642997.635), cv::Point2d(4137012.190, 671808.029), 
   *     cv::Point2d(4146292.729, 666952.887), cv::Point2d(4138759.902, 702670.738) };
   * std::vector<cv::Point2d> pts_out;
   * h2d.transform(pts_in, &pts_out);
   * \endcode
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica un helmert 2D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Devuelve el giro
   * \return[in] Ángulo de rotación en radianes
   */
  double getRotation() const { return mRotation; };

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double getScale() const { return mScale;  };

  /*!
   * \brief Establece los parámetros
   * \param[in] tx Traslación en x
   * \param[in] ty Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  void setParameters(double tx, double ty, double scale, double rotation);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] rotation Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] scale Escala de la transformación
   */
  void setScale(double scale);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename Point_t> inline
double Helmert2D<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 4;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = -pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        tx = C.at<double>(2);
        ty = C.at<double>(3);

        mRotation = atan2(b, a);
        mScale = sqrt(a*a + b*b);

        rmse = this->_rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return rmse;
}

template<typename Point_t> inline
transform_status Helmert2D<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  transform_status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status Helmert2D<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  sub_type x_aux = ptIn.x;
  try {
    if (trfOrder == transform_order::DIRECT){
      ptOut->x = static_cast<sub_type>(a * x_aux - b * ptIn.y + tx);
      ptOut->y = static_cast<sub_type>(b * x_aux + a * ptIn.y + ty);
    } else {
      double det = a*a + b*b;
      if ( det == 0. ) throw I3D_ERROR("División por cero");
      ptOut->x = static_cast<sub_type>((a*(x_aux - tx) + b*(ptIn.y - ty)) / det);
      ptOut->y = static_cast<sub_type>((-b*(x_aux - tx) + a*(ptIn.y - ty)) / det);
    }
  } catch (I3D::Exception &e ) {
    printError("Error al aplicar la transformación Helmert 2D: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t Helmert2D<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>(a * ptIn.x - b * ptIn.y + tx);
    r_pt.y = static_cast<sub_type>(b * ptIn.x + a * ptIn.y + ty);
  } else {
    double det = a*a + b*b;
    if ( det == 0. ) throw I3D_ERROR("División por cero");
    r_pt.x = static_cast<sub_type>((a*(ptIn.x - tx) + b*(ptIn.y - ty)) / det);
    r_pt.y = static_cast<sub_type>((-b*(ptIn.x - tx) + a*(ptIn.y - ty)) / det);
  }
  return r_pt;
}

template<typename Point_t> inline
void Helmert2D<Point_t>::setParameters(double tx, double ty, double scale, double rotation)
{
  this->tx = tx;
  this->ty = ty;
  mScale = scale;
  mRotation = rotation;
  update();
}

template<typename Point_t> inline
void Helmert2D<Point_t>::setRotation(double rotation) 
{
  mRotation = rotation;
  update();
}

template<typename Point_t> inline
void Helmert2D<Point_t>::setScale(double scale)
{
  mScale = scale;
  update();
}

template<typename Point_t> inline
void Helmert2D<Point_t>::update()
{
  a = mScale * cos(mRotation);
  b = mScale * sin(mRotation);
}


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación Affine
 *
 * La Transformación Afín expresa la relación que existe (o la transformación que es 
 * preciso realizar) entre dos sistemas cartesianos que discrepan en la situación del 
 * origen, en la orientación de los ejes y en la unidad de medida a lo largo de los 
 * mismos de manera que dicha variación en unidad de medida es constante a lo largo 
 * de cada eje pero no entre los dos ejes.
 *
 * \f[ a =  scaleX * cos(rotation)\f]
 * \f[ b = -scaleY * sin(rotation)\f]
 * \f[ c =  scaleX * sin(rotation)\f]
 * \f[ d =  scaleY * cos(rotation)\f]
 *
 * \f[ x' = a * x + b * y + x0\f]
 * \f[ y' = c * x + d * y + y0\f]
 */
template<typename Point_t>
class I3D_EXPORT Affine : public Transform2D<Point_t>
{
public:

  /*!
   * \brief Traslación en x
   */
  double tx;

  /*!
   * \brief Traslación en y
   */
  double ty;

private:

  /*!
   * \brief Escala en el eje X
   */
  double mScaleX;

  /*!
   * \brief Escala en el eje Y
   */
  double mScaleY;
  /*!
   * \brief Ángulo de rotation
   */
  double mRotation;

  /*!
   * \brief a = scaleX * cos(rotation)
   */
  double a;

  /*!
   * \brief b = -scaleY * sin(rotation)
   */
  double b;

  /*!
   * \brief c = scaleX * sin(rotation)
   */
  double c;

  /*!
   * \brief d = scaleY * cos(rotation)
   */
  double d;

  double ai;

  double bi;

  double ci;

  double di;

  /*!
   * \brief Traslación en x transformación inversa
   */
  double txi;

  /*!
   * \brief Traslación en y transformación inversa
   */
  double tyi;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  Affine()
    : Transform2D<Point_t>(transform_type::AFFINE, 3), tx(0.), ty(0.), mScaleX(1.), mScaleY(1.), mRotation(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en el eje X
   * \param[in] ty Traslación en el eje Y
   * \param[in] scaleX Escala en el eje X
   * \param[in] scaleY Escala en el eje Y
   * \param[in] rotation Rotación
   */
  Affine(double tx, double ty, double scaleX, double scaleY, double rotation)
    : Transform2D<Point_t>(transform_type::AFFINE, 3), tx(tx), ty(ty), mScaleX(scaleX), mScaleY(scaleY), mRotation(rotation)
  {
    update();
  }

  //~Affine();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación afín
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una transformación afín a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una transformación afín a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Devuelve los coeficientes de la transformación
   * \param[out] a Coeficiente a
   * \param[out] b Coeficiente b
   * \param[out] c Coeficiente c
   * \param[out] d Coeficiente d
   */
  void getParameters(double *a, double *b, double *c, double *d);

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  double getRotation() const { return mRotation; };

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   */
  double getScaleX() const { return mScaleX; };

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   */
  double getScaleY() const { return mScaleY; };

  /*!
   * \brief Establece los parámetros
   * \param[in] tx Traslación en x
   * \param[in] ty Traslación en y
   * \param[in] scaleX Escala en x
   * \param[in] scaleY Escala en y
   * \param[in] rotation Rotación
   */
  void setParameters(double tx, double ty, double scaleX, double scaleY, double rotation);

  /*!
   * \brief Establece los parámetros
   * \param[in] a Coeficiente a
   * \param[in] b Coeficiente b
   * \param[in] c Coeficiente c
   * \param[in] d Coeficiente d
   * \param[in] tx Traslación en x
   * \param[in] ty Traslación en y
   */
  void setParameters(double a, double b, double c, double d, double x0, double y0);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] rotation Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación en el eje X
   * \param[in] scaleX Escala de la transformación
   */
  void setScaleX(double scaleX);

  /*!
   * \brief Establece la escala de la transformación en el eje Y
   * \param[in] scaleY Escala de la transformación
   */
  void setScaleY(double scaleY);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

  /*!
   * \brief Actualiza los parámetros de la transformacion inversa
   */
  void updateInv();
};


template<typename Point_t> inline
double Affine<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 6;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        c = C.at<double>(2);
        d = C.at<double>(3);
        tx = C.at<double>(4);
        ty = C.at<double>(5);
        
        updateInv();
        //double f = atan2( c, a );
        //double w = atan2( -b, d );

        mRotation = (atan2(c, a) + atan2(-b, d) ) / 2.; //... Revisar lo de un unico giro
        mScaleX = sqrt(a*a + c*c);
        mScaleY = sqrt(b*b + d*d);

        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return rmse;
}

template<typename Point_t> inline
transform_status Affine<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  transform_status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status Affine<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  sub_type x_aux = ptIn.x;
  try {
    if (trfOrder == transform_order::DIRECT){
      ptOut->x = static_cast<sub_type>(a * x_aux + b * ptIn.y + tx);
      ptOut->y = static_cast<sub_type>(c * x_aux + d * ptIn.y + ty);
    } else {
      ptOut->x = static_cast<sub_type>(ai * x_aux + bi * ptIn.y + txi);
      ptOut->y = static_cast<sub_type>(ci * x_aux + di * ptIn.y + tyi);
    }
  } catch (std::exception &e ) {
    printError("Error al aplicar la transformación afín: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Affine<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>(a * ptIn.x + b * ptIn.y + tx);
    r_pt.y = static_cast<sub_type>(c * ptIn.x + d * ptIn.y + ty);
  }
  else {
    r_pt.x = static_cast<sub_type>(ai * ptIn.x + bi * ptIn.y + txi);
    r_pt.y = static_cast<sub_type>(ci * ptIn.x + di * ptIn.y + tyi);
  }
  return r_pt;
}

template<typename Point_t> inline
void Affine<Point_t>::getParameters(double *_a, double *_b, double *_c, double *_d)
{
  *_a = a;
  *_b = b;
  *_c = c;
  *_d = d;
}


template<typename Point_t> inline
void Affine<Point_t>::setParameters(double tx, double ty, double scaleX, double scaleY, double rotation)
{
  this->tx = tx;
  this->ty = ty;
  mScaleX = scaleX;
  mScaleY = scaleY;
  mRotation = rotation;
  update();
}

template<typename Point_t> inline
void Affine<Point_t>::setParameters(double a, double b, double c, double d, double tx, double ty)
{
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
  this->tx = tx;
  this->ty = ty;
  mRotation = (atan2(b, a) + atan2(-c, d) ) / 2.;
  mScaleX = sqrt(a*a + b*b);
  mScaleY = sqrt(c*c + d*d);
  updateInv();
}

template<typename Point_t> inline
void Affine<Point_t>::setRotation(double rotation)
{
  mRotation = rotation;
  update();
}

template<typename Point_t> inline
void Affine<Point_t>::setScaleX(double scaleX)
{
  mScaleX = scaleX;
  update();
}

template<typename Point_t> inline
void Affine<Point_t>::setScaleY(double scaleY)
{
  mScaleY = scaleY;
  update();
}

template<typename Point_t> inline
void Affine<Point_t>::update()
{
  a =  mScaleX * cos(mRotation);
  b = -mScaleY * sin(mRotation);
  c =  mScaleX * sin(mRotation);
  d =  mScaleY * cos(mRotation);
  
  updateInv();
}

template<typename Point_t> inline
void Affine<Point_t>::updateInv()
{
  // Transformación inversa
  double det = a * d - c * b;
  if (!det) {
    printError("Determinante nulo");
  } else {
    ai = d / det;
    bi = -b / det;
    ci = -c / det;
    di = a / det;
    I3D_DISABLE_WARNING(4244)
    this->txi = (-d * tx + b * ty) / det;
    this->tyi = (-a * ty + c * tx) / det;
    I3D_ENABLE_WARNING(4244)
  }
}
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación Projectiva
 *
 * La Transformación Projectiva expresa la relación que existe entre dos planos.
 *
 * <BLOCKQUOTE>
 *    x' = ( a * x + b * y + c ) / ( g * x + h * y + 1 ) <BR>
 *    y' = ( d * x + e * y + f ) / ( g * x + h * y + 1 )
 * </BLOCKQUOTE>
 */
template<typename Point_t>
class I3D_EXPORT Projective : public Transform2D<Point_t>
{
private:

  /*!
   * \brief Parámetro a
   */
  double a;

  /*!
   * \brief Parámetro b
   */
  double b;

  /*!
   * \brief Parámetro c
   */
  double c;

  /*!
   * \brief Parámetro d
   */
  double d;

  /*!
   * \brief Parámetro e
   */
  double e;

  /*!
   * \brief Parámetro f
   */
  double f;

  /*!
   * \brief Parámetro g
   */
  double g;
  
  /*!
   * \brief Parámetro h
   */
  double h;

  /*!
   * \brief Parámetro 'a' transformación inversa
   */
  double ai;

  /*!
   * \brief Parámetro 'b' transformación inversa
   */
  double bi;

  /*!
   * \brief Parámetro 'c' transformación inversa
   */
  double ci;

  /*!
   * \brief Parámetro 'd' transformación inversa
   */
  double di;

  /*!
   * \brief Parámetro 'e' transformación inversa
   */
  double ei;

  /*!
   * \brief Parámetro 'f' transformación inversa
   */
  double fi;

  /*!
   * \brief Parámetro 'g' transformación inversa
   */
  double gi;
  
  /*!
   * \brief Parámetro 'h' transformación inversa
   */
  double hi;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o cv::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  Projective()
    : Transform2D<Point_t>(transform_type::PROJECTIVE, 4), a(1), b(0), c(0), d(0), e(1), f(0), g(0), h(0)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] a
   * \param[in] b
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  Projective(double a, double b, double c, double d, double e, double f, double g, double h)
    : Transform2D<Point_t>(transform_type::PROJECTIVE, 4), a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h)
  {
    update();
  }

  //~Projective();

  /*!
   * \brief Calcula la transformación proyectiva entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una 
   * transformación proyectiva
   *
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una transformación proyectiva a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una transformación proyectiva a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] a 
   * \param[in] b 
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  void setParameters(double a, double b, double c, double d, double e, double f, double g, double h);

  void getParameters(double *a, double *b, double *c, double *d, double *e, double *f, double *g, double *h);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};


template<typename Point_t> inline
double Projective<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 8;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = -pts1[i].x * pts2[i].x;
          *pa++ = -pts2[i].x * pts1[i].y;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = -pts2[i].y * pts1[i].x;
          *pa++ = -pts2[i].y * pts1[i].y;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        c = C.at<double>(2);
        d = C.at<double>(3);
        e = C.at<double>(4);
        f = C.at<double>(5);
        g = C.at<double>(6);
        h = C.at<double>(7);

        rmse = _rootMeanSquareError(pts1, pts2, error);

      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return rmse;
}

template<typename Point_t> inline
transform_status Projective<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  transform_status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status Projective<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  Point_t pt_aux = ptIn;
  try {
    if ( trfOrder == transform_order::DIRECT ) {
      ptOut->x = static_cast<sub_type>((a * ptIn.x + b * ptIn.y + c) / (g * ptIn.x + h * ptIn.y + 1));
      ptOut->y = static_cast<sub_type>((d * ptIn.x + e * ptIn.y + f) / (g * ptIn.x + h * ptIn.y + 1));
    } else {
      ptOut->x = static_cast<sub_type>((ai * ptIn.x + bi * ptIn.y + ci) / (g * ptIn.x + h * ptIn.y + 1));
      ptOut->y = static_cast<sub_type>((di * ptIn.x + ei * ptIn.y + fi) / (g * ptIn.x + h * ptIn.y + 1));
    }
  } catch (std::exception &e ) {
    printError("Error al aplicar la transformación proyectiva: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Projective<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>((a * ptIn.x + b * ptIn.y + c) / (g * ptIn.x + h * ptIn.y + 1));
    r_pt.y = static_cast<sub_type>((d * ptIn.x + e * ptIn.y + f) / (g * ptIn.x + h * ptIn.y + 1));
  } else {
    r_pt.x = static_cast<sub_type>((ai * ptIn.x + bi * ptIn.y + ci) / (g * ptIn.x + h * ptIn.y + 1));
    r_pt.y = static_cast<sub_type>((di * ptIn.x + ei * ptIn.y + fi) / (g * ptIn.x + h * ptIn.y + 1));
  }
  return r_pt;
}

template<typename Point_t> inline
void Projective<Point_t>::setParameters(double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h)
{
  a = _a;
  b = _b;
  c = _c;
  d = _d;
  e = _e;
  f = _f;
  g = _g;
  h = _h;
  update();
}

template<typename Point_t> inline
void Projective<Point_t>::getParameters(double *_a, double *_b, double *_c, double *_d, double *_e, double *_f, double *_g, double *_h)
{
  *_a = a;
  *_b = b;
  *_c = c;
  *_d = d;
  *_e = e;
  *_f = f;
  *_g = g;
  *_h = h;
}

template<typename Point_t> inline
void Projective<Point_t>::update()
{
  // Transformación inversa
  double aux = a * e - b * d;
  if (!aux) {
    printError("División por cero");
  } else {
    ai = (e - f * h) / aux;
    bi = (c * h - b) / aux;
    ci = (b * f - c * e) / aux;
    di = (f * g - d) / aux;
    ei = (a - c * g) / aux;
    fi = (c * d - a * f) / aux;
    gi = (d * h - e * g) / aux;
    hi = (b * g - a * h) / aux;
  }

}




/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Tranformación polinómica
 *
 */
template<typename Point_t>
class I3D_EXPORT polynomialTransform : public Transform2D<Point_t>
{
private:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  polynomialTransform()
    : Transform2D<Point_t>(transform_type::POLYNOMIAL, 2)
  {
  }

  //~Helmert2D();

  /*!
   * \brief Calcula la transformación polinómica entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación polinómica
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica una transformación polinómica a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &in, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica una transformación polinómica a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename Point_t> inline
double polynomialTransform<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)) {
      double xmin = I3D_DOUBLE_MAX;
      double ymin = I3D_DOUBLE_MAX;
      double xmax = I3D_DOUBLE_MIN;
      double ymax = I3D_DOUBLE_MIN;
      for( int ipt = 0; ipt < n1; ipt++ ) {
        Point_t &ptoi = pts1[ipt];
        if ( ptoi.x > xmax ) xmax = ptoi.x;
        if ( ptoi.x < xmin ) xmin = ptoi.x;
        if ( ptoi.y > ymax ) ymax = ptoi.y;
        if ( ptoi.y < ymin ) ymin = ptoi.y;
      }
      double xc = (xmax + xmin) / 2;
      double yc = (ymax + ymin) / 2;

      //...
    

    }
  }
  return rmse;
}

template<typename Point_t> inline
transform_status polynomialTransform<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  transform_status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status polynomialTransform<Point_t>::transform(const Point_t &ptIn, Point_t *out, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  sub_type x_aux = ptIn.x;
  try {
    //...
  } catch (std::exception &e ) {
    printError("Error al aplicar la rotación: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t polynomialTransform<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt;
  
  //...
  
  return r_pt;
}

/*! \} */ // end of trf2DGroup

/* ---------------------------------------------------------------------------------- */

/*! \defgroup trf3DGroup Transformaciones 3D
 *  Transformaciones geométricas en el espacio
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

template<typename Point_t>
class I3D_EXPORT Transform3D : public Transform<Point_t>
{
private:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o cv::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Transform
   */
  Transform3D(transform_type trfType, int n_min = 0)
    : Transform<Point_t>(trfType, n_min)
  {
    this->mDimensions = 3;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform3D() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  virtual double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override = 0;

  /*!
   * \brief Aplica la transformación
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  virtual transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  virtual Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  ///*!
  // * \brief Aplica la transformación a una entidad geométrica
  // * \param[in] in Entidad de entrada
  // * \param[out] out Entidad de salida
  // * \param[in] bDirect Transformación directa (por defecto)
  // */
  //void transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const override;

};

//template<typename Point_t> inline
//void Transform3D<Point_t>::transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const
//{
//  if (in.getType() == entity_type::BBOX) {
//    Bbox<sub_type> *bbox = dynamic_cast<const Bbox<sub_type> *>(out);
//    this->transform(dynamic_cast<const Bbox<sub_type> &>(in).pt1, &bbox->pt1, trfOrder);
//    this->transform(dynamic_cast<const Bbox<sub_type> &>(in).pt2, &bbox->pt2, trfOrder);
//  } else if(in.getType() == entity_type::SEGMENT_3D) {
//    Segment3D<sub_type> *segment3d = dynamic_cast<Segment3D<sub_type> *>(out);
//    this->transform(dynamic_cast<const Segment3D<sub_type> &>(in).pt1, &segment3d->pt1, trfOrder);
//    this->transform(dynamic_cast<const Segment3D<sub_type> &>(in).pt2, &segment3d->pt2, trfOrder);
//  } else if (in.getType() == entity_type::LINESTRING_3D ||
//             in.getType() == entity_type::MULTIPOINT_POINT_3D ||
//             in.getType() == entity_type::POLYGON_3D) {
//    const EntityPoints<sub_type> &_in = dynamic_cast<const EntityPoints<sub_type> &>(in);
//    dynamic_cast<EntityPoints<sub_type> *>(out)->resize(_in.getSize());
//    typename std::vector<cv::Point3_<sub_type>>::iterator it_out = dynamic_cast<EntityPoints<sub_type> *>(out)->begin();
//    for (typename std::vector<cv::Point3_<sub_type>>::const_iterator it = _in.begin();
//      it != _in.end(); it++, it_out++) {
//      this->transform(*it, &(*it_out), trfOrder);
//    }
//  } else {
//    //tipo no soportado
//    return;
//  }
//}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Helmert 3D
 *
 * El Helmert 3D o Transformación de Semejanza en el espacio expresa la relación 
 * que existe (o la transformación que es preciso realizar) entre dos sistemas 
 * cartesianos que difieren en la situación del origen, en la orientación de los 
 * ejes y en la unidad de medida a lo largo de los mismos (eslala uniforme a lo
 * largo de los tres ejes)  
 *
 * <BLOCKQUOTE>

 * </BLOCKQUOTE>
 */
template<typename Point_t>
class I3D_EXPORT Helmert3D : public Transform3D<Point_t>
{
public:

  /*!
   * \brief Traslación en x
   */
  double tx;

  /*!
   * \brief Traslación en y
   */
  double ty;

  /*!
   * \brief Traslación en z
   */
  double tz;

private:

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mOmega;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mPhi;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mKappa;

  /*!
   * \brief Matriz de rotación
   */
  std::array<std::array<double, 3>, 3> mR;

  /*!
   * \brief Matriz de rotación inversa
   */
  std::array<std::array<double, 3>, 3> mRinv;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<Point_t> o CV::Point_<Point_t>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert3D()
    : Transform3D<Point_t>(transform_type::HELMERT_3D, 3), tx(0.), ty(0.), tz(0.), mScale(1.), mOmega(0.), mPhi(0.), mKappa(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  Helmert3D(double tx, double ty, double tz, double scale, double omega, double phi, double kappa) 
    : Transform3D<Point_t>(transform_type::HELMERT_3D, 3), tx(tx), ty(ty), tz(tz), mScale(scale), mOmega(omega), mPhi(phi), mKappa(kappa)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] rotation Matriz de rotación
   */
  Helmert3D(double tx, double ty, double tz, double scale, const std::array<std::array<double, 3>, 3> &rotation) 
    : Transform3D<Point_t>(transform_type::HELMERT_3D, 3), tx(tx), ty(ty), tz(tz), mScale(scale), mR(rotation)
  {
    eulerAngles(mR, &mOmega, &mPhi, &mKappa);
    update();
  }

  //~Helmert3D();

  /*!
   * \brief Calcula la transformación Helmert 3D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Devuelve una referencia a la matriz de rotación
   */
  const std::array<std::array<double, 3>, 3> &getRotationMatrix() const;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 3D
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 3D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return transform_status
   * \see transform_order, transform_status
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 3D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double getScale() const { return mScale; };

  /*!
   * \brief Establece los parámetros
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  void setParameters(double tx, double ty, double tz, double scale, double omega, double phi, double kappa);

  /*!
   * \brief Establece la rotación de la transformación
   * \param Ángulo de rotación en radianes
   */
  //void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] Escala de la transformación
   */
  void setScale(double scale);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename Point_t> inline
double Helmert3D<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)) {
      int m = n1 * this->mDimensions, n = 7;
      double *A = new double[m*n], *pa = A, *L = new double[m], *pl = L;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = -pts1[i].z;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pa++ = 0;
          *pl++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = pts1[i].z;
          *pa++ = 0;
          *pa++ = -pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pl++ = pts2[i].y;
          *pa++ = pts1[i].z;
          *pa++ = -pts1[i].y;
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 1;
          *pl++ = pts2[i].z;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, L);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        mScale = C.at<double>(0);
        mOmega = C.at<double>(1);
        mPhi = C.at<double>(2);
        mKappa = C.at<double>(3);
        tx = C.at<double>(4);
        ty = C.at<double>(5);
        tz = C.at<double>(6);

        update();
        
        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] L;
    }
  }
  return rmse;
}

template<typename Point_t> inline
const std::array<std::array<double, 3>, 3> &Helmert3D<Point_t>::getRotationMatrix() const 
{
  return mR;
}

template<typename Point_t> inline
transform_status Helmert3D<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  transform_status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status Helmert3D<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  Point_t ptAux = ptIn;
  try {
    if (trfOrder == transform_order::DIRECT){
      ptOut->x = static_cast<sub_type>( mScale * (ptAux.x*mR[0][0] + ptAux.y*mR[0][1] + ptAux.z*mR[0][2]) + tx );
      ptOut->y = static_cast<sub_type>( mScale * (ptAux.x*mR[1][0] + ptAux.y*mR[1][1] + ptAux.z*mR[1][2]) + ty );
      ptOut->z = static_cast<sub_type>( mScale * (ptAux.x*mR[2][0] + ptAux.y*mR[2][1] + ptAux.z*mR[2][2]) + tz );
    } else {
      sub_type dx = ptIn.x - tx; 
      sub_type dy = ptIn.y - ty; 
      sub_type dz = ptIn.z - tz;
      ptOut->x = static_cast<sub_type>(mScale * (dx*mRinv[0][0] + dy*mRinv[0][1] + dz*mRinv[0][2]));
      ptOut->y = static_cast<sub_type>(mScale * (dx*mRinv[1][0] + dy*mRinv[1][1] + dz*mRinv[1][2]));
      ptOut->z = static_cast<sub_type>(mScale * (dx*mRinv[2][0] + dy*mRinv[2][1] + dz*mRinv[2][2]));
    }
  } catch (std::exception &e ) {
    printError("Error al aplicar la rotación: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Helmert3D<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>( mScale * (ptIn.x*mR[0][0] + ptIn.y*mR[0][1] + ptIn.z*mR[0][2]) + tx );
    r_pt.y = static_cast<sub_type>( mScale * (ptIn.x*mR[1][0] + ptIn.y*mR[1][1] + ptIn.z*mR[1][2]) + ty );
    r_pt.z = static_cast<sub_type>( mScale * (ptIn.x*mR[2][0] + ptIn.y*mR[2][1] + ptIn.z*mR[2][2]) + tz );
  } else {
    sub_type dx = ptIn.x - tx; 
    sub_type dy = ptIn.y - ty; 
    sub_type dz = ptIn.z - tz;
    r_pt.x = static_cast<sub_type>(mScale * (dx*mRinv[0][0] + dy*mRinv[0][1] + dz*mRinv[0][2]));
    r_pt.y = static_cast<sub_type>(mScale * (dx*mRinv[1][0] + dy*mRinv[1][1] + dz*mRinv[1][2]));
    r_pt.z = static_cast<sub_type>(mScale * (dx*mRinv[2][0] + dy*mRinv[2][1] + dz*mRinv[2][2]));
  }
  return r_pt;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setParameters(double tx, double ty, double tz, double scale, double omega, double phi, double kappa)
{
  this->tx = tx;
  this->ty = ty;
  this->tz = tz;
  mScale = scale;
  mOmega = omega;
  mPhi = phi;
  mKappa = kappa;
  update();
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setScale(double scale)
{
  mScale = scale;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::update()
{
  rotationMatrix(mOmega, mPhi, mKappa, &mR);
  cv::Mat inv = cv::Mat(3, 3, CV_64F, mR.data()).inv();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mRinv[i][j] = inv.at<double>(i, j);
    }
  }
}

template<typename Point_t> inline
Helmert3D<Point_t> operator*(Helmert3D<Point_t> &trf1, Helmert3D<Point_t> &trf2)
{
  //const std::array<std::array<double, 3>, 3> R1 = trf1.getRotationMatrix();
  //const std::array<std::array<double, 3>, 3> R2 = trf2.getRotationMatrix();

  //cv::Mat _r1 = cv::Mat(3, 3, CV_32F, R1);

  //std::array<std::array<double, 3>, 3> R;
  //cv::Mat matRot1 = cv::Mat(3, 3, CV_64F, R1.data());
  //cv::Mat matRot2 = cv::Mat(3, 3, CV_64F, R2.data());
  //cv::Mat rot = matRot1 * matRot2;

  //Point_t r_pt;
  //r_pt.x = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[0][0] + trf2.y*R1[0][1] + trf2.z*R1[0][2]) + trf2.x0 );
  //r_pt.y = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[1][0] + trf2.y*R1[1][1] + trf2.z*R1[1][2]) + trf2.y0 );
  //r_pt.z = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[2][0] + trf2.y*R1[2][1] + trf2.z*R1[2][2]) + trf2.z0 );

  //Helmert3D<Point_t> trf(r_pt.x, r_pt.y, r_pt.z, trf1.getScale(), rot);

}

/*! \} */ // end of trf3DGroup

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Aplica una traslación a un conjunto de segmentos
 * \param[in] lines_in
 * \param[out] lines_out
 * \param[in] dx
 * \param[in] dy
 * \deprecated{ Reemplazada por I3D::Translate::transform }
 */
I3D_DEPRECATED("transform(const Entity<T> &in, Entity<T> *out, Transform<Point_t> *trf, transform_order trfOrder)")
I3D_EXPORT void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy);

/*!
 * \brief Aplica una transformación a una entidad
 * \param[in] in Entidad de entrada
 * \param[out] out Entidad de salida
 * \param[in] trf Transformación que se aplica a la entidad
 * \param[in] trfOrder Orden de la transformación. Por defecto transform_order::DIRECT
 */
template<typename T, typename Point_t>
I3D_EXPORT void transform(const Entity<T> &in, Entity<T> *out, Transform<Point_t> *trf, transform_order trfOrder = transform_order::DIRECT)
{
  if (in.getType() == entity_type::WINDOW) {
    Window<T> *w = dynamic_cast<Window<T> *>(out);
    trf->transform(dynamic_cast<const Window<T> &>(in).pt1, &w->pt1, trfOrder);
    trf->transform(dynamic_cast<const Window<T> &>(in).pt2, &w->pt2, trfOrder);
  } else if ( in.getType() == entity_type::SEGMENT_2D) {
    Segment<T> *s = dynamic_cast<Segment<T> *>(out);
    trf->transform(dynamic_cast<const Segment<T> &>(in).pt1, &s->pt1, trfOrder);
    trf->transform(dynamic_cast<const Segment<T> &>(in).pt2, &s->pt2, trfOrder);
  } else if (in.getType() == entity_type::LINESTRING_2D ||
             in.getType() == entity_type::MULTIPOINT_POINT_2D ||
             in.getType() == entity_type::POLYGON_2D) {
    const EntityPoints<T> &_in = dynamic_cast<const EntityPoints<T> &>(in);
    dynamic_cast<EntityPoints<T> *>(out)->resize(_in.getSize());
    typename std::vector<Point<T>>::iterator it_out = dynamic_cast<EntityPoints<T> *>(out)->begin();
    for (typename std::vector<Point<T>>::const_iterator it = _in.begin(); it != _in.end(); it++, it_out++) {
      trf->transform(*it, &(*it_out), trfOrder);
    }
  } else {
    //tipo no soportado
    return;
  }
}

/*!
 * \brief Aplica una transformación a un conjunto de entidades
 * \param[in] in Entidad de entrada
 * \param[out] out Entidad de salida
 * \param[in] trf Transformación que se aplica a la entidad
 * \param[in] trfOrder Orden de la transformación. Por defecto transform_order::DIRECT
 */
template<typename Entity_t, typename Point_t>
I3D_EXPORT void transform(const std::vector<Entity_t> &in, std::vector<Entity_t> *out, Transform<Point_t> *trf, transform_order trfOrder = transform_order::DIRECT)
{
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trf, trfOrder);
  }
}

#ifdef HAVE_OPENCV

/*!
 * \brief Aplica una transformación a una imagen
 * \param[in] in Imagen de entrada
 * \param[out] out Imagen de salida
 * \param[in] trf Transformación que se aplica a la imagen
 * \param[in] trfOrder Orden de la transformación. Por defecto transform_order::DIRECT
 */
template<typename Point_t>
I3D_EXPORT void transform(cv::Mat in, cv::Mat out, Transform<Point_t> *trf, transform_order trfOrder)
{
  transform_type type = trf->getTransformType();
  switch (type) {
  case I3D::transform_type::TRANSLATE:
    Translate<Point_t> transTrf = dynamic_cast<Translate<Point_t>>(trf);
    cv::Mat translateMat( 2, 3, CV_32FC1 );
    translateMat.at<float>(0, 0) = 1.f;
    translateMat.at<float>(0, 1) = 0.f;
    translateMat.at<float>(0, 2) = transTrf.getTranslationX();
    translateMat.at<float>(1, 0) = 0.f;
    translateMat.at<float>(1, 1) = 1.f;
    translateMat.at<float>(1, 2) = transTrf.getTranslationY();
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, translateMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, translateMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case I3D::transform_type::ROTATION:
    Rotation<Point_t> rotTrf = dynamic_cast<Rotation<Point_t>>(trf);
    cv::Mat rotMat( 2, 3, CV_32FC1 );
    double r1 = cos(rotTrf.getAngle());
    double r2 = sin(rotTrf.getAngle());
    rotMat.at<float>(0, 0) = r1;
    rotMat.at<float>(0, 1) = r2;
    rotMat.at<float>(0, 2) = 0.f;
    rotMat.at<float>(1, 0) = r2;
    rotMat.at<float>(1, 1) = r1;
    rotMat.at<float>(1, 2) = 0.f;
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, rotMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, rotMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case I3D::transform_type::HELMERT_2D:
    Helmert2D<Point_t> h2dTrf = dynamic_cast<Helmert2D<Point_t>>(trf);
    cv::Mat h2DMat( 2, 3, CV_32FC1 );
    double rotation = h2dTrf.getRotation();
    double scale = h2dTrf.getScale();
    double a = scale * cos(rotation);
    double b = scale * sin(rotation);
    h2DMat.at<float>(0, 0) = a;
    h2DMat.at<float>(0, 1) = b;
    h2DMat.at<float>(0, 2) = h2dTrf.x0;
    h2DMat.at<float>(1, 0) = b;
    h2DMat.at<float>(1, 1) = a;
    h2DMat.at<float>(1, 2) = h2dTrf.y0;
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, h2DMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, h2DMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case I3D::transform_type::Affine:
    Affine<Point_t> affineTrf = dynamic_cast<Affine<Point_t>>(trf);
    double r00, r10, r01, r11;
    affineTrf.getParameters(&r00, &r10, &r01, &r11);
    cv::Mat affMat( 2, 3, CV_32FC1 );
    affMat.at<float>(0, 0) = r00;
    affMat.at<float>(0, 1) = r10;
    affMat.at<float>(0, 2) = affineTrf.x0;
    affMat.at<float>(1, 0) = r01;
    affMat.at<float>(1, 1) = r11;
    affMat.at<float>(1, 2) = affineTrf.y0;
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, affMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, affMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case I3D::transform_type::PERSPECTIVE:
    TrfPerspective<Point_t> perspTrf = dynamic_cast<TrfPerspective<Point_t>>(trf);
    if (trfOrder == transform_order::DIRECT)
      cv::warpPerspective(in, out, perspTrf.H, in.size(), cv::INTER_LINEAR);
    else
      cv::warpPerspective(in, out, perspTrf.H.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case I3D::transform_type::PROJECTIVE:
    break;
  case I3D::transform_type::POLYNOMIAL:
    break;
  default:
    break;
  }
}

#endif // HAVE_OPENCV

/*! \} */ // end of trfGroup










/* ---------------------------------------------------------------------------------- */
#ifdef HAVE_GDAL

// Clase sistema de referencia
class I3D_EXPORT Crs
{
private:
  
  std::string mEpsg;

  std::string mGrid;

  std::string mGeoid;

  OGRSpatialReference *pCrs;

public:

  Crs(const char *epsg, const char *grid = NULL, const char *geoid = NULL);

  ~Crs();

  const char *getEPSG();

  bool isGeocentric();

  bool isGeographic();

  const OGRSpatialReference &getOGRSpatialReference( ) const { return *pCrs; };
};



//... Cache de sistemas de referencia
// Tiene que ser un singleton
class CrsCache
{
private:
  
  static std::unique_ptr<CrsCache> sCrsCache;

  std::vector <std::shared_ptr<Crs>> mCrss;

private:

  /*!
   * \brief Constructor privado
   */
  CrsCache() {}

public:

  ~CrsCache() {}

  // Se impide la copia y asignación
  CrsCache(CrsCache const&) = delete;
  void operator=(CrsCache const&) = delete;
  
  static void get();

  std::shared_ptr<Crs> getCrs(const char *epsg);

  std::shared_ptr<Crs> findCrs(const char *epsg);

private:

  int add(const char *epsg);

};

// controlar cuando es altura elipsoidal y ortométrica

template<typename Point_t>
class I3D_EXPORT CrsTransform : public Transform3D<Point_t>
{
protected:
  
  /*!
   * \brief Sistema de referencia de entrada
   */
  std::shared_ptr<Crs> mEpsgIn;

  /*!
   * \brief Sistema de referencia de salida
   */
  std::shared_ptr<Crs> mEpsgOut;

  OGRCoordinateTransformation *pCoordinateTransformation;
  
  OGRCoordinateTransformation *pCoordinateTransformationInv;

private:

public:

  /*!
   * \brief Constructor
   */
  CrsTransform(const Crs &epsgIn, const Crs &epsgOut);
  
  CrsTransform(const char *epsgIn, const char *epsgOut);

  ~CrsTransform();

  /*!
   * \brief Operación no soportada para CrsTransform
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Vector con los errores para cada punto
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos a otro sistema de referencia
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \see transform_order
   */
  void transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Transforma un punto a otro sistema de referencia
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \see transform_order
   */
  void transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Transforma un punto a otro sistema de referencia
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::DIRECT) const override;

private:

  void init();

};


template<typename Point_t> inline
CrsTransform<Point_t>::CrsTransform(const Crs &epsgIn, const Crs &epsgOut) 
  : Transform3D<Point_t>(transform_type::CRS), mEpsgIn(epsgin), mEpsgOut(epsgout), 
  pCoordinateTransformation(0), pCoordinateTransformationInv(0) 
{
  init();
}

template<typename Point_t> inline
CrsTransform<Point_t>::CrsTransform(const char *epsgIn, const char *epsgOut) 
  : Transform3D<Point_t>(transform_type::CRS), mEpsgIn(std::make_shared<Crs>(epsgin)), mEpsgOut(std::make_shared<Crs>(epsgout)), 
  pCoordinateTransformation(0), pCoordinateTransformationInv(0) 
{
  init();
}

template<typename Point_t> inline
CrsTransform<Point_t>::~CrsTransform() 
{
  if (pCoordinateTransformation) 
    OGRCoordinateTransformation::DestroyCT( pCoordinateTransformation );
  if (pCoordinateTransformationInv) 
    OGRCoordinateTransformation::DestroyCT( pCoordinateTransformationInv );
  OSRCleanup();
}


template<typename Point_t> inline
double CrsTransform<Point_t>::compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2, std::vector<double> *error)
{
  printError("'compute' no esta soportado para CrsTransform");
  I3D_COMPILER_WARNING("'compute' no esta soportado para CrsTransform");
  return -1.;
}


template<typename Point_t> inline
void CrsTransform<Point_t>::transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, transform_order trfOrder) const
{
  formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
  }
}


template<typename Point_t> inline
void CrsTransform<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const
{
  *ptOut = ptIn;
  try {
    if (trfOrder == transform_order::DIRECT){
      pCoordinateTransformation->Transform(1, &ptOut->x, &ptOut->y, &ptOut->z);
    } else {
      pCoordinateTransformationInv->Transform(1, &ptOut->x, &ptOut->y, &ptOut->z);
    }
  } catch (std::exception &e) {
    throw std::runtime_error( e.what() );
  }
}


template<typename Point_t> inline
Point_t CrsTransform<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt = ptIn;
  try{
    if (trfOrder == transform_order::DIRECT){
      pCoordinateTransformation->Transform(1, &r_pt->x, &r_pt->y, &r_pt->z);
    } else {
      pCoordinateTransformationInv->Transform(1, &r_pt->x, &r_pt->y, &r_pt->z);
    }
  } catch (std::exception &e) {
    throw std::runtime_error( e.what() );
  }
  return r_pt;
}

template<typename Point_t> inline
void CrsTransform<Point_t>::init()
{
  pCoordinateTransformation = OGRCreateCoordinateTransformation( mEpsgIn.getOGRSpatialReference(), mEpsgOut.getOGRSpatialReference() );
  pCoordinateTransformationInv = OGRCreateCoordinateTransformation( mEpsgOut.getOGRSpatialReference(), mEpsgIn.getOGRSpatialReference() );
  OSRCleanup();
}


//class CrsTransformCache
//{
//public:
//  CrsTransformCache();
//  ~CrsTransformCache();
//
//private:
//
//};
//
//CrsTransformCache::CrsTransformCache()
//{
//}
//
//CrsTransformCache::~CrsTransformCache()
//{
//}

/* ---------------------------------------------------------------------------------- */

#endif // HAVE_GDAL




} // End namespace I3D

#endif // I3D_TRANSFORM_H
