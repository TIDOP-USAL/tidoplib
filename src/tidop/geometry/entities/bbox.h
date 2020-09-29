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
  typedef Point3_t value_type;

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
  BoundingBox(const Point3_t &pt1, const Point3_t &pt2);

  /*!
   * \brief Constructor
   * \param[in] pt Punto central
   * \param[in] sxx Ancho de la ventana
   * \param[in] szy profundidad de la ventana
   * \param[in] szz Alto de la ventana 
   */
  template<typename T>
  BoundingBox(const Point3_t &pt, T sxx, T szy, T szz);

  /*!
   * \brief Constructor 
   * \param[in] pt Punto central
   * \param[in] sz Dimensiones
   */
  template<typename T>
  BoundingBox(const Point3_t &pt, T sz);

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
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt1, const Point3_t &pt2) 
  : Entity(Entity::Type::bounding_box)
{
  this->pt1.x = std::min(pt1.x, pt2.x);
  this->pt1.y = std::min(pt1.y, pt2.y);
  this->pt1.z = std::min(pt1.z, pt2.z);
  this->pt2.x = std::max(pt1.x, pt2.x);
  this->pt2.y = std::max(pt1.y, pt2.y);
  this->pt2.z = std::max(pt1.z, pt2.z);
}

TL_DISABLE_WARNING(TL_WARNING_C4244)
template<typename Point3_t> template<typename T> inline
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt, T sxx, T szy, T szz) 
  : Entity(Entity::Type::bounding_box)
{ 
  if (std::is_integral<typename Point3_t::value_type>::value) {
    int sxx_2 = TL_ROUND_TO_INT(sxx / 2);
    int szy_2 = TL_ROUND_TO_INT(szy / 2);
    int szz_2 = TL_ROUND_TO_INT(szz / 2);
    int dx = static_cast<int>(sxx) % 2;
    int dy = static_cast<int>(szy) % 2;
    int dz = static_cast<int>(szz) % 2;
    pt1 = Point3_t(pt.x - sxx_2, pt.y - szy_2, pt.z - szz_2);
    pt2 = Point3_t(pt.x + sxx_2 + dx, pt.y + szy_2 + dy, pt.z + szz_2 + dz);
  } else {
    pt1 = Point3_t(pt.x - sxx / 2., pt.y - szy / 2., pt.z - szz / 2.);
    pt2 = Point3_t(pt.x + sxx / 2., pt.y + szy / 2., pt.z + szz / 2.);
  }
}


template<typename Point3_t> template<typename T> inline
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt, T sz) 
  : Entity(Entity::Type::bounding_box) 
{
  if (std::is_integral<typename Point3_t::value_type>::value) {
    int sz_2 = TL_ROUND_TO_INT(sz / 2);
    int dxyz = static_cast<int>(sz) % 2;
    pt1 = Point3_t(pt.x - sz_2, pt.y - sz_2, pt.z - sz_2);
    pt2 = Point3_t(pt.x + sz_2 + dxyz, pt.y + sz_2 + dxyz, pt.z + sz_2 + dxyz);
  } else {
    int sz_2 = sz / 2.;
    pt1 = Point3_t(pt.x - sz_2, pt.y - sz_2, pt.z - sz_2);
    pt2 = Point3_t(pt.x + sz_2, pt.y + sz_2, pt.z + sz_2);
  }
}
TL_ENABLE_WARNING(TL_WARNING_C4244)

template<typename Point3_t> inline
BoundingBox<Point3_t> &BoundingBox<Point3_t>::operator = (const BoundingBox &bbox)
{
  if (this != &bbox) {
    Entity::operator = (bbox.mEntityType);
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
  //if (std::is_integral<typename Point3_t::value_type>::value) {
  //  double x = (static_cast<double>(pt1.x) + static_cast<double>(pt2.x)) / 2.;
  //  double y = (static_cast<double>(pt1.y) + static_cast<double>(pt2.y)) / 2.;
  //  double z = (static_cast<double>(pt1.z) + static_cast<double>(pt2.z)) / 2.;
  //  Point3_t pt(static_cast<typename Point3_t::value_type>(round(x)),
  //              static_cast<typename Point3_t::value_type>(round(y)),
  //              static_cast<typename Point3_t::value_type>(round(z)));
  //  //return Point3_t(static_cast<typename Point3_t::value_type>(round((pt1.x + pt2.x) / 2.)),
  //  //                static_cast<typename Point3_t::value_type>(round((pt1.y + pt2.y) / 2.)),
  //  //                static_cast<typename Point3_t::value_type>(round((pt1.z + pt2.z) / 2.)));
  //} else {
    typename Point3_t::value_type two{2};
    return Point3_t((pt1.x + pt2.x) / two, 
                    (pt1.y + pt2.y) / two, 
                    (pt1.z + pt2.z) / two);
  //}
}
TL_ENABLE_WARNING(TL_WARNING_C4244)

template<typename Point3_t> inline
typename Point3_t::value_type BoundingBox<Point3_t>::width() const
{
  return pt2.x - pt1.x;
}

template<typename Point3_t> inline
typename Point3_t::value_type BoundingBox<Point3_t>::height() const
{
  return pt2.y - pt1.y;
}

template<typename Point3_t> inline
typename Point3_t::value_type BoundingBox<Point3_t>::depth() const
{
  return pt2.z - pt1.z;
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::isEmpty() const 
{ 
  return (   pt1.x == std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt1.y == std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt1.z == std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt2.x == -std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt2.y == -std::numeric_limits<typename Point3_t::value_type>().max()
          && pt2.z == -std::numeric_limits<typename Point3_t::value_type>().max()); 
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::containsPoint(const Point3_t &pt) const
{
  return ((pt2.x >= pt.x) && (pt2.y >= pt.y) && (pt2.z >= pt.z)
       && (pt1.x <= pt.x) && (pt1.y <= pt.y) && (pt1.z <= pt.z));
}

template<typename Point3_t> inline
bool BoundingBox<Point3_t>::containsBox(const BoundingBox<Point3_t> &bbox) const
{
  return (pt1.x <= bbox.pt1.x && pt1.y <= bbox.pt1.y && pt1.z <= bbox.pt1.z &&
          pt2.x >= bbox.pt2.x && pt2.y >= bbox.pt2.y && pt2.z >= bbox.pt2.z);
}

typedef BoundingBox<Point3<int>> BoundingBoxI;
typedef BoundingBox<Point3<double>> BoundingBoxD;
typedef BoundingBox<Point3<float>> BoundingBoxF;


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
