#ifndef I3D_ENTITY_H
#define I3D_ENTITY_H

#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

#include "opencv2/core/core.hpp"

#include "core/defs.h"
//#include "geometric_entities/window.h"

namespace I3D
{

// forward declaration
template<typename T> class Window;


/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Tipos de transformaciones
 */
enum class entity_type {
  POINT_2D,                   /*!< Punto 2D */
  POINT_3D,                   /*!< Punto 3D */
  LINESTRING_2D,              /*!< Polilinea 2D */
  LINESTRING_3D,              /*!< Polilinea 3D */
  POLYGON_2D,                 /*!< Poligono 2D */
  POLYGON_3D,                 /*!< Poligono 3D */
  SEGMENT_2D,                 /*!< Segmento 2D */
  SEGMENT_3D,                 /*!< Segmento 3D */
  WINDOW,                     /*!< Ventana */
  BBOX,                       /*!< BBOX */
  MULTIPOINT_POINT_2D,        /*!< Multipunto 2D */
  MULTIPOINT_POINT_3D         /*!< Multipunto 3D */
};

/*!
 * \brief Clase base para entidades geometricas
 */
template<typename T>
class I3D_EXPORT Entity
{
protected:

  /*!
   * \brief Tipo de entidad
   * \see entity_type
   */
  entity_type mEntityType;

public:

  /*!
   * \brief Constructora
   * \param[in] type Tipo de entidad
   */
  Entity(entity_type type) : mEntityType(type) {}

  /*!
   * \brief Destructora
   */
  virtual ~Entity() {}

  /*!
   * \brief Devuelve el tipo de entidad 
   */
  const entity_type getType() const { return mEntityType; }
};

/* ---------------------------------------------------------------------------------- */

template<typename T>
class I3D_EXPORT EntityPoints : public Entity<T>
{
public:

  /*!
   * \brief type
   */
  typedef T type;

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<cv::Point_<T>> mPoints;

public:

  /*!
   * \brief Constructora por defecto
   */
  EntityPoints(entity_type type);

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  EntityPoints(entity_type type, int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  EntityPoints(entity_type type, const EntityPoints &entityPoints);

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  EntityPoints(entity_type type, const std::vector<cv::Point_<T>> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  EntityPoints(entity_type type, std::initializer_list<cv::Point_<T>> listPoints);

  //~EntityPoints();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] EntityPoints EntityPoints que se asigna
   * \return Referencia al EntityPoints
   */
  EntityPoints &operator = (const EntityPoints &entityPoints);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  virtual void add(const cv::Point_<T> &point) = 0;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  const std::vector<cv::Point_<T>> &getPoints() const { return mPoints; }

  /*!
   * \brief Devuelve los puntos que esta dentro de una ventana
   * \param[in] w Ventana
   * \return Puntos que entran dentro de la ventana
   */
  std::vector<cv::Point_<T>> getPointsInWindow(const Window<T> &w) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  int getSize() const { return static_cast<int>(mPoints.size()); }

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<T> getWindow() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size) { mPoints.resize(size); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return cv::Point_<T>
   */
  cv::Point_<T> &operator[](size_t id) { return mPoints[id];  }

  /*!
   * \brief Iterador al inicio
   */
  virtual typename std::vector<cv::Point_<T>>::iterator begin();

  /*!
   * \brief Iterador constante al inicio
   */
  virtual typename std::vector<cv::Point_<T>>::const_iterator begin() const;

  /*!
   * \brief Iterador al final
   */
  virtual typename std::vector<cv::Point_<T>>::iterator end();

  /*!
   * \brief Iterador constante al final
   */
  virtual typename std::vector<cv::Point_<T>>::const_iterator end() const;
};


template<typename T> inline
EntityPoints<T>::EntityPoints(entity_type type) 
  : Entity<T>(type), mPoints(0) {}

template<typename T> inline
EntityPoints<T>::EntityPoints(entity_type type, int size) 
  : Entity<T>(type), mPoints(size) {}

template<typename T> inline
EntityPoints<T>::EntityPoints(entity_type type, const EntityPoints &entityPoints) 
  : Entity<T>(type), mPoints(entityPoints.mPoints) {}

template<typename T> inline
EntityPoints<T>::EntityPoints(entity_type type, const std::vector<cv::Point_<T>> &vPoint) 
  : Entity<T>(type), mPoints(vPoint) {}

template<typename T> inline
EntityPoints<T>::EntityPoints(entity_type type, std::initializer_list<cv::Point_<T>> entityPoints)
  : Entity<T>(type), mPoints(entityPoints) {}

template<typename T> inline
EntityPoints<T> &EntityPoints<T>::operator = (const EntityPoints &entityPoints)
{
  if (this != &entityPoints) {
    this->mPoints = entityPoints.mPoints;
    this->mEntityType = entityPoints.mEntityType;
  }
  return *this;
}

template<typename T> inline
std::vector<cv::Point_<T>> EntityPoints<T>::getPointsInWindow(const Window<T> &w) const
{
  std::vector<cv::Point_<T>> r_points(mPoints.size());
  int j = 0;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.containsPoint(mPoints[i])) {
      r_points[i] = mPoints[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename T> inline
Window<T> EntityPoints<T>::getWindow() const
{
  Window<T> w;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
  }
  return w;
}

template<typename T> inline 
typename std::vector<cv::Point_<T>>::iterator EntityPoints<T>::begin() 
{
  return mPoints.begin();
}

template<typename T> inline 
typename std::vector<cv::Point_<T>>::const_iterator EntityPoints<T>::begin() const 
{
  return mPoints.cbegin();
}

template<typename T> inline 
typename std::vector<cv::Point_<T>>::iterator EntityPoints<T>::end()
{
  return mPoints.end();
}

template<typename T> inline 
typename std::vector<cv::Point_<T>>::const_iterator EntityPoints<T>::end() const 
{
  return mPoints.cend();
}

/* ---------------------------------------------------------------------------------- */

template<typename T>
class I3D_EXPORT Entity3DPoints : public Entity<T>
{
public:

  /*!
   * \brief type
   */
  typedef T type;

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<cv::Point3_<T>> mPoints;

public:

  /*!
   * \brief Constructora por defecto
   */
  Entity3DPoints(entity_type type);

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  Entity3DPoints(entity_type type, int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  Entity3DPoints(entity_type type, const Entity3DPoints &entity3DPoints);

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  Entity3DPoints(entity_type type, const std::vector<cv::Point3_<T>> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Entity3DPoints(entity_type type, std::initializer_list<cv::Point3_<T>> listPoints);

  //~Entity3DPoints();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] EntityPoints EntityPoints que se asigna
   * \return Referencia al EntityPoints
   */
  Entity3DPoints &operator = (const Entity3DPoints &entityPoints);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  virtual void add(const cv::Point3_<T> &point) = 0;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  const std::vector<cv::Point3_<T>> &getPoints() const { return mPoints; }

  /*!
   * \brief Devuelve los puntos que esta dentro de una ventana
   * \param[in] w Ventana
   * \return Puntos que entran dentro de la ventana
   */
  std::vector<cv::Point3_<T>> getPointsInWindow(const Window<T> &w) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  int getSize() const { return static_cast<int>(mPoints.size()); }

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<T> getWindow() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size) { mPoints.resize(size); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return cv::Point_<T>
   */
  cv::Point3_<T> &operator[](size_t id) { return mPoints[id];  }

  /*!
   * \brief Iterador al inicio
   */
  virtual typename std::vector<cv::Point3_<T>>::iterator begin();

  /*!
   * \brief Iterador constante al inicio
   */
  virtual typename std::vector<cv::Point3_<T>>::const_iterator begin() const;

  /*!
   * \brief Iterador al final
   */
  virtual typename std::vector<cv::Point3_<T>>::iterator end();

  /*!
   * \brief Iterador constante al final
   */
  virtual typename std::vector<cv::Point3_<T>>::const_iterator end() const;
};


template<typename T> inline
Entity3DPoints<T>::Entity3DPoints(entity_type type) 
  : Entity<T>(type), mPoints(0) {}

template<typename T> inline
Entity3DPoints<T>::Entity3DPoints(entity_type type, int size) 
  : Entity<T>(type), mPoints(size) {}

template<typename T> inline
Entity3DPoints<T>::Entity3DPoints(entity_type type, const Entity3DPoints &entityPoints) 
  : Entity<T>(type), mPoints(entityPoints.mPoints) {}

template<typename T> inline
Entity3DPoints<T>::Entity3DPoints(entity_type type, const std::vector<cv::Point3_<T>> &vPoint) 
  : Entity<T>(type), mPoints(vPoint) {}

template<typename T> inline
Entity3DPoints<T>::Entity3DPoints(entity_type type, std::initializer_list<cv::Point3_<T>> entityPoints) 
  : Entity<T>(type), mPoints(entityPoints) {}

template<typename T> inline
Entity3DPoints<T> &Entity3DPoints<T>::operator = (const Entity3DPoints &entityPoints)
{
  if (this != &entityPoints) {
    this->mPoints = entityPoints.mPoints;
    this->mEntityType = entityPoints.mEntityType;
  }
  return *this;
}

template<typename T> inline
std::vector<cv::Point3_<T>> Entity3DPoints<T>::getPointsInWindow(const Window<T> &w) const
{
  std::vector<cv::Point3_<T>> r_points(mPoints.size());
  int j = 0;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.containsPoint(mPoints[i])) {
      r_points[i] = mPoints[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename T> inline
Window<T> Entity3DPoints<T>::getWindow() const
{
  Window<T> w;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
  }
  return w;
}

template<typename T> inline 
typename std::vector<cv::Point3_<T>>::iterator Entity3DPoints<T>::begin() 
{
  return mPoints.begin();
}

template<typename T> inline 
typename std::vector<cv::Point3_<T>>::const_iterator Entity3DPoints<T>::begin() const 
{
  return mPoints.cbegin();
}

template<typename T> inline 
typename std::vector<cv::Point3_<T>>::iterator Entity3DPoints<T>::end()
{
  return mPoints.end();
}

template<typename T> inline 
typename std::vector<cv::Point3_<T>>::const_iterator Entity3DPoints<T>::end() const 
{
  return mPoints.cend();
}

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_ENTITY_H
