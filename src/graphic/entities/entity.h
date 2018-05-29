#ifndef TL_GRAPHIC_ENTITY_H
#define TL_GRAPHIC_ENTITY_H

#include "config_tl.h"

#include <string>
#include <vector>
#include <list>
#include <array>
#include <memory>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "graphic/color.h"
#include "graphic/styles.h"

namespace TL
{

template<typename T> class Transform;


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

//datos que pueden ir asociados a una entidad.
class TL_EXPORT GData
{
public:
  GData()
  {
  }

  GData(const GData &gData)
  {
  }
  ~GData()
  {
  }

  GData &operator =(const GData &gData)
  {
    ///TODO: terminar
    return *this;
  }

private:

};



// clase base para las entidades gráficas. Una entidad punto
// que se dibuje heredará de GraphicEntity y de Point.
// Point, LineString, etc definen la geometría, GraphicEntity
// las propiedades generales y GPoint, GLineString, ... las especificas

/*!
 * \brief Clase base para las entidades gráficas
 *
 */
class TL_EXPORT GraphicEntity : public GraphicStyle, public GData
{

public:

  /*!
   * \brief tipos de entidades gráficas
   */
  enum class Type {
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
  Type mEntityType;

public:

  GraphicEntity(Type type);
  GraphicEntity(const GraphicEntity &graphicEntity);
  ~GraphicEntity();
  
  GraphicEntity &operator = (const GraphicEntity &graphicEntity);

  /*!
   * \brief Devuelve el tipo de entidad 
   */
  Type getType() const { return mEntityType; }

  ///TODO: Tiene que ser virtual...
#ifdef HAVE_OPENCV
  virtual void draw(cv::Mat &canvas) const = 0;
#endif
};


/*!
 * Capa. Puede contener elementos de uno o varios tipos. Pueden ser entidades gráficas o simples
 */
class TL_EXPORT GLayer
{

public:

  /*!
   * \brief Allocator
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::allocator_type allocator_type;

  /*!
   * \brief value_type
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::value_type value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t) 
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer 
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef std::list<std::shared_ptr<GraphicEntity>>::const_iterator const_iterator;

protected:

  /*!
   * \brief Nombre de la capa
   */
  std::string mName;

  std::list<std::shared_ptr<GraphicEntity>> mEntities;

  std::shared_ptr<GraphicEntity> mSelectEntity;

public:

  /*!
   * \brief Constructora por defecto
   */
  GLayer();

  /*!
   * \brief Constructor de copia
   * \param[in] gLayer Objeto que se copia
   */
  GLayer(const GLayer &gLayer);

  /*!
   * \brief Constructor de movimiento
   * \param[in] gLayer Objeto que se copia
   */
  GLayer(GLayer &&gLayer);


  /*!
   * \brief Constructora de lista
   * \param[in] entities listado de entidades
   */
  GLayer(std::initializer_list<std::shared_ptr<GraphicEntity>> entities);

  /*!
   * \brief Destructora
   */
  ~GLayer() {}

  /*!
   * \brief Devuelve un iterador al inicio de la capa
   * \return Iterador al primer elemento
   */
  virtual iterator begin();

  /*!
   * \brief Devuelve un iterador constante al inicio de la capa
   * \return Iterador al primer elemento
   */
  virtual const_iterator begin() const;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después de la última entidad
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después de la última entidad
   */
  virtual iterator end();

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después de la última entidad
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento después de la última entidad
   */
  virtual const_iterator end() const;

  /*!
   * \brief Agrega un elemento al final del contenedor
   */
  TL_DEPRECATED("push_back(const std::shared_ptr<GraphicEntity> &entity)")
  void add(const std::shared_ptr<GraphicEntity> &entity);
  void push_back(const std::shared_ptr<GraphicEntity> &entity);
  void push_back(std::shared_ptr<GraphicEntity> &&entity);

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
  void resize(size_type count, const std::shared_ptr<GraphicEntity> &value);

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  size_type size() const;

  /*!
   * \brief Asignación de copia
   */
  GLayer& operator=(const GLayer& entity);

  /*!
   * \brief Asignación de movimiento
   */
  GLayer& operator=(GLayer&& entity);

  /*!
   * \brief Elimina el intervalo
   */
  //iterator erase(const_iterator first, const_iterator last);


  std::string getName() const;
  void setName(const std::string &name);

};


} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITY_H
