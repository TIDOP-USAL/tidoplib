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


#ifndef TL_GEOM_CURVES_H
#define TL_GEOM_CURVES_H

#include "config_tl.h"

#include <limits>
#include <numeric>

#include "tidop/core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/types.hpp"
#endif // HAVE_OPENCV

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace TL
{

/*! \addtogroup GeometricEntities
 *  \{
 */


namespace geometry
{

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase circulo
 *
 */
template<typename T>
class Circle
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Centro
   */
  Point<T> center;

  /*!
   * \brief Radio
   */
  T radius;

public:

  /*!
   * \brief Constructora por defecto
   */
  Circle();

  /*!
   * \brief Constructor
   * \param[in] center Centro
   * \param[in] radius Radio
   */
  Circle(const Point<T> &center, T radius);

  /*!
   * \brief Constructor de copia
   * \param[in] circle Objeto circulo que se copia
   */
  Circle(const Circle<T> &circle);
   
  /*!
   * \brief Constructor de movimiento
   * \param[in] circle Objeto circulo que se mueve
   */
  Circle(Circle<T> &&circle) TL_NOEXCEPT;

  /*!
   * \brief Operador de asignación
   * \param[in] circle Objeto Point que se copia
   */
  Circle<T> &operator = (const Circle<T> &circle);
    
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] circle Objeto Point que se mueve
   */
  Circle<T>& operator = (Circle<T> &&circle) TL_NOEXCEPT;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Circle<T2>() const;

  /*!
   * \brief Area de un circulo
   * \return Area
   */
  double area() const;

  /*!
   * \brief Longitud o circunferencia del circulo
   * \return longitud
   */
  double length() const;

};

typedef Circle<int> CircleI;
typedef Circle<double> CircleD;
typedef Circle<float> CircleF;


template<typename T> inline
Circle<T>::Circle()
  : Entity(Entity::Type::CIRCLE),
    center(Point<T>()),
    radius(static_cast<T>(1))
{
}

template<typename T> inline
Circle<T>::Circle(const Point<T> &center, T radius)
  : Entity(Entity::Type::CIRCLE),
    center(center),
    radius(radius)
{
}

template<typename T> inline
Circle<T>::Circle(const Circle<T> &circle)
  : Entity(Entity::Type::CIRCLE),
    center(circle.center),
    radius(circle.radius)
{
}

template<typename T> inline
Circle<T>::Circle(Circle<T> &&circle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(circle)),
    center(std::move(circle.center)),
    radius(std::move(circle.radius))
{
}

template<typename T> inline
Circle<T> &Circle<T>::operator = (const Circle &circle)
{
  if (this != &circle) {
    this->mEntityType = circle.mEntityType;
    this->center = circle.center;
    this->radius = circle.radius;
  }
  return *this;
}

template<typename T> inline
Circle<T> &Circle<T>::operator = (Circle &&circle) TL_NOEXCEPT
{
  if (this != &circle) {
    this->mEntityType = std::move(circle.mEntityType);
    this->center = std::move(circle.center);
    this->radius = std::move(circle.radius);
  }
  return *this;
}

template<typename T> template<typename T2> inline
Circle<T>::operator Circle<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Circle<T2>(static_cast<Point<T2>>(this->center), 
                      static_cast<T2>(std::round(this->radius)));
  } else {
    return Circle<T2>(Point<T2>(center), static_cast<T2>(this->radius));
  }
}

template<typename T> inline
double Circle<T>::area() const
{
  return TL_PI * radius * radius;
}

template<typename T> inline
double Circle<T>::length() const
{
  return TL_2PI * radius;
}


/*!
 * \brief Clase Ellipse
 *
 */
template<typename T>
class Ellipse
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Centro
   */
  Point<T> center;

  /*!
   * \brief Semieje mayor
   */
  T a;

  /*!
   * \brief Semieje menor
   */
  T b;

public:

  /*!
   * \brief Constructora por defecto
   */
  Ellipse();

  /*!
   * \brief Constructor
   * \param[in] center Centro de la elipse
   * \param[in] a Semieje mayor
   * \param[in] b Semieje menor
   */
  Ellipse(const Point<T> &center, T a, T b);

  /*!
   * \brief Constructor de copia
   * \param[in] ellipse Objeto circulo que se copia
   */
  Ellipse(const Ellipse<T> &ellipse);
   
  /*!
   * \brief Constructor de movimiento
   * \param[in] ellipse Objeto circulo que se mueve
   */
  Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT;

  /*!
   * \brief Operador de asignación
   * \param[in] ellipse Objeto Elipse que se copia
   */
  Ellipse<T> &operator = (const Ellipse<T> &ellipse);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] ellipse Objeto Elipse que se mueve
   */
  Ellipse<T> &operator = (Ellipse<T> &&ellipse) TL_NOEXCEPT;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Ellipse<T2>() const;

  /*!
   * \brief Area de una elipse
   * \return Area
   */
  double area() const;

  /*!
   * \brief Longitud o circunferencia de la elipse
   * \return longitud
   */
  double length() const;

};

typedef Ellipse<int> EllipseI;
typedef Ellipse<double> EllipseD;
typedef Ellipse<float> EllipseF;


template<typename T> inline
Ellipse<T>::Ellipse()
  : Entity(Entity::Type::ELLIPSE),
    center(Point<T>()),
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T> inline
Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
  : Entity(Entity::Type::ELLIPSE),
    center(center),
    a(a),
    b(b)
{
  if (b > a) {
    T aux = b;
    b = a;
    a = aux;
  }
}

template<typename T> inline
Ellipse<T>::Ellipse(const Ellipse<T> &ellipse)
  : Entity(ellipse),
    center(ellipse.center),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T> inline
Ellipse<T>::Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT
  : Entity(std::forward<Entity>(ellipse)),
    center(ellipse.center),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T> inline
Ellipse<T> &Ellipse<T>::operator = (const Ellipse &ellipse)
{
  if (this != &ellipse) {
    this->mEntityType = ellipse.mEntityType;
    this->center = ellipse.center;
    this->a = ellipse.a;
    this->b = ellipse.b;
  }
  return *this;
}

template<typename T> inline
Ellipse<T> &Ellipse<T>::operator = (Ellipse &&ellipse) TL_NOEXCEPT
{
  if (this != &ellipse) {
    this->mEntityType = std::move(ellipse.mEntityType);
    this->center = std::move(ellipse.center);
    this->a = std::move(ellipse.a);
    this->b = std::move(ellipse.b);
  }
  return *this;
}

template<typename T> template<typename T2> inline
Ellipse<T>::operator Ellipse<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Ellipse<T2>(static_cast<Point<T2>>(this->center), 
                       static_cast<T2>(std::round(this->a)), 
                       static_cast<T2>(std::round(this->b)));
  } else {
    return Ellipse<T2>(static_cast<Point<T2>>(this->center), 
                       static_cast<T2>(this->a), 
                       static_cast<T2>(this->b));
  }
}

template<typename T> inline
double Ellipse<T>::area() const
{
  return TL_PI * a * b;
}

template<typename T> inline
double Ellipse<T>::length() const
{
  return TL_PI * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
}
 
} // End namespace geometry

/*! \} */ // end of GeometricEntities

} // End namespace TL

#endif // TL_GEOM_CURVES_H
