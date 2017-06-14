#ifndef I3D_ENTITY_H
#define I3D_ENTITY_H

#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"

namespace I3D
{


/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/* ---------------------------------------------------------------------------------- */


namespace geometry
{

// forward declaration

template<typename T> class Point;
template<typename T> class Point3;
template<typename T> class Window;
template<typename T> class Box;


/*!
 * \brief Clase base para entidades geometricas
 */
class I3D_EXPORT Entity
{

public:

  /*!
   * \brief tipos de entidades geométricas
   */
  enum class type {
    POINT_2D,                   /*!< Punto 2D */
    POINT_3D,                   /*!< Punto 3D */
    LINESTRING_2D,              /*!< Polilinea 2D */
    LINESTRING_3D,              /*!< Polilinea 3D */
    POLYGON_2D,                 /*!< Poligono 2D */
    POLYGON_3D,                 /*!< Poligono 3D */
    SEGMENT_2D,                 /*!< Segmento 2D */
    SEGMENT_3D,                 /*!< Segmento 3D */
    WINDOW,                     /*!< Ventana */
    BOX,                        /*!< Caja */
    MULTIPOINT_2D,              /*!< Multipunto 2D */
    MULTIPOINT_3D,              /*!< Multipunto 3D */
    MULTILINE_2D,
    MULTILINE_3D,
    MULTIPOLYGON_2D,
    MULTIPOLYGON_3D
  };

protected:

  /*!
   * \brief Tipo de entidad
   * \see type
   */
  type mEntityType;

public:

  /*!
   * \brief Constructora
   * \param[in] type Tipo de entidad
   */
  Entity(type type) : mEntityType(type) {}

  /*!
   * \brief Destructora
   */
  virtual ~Entity() {}

  /*!
   * \brief Devuelve el tipo de entidad 
   */
  type getType() const { return mEntityType; }
};


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Entidades 2D
 */
template<typename Point_t>
class EntityPoints : public Entity
{
public:

  /*!
   * \brief Tipo de punto
   */
  //typedef Point_t value_type;

  typedef typename std::vector<Point_t>::difference_type difference_type;
  typedef typename std::vector<Point_t>::value_type value_type;
  typedef typename std::vector<Point_t>::pointer pointer;
  typedef typename std::vector<Point_t>::const_pointer const_pointer;
  typedef typename std::vector<Point_t>::reference reference;
  typedef typename std::vector<Point_t>::const_reference const_reference;
  //TODO: añadir la sobrecarga de operadores necesarios para el acceso generico como contenedores

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<Point_t> mPoints;


  /*!
   * \brief Tipo básico
   */
  //typedef typename Point_t::value_type basic_type;

public:

  /*!
   * \brief Constructora por defecto
   * \param[in] type Tipo de entidad
   */
  EntityPoints(Entity::type type);

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] type Tipo de entidad
   * \param[in] size Tamaños que se reserva
   */
  EntityPoints(Entity::type type, int size);

  /*!
   * \brief Constructor de copia
   * \param[in] type Tipo de entidad
   * \param[in] entityPoints Objeto EntityPoints que se copia
   */
  EntityPoints(Entity::type type, const EntityPoints &entityPoints);

  /*!
   * \brief Constructor
   * \param[in] type Tipo de entidad
   * \param[in] vPoint vector de puntos
   */
  EntityPoints(Entity::type type, const std::vector<Point_t> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] type Tipo de entidad
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  EntityPoints(Entity::type type, std::initializer_list<Point_t> listPoints);

  //~EntityPoints();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] entityPoints EntityPoints que se asigna
   * \return Referencia al EntityPoints
   */
  EntityPoints &operator = (const EntityPoints &entityPoints);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  virtual void add(const Point_t &point) = 0;

  /*!
   * \brief Iterador al inicio
   */
  virtual typename std::vector<Point_t>::iterator begin();

  /*!
   * \brief Iterador constante al inicio
   */
  virtual typename std::vector<Point_t>::const_iterator begin() const;

  /*!
   * \brief Iterador al final
   */
  virtual typename std::vector<Point_t>::iterator end();

  /*!
   * \brief Iterador constante al final
   */
  virtual typename std::vector<Point_t>::const_iterator end() const;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  const std::vector<Point_t> &getPoints() const;

  /*!
   * \brief Devuelve los puntos que esta dentro de una ventana
   * \param[in] w Ventana
   * \return Puntos que entran dentro de la ventana
   */
  std::vector<Point_t> getPointsInWindow(const Window<Point_t> &w) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  size_t getSize() const;

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<Point_t> getWindow() const;

  /*!
   * \brief Comprueba si la entidad esta vacia
   * \return True si la entidad es nula
   */
  bool isEmpty() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size);

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return Point_t
   */
  Point_t &operator[](size_t id) { return mPoints[id];  }

  const Point_t &operator[](size_t id) const { return mPoints[id];  }
};


template<typename Point_t> inline
EntityPoints<Point_t>::EntityPoints(Entity::type type) 
  : Entity(type), 
    mPoints(0) 
{
}

template<typename Point_t> inline
EntityPoints<Point_t>::EntityPoints(Entity::type type, int size) 
  : Entity(type), 
    mPoints(size) 
{
}

template<typename Point_t> inline
EntityPoints<Point_t>::EntityPoints(Entity::type type, const EntityPoints &entityPoints) 
  : Entity(type), 
    mPoints(entityPoints.mPoints) 
{
}

template<typename Point_t> inline
EntityPoints<Point_t>::EntityPoints(Entity::type type, const std::vector<Point_t> &vPoint) 
  : Entity(type),
    mPoints(vPoint) 
{
}

template<typename Point_t> inline
EntityPoints<Point_t>::EntityPoints(Entity::type type, std::initializer_list<Point_t> entityPoints)
  : Entity(type),
    mPoints(entityPoints)
{
}

template<typename Point_t> inline
EntityPoints<Point_t> &EntityPoints<Point_t>::operator = (const EntityPoints &entityPoints)
{
  if (this != &entityPoints) {
    this->mPoints = entityPoints.mPoints;
    this->mEntityType = entityPoints.mEntityType;
  }
  return *this;
}

template<typename Point_t> inline 
typename std::vector<Point_t>::iterator EntityPoints<Point_t>::begin() 
{
  return mPoints.begin();
}

template<typename Point_t> inline 
typename std::vector<Point_t>::const_iterator EntityPoints<Point_t>::begin() const 
{
  return mPoints.cbegin();
}

template<typename Point_t> inline 
typename std::vector<Point_t>::iterator EntityPoints<Point_t>::end()
{
  return mPoints.end();
}

template<typename Point_t> inline 
typename std::vector<Point_t>::const_iterator EntityPoints<Point_t>::end() const 
{
  return mPoints.cend();
}

template<typename Point_t> inline
const std::vector<Point_t> &EntityPoints<Point_t>::getPoints() const 
{ 
  return mPoints; 
}

template<typename Point_t> inline
std::vector<Point_t> EntityPoints<Point_t>::getPointsInWindow(const Window<Point_t> &w) const
{
  std::vector<Point_t> r_points(mPoints.size());
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

template<typename Point_t> inline
size_t EntityPoints<Point_t>::getSize() const 
{ 
  return mPoints.size(); 
}

template<typename Point_t> inline
Window<Point_t> EntityPoints<Point_t>::getWindow() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
  }
  return w;
}

template<typename Point_t> inline
void EntityPoints<Point_t>::resize(int size) 
{ 
  mPoints.resize(size); 
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Entidades 3D
 */
template<typename Point3_t>
class Entity3DPoints : public Entity
{

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<Point3_t> mPoints;

  /*!
   * \brief type
   */
  typedef Point3_t value_type;

public:

  /*!
   * \brief Constructora por defecto
   * \param[in] type Tipo de entidad
   */
  Entity3DPoints(Entity::type type);

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] type Tipo de entidad
   * \param[in] size Tamaño que se reserva
   * \see entity_type
   */
  Entity3DPoints(Entity::type type, int size);

  /*!
   * \brief Constructor de copia
   * \param[in] type Tipo de entidad
   * \param[in] entity3DPoints Entidad que se copia
   * \see entity_type
   */
  Entity3DPoints(Entity::type type, const Entity3DPoints &entity3DPoints);

  /*!
   * \brief Constructor
   * \param[in] type Tipo de entidad
   * \param[in] vPoint vector de puntos
   * \see entity_type
   */
  Entity3DPoints(Entity::type type, const std::vector<Point3_t> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] type Tipo de entidad
   * \param[in] listPoints Inicializador de lista con los puntos
   * \see entity_type
   */
  Entity3DPoints(Entity::type type, std::initializer_list<Point3_t> listPoints);

  //~Entity3DPoints();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] entityPoints EntityPoints que se asigna
   * \return Referencia al EntityPoints
   */
  Entity3DPoints &operator = (const Entity3DPoints &entityPoints);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  virtual void add(const Point3_t &point) = 0;

  /*!
   * \brief Iterador al inicio
   */
  virtual typename std::vector<Point3_t>::iterator begin();

  /*!
   * \brief Iterador constante al inicio
   */
  virtual typename std::vector<Point3_t>::const_iterator begin() const;

  /*!
   * \brief Iterador al final
   */
  virtual typename std::vector<Point3_t>::iterator end();

  /*!
   * \brief Iterador constante al final
   */
  virtual typename std::vector<Point3_t>::const_iterator end() const;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los puntos
   */
  Box<Point3_t> getBox() const;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  const std::vector<Point3_t> &getPoints() const;
  
  /*!
   * \brief Devuelve los puntos que esta dentro de una caja
   * \param[in] box Caja
   * \return Puntos que entran dentro de la caja
   */
  std::vector<Point3_t> getPointsInBox(const Box<Point3_t> &box) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  size_t getSize() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size);

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return Point3_t
   */
  Point3_t &operator[](size_t id) { return mPoints[id]; }
  const Point3_t &operator[](size_t id) const { return mPoints[id]; }
};


template<typename Point3_t> inline
Entity3DPoints<Point3_t>::Entity3DPoints(Entity::type type) 
  : Entity(type), mPoints(0) {}

template<typename Point3_t> inline
Entity3DPoints<Point3_t>::Entity3DPoints(Entity::type type, int size) 
  : Entity(type), mPoints(size) {}

template<typename Point3_t> inline
Entity3DPoints<Point3_t>::Entity3DPoints(Entity::type type, const Entity3DPoints &entityPoints) 
  : Entity(type), mPoints(entityPoints.mPoints) {}

template<typename Point3_t> inline
Entity3DPoints<Point3_t>::Entity3DPoints(Entity::type type, const std::vector<Point3_t> &vPoint) 
  : Entity(type), mPoints(vPoint) {}

template<typename Point3_t> inline
Entity3DPoints<Point3_t>::Entity3DPoints(Entity::type type, std::initializer_list<Point3_t> entityPoints) 
  : Entity(type), mPoints(entityPoints) {}

template<typename Point3_t> inline
Entity3DPoints<Point3_t> &Entity3DPoints<Point3_t>::operator = (const Entity3DPoints &entityPoints)
{
  if (this != &entityPoints) {
    this->mPoints = entityPoints.mPoints;
    this->mEntityType = entityPoints.mEntityType;
  }
  return *this;
}

template<typename Point3_t> inline 
typename std::vector<Point3_t>::iterator Entity3DPoints<Point3_t>::begin() 
{
  return mPoints.begin();
}

template<typename Point3_t> inline 
typename std::vector<Point3_t>::const_iterator Entity3DPoints<Point3_t>::begin() const 
{
  return mPoints.cbegin();
}

template<typename Point3_t> inline 
typename std::vector<Point3_t>::iterator Entity3DPoints<Point3_t>::end()
{
  return mPoints.end();
}

template<typename Point3_t> inline 
typename std::vector<Point3_t>::const_iterator Entity3DPoints<Point3_t>::end() const 
{
  return mPoints.cend();
}

template<typename Point3_t> inline
Box<Point3_t> Entity3DPoints<Point3_t>::getBox() const
{
  Box<Point3_t> box;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (box.pt1.x > mPoints[i].x) box.pt1.x = mPoints[i].x;
    if (box.pt1.y > mPoints[i].y) box.pt1.y = mPoints[i].y;
    if (box.pt1.z > mPoints[i].z) box.pt1.z = mPoints[i].z;
    if (box.pt2.x < mPoints[i].x) box.pt2.x = mPoints[i].x;
    if (box.pt2.y < mPoints[i].y) box.pt2.y = mPoints[i].y;
    if (box.pt2.z < mPoints[i].z) box.pt2.z = mPoints[i].z;
  }
  return box;
}

template<typename Point3_t> inline
const std::vector<Point3_t> &Entity3DPoints<Point3_t>::getPoints() const 
{ 
  return mPoints; 
}

template<typename Point3_t> inline
std::vector<Point3_t> Entity3DPoints<Point3_t>::getPointsInBox(const Box<Point3_t> &box) const
{
  std::vector<Point3_t> r_points(mPoints.size());
  int j = 0;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (box.containsPoint(mPoints[i])) {
      r_points[i] = mPoints[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename Point3_t> inline
size_t Entity3DPoints<Point3_t>::getSize() const 
{ 
  return mPoints.size(); 
}

template<typename Point3_t> inline
void Entity3DPoints<Point3_t>::resize(int size) 
{ 
  mPoints.resize(size); 
}





} // End namespace geometry

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_ENTITY_H
