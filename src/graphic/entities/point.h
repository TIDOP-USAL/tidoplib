#ifndef TL_GRAPHIC_ENTITIES_POINT_H
#define TL_GRAPHIC_ENTITIES_POINT_H

#include "config_tl.h"

#include "geometry/entities/point.h"
#include "graphic/entities/entity.h"

namespace TL
{

/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

class TL_EXPORT GPoint
  : public geometry::Point<double>,
    public GraphicEntity
{
public:

  /*!
   * \brief Constructora por defecto
   */
  GPoint();

  /*!
   * \brief Constructora
   * \param[in] x Coordenada x
   * \param[in] y Coordenada y
   */
  GPoint(double x, double y);

  /*!
   * \brief Constructor a partir de un punto
   * \param[in] pt Punto
   * \see Point
   */
  GPoint(const Point<double> &pt);

  /*!
   * \brief Constructor de copia
   * \param[in] pt Objeto GPoint que se copia
   */
  GPoint(const GPoint &pt);

  /*!
   * \brief Constructor de movimiento
   * \param[in] pt Objeto GPoint que se mueve
   */
  GPoint(GPoint &&pt) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  ~GPoint() override;

  /*!
   * \brief Operador de asignación
   * \param gPoint Objeto GPoint que se asigna
   * \return Referencia al objeto
   */
  GPoint &operator = (const GPoint &gPoint);

  /*!
   * \brief Operador de asignación de movimiento
   * \param gPoint Objeto GPoint que se mueve
   * \return Referencia al objeto
   */
  GPoint &operator = (GPoint &&gPoint) TL_NOEXCEPT;

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


class TL_EXPORT GPoint3D 
  : public geometry::Point3<double>, 
    public GraphicEntity
{
public:

  GPoint3D();
  GPoint3D(double x, double y, double z);
  GPoint3D(const Point3<double> &pt);
  GPoint3D(const GPoint3D &pt);
  ~GPoint3D() override;

  GPoint3D &operator = (const GPoint3D &gPoint);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


class TL_EXPORT GMultiPoint 
  : public geometry::MultiPoint<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  GMultiPoint();
  GMultiPoint(const MultiPoint<geometry::Point<double>> &multiPoint);
  GMultiPoint(const GMultiPoint &gMultiPoint);
  ~GMultiPoint() override;

  GMultiPoint &operator = (const GMultiPoint &gMultiPoint);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


class TL_EXPORT GMultiPoint3D 
  : public geometry::MultiPoint3D<geometry::Point3<double>>, 
    public GraphicEntity
{
public:

  GMultiPoint3D();
  GMultiPoint3D(const MultiPoint3D<geometry::Point3<double>> &multiPoint);
  GMultiPoint3D(const GMultiPoint3D &gMultiPoint);
  ~GMultiPoint3D() override;

  GMultiPoint3D &operator = (const GMultiPoint3D &gMultiPoint);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_POINT_H
