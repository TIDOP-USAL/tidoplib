#ifndef TL_GEOM_ENTITY_H
#define TL_GEOM_ENTITY_H

#include "config_tl.h"

#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"

namespace TL
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
class TL_EXPORT Entity
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
    MULTILINE_2D,               /*!< Multi-línea 2D */
    MULTILINE_3D,               /*!< Multi-línea 3D */
    MULTIPOLYGON_2D,            /*!< Multi-polígono 2D */
    MULTIPOLYGON_3D,            /*!< Multi-polígono 3D */
    CIRCLE,                     /*!< Circulo */
    ELLIPSE                     /*!< Elipse */
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
   * \brief Constructor de copia
   * \param[in] entity Objeto que se copia
   */
  Entity(const Entity &entity) : mEntityType(entity.mEntityType) {}

  /*!
   * \brief Destructora
   */
  virtual ~Entity() {}

  /*!
   * \brief Devuelve el tipo de entidad 
   */
  type getType() const { return mEntityType; }

  /*!
   * \brief Operador de asignación
   */
  Entity &operator = (const Entity &entity)
  {
    if (this != &entity) {
      mEntityType = entity.mEntityType;
    }
    return *this;
  }

};


/* ---------------------------------------------------------------------------------- */

//TODO: Añadir constructor a partir de iteradores first end
//TODO: Añadir constructor de movimiento y asignación con movimiento

/*!
 * \brief Contenedor para entidades geométricas
 * 
 * Es un simple wrapper de std::vector para que las clases de geometrías 
 * se puedan utilizar como contenedores.
 */
template<typename Entity_t>
class EntityContainer
{

public:

  /*!
   * \brief Allocator
   */
  typedef typename std::vector<Entity_t>::allocator_type allocator_type;

  /*!
   * \brief value_type
   */
  typedef typename std::vector<Entity_t>::value_type value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t) 
   */
  typedef typename std::vector<Entity_t>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef typename std::vector<Entity_t>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef typename std::vector<Entity_t>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer 
   */
  typedef typename std::vector<Entity_t>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef typename std::vector<Entity_t>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef typename std::vector<Entity_t>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::vector<Entity_t>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::vector<Entity_t>::const_iterator const_iterator;

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<Entity_t> mEntities;

public:
  
  /*!
   * \brief Constructora por defecto
   */
  EntityContainer();

  /*!
   * \brief Constructora
   * \param[in] Tamaño que se reserva para el contenedor
   */
  EntityContainer(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity Objeto que se copia
   */
  EntityContainer(const EntityContainer &entity);

  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Objeto que se copia
   */
  EntityContainer(EntityContainer &&entity);

  /*!
   * \brief Constructor
   * \param[in] entities
   */
  EntityContainer(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructora de lista
   * \param[in] entities listado de entidades
   */
  EntityContainer(std::initializer_list<Entity_t> entities);

  /*!
   * \brief Destructora
   */
  ~EntityContainer() {}


  /*!
   * \brief Devuelve un iterador al inicio del contenedor
   * \return Iterador al primer elemento
   */
  virtual iterator begin();

  /*!
   * \brief Devuelve un iterador constante al inicio del contenedor
   * \return Iterador al primer elemento
   */
  virtual const_iterator begin() const;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  virtual iterator end();

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  virtual const_iterator end() const;

  /*!
   * \brief Agrega un elemento al final del contenedor
   */
  TL_DEPRECATED("push_back(const std::shared_ptr<GraphicEntity> &entity)")
  void add(const Entity_t &entity);
  void push_back(const Entity_t &entity);
  //TODO Añadir add con movimiento
  void push_back(Entity_t &&entity);

  /*!
   * \brief Devuelve una referencia constante al elemento de la posición indicada
   * return Referencia constante al elemento
   */
  const_reference at(size_type position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * return Referencia al elemento
   */
  reference at(size_type position);

  /*!
   * \brief Elimina los elementos del recipiente
   */
  void clear();
    
  /*!
   * \brief Comprueba si el contenedor esta vacio
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const;

  /*!
   * \brief Establece el tamaño del contenedor
   * \param[in] size
   */
  void reserve(size_type size);

  /*!
   * \brief Modifica el tamaño del contenedor
   * Si el tamaño actual es menor que count, se añaden elementos adicionales. Si el tamaño actual 
   * es mayor que count el contenedor se trunca al número de elementos indicados.
   * \param[in] count Nuevo tamaño del contenedor
   */
  void resize(size_type count);

  /*!
   * \brief Modifica el tamaño del contenedor
   * Si el tamaño actual es menor que count, se añaden elementos adicionales y se inicializan con value.
   * Si el tamaño actual es mayor que count el contenedor se trunca al número de elementos indicados.
   * \param[in] count Nuevo tamaño del contenedor
   * \param[in] value Valor que se asigna a los nuevos elementos
   */
  void resize(size_type count, const Entity_t &value);

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  size_type size() const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia constante al elemento
   */
  const_reference operator[](size_type position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia al elemento
   */
  reference operator[](size_type position);

  /*!
   * \brief Asignación de copia
   */
  EntityContainer<Entity_t>& operator=(const EntityContainer<Entity_t> &entity);

  /*!
   * \brief Asignación de movimiento
   */
  EntityContainer<Entity_t>& operator=(EntityContainer<Entity_t> &&entity);

  /*!
   * \brief Elimina el intervalo
   */
  iterator erase(const_iterator first, const_iterator last);

  //TODO: insert (para insertar nuevos valores en una posición)
};

// Implementación EntityContainer

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer() 
  : mEntities(0)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(size_type size)
  : mEntities(size)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(const EntityContainer &entity)
  : mEntities(entity.mEntities)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(EntityContainer &&entity)
  : mEntities(std::forward<Entity_t>(entity.mEntities))
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
void EntityContainer<Entity_t>::add(const Entity_t &entity)
{
  mEntities.push_back(entity);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::push_back(const Entity_t &entity)
{
  mEntities.push_back(entity);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::push_back(Entity_t &&entity)
{
  mEntities.push_back(std::forward<Entity_t>(entity));
}

template<typename Entity_t> inline
typename std::vector<Entity_t>::const_reference EntityContainer<Entity_t>::at(size_type position) const
{
  return mEntities.at(position);
}

template<typename Entity_t> inline
typename std::vector<Entity_t>::reference EntityContainer<Entity_t>::at(size_type position)
{
  return mEntities.at(position);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::clear() 
{ 
  mEntities.clear();
}

template<typename Entity_t> inline
bool EntityContainer<Entity_t>::empty() const
{
  return mEntities.empty();
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::reserve(size_type size)
{
  mEntities.reserve(size);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::resize(size_type count)
{
  mEntities.resize(count);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::resize(size_type count, const Entity_t &value)
{
  mEntities.resize(count, value);
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::size_type EntityContainer<Entity_t>::size() const
{ 
  return mEntities.size();
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

template<typename Entity_t> inline
EntityContainer<Entity_t>& EntityContainer<Entity_t>::operator=(const EntityContainer<Entity_t>& entity) 
{
  if (this != &entity) {
    this->mEntities = entity.mEntities;
  }
  return (*this);
}

template<typename Entity_t> inline
EntityContainer<Entity_t>& EntityContainer<Entity_t>::operator=(EntityContainer<Entity_t>&& entity) 
{
  if (this != &entity) {
    this->mEntities.clear();
    this->mEntities = std::move(entity.mEntities);
    entity = 0;
  }
  return (*this);
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::iterator EntityContainer<Entity_t>::erase(const_iterator first, const_iterator last)
{
  return mEntities.erase(first, last);
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
  Entities2D(size_type size);

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
Entities2D<Entity_t>::Entities2D(size_type size) 
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
  size_t j = 0;
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
  Entities3D(size_type size);

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
Entities3D<Entity_t>::Entities3D(size_type size) 
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
  size_t j = 0;
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

} // End namespace TL

#endif // TL_GEOM_ENTITY_H
