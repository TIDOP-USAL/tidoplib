/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOMETRY_SHAPES_H
#define TL_GEOMETRY_SHAPES_H

#include "config_tl.h"

#include <limits>
#include <numeric>

#include "tidop/core/defs.h"

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

class Shape
{

public:

  Shape() = default;
  virtual ~Shape() = default;

  virtual double area() const = 0;
};


/*!
 * \brief Clase circulo
 *
 */
template<typename T>
class Circle
  : public Entity,
    public Shape
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

  ~Circle() override = default;

  /*!
   * \brief Operador de asignación
   * \param[in] circle Objeto Circle que se copia
   */
  Circle<T> &operator = (const Circle<T> &circle);
    
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] circle Objeto Circle que se mueve
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
  double area() const override;

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
  : Entity(Entity::Type::circle),
    center(Point<T>()),
    radius(static_cast<T>(1))
{
}

template<typename T> inline
Circle<T>::Circle(const Point<T> &center, T radius)
  : Entity(Entity::Type::circle),
    center(center),
    radius(radius)
{
}

template<typename T> inline
Circle<T>::Circle(const Circle<T> &circle)
  : Entity(Entity::Type::circle),
    center(circle.center),
    radius(circle.radius)
{
}

template<typename T> inline
Circle<T>::Circle(Circle<T> &&circle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(circle)),
    center(std::move(circle.center)),
    radius(circle.radius)
{
}

template<typename T> inline
Circle<T> &Circle<T>::operator = (const Circle &circle)
{
  if (this != &circle) {
    Entity::operator = (circle);
    this->center = circle.center;
    this->radius = circle.radius;
  }
  return *this;
}

template<typename T> inline
Circle<T> &Circle<T>::operator = (Circle &&circle) TL_NOEXCEPT
{
  if (this != &circle) {
    Entity::operator = (std::forward<Entity>(circle));
    this->center = std::move(circle.center);
    this->radius = circle.radius;
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
  return math::consts::pi<double> * radius * radius;
}

template<typename T> inline
double Circle<T>::length() const
{
  return math::consts::two_pi<double> * radius;
}


/*!
 * \brief Clase Ellipse
 *
 */
template<typename T>
class Ellipse
  : public Entity,
    public Shape
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

  TL_TODO("angulo")

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
   * \param[in] ellipse Objeto Ellipse que se copia
   */
  Ellipse(const Ellipse<T> &ellipse);
   
  /*!
   * \brief Constructor de movimiento
   * \param[in] ellipse Objeto Ellipse que se mueve
   */
  Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT;

  ~Ellipse() override = default;

  /*!
   * \brief Operador de asignación
   * \param[in] ellipse Objeto Ellipse que se copia
   */
  Ellipse<T> &operator = (const Ellipse<T> &ellipse);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] ellipse Objeto Ellipse que se mueve
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
  double area() const override;

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
  : Entity(Entity::Type::ellipse),
    center(Point<T>()),
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T> inline
Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
  : Entity(Entity::Type::ellipse),
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
    center(std::move(ellipse.center)),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T> inline
Ellipse<T> &Ellipse<T>::operator = (const Ellipse &ellipse)
{
  if (this != &ellipse) {
    Entity::operator = (ellipse);
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
    Entity::operator = (std::forward<Entity>(ellipse));
    this->center = std::move(ellipse.center);
    this->a = ellipse.a;
    this->b = ellipse.b;
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
  return math::consts::pi<double> * a * b;
}

template<typename T> inline
double Ellipse<T>::length() const
{
  return math::consts::pi<double> * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
}


/*!
 * \brief Clase triangulo
 *
 */
template<typename T>
class Triangle
  : public Entity,
    public Shape
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Triangle();

  Triangle(const Point<T> &pt1, const Point<T> &pt2, const Point<T> &pt3);

  /*!
   * \brief Constructor de copia
   * \param[in] triangle Objeto Triangle que se copia
   */
  Triangle(const Triangle<T> &triangle);
   
  /*!
   * \brief Constructor de movimiento
   * \param[in] triangle Objeto Triangle que se mueve
   */
  Triangle(Triangle<T> &&triangle) TL_NOEXCEPT;

  ~Triangle() override = default;

  /*!
   * \brief Operador de asignación
   * \param[in] triangle Objeto Triangle que se copia
   */
  Triangle<T> &operator = (const Triangle<T> &circle);
    
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] triangle Objeto Triangle que se mueve
   */
  Triangle<T> &operator = (Triangle<T> &&triangle) TL_NOEXCEPT;

  /*!
   * \brief Area de un circulo
   * \return Area
   */
  double area() const override;

protected:

  Point<T> mPt1;
  Point<T> mPt2;
  Point<T> mPt3;

};


template<typename T> inline
Triangle<T>::Triangle()
  : Entity(Entity::Type::triangle),
    mPt1(),
    mPt2(),
    mPt3()
{
}

template<typename T> inline 
Triangle<T>::Triangle(const Point<T> &pt1, 
                      const Point<T> &pt2, 
                      const Point<T> &pt3)
  : Entity(Entity::Type::triangle),
    mPt1(pt1),
    mPt2(pt2),
    mPt3(pt3)
{
}

template<typename T> inline
Triangle<T>::Triangle(const Triangle<T> &triangle)
  : Entity(triangle),
    mPt1(triangle.pt1),
    mPt2(triangle.pt2),
    mPt3(triangle.pt3)
{
}

template<typename T> inline
Triangle<T>::Triangle(Triangle<T> &&triangle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(triangle)),
    mPt1(std::move(triangle.mPt1)),
    mPt2(std::move(triangle.mPt2)),
    mPt3(std::move(triangle.mPt3))
{
}

template<typename T> inline
Triangle<T> &Triangle<T>::operator = (const Triangle &triangle)
{
  if (this != &triangle) {
    Entity::operator = (triangle);
    this->mPt1 = triangle.mPt1;
    this->mPt2 = triangle.mPt2;
    this->mPt3 = triangle.mPt3;
  }
  return *this;
}

template<typename T> inline
Triangle<T> &Triangle<T>::operator = (Triangle &&triangle) TL_NOEXCEPT
{
  if (this != &triangle) {
    Entity::operator = (std::forward<Entity>(triangle));
    this->mPt1 = std::move(triangle.mPt1);
    this->mPt2 = std::move(triangle.mPt2);
    this->mPt3 = std::move(triangle.mPt3);
  }
  return *this;
}

template<typename T> inline
double Triangle<T>::area() const
{
  return 0;
}


/*!
 * \brief Clase cuadrado
 *
 */
template<typename T>
class Square
  : public Entity,
    public Shape
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Square();

  /*!
   * \brief Area
   * \return Area
   */
  double area() const override;

};


template<typename T> inline
Square<T>::Square()
{
}


template<typename T> inline
double Square<T>::area() const
{
  return 0;
}


/*!
 * \brief Clase rectangulo
 *
 */
template<typename T>
class Rectangle
  : public Entity,
    public Shape
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Rectangle();

  /*!
   * \brief Area
   * \return Area
   */
  double area() const override;

};


template<typename T> inline
Rectangle<T>::Rectangle()
{
}


template<typename T> inline
double Rectangle<T>::area() const
{
  return 0;
}


/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_SHAPES_H
