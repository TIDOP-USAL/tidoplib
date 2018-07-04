#ifndef TL_GRAPHIC_ENTITIES_LINESTRING_H
#define TL_GRAPHIC_ENTITIES_LINESTRING_H

#include "config_tl.h"
#include "core/defs.h"

#include "geometry/entities/point.h"
#include "geometry/entities/lineString.h"
#include "graphic/entities/entity.h"

namespace TL
{

/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

/*!
 * \brief Clase gráfica polilínea
 */
class TL_EXPORT GLineString
   : public geometry::LineString<geometry::Point<double>>,
     public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GLineString();

  /*!
   * \brief Constructora a partir de una polilínea
   * Representa una polilinea sin estilo
   * \param lineString Objeto de la clase LineString
   * \see LineString
   */
  GLineString(const LineString<geometry::Point<double>> &lineString);

  /*!
   * \brief Constructor de copia
   * \param lineString Objeto que se copia
   */
  GLineString(const GLineString &lineString);

  ~GLineString();

  /*!
   * \brief Operador de asignación
   * \param gLineString Objeto GLineString que se copia
   * \return Referencia al objeto
   */
  GLineString &operator = (const GLineString &gLineString);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;

};


/*!
 * \brief Clase gráfica polilinea 3D
 */
class TL_EXPORT GLineString3D
  : public geometry::LineString3D<geometry::Point3<double>>,
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GLineString3D();

  /*!
   * \brief Constructora a partir de una polilinea 3D
   * Representa una polilinea 3D sin estilo
   * \param lineString Objeto de la clase LineString3D
   * \see LineString3D
   */
  GLineString3D(const geometry::LineString3D<geometry::Point3<double>> &lineString);

  /*!
   * \brief Constructor de copia
   * \param lineString Objeto que se copia
   */
  GLineString3D(const GLineString3D &lineString);

  ~GLineString3D();

  /*!
   * \brief Operador de asignación
   * \param gLineString Objeto GLineString3D que se copia
   * \return Referencia al objeto
   */
  GLineString3D &operator = (const GLineString3D &gLineString);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


/*!
 * \brief Clase gráfica multi-polilinea
 */
class TL_EXPORT GMultiLineString 
  : public geometry::MultiLineString<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GMultiLineString();

  /*!
   * \brief Constructora a partir de una multi-polilinea
   * Representa una multi-polilinea sin estilo
   * \param multiLineString Objeto de la clase MultiLineString
   * \see MultiLineString
   */
  GMultiLineString(const geometry::MultiLineString<geometry::Point<double>> &multiLineString);

  /*!
   * \brief Constructor de copia
   * \param gMultiLineString Objeto que se copia
   */
  GMultiLineString(const GMultiLineString &gMultiLineString);

  ~GMultiLineString();

  /*!
   * \brief Operador de asignación
   * \param gMultiLineString Objeto GMultiLineString que se copia
   * \return Referencia al objeto
   */
  GMultiLineString &operator = (const GMultiLineString &gMultiLineString);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};

/*!
 * \brief Clase gráfica multi-polilinea 3D
 */
class TL_EXPORT GMultiLineString3D 
  : public geometry::MultiLineString3D<geometry::Point3<double>>,
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GMultiLineString3D();

  /*!
   * \brief Constructora a partir de una multi-polilinea 3D
   * Representa una multi-polilinea 3D sin estilo
   * \param multiLineString Objeto de la clase MultiLineString3D
   * \see MultiLineString3D
   */
  GMultiLineString3D(const geometry::MultiLineString3D<geometry::Point3<double>> &multiLineString);

  /*!
   * \brief Constructor de copia
   * \param gMultiLineString Objeto que se copia
   */
  GMultiLineString3D(const GMultiLineString3D &gMultiLineString);

  ~GMultiLineString3D();

  /*!
   * \brief Operador de asignación
   * \param gMultiLineString Objeto GMultiLineString3D que se copia
   * \return Referencia al objeto
   */
  GMultiLineString3D &operator = (const GMultiLineString3D &gMultiLineString);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;

};

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_LINESTRING_H
