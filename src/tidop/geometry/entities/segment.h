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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOMETRY_SEGMENT_H
#define TL_GEOMETRY_SEGMENT_H

#include <utility>

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/math/mathutils.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/geometry/algorithms.h"
//#include "tidop/math/algebra/vectors.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Clase segmento 2D
 *
 * Esta template representa un segmento recto entre dos puntos independientemente de
 * que sean int, double o float.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Segment<Point<int>> SegmentI;
 * typedef Segment<Point<double>> SegmentD;
 * typedef Segment<Point<float>> SegmentF;
 * typedef SegmentI Line;
 * \endcode
 */
template<typename Point_t>
class Segment
  : public Entity
{

public:

  /*!
   * \brief type Tipo de punto
   */
  using value_type = Point_t;

  /*!
   * \brief Punto inicial del segmento
   */
  Point_t pt1;

  /*!
   * \brief Punto final del segmento
   */
  Point_t pt2;

public:

  /*!
   * \brief Constructora por defecto
   */
  Segment();

  /*!
   * \brief Constructor de copia
   */
  Segment(const Segment &segment);
    
  /*!
   * \brief Constructor de movimiento
   */
  Segment(Segment &&segment) TL_NOEXCEPT;

  /*!
   * \brief Constructor segment
   * \param[in] _pt1 Punto inicial del segmento
   * \param[in] _pt2 Punto final del segmento
   */
  Segment(Point_t _pt1, Point_t _pt2);

  /*!
   * \brief Constructor segment
   * \param[in] pt Coordenadas del punto central o inicial (según el valor de bCenter)
   * \param[in] angle Ángulo de la recta
   * \param[in] length Longitud del segmento
   * \param[in] bCenter Si es verdadero es el centro de la línea
   */
  Segment(const Point_t &pt, double angle, double length, bool bCenter = true);

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia al segmento
   */
  Segment &operator = (const Segment &segment);
    
  /*!
   * \brief Sobrecarga del operador de asignación de movimiento
   * \param[in] segment Segmento que se mueve
   * \return Referencia al segmento
   */
  Segment &operator = (Segment &&segment) TL_NOEXCEPT;

  ~Segment() override = default;

  /*!
   * \brief Conversión a un segmento de un tipo diferente
   */
  template<typename Point_t2> operator Segment<Point_t2>() const;

  /*!
   * \brief Angulo medido respecto al eje x.
   * \return Ángulo en radianes
   */
  double angleOX() const;

  /*!
   * \brief Angulo medido respecto al eje y.
   * \return Ángulo en radianes
   */
  double angleOY() const;

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente del segmento
   */
  Window<Point_t> window() const;

  /*!
   * \brief Comprueba si el segmento esta vacio pt1 = (0, 0) ; pt2 = (0, 0)
   * \return Verdadero si esta vacio
   */
  bool empty() const;

  /*!
   * \brief Comprueba si dos segmentos están próximos
   * \param[in] l2 Segmento con el que se compara
   * \param[in] dist Distancia máxima de separación
   * \return Verdadero si esta a menos distancia de la indicada.
   */
  bool isNear(const Segment<Point_t> &l2, double dist = 10.) const;

  /*!
   * \brief Comprueba si es paralelo a otro segmento
   * \param[in] l2 Segmento con el que se compara
   * \param[in] tol Toleracia angular. Si es 0 los segmentos tienen que ser paralelos.
   *  Si tiene otro valor se aceptan lineas que esten por debajo de esa tolerancia
   * \return Verdadero si es paralela (± tol).
   */
  bool isParallel(const Segment<Point_t> &l2, double tol = 0.) const;

  /*!
   * \brief Longitud del segmento
   * \return Longitud del segmento
   */
  double length() const;

  /*!
   * \brief Vector
   * \return Vector del segmento
   */
  Point_t vector() const;
};

// Definición de métodos

template<typename Point_t> inline
Segment<Point_t>::Segment()
  : Entity(Entity::Type::segment2d),
    pt1(), 
    pt2()
{
}

template<typename Point_t> inline
Segment<Point_t>::Segment(const Segment &segment)
  : Entity(Entity::Type::segment2d),
    pt1(segment.pt1), 
    pt2(segment.pt2) 
{
}

template<typename Point_t> inline
Segment<Point_t>::Segment(Segment &&segment) TL_NOEXCEPT
  : Entity(std::forward<Entity>(segment)), 
    pt1(std::move(segment.pt1)), 
    pt2(std::move(segment.pt2)) 
{
}

template<typename Point_t> inline
Segment<Point_t>::Segment(Point_t _pt1, Point_t _pt2)
  : Entity(Entity::Type::segment2d),
    pt1(std::move(_pt1)), 
    pt2(std::move(_pt2))
{
}

template<typename Point_t> inline
Segment<Point_t>::Segment(const Point_t &pt, double angle, double length, bool bCenter)
  : Entity(Entity::Type::segment2d)
{
  double a = cos(angle);
  double b = sin(angle);
  double l1 = 0;
  double l2 = length;
  if (bCenter) {
    l1 = l2 = length / 2;
  }
  if (std::is_integral<typename Point_t::value_type>::value) {
    pt1.x = TL_ROUND_TO_INT(pt.x - l1 * -b);
    pt1.y = TL_ROUND_TO_INT(pt.y - l1 * a);
    pt2.x = TL_ROUND_TO_INT(pt.x + l2 * -b);
    pt2.y = TL_ROUND_TO_INT(pt.y + l2 * a);
  } else {
    pt1.x = static_cast<typename Point_t::value_type>(pt.x - l1 * -b);
    pt1.y = static_cast<typename Point_t::value_type>(pt.y - l1 * a);
    pt2.x = static_cast<typename Point_t::value_type>(pt.x + l2 * -b);
    pt2.y = static_cast<typename Point_t::value_type>(pt.y + l2 * a);
  }
}


template<typename Point_t> inline
Segment<Point_t> &Segment<Point_t>::operator = (const Segment &segment)
{
  if (this != &segment) {
    Entity::operator = (segment);
    this->pt1 = segment.pt1;
    this->pt2 = segment.pt2;
  }
  return *this;
}

template<typename Point_t> inline
Segment<Point_t> &Segment<Point_t>::operator = (Segment &&segment) TL_NOEXCEPT
{
  if (this != &segment) {
    Entity::operator = (std::forward<Entity>(segment));
    this->pt1 = std::move(segment.pt1);
    this->pt2 = std::move(segment.pt2);
  }
  return *this;
}

template<typename Point_t> template<typename Point_t2> inline
Segment<Point_t>::operator Segment<Point_t2>() const
{
  Segment<Point_t2> s;
  if (std::is_integral<typename Point_t2::value_type>::value) {
    s.pt1.x = TL_ROUND_TO_INT(pt1.x);
    s.pt1.y = TL_ROUND_TO_INT(pt1.y);
    s.pt2.x = TL_ROUND_TO_INT(pt2.x);
    s.pt2.y = TL_ROUND_TO_INT(pt2.y);
  } else {
    s.pt1.x = static_cast<typename Point_t2::value_type>(pt1.x);
    s.pt1.y = static_cast<typename Point_t2::value_type>(pt1.y);
    s.pt2.x = static_cast<typename Point_t2::value_type>(pt2.x);
    s.pt2.y = static_cast<typename Point_t2::value_type>(pt1.y);
  }
  return s;
}

template<typename Point_t> inline
double Segment<Point_t>::angleOX() const
{
  double angle = 0.0;
  if (pt1 != pt2) {
    angle = vectorAngleOX(vector());
  }
  return angle;

}

template<typename Point_t> inline
double Segment<Point_t>::angleOY() const
{
  double angle = 0.0;
  if (pt1 != pt2) {
    angle = vectorAngleOY(vector());
  }
  return angle;
}

template<typename Point_t> inline
Window<Point_t> Segment<Point_t>::window() const
{
  Window<Point_t> w(pt1, pt2);
  w.normalized();
  return w;
}

template<typename Point_t> inline
bool Segment<Point_t>::empty() const
{
  return (pt1.x == static_cast<typename Point_t::value_type>(0) && 
          pt1.y == static_cast<typename Point_t::value_type>(0) && 
          pt2.x == static_cast<typename Point_t::value_type>(0) && 
          pt2.y == static_cast<typename Point_t::value_type>(0)); 
}

template<typename Point_t> inline
bool Segment<Point_t>::isNear(const Segment<Point_t> &l2, double dist) const
{
  double dist1 = minDistanceSegments(*this, l2);
  return (dist1 <= dist);
}

template<typename Point_t> inline
bool Segment<Point_t>::isParallel(const Segment<Point_t> &l2, 
                                  double tol) const
{
  return (abs(angleOX() - l2.angleOX()) < tol);
}

template<typename Point_t> inline
double Segment<Point_t>::length() const 
{ 
  return distance(pt1, pt2); 
}

template<typename Point_t> inline
Point_t Segment<Point_t>::vector() const 
{ 
  return (pt2 - pt1); 
}

using SegmentI = Segment<Point<int> >;
using SegmentD = Segment<Point<double> >;
using SegmentF = Segment<Point<float> >;
using Line = SegmentI;




/*!
 * \brief Clase segmento 3D
 *
 * Esta template representa un segmento recto entre dos puntos independientemente de
 * que sean int, double o float. Se utiliza la clase Point3_ de OpenCV.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 *
 * typedef Segment3D<Point<int>> Segment3dI;
 * typedef Segment3D<Point<double>> Segment3dD;
 * typedef Segment3D<Point<float>> Segment3dF;
 *
 */
template<typename Point3_t>
class Segment3D
  : public Entity
{
public:

  using value_type = Point3_t;

  /*!
   * \brief Punto 1
   */
  Point3_t pt1;

  /*!
   * \brief Punto 2
   */
  Point3_t pt2;

public:

  /*!
   * \brief Constructor por defecto
   */
  Segment3D();

  /*!
   * \brief Constructor de copia
   * \param[in] segment Segmento que se asigna
   */
  Segment3D(const Segment3D &segment);

  /*!
   * \brief Constructor de movimiento
   * \param[in] segment Segmento que se mueve
   */
  Segment3D(Segment3D &&segment) TL_NOEXCEPT;

  /*!
   * \brief Constructor segment
   * \param[in] _pt1 Punto 1
   * \param[in] _pt2 Punto 2
   */
  Segment3D(const Point3_t &_pt1, const Point3_t &_pt2);

  ~Segment3D() override = default;

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia a la ventana
   */
  Segment3D &operator = (const Segment3D &segment);

  /*!
   * \brief Sobrecarga del operador de asignación de movimiento
   * \param[in] segment Segmento que se mueve
   * \return Referencia a la ventana
   */
  Segment3D &operator = (Segment3D &&segment) TL_NOEXCEPT;

  /*!
   * \brief Conversión a un segmento de un tipo diferente
   */
  template<typename Point3_t2> operator Segment3D<Point3_t2>() const;

  /*!
   * \brief Caja envolvente del segmento
   */
  BoundingBox<Point3_t> boundingBox() const;

  /*!
   * \brief Comprueba si el segmento esta vacio pt1 = (0, 0, 0) ; pt2 = (0, 0, 0)
   * \return Verdadero si esta vacio
   */
  bool isEmpty() const;

  //  bool isNear(const Segment<T> &l2, double dist = 10) const;

  /*!
   * \brief Longitud del segmento
   * \return Longitud del segmento
   */
  double length() const;

  /*!
   * \brief Vector
   * \return Vector del segmento
   */
  Point3_t vector() const;
};

// Definición de métodos

template<typename Point3_t> inline
Segment3D<Point3_t>::Segment3D()
  : Entity(Entity::Type::segment3d),
    pt1(Point3_t()), 
    pt2(Point3_t()) 
{
}

template<typename T> inline
Segment3D<T>::Segment3D(const Segment3D &segment)
: Entity(segment), 
  pt1(segment.pt1), 
  pt2(segment.pt2) 
{
}

template<typename T> inline
Segment3D<T>::Segment3D(Segment3D &&segment) TL_NOEXCEPT
: Entity(std::forward<Entity>(segment)), 
  pt1(std::move(segment.pt1)), 
  pt2(std::move(segment.pt2)) 
{
}

template<typename Point3_t> inline
Segment3D<Point3_t>::Segment3D(const Point3_t &_pt1, const Point3_t &_pt2)
  : Entity(Entity::Type::segment3d),
    pt1(_pt1), 
    pt2(_pt2) 
{
}

template<typename Point3_t> inline
Segment3D<Point3_t> &Segment3D<Point3_t>::operator = (const Segment3D &segment)
{
  if (this != &segment) {
    Entity::operator=(segment);
    this->pt1 = segment.pt1;
    this->pt2 = segment.pt2;
  }
  return *this;
}

template<typename Point3_t> inline
Segment3D<Point3_t> &Segment3D<Point3_t>::operator = (Segment3D &&segment) TL_NOEXCEPT
{
  if (this != &segment) {
    Entity::operator=(std::forward<Entity>(segment));
    this->pt1 = std::move(segment.pt1);
    this->pt2 = std::move(segment.pt2);
  }
  return *this;
}

template<typename Point3_t> template<typename Point3_t2> inline
Segment3D<Point3_t>::operator Segment3D<Point3_t2>() const
{
  Segment3D<Point3_t2> s;
  if (std::is_integral<typename Point3_t2::value_type>::value) {
    s.pt1.x = TL_ROUND_TO_INT(pt1.x);
    s.pt1.y = TL_ROUND_TO_INT(pt1.y);
    s.pt1.z = TL_ROUND_TO_INT(pt1.z);
    s.pt2.x = TL_ROUND_TO_INT(pt2.x);
    s.pt2.y = TL_ROUND_TO_INT(pt2.y);
    s.pt2.z = TL_ROUND_TO_INT(pt2.z);
  } else {
    s.pt1.x = static_cast<typename Point3_t2::value_type>(pt1.x);
    s.pt1.y = static_cast<typename Point3_t2::value_type>(pt1.y);
    s.pt1.z = static_cast<typename Point3_t2::value_type>(pt1.z);
    s.pt2.x = static_cast<typename Point3_t2::value_type>(pt2.x);
    s.pt2.y = static_cast<typename Point3_t2::value_type>(pt1.y);
    s.pt2.z = static_cast<typename Point3_t2::value_type>(pt1.z);
  }
  return s;
}

template<typename Point3_t> inline
BoundingBox<Point3_t> Segment3D<Point3_t>::boundingBox() const
{
  return BoundingBox<Point3_t>(pt1, pt2);
}

template<typename Point3_t> inline
bool Segment3D<Point3_t>::isEmpty() const
{
  return (pt1.x == static_cast<typename Point3_t::value_type>(0)  && 
          pt1.y == static_cast<typename Point3_t::value_type>(0)  && 
          pt2.x == static_cast<typename Point3_t::value_type>(0)  && 
          pt2.y == static_cast<typename Point3_t::value_type>(0)); 
}

//template<typename T> inline
//bool Segment<T>::isNear(const Segment<T> &l2, double dist) const
//{
//  double dist1 = minDistanceSegments(*this, l2);
//  return (dist1 < dist);
//}

template<typename Point3_t> inline
double Segment3D<Point3_t>::length() const 
{ 
  return distance(pt1, pt2); 
}

template<typename Point3_t> inline
Point3_t Segment3D<Point3_t>::vector() const 
{ 
  return (pt2 - pt1); 
}

using Segment3dI = Segment3D<Point3<int> >;
using Segment3dD = Segment3D<Point3<double> >;
using Segment3dF = Segment3D<Point3<float> >;




/*!
 * \brief The ldGroupLines class
 */
class TL_EXPORT GroupLines
{
private:
  /*!
   * \brief linesgrops
   */
  std::vector<Line> linesgroup;

  /*!
   * \brief Ventana envolvente del grupo de líneas
   */
  WindowI bbox;

public:

  /*!
   * \brief Constructora GroupLines
   */
  GroupLines();

  /*!
   * \brief GroupLines
   * \param[in] lines Vector de lineas
   */
  GroupLines(const std::vector<Line> &lines);

  /*!
   * \brief Añade una línea
   * \param[in] line Linea
   */
  void add(const Line &line);

#ifdef HAVE_OPENCV

  /*!
   * \brief Añade una línea
   * \param[in] lvect
   */
  void add(const cv::Vec4i &lvect);

#endif

  /*!
   * \brief Ángulo medio
   * \return
   */
  double angleMean();

  void deleteLine(int id);

  /*!
   * \brief Ventana envolvente del grupo de lineas
   * \return
   */
  WindowI window() const { return bbox; }

  /*!
   * \brief Número de líneas
   * \return
   */
  size_t size() const { return linesgroup.size(); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id Indice del elemento
   * \return Linea
   */
  const Line &operator[](size_t id) const { return linesgroup[id]; }
  Line &operator[](size_t id)  { return linesgroup[id]; }

  const std::vector<Line> &getLines() const { return linesgroup; }

};


/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOMETRY_SEGMENT_H
