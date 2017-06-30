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
 * \brief Contenedor para entidades geométricas
 * 
 * Es un simple wrapper de std::vector para que las clases de geometrias 
 * se puedan utilizar como contenedores.
 */
template<typename Entity_t>
class EntityContainer
{

public:

  typedef typename std::vector<Entity_t>::allocator_type allocator_type;

  typedef typename std::vector<Entity_t>::value_type value_type;
  typedef typename std::vector<Entity_t>::size_type size_type;
  typedef typename std::vector<Entity_t>::difference_type difference_type;
  typedef typename std::vector<Entity_t>::pointer pointer;
  typedef typename std::vector<Entity_t>::const_pointer const_pointer;
  typedef typename std::vector<Entity_t>::reference reference;
  typedef typename std::vector<Entity_t>::const_reference const_reference;

  // Iteradores
  typedef typename std::vector<Entity_t>::iterator iterator;
  typedef typename std::vector<Entity_t>::const_iterator const_iterator;

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<Entity_t> mEntities;

public:
  
  EntityContainer();
  EntityContainer(int size);
  EntityContainer(const EntityContainer &entity);
  EntityContainer(const std::vector<Entity_t> &entities);
  EntityContainer(std::initializer_list<Entity_t> entities);

  ~EntityContainer() {}


  /*!
   * \brief Iterador al inicio
   */
  virtual iterator begin();

  /*!
   * \brief Iterador constante al inicio
   */
  virtual const_iterator begin() const;

  /*!
   * \brief Iterador al final
   */
  virtual iterator end();

  /*!
   * \brief Iterador constante al final
   */
  virtual const_iterator end() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size);

  /*!
   * \brief clear
   */
  void clear();
    
  //TODO: ordenar y documentar
  void reserve(size_type count);
  void resize(size_type newSize);
  void resize(size_type newSize, const Entity_t &entity);
  size_type size() const;
  bool empty() const;
  const_reference at(size_type position) const;
  reference at(size_type position);
  const_reference operator[](size_type position) const;
  reference operator[](size_type position);
  void add(const Entity_t &entity);

};

// Implementación EntityContainer

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer() 
  : mEntities(0)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(int size)
  : mEntities(size)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(const EntityContainer &entity)
  : mEntities(entity.mEntities)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(const std::vector<Entity_t> &entities)
  : mEntities(entities)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(std::initializer_list<Entity_t> entities)
  : mEntities(entities)
{
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::add(const Entity_t &entity)
{
  mEntities.push_back(entity);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::clear() 
{ 
  mEntities.clear();
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::size_type EntityContainer<Entity_t>::size() const
{ 
  return mEntities.size();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::iterator EntityContainer<Entity_t>::begin() 
{
  return mEntities.begin();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::const_iterator EntityContainer<Entity_t>::begin() const 
{
  return mEntities.cbegin();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::iterator EntityContainer<Entity_t>::end()
{
  return mEntities.end();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::const_iterator EntityContainer<Entity_t>::end() const 
{
  return mEntities.cend();
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::resize(int size) 
{ 
  mEntities.resize(size); 
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::const_reference EntityContainer<Entity_t>::operator[](size_type position) const
{
  return mEntities[position];
}
  
template<typename Entity_t> inline
typename EntityContainer<Entity_t>::reference EntityContainer<Entity_t>::operator[](size_type position)
{
  return mEntities[position];
}


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Entidades 2D
 */
template<typename Entity_t>
class Entities2D : public EntityContainer<Entity_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Entities2D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaños que se reserva
   */
  Entities2D(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] entityPoints Objeto Entities2D que se copia
   */
  Entities2D(const Entities2D &entity);

  /*!
   * \brief Constructor
   * \param[in] entities vector de entidades
   */
  Entities2D(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] entities Inicializador de lista con las entidades
   */
  Entities2D(std::initializer_list<Entity_t> entities);

  //~Entities2D();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] entityPoints EntityPoints que se asigna
   * \return Referencia al EntityPoints
   */
  //Entities2D &operator = (const Entities2D &entity);

  /*!
   * \brief Devuelve las entidades que están dentro de una ventana
   * \param[in] w Ventana
   * \return Entidades seleccionadas
   */
  template<typename Window_t>
  std::vector<Entity_t> getEntitiesInWindow(const Window_t &w) const;


  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<Entity_t> getWindow() const;
};


template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D() 
  : EntityContainer<Entity_t>(0)
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(int size) 
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(const Entities2D &entities) 
  : EntityContainer<Entity_t>(entities) 
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

//template<typename Entity_t> inline
//Entities2D<Entity_t> &Entities2D<Entity_t>::operator = (const Entities2D &entities)
//{
//  if (this != &entities) {
//    this->mEntities = entities.mEntities;
//  }
//  return *this;
//}

template<typename Entity_t> template<typename Window_t> inline
std::vector<Entity_t> Entities2D<Entity_t>::getEntitiesInWindow(const Window_t &w) const
{
  std::vector<Entity_t> r_points(mEntities.size());
  int j = 0;
  for (size_t i = 0; i < mEntities.size(); i++) {
    if (w.containsPoint(mEntities[i])) {
      r_points[i] = mEntities[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename Entity_t> inline
Window<Entity_t> Entities2D<Entity_t>::getWindow() const
{
  Window<Entity_t> w;
  for (size_t i = 0; i < mEntities.size(); i++) {
    if (w.pt1.x > mEntities[i].x) w.pt1.x = mEntities[i].x;
    if (w.pt1.y > mEntities[i].y) w.pt1.y = mEntities[i].y;
    if (w.pt2.x < mEntities[i].x) w.pt2.x = mEntities[i].x;
    if (w.pt2.y < mEntities[i].y) w.pt2.y = mEntities[i].y;
  }
  return w;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Entidades 3D
 */
template<typename Entity_t>
class Entities3D : public EntityContainer<Entity_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Entities3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva
   * \see entity_type
   */
  Entities3D(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity3DPoints Entidad que se copia
   * \see entity_type
   */
  Entities3D(const Entities3D &entity);

  Entities3D(const std::vector<Entity_t> &entities);

  Entities3D(std::initializer_list<Entity_t> entities);

  //~Entity3DPoints();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] entityPoints EntityPoints que se asigna
   * \return Referencia al EntityPoints
   */
  //Entities3D &operator = (const Entities3D &entities);


  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los puntos
   */
  Box<Entity_t> getBox() const;
  
  /*!
   * \brief Devuelve los puntos que esta dentro de una caja
   * \param[in] box Caja
   * \return Puntos que entran dentro de la caja
   */
  std::vector<Entity_t> getEntitiesInBox(const Box<Entity_t> &box) const;

};


template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D() 
  : EntityContainer<Entity_t>() 
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(int size) 
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(const Entities3D &entities) 
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

//template<typename Entity_t> inline
//Entities3D<Entity_t> &Entities3D<Entity_t>::operator = (const Entities3D &entities)
//{
//  if (this != &entities) {
//    this->mEntities = entities.mEntities;
//  }
//  return *this;
//}

template<typename Entity_t> inline
Box<Entity_t> Entities3D<Entity_t>::getBox() const
{
  Box<Entity_t> box;
  for (size_t i = 0; i < mEntities.size(); i++) {
    if (box.pt1.x > mEntities[i].x) box.pt1.x = mEntities[i].x;
    if (box.pt1.y > mEntities[i].y) box.pt1.y = mEntities[i].y;
    if (box.pt1.z > mEntities[i].z) box.pt1.z = mEntities[i].z;
    if (box.pt2.x < mEntities[i].x) box.pt2.x = mEntities[i].x;
    if (box.pt2.y < mEntities[i].y) box.pt2.y = mEntities[i].y;
    if (box.pt2.z < mEntities[i].z) box.pt2.z = mEntities[i].z;
  }
  return box;
}

template<typename Entity_t> inline
std::vector<Entity_t> Entities3D<Entity_t>::getEntitiesInBox(const Box<Entity_t> &box) const
{
  std::vector<Entity_t> r_points(mEntities.size());
  int j = 0;
  for (size_t i = 0; i < mEntities.size(); i++) {
    if (box.containsPoint(mEntities[i])) {
      r_points[i] = mEntities[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}



/* ---------------------------------------------------------------------------------- */






} // End namespace geometry

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_ENTITY_H
