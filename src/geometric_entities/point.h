#ifndef I3D_POINT_H
#define I3D_POINT_H

#include <limits>
#include <numeric>

#include "core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "geometric_entities/entity.h"
#include "geometric_entities/window.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/*!
 * \brief Clase punto
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Point<int> PointI;
 * typedef Point<double> PointD;
 * typedef Point<float> PointF;
 * \endcode
 */
template<typename T>
class I3D_EXPORT Point
#ifdef HAVE_OPENCV
  : public cv::Point_<T>
#endif
{
  // Por si no esta activado OpenCV. Para evitar problemas ya que el
  // uso de point esta muy extendido por la libreria se copia la clase
  // Point_ de OpenCV
#ifndef HAVE_OPENCV

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

#endif

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

#ifndef HAVE_OPENCV

  //! dot product
  T dot(const Point& pt) const;
    
  //! dot product computed in double-precision arithmetics
  double ddot(const Point& pt) const;
    
  //! cross-product
  double cross(const Point& pt) const;

#else

  /*!
   * \brief Constructor de copia
   * \param[in] point Objeto Point que se copia
   */
  //Point(const cv::Point_<T> &point);


  //Para evitar su uso y no de problemas si no esta activado OpenCV
  //operator cv::Vec<T, 2>() const = delete;

  //Point(const cv::Size_<T>& sz) = delete;
  //Point(const cv::Vec<T, 2>& v) = delete;
  //bool inside(const cv::Rect_<T>& r) const = delete;

#endif

};

typedef Point<int> PointI;
typedef Point<double> PointD;
typedef Point<float> PointF;

#ifdef HAVE_OPENCV

template<typename T> inline
Point<T>::Point() : cv::Point_<T>() {}

template<typename T> inline
Point<T>::Point(T x, T y) : cv::Point_<T>(x, y) {}

template<typename T> inline
Point<T>::Point(const Point<T>& pt) : cv::Point_<T>(pt.x, pt.y) {}

//template<typename T> inline
//Point<T>::Point(const cv::Point_<T>& pt) : cv::Point_<T>(pt) {}


#else

template<typename T> inline
Point<T>::Point()
    : x(0), y(0) {}

template<typename T> inline
Point<T>::Point(T x, T y)
    : x(x), y(y) {}

template<typename T> inline
Point<T>::Point(const Point& pt)
    : x(pt.x), y(pt.y) {}

template<typename T> inline
T Point<T>::dot(const Point& pt) const
{
  if (typeid(T2) == typeid(int))
    return I3D_ROUND_TO_INT(x*pt.x + y*pt.y);
  else
    return static_cast<T>(x*pt.x + y*pt.y);
}

template<typename T> inline
double Point<T>::ddot(const Point& pt) const
{
  return static_cast<double>(x*pt.x + y*pt.y);
}

template<typename T> inline
double Point<T>::cross(const Point& pt) const
{
  return static_cast<double>(x*pt.y - y*pt.x);
}

#endif // HAVE_OPENCV

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
I3D_ENABLE_WARNING(4244)

template<typename T1, typename T2> static inline
Point<T1>& operator += (Point<T1>& a, const Point<T2>& b)
{
  if (typeid(T1) == typeid(int)) {
    a.x += I3D_ROUND_TO_INT(b.x);
    a.y += I3D_ROUND_TO_INT(b.y);
  } else {
    a.x += static_cast<T1>(b.x);
    a.y += static_cast<T1>(b.y);
  }
  return a;
}

template<typename T1, typename T2> static inline
Point<T1>& operator -= (Point<T1>& a, const Point<T2>& b)
{
  if (typeid(T1) == typeid(int)) {
    a.x -= I3D_ROUND_TO_INT(b.x);
    a.y -= I3D_ROUND_TO_INT(b.y);
  } else {
    a.x -= static_cast<T1>(b.x);
    a.y -= static_cast<T1>(b.y);
  }
  return a;
}

template<typename T1, typename T2> static inline
Point<T1>& operator *= (Point<T1>& a, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    a.x = I3D_ROUND_TO_INT(a.x * b);
    a.y = I3D_ROUND_TO_INT(a.y * b);
  } else {
    a.x = static_cast<T1>(a.x * b);
    a.y = static_cast<T1>(a.y * b);
  }
  return a;
}

template<typename T1, typename T2> static inline
Point<T1>& operator /= (Point<T1>& a, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    a.x = I3D_ROUND_TO_INT(a.x / b);
    a.y = I3D_ROUND_TO_INT(a.y / b);
  } else {
    a.x = static_cast<T1>(a.x / b);
    a.y = static_cast<T1>(a.y / b);
  }
  return a;
}

template<typename T> static inline
bool operator == (const Point<T>& a, const Point<T>& b)
{
  return a.x == b.x && a.y == b.y;
}

template<typename T> static inline
bool operator != (const Point<T>& a, const Point<T>& b)
{
  return a.x != b.x || a.y != b.y;
}

template<typename T> static inline
Point<T> operator + (const Point<T>& a, const Point<T>& b)
{
  return Point<T>(a.x + b.x, a.y + b.y);
}

template<typename T> static inline
Point<T> operator - (const Point<T>& a, const Point<T>& b)
{
  return Point<T>(a.x - b.x, a.y - b.y);
}

template<typename T> static inline
Point<T> operator - (const Point<T>& a)
{
  return Point<T>(-a.x, -a.y);
}

template<typename T1, typename T2> static inline
Point<T1> operator * (const Point<T1>& a, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    return Point<T1>(I3D_ROUND_TO_INT(a.x*b), I3D_ROUND_TO_INT(a.y*b));
  } else {
    return Point<T1>(static_cast<T1>(a.x*b), static_cast<T1>(a.y*b));
  }
}

template<typename T1, typename T2> static inline
Point<T2> operator * (T1 a, const Point<T2>& b)
{
  if (typeid(T2) == typeid(int)) {
    return Point<T2>(I3D_ROUND_TO_INT(b.x*a), I3D_ROUND_TO_INT(b.y*a));
  } else {
    return Point<T2>(static_cast<T2>(b.x*a), static_cast<T2>(b.y*a));
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
template<typename T>
class I3D_EXPORT MultiPoint : public EntityPoints<T>
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
  MultiPoint(const std::vector<Point<T>> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint(std::initializer_list<Point<T>> listPoints);

  ~MultiPoint() {}

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  void add(const Point<T> &point) override;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size) { this->mPoints.resize(size); }

};

template<typename T> inline
MultiPoint<T>::MultiPoint() 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(int size) 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, size) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(const MultiPoint &multiPoint) 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, multiPoint) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(const std::vector<Point<T>> &vPoint) 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, vPoint) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(std::initializer_list<Point<T>> listPoints) 
: EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, listPoints) {}


template<typename T> inline
void MultiPoint<T>::add(const Point<T> &point)
{
  this->mPoints.push_back(point);
}

typedef MultiPoint<int> MultiPointI;
typedef MultiPoint<double> MultiPointD;
typedef MultiPoint<float> MultiPointF;




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
class I3D_EXPORT Point3
#ifdef HAVE_OPENCV
  : public cv::Point3_<T>
#endif
{
  // Por si no esta activado OpenCV. Para evitar problemas ya que el
  // uso de point esta muy extendido por la libreria se copia la clase
  // Point_ de OpenCV
#ifndef HAVE_OPENCV

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  T x;

  T y;

  T z;

#endif

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

#ifndef HAVE_OPENCV

  //! dot product
  T dot(const Point3 &pt) const;
    
  //! dot product computed in double-precision arithmetics
  double ddot(const Point3 &pt) const;
    
  //! cross-product
  Point3<T> cross(const Point3 &pt) const;

#else

  /*!
   * \brief Constructor de copia
   * \param[in] point Objeto Point que se copia
   */
  //Point(const cv::Point3_<T> &point);


  //Para evitar su uso y no de problemas si no esta activado OpenCV
  //operator cv::Vec<T, 2>() const = delete;

  //Point(const cv::Size_<T>& sz) = delete;
  //Point(const cv::Vec<T, 2>& v) = delete;
  //bool inside(const cv::Rect_<T>& r) const = delete;

#endif

};

typedef Point3<int> Point3I;
typedef Point3<double> Point3D;
typedef Point3<float> Point3F;

#ifdef HAVE_OPENCV

template<typename T> inline
Point3<T>::Point3() : cv::Point3_<T>() {}

template<typename T> inline
Point3<T>::Point3(T x, T y, T z) : cv::Point3_<T>(x, y, z) {}

template<typename T> inline
Point3<T>::Point3(const Point3<T>& pt) : cv::Point3_<T>(pt.x, pt.y, pt.z) {}

//template<typename T> inline
//Point3<T>::Point3(const cv::Point3_<T>& pt) : cv::Point3_<T>(pt) {}


#else

template<typename T> inline
Point3<T>::Point3()
    : x(0), y(0), z(0) {}

template<typename T> inline
Point3<T>::Point3(T x, T y, T z)
    : x(x), y(y), z(z) {}

template<typename T> inline
Point3<T>::Point3(const Point3 &pt)
    : x(pt.x), y(pt.y), z(pt.z) {}
    

template<typename T> inline
T Point3<T>::dot(const Point3 &pt) const
{
  if (typeid(T2) == typeid(int))
    return I3D_ROUND_TO_INT(x*pt.x + y*pt.y + z*pt.z);
  else
    return static_cast<T>(x*pt.x + y*pt.y + z*pt.z);
}

template<typename T> inline
double Point3<T>::ddot(const Point3& pt) const
{
  return static_cast<double>(x*pt.x + y*pt.y + z*pt.z);
}

template<typename T> inline
Point3<T>  Point3<T>::cross(const Point3& pt) const
{
  return Point3<T>(y*pt.z - z*pt.y, z*pt.x - x*pt.z, x*pt.y - y*pt.x);
}

#endif // HAVE_OPENCV

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
    return Point3<T2>(I3D_ROUND_TO_INT(this->x), I3D_ROUND_TO_INT(this->y), I3D_ROUND_TO_INT(this->z));
  } else {
    return Point3<T2>(static_cast<T2>(this->x), static_cast<T2>(this->y), static_cast<T2>(this->z));
  }
}
I3D_ENABLE_WARNING(4244)

template<typename T1, typename T2> static inline
Point3<T1>& operator += (Point3<T1>& a, const Point3<T2>& b)
{
  if (typeid(T1) == typeid(int)) {
    a.x += I3D_ROUND_TO_INT(b.x);
    a.y += I3D_ROUND_TO_INT(b.y);
    a.z += I3D_ROUND_TO_INT(b.z);
  } else {
    a.x += static_cast<T1>(b.x);
    a.y += static_cast<T1>(b.y);
    a.z += static_cast<T1>(b.z);
  }
  return a;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator -= (Point3<T1>& a, const Point3<T2>& b)
{
  if (typeid(T1) == typeid(int)) {
    a.x -= I3D_ROUND_TO_INT(b.x);
    a.y -= I3D_ROUND_TO_INT(b.y);
    a.z -= I3D_ROUND_TO_INT(b.z);
  } else {
    a.x -= static_cast<T1>(b.x);
    a.y -= static_cast<T1>(b.y);
    a.z -= static_cast<T1>(b.z);
  }
  return a;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator *= (Point3<T1>& a, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    a.x = I3D_ROUND_TO_INT(a.x * b);
    a.y = I3D_ROUND_TO_INT(a.y * b);
    a.z = I3D_ROUND_TO_INT(a.z * b);
  } else {
    a.x = static_cast<T1>(a.x * b);
    a.y = static_cast<T1>(a.y * b);
    a.z = static_cast<T1>(a.z * b);
  }
  return a;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator /= (Point3<T1>& a, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    a.x = I3D_ROUND_TO_INT(a.x / b);
    a.y = I3D_ROUND_TO_INT(a.y / b);
    a.z = I3D_ROUND_TO_INT(a.z / b);
  } else {
    a.x = static_cast<T1>(a.x / b);
    a.y = static_cast<T1>(a.y / b);
    a.z = static_cast<T1>(a.z / b);
  }
  return a;
}

template<typename T> static inline
bool operator == (const Point3<T>& a, const Point3<T>& b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

template<typename T> static inline
bool operator != (const Point3<T>& a, const Point3<T>& b)
{
  return a.x != b.x || a.y != b.y || a.z != b.z;
}

template<typename T> static inline
Point3<T> operator + (const Point3<T>& a, const Point3<T>& b)
{
  return Point3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T> static inline
Point3<T> operator - (const Point3<T>& a, const Point3<T>& b)
{
  return Point3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T> static inline
Point3<T> operator - (const Point3<T>& a)
{
  return Point3<T>(-a.x, -a.y, -a.z);
}

template<typename T1, typename T2> static inline
Point3<T1> operator * (const Point3<T1>& a, T2 b)
{
  if (typeid(T1) == typeid(int)) {
    return Point3<T1>(I3D_ROUND_TO_INT(a.x*b), I3D_ROUND_TO_INT(a.y*b), I3D_ROUND_TO_INT(a.z*b));
  } else {
    return Point3<T1>(static_cast<T1>(a.x*b), static_cast<T1>(a.y*b), static_cast<T1>(a.z*b));
  }
}

template<typename T1, typename T2> static inline
Point3<T2> operator * (T1 a, const Point3<T2>& b)
{
  if (typeid(T2) == typeid(int)) {
    return Point3<T2>(I3D_ROUND_TO_INT(b.x*a), I3D_ROUND_TO_INT(b.y*a), I3D_ROUND_TO_INT(b.z*a));
  } else {
    return Point3<T2>(static_cast<T2>(b.x*a), static_cast<T2>(b.y*a), static_cast<T2>(b.z*a));
  }
}


/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_POINT_H
