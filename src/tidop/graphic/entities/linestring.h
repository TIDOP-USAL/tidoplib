#ifndef TL_GRAPHIC_ENTITIES_LINESTRING_H
#define TL_GRAPHIC_ENTITIES_LINESTRING_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/linestring.h"
#include "tidop/graphic/entities/entity.h"

namespace tl
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
  : public LineString<Point<double>>,
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
   * \param[in] lineString Objeto de la clase LineString
   * \see LineString
   */
  GLineString(const LineString<Point<double>> &lineString);

  /*!
   * \brief Constructor de movimiento
   * \param[in] lineString Objeto que se nueve
   */
  GLineString(GLineString &&lineString) TL_NOEXCEPT;

  /*!
   * \brief Constructor de copia
   * \param[in] lineString Objeto que se copia
   */
  GLineString(const GLineString &lineString);

  ~GLineString() override;

  /*!
   * \brief Operador de asignación
   * \param[in] gLineString Objeto GLineString que se copia
   * \return Referencia al objeto
   */
  GLineString &operator = (const GLineString &gLineString);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] gLineString Objeto GLineString que se mueve
   * \return Referencia al objeto
   */
  GLineString &operator = (GLineString &&gLineString) TL_NOEXCEPT;

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;

};


/*!
 * \brief Clase gráfica polilinea 3D
 */
class TL_EXPORT GLineString3D
  : public LineString3D<Point3<double>>,
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
   * \param[in] gLineString3D Objeto de la clase LineString3D
   * \see LineString3D
   */
  GLineString3D(const LineString3D<Point3<double>> &gLineString3D);

  /*!
   * \brief Constructor de copia
   * \param[in] gLineString3D Objeto que se copia
   */
  GLineString3D(const GLineString3D &gLineString3D);
    
  /*!
   * \brief Constructor de movimiento
   * \param[in] gLineString3D Objeto que se mueve
   */
  GLineString3D(GLineString3D &&gLineString3D) TL_NOEXCEPT;

  ~GLineString3D() override;

  /*!
   * \brief Operador de asignación
   * \param[in] gLineString Objeto GLineString3D que se copia
   * \return Referencia al objeto
   */
  GLineString3D &operator = (const GLineString3D &gLineString);
    
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] gLineString Objeto GLineString3D que se mueve
   * \return Referencia al objeto
   */
  GLineString3D &operator = (GLineString3D &&gLineString) TL_NOEXCEPT;

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


/*!
 * \brief Clase gráfica multi-polilinea
 */
class TL_EXPORT GMultiLineString 
  : public MultiLineString<Point<double>>, 
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
   * \param[in] multiLineString Objeto de la clase MultiLineString
   * \see MultiLineString
   */
  GMultiLineString(const MultiLineString<Point<double>> &multiLineString);

  /*!
   * \brief Constructor de copia
   * \param[in] gMultiLineString Objeto que se copia
   */
  GMultiLineString(const GMultiLineString &gMultiLineString);
    
  /*!
   * \brief Constructor de movimiento
   * \param[in] gMultiLineString Objeto que se mueve
   */
  GMultiLineString(GMultiLineString &&gMultiLineString) TL_NOEXCEPT;

  ~GMultiLineString() override;

  /*!
   * \brief Operador de asignación
   * \param[in] gMultiLineString Objeto GMultiLineString que se copia
   * \return Referencia al objeto
   */
  GMultiLineString &operator = (const GMultiLineString &gMultiLineString);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] gMultiLineString Objeto GMultiLineString que se mueve
   * \return Referencia al objeto
   */
  GMultiLineString &operator = (GMultiLineString &&gMultiLineString) TL_NOEXCEPT;

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};

/*!
 * \brief Clase gráfica multi-polilinea 3D
 */
class TL_EXPORT GMultiLineString3D 
  : public MultiLineString3D<Point3<double>>,
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
   * \param[in] gMultiLineString3D Objeto de la clase MultiLineString3D
   * \see MultiLineString3D
   */
  GMultiLineString3D(const MultiLineString3D<Point3<double>> &gMultiLineString3D);

  /*!
   * \brief Constructor de copia
   * \param[in] gMultiLineString3D Objeto que se copia
   */
  GMultiLineString3D(const GMultiLineString3D &gMultiLineString3D);
   
  /*!
   * \brief Constructor de movimiento
   * \param[in] gMultiLineString3D Objeto que se mueve
   */   
  GMultiLineString3D(GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT;

  ~GMultiLineString3D() override;

  /*!
   * \brief Operador de asignación
   * \param[in] gMultiLineString3D Objeto GMultiLineString3D que se copia
   * \return Referencia al objeto
   */
  GMultiLineString3D &operator = (const GMultiLineString3D &gMultiLineString3D);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] gMultiLineString3D Objeto GMultiLineString3D que se asigna
   * \return Referencia al objeto
   */
  GMultiLineString3D &operator = (GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT;

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;

};

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_LINESTRING_H
