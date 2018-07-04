#ifndef TL_GRAPHIC_LAYER_H
#define TL_GRAPHIC_LAYER_H

#include "config_tl.h"
#include "core/defs.h"

#include <list>
#include <memory>


namespace TL
{


/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

class GraphicEntity;

/*!
 * Capa. Puede contener elementos de uno o varios tipos. Pueden 
 * ser entidades gráficas o simples
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

  /*!
   * \brief Listado de entidades
   */
  std::list<std::shared_ptr<GraphicEntity>> mEntities;

  /*!
   * \brief Entidades seleccionadas
   */
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
   * \deprecated Use push_back en su lugar
   */
  TL_DEPRECATED("push_back(const std::shared_ptr<GraphicEntity> &entity)")
  void add(const std::shared_ptr<GraphicEntity> &entity);

  /*!
   * \brief Agrega una entidad mediante copia al final del contenedor
   * \param[in] entity Entidad que se añade
   */
  void push_back(const std::shared_ptr<GraphicEntity> &entity);

  /*!
   * \brief Agrega una entidad mediante movimiento al final del contenedor
   * \param[in] entity Entidad que se añade
   */
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
  iterator erase(const_iterator first, const_iterator last);

  /*!
   * \brief Devuelve el nombre de la capa
   * \return Nombre de la capa
   */
  std::string getName() const;

  /*!
   * \brief Establece el nombre de la capa
   * \param[in] name Nombre de la capa
   */
  void setName(const std::string &name);

};


} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_LAYER_H
