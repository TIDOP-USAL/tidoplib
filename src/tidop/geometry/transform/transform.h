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


#include <vector>
#include <memory>

#include "tidop/core/defs.h"

namespace tl
{

/*! \defgroup trfGroup Transformaciones
 *  Transformaciones geométricas
 *  \{
 */


template<typename Point_t> class Helmert2D;
template<typename Point_t> class Rotation;
template<typename Point_t> class Affine;
template<typename Point_t> class Translation;

/*!
 * \brief Interfaz Transform
 */
class TL_EXPORT Transform
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
    reflection,      /*!< Reflexión */
    shear,           /*!< Mapeo de corte, transformación de corte o transvección*/
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
  virtual Transform::Status transform(const Point_t &ptIn, 
                                      Point_t &ptOut, 
                                      Order trfOrder = Order::direct) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto transformado
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
   * \brief Aplica la transformación a un vector de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transform(const std::vector<Point_t> &ptsIn,
                                      std::vector<Point_t> &ptsOut,
                                      Transform::Order trfOrder = Transform::Order::direct) const = 0;

  /*!
   * \brief Aplica la transformación a un vector de puntos aplicando paralelismo
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transformParallel(const std::vector<Point_t> &ptsIn,
                                             std::vector<Point_t> &ptsOut,
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
                       std::vector<T2> &ptsOut) const;

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

public:

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


/// Implementación TransformBase

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
                                                            std::vector<Point_t> &ptsOut,
                                                            Transform::Order trfOrder) const
{
  formatVectorOut(ptsIn, ptsOut);

  Transform::Status r_status;

  parallel_for(0, ptsIn.size(), [&](size_t i) {
    r_status = transform(ptsIn[i], ptsOut[i], trfOrder);
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
      transform(ptsIn[i], pts_out[i]);
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
                                             std::vector<T2> &ptsOut) const 
{
  if (ptsIn != ptsOut && ptsIn.size() != ptsOut.size()) {
    ptsOut.clear();
    ptsOut.resize(ptsIn.size());
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
    transform(ptsIn[i], pts_out[i]);
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
class TransformMultiple
  : public TransformBase<Point_t>
{

public:

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
  TransformMultiple();

  /*!
   * \brief Constructor de lista
   * \param[in] transfList listado de transformaciones
   */
  TransformMultiple(std::initializer_list<std::shared_ptr<TransformBase<Point_t>>> transfList);

  /*!
   * \brief Destructora
   */
  virtual ~TransformMultiple() override {}

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

  bool isNumberOfPointsValid(size_t npoints) const override;

// TransformBase Interfaz

  Transform::Status compute(const std::vector<Point_t> &pts1, 
                            const std::vector<Point_t> &pts2, 
                            std::vector<double> *error = nullptr,
                            double *rmse = nullptr) override;
  Transform::Status transform(const Point_t &ptIn, 
                              Point_t &ptOut, 
                              Transform::Order trfOrder = Transform::Order::direct) const override;
  Point_t transform(const Point_t &ptIn, 
                    Transform::Order trfOrder = Transform::Order::direct) const override;
  Transform::Status transform(const std::vector<Point_t> &ptsIn, 
                              std::vector<Point_t> &ptsOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

private:

  /*!
   * \brief Lista de transformaciones
   */
  std::list<std::shared_ptr<TransformBase<Point_t>>> mTransformationList;

};


/// Implementación TransformMultiple

template<typename Point_t>
TransformMultiple<Point_t>::TransformMultiple()
  : TransformBase<Point_t>(Type::multiple)
{
}

template<typename Point_t>
TransformMultiple<Point_t>::TransformMultiple(std::initializer_list<std::shared_ptr<TransformBase<Point_t> > > transformations)
  : TransformBase<Point_t>(Transform::Type::multiple),
    mTransformationList(transformations)
{
}

template<typename Point_t>
typename TransformMultiple<Point_t>::reference TransformMultiple<Point_t>::front()
{
  return mTransformationList.front();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::const_reference TransformMultiple<Point_t>::front() const
{
  return mTransformationList.front();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::reference TransformMultiple<Point_t>::back()
{
  return mTransformationList.back();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::const_reference TransformMultiple<Point_t>::back() const
{
  return mTransformationList.back();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::iterator TransformMultiple<Point_t>::begin() TL_NOEXCEPT
{
  return mTransformationList.begin();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::const_iterator TransformMultiple<Point_t>::begin() const TL_NOEXCEPT
{
  return mTransformationList.cbegin();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::iterator TransformMultiple<Point_t>::end() TL_NOEXCEPT
{
  return mTransformationList.end();
}

template<typename Point_t>
typename TransformMultiple<Point_t>::const_iterator TransformMultiple<Point_t>::end() const TL_NOEXCEPT
{
  return mTransformationList.end();
}

template<typename Point_t>
void TransformMultiple<Point_t>::push_back(const std::shared_ptr<TransformBase<Point_t>> &trf)
{
  mTransformationList.push_back(trf);
}

template<typename Point_t>
void TransformMultiple<Point_t>::push_back(std::shared_ptr<TransformBase<Point_t>> &&trf) TL_NOEXCEPT
{
  mTransformationList.push_back(std::forward<std::shared_ptr<TransformBase<Point_t>>>(trf));
}

template<typename Point_t>
void TransformMultiple<Point_t>::clear() TL_NOEXCEPT
{
  mTransformationList.clear();
}

template<typename Point_t>
bool TransformMultiple<Point_t>::empty() const TL_NOEXCEPT
{
  return mTransformationList.empty();
}

template<typename Point_t>
size_t TransformMultiple<Point_t>::size() const
{
  return mTransformationList.size();
}

template<typename Point_t>
void TransformMultiple<Point_t>::del(int id)
{
  mTransformationList.erase(mTransformationList.begin() + id);
}

template<typename Point_t> inline
bool TransformMultiple<Point_t>::isNumberOfPointsValid(size_t npoints) const
{ 
  msgError("'isNumberOfPointsValid' is not supported for TransformMultiple");
  TL_COMPILER_WARNING("'isNumberOfPointsValid' is not supported for TransformMultiple");
  return true;
}

template<typename Point_t> inline
Transform::Status TransformMultiple<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                                      const std::vector<Point_t> &pts2, 
                                                      std::vector<double> *error, 
                                                      double *rmse)
{
  msgError("'compute' is not supported for TransformMultiple");
  TL_COMPILER_WARNING("'compute' is not supported for TransformMultiple");
  return Transform::Status::failure;
}

template<typename Point_t> inline
Transform::Status TransformMultiple<Point_t>::transform(const Point_t &ptIn, 
                                                        Point_t &ptOut, 
                                                        Transform::Order trfOrder) const
{
  ptOut = ptIn;

  Transform::Status r_status;

  for (const auto &transformation : mTransformationList) {
    r_status = transformation->transform(ptOut, ptOut, trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }

  return r_status;
}

template<typename Point_t> inline
Point_t TransformMultiple<Point_t>::transform(const Point_t &ptIn, 
                                              Transform::Order trfOrder) const
{
  Point_t out = ptIn;

  for (const auto &trf : transformation) {
    out = transformation->transform(out, trfOrder);
  }

  return out;
}

template<typename Point_t> inline
Transform::Status TransformMultiple<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                        std::vector<Point_t> &ptsOut,
                                                        Transform::Order trfOrder) const
{
  ptsOut = ptsIn;

  Transform::Status r_status;

  for (const auto &trf : transformation) {
    r_status = transformation->transform(ptsOut, ptsOut, trfOrder);
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
  ~Transform2D() override = default;

};


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
  virtual Transform::Status transform(const std::vector<Point_t> &ptsIn, 
                                      std::vector<Point_t> &ptsOut, 
                                      Transform::Order trfOrder = Transform::Order::direct) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  virtual Transform::Status transform(const Point_t &ptIn, 
                                      Point_t &ptOut, 
                                      Transform::Order trfOrder = Transform::Order::direct) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  virtual Point_t transform(const Point_t &ptIn, 
                            Transform::Order trfOrder = Transform::Order::direct) const override = 0;

};


/*! \} */ // end of trfGroup


} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_H
