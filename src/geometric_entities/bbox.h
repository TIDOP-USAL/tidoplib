#ifndef I3D_BBOX_H
#define I3D_BBOX_H

#include <algorithm>

#include "opencv2/core/core.hpp"

#include "core/defs.h"
#include "geometric_entities/entity.h"


namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/*!
 * \brief The Window class
 */
//template<typename T>
//class I3D_DEPRECATED("geometry::Bbox<Point3<T>>") Bbox : public Entity<T>
//{
//public:
//
//  /*!
//   * \brief type
//   */
//  typedef T value_type;
//
//  /*!
//   * \brief Punto 1
//   */
//  Point3<T> pt1;
//
//  /*!
//   * \brief Punto 2
//   */
//  Point3<T> pt2;
//
//public:
//
//  /*!
//   * \brief Constructor por defecto
//   */
//  Bbox();
//
//  /*!
//   * \brief Constructor de copia
//   */
//  Bbox(const Bbox &box);
//
//  /*!
//   * \brief Constructor
//   * \param[in] pt1 Primer punto
//   * \param[in] pt2 Segundo punto
//   */
//  Bbox(const Point3<T> &pt1, const Point3<T> &pt2);
//
//  /*!
//   * \brief Constructor
//   * \param[in] pt Punto central
//   * \param[in] sxx Ancho de la ventana
//   * \param[in] szy profundidad de la ventana
//   * \param[in] szz Alto de la ventana 
//   * tendria que mirar el tema de los ejes...
//   */
//  Bbox(Point3<T> &pt, T sxx, T szy, T szz);
//
//  /*!
//   * \brief Constructor 
//   * \param[in] pt Punto central
//   * \param[in] sz Dimensiones
//   */
//  Bbox(Point3<T> &pt, T sz);
//
//  //~Bbox();
//
//  /*!
//   * \brief Sobrecarga del operador  de asignación
//   * \param[in] bbox Bbox que se asigna
//   * \return Referencia al Bbox
//   */
//  Bbox &operator = (const Bbox &bbox);
//
//  /*!
//   * \brief Sobrecarga del operador 'igual que'
//   * \param[in] bbox bbox con el que se compara
//   * \return true si ambos bbox son iguales
//   */
//  bool operator == (const Bbox &bbox) const;
//
//  /*!
//   * \brief Conversión a una ventana de un tipo diferente
//   */
//  template<typename T2> operator Bbox<T2>() const;
//
//  ///*!
//  // * \brief Devuelve el ancho de la ventana
//  // * \return Ancho
//  // */
//  //T getWidth() const { return pt2.x - pt1.x; }
//
//  ///*!
//  // * \brief Devuelve el alto de la ventana
//  // * \return Alto
//  // */
//  //T getHeight() const { return pt2.y - pt1.y; }
//
//  /*!
//   * \brief Devuelve centro del Bbox
//   * \return Centro del Bbox
//   */
//  Point3<T> getCenter() const;
//
//  /*!
//   * \brief Comprueba si el Bbox esta vacio
//   * \return True si el Bbox es nula
//   */
//  bool isEmpty() const;
//
//  /*!
//   * \brief Comprueba si un punto está contenido dentro del Bbox
//   * \param[in] pt Punto
//   * \return true si el punto esta dentro del Bbox
//   */
//  template<typename T2> bool containsPoint(const Point3<T2> &pt) const;
//
//  /*!
//   * \brief La ventana contiene la ventana
//   */
//  template<typename T2> bool containsBbox(const Bbox<T2> &bbox) const;
//};
//
//// Definición de métodos
//
//template<typename T> inline
//Bbox<T>::Bbox() 
//: Entity<T>(entity_type::BBOX), pt1(std::numeric_limits<T>().max(), std::numeric_limits<T>().max(), std::numeric_limits<T>().max()), 
//  pt2(-std::numeric_limits<T>().max(), -std::numeric_limits<T>().max(), -std::numeric_limits<T>().max()) {}
//
//template<typename T> inline
//Bbox<T>::Bbox(const Bbox &bbox) 
//  : Entity<T>(entity_type::BBOX), pt1(bbox.pt1), pt2(bbox.pt2) {}
//
//template<typename T> inline
//Bbox<T>::Bbox(const Point3<T> &_pt1, const Point3<T> &_pt2) 
//  : Entity<T>(entity_type::BBOX)
//{
//  pt1.x = std::min(_pt1.x, _pt2.x);
//  pt1.y = std::min(_pt1.y, _pt2.y);
//  pt1.z = std::min(_pt1.z, _pt2.z);
//  pt2.x = std::max(_pt1.x, _pt2.x);
//  pt2.y = std::max(_pt1.y, _pt2.y);
//  pt2.z = std::max(_pt1.z, _pt2.z);
//}
//
//template<typename T> inline
//Bbox<T>::Bbox(Point3<T> &_pt, T sxx, T szy, T szz) 
//  : Entity<T>(entity_type::BBOX)
//{ 
//  if (typeid(T) == typeid(int)) {
//    int sxx_2 = I3D_ROUND_TO_INT(sxx / 2);
//    int szy_2 = I3D_ROUND_TO_INT(szy / 2);
//    int szz_2 = I3D_ROUND_TO_INT(szz / 2);
//    int dx = static_cast<int>(sxx) % 2;
//    int dy = static_cast<int>(szy) % 2;
//    int dz = static_cast<int>(szz) % 2;
//    pt1 = Point3<T>(_pt.x - sxx_2, _pt.y - szy_2, _pt.z - szz_2);
//    pt2 = Point3<T>(_pt.x + sxx_2 + dx, _pt.y + szy_2 + dy, _pt.z + szz_2 + dz);
//  } else {
//    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
//    I3D_DISABLE_WARNING(4244)
//    pt1 = Point3<T>(_pt.x - sxx / 2., _pt.y - szy / 2., _pt.z - szz / 2.);
//    pt2 = Point3<T>(_pt.x + sxx / 2., _pt.y + szy / 2., _pt.z + szz / 2.);
//    I3D_ENABLE_WARNING(4244)
//  }
//}
//
//template<typename T> inline
//Bbox<T>::Bbox(Point3<T> &_pt, T sz) 
//  : Entity<T>(entity_type::BBOX) 
//{
//  if (typeid(T) == typeid(int)) {
//    int sz_2 = I3D_ROUND_TO_INT(sz / 2);
//    int dxyz = static_cast<int>(sz) % 2;
//    pt1 = Point3<T>(_pt.x - sz_2, _pt.y - sz_2, _pt.z - sz_2);
//    pt2 = Point3<T>(_pt.x + sz_2 + dxyz, _pt.y + sz_2 + dxyz, _pt.z + sz_2 + dxyz);
//  } else {
//    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
//    I3D_DISABLE_WARNING(4244)
//    int sz_2 = sz / 2.;
//    pt1 = Point3<T>(_pt.x - sz_2, _pt.y - sz_2, _pt.z - sz_2);
//    pt2 = Point3<T>(_pt.x + sz_2, _pt.y + sz_2, _pt.z + sz_2);
//    I3D_ENABLE_WARNING(4244)
//  }
//}
//
//template<typename T> inline
//Bbox<T> &Bbox<T>::operator = (const Bbox &bbox)
//{
//  if (this != &bbox) {
//    this->pt1 = bbox.pt1;
//    this->pt2 = bbox.pt2;
//    this->mEntityType = bbox.mEntityType;
//  }
//  return *this;
//}
//
//template<typename T> inline
//bool Bbox<T>::operator == (const Bbox &bbox) const
//{
//  return (pt1 == bbox.pt1 && pt2 == bbox.pt2);
//}
//
//template<typename T> template<typename T2> inline
//Bbox<T>::operator Bbox<T2>() const
//{
//  if (typeid(T2) == typeid(int)) {
//    //Dos posibles soluciones. Ver cual es mas eficiente
//    Point3<T2> _pt1(I3D_ROUND_TO_INT(pt1.x), I3D_ROUND_TO_INT(pt1.y), I3D_ROUND_TO_INT(pt1.z));
//    Point3<T2> _pt2(I3D_ROUND_TO_INT(pt2.x), I3D_ROUND_TO_INT(pt2.y), I3D_ROUND_TO_INT(pt2.z));
//    return Bbox<T2>(_pt1, _pt2);
//  } else {
//    Point3<T2> _pt1 = pt1;
//    Point3<T2> _pt2 = pt2;
//    return Bbox<T2>(_pt1, _pt2);
//  }
//}
//
//template<typename T> inline
//Point3<T> Bbox<T>::getCenter() const
//{
//  if (typeid(T) == typeid(int)) {
//    return Point3<T>(static_cast<int>(std::floor((pt1.x + pt2.x) / 2)), 
//                          static_cast<int>(std::floor((pt1.y + pt2.y) / 2)), 
//                          static_cast<int>(std::floor((pt1.z + pt2.z) / 2)));
//  } else {
//    I3D_DISABLE_WARNING(4244)
//    return Point3<T>((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2., (pt1.z + pt2.z) / 2.);
//    I3D_ENABLE_WARNING(4244)
//  }
//}
//
//template<typename T> inline
//bool Bbox<T>::isEmpty() const 
//{ 
//  return (   pt1.x == std::numeric_limits<T>().max() 
//          && pt1.y == std::numeric_limits<T>().max() 
//          && pt1.z == std::numeric_limits<T>().max() 
//          && pt2.x == -std::numeric_limits<T>().max() 
//          && pt2.y == -std::numeric_limits<T>().max()
//          && pt2.z == -std::numeric_limits<T>().max()); 
//}
//
//template<typename T> template<typename T2> inline
//bool Bbox<T>::containsPoint(const Point3<T2> &pt) const
//{
//  return ((pt2.x >= pt.x) && (pt2.y >= pt.y) && (pt2.z >= pt.z)
//       && (pt1.x <= pt.x) && (pt1.y <= pt.y) && (pt1.z <= pt.z));
//}
//
//template<typename T> template<typename T2> inline
//bool Bbox<T>::containsBbox(const Bbox<T2> &bbox) const
//{
//  return (pt1.x <= bbox.pt1.x && pt1.y <= bbox.pt1.y && pt1.z <= bbox.pt1.z &&
//          pt2.x >= bbox.pt2.x && pt2.y >= bbox.pt2.y && pt2.z >= bbox.pt2.z);
//}
//
//typedef Bbox<int> BboxI;
//typedef Bbox<double> BboxD;
//typedef Bbox<float> BboxF;
//




/* ---------------------------------------------------------------------------------- */


namespace geometry
{

/*!
 * \brief The Window class
 */
template<typename Point3_t>
class Box : public Entity
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
   */
  Box(const Box &box);

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
   * \param[in] bbox Bbox que se asigna
   * \return Referencia al Bbox
   */
  Box &operator = (const Box &box);

  /*!
   * \brief Sobrecarga del operador 'igual que'
   * \param[in] bbox bbox con el que se compara
   * \return true si ambos bbox son iguales
   */
  bool operator == (const Box &box) const;

  /*!
   * \brief Conversión a una ventana de un tipo diferente
   */
  template<typename Point_t2> operator Box<Point_t2>() const;

  ///*!
  // * \brief Devuelve el ancho de la ventana
  // * \return Ancho
  // */
  //T getWidth() const { return pt2.x - pt1.x; }

  ///*!
  // * \brief Devuelve el alto de la ventana
  // * \return Alto
  // */
  //T getHeight() const { return pt2.y - pt1.y; }

  /*!
   * \brief Devuelve centro del Bbox
   * \return Centro del Bbox
   */
  Point3_t getCenter() const;

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
  template<typename Point3_t2> bool containsPoint(const Point3_t2 &pt) const;

  /*!
   * \brief La ventana contiene la ventana
   */
  template<typename Point3_t2> bool containsBbox(const Box<Point3_t2> &box) const;
};

// Definición de métodos

template<typename Point3_t> inline
Box<Point3_t>::Box() 
  : Entity(Entity::type::BOX), 
    pt1(std::numeric_limits<typename Point3_t::value_type>().max(), std::numeric_limits<typename Point3_t::value_type>().max(), std::numeric_limits<typename Point3_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point3_t::value_type>().max(), -std::numeric_limits<typename Point3_t::value_type>().max(), -std::numeric_limits<typename Point3_t::value_type>().max()) 
{
}

template<typename Point3_t> inline
Box<Point3_t>::Box(const Box &box) 
  : Entity(Entity::type::BOX), 
    pt1(bbox.pt1), pt2(bbox.pt2) 
{
}

template<typename Point3_t> inline
Box<Point3_t>::Box(const Point3_t &_pt1, const Point3_t &_pt2) 
  : Entity(Entity::type::BOX)
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
  : Entity(Entity::type::BOX)
{ 
  if (typeid(typename Point3_t::value_type) == typeid(int)) {
    int sxx_2 = I3D_ROUND_TO_INT(sxx / 2);
    int szy_2 = I3D_ROUND_TO_INT(szy / 2);
    int szz_2 = I3D_ROUND_TO_INT(szz / 2);
    int dx = static_cast<int>(sxx) % 2;
    int dy = static_cast<int>(szy) % 2;
    int dz = static_cast<int>(szz) % 2;
    pt1 = Point3_t(_pt.x - sxx_2, _pt.y - szy_2, _pt.z - szz_2);
    pt2 = Point3_t(_pt.x + sxx_2 + dx, _pt.y + szy_2 + dy, _pt.z + szz_2 + dz);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    I3D_DISABLE_WARNING(4244)
    pt1 = Point3_t(_pt.x - sxx / 2., _pt.y - szy / 2., _pt.z - szz / 2.);
    pt2 = Point3_t(_pt.x + sxx / 2., _pt.y + szy / 2., _pt.z + szz / 2.);
    I3D_ENABLE_WARNING(4244)
  }
}

template<typename Point3_t> template<typename T> inline
Box<Point3_t>::Box(const Point3_t &_pt, T sz) 
  : Entity(Entity::type::BOX) 
{
  if (typeid(typename Point3_t::value_type) == typeid(int)) {
    int sz_2 = I3D_ROUND_TO_INT(sz / 2);
    int dxyz = static_cast<int>(sz) % 2;
    pt1 = Point3_t(_pt.x - sz_2, _pt.y - sz_2, _pt.z - sz_2);
    pt2 = Point3_t(_pt.x + sz_2 + dxyz, _pt.y + sz_2 + dxyz, _pt.z + sz_2 + dxyz);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    I3D_DISABLE_WARNING(4244)
    int sz_2 = sz / 2.;
    pt1 = Point3_t(_pt.x - sz_2, _pt.y - sz_2, _pt.z - sz_2);
    pt2 = Point3_t(_pt.x + sz_2, _pt.y + sz_2, _pt.z + sz_2);
    I3D_ENABLE_WARNING(4244)
  }
}

template<typename Point3_t> inline
Box<Point3_t> &Box<Point3_t>::operator = (const Box &box)
{
  if (this != &box) {
    this->pt1 = box.pt1;
    this->pt2 = box.pt2;
    this->mEntityType = box.mEntityType;
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
  if (typeid(typename Point3_t2::value_type) == typeid(int)) {
    Point3_t2 _pt1(I3D_ROUND_TO_INT(pt1.x), I3D_ROUND_TO_INT(pt1.y), I3D_ROUND_TO_INT(pt1.z));
    Point3_t2 _pt2(I3D_ROUND_TO_INT(pt2.x), I3D_ROUND_TO_INT(pt2.y), I3D_ROUND_TO_INT(pt2.z));
    return Bbox<Point3_t2>(_pt1, _pt2);
  } else {
    Point3_t2 _pt1 = pt1;
    Point3_t2 _pt2 = pt2;
    return Bbox<Point3_t2>(_pt1, _pt2);
  }
}

template<typename Point3_t> inline
Point3_t Box<Point3_t>::getCenter() const
{
  if (typeid(typename Point3_t::value_type) == typeid(int)) {
    return Point3_t(I3D_ROUND_TO_INT((pt1.x + pt2.x) / 2), 
                    I3D_ROUND_TO_INT((pt1.y + pt2.y) / 2), 
                    I3D_ROUND_TO_INT((pt1.z + pt2.z) / 2));
  } else {
    I3D_DISABLE_WARNING(4244)
    return Point3_t((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2., (pt1.z + pt2.z) / 2.);
    I3D_ENABLE_WARNING(4244)
  }
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

template<typename Point3_t> template<typename Point3_t2> inline
bool Box<Point3_t>::containsPoint(const Point3_t2 &pt) const
{
  return ((pt2.x >= pt.x) && (pt2.y >= pt.y) && (pt2.z >= pt.z)
       && (pt1.x <= pt.x) && (pt1.y <= pt.y) && (pt1.z <= pt.z));
}

template<typename Point3_t> template<typename Point3_t2> inline
bool Box<Point3_t>::containsBbox(const Box<Point3_t2> &box) const
{
  return (pt1.x <= box.pt1.x && pt1.y <= box.pt1.y && pt1.z <= box.pt1.z &&
          pt2.x >= box.pt2.x && pt2.y >= box.pt2.y && pt2.z >= box.pt2.z);
}

typedef Box<Point3<int>> BoxI;
typedef Box<Point3<double>> BoxD;
typedef Box<Point3<float>> BoxF;

}



/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_BBOX_H
