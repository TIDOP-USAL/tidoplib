#ifndef TL_GRAPHIC_ENTITY_H
#define TL_GRAPHIC_ENTITY_H

#include "config_tl.h"

#include <string>
#include <vector>
#include <list>
#include <array>
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/graphic/styles.h"

namespace tl
{

template<typename T> class Transform;


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{


/*!
 * \brief Datos que pueden ir asociados a una entidad.
 */
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



/*!
 * \brief Clase base para las entidades gráficas
 * \see GraphicStyle, GData
 */
class TL_EXPORT GraphicEntity 
  : public GraphicStyle, 
    public GData
{

public:

  /*!
   * \brief Tipos de entidades gráficas
   */
  enum class Type {
    point_2d,                   /*!< Punto 2D */
    point_3d,                   /*!< Punto 3D */
    linestring_2d,              /*!< Polilinea 2D */
    linestring_3d,              /*!< Polilinea 3D */
    polygon_2d,                 /*!< Poligono 2D */
    polygon_3d,                 /*!< Poligono 3D */
    segment_2d,                 /*!< Segmento 2D */
    segment_3d,                 /*!< Segmento 3D */
    window,                     /*!< Ventana */
    box,                        /*!< Caja */
    multipoint_2d,              /*!< Multipunto 2D */
    multipoint_3d,              /*!< Multipunto 3D */
    multiline_2d,               /*!< Multi-línea 2D */
    multiline_3d,               /*!< Multi-línea 3D */
    multipolygon_2d,            /*!< Multi-polígono 2D */
    multipolygon_3d,            /*!< Multi-polígono 3D */
    circle,                     /*!< Circulo */
    ellipse                     /*!< Elipse */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
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
#endif
  };

protected:

  /*!
   * \brief Tipo de entidad
   * \see Type
   */
  Type mEntityType;

public:

  /*!
   * \brief GraphicEntity
   * \param type
   */
  GraphicEntity(Type type);

  /*!
   * \brief Constructor de copia
   * \param graphicEntity Entidad gráfica que se copia
   */
  GraphicEntity(const GraphicEntity &graphicEntity);

  /*!
   * \brief Constructor de movimiento
   * \param graphicEntity Entidad gráfica que se mueve
   */
  GraphicEntity(GraphicEntity &&graphicEntity) TL_NOEXCEPT;

  virtual ~GraphicEntity();
  
  /*!
   * \brief Operador de asignación
   * \param graphicEntity Entidad gráfica que se asigna
   */
  GraphicEntity &operator = (const GraphicEntity &graphicEntity);

  /*!
   * \brief Operador de asignación de movimiento
   * \param graphicEntity Entidad gráfica que se mueve
   */
  GraphicEntity &operator = (GraphicEntity &&graphicEntity) TL_NOEXCEPT;

  /*!
   * \brief Devuelve el tipo de entidad 
   */
  Type type() const;

  /*!
   * \brief Multi-entidad
   * \return
   */
  virtual bool isMultiEntity() const = 0;

  /*!
   * \brief Entidad simple
   * \return
   */
  virtual bool isSimpleEntity() const = 0;

};


} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace tl

#endif // TL_GRAPHIC_ENTITY_H
