#ifndef I3D_POINT_H
#define I3D_POINT_H

#include <limits>
#include <numeric>

#include "core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/types.hpp"
#endif // HAVE_OPENCV

#include "geometric_entities/entity.h"
#include "geometric_entities/window.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */


namespace geometry
{

/*!
 *
 */
//template<typename Point_t> static inline
//bool isSamePoint(const Point_t &a, const Point_t &b)
//{
//  return a.x == b.x && a.y == b.y;
//}

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
class Point : public Entity
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
   * \brief Operador de asignación
   * \param[in] point Objeto Point que se copia
   */
  Point<T>& operator = (const Point<T>& point);

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Point<T2>() const;

  /*!
   * \brief Conversión a punto OpenCV
   */
  template<typename T2> operator cv::Point_<T2>() const;

};

typedef Point<int> PointI;
typedef Point<double> PointD;
typedef Point<float> PointF;


template<typename T> inline
Point<T>::Point()
  : Entity(Entity::type::POINT_2D), 
    x(0), 
    y(0) 
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
Point<T>::Point(const Point& pt)
  : Entity(Entity::type::POINT_2D), 
    x(pt.x), 
    y(pt.y) 
{
}

template<typename T> inline
Point<T>& Point<T>::operator = (const Point& pt)
{
  this->x = pt.x;
  this->y = pt.y;
  return *this;
}

I3D_DISABLE_WARNING(4244)
template<typename T> template<typename T2> inline
Point<T>::operator Point<T2>() const
{
  if (typeid(T2) == typeid(int)) {
    return Point<T2>(I3D_ROUND_TO_INT(this->x), I3D_ROUND_TO_INT(this->y));
  } else {
    return Point<T2>(static_cast<T2>(this->x), static_cast<T2>(this->y));
  }
}

#ifdef HAVE_OPENCV
template<typename T> template<typename T2> inline
Point<T>::operator cv::Point_<T2>() const
{
  if (typeid(T2) == typeid(int)) {
    return cv::Point_<T2>(I3D_ROUND_TO_INT(this->x), I3D_ROUND_TO_INT(this->y));
  } else {
    return cv::Point_<T2>(static_cast<T2>(this->x), static_cast<T2>(this->y));
  }
}
#endif
I3D_ENABLE_WARNING(4244)

template<typename T1, typename T2> static inline
Point<T1>& operator += (Point<T1> &pt1, const Point<T2> &pt2)
{
  if (typeid(T1) == typeid(int)) {
    pt1.x += I3D_ROUND_TO_INT(pt2.x);
    pt1.y += I3D_ROUND_TO_INT(pt2.y);
  } else {
    pt1.x += static_cast<T1>(pt2.x);
    pt1.y += static_cast<T1>(pt2.y);
  }
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator -= (Point<T1> &pt1, const Point<T2> &pt2)
{
  if (typeid(T1) == typeid(int)) {
    pt1.x -= I3D_ROUND_TO_INT(pt2.x);
    pt1.y -= I3D_ROUND_TO_INT(pt2.y);
  } else {
    pt1.x -= static_cast<T1>(pt2.x);
    pt1.y -= static_cast<T1>(pt2.y);
  }
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator *= (Point<T1>& pt, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    pt.x = I3D_ROUND_TO_INT(pt.x * b);
    pt.y = I3D_ROUND_TO_INT(pt.y * b);
  } else {
    pt.x = static_cast<T1>(pt.x * b);
    pt.y = static_cast<T1>(pt.y * b);
  }
  return pt;
}

template<typename T1, typename T2> static inline
Point<T1> &operator /= (Point<T1> &pt, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    pt.x = I3D_ROUND_TO_INT(pt.x / b);
    pt.y = I3D_ROUND_TO_INT(pt.y / b);
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
  if (typeid(T1) == typeid(int)) {
    return Point<T1>(I3D_ROUND_TO_INT(pt.x*b), I3D_ROUND_TO_INT(pt.y*b));
  } else {
    return Point<T1>(static_cast<T1>(pt.x*b), static_cast<T1>(pt.y*b));
  }
}

template<typename T1, typename T2> static inline
Point<T2> operator * (T1 a, const Point<T2> &b)
{
  if (typeid(T2) == typeid(int)) {
    return Point<T2>(I3D_ROUND_TO_INT(b.x*a), I3D_ROUND_TO_INT(b.y*a));
  } else {
    return Point<T2>(static_cast<T2>(b.x*a), static_cast<T2>(b.y*a));
  }
}


template<typename T1, typename T2> static inline
Point<T1> operator / (const Point<T1> &pt, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    return Point<T1>(I3D_ROUND_TO_INT(pt.x/b), I3D_ROUND_TO_INT(pt.y/b));
  } else {
    return Point<T1>(static_cast<T1>(pt.x/b), static_cast<T1>(pt.y/b));
  }
}

template<typename T1, typename T2> static inline
Point<T2> operator / (T1 a, const Point<T2> &b)
{
  if (typeid(T2) == typeid(int)) {
    return Point<T2>(I3D_ROUND_TO_INT(b.x/a), I3D_ROUND_TO_INT(b.y/a));
  } else {
    return Point<T2>(static_cast<T2>(b.x/a), static_cast<T2>(b.y/a));
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
class Point3 : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  T x;

  T y;

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
   * \param[in] point Objeto Point que se copia
   */
  Point3(const Point3<T> &point);

  /*!
   * \brief Operador de asignación
   * \param[in] point Objeto Point que se copia
   */
  Point3<T>& operator = (const Point3<T>& point);

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Point3<T2>() const;

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
{}

template<typename T> inline
Point3<T>::Point3(const Point3 &pt)
  : Entity(Entity::type::POINT_3D),
    x(pt.x), 
    y(pt.y), 
    z(pt.z)
{}

template<typename T> inline
Point3<T>& Point3<T>::operator = (const Point3& pt)
{
  this->x = pt.x;
  this->y = pt.y;
  this->z = pt.z;
  return *this;
}

I3D_DISABLE_WARNING(4244)
template<typename T> template<typename T2> inline
Point3<T>::operator Point3<T2>() const
{
  if (typeid(T2) == typeid(int)) {
    return Point3<T2>(I3D_ROUND_TO_INT(this->x), 
                      I3D_ROUND_TO_INT(this->y), 
                      I3D_ROUND_TO_INT(this->z));
  } else {
    return Point3<T2>(static_cast<T2>(this->x), 
                      static_cast<T2>(this->y), 
                      static_cast<T2>(this->z));
  }
}
I3D_ENABLE_WARNING(4244)

template<typename T1, typename T2> static inline
Point3<T1>& operator += (Point3<T1>& pt1, const Point3<T2>& pt2)
{
  if (typeid(T1) == typeid(int)) {
    pt1.x += I3D_ROUND_TO_INT(pt2.x);
    pt1.y += I3D_ROUND_TO_INT(pt2.y);
    pt1.z += I3D_ROUND_TO_INT(pt2.z);
  } else {
    pt1.x += static_cast<T1>(pt2.x);
    pt1.y += static_cast<T1>(pt2.y);
    pt1.z += static_cast<T1>(pt2.z);
  }
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator -= (Point3<T1>& pt1, const Point3<T2>& pt2)
{
  if (typeid(T1) == typeid(int)) {
    pt1.x -= I3D_ROUND_TO_INT(pt2.x);
    pt1.y -= I3D_ROUND_TO_INT(pt2.y);
    pt1.z -= I3D_ROUND_TO_INT(pt2.z);
  } else {
    pt1.x -= static_cast<T1>(pt2.x);
    pt1.y -= static_cast<T1>(pt2.y);
    pt1.z -= static_cast<T1>(pt2.z);
  }
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator *= (Point3<T1>& pt, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    pt.x = I3D_ROUND_TO_INT(pt.x * b);
    pt.y = I3D_ROUND_TO_INT(pt.y * b);
    pt.z = I3D_ROUND_TO_INT(pt.z * b);
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
  if (typeid(T1) == typeid(int)) {
    pt.x = I3D_ROUND_TO_INT(pt.x / b);
    pt.y = I3D_ROUND_TO_INT(pt.y / b);
    pt.z = I3D_ROUND_TO_INT(pt.z / b);
  } else {
    pt.x = static_cast<T1>(pt.x / b);
    pt.y = static_cast<T1>(pt.y / b);
    pt.z = static_cast<T1>(pt.z / b);
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
  if (typeid(T1) == typeid(int)) {
    return Point3<T1>(I3D_ROUND_TO_INT(pt.x*b), 
                      I3D_ROUND_TO_INT(pt.y*b), 
                      I3D_ROUND_TO_INT(pt.z*b));
  } else {
    return Point3<T1>(static_cast<T1>(pt.x*b), 
                      static_cast<T1>(pt.y*b), 
                      static_cast<T1>(pt.z*b));
  }
}

template<typename T1, typename T2> static inline
Point3<T2> operator * (T1 a, const Point3<T2>& pt)
{
  if (typeid(T2) == typeid(int)) {
    return Point3<T2>(I3D_ROUND_TO_INT(pt.x*a), 
                      I3D_ROUND_TO_INT(pt.y*a), 
                      I3D_ROUND_TO_INT(pt.z*a));
  } else {
    return Point3<T2>(static_cast<T2>(pt.x*a), 
                      static_cast<T2>(pt.y*a), 
                      static_cast<T2>(pt.z*a));
  }
}

template<typename T1, typename T2> static inline
Point3<T1> operator / (const Point3<T1>& pt, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    return Point3<T1>(I3D_ROUND_TO_INT(pt.x/b), 
                      I3D_ROUND_TO_INT(pt.y/b), 
                      I3D_ROUND_TO_INT(pt.z/b));
  } else {
    return Point3<T1>(static_cast<T1>(pt.x/b), 
                      static_cast<T1>(pt.y/b), 
                      static_cast<T1>(pt.z/b));
  }
}

template<typename T1, typename T2> static inline
Point3<T2> operator / (T1 a, const Point3<T2>& pt)
{
  if (typeid(T2) == typeid(int)) {
    return Point3<T2>(I3D_ROUND_TO_INT(pt.x/a), 
                      I3D_ROUND_TO_INT(pt.y/a), 
                      I3D_ROUND_TO_INT(pt.z/a));
  } else {
    return Point3<T2>(static_cast<T2>(pt.x/a), 
                      static_cast<T2>(pt.y/a), 
                      static_cast<T2>(pt.z/a));
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
class MultiPoint : public Entity, public Entities2D<Point_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  MultiPoint(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint(const MultiPoint &multiPoint);

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

  ~MultiPoint() {}

};

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint() 
  : Entity(Entity::type::MULTIPOINT_2D), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(int size) 
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
class MultiPoint3D : public Entity, public Entities3D<Point_t>
{
  
public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  MultiPoint3D(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint3D(const MultiPoint3D &multiPoint);

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

  ~MultiPoint3D() {}

};

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D() 
  : Entity(Entity::type::MULTIPOINT_3D), 
    Entities3D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(int size) 
  : Entity(Entity::type::MULTIPOINT_3D),
    Entities3D<Point_t>(size) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(const MultiPoint3D &multiPoint) 
  : Entity(Entity::type::MULTIPOINT_3D),
    Entities3D<Point_t>(multiPoint) 
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


typedef MultiPoint3D<Point3<int>> MultiPoint3dI;
typedef MultiPoint3D<Point3<double>> MultiPoint3dD;
typedef MultiPoint3D<Point3<float>> MultiPoint3dF;

/* ---------------------------------------------------------------------------------- */

}

//TODO: revisar y ver si se puede hacer mejor

template<typename Point_t> static inline 
Point_t point_cast(const geometry::Point<int> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
Point_t point_cast(const geometry::Point<double> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
Point_t point_cast(const geometry::Point<float> &pt)    
{ 
  return Point_t(pt.x, pt.y); 
}

template<typename Point_t> static inline 
Point_t point_cast(const geometry::Point3<int> &pt)    
{ 
  return Point_t(pt.x, pt.y, pt.z); 
}

template<typename Point_t> static inline 
Point_t point_cast(const geometry::Point3<double> &pt)    
{ 
  return Point_t(pt.x, pt.y, pt.z); 
}

template<typename Point_t> static inline 
Point_t point_cast(const geometry::Point3<float> &pt)    
{ 
  return Point_t(pt.x, pt.y, pt.z); 
}


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

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_POINT_H
