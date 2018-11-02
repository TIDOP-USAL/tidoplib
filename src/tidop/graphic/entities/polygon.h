#ifndef TL_GRAPHIC_ENTITIES_POLYGON_H
#define TL_GRAPHIC_ENTITIES_POLYGON_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/polygon.h"
#include "tidop/graphic/entities/entity.h"

namespace TL
{

/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

/*!
 * \brief Clase gráfica polígono
 */
class TL_EXPORT GPolygon 
  : public geometry::Polygon<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GPolygon();

  /*!
   * \brief Constructora a partir de un poligono
   * Representa un poligono sin estilo
   * \param polygon Objeto de la clase Polygon
   * \see Polygon
   */
  GPolygon(const geometry::Polygon<geometry::Point<double>> &polygon);

  /*!
   * \brief Constructor de copia
   * \param gPolygon Objeto que se copia
   */
  GPolygon(const GPolygon &gPolygon);

  /*!
   * \brief Constructor de movimiento
   * \param[in] gPolygon Objeto Polygon que se mueve
   */
  GPolygon(GPolygon &&gPolygon) TL_NOEXCEPT;

  ~GPolygon() override;

  /*!
   * \brief Operador de asignación
   * \param gPolygon Objeto GPolygon que se copia
   * \return Referencia al objeto
   */
  GPolygon &operator = (const GPolygon &gPolygon);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] gPolygon Objeto Polygon que se mueve
   */
  GPolygon &operator = (GPolygon &&gPolygon) TL_NOEXCEPT;

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};

/*!
 * \brief Clase gráfica polígono 3D
 */
class TL_EXPORT GPolygon3D 
  : public geometry::Polygon3D<geometry::Point3<double>>,
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GPolygon3D();

  /*!
   * \brief Constructora a partir de un poligono
   * Representa un poligono sin estilo
   * \param polygon Objeto de la clase Polygon
   * \see Polygon
   */
  GPolygon3D(const geometry::Polygon3D<geometry::Point3<double>> &polygon);

  /*!
   * \brief Constructor de copia
   * \param gPolygon3D Objeto que se copia
   */
  GPolygon3D(const GPolygon3D &gPolygon3D);

  ~GPolygon3D() override;

  /*!
   * \brief Operador de asignación
   * \param gPolygon3D Objeto GPolygon3D que se copia
   * \return Referencia al objeto
   */
  GPolygon3D &operator = (const GPolygon3D &gPolygon3D);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


/*!
 * \brief Clase gráfica multi-polígono
 */
class TL_EXPORT GMultiPolygon 
  : public geometry::MultiPolygon<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GMultiPolygon();

  /*!
   * \brief Constructora a partir de un multi-polígono
   * Representa un multi-polígono sin estilo
   * \param multiPolygon Objeto de la clase MultiPolygon
   * \see MultiPolygon
   */
  GMultiPolygon(const geometry::MultiPolygon<geometry::Point<double>> &multiPolygon);

  /*!
   * \brief Constructor de copia
   * \param gMultiPolygon Objeto que se copia
   */
  GMultiPolygon(const GMultiPolygon &gMultiPolygon);

  ~GMultiPolygon() override;

  /*!
   * \brief Operador de asignación
   * \param gMultiPolygon Objeto GMultiPolygon que se copia
   * \return Referencia al objeto
   */
  GMultiPolygon &operator = (const GMultiPolygon &gMultiPolygon);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


/*!
 * \brief Clase gráfica multi-polígono 3D
 */
class TL_EXPORT GMultiPolygon3D 
  : public geometry::MultiPolygon3D<geometry::Point3<double>>,
    public GraphicEntity
{
public:

  /*!
   * \brief Constructor por defecto
   */
  GMultiPolygon3D();

  /*!
   * \brief Constructora a partir de un multi-polígono 3D
   * Representa un multi-polígono 3D sin estilo
   * \param multiPolygon Objeto de la clase MultiPolygon
   * \see MultiPolygon
   */
  GMultiPolygon3D(const geometry::MultiPolygon3D<geometry::Point3<double>> &multiPolygon);

  /*!
   * \brief Constructor de copia
   * \param gMultiPolygon3D Objeto que se copia
   */
  GMultiPolygon3D(const GMultiPolygon3D &gMultiPolygon3D);

  ~GMultiPolygon3D() override;

  /*!
   * \brief Operador de asignación
   * \param gMultiPolygon3D Objeto GMultiPolygon3D que se copia
   * \return Referencia al objeto
   */
  GMultiPolygon3D &operator = (const GMultiPolygon3D &gMultiPolygon3D);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};



} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_POLYGON_H
