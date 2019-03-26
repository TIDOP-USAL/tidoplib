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


#ifndef TL_GEOM_BBOX_H
#define TL_GEOM_BBOX_H

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

namespace geometry
{

/*!
 * \brief Unión de caja
 * \param[in] b1 Caja 1
 * \param[in] b2 Caja 2
 * \return Caja unión
 */
template<typename T> inline
T joinBox(const T &b1, const T &b2)
{
  T box;
  box.pt1.x = std::min(b1.pt1.x, b2.pt1.x);
  box.pt1.y = std::min(b1.pt1.y, b2.pt1.y);
  box.pt1.z = std::min(b1.pt1.z, b2.pt1.z);
  box.pt2.x = std::max(b1.pt2.x, b2.pt2.x);
  box.pt2.y = std::max(b1.pt2.y, b2.pt2.y);
  box.pt2.z = std::max(b1.pt2.z, b2.pt2.z);
  return box;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase caja
 */
template<typename Point3_t>
class Box 
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
  Box();

  /*!
   * \brief Constructor de copia
   * \param[in] box Objeto que se copia
   */
  Box(const Box &box);
    
  /*!
   * \brief Constructor de movimiento
   * \param[in] box Objeto que se mueve
   */
  Box(Box &&box) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] pt1 Primer punto
   * \param[in] pt2 Segundo punto
   */
  Box(const Point3_t &pt1, const Point3_t &pt2);

  /*!
   * \brief Constructor
   * \param[in] pt Punto central
   * \param[in] sxx Ancho de la ventana
   * \param[in] szy profundidad de la ventana
   * \param[in] szz Alto de la ventana 
   * tendria que mirar el tema de los ejes...
   */
  template<typename T>
  Box(const Point3_t &pt, T sxx, T szy, T szz);

  /*!
   * \brief Constructor 
   * \param[in] pt Punto central
   * \param[in] sz Dimensiones
   */
  template<typename T>
  Box(const Point3_t &pt, T sz);

  //~Bbox();

  /*!
   * \brief Sobrecarga del operador  de asignación
   * \param[in] box Box que se asigna
   * \return Referencia al Bbox
   */
  Box &operator = (const Box &box);

  /*!
   * \brief Sobrecarga del operador de asignación de movimiento
   * \param[in] box Box que se mueve
   * \return Referencia al Bbox
   */
  Box &operator = (Box &&box) TL_NOEXCEPT;

  /*!
   * \brief Sobrecarga del operador 'igual que'
   * \param[in] box bbox con el que se compara
   * \return true si ambos bbox son iguales
   */
  bool operator == (const Box &box) const;

  /*!
   * \brief Conversión a una ventana de un tipo diferente
   */
  template<typename Point3_t2> operator Box<Point3_t2>() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve centro del Bbox
   * \return Centro del Bbox
   * \deprecated Use 'center()' en su lugar
   */
  TL_DEPRECATED("center()", "2.0")
  Point3_t getCenter() const;
#endif
  /*!
   * \brief Devuelve centro del Bbox
   * \return Centro del Bbox
   */
  Point3_t center() const;


#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve el ancho de la caja
   * \return Ancho
   * \deprecated Use 'width()' en su lugar
   */
  TL_DEPRECATED("width()", "2.0")
  typename Point3_t::value_type getWidth() const;
#endif
  /*!
   * \brief Devuelve el ancho de la caja
   * \return Ancho
   */
  typename Point3_t::value_type width() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve el alto de la caja
   * \return Alto
   * \deprecated Use 'height()' en su lugar
   */
  TL_DEPRECATED("height()", "2.0")
  typename Point3_t::value_type getHeight() const;
#endif
  /*!
   * \brief Devuelve el alto de la caja
   * \return Alto
   */
  typename Point3_t::value_type height() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve la profundidad de la caja
   * \return Profundidad
   * \deprecated Use 'depth()' en su lugar
   */
  TL_DEPRECATED("depth()", "2.0")
  typename Point3_t::value_type getDepth() const;
#endif
  /*!
   * \brief Devuelve la profundidad de la caja
   * \return Profundidad
   */
  typename Point3_t::value_type depth() const;

  /*!
   * \brief Comprueba si el Bbox esta vacio
   * \return True si el Bbox es nula
   */
  bool isEmpty() const;

  /*!
   * \brief Comprueba si un punto está contenido dentro del Bbox
   * \param[in] pt Punto
   * \return true si el punto esta dentro del Bbox
   */
  //template<typename Point3_t2> bool containsPoint(const Point3_t2 &pt) const;
  bool containsPoint(const Point3_t &pt) const;

  /*!
   * \brief La ventana contiene la ventana
   */
  //template<typename Point3_t2> bool containsBbox(const Box<Point3_t2> &box) const;
  bool containsBox(const Box<Point3_t> &box) const;
};

// Definición de métodos

template<typename Point3_t> inline
Box<Point3_t>::Box() 
  : Entity(Entity::Type::box), 
    pt1(std::numeric_limits<typename Point3_t::value_type>().max(), 
        std::numeric_limits<typename Point3_t::value_type>().max(), 
        std::numeric_limits<typename Point3_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point3_t::value_type>().max(), 
        -std::numeric_limits<typename Point3_t::value_type>().max(), 
        -std::numeric_limits<typename Point3_t::value_type>().max()) 
{
}

template<typename Point3_t> inline
Box<Point3_t>::Box(const Box &box) 
  : Entity(Entity::Type::box), 
    pt1(box.pt1), 
    pt2(box.pt2) 
{
}

template<typename Point3_t> inline
Box<Point3_t>::Box(Box &&box) TL_NOEXCEPT
  : Entity(std::forward<Entity>(box)), 
    pt1(std::move(box.pt1)), 
    pt2(std::move(box.pt2)) 
{
}

template<typename Point3_t> inline
Box<Point3_t>::Box(const Point3_t &_pt1, const Point3_t &_pt2) 
  : Entity(Entity::Type::box)
{
  pt1.x = std::min(_pt1.x, _pt2.x);
  pt1.y = std::min(_pt1.y, _pt2.y);
  pt1.z = std::min(_pt1.z, _pt2.z);
  pt2.x = std::max(_pt1.x, _pt2.x);
  pt2.y = std::max(_pt1.y, _pt2.y);
  pt2.z = std::max(_pt1.z, _pt2.z);
}

template<typename Point3_t> template<typename T> inline
Box<Point3_t>::Box(const Point3_t &_pt, T sxx, T szy, T szz) 
  : Entity(Entity::Type::box)
{ 
  if (std::is_integral<typename Point3_t::value_type>::value) {
    int sxx_2 = TL_ROUND_TO_INT(sxx / 2);
    int szy_2 = TL_ROUND_TO_INT(szy / 2);
    int szz_2 = TL_ROUND_TO_INT(szz / 2);
    int dx = static_cast<int>(sxx) % 2;
    int dy = static_cast<int>(szy) % 2;
    int dz = static_cast<int>(szz) % 2;
    pt1 = Point3_t(_pt.x - sxx_2, _pt.y - szy_2, _pt.z - szz_2);
    pt2 = Point3_t(_pt.x + sxx_2 + dx, _pt.y + szy_2 + dy, _pt.z + szz_2 + dz);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    pt1 = Point3_t(_pt.x - sxx / 2., _pt.y - szy / 2., _pt.z - szz / 2.);
    pt2 = Point3_t(_pt.x + sxx / 2., _pt.y + szy / 2., _pt.z + szz / 2.);
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}

template<typename Point3_t> template<typename T> inline
Box<Point3_t>::Box(const Point3_t &_pt, T sz) 
  : Entity(Entity::Type::box) 
{
  if (std::is_integral<typename Point3_t::value_type>::value) {
    int sz_2 = TL_ROUND_TO_INT(sz / 2);
    int dxyz = static_cast<int>(sz) % 2;
    pt1 = Point3_t(_pt.x - sz_2, _pt.y - sz_2, _pt.z - sz_2);
    pt2 = Point3_t(_pt.x + sz_2 + dxyz, _pt.y + sz_2 + dxyz, _pt.z + sz_2 + dxyz);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    int sz_2 = sz / 2.;
    pt1 = Point3_t(_pt.x - sz_2, _pt.y - sz_2, _pt.z - sz_2);
    pt2 = Point3_t(_pt.x + sz_2, _pt.y + sz_2, _pt.z + sz_2);
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}

template<typename Point3_t> inline
Box<Point3_t> &Box<Point3_t>::operator = (const Box &box)
{
  if (this != &box) {
    this->mEntityType = box.mEntityType;
    this->pt1 = box.pt1;
    this->pt2 = box.pt2;
  }
  return *this;
}

template<typename Point3_t> inline
Box<Point3_t> &Box<Point3_t>::operator = (Box &&box) TL_NOEXCEPT
{
  if (this != &box) {
    this->mEntityType = std::move(box.mEntityType);
    this->pt1 = std::move(box.pt1);
    this->pt2 = std::move(box.pt2);
  }
  return *this;
}

template<typename Point3_t> inline
bool Box<Point3_t>::operator == (const Box &box) const
{
  return (pt1 == box.pt1 && pt2 == box.pt2);
}

template<typename Point3_t> template<typename Point3_t2> inline
Box<Point3_t>::operator Box<Point3_t2>() const
{
  return Box<Point3_t2>(static_cast<Point3_t2>(pt1), static_cast<Point3_t2>(pt2));
}

TL_DISABLE_WARNING(TL_WARNING_C4244)
#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point3_t> inline
Point3_t Box<Point3_t>::getCenter() const
{
  if (std::is_integral<typename Point3_t::value_type>::value) {
    return Point3_t(TL_ROUND_TO_INT((pt1.x + pt2.x) / 2), 
                    TL_ROUND_TO_INT((pt1.y + pt2.y) / 2), 
                    TL_ROUND_TO_INT((pt1.z + pt2.z) / 2));
  } else {
    return Point3_t((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2., (pt1.z + pt2.z) / 2.);
  }
}
#endif

template<typename Point3_t> inline
Point3_t Box<Point3_t>::center() const
{
  if (std::is_integral<typename Point3_t::value_type>::value) {
    return Point3_t(TL_ROUND_TO_INT((pt1.x + pt2.x) / 2),
                    TL_ROUND_TO_INT((pt1.y + pt2.y) / 2),
                    TL_ROUND_TO_INT((pt1.z + pt2.z) / 2));
  } else {
    return Point3_t((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2., (pt1.z + pt2.z) / 2.);
  }
}
TL_ENABLE_WARNING(TL_WARNING_C4244)

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point3_t> inline
typename Point3_t::value_type Box<Point3_t>::getWidth() const 
{ 
  return pt2.x - pt1.x; 
}
#endif

template<typename Point3_t> inline
typename Point3_t::value_type Box<Point3_t>::width() const
{
  return pt2.x - pt1.x;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point3_t> inline
typename Point3_t::value_type Box<Point3_t>::getHeight() const 
{ 
  return pt2.y - pt1.y; 
}
#endif

template<typename Point3_t> inline
typename Point3_t::value_type Box<Point3_t>::height() const
{
  return pt2.y - pt1.y;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point3_t> inline
typename Point3_t::value_type Box<Point3_t>::getDepth() const 
{ 
  return pt2.z - pt1.z; 
}
#endif

template<typename Point3_t> inline
typename Point3_t::value_type Box<Point3_t>::depth() const
{
  return pt2.z - pt1.z;
}

template<typename Point3_t> inline
bool Box<Point3_t>::isEmpty() const 
{ 
  return (   pt1.x == std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt1.y == std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt1.z == std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt2.x == -std::numeric_limits<typename Point3_t::value_type>().max() 
          && pt2.y == -std::numeric_limits<typename Point3_t::value_type>().max()
          && pt2.z == -std::numeric_limits<typename Point3_t::value_type>().max()); 
}

//template<typename Point3_t> template<typename Point3_t2> inline
//bool Box<Point3_t>::containsPoint(const Point3_t2 &pt) const
//{
//  return ((pt2.x >= pt.x) && (pt2.y >= pt.y) && (pt2.z >= pt.z)
//       && (pt1.x <= pt.x) && (pt1.y <= pt.y) && (pt1.z <= pt.z));
//}
//
//template<typename Point3_t> template<typename Point3_t2> inline
//bool Box<Point3_t>::containsBbox(const Box<Point3_t2> &box) const
//{
//  return (pt1.x <= box.pt1.x && pt1.y <= box.pt1.y && pt1.z <= box.pt1.z &&
//          pt2.x >= box.pt2.x && pt2.y >= box.pt2.y && pt2.z >= box.pt2.z);
//}

template<typename Point3_t> inline
bool Box<Point3_t>::containsPoint(const Point3_t &pt) const
{
  return ((pt2.x >= pt.x) && (pt2.y >= pt.y) && (pt2.z >= pt.z)
       && (pt1.x <= pt.x) && (pt1.y <= pt.y) && (pt1.z <= pt.z));
}

template<typename Point3_t> inline
bool Box<Point3_t>::containsBox(const Box<Point3_t> &box) const
{
  return (pt1.x <= box.pt1.x && pt1.y <= box.pt1.y && pt1.z <= box.pt1.z &&
          pt2.x >= box.pt2.x && pt2.y >= box.pt2.y && pt2.z >= box.pt2.z);
}

typedef Box<Point3<int>> BoxI;
typedef Box<Point3<double>> BoxD;
typedef Box<Point3<float>> BoxF;

} // End namespace geom


/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOM_BBOX_H
