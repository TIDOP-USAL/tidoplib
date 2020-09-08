/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


#ifndef TL_GEOMETRY_TRANSFORM_H
#define TL_GEOMETRY_TRANSFORM_H

#include "config_tl.h"

#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <array>
#include <thread>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#endif

#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/core/exception.h"
#include "tidop/math/mathutils.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/segment.h"
#include "tidop/geometry/entities/bbox.h"
#include "tidop/geometry/entities/window.h"

namespace tl
{

template<typename Point_t> class Helmert2D;
template<typename Point_t> class Affine;

/*! \defgroup trfGroup Transformaciones
 *  Transformaciones geométricas
 *  \{
 */

///*!
// * \brief Tipos de transformaciones
// */
//enum class Transform::Type {
//  translation,     /*!< Desplazamiento */
//  rotation,        /*!< Giro */
//  scaling,         /*!< Escalado */
//  reflection,      /*!< */
//  shear,
//  helmert_2d,      /*!< Helmert 2D */
//  affine,          /*!< Afin */
//  perspective,     /*!< Perspectiva */
//  projective,      /*!< Projectiva */
//  helmert_3d,      /*!< Helmert 3D */
//  polynomial,      /*!< Transformación polinómica */
//  // tipos especiales
//  multiple,
//  crs,
//#ifdef TL_ENABLE_DEPRECATED_METHODS
//  TRANSLATION = translation,    /*!< Desplazamiento. */
//  ROTATION = rotation,          /*!< Giro. */
//  HELMERT_2D = helmert_2d,      /*!< Helmert 2D */
//  AFFINE = affine,              /*!< Afin */
//  PERSPECTIVE = perspective,    /*!< Perspectiva */
//  PROJECTIVE = projective,      /*!< Projectiva */
//  HELMERT_3D = helmert_3d,      /*!< Helmert 3D */
//  POLYNOMIAL = polynomial,      /*!< Transformación polinómica*/
//  // tipos especiales
//  MULTIPLE = multiple,
//  CRS = crs
//#endif // TL_ENABLE_DEPRECATED_METHODS
//};
//
//enum class Transform::Order {
//  direct,   /*!< Transformación directa. */
//  inverse,  /*!< Transformación inversa. */
//#ifdef TL_ENABLE_DEPRECATED_METHODS
//  DIRECT = direct,    /*!< Transformación directa. */
//  INVERSE = inverse   /*!< Transformación inversa. */
//#endif // TL_ENABLE_DEPRECATED_METHODS
//};
//
//enum class Transform::Status {
//  success,
//  failure,
//#ifdef TL_ENABLE_DEPRECATED_METHODS
//  SUCCESS = success,
//  FAILURE =failure
//#endif // TL_ENABLE_DEPRECATED_METHODS
//};

/// TODO: inverse()


class Transform
{

public:

  /*!
   * \brief Tipos de transformaciones
   */
  enum class Type
  {
    translation,     /*!< Desplazamiento */
    rotation,        /*!< Giro */
    scaling,         /*!< Escalado */
    reflection,      /*!< */
    shear,
    helmert_2d,      /*!< Helmert 2D */
    affine,          /*!< Afin */
    perspective,     /*!< Perspectiva */
    projective,      /*!< Projectiva */
    helmert_3d,      /*!< Helmert 3D */
    polynomial,      /*!< Transformación polinómica */
    // tipos especiales
    multiple,
    crs
  };

  enum class Order 
  {
    direct,   /*!< Transformación directa. */
    inverse,  /*!< Transformación inversa. */
  };

  enum class Status
  {
    success,
    failure
  };

public:

  Transform() {}
  virtual ~Transform() = default;

  /*!
   * \brief Tipo de transformación
   * \return Tipo de transformación
   * \see Transform::Type
   */
  virtual Type transformType() const = 0;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   * \return Número mínimo de puntos
   */
  virtual int minNumberOfPoints() const = 0;

  /*!
   * \brief Determina si el numero de puntos son suficientes para calcular la transformación
   * \param[in] npoints Número de puntos para calcular la transformación
   * \return Verdadero si son puntos suficientes
   */
  virtual bool isNumberOfPointsValid(size_t npoints) const = 0;

};


/*!
 * \brief Clase base para transformaciones
 */
template<typename Point_t>
class TransformBase
  : public Transform
{

public:

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
   * \param[in] nMin Número mínimo de puntos necesario para la transformación
   */
  TransformBase(Type trfType, int nMin = 0);

  /*!
   * \brief Destructora
   */
  ~TransformBase() override = default;

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  virtual Transform::Status compute(const std::vector<Point_t> &pts1, 
                                    const std::vector<Point_t> &pts2, 
                                    std::vector<double> *error = nullptr,
                                    double *rmse = nullptr) = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                                      Order trfOrder = Order::direct) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  virtual Point_t transform(const Point_t &ptIn, 
                            Transform::Order trfOrder = Transform::Order::direct) const = 0;
    
  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \return Punto de salida
   */
  virtual Point_t operator() (const Point_t &ptIn) const;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transform(const std::vector<Point_t> &ptsIn,
                                      std::vector<Point_t> *ptsOut,
                                      Transform::Order trfOrder = Transform::Order::direct) const = 0;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos aplicando paralelismo
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transformParallel(const std::vector<Point_t> &ptsIn,
                                             std::vector<Point_t> *ptsOut,
                                             Transform::Order trfOrder = Transform::Order::direct) const;

  /*!
   * \brief root-mean-square error (Raiz cuadrada de error cuadratico medio)
   * \param ptsIn Puntos en el sistema de entrada
   * \param ptsOut Puntos en el sistema de salida
   * \param error Vector con los errores para cada punto
   * \return RMSE
   */
  double rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                             const std::vector<Point_t> &ptsOut, 
                             std::vector<double> *error = nullptr);

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
  void formatVectorOut(const std::vector<T2> &ptsIn, 
                       std::vector<T2> *ptsOut) const;

  /*!
   * \brief root-mean-square error
   * \param ptsIn Puntos en el sistema de entrada
   * \param ptsOut Puntos en el sistema de salida
   * \param error Vector con los errores para cada punto
   * \return RMSE
   */
  double _rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                              const std::vector<Point_t> &ptsOut, 
                              std::vector<double> *error = nullptr);

// Transform interfaz

  Type transformType() const override;
  int minNumberOfPoints() const override;
  bool isNumberOfPointsValid(size_t npoints) const override;

protected:

  /*!
   * \brief Tipo de transformación
   * \see Transform::Type
   */
  Type mTrfType;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   */
  int mMinPoint;

  /*!
   * \brief Dimensiones de la transformación. 2D o 3D
   */
  int mDimensions;
};


/// Implementación Transform

template<typename Point_t> inline
TransformBase<Point_t>::TransformBase(Type trfType, int nMin)
  : mTrfType(trfType),
    mMinPoint(nMin)
{

}

template<typename Point_t>
Transform::Type TransformBase<Point_t>::transformType() const
{
  return mTrfType;
}

template<typename Point_t> inline
int TransformBase<Point_t>::minNumberOfPoints() const
{
  return mMinPoint;
}

template<typename Point_t> inline
bool TransformBase<Point_t>::isNumberOfPointsValid(size_t npoints) const
{
  return npoints >= static_cast<size_t>(mMinPoint);
}

template<typename Point_t>
Point_t TransformBase<Point_t>::operator()(const Point_t &ptIn) const
{
  return this->transform(ptIn);
}

template<typename Point_t> inline
Transform::Status TransformBase<Point_t>::transformParallel(const std::vector<Point_t> &ptsIn,
                                                            std::vector<Point_t> *ptsOut,
                                                            Transform::Order trfOrder) const
{
  formatVectorOut(ptsIn, ptsOut);
  Transform::Status r_status;
  parallel_for(0, ptsIn.size(), [&](size_t i) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if (r_status == Transform::Status::failure) return;
  });
  return r_status;
}

template<typename Point_t> inline
double TransformBase<Point_t>::rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                                                   const std::vector<Point_t> &ptsOut, 
                                                   std::vector<double> *error)
{
  size_t n = ptsIn.size();
  std::vector<Point_t> pts_out(n);
  std::vector<double> err(n);
  double sumErr = 0.;

  //... Sería mejor añadirlo en el propio calculo de los parámetros?
  if (compute(ptsIn, ptsOut) == Transform::Status::success) { //... Revisar esto
    for (size_t i = 0; i < n; i++) {
      transform(ptsIn[i], &pts_out[i]);
      pts_out[i] -= ptsOut[i];
      err[i] = static_cast<double>(pts_out[i].x * pts_out[i].x + pts_out[i].y * pts_out[i].y);
      sumErr += err[i];
    }
    if (error) *error = err;
    return sqrt(sumErr/(mDimensions * (n - mMinPoint)));
  }
  return TL_DOUBLE_MAX;
}

template<typename Point_t> template<typename T2> inline
void TransformBase<Point_t>::formatVectorOut(const std::vector<T2> &ptsIn, 
                                             std::vector<T2> *ptsOut) const 
{
  if (&ptsIn != ptsOut && ptsIn.size() != ptsOut->size()) {
    ptsOut->clear();
    ptsOut->resize(ptsIn.size());
  }
}

template<typename Point_t> inline
double TransformBase<Point_t>::_rootMeanSquareError(const std::vector<Point_t> &ptsIn, 
                                                    const std::vector<Point_t> &ptsOut, 
                                                    std::vector<double> *error)
{
  size_t n = ptsIn.size();
  std::vector<Point_t> pts_out(n);
  std::vector<double> err(n);
  double sumErr = 0.;

  for (size_t i = 0; i < n; i++) {
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
 * forma que se ejecutan de manera secuencial.
 */
template<typename Point_t>
class TrfMultiple
  : public TransformBase<Point_t>
{

public:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::list<std::shared_ptr<TransformBase<Point_t>>>::const_iterator const_iterator;

public:

  /*!
   * \brief Constructora
   */
  TrfMultiple();

  /*!
   * \brief Constructor de lista
   * \param[in] transfList listado de transformaciones
   */
  TrfMultiple(std::initializer_list<std::shared_ptr<TransformBase<Point_t>>> transfList);

  /*!
   * \brief Destructora
   */
  virtual ~TrfMultiple() override {}

public:

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  iterator begin() TL_NOEXCEPT;
  const_iterator begin() const TL_NOEXCEPT;
  iterator end() TL_NOEXCEPT;
  const_iterator end() const TL_NOEXCEPT;
  void push_back(const std::shared_ptr<TransformBase<Point_t>> &trf);
  void push_back(std::shared_ptr<TransformBase<Point_t>> &&trf) TL_NOEXCEPT;
  void clear() TL_NOEXCEPT;
  bool empty() const TL_NOEXCEPT;
  size_t size() const;

  void del(int id);

  /*!
   * \brief Elimina el intervalo
   */
  ///TODO: incluir y quitar del
  iterator erase(const_iterator first, const_iterator last);

// Transform Interfaz

  bool isNumberOfPointsValid(int npoints) const override;

// TransformBase Interfaz

  Transform::Status compute(const std::vector<Point_t> &pts1, 
                            const std::vector<Point_t> &pts2, 
                            std::vector<double> *error = nullptr,
                            double *rmse = nullptr) override;
  Transform::Status transform(const Point_t &ptIn, 
                              Point_t *ptOut, 
                              Transform::Order trfOrder = Transform::Order::direct) const override;
  Point_t transform(const Point_t &ptIn, 
                    Transform::Order trfOrder = Transform::Order::direct) const override;
  Transform::Status transform(const std::vector<Point_t> &ptsIn, 
                              std::vector<Point_t> *ptsOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

private:

  /*!
   * \brief Lista de transformaciones
   */
  std::list<std::shared_ptr<TransformBase<Point_t>>> mTransformationList;

};


/// Implementación TrfMultiple

template<typename Point_t>
TrfMultiple<Point_t>::TrfMultiple()
  : TransformBase<Point_t>(Type::multiple)
{
}

template<typename Point_t>
TrfMultiple<Point_t>::TrfMultiple(std::initializer_list<std::shared_ptr<TransformBase<Point_t> > > transformations)
  : TransformBase<Point_t>(Transform::Type::multiple),
    mTransformationList(transformations)
{
}

template<typename Point_t>
typename TrfMultiple<Point_t>::reference TrfMultiple<Point_t>::front()
{
  return mTransformationList.front();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::const_reference TrfMultiple<Point_t>::front() const
{
  return mTransformationList.front();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::reference TrfMultiple<Point_t>::back()
{
  return mTransformationList.back();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::const_reference TrfMultiple<Point_t>::back() const
{
  return mTransformationList.back();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::iterator TrfMultiple<Point_t>::begin() TL_NOEXCEPT
{
  return mTransformationList.begin();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::const_iterator TrfMultiple<Point_t>::begin() const TL_NOEXCEPT
{
  return mTransformationList.cbegin();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::iterator TrfMultiple<Point_t>::end() TL_NOEXCEPT
{
  return mTransformationList.end();
}

template<typename Point_t>
typename TrfMultiple<Point_t>::const_iterator TrfMultiple<Point_t>::end() const TL_NOEXCEPT
{
  return mTransformationList.end();
}

template<typename Point_t>
void TrfMultiple<Point_t>::push_back(const std::shared_ptr<TransformBase<Point_t>> &trf)
{
  mTransformationList.push_back(trf);
}

template<typename Point_t>
void TrfMultiple<Point_t>::push_back(std::shared_ptr<TransformBase<Point_t>> &&trf) TL_NOEXCEPT
{
  mTransformationList.push_back(std::forward<std::shared_ptr<TransformBase<Point_t>>>(trf));
}

template<typename Point_t>
void TrfMultiple<Point_t>::clear() TL_NOEXCEPT
{
  mTransformationList.clear();
}

template<typename Point_t>
bool TrfMultiple<Point_t>::empty() const TL_NOEXCEPT
{
  return mTransformationList.empty();
}

template<typename Point_t>
size_t TrfMultiple<Point_t>::size() const
{
  return mTransformationList.size();
}

template<typename Point_t>
void TrfMultiple<Point_t>::del(int id)
{
  mTransformationList.erase(mTransformationList.begin() + id);
}

template<typename Point_t> inline
bool TrfMultiple<Point_t>::isNumberOfPointsValid(int npoints) const
{ 
  msgError("'isNumberOfPointsValid' is not supported for TrfMultiple");
  TL_COMPILER_WARNING("'isNumberOfPointsValid' is not supported for TrfMultiple");
  return true;
}

template<typename Point_t> inline
Transform::Status TrfMultiple<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                                const std::vector<Point_t> &pts2, 
                                                std::vector<double> *error, 
                                                double *rmse)
{
  msgError("'compute' is not supported for TrfMultiple");
  TL_COMPILER_WARNING("'compute' is not supported for TrfMultiple");
  return Transform::Status::failure;
}

template<typename Point_t> inline
Transform::Status TrfMultiple<Point_t>::transform(const Point_t &ptIn, 
                                                  Point_t *ptOut, 
                                                  Transform::Order trfOrder) const
{
  *ptOut = ptIn;
  Transform::Status r_status;
  for (const auto &transformation : mTransformationList) {
    r_status = transformation->transform(*ptOut, ptOut, trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t TrfMultiple<Point_t>::transform(const Point_t &ptIn, 
                                        Transform::Order trfOrder) const
{
  Point_t out = ptIn;
  for (const auto &trf : transformation) {
    out = transformation->transform(out, trfOrder);
  }
  return out;
}

template<typename Point_t> inline
Transform::Status TrfMultiple<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                  std::vector<Point_t> *ptsOut,
                                                  Transform::Order trfOrder) const
{
  *ptsOut = ptsIn;
  Transform::Status r_status;
  for (const auto &trf : transformation) {
    r_status = transformation->transform(*ptsOut, ptsOut, trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
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
class Transform2D
  : public TransformBase<Point_t>
{

public:

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
   * \param[in] trfType Tipo de transformación
   * \param[in] n_min Número mínimo de puntos necesario para la transformación
   */
  Transform2D(Transform::Type trfType, int n_min = 0)
    : TransformBase<Point_t>(trfType, n_min)
  {
    this->mDimensions = 2;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform2D() {}

  ///*!
  // * \brief Calcula los parámetros de transformación
  // * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
  // * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
  // * \param[out] error Vector con los errores para cada punto
  // * \param[out] rmse Root Mean Square Error
  // * \return Transform::Status
  // * \see Transform::Status
  // */
  //virtual Transform::Status compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2,
  //                                 std::vector<double> *error = nullptr, double *rmse = nullptr) override = 0;
  //
  ///*!
  // * \brief Aplica la transformación a un conjunto de puntos
  // * \param[in] ptsIn Puntos de entrada
  // * \param[out] ptsOut Puntos de salida
  // * \param[in] trfOrder Transformación directa (por defecto) o inversa
  // * \return Transform::Status
  // * \see Transform::Order, Transform::Status
  // */
  //virtual Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut,
  //                                   Transform::Order trfOrder = Transform::Order::direct) const override = 0;
  //
  ///*!
  // * \brief Aplica la transformación a un punto
  // * \param[in] ptIn Punto de entrada
  // * \param[out] ptOut Punto de salida
  // * \param[in] trfOrder Transformación directa (por defecto) o inversa
  // * \return Transform::Status
  // * \see Transform::Order, Transform::Status
  // */
  //virtual Transform::Status transform(const Point_t &ptIn, Point_t *ptOut,
  //                                   Transform::Order trfOrder = Transform::Order::direct) const override = 0;
  //
  ///*!
  // * \brief Aplica la transformación a un punto
  // * \param[in] ptIn Punto de entrada
  // * \param[in] trfOrder Transformación directa (por defecto) o inversa
  // * \return Punto de salida
  // * \see Transform::Order
  // */
  //virtual Point_t transform(const Point_t &ptIn, Transform::Order trfOrder = Transform::Order::direct) const override = 0;

};


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Traslación
 *
 * Transformación que aplica una traslación en el plano a un conjunto de puntos
 */
template<typename Point_t>
class Translation
  : public Transform2D<Point_t>
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
  Translation();

  /*!
   * \brief Constructora
   * \param[in] tx Traslación en el eje X
   * \param[in] ty Traslación en el eje Y
   */
  Translation(double tx, double ty);

  /*!
   * \brief Constructor de copia
   * \param[in] translation Objeto que se copia
   */
  Translation(const Translation &translation);

  /*!
   * \brief Constructor de movimiento
   * \param[in] translation Objeto que se copia
   */
  Translation(Translation &&translation) TL_NOEXCEPT;


  Transform::Status compute(const std::vector<Point_t> &pts1, 
                            const std::vector<Point_t> &pts2, 
                            std::vector<double> *error = nullptr,
                            double *rmse = nullptr) override;
  Transform::Status transform(const Point_t &ptIn,
                              Point_t *ptOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;
  Transform::Status transform(const std::vector<Point_t> &ptsIn,
                              std::vector<Point_t> *ptsOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Transformación inversa
   * \return
   */
  Translation inverse() const;

  /*!
   * \brief Operador de asignación
   * \param[in] translation Objeto Translation que se copia
   */
  Translation &operator = (const Translation<Point_t> &translation);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] translation Objeto Translation que se mueve
   */
  Translation &operator = (Translation<Point_t> &&translation) TL_NOEXCEPT;


  // conversión a otras transformaciones
  template <typename Point_t2>
  explicit operator Translation<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Helmert2D<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Affine<Point_t2>() const;
};


/// Implementación Translate

template<typename Point_t> inline
Translation<Point_t>::Translation()
  : Transform2D<Point_t>(Transform::Type::translation, 1),
    tx(0.),
    ty(0.)
{
}

template<typename Point_t> inline
Translation<Point_t>::Translation(double tx, double ty)
  : Transform2D<Point_t>(Transform::Type::translation, 1),
    tx(tx),
    ty(ty)
{
}

template<typename Point_t> inline
Translation<Point_t>::Translation(const Translation &translation)
  : Transform2D<Point_t>(translation),
    tx(translation.tx),
    ty(translation.ty)
{
}

template<typename Point_t> inline
Translation<Point_t>::Translation(Translation &&translation) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(translation)),
    tx(std::move(translation.tx)),
    ty(std::move(translation.ty))
{
}

template<typename Point_t> inline
Transform::Status Translation<Point_t>::compute(const std::vector<Point_t> &pts1,
                                                const std::vector<Point_t> &pts2,
                                                std::vector<double> *error,
                                                double *rmse)
{
  size_t n1 = pts1.size();
  size_t n2 = pts2.size();
  
  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zui", n1, n2);
    return Transform::Status::failure;
  } 
  
  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;
  size_t m = n1 * this->mDimensions, n = 4;
  double *a = new double[m*n];
  double *pa = a;
  double *b = new double[m];
  double *pb = b;
  double *c = new double[n];
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

    solveSVD(m, n, a, b, c);
    tx = c[2];
    ty = c[3];

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }
    
  } catch (std::exception &e) {
    MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  delete[] a;
  delete[] b;
  delete[] c;

  return status;
}

template<typename Point_t> inline
Transform::Status Translation<Point_t>::transform(const Point_t &ptIn,
                                                  Point_t *ptOut,
                                                  Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  Point_t pt_aux;
  if (std::is_integral<sub_type>::value) {
    pt_aux.x = TL_ROUND_TO_INT(tx);
    pt_aux.y = TL_ROUND_TO_INT(ty);
  } else {
    pt_aux.x = static_cast<sub_type>(tx);
    pt_aux.y = static_cast<sub_type>(ty);
  }
  if(ptOut)
    *ptOut = (trfOrder == Transform::Order::direct) ? ptIn + pt_aux : ptIn - pt_aux;
  return r_status;
}

template<typename Point_t> inline
Point_t Translation<Point_t>::transform(const Point_t &ptIn, 
                                        Transform::Order trfOrder) const
{
  Point_t pt_aux;
  Transform::Status r_status = transform(ptIn, &pt_aux, trfOrder);
  return r_status == Transform::Status::success ? pt_aux : ptIn;
}

template<typename Point_t> inline
Transform::Status Translation<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                  std::vector<Point_t> *ptsOut,
                                                  Transform::Order trfOrder) const
{
  this->formatVectorOut(ptsIn, ptsOut);
  Transform::Status r_status = Transform::Status::success;
  for (size_t i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Translation<Point_t> Translation<Point_t>::inverse() const
{
  return Translation<Point_t>(-this->tx, -this->ty);
}

template<typename Point_t>
Translation<Point_t> &Translation<Point_t>::operator =(const Translation<Point_t> &translation)
{
  if (this != &translation) {
    Transform2D<Point_t>::operator = (translation);
    this->tx = translation.tx;
    this->ty = translation.ty;
  }
  return *this;
}

template<typename Point_t>
Translation<Point_t> &Translation<Point_t>::operator =(Translation<Point_t> &&translation) TL_NOEXCEPT
{
  if (this != &translation) {
    Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(translation));
    this->tx = std::move(translation.tx);
    this->ty = std::move(translation.ty);
  }
  return *this;
}

template<typename Point_t> template<typename Point_t2> inline
Translation<Point_t>::operator Translation<Point_t2>() const
{
  return Translation<Point_t2>(this->tx, this->ty);
}

template<typename Point_t> template<typename Point_t2> inline
Translation<Point_t>::operator Helmert2D<Point_t2>() const
{
  return Helmert2D<Point_t2>(this->tx, this->ty, 1., 0.);
}
  
template<typename Point_t> template<typename Point_t2> inline
Translation<Point_t>::operator Affine<Point_t2>() const
{
  return Affine<Point_t2>(this->tx, this->ty, 1., 1., 0.);
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Rotación
 *
 * Transformación que aplica una rotación en el plano a un conjunto de puntos 
 */
template<typename Point_t> 
class Rotation
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o cv::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

private:

  /*!
   * \brief Ángulo de rotación
   */
  double mAngle;

  /*!
   * \brief r1 = cos(angle);
   */
  double r1;

  /*!
   * \brief r2 = sin(angle);
   */
  double r2;

public:

  /*!
   * \brief Constructora por defecto
   */
  Rotation();

  /*!
   * \brief Constructora
   * \param[in] angle Ángulo en radianes
   */
  Rotation(double angle);

  /*!
   * \brief Constructor de copia
   * \param[in] rotation Objeto Rotation que se copia
   */
  Rotation(const Rotation &rotation);

  /*!
   * \brief Constructor de movimiento
   * \param[in] rotation Objeto Rotation que se copia
   */
  Rotation(Rotation &&rotation) TL_NOEXCEPT;

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
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, 
                            const std::vector<Point_t> &pts2, 
                            std::vector<double> *error = nullptr,
                            double *rmse = nullptr) override;


  /*!
   * \brief Devuelve el ángulo de la rotación
   * \return Ángulo en radianes
   */
  double angle() const;

  /*!
   * \brief Establece en ángulo de la rotación
   * \param[in] angle Ángulo en radianes
   */
  void setAngle(double angle);

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

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
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn,
                             std::vector<Point_t> *ptsOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Transformación inversa
   * \return
   */
  Rotation inverse() const;

  /*!
   * \brief Operador de asignación
   * \param[in] rotation Objeto Rotation que se copia
   */
  Rotation &operator = (const Rotation<Point_t> &rotation);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] rotation Objeto Rotation que se mueve
   */
  Rotation &operator = (Rotation<Point_t> &&rotation) TL_NOEXCEPT;


  template<typename Point_t2>
  explicit operator Rotation<Point_t2>() const;

  template<typename Point_t2>
  explicit operator Helmert2D<Point_t2>() const;

  template<typename Point_t2>
  explicit operator Affine<Point_t2>() const;

private:

  /*!
   * \brief Actualiza los parámetros r1 y r2
   */
  void update();
};

template<typename Point_t> inline
Rotation<Point_t>::Rotation()
  : Transform2D<Point_t>(Transform::Type::rotation, 1),
    mAngle(0.)
{
  update();
}

template<typename Point_t> inline
Rotation<Point_t>::Rotation(double angle)
  : Transform2D<Point_t>(Transform::Type::rotation, 1),
    mAngle(angle)
{
  update();
}

template<typename Point_t> inline
Rotation<Point_t>::Rotation(const Rotation &rotation)
  : Transform2D<Point_t>(rotation),
    mAngle(rotation.mAngle),
    r1(rotation.r1),
    r2(rotation.r2)
{
}

template<typename Point_t> inline
Rotation<Point_t>::Rotation(Rotation &&rotation) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(rotation)),
    mAngle(std::move(rotation.mAngle)),
    r1(std::move(rotation.r1)),
    r2(std::move(rotation.r2))
{
}

template<typename Point_t> inline
Transform::Status Rotation<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                            const std::vector<Point_t> &pts2,
                                            std::vector<double> *error, 
                                            double *rmse)
{
  size_t n1 = pts1.size();
  size_t n2 = pts2.size();
  
  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zu", n1, n2);
    return Transform::Status::failure;
  } 

  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;
  size_t m = n1 * this->mDimensions, n = 2;
  double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
  double *c = new double[n];

  try {
    for (int i = 0; i < n1; i++) {
      *pa++ = pts1[i].x;
      *pa++ = -pts1[i].y;
      *pb++ = pts2[i].x;
      *pa++ = pts1[i].y;
      *pa++ = pts1[i].x;
      *pb++ = pts2[i].y;
    }
    
    solveSVD(m, n, a, b, c);
    r1 = c[0];
    r2 = c[1];
    mAngle = acos(r1);

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }
  } catch (std::exception &e) {
    MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  delete[] a;
  delete[] b;
  delete[] c;

  return status;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
double Rotation<Point_t>::getAngle() const 
{ 
  return mAngle; 
}
#endif // TL_ENABLE_DEPRECATED_METHODS

template<typename Point_t> inline
double Rotation<Point_t>::angle() const
{
  return mAngle;
}

template<typename Point_t> inline
void Rotation<Point_t>::setAngle(double angle)
{ 
  mAngle = angle;
  update(); 
}

template<typename Point_t> inline
Transform::Status Rotation<Point_t>::transform(const Point_t &ptsIn, Point_t *ptsOut, 
                                              Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  if (ptsOut){
    sub_type x_aux = ptsIn.x;
    if (trfOrder == Transform::Order::direct) {
      ptsOut->x = static_cast<sub_type>(x_aux*r1 - ptsIn.y*r2);
      ptsOut->y = static_cast<sub_type>(x_aux*r2 + ptsIn.y*r1);
    } else {
      ptsOut->x = static_cast<sub_type>(x_aux*r1 + ptsIn.y*r2);
      ptsOut->y = static_cast<sub_type>(ptsIn.y*r1 - x_aux*r2);
    }
  } else
    r_status = Transform::Status::failure;
  return r_status; 
}

template<typename Point_t> inline
Point_t Rotation<Point_t>::transform(const Point_t &ptsIn, Transform::Order trfOrder) const
{
  Point_t out;
  if (trfOrder == Transform::Order::direct) {
    out.x = ptsIn.x*r1 - ptsIn.y*r2;
    out.y = ptsIn.x*r2 + ptsIn.y*r1;
  } else {
    out.x = ptsIn.x*r1 + ptsIn.y*r2;
    out.y = ptsIn.y*r1 - ptsIn.x*r2;
  }
  return out;
}

template<typename Point_t> inline
Transform::Status Rotation<Point_t>::transform(const std::vector<Point_t> &in,
                                              std::vector<Point_t> *out,
                                              Transform::Order trfOrder) const
{
  this->formatVectorOut(in, out);
  Transform::Status r_status;
  for (int i = 0; i < in.size(); i++) {
    r_status = transform(in[i], &(*out)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t>
Rotation<Point_t> &Rotation<Point_t>::operator =(const Rotation<Point_t> &rotation)
{
  if (this != &rotation) {
    Transform2D<Point_t>::operator = (rotation);
    this->mAngle = rotation.mAngle;
    this->r1 = rotation.r1;
    this->r2 = rotation.r2;
  }
  return *this;
}

template<typename Point_t>
Rotation<Point_t> &Rotation<Point_t>::operator =(Rotation<Point_t> &&rotation) TL_NOEXCEPT
{
  if (this != &rotation) {
    Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(rotation));
    this->mAngle = std::move(rotation.mAngle);
    this->r1 = std::move(rotation.r1);
    this->r2 = std::move(rotation.r2);
  }
  return *this;
}

template<typename Point_t> inline
Rotation<Point_t> Rotation<Point_t>::inverse() const
{
  return Rotation<Point_t>(-this->angle());
}

template<typename Point_t> template<typename Point_t2> inline
Rotation<Point_t>::operator Rotation<Point_t2>() const
{
  return Rotation<Point_t2>(mAngle);
}

template<typename Point_t> template<typename Point_t2> inline
Rotation<Point_t>::operator Helmert2D<Point_t2>() const
{
  return Helmert2D<Point_t2>(0., 0., 1., mAngle);
}
  
template<typename Point_t> template<typename Point_t2>  inline
Rotation<Point_t>::operator Affine<Point_t2>() const
{
  return Affine<Point_t2>(0., 0., 1., 1., mAngle);
}

template<typename Point_t> inline
void Rotation<Point_t>::update()
{
  r1 = cos(mAngle);
  r2 = sin(mAngle);
}


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Escala
 *
 * Transformación que aplica una traslación en el plano a un conjunto de puntos
 */
template<typename Point_t>
class Scaling
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Escala
   */
  double mScale;

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
  Scaling();

  /*!
   * \brief Constructora
   * \param[in] scale Escala
   */
  Scaling(double scale);

  /*!
   * \brief Constructor de copia
   * \param[in] obj Objeto que se copia
   */
  Scaling(const Scaling &obj);

  /*!
   * \brief Constructor de movimiento
   * \param[in] obj Objeto que se copia
   */
  Scaling(Scaling &&obj) TL_NOEXCEPT;

  /*!
   * \brief Cálculo de la traslación
   *
   * Calcula la traslación entre dos sistemas diferentes a partir
   * de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1,
                           const std::vector<Point_t> &pts2,
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn,
                             Point_t *ptOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una traslación
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn,
                             std::vector<Point_t> *ptsOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala
   */
  double scale() const;

  void setScale(double scale);

  /*!
   * \brief Transformación inversa
   * \return
   */
  Scaling inverse() const;

  /*!
   * \brief Operador de asignación
   * \param[in] obj Objeto Scaling que se copia
   */
  Scaling &operator = (const Scaling<Point_t> &obj);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] obj Objeto Scaling que se mueve
   */
  Scaling &operator = (Scaling<Point_t> &&obj) TL_NOEXCEPT;


  // conversión a otras transformaciones
  template <typename Point_t2>
  explicit operator Scaling<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Helmert2D<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Affine<Point_t2>() const;
};


/// Implementación Scaling

template<typename Point_t> inline
Scaling<Point_t>::Scaling()
  : Transform2D<Point_t>(Transform::Type::scaling, 1),
    mScale(1.)
{
}

template<typename Point_t> inline
Scaling<Point_t>::Scaling(double scale)
  : Transform2D<Point_t>(Transform::Type::scaling, 1),
    mScale(scale)
{
}

template<typename Point_t> inline
Scaling<Point_t>::Scaling(const Scaling &obj)
  : Transform2D<Point_t>(obj),
    mScale(obj.mScale)
{
}

template<typename Point_t> inline
Scaling<Point_t>::Scaling(Scaling &&obj) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(obj)),
    mScale(std::move(obj.mScale))
{
}

template<typename Point_t> inline
Transform::Status Scaling<Point_t>::compute(const std::vector<Point_t> &pts1,
                                              const std::vector<Point_t> &pts2,
                                              std::vector<double> *error,
                                              double *rmse)
{
  size_t n1 = pts1.size();
  size_t n2 = pts2.size();

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zui", n1, n2);
    return Transform::Status::failure;
  }

  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;
  size_t m = n1 * this->mDimensions, n = 4;
  double *a = new double[m*n];
  double *pa = a;
  double *b = new double[m];
  double *pb = b;
  double *c = new double[n];
  try {
    for (int i = 0; i < n1; i++) {
      *pa++ = pts1[i].x;
      *pa++ = 0;
      *pb++ = pts2[i].x;
      *pa++ = 0;
      *pa++ = pts1[i].y;
      *pb++ = pts2[i].y;
    }

    solveSVD(m, n, a, b, c);
    double a = c[0];
    double b = c[1];

    mScale = sqrt(a*a + b*b);

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  delete[] a;
  delete[] b;
  delete[] c;

  return status;
}

template<typename Point_t> inline
Transform::Status Scaling<Point_t>::transform(const Point_t &ptIn,
                                             Point_t *ptOut,
                                             Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  try {
    if (trfOrder == Transform::Order::direct){
      ptOut->x = static_cast<sub_type>(mScale * ptIn.x);
      ptOut->y = static_cast<sub_type>(mScale * ptIn.y);
    } else {
      double inv = 1. / mScale;
      ptOut->x = static_cast<sub_type>(inv * ptIn.x);
      ptOut->y = static_cast<sub_type>(inv * ptIn.y);
    }
  } catch (tl::Exception &e ) {
    MessageManager::release(
          MessageManager::Message("Scaling transformation error: %s", e.what()).message(),
          tl::MessageLevel::msg_error);
    r_status = Transform::Status::failure;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t Scaling<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t pt_aux;
  Transform::Status r_status = transform(ptIn, &pt_aux, trfOrder);
  return r_status == Transform::Status::success ? pt_aux : ptIn;
}

template<typename Point_t> inline
Transform::Status Scaling<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                std::vector<Point_t> *ptsOut,
                                                Transform::Order trfOrder) const
{
  this->formatVectorOut(ptsIn, ptsOut);
  Transform::Status r_status = Transform::Status::success;
  for (size_t i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t>
double Scaling<Point_t>::scale() const
{
  return mScale;
}

template<typename Point_t>
void Scaling<Point_t>::setScale(double scale)
{
  mScale = scale;
}

template<typename Point_t> inline
Scaling<Point_t> Scaling<Point_t>::inverse() const
{
  return Scaling<Point_t>(1./mScale);
}

template<typename Point_t>
Scaling<Point_t> &Scaling<Point_t>::operator =(const Scaling<Point_t> &obj)
{
  if (this != &obj) {
    Transform2D<Point_t>::operator = (obj);
    this->mScale = obj.mScale;
  }
  return *this;
}

template<typename Point_t>
Scaling<Point_t> &Scaling<Point_t>::operator =(Scaling<Point_t> &&obj) TL_NOEXCEPT
{
  if (this != &obj) {
    Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(obj));
    this->mScale = std::move(obj.mScale);
  }
  return *this;
}

template<typename Point_t> template<typename Point_t2> inline
Scaling<Point_t>::operator Scaling<Point_t2>() const
{
  return Scaling<Point_t2>(mScale);
}

template<typename Point_t> template<typename Point_t2> inline
Scaling<Point_t>::operator Helmert2D<Point_t2>() const
{
  return Helmert2D<Point_t2>(0., 0., mScale, 0.);
}

template<typename Point_t> template<typename Point_t2> inline
Scaling<Point_t>::operator Affine<Point_t2>() const
{
  return Affine<Point_t2>(0., 0., mScale, mScale, 0.);
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
 * \f$ y' = a * y - b * x + Y0\f$
 */
template<typename Point_t>
class Helmert2D
  : public Transform2D<Point_t>
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
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

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

  double ai;
  double bi;
  double txi;
  double tyi;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert2D();

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en x
   * \param[in] ty Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  Helmert2D(double tx, double ty, double scale, double rotation);

  /*!
   * \brief Constructor de copia
   * \param[in] helmert2D Objeto que se copia
   */
  Helmert2D(const Helmert2D &helmert2D);

  /*!
   * \brief Constructor de movimiento
   * \param[in] helmert2D Objeto que se copia
   */
  Helmert2D(Helmert2D &&helmert2D) TL_NOEXCEPT;

  ~Helmert2D() override {}

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, 
                           const std::vector<Point_t> &pts2, 
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
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
  Transform::Status transform(const std::vector<Point_t> &ptsIn,
                             std::vector<Point_t> *ptsOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
  * \brief Aplica un helmert 2D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn,
                             Point_t *ptOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;


  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve el giro
   * \return[in] Ángulo de rotación en radianes
   */
  TL_DEPRECATED("rotation", "2.0")
  double getRotation() const;


  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  TL_DEPRECATED("scale", "2.0")
  double getScale() const;
#endif

  /*!
   * \brief Devuelve el giro
   * \return[in] Ángulo de rotación en radianes
   */
  double rotation() const;


  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double scale() const;

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

  /*!
   * \brief Transformación inversa
   * \return
   */
  Helmert2D inverse() const;

  /*!
   * \brief Operador de asignación
   * \param[in] helmert Objeto Helmert2D que se copia
   */
  Helmert2D &operator = (const Helmert2D<Point_t> &helmert);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] helmert Objeto Helmert2D que se mueve
   */
  Helmert2D &operator = (Helmert2D<Point_t> &&helmert) TL_NOEXCEPT;

  // conversión a otras transformaciones

  template <typename Point_t2>
  explicit operator Helmert2D<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Affine<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Translation<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Rotation<Point_t2>() const;

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
Helmert2D<Point_t>::Helmert2D()
  : Transform2D<Point_t>(Transform::Type::helmert_2d, 2),
    tx(0),
    ty(0),
    mScale(1.),
    mRotation(0.)
{
  update();
}

template<typename Point_t>
Helmert2D<Point_t>::Helmert2D(double tx, double ty, double scale, double rotation)
  : Transform2D<Point_t>(Transform::Type::helmert_2d, 2),
    tx(tx),
    ty(ty),
    mScale(scale),
    mRotation(rotation)
{
  update();
}

template<typename Point_t> inline
Helmert2D<Point_t>::Helmert2D(const Helmert2D &helmert2D)
  : Transform2D<Point_t>(helmert2D),
    tx(helmert2D.tx),
    ty(helmert2D.ty),
    mScale(helmert2D.mScale),
    mRotation(helmert2D.mRotation),
    a(helmert2D.a),
    b(helmert2D.b),
    ai(helmert2D.ai),
    bi(helmert2D.bi),
    txi(helmert2D.txi),
    tyi(helmert2D.tyi)
{
}

template<typename Point_t> inline
Helmert2D<Point_t>::Helmert2D(Helmert2D &&helmert2D) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(helmert2D)),
    tx(std::move(helmert2D.tx)),
    ty(std::move(helmert2D.ty)),
    mScale(std::move(helmert2D.mScale)),
    mRotation(std::move(helmert2D.mRotation)),
    a(std::move(helmert2D.a)),
    b(std::move(helmert2D.b)),
    ai(std::move(helmert2D.ai)),
    bi(std::move(helmert2D.bi)),
    txi(std::move(helmert2D.txi)),
    tyi(std::move(helmert2D.tyi))
{
}


template<typename Point_t> inline
Transform::Status Helmert2D<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                             const std::vector<Point_t> &pts2,
                                             std::vector<double> *error,
                                             double *rmse)
{
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %i and size pts2 = %i", n1, n2);
    return Transform::Status::failure;
  } 
  
  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }
    
  Transform::Status status = Transform::Status::success;
  size_t m = n1 * this->mDimensions, n = 4;
  double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
  double *c = new double[n];
  try {
    for (int i = 0; i < n1; i++) {
      *pa++ = pts1[i].x;
      *pa++ = -pts1[i].y;
      *pa++ = 1;
      *pa++ = 0;
      *pb++ = pts2[i].x;
      *pa++ = pts1[i].y;
      *pa++ = pts1[i].x;
      *pa++ = 0;
      *pa++ = 1;
      *pb++ = pts2[i].y;
    }

    solveSVD(m, n, A, B, c);

    a = c[0];
    b = c[1];
    tx = c[2];
    ty = c[3];

    updateInv();

    mRotation = atan2(b, a);
    mScale = sqrt(a*a + b*b);

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }
  delete[] A;
  delete[] B;
  delete[] c;

  return status;
}

template<typename Point_t> inline
Transform::Status Helmert2D<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, 
                                               Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  sub_type x_aux = ptIn.x;
  try {
    if (trfOrder == Transform::Order::direct){
      ptOut->x = static_cast<sub_type>(a * x_aux - b * ptIn.y + tx);
      ptOut->y = static_cast<sub_type>(b * x_aux + a * ptIn.y + ty);
    } else {
//      double det = a*a + b*b;
//      if ( det == 0. ) throw TL_ERROR("Division by zero");
//      ptOut->x = static_cast<sub_type>((a*(x_aux - tx) + b*(ptIn.y - ty)) / det);
//      ptOut->y = static_cast<sub_type>((-b*(x_aux - tx) + a*(ptIn.y - ty)) / det);
      ptOut->x = static_cast<sub_type>(ai * x_aux - bi * ptIn.y + txi);
      ptOut->y = static_cast<sub_type>(bi * x_aux + ai * ptIn.y + tyi);
    }
  } catch (tl::Exception &e ) {
    MessageManager::release(
          MessageManager::Message("Helmert 2D transformation error: %s", e.what()).message(),
          tl::MessageLevel::msg_error);
    r_status = Transform::Status::failure;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t Helmert2D<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == Transform::Order::direct){
    r_pt.x = static_cast<sub_type>(a * ptIn.x - b * ptIn.y + tx);
    r_pt.y = static_cast<sub_type>(b * ptIn.x + a * ptIn.y + ty);
  } else {
    r_pt.x = static_cast<sub_type>(ai * ptIn.x - bi * ptIn.y + txi);
    r_pt.y = static_cast<sub_type>(bi * ptIn.x + ai * ptIn.y + tyi);
  }
  return r_pt;
}

template<typename Point_t> inline
Transform::Status Helmert2D<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                               std::vector<Point_t> *ptsOut,
                                               Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
double Helmert2D<Point_t>::getRotation() const
{
  return mRotation;
}

template<typename Point_t>
double Helmert2D<Point_t>::getScale() const
{
  return mScale;
}
#endif

template<typename Point_t> inline
double Helmert2D<Point_t>::rotation() const
{
  return mRotation;
}

template<typename Point_t>
double Helmert2D<Point_t>::scale() const
{
  return mScale;
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
Helmert2D<Point_t> Helmert2D<Point_t>::inverse() const
{
  return Helmert2D<Point_t>(txi, tyi, sqrt(ai*ai + bi*bi), atan2(bi, ai));
}

template<typename Point_t> inline
Helmert2D<Point_t> &Helmert2D<Point_t>::operator = (const Helmert2D<Point_t> &helmert)
{
  if (this != &helmert) {
    Transform2D<Point_t>::operator = (helmert);
    this->tx = helmert.tx;
    this->ty = helmert.ty;
    this->mRotation = helmert.mRotation;
    this->mScale = helmert.mScale;
    this->a = helmert.a;
    this->b = helmert.b;
    this->ai = helmert.ai;
    this->bi = helmert.bi;
    this->txi = helmert.txi;
    this->tyi = helmert.tyi;
  }
  return *this;
}

template<typename Point_t> inline
Helmert2D<Point_t> &Helmert2D<Point_t>::operator = (Helmert2D<Point_t> &&helmert) TL_NOEXCEPT
{
  if (this != &helmert) {
    Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(helmert));
    this->tx = std::move(helmert.tx);
    this->ty = std::move(helmert.ty);
    this->mRotation = std::move(helmert.mRotation);
    this->mScale = std::move(helmert.mScale);
    this->a = std::move(helmert.a);
    this->b = std::move(helmert.b);
    this->ai = std::move(helmert.ai);
    this->bi = std::move(helmert.bi);
    this->txi = std::move(helmert.txi);
    this->tyi = std::move(helmert.tyi);
  }
  return *this;
}

template<typename Point_t> template <typename Point_t2> inline
Helmert2D<Point_t>::operator Helmert2D<Point_t2>() const
{
  return Helmert2D<Point_t2>(this->tx, this->ty, this->mScale, this->mRotation);
}

template<typename Point_t> template<typename Point_t2> inline
Helmert2D<Point_t>::operator Affine<Point_t2>() const
{
  return Affine<Point_t2>(this->tx, this->ty, this->mScale, this->mScale, this->mRotation);
}

template<typename Point_t>  template <typename Point_t2> inline
Helmert2D<Point_t>::operator Translation<Point_t2>() const
{
  return Translation<Point_t2>(this->tx, this->ty);
}

template<typename Point_t>  template <typename Point_t2> inline
Helmert2D<Point_t>::operator Rotation<Point_t2>() const
{
  return Rotation<Point_t2>(this->mRotation);
}

template<typename Point_t> inline
void Helmert2D<Point_t>::update()
{
  a = mScale * cos(mRotation);
  b = mScale * sin(mRotation);

  updateInv();
}

template<typename Point_t>
void Helmert2D<Point_t>::updateInv()
{
  // Transformación inversa
  //double det = a * d - c * b;
  double det = a*a + b*b;
  if (det == 0.) {
    msgError("determinant null");
  } else {
    ai = a / det;
    bi = -b / det;
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    txi = (-a * tx - b * ty) / det;
    tyi = (-a * ty + b * tx) / det;
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación afín
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
class Affine
  : public Transform2D<Point_t>
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
   * \brief Tipo de objeto punto. Puede ser Point<sub_type> o CV::Point_<sub_type>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

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

public:

  /*!
   * \brief Constructor por defecto
   */
  Affine();

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en el eje X
   * \param[in] ty Traslación en el eje Y
   * \param[in] scaleX Escala en el eje X
   * \param[in] scaleY Escala en el eje Y
   * \param[in] rotation Rotación
   */
  Affine(double tx, double ty, double scaleX, double scaleY, double rotation);

  /*!
   * \brief Constructor a partir de una matriz con los coeficientes
   * \f[
   *   A=\begin{bmatrix}
   *   a & b & x0 \\
   *   c & d & y0 \\
   * \end{bmatrix}
   * \f]
   *
   * \f[ a =  scaleX * cos(rotation)\f]
   * \f[ b = -scaleY * sin(rotation)\f]
   * \f[ c =  scaleX * sin(rotation)\f]
   * \f[ d =  scaleY * cos(rotation)\f]
   *
   * \param[in] mat Matriz de coficientes
   */
  Affine(const math::Matrix<2, 3, double> &mat);

  //~Affine();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, 
                           const std::vector<Point_t> &pts2, 
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación afín
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una transformación afín a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una transformación afín a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;
#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve los coeficientes de la transformación
   * \param[out] a Coeficiente a
   * \param[out] b Coeficiente b
   * \param[out] c Coeficiente c
   * \param[out] d Coeficiente d
   * \deprecated Use 'parameters(double *a, double *b, double *c, double *d)' en su lugar
   */
  TL_DEPRECATED("parameters", "2.0")
  void getParameters(double *a, double *b, double *c, double *d);

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   * \deprecated Use 'rotation()' en su lugar
   */
  TL_DEPRECATED("rotation", "2.0")
  double getRotation() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   * \deprecated Use 'scaleX()' en su lugar
   */
  TL_DEPRECATED("scaleX", "2.0")
  double getScaleX() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   * \deprecated Use 'scaleY()' en su lugar
   */
  TL_DEPRECATED("scaleY", "2.0")
  double getScaleY() const;
#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve los coeficientes de la transformación
   * \param[out] a Coeficiente a
   * \param[out] b Coeficiente b
   * \param[out] c Coeficiente c
   * \param[out] d Coeficiente d
   * \param[out] x0 Traslación en x
   * \param[out] y0 Traslación en y
   */
  void parameters(double *a, double *b, double *c, double *d, double *x0, double *y0) const;

  /*!
   * \brief Devuelve los coeficientes de la transformación
   * \param[out] a Coeficiente a
   * \param[out] b Coeficiente b
   * \param[out] c Coeficiente c
   * \param[out] d Coeficiente d
   * \param[out] tx Traslación en x
   * \param[out] ty Traslación en y
   */
  math::Matrix<2,3,double> parameters() const;

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  double rotation() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   */
  double scaleX() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   */
  double scaleY() const;

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
   * \f[ a =  scaleX * cos(rotation)\f]
   * \f[ b = -scaleY * sin(rotation)\f]
   * \f[ c =  scaleX * sin(rotation)\f]
   * \f[ d =  scaleY * cos(rotation)\f]
   * \param[in] a Coeficiente a
   * \param[in] b Coeficiente b
   * \param[in] c Coeficiente c
   * \param[in] d Coeficiente d
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
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
Affine<Point_t>::Affine()
  : Transform2D<Point_t>(Transform::Type::affine, 3),
    tx(0.),
    ty(0.),
    mScaleX(1.),
    mScaleY(1.),
    mRotation(0.)
{
  update();
}

template<typename Point_t> inline
Affine<Point_t>::Affine(double tx, double ty, double scaleX, double scaleY, double rotation)
  : Transform2D<Point_t>(Transform::Type::affine, 3),
    tx(tx),
    ty(ty),
    mScaleX(scaleX),
    mScaleY(scaleY),
    mRotation(rotation)
{
  update();
}

template<typename Point_t> inline
Affine<Point_t>::Affine(const math::Matrix<2, 3, double> &mat)
  : Transform2D<Point_t>(Transform::Type::affine, 3)
{
  this->setParameters(mat.at(0,0), mat.at(0,1), mat.at(1,0), mat.at(1,1), mat.at(0,2), mat.at(2,2));
}

template<typename Point_t> inline
Transform::Status Affine<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                          const std::vector<Point_t> &pts2,
                                          std::vector<double> *error,
                                          double *rmse)
{
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %i and size pts2 = %i", n1, n2);
    return Transform::Status::failure;
  } 
  
  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;

  size_t m = n1 * this->mDimensions, n = 6;
  double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
  double *C = new double[n];
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

    tl::solveSVD(m, n, A, B, C);

    a = C[0];
    b = C[1];
    c = C[2];
    d = C[3];
    tx = C[4];
    ty = C[5];
    
    updateInv();
    //double f = atan2( c, a );
    //double w = atan2( -b, d );

    mRotation = (atan2(c, a) + atan2(-b, d) ) / 2.; //... Revisar lo de un unico giro
    mScaleX = sqrt(a*a + c*c);
    mScaleY = sqrt(b*b + d*d);

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }

  delete[] A;
  delete[] B;
  delete[] C;

  return status;
}

template<typename Point_t> inline
Transform::Status Affine<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                            std::vector<Point_t> *ptsOut, 
                                            Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Transform::Status Affine<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  sub_type x_aux = ptIn.x;
  try {
    if (trfOrder == Transform::Order::direct){
      ptOut->x = static_cast<sub_type>(a * x_aux + b * ptIn.y + tx);
      ptOut->y = static_cast<sub_type>(c * x_aux + d * ptIn.y + ty);
    } else {
      ptOut->x = static_cast<sub_type>(ai * x_aux + bi * ptIn.y + txi);
      ptOut->y = static_cast<sub_type>(ci * x_aux + di * ptIn.y + tyi);
    }
  } catch (std::exception &e ) {
    msgError("Affine transformation error: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Affine<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == Transform::Order::direct){
    r_pt.x = static_cast<sub_type>(a * ptIn.x + b * ptIn.y + tx);
    r_pt.y = static_cast<sub_type>(c * ptIn.x + d * ptIn.y + ty);
  }
  else {
    r_pt.x = static_cast<sub_type>(ai * ptIn.x + bi * ptIn.y + txi);
    r_pt.y = static_cast<sub_type>(ci * ptIn.x + di * ptIn.y + tyi);
  }
  return r_pt;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
void Affine<Point_t>::getParameters(double *_a, double *_b, double *_c, double *_d)
{
  *_a = a;
  *_b = b;
  *_c = c;
  *_d = d;
}

template<typename Point_t> inline
double Affine<Point_t>::getRotation() const
{
  return mRotation;
}

template<typename Point_t>
double Affine::getScaleX() const
{
  return mScaleX;
}

template<typename Point_t>
double Affine::getScaleY() const
{
  return mScaleY;
}
#endif // TL_ENABLE_DEPRECATED_METHODS

template<typename Point_t>
void Affine<Point_t>::parameters(double *a, double *b, double *c, double *d, double *x0, double *y0) const
{
  *a = this->a;
  *b = this->b;
  *c = this->c;
  *d = this->d;
  *x0 = this->tx;
  *y0 = this->ty;
}

template<typename Point_t>
math::Matrix<2, 3, double> Affine<Point_t>::parameters() const
{
  math::Matrix<2, 3, double> mat;
  mat.at(0, 0) = this->a;
  mat.at(0, 1) = this->b;
  mat.at(0, 2) = this->x0;
  mat.at(1, 0) = this->c;
  mat.at(1, 1) = this->d;
  mat.at(1, 2) = this->y0;
  return mat;
}

template<typename Point_t> inline
double Affine<Point_t>::rotation() const
{
  return mRotation;
}

template<typename Point_t>
double Affine<Point_t>::scaleX() const
{
  return mScaleX;
}

template<typename Point_t>
double Affine<Point_t>::scaleY() const
{
  return mScaleY;
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
  if (det == 0.) {
    msgError("determinant null");
  } else {
    ai = d / det;
    bi = -b / det;
    ci = -c / det;
    di = a / det;
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    this->txi = (-d * tx + b * ty) / det;
    this->tyi = (-a * ty + c * tx) / det;
    TL_ENABLE_WARNING(TL_WARNING_C4244)
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
class Projective
  : public Transform2D<Point_t>
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
  Projective();

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
  Projective(double a, double b, double c, double d, double e, double f, double g, double h);

  //~Projective();

  /*!
   * \brief Calcula la transformación proyectiva entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, 
                           const std::vector<Point_t> &pts2, 
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

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
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una transformación proyectiva a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una transformación proyectiva a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, 
                    Transform::Order trfOrder = Transform::Order::direct) const override;

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
Projective<Point_t>::Projective()
  : Transform2D<Point_t>(Transform::Type::projective, 4),
    a(1),
    b(0),
    c(0),
    d(0),
    e(1),
    f(0),
    g(0),
    h(0)
{
  update();
}

template<typename Point_t> inline
Projective<Point_t>::Projective(double a, double b, double c, double d, double e, double f, double g, double h)
  : Transform2D<Point_t>(Transform::Type::projective, 4),
    a(a),
    b(b),
    c(c),
    d(d),
    e(e),
    f(f),
    g(g),
    h(h)
{
  update();
}

template<typename Point_t> inline
Transform::Status Projective<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                              const std::vector<Point_t> &pts2,
                                              std::vector<double> *error,
                                              double *rmse)
{
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %i and size pts2 = %i", n1, n2);
    return Transform::Status::failure;
  } 
  
  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;

  size_t m = n1 * this->mDimensions, n = 8;
  double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
  double *C = new double[n];
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

    solveSVD(m, n, A, B, C);

    a = C[0];
    b = C[1];
    c = C[2];
    d = C[3];
    e = C[4];
    f = C[5];
    g = C[6];
    h = C[7];

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }

  delete[] A;
  delete[] B;
  delete[] C;

  return status;
}

template<typename Point_t> inline
Transform::Status Projective<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                                std::vector<Point_t> *ptsOut, 
                                                Transform::Order trfOrder) const
{
  Transform::Status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Transform::Status Projective<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, 
                                                Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  Point_t pt_aux = ptIn;
  try {
    if ( trfOrder == Transform::Order::direct ) {
      ptOut->x = static_cast<sub_type>((a * ptIn.x + b * ptIn.y + c) 
                                       / (g * ptIn.x + h * ptIn.y + 1));
      ptOut->y = static_cast<sub_type>((d * ptIn.x + e * ptIn.y + f) 
                                       / (g * ptIn.x + h * ptIn.y + 1));
    } else {
      ptOut->x = static_cast<sub_type>((ai * ptIn.x + bi * ptIn.y + ci) 
                                       / (g * ptIn.x + h * ptIn.y + 1));
      ptOut->y = static_cast<sub_type>((di * ptIn.x + ei * ptIn.y + fi) 
                                       / (g * ptIn.x + h * ptIn.y + 1));
    }
  } catch (std::exception &e ) {
    msgError("Error in projective transformation: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Projective<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == Transform::Order::direct){
    r_pt.x = static_cast<sub_type>((a * ptIn.x + b * ptIn.y + c) 
                                   / (g * ptIn.x + h * ptIn.y + 1));
    r_pt.y = static_cast<sub_type>((d * ptIn.x + e * ptIn.y + f) 
                                   / (g * ptIn.x + h * ptIn.y + 1));
  } else {
    r_pt.x = static_cast<sub_type>((ai * ptIn.x + bi * ptIn.y + ci) 
                                   / (g * ptIn.x + h * ptIn.y + 1));
    r_pt.y = static_cast<sub_type>((di * ptIn.x + ei * ptIn.y + fi) 
                                   / (g * ptIn.x + h * ptIn.y + 1));
  }
  return r_pt;
}

template<typename Point_t> inline
void Projective<Point_t>::setParameters(double _a, double _b, double _c, double _d, 
                                        double _e, double _f, double _g, double _h)
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
void Projective<Point_t>::getParameters(double *_a, double *_b, double *_c, double *_d, 
                                        double *_e, double *_f, double *_g, double *_h)
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
  if (aux == 0.) {
    msgError("Division by zero");
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

#ifdef HAVE_OPENCV

/*!
 * \brief Transformación perspectiva
 */
template<typename Point_t>
class TrfPerspective
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Parámetros de la transformación
   */
  cv::Mat H;

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
   */
  TrfPerspective();

  /*!
   * \brief Destructora
   */
  ~TrfPerspective() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, const std::vector<Point_t> &pts2,
                           std::vector<double> *error = nullptr, double *rmse = nullptr) override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn, Point_t *ptOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut,
                             Transform::Order trfOrder = Transform::Order::direct) const override;

};


template<typename Point_t> inline
TrfPerspective<Point_t>::TrfPerspective()
  : Transform2D<Point_t>(Transform::Type::perspective, 4)
{

}

template<typename Point_t> inline
Transform::Status TrfPerspective<Point_t>::compute(const std::vector<Point_t> &pts1,
                                                  const std::vector<Point_t> &pts2,
                                                  std::vector<double> *error,
                                                  double *rmse)
{
  //if (error) error = NULL; // Habria que poder calcular el error
  //if (rmse) rmse = NULL;

  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %i and size pts2 = %i", n1, n2);
    return Transform::Status::failure;
  }

  //TODO: chapuza...
  std::vector<cv::Point_<sub_type>> in(n1);
  std::vector<cv::Point_<sub_type>> out(n1);
  for ( int i = 0; i < n1; i++ ) {
    in[i] = pts1[i];
    out[i] = pts2[i];
  }

  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;

  try {
    H = cv::findHomography(in, out, cv::RANSAC);
    //cv::Mat H0 = cv::findHomography(pts1, pts2, cv::RANSAC);
    //cv::Mat H1 = cv::findHomography(pts1, pts2, cv::LMEDS);
    //cv::Mat H2 = cv::findHomography(pts1, pts2);
    //... determinar error
    //TL_THROW_ASSERT(!H.empty(), "Error al calcular los parámetros de la transformación");
    if (H.empty()) {
      msgError("Error al calcular los parámetros de la transformación");
      return Transform::Status::failure;
    }
    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    //msgError(e.what());
    tl::MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  return status;
}

template<typename Point_t> inline
Transform::Status TrfPerspective<Point_t>::transform(const Point_t &ptIn,
                                                    Point_t *ptOut,
                                                    Transform::Order trfOrder) const
{
  std::vector<cv::Point_<sub_type>> vIn, vOut;
  vIn.push_back(cv::Point_<sub_type>(ptIn.x, ptIn.y));
  try {
    if (trfOrder == Transform::Order::direct) {
      cv::perspectiveTransform(vIn, vOut, H);
    } else {
      cv::perspectiveTransform(vIn, vOut, H.inv());
    }
    ptOut->x = vOut[0].x;
    ptOut->y = vOut[0].y;
  } catch ( cv::Exception &e ) {
    msgError("Error in perspective transformation: %s", e.what());
    return Transform::Status::failure;
  }
  return Transform::Status::success;
}

template<typename Point_t> inline
Point_t TrfPerspective<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t out = ptIn;
  transform(ptIn, &out, trfOrder);
  return out;
}

template<typename Point_t> inline
Transform::Status TrfPerspective<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                    std::vector<Point_t> *ptsOut,
                                                    Transform::Order trfOrder) const
{
  // TODO: chapuza...
  size_t n = ptsIn.size();
  std::vector<cv::Point_<sub_type>> in(n);
  std::vector<cv::Point_<sub_type>> out;
  for ( size_t i = 0; i < n; i++ ) {
    in[i] = ptsIn[i];
  }

  try {
    if (trfOrder == Transform::Order::direct)
      cv::perspectiveTransform(in, out, H);
    else {
      cv::perspectiveTransform(in, out, H.inv());
    }
  } catch ( cv::Exception &e ) {
    msgError("Error in perspective transformation: %s",e.what());
    return Transform::Status::failure;
  }
  ptsOut->resize(n);
  for (size_t i = 0; i < n; i++) {
    (*ptsOut)[i].x = out[i].x;
    (*ptsOut)[i].y = out[i].y;
  }
  return Transform::Status::success;
}


#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Tranformación polinómica
 *
 */
template<typename Point_t>
class polynomialTransform
  : public Transform2D<Point_t>
{
public:

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
    : Transform2D<Point_t>(Transform::Type::polynomial, 2)
  {
  }

  //~Helmert2D();

  /*!
   * \brief Calcula la transformación polinómica entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, 
                           const std::vector<Point_t> &pts2, 
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación polinómica
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
  * \brief Aplica una transformación polinómica a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
  * \brief Aplica una transformación polinómica a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, Transform::Order trfOrder = Transform::Order::direct) const override;

};

template<typename Point_t> inline
Transform::Status polynomialTransform<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                                       const std::vector<Point_t> &pts2, 
                                                       std::vector<double> *error,
                                                       double *rmse)
{
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %i and size pts2 = %i", n1, n2);
    return Transform::Status::failure;
  } 
  
  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;

  double xmin = TL_DOUBLE_MAX;
  double ymin = TL_DOUBLE_MAX;
  double xmax = TL_DOUBLE_MIN;
  double ymax = TL_DOUBLE_MIN;
  for( int ipt = 0; ipt < n1; ipt++ ) {
    Point_t &ptoi = pts1[ipt];
    if ( ptoi.x > xmax ) xmax = ptoi.x;
    if ( ptoi.x < xmin ) xmin = ptoi.x;
    if ( ptoi.y > ymax ) ymax = ptoi.y;
    if ( ptoi.y < ymin ) ymin = ptoi.y;
  }
  double xc = (xmax + xmin) / 2.;
  double yc = (ymax + ymin) / 2.;

  //...
  

  if (error) {
    if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
  }  
  
  return status;
}

template<typename Point_t> inline
Transform::Status polynomialTransform<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                                         std::vector<Point_t> *ptsOut, 
                                                         Transform::Order trfOrder) const
{
  Transform::Status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Transform::Status polynomialTransform<Point_t>::transform(const Point_t &ptIn, Point_t *out, 
                                                         Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  sub_type x_aux = ptIn.x;
  try {
    //...
  } catch (std::exception &e ) {
    msgError("Polynomial transform error: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t polynomialTransform<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t r_pt;
  
  //...
  
  return r_pt;
}


/* ---------------------------------------------------------------------------------- */



/*! \} */ // end of trf2DGroup




/*! \defgroup trf3DGroup Transformaciones 3D
 *  Transformaciones geométricas en el espacio
 *  \{
 */


/* ---------------------------------------------------------------------------------- */


template<typename Point_t>
class Transform3D
  : public TransformBase<Point_t>
{

public:

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
  Transform3D(Transform::Type trfType, int n_min = 0)
    : TransformBase<Point_t>(trfType, n_min)
  {
    this->mDimensions = 3;
  }

  /*!
   * \brief Destructora
   */
  ~Transform3D() override = default;

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  virtual Transform::Status compute(const std::vector<Point_t> &pts1, 
                                   const std::vector<Point_t> &pts2, 
                                   std::vector<double> *error = nullptr,
                                   double *rmse = nullptr) override = 0;

  /*!
   * \brief Aplica la transformación
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, 
                                     Transform::Order trfOrder = Transform::Order::direct) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                                     Transform::Order trfOrder = Transform::Order::direct) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  virtual Point_t transform(const Point_t &ptIn, Transform::Order trfOrder = Transform::Order::direct) const override = 0;

};

//template<typename Point_t> inline
//void Transform3D<Point_t>::transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, Transform::Order trfOrder = Transform::Order::direct) const
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
class Helmert3D
  : public Transform3D<Point_t>
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

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<Point_t> o CV::Point_<Point_t>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

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
  //math::RotationMatrix<double> mR;

  /*!
   * \brief Matriz de rotación inversa
   */
  std::array<std::array<double, 3>, 3> mRinv;
  //math::RotationMatrix<double> mRinv;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert3D();

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
  Helmert3D(double tx, double ty, double tz, double scale, double omega, double phi, double kappa);

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] rotation Matriz de rotación
   */
  Helmert3D(double tx, double ty, double tz, double scale, const std::array<std::array<double, 3>, 3> &rotation);
  //Helmert3D(double tx, double ty, double tz, double scale, const math::RotationMatrix<double> &rotation);

  /*!
   * \brief Constructor de copia Helmert3D
   * \param[in] trf Objeto Helmert3D que se copia
   */
  Helmert3D(const Helmert3D &trf);


  ~Helmert3D() override {}

  /*!
   * \brief Calcula la transformación Helmert 3D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point_t> &pts1, 
                           const std::vector<Point_t> &pts2, 
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

  /*!
   * \brief Devuelve una referencia a la matriz de rotación
   */
  const std::array<std::array<double, 3>, 3> &getRotationMatrix() const;
  //const RotationMatrix<double> &rotationMatrix() const;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 3D
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica un helmert 3D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const Point_t &ptIn, Point_t *ptOut, 
                             Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica un helmert 3D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, Transform::Order trfOrder = Transform::Order::direct) const override;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  TL_DEPRECATED("scale()", "2.0")
  double getScale() const { return mScale; }
#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double scale() const { return mScale; }

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] scale Escala de la transformación
   */
  void setScale(double scale);

  double omega() const;
  void setOmega(double omega);

  double phi() const;
  void setPhi(double phi);

  double kappa() const;
  void setKappa(double kappa);

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


private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};



template<typename Point_t> inline
Helmert3D<Point_t>::Helmert3D()
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(0.),
    ty(0.),
    tz(0.),
    mScale(1.),
    mOmega(0.),
    mPhi(0.),
    mKappa(0.)
{
  update();
}

template<typename Point_t> inline
Helmert3D<Point_t>::Helmert3D(double tx, double ty, double tz, double scale, double omega, double phi, double kappa)
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(tx),
    ty(ty),
    tz(tz),
    mScale(scale),
    mOmega(omega),
    mPhi(phi),
    mKappa(kappa)
{
  update();
}

template<typename Point_t> inline
Helmert3D<Point_t>::Helmert3D(double tx, double ty, double tz, double scale, const std::array<std::array<double, 3>, 3> &rotation)
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(tx),
    ty(ty),
    tz(tz),
    mScale(scale),
    mR(rotation)
{
  eulerAngles(mR, &mOmega, &mPhi, &mKappa);
  update();
}

//template<typename Point_t> inline
//Helmert3D<Point_t>::Helmert3D(double tx, double ty, double tz, double scale, const math::RotationMatrix<double> &rotation)
//  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
//    tx(tx),
//    ty(ty),
//    tz(tz),
//    mScale(scale),
//    mR(rotation)
//{
//  eulerAngles(mR, &mOmega, &mPhi, &mKappa);
//  update();
//}

template<typename Point_t>
Helmert3D<Point_t>::Helmert3D(const Helmert3D &trf)
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(trf.tx),
    ty(trf.ty),
    tz(trf.tz),
    mScale(trf.mScale),
    mOmega(trf.mOmega),
    mPhi(trf.mPhi),
    mKappa(trf.mKappa),
    mR(trf.mR),
    mRinv(trf.mRinv)
{

}

template<typename Point_t> inline
Transform::Status Helmert3D<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                             const std::vector<Point_t> &pts2,
                                             std::vector<double> *error,
                                             double *rmse)
{
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %i and size pts2 = %i", n1, n2);
    return Transform::Status::failure;
  } 
  
  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %i < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;

  size_t m = n1 * this->mDimensions, n = 7;
  double *A = new double[m*n], *pa = A, *L = new double[m], *pl = L;
  double *C = new double[n];
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

    solveSVD(m, n, A, L, C);

    mScale = C[0];
    mOmega = C[1];
    mPhi = C[2];
    mKappa = C[3];
    tx = C[4];
    ty = C[5];
    tz = C[6];

    if (error) {
      ///TODO: este metodo sólo contempla puntos en 2d....
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }
    
    //rmse = _rootMeanSquareError(pts1, pts2, error);
  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }

  delete[] A;
  delete[] L;
  delete[] C;

  return status;
}

template<typename Point_t> inline
const std::array<std::array<double, 3>, 3> &Helmert3D<Point_t>::getRotationMatrix() const 
{
  return mR;
}

//template<typename Point_t> inline
//const RotationMatrix<double> &Helmert3D<Point_t>::rotationMatrix() const
//{
//  return mR;
//}

template<typename Point_t> inline
Transform::Status Helmert3D<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                               std::vector<Point_t> *ptsOut, 
                                               Transform::Order trfOrder) const
{
  Transform::Status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Transform::Status Helmert3D<Point_t>::transform(const Point_t &ptIn, Point_t *ptOut, Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  Point_t ptAux = ptIn;
  try {
    if (trfOrder == Transform::Order::direct){
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
    msgError("Error in Helmert 3D transformation: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Helmert3D<Point_t>::transform(const Point_t &ptIn, Transform::Order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == Transform::Order::direct){
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
double Helmert3D<Point_t>::omega() const
{
return mOmega;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setOmega(double omega)
{
  mOmega = omega;
  update();
}

template<typename Point_t> inline
double Helmert3D<Point_t>::phi() const
{
return mPhi;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setPhi(double phi)
{
  mPhi = phi;
  update();
}

template<typename Point_t> inline
double Helmert3D<Point_t>::kappa() const
{
return mKappa;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setKappa(double kappa)
{
  mKappa = kappa;
  update();
}

template<typename Point_t> inline
void Helmert3D<Point_t>::update()
{
  rotationMatrix(mOmega, mPhi, mKappa, &mR);
#ifdef HAVE_OPENCV
  cv::Mat inv = cv::Mat(3, 3, CV_64F, mR.data()).inv();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mRinv[i][j] = inv.at<double>(i, j);
    }
  }
#elif HAVE_EIGEN
  Eigen::Map<Matrix<double, 3, 3, RowMajor> > mat(mR.data());
  mat = mat.inverse();
#endif
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


TL_TODO("Marcar los métodos 'transform' como obsoletos y usar std::transform con iteradores")

/*!
 * \brief Aplica una transformación a una ventana
 * \param[in] in Ventana de entrada
 * \param[out] out Ventana de salida
 * \param[in] trf Transformación que se aplica
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::direct
 */
template<typename Point_t> inline
void transform(const Window<Point_t> &in, Window<Point_t> *out,
                          TransformBase<Point_t> *trf, Transform::Order trfOrder = Transform::Order::direct)
{
  Point_t pt1;
  Point_t pt2;
  trf->transform(in.pt1, &pt1, trfOrder);
  trf->transform(in.pt2, &pt2, trfOrder);
  *out = Window<Point_t>(pt1, pt2);
}



/*!
 * \brief Aplica una transformación a un segmento
 * \param[in] in Segmento de entrada
 * \param[out] out Segmento de salida
 * \param[in] trf Transformación que se aplica
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::direct
 */
template<typename Point_t> inline
void transform(const Segment<Point_t> &in, Segment<Point_t> *out, 
                          TransformBase<Point_t> *trf, Transform::Order trfOrder = Transform::Order::direct)
{
  trf->transform(in.pt1, &out->pt1, trfOrder);
  trf->transform(in.pt2, &out->pt2, trfOrder);
}

/*!
 * \brief Aplica una transformación a un contenedor de entidades de los tipos Polygon, LinesString, etc
 * \param[in] ptsIn Contenedor de entrada
 * \param[out] ptsOut Contenedor de salida
 * \param[in] trf Transformación que se aplica
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::direct
 */
template<typename Point_t> inline
Transform::Status transform(const EntityContainer<Point_t> &ptsIn, EntityContainer<Point_t> *ptsOut,
               TransformBase<Point_t> *trf, Transform::Order trfOrder = Transform::Order::direct)
{
  Transform::Status r_status = Transform::Status::success;
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = trf->transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

/*!
 * \brief Aplica una transformación a un conjunto de entidades
 * \param[in] in Entidad de entrada
 * \param[out] out Entidad de salida
 * \param[in] trf Transformación que se aplica a la entidad
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::direct
 */
template<typename Entity_t, typename Point_t> inline
void transform(const std::vector<Entity_t> &in, std::vector<Entity_t> *out, 
                          TransformBase<Point_t> *trf, Transform::Order trfOrder = Transform::Order::direct)
{
  out->resize(in.size());
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trf, trfOrder);
  }
}

/*!
 * \brief Aplica una transformación en paralelo a un conjunto de entidades
 * \param[in] in Entidad de entrada
 * \param[out] out Entidad de salida
 * \param[in] trf Transformación que se aplica a la entidad
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::direct
 */
template<typename Entity_t, typename Point_t> inline
void transformParalell(const std::vector<Entity_t> &in, std::vector<Entity_t> *out, 
                                  TransformBase<Point_t> *trf, Transform::Order trfOrder = Transform::Order::direct)
{
  parallel_for(0, static_cast<int>(in.size()), [&](int i) {
    transform(in[i], &(*out)[i], trf, trfOrder);
  });
}

/*!
 * \brief Aplica una transformación a un conjunto de entidades
 * \param[in] in_first Iterador al primer elemento de entrada
 * \param[in] in_last  Iterador al último elemento de entrada
 * \param[out] out_first Iterador al primer elemento de salida
 * \param[in] trf Transformación que se aplica a la entidad
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::direct
 */
template<typename itIn, typename itOut, typename trf_t> inline
void transform(itIn in_first, itIn in_last, itOut out_first, trf_t *trf, Transform::Order trfOrder = Transform::Order::direct)
{
  while (in_first != in_last) {
    trf->transform(*in_first++, &(*out_first++), trfOrder);
  }
}


/*! \} */ // end of trfGroup


} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_H
