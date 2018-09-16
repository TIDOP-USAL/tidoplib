#ifndef TL_GEOM_POINT_H
#define TL_GEOM_POINT_H

#include "config_tl.h"

#include <limits>
#include <numeric>
#include <array>

#include "tidop/core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/types.hpp"
#endif // HAVE_OPENCV

#include "tidop/geometry/entities/entity.h"

namespace TL
{

/*! \addtogroup GeometricEntities
 *  \{
 */


namespace geometry
{


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase punto 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Point<int> PointI;
 * typedef Point<double> PointD;
 * typedef Point<float> PointF;
 * \endcode
 */
template<typename T>
class Point 
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Coordenada X
   */
  T x;

  /*!
   * \brief Coordenada Y
   */
  T y;

public:

  /*!
   * \brief Constructora por defecto
   */
  Point();

  /*!
   * \brief Constructor
   * \param[in] x Coordenada x
   * \param[in] y Coordenada y
   */
  Point(T x, T y);

  /*!
   * \brief Constructor de copia
   * \param[in] point Objeto Point que se copia
   */
  Point(const Point<T> &point);

  /*!
   * \brief Constructor de movimiento
   * \param[in] point Objeto Point que se mueve
   */
  Point(Point<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] v Objeto Point que se copia
   */
  Point(const std::array<T, 2> &v);


  /*!
   * \brief Operador de asignación
   * \param[in] point Objeto Point que se copia
   */
  Point<T> &operator = (const Point<T>& point);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] point Objeto Point que se mueve
   */
  Point<T> &operator = (Point<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Point<T2>() const;

  /*!
   * \brief Conversión a un punto 3D
   */
  template<typename T2> operator Point3<T2>() const;

#ifdef HAVE_OPENCV
  /*!
   * \brief Conversión a punto OpenCV
   */
  template<typename T2> operator cv::Point_<T2>() const;
#endif

};

typedef Point<int> PointI;
typedef Point<double> PointD;
typedef Point<float> PointF;


template<typename T> inline
Point<T>::Point()
  : Entity(Entity::type::POINT_2D), 
    x(static_cast<T>(0)), 
    y(static_cast<T>(0)) 
{
}

template<typename T> inline
Point<T>::Point(T x, T y)
  : Entity(Entity::type::POINT_2D), 
    x(x), 
    y(y) 
{
}

template<typename T> inline
Point<T>::Point(const Point &pt)
  : Entity(Entity::type::POINT_2D), 
    x(pt.x), 
    y(pt.y) 
{
}

template<typename T> inline
Point<T>::Point(Point &&pt) TL_NOEXCEPT
  : Entity(std::forward<Entity>(pt)), 
    x(std::move(pt.x)), 
    y(std::move(pt.y)) 
{
}

template<typename T> inline
Point<T>::Point(const std::array<T, 2> &v)
  : Entity(Entity::type::POINT_2D), 
    x(v[0]), 
    y(v[1]) 
{

}

template<typename T> inline
Point<T>& Point<T>::operator = (const Point &pt)
{
  if (this != &pt) {
    this->x = pt.x;
    this->y = pt.y;
  }
  return *this;
}


template<typename T> inline
Point<T>& Point<T>::operator = (Point &&pt) TL_NOEXCEPT
{
  if (this != &pt) {
    this->mEntityType = std::move(pt.mEntityType);
    this->x = std::move(pt.x);
    this->y = std::move(pt.y);
  }
  return *this;
}


TL_DISABLE_WARNING(TL_WARNING_C4244)
template<typename T> template<typename T2> inline
Point<T>::operator Point<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point<T2>(static_cast<T2>(std::round(this->x)), 
                     static_cast<T2>(std::round(this->y)));
  } else {
    return Point<T2>(static_cast<T2>(this->x), static_cast<T2>(this->y));
  }
}

template<typename T> template<typename T2> inline
Point<T>::operator Point3<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point3<T2>(static_cast<T2>(std::round(this->x)), 
                      static_cast<T2>(std::round(this->y)),
                      0);
  } else {
    return Point3<T2>(static_cast<T2>(this->x), 
                      static_cast<T2>(this->y), 
                      0);
  }
}


#ifdef HAVE_OPENCV
template<typename T> template<typename T2> inline
Point<T>::operator cv::Point_<T2>() const
{
  if (std::is_integral<T2>::value) {
    return cv::Point_<T2>(static_cast<T2>(std::round(this->x)), 
                          static_cast<T2>(std::round(this->y)));
  } else {
    return cv::Point_<T2>(static_cast<T2>(this->x), static_cast<T2>(this->y));
  }
}
#endif
TL_ENABLE_WARNING(TL_WARNING_C4244)

template<typename T> static inline
Point<T>& operator += (Point<T> &pt1, const Point<T> &pt2)
{
  pt1.x += pt2.x;
  pt1.y += pt2.y;
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator += (Point<T1> &pt1, const Point<T2> &pt2)
{
  pt1 += static_cast<Point<T1>>(pt2);
  return pt1;
}

template<typename T> static inline
Point<T>& operator -= (Point<T> &pt1, const Point<T> &pt2)
{
  pt1.x -= pt2.x;
  pt1.y -= pt2.y;
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator -= (Point<T1> &pt1, const Point<T2> &pt2)
{
  pt1 -= static_cast<Point<T1>>(pt2);
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator *= (Point<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x * b));
    pt.y = static_cast<T1>(std::round(pt.y * b));
  } else {
    pt.x = static_cast<T1>(pt.x * b);
    pt.y = static_cast<T1>(pt.y * b);
  }
  return pt;
}

template<typename T1, typename T2> static inline
Point<T1> &operator /= (Point<T1> &pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x / static_cast<double>(b)));
    pt.y = static_cast<T1>(std::round(pt.y / static_cast<double>(b)));
  } else {
    pt.x = static_cast<T1>(pt.x / b);
    pt.y = static_cast<T1>(pt.y / b);
  }
  return pt;
}

template<typename T> static inline
bool operator == (const Point<T> &pt1, const Point<T> &pt2)
{
  return pt1.x == pt2.x && pt1.y == pt2.y;
}

template<typename T> static inline
bool operator != (const Point<T> &pt1, const Point<T> &pt2)
{
  return pt1.x != pt2.x || pt1.y != pt2.y;
}

template<typename T> static inline
Point<T> operator + (const Point<T> &pt1, const Point<T> &pt2)
{
  return Point<T>(pt1.x + pt2.x, pt1.y + pt2.y);
}

template<typename T> static inline
Point<T> operator - (const Point<T> &pt1, const Point<T> &pt2)
{
  return Point<T>(pt1.x - pt2.x, pt1.y - pt2.y);
}

template<typename T> static inline
Point<T> operator - (const Point<T> &pt1)
{
  return Point<T>(-pt1.x, -pt1.y);
}

template<typename T1, typename T2> static inline
Point<T1> operator * (const Point<T1> &pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point<T1>(static_cast<T1>(std::round(pt.x*b)), 
                     static_cast<T1>(std::round(pt.y*b)));
  } else {
    return Point<T1>(static_cast<T1>(pt.x*b), static_cast<T1>(pt.y*b));
  }
}

template<typename T1, typename T2> static inline
Point<T2> operator * (T1 a, const Point<T2> &b)
{
  if (std::is_integral<T2>::value) {
    return Point<T2>(static_cast<T2>(std::round(b.x*a)), 
                     static_cast<T2>(std::round(b.y*a)));
  } else {
    return Point<T2>(static_cast<T2>(b.x*a), static_cast<T2>(b.y*a));
  }
}


template<typename T1, typename T2> static inline
Point<T1> operator / (const Point<T1> &pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point<T1>(static_cast<T1>(std::round(pt.x / static_cast<double>(b))),
                     static_cast<T1>(std::round(pt.y / static_cast<double>(b))));
  } else {
    return Point<T1>(static_cast<T1>(pt.x / b), static_cast<T1>(pt.y / b));
  }
}


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Clase punto 3D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Point<int> Point3I;
 * typedef Point<double> Point3D;
 * typedef Point<float> Point3F;
 * \endcode
 */
template<typename T>
class Point3 
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Coordenada X
   */
  T x;

  /*!
   * \brief Coordenada Y
   */
  T y;

  /*!
   * \brief Coordenada Z
   */ 
  T z;

public:

  /*!
   * \brief Constructora por defecto
   */
  Point3();

  /*!
   * \brief Constructor
   * \param[in] x Coordenada x
   * \param[in] y Coordenada y
   * \param[in] z Coordenada z
   */
  Point3(T x, T y, T z);

  /*!
   * \brief Constructor de copia
   * \param[in] point Objeto Point3 que se copia
   */
  Point3(const Point3<T> &point);

  /*!
   * \brief Constructor de movimiento
   * \param[in] point Objeto Point que se mueve
   */
  Point3(Point3<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Constructor vector
   * \param[in] v Vector de coordenadas
   */
  Point3(const std::array<T, 3> &v);

  /*!
   * \brief Operador de asignación
   * \param[in] point Objeto Point3 que se copia
   */
  Point3<T> &operator = (const Point3<T> &point);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] point Objeto Point3 que se mueve
   */
  Point3<T> &operator = (Point3<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Point3<T2>() const;
  template<typename T2> operator Point<T2>() const;
};

typedef Point3<int> Point3I;
typedef Point3<double> Point3D;
typedef Point3<float> Point3F;


template<typename T> inline
Point3<T>::Point3()
  : Entity(Entity::type::POINT_3D),
    x(0), 
    y(0), 
    z(0)
{
}

template<typename T> inline
Point3<T>::Point3(T x, T y, T z)
  : Entity(Entity::type::POINT_3D), 
    x(x), 
    y(y), 
    z(z)
{
}

template<typename T> inline
Point3<T>::Point3(const Point3 &pt)
  : Entity(pt),
    x(pt.x), 
    y(pt.y), 
    z(pt.z)
{
}

template<typename T> inline
Point3<T>::Point3(Point3 &&pt) TL_NOEXCEPT
  : Entity(std::forward<Entity>(pt)),
    x(std::move(pt.x)), 
    y(std::move(pt.y)), 
    z(std::move(pt.z))
{
}

template<typename T> inline
Point3<T>::Point3(const std::array<T, 3> &v)
  : Entity(Entity::type::POINT_3D), 
    x(v[0]), 
    y(v[1]),
    z(v[2])
{
}

template<typename T> inline
Point3<T> &Point3<T>::operator = (const Point3 &pt)
{
  if (this != &pt) {
    this->x = pt.x;
    this->y = pt.y;
    this->z = pt.z;
  }
  return *this;
}

template<typename T> inline
Point3<T> &Point3<T>::operator = (Point3 &&pt) TL_NOEXCEPT
{
  if (this != &pt) {
    Entity::operator = (std::forward<Entity>(pt));
    this->x = std::move(pt.x);
    this->y = std::move(pt.y);
    this->z = std::move(pt.z);
  }
  return *this;
}
TL_DISABLE_WARNING(TL_WARNING_C4244)
template<typename T> template<typename T2> inline
Point3<T>::operator Point3<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point3<T2>(static_cast<T2>(std::round(this->x)), 
                      static_cast<T2>(std::round(this->y)), 
                      static_cast<T2>(std::round(this->z)));
  } else {
    return Point3<T2>(static_cast<T2>(this->x), 
                      static_cast<T2>(this->y), 
                      static_cast<T2>(this->z));
  }
}

template<typename T> template<typename T2> inline
Point3<T>::operator Point<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point<T2>(static_cast<T2>(std::round(this->x)), 
                     static_cast<T2>(std::round(this->y)));
  } else {
    return Point<T2>(static_cast<T2>(this->x), 
                     static_cast<T2>(this->y));
  }
}
TL_ENABLE_WARNING(TL_WARNING_C4244)

template<typename T> static inline
Point3<T>& operator += (Point3<T>& pt1, const Point3<T>& pt2)
{
  pt1.x += pt2.x;
  pt1.y += pt2.y;
  pt1.z += pt2.z;
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator += (Point3<T1>& pt1, const Point3<T2>& pt2)
{
  pt1 += static_cast<Point3<T1>>(pt2);
  return pt1;
}

template<typename T> static inline
Point3<T>& operator -= (Point3<T>& pt1, const Point3<T>& pt2)
{
  pt1.x -= pt2.x;
  pt1.y -= pt2.y;
  pt1.z -= pt2.z;
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator -= (Point3<T1>& pt1, const Point3<T2>& pt2)
{
  pt1 -= static_cast<Point3<T1>>(pt2);
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator *= (Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x * b));
    pt.y = static_cast<T1>(std::round(pt.y * b));
    pt.z = static_cast<T1>(std::round(pt.z * b));
  } else {
    pt.x = static_cast<T1>(pt.x * b);
    pt.y = static_cast<T1>(pt.y * b);
    pt.z = static_cast<T1>(pt.z * b);
  }
  return pt;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator /= (Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x / static_cast<double>(b)));
    pt.y = static_cast<T1>(std::round(pt.y / static_cast<double>(b)));
    pt.z = static_cast<T1>(std::round(pt.z / static_cast<double>(b)));
  } else {
    pt.x = static_cast<T1>(pt.x / static_cast<double>(b));
    pt.y = static_cast<T1>(pt.y / static_cast<double>(b));
    pt.z = static_cast<T1>(pt.z / static_cast<double>(b));
  }
  return pt;
}

template<typename T> static inline
bool operator == (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return pt1.x == pt2.x && pt1.y == pt2.y && pt1.z == pt2.z;
}

template<typename T> static inline
bool operator != (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return pt1.x != pt2.x || pt1.y != pt2.y || pt1.z != pt2.z;
}

template<typename T> static inline
Point3<T> operator + (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return Point3<T>(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
}

template<typename T> static inline
Point3<T> operator - (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return Point3<T>(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
}

template<typename T> static inline
Point3<T> operator - (const Point3<T>& pt1)
{
  return Point3<T>(-pt1.x, -pt1.y, -pt1.z);
}

template<typename T1, typename T2> static inline
Point3<T1> operator * (const Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point3<T1>(static_cast<T1>(std::round(pt.x*b)), 
                      static_cast<T1>(std::round(pt.y*b)), 
                      static_cast<T1>(std::round(pt.z*b)));
  } else {
    return Point3<T1>(static_cast<T1>(pt.x*b), 
                      static_cast<T1>(pt.y*b), 
                      static_cast<T1>(pt.z*b));
  }
}

template<typename T1, typename T2> static inline
Point3<T2> operator * (T1 a, const Point3<T2>& pt)
{
  if (std::is_integral<T2>::value) {
    return Point3<T2>(static_cast<T2>(std::round(pt.x*a)), 
                      static_cast<T2>(std::round(pt.y*a)), 
                      static_cast<T2>(std::round(pt.z*a)));
  } else {
    return Point3<T2>(static_cast<T2>(pt.x*a), 
                      static_cast<T2>(pt.y*a), 
                      static_cast<T2>(pt.z*a));
  }
}

template<typename T1, typename T2> static inline
Point3<T1> operator / (const Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point3<T1>(static_cast<T1>(std::round(pt.x/static_cast<double>(b))), 
                      static_cast<T1>(std::round(pt.y/static_cast<double>(b))), 
                      static_cast<T1>(std::round(pt.z/static_cast<double>(b))));
  } else {
    return Point3<T1>(static_cast<T1>(pt.x/b), 
                      static_cast<T1>(pt.y/b), 
                      static_cast<T1>(pt.z/b));
  }
}



/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase multi-punto
 *
 * Esta template representa un conjunto de puntos relaccionados que se agrupan
 * en una misma entidad multipunto.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef MultiPoint<int> MultiPointI;
 * typedef MultiPoint<double> MultiPointD;
 * typedef MultiPoint<float> MultiPointF;
 * \endcode
 */
template<typename Point_t>
class MultiPoint 
  : public Entity, 
    public Entities2D<Point_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva para el contenedor
   */
  MultiPoint(typename MultiPoint<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint(const MultiPoint &multiPoint);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiPoint Objeto MultiPoint que se mueve
   */
  MultiPoint(MultiPoint &&multiPoint) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  MultiPoint(const std::vector<Point_t> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint(std::initializer_list<Point_t> listPoints);

  /*!
   * \brief Destructora
   */
  ~MultiPoint() {}

  /*!
   * \brief Operador asignación
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint<Point_t> &operator = (const MultiPoint &multiPoint);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] multiPoint Objeto MultiPoint que se mueve
   */
  MultiPoint<Point_t> &operator = (MultiPoint &&multiPoint) TL_NOEXCEPT; 

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<Point_t> getWindow() const;
};

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint() 
  : Entity(Entity::type::MULTIPOINT_2D), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(typename MultiPoint<Point_t>::size_type size)
  : Entity(Entity::type::MULTIPOINT_2D),
    Entities2D<Point_t>(size) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(const MultiPoint &multiPoint) 
  : Entity(Entity::type::MULTIPOINT_2D), 
    Entities2D<Point_t>(multiPoint) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(MultiPoint &&multiPoint) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPoint)), 
    Entities2D<Point_t>(std::forward<MultiPoint<Point_t>>(multiPoint)) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(const std::vector<Point_t> &vPoint) 
  : Entity(Entity::type::MULTIPOINT_2D), 
    Entities2D<Point_t>(vPoint) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(std::initializer_list<Point_t> listPoints) 
  : Entity(Entity::type::MULTIPOINT_2D), 
    Entities2D<Point_t>(listPoints)
{
}

template<typename Point_t> inline
MultiPoint<Point_t> &MultiPoint<Point_t>::operator = (const MultiPoint &multiPoint)
{
  if (this != &multiPoint) {
    this->mEntityType = multiPoint.mEntityType;
    //Entities2D<Point_t>::operator = (multiPoint);
    this->mEntities = multiPoint.mEntities;
  }
  return *this;
}

template<typename Point_t> inline
MultiPoint<Point_t> &MultiPoint<Point_t>::operator = (MultiPoint &&multiPoint) TL_NOEXCEPT
{
  if (this != &multiPoint) {
    this->mEntityType = std::move(multiPoint.mEntityType);
    Entities2D<Point_t>::operator = (std::forward<MultiPoint<Point_t>>(multiPoint));
  }
  return *this;
}

template<typename Point_t> inline
Window<Point_t> MultiPoint<Point_t>::getWindow() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (w.pt1.x > this->mEntities[i].x) w.pt1.x = this->mEntities[i].x;
    if (w.pt1.y > this->mEntities[i].y) w.pt1.y = this->mEntities[i].y;
    if (w.pt2.x < this->mEntities[i].x) w.pt2.x = this->mEntities[i].x;
    if (w.pt2.y < this->mEntities[i].y) w.pt2.y = this->mEntities[i].y;
  }
  return w;
}


typedef MultiPoint<Point<int>> MultiPointI;
typedef MultiPoint<Point<double>> MultiPointD;
typedef MultiPoint<Point<float>> MultiPointF;

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase multi-punto 3D
 *
 * Esta template representa un conjunto de puntos relaccionados que se agrupan
 * en una misma entidad multipunto.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef MultiPoint3D<int> MultiPoint3dI;
 * typedef MultiPoint3D<double> MultiPoint3dD;
 * typedef MultiPoint3D<float> MultiPoint3dF;
 * \endcode
 */
template<typename Point_t>
class MultiPoint3D
  : public Entity,
    public Entities3D<Point_t>
{
  
public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva para el contenedor
   */
  MultiPoint3D(typename MultiPoint3D<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint3D(const MultiPoint3D &multiPoint);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiPoint Objeto MultiPoint3D que se mueve
   */
  MultiPoint3D(MultiPoint3D &&multiPoint) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  MultiPoint3D(const std::vector<Point_t> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint3D(std::initializer_list<Point_t> listPoints);
  
  /*!
   * \brief Destructora
   */
  ~MultiPoint3D() {}

  /*!
   * \brief Operador asignación
   * \param[in] multiPoint Objeto MultiPoint3D que se copia
   */
  MultiPoint3D<Point_t> &operator = (const MultiPoint3D &multiPoint);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] multiPoint Objeto MultiPoint3D que se mueve
   */
  MultiPoint3D<Point_t> &operator = (MultiPoint3D &&multiPoint) TL_NOEXCEPT;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los puntos
   */
  Box<Point_t> getBox() const;
};

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D() 
  : Entity(Entity::type::MULTIPOINT_3D), 
    Entities3D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(typename MultiPoint3D<Point_t>::size_type size)
  : Entity(Entity::type::MULTIPOINT_3D),
    Entities3D<Point_t>(size) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(const MultiPoint3D &multiPoint) 
  : Entity(multiPoint),
    Entities3D<Point_t>(multiPoint) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(MultiPoint3D &&multiPoint) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPoint)),
    Entities3D<Point_t>(std::forward<MultiPoint3D<Point_t>>(multiPoint)) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(const std::vector<Point_t> &vPoint) 
  : Entity(Entity::type::MULTIPOINT_3D),
    Entities3D<Point_t>(vPoint) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(std::initializer_list<Point_t> listPoints) 
  : Entity(Entity::type::MULTIPOINT_3D),
    Entities3D<Point_t>(listPoints)
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t> &MultiPoint3D<Point_t>::operator = (const MultiPoint3D &multiPoint)
{
  if (this != &multiPoint) {
    this->mEntityType = multiPoint.mEntityType;
    this->mEntities = multiPoint.mEntities;
  }
  return *this;
}

template<typename Point_t> inline
MultiPoint3D<Point_t> &MultiPoint3D<Point_t>::operator = (MultiPoint3D &&multiPoint) TL_NOEXCEPT
{
  if (this != &multiPoint) {
    this->mEntityType = std::move(multiPoint.mEntityType);
    Entities3D<Point_t>::operator = (std::forward<MultiPoint3D<Point_t>>(multiPoint));
  }
  return *this;
}

template<typename Point_t> inline
Box<Point_t> MultiPoint3D<Point_t>::getBox() const
{
  Box<Point_t> box;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (box.pt1.x > this->mEntities[i].x) box.pt1.x = this->mEntities[i].x;
    if (box.pt1.y > this->mEntities[i].y) box.pt1.y = this->mEntities[i].y;
    if (box.pt1.z > this->mEntities[i].z) box.pt1.z = this->mEntities[i].z;
    if (box.pt2.x < this->mEntities[i].x) box.pt2.x = this->mEntities[i].x;
    if (box.pt2.y < this->mEntities[i].y) box.pt2.y = this->mEntities[i].y;
    if (box.pt2.z < this->mEntities[i].z) box.pt2.z = this->mEntities[i].z;
  }
  return box;
}

typedef MultiPoint3D<Point3<int>> MultiPoint3dI;
typedef MultiPoint3D<Point3<double>> MultiPoint3dD;
typedef MultiPoint3D<Point3<float>> MultiPoint3dF;

/* ---------------------------------------------------------------------------------- */

}

//TODO: revisar y ver si se puede hacer mejor

template<typename Point_t> static inline 
TL_DEPRECATED("static_cast<Point_t>()")
Point_t point_cast(const geometry::Point<int> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
TL_DEPRECATED("static_cast<Point_t>()")
Point_t point_cast(const geometry::Point<double> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
TL_DEPRECATED("static_cast<Point_t>()")
Point_t point_cast(const geometry::Point<float> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
TL_DEPRECATED("static_cast<Point_t>()")
Point_t point_cast(const geometry::Point3<int> &pt)    
{ 
  return Point_t(pt.x, pt.y, pt.z); 
}

template<typename Point_t> static inline 
TL_DEPRECATED("static_cast<Point_t>()")
Point_t point_cast(const geometry::Point3<double> &pt)    
{ 
  return Point_t(pt.x, pt.y, pt.z); 
}

template<typename Point_t> static inline 
TL_DEPRECATED("static_cast<Point_t>()")
Point_t point_cast(const geometry::Point3<float> &pt)    
{ 
  return Point_t(pt.x, pt.y, pt.z); 
}

#ifdef HAVE_OPENCV

template<typename Point_t> static inline 
Point_t point_cast(const cv::Point_<int> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}


template<typename Point_t> static inline 
Point_t point_cast(const cv::Point_<double> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
Point_t point_cast(const cv::Point_<float> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

#endif

/*! \} */ // end of GeometricEntities

} // End namespace TL

#endif // TL_GEOM_POINT_H
