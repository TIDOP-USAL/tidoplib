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

#ifndef TL_GEOMETRY_BOUNDING_BOX_H
#define TL_GEOMETRY_BOUNDING_BOX_H

#include "config_tl.h"

#include <algorithm>

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Clase cuadro delimitador (bounding bbox)
 */
template<typename Point3_t>
class BoundingBox 
  : public Entity
{

public:

  /*!
   * \brief type
   */
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
  BoundingBox();

  /*!
   * \brief Constructor de copia
   * \param[in] bbox Objeto que se copia
   */
  BoundingBox(const BoundingBox &bbox);
    
  /*!
   * \brief Constructor de movimiento
   * \param[in] bbox Objeto que se mueve
   */
  BoundingBox(BoundingBox &&bbox) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] pt1 Primer punto
   * \param[in] pt2 Segundo punto
   */
  BoundingBox(const Point3_t &pt1, 
              const Point3_t &pt2);

  /*!
   * \brief Constructor
   * \param[in] pt Punto central
   * \param[in] width Anchura de la caja
   * \param[in] depth Profundidad de la caja
   * \param[in] height Altura de la caja 
   */
  template<typename T>
  BoundingBox(const Point3_t &pt, 
              T width, T depth, T height);

  /*!
   * \brief Constructor 
   * \param[in] pt Punto central
   * \param[in] side Dimensiones
   */
  template<typename T>
  BoundingBox(const Point3_t &pt, T side);

  ~BoundingBox() override = default;

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] bbox BoundingBox que se asigna
   * \return Referencia al Bbox
   */
  BoundingBox &operator = (const BoundingBox &bbox);

  /*!
   * \brief Sobrecarga del operador de asignación de movimiento
   * \param[in] bbox Bounding Box que se mueve
   * \return Referencia al Bbox
   */
  BoundingBox &operator = (BoundingBox &&bbox) TL_NOEXCEPT;

  /*!
   * \brief Sobrecarga del operador 'igual que'
   * \param[in] bbox Bounding Box con el que se compara
   * \return true si ambos bbox son iguales
   */
  bool operator == (const BoundingBox &bbox) const;

  /*!
   * \brief Conversión a una caja envolvente de un tipo diferente
   */
  template<typename Point3_t2> operator BoundingBox<Point3_t2>() const;

  /*!
   * \brief Devuelve centro del Bbox
   * \return Centro del Bbox
   */
  Point3_t center() const;

  /*!
   * \brief Devuelve el ancho del cuadro envolvente
   * \return Ancho
   */
  typename Point3_t::value_type width() const;

  /*!
   * \brief Devuelve el alto de la caja
   * \return Alto
   */
  typename Point3_t::value_type height() const;

  /*!
   * \brief Devuelve la profundidad del cuadro delimitador
   * \return Profundidad
   */
  typename Point3_t::value_type depth() const;

  /*!
   * \brief Comprueba si el cuadro delimitador esta vacio
   */
  bool isEmpty() const;
  bool isValid() const;
  void normalized();

  /*!
   * \brief Comprueba si un punto está contenido dentro del cuadro delimitador
   * \param[in] pt Punto
   * \return true si el punto esta dentro del cuadro delimitador
   */
  bool containsPoint(const Point3_t &pt) const;

  /*!
   * \brief Comprueba si el cuadro delimitador pasado como parámetro esta contenido en el cuadro delimitador
   */
  bool containsBox(const BoundingBox<Point3_t> &bbox) const;
};



// Definición de métodos

template<typename Point3_t> inline
BoundingBox<Point3_t>::BoundingBox() 
  : Entity(Entity::Type::bounding_box), 
    pt1(std::numeric_limits<typename Point3_t::value_type>().max(), 
        std::numeric_limits<typename Point3_t::value_type>().max(), 
        std::numeric_limits<typename Point3_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point3_t::value_type>().max(), 
        -std::numeric_limits<typename Point3_t::value_type>().max(), 
        -std::numeric_limits<typename Point3_t::value_type>().max()) 
{
}

template<typename Point3_t> inline
BoundingBox<Point3_t>::BoundingBox(const BoundingBox &bbox) 
  : Entity(Entity::Type::bounding_box), 
    pt1(bbox.pt1), 
    pt2(bbox.pt2) 
{
}

template<typename Point3_t> inline
BoundingBox<Point3_t>::BoundingBox(BoundingBox &&bbox) TL_NOEXCEPT
  : Entity(std::forward<Entity>(bbox)), 
    pt1(std::move(bbox.pt1)), 
    pt2(std::move(bbox.pt2)) 
{
}

template<typename Point3_t> inline
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt1, 
                                   const Point3_t &pt2) 
  : Entity(Entity::Type::bounding_box),
    pt1(std::move(pt1)),
    pt2(std::move(pt2))
{
}

template<typename Point3_t> template<typename T> inline
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt, 
                                   T width, T depth, T height) 
  : Entity(Entity::Type::bounding_box)
{ 
  typename Point3_t::value_type two{2};
  auto half_width = width / two;
  auto half_depth = depth / two;
  auto half_height = height / two;

  if (std::is_integral<typename Point3_t::value_type>::value) {

    int dx = static_cast<int>(width) % 2;
    int dy = static_cast<int>(depth) % 2;
    int dz = static_cast<int>(height) % 2;

    pt1 = Point3_t(pt.x - half_width, 
                   pt.y - half_depth, 
                   pt.z - half_height);
    pt2 = Point3_t(pt.x + half_width + dx, 
                   pt.y + half_depth + dy, 
                   pt.z + half_height + dz);
  } else {
    pt1 = Point3_t(pt.x - half_width, 
                   pt.y - half_depth, 
                   pt.z - half_height);
    pt2 = Point3_t(pt.x + half_width, 
                   pt.y + half_depth, 
                   pt.z + half_height);
  }
}


template<typename Point3_t> template<typename T> inline
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt, 
                                   T side) 
  : Entity(Entity::Type::bounding_box) 
{
  typename Point3_t::value_type two{2};
  auto half_side = side / two;

  if (std::is_integral<typename Point3_t::value_type>::value) {

    int dxyz = static_cast<int>(side) % 2;

    pt1 = Point3_t(pt.x - half_side, 
                   pt.y - half_side, 
                   pt.z - half_side);
    pt2 = Point3_t(pt.x + half_side + dxyz, 
                   pt.y + half_side + dxyz, 
                   pt.z + half_side + dxyz);
  } else {
    pt1 = Point3_t(pt.x - half_side, 
                   pt.y - half_side, 
                   pt.z - half_side);
    pt2 = Point3_t(pt.x + half_side, 
                   pt.y + half_side, 
                   pt.z + half_side);
  }
}

template<typename Point3_t> inline
BoundingBox<Point3_t> &BoundingBox<Point3_t>::operator = (const BoundingBox &bbox)
{
  if (this != &bbox) {
    Entity::operator = (bbox);
    this->pt1 = bbox.pt1;
    this->pt2 = bbox.pt2;
  }
  return *this;
}

template<typename Point3_t> inline
BoundingBox<Point3_t> &BoundingBox<Point3_t>::operator = (BoundingBox &&bbox) TL_NOEXCEPT
{
  if (this != &bbox) {
    Entity::operator = (std::forward<Entity>(bbox));
    this->pt1 = std::move(bbox.pt1);
    this->pt2 = std::move(bbox.pt2);
  }
  return *this;
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::operator == (const BoundingBox &bbox) const
{
  return (this->pt1 == bbox.pt1 && this->pt2 == bbox.pt2);
}

template<typename Point3_t> template<typename Point3_t2> inline
BoundingBox<Point3_t>::operator BoundingBox<Point3_t2>() const
{
  return BoundingBox<Point3_t2>(static_cast<Point3_t2>(pt1), 
                                static_cast<Point3_t2>(pt2));
}

template<typename Point3_t> inline
Point3_t BoundingBox<Point3_t>::center() const
{
  Point3_t pt_center;

  if (!this->isEmpty()) {
    typename Point3_t::value_type two{2};

    pt_center.x = (pt1.x + pt2.x) / two;
    pt_center.y = (pt1.y + pt2.y) / two;
    pt_center.z = (pt1.z + pt2.z) / two;
  }

  return pt_center;
}

template<typename Point3_t> inline
typename Point3_t::value_type BoundingBox<Point3_t>::width() const
{
  return this->isEmpty() ?  static_cast<typename Point3_t::value_type>(0) : pt2.x - pt1.x;
}

template<typename Point3_t> inline
typename Point3_t::value_type BoundingBox<Point3_t>::height() const
{
  return this->isEmpty() ?  static_cast<typename Point3_t::value_type>(0) : pt2.y - pt1.y;
}

template<typename Point3_t> inline
typename Point3_t::value_type BoundingBox<Point3_t>::depth() const
{
  return this->isEmpty() ?  static_cast<typename Point3_t::value_type>(0) : pt2.z - pt1.z;
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::isEmpty() const 
{ 
  return (pt1.x == std::numeric_limits<typename Point3_t::value_type>().max() && 
          pt1.y == std::numeric_limits<typename Point3_t::value_type>().max() &&
          pt1.z == std::numeric_limits<typename Point3_t::value_type>().max() &&
          pt2.x == -std::numeric_limits<typename Point3_t::value_type>().max() &&
          pt2.y == -std::numeric_limits<typename Point3_t::value_type>().max() &&
          pt2.z == -std::numeric_limits<typename Point3_t::value_type>().max());
}

template<typename Point3_t>
bool BoundingBox<Point3_t>::isValid() const
{
  return this->width() > static_cast<typename Point3_t::value_type>(0) &&
         this->height() > static_cast<typename Point3_t::value_type>(0) &&
      this->depth() > static_cast<typename Point3_t::value_type>(0);
}

template<typename Point3_t>
void BoundingBox<Point3_t>::normalized()
{
  if (!this->isValid()) {
    if (this->pt1.x > this->pt2.x) std::swap(this->pt1.x, this->pt2.x);
    if (this->pt1.y > this->pt2.y) std::swap(this->pt1.y, this->pt2.y);
    if (this->pt1.z > this->pt2.z) std::swap(this->pt1.z, this->pt2.z);
  }
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::containsPoint(const Point3_t &pt) const
{
  return ((pt2.x >= pt.x) &&
          (pt2.y >= pt.y) &&
          (pt2.z >= pt.z) &&
          (pt1.x <= pt.x) &&
          (pt1.y <= pt.y) &&
          (pt1.z <= pt.z));
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::containsBox(const BoundingBox<Point3_t> &bbox) const
{
  return (pt1.x <= bbox.pt1.x &&
          pt1.y <= bbox.pt1.y &&
          pt1.z <= bbox.pt1.z &&
          pt2.x >= bbox.pt2.x &&
          pt2.y >= bbox.pt2.y &&
          pt2.z >= bbox.pt2.z);
}

using BoundingBoxI = BoundingBox<Point3<int> >;
using BoundingBoxD = BoundingBox<Point3<double> >;
using BoundingBoxF = BoundingBox<Point3<float> >;


/*!
 * \brief Unión de bounding boxes
 * \param[in] b1 Caja 1
 * \param[in] b2 Caja 2
 * \return Caja unión
 */
template<typename T> inline
T joinBoundingBoxes(const T &b1, const T &b2)
{
  T bounding_box;
  bounding_box.pt1.x = std::min(b1.pt1.x, b2.pt1.x);
  bounding_box.pt1.y = std::min(b1.pt1.y, b2.pt1.y);
  bounding_box.pt1.z = std::min(b1.pt1.z, b2.pt1.z);
  bounding_box.pt2.x = std::max(b1.pt2.x, b2.pt2.x);
  bounding_box.pt2.y = std::max(b1.pt2.y, b2.pt2.y);
  bounding_box.pt2.z = std::max(b1.pt2.z, b2.pt2.z);
  return bounding_box;
}


/*!
 * \brief Intersección de bounding boxes
 * \param[in] b1 Caja 1
 * \param[in] b2 Caja 2
 * \return Caja envolovente intersección
 */
template<typename T> inline
T intersectBoundingBoxes(const T &b1, const T &b2)
{
  T bounding_box;
  bounding_box.pt1.x = std::max(b1.pt1.x, b2.pt1.x);
  bounding_box.pt1.y = std::max(b1.pt1.y, b2.pt1.y);
  bounding_box.pt1.z = std::max(b1.pt1.z, b2.pt1.z);
  bounding_box.pt2.x = std::min(b1.pt2.x, b2.pt2.x);
  bounding_box.pt2.y = std::min(b1.pt2.y, b2.pt2.y);
  bounding_box.pt2.z = std::min(b1.pt2.z, b2.pt2.z);
  return bounding_box;
}


/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOMETRY_BOUNDING_BOX_H
