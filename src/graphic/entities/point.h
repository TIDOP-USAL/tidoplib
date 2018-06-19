#ifndef TL_GRAPHIC_ENTITIES_POINT_H
#define TL_GRAPHIC_ENTITIES_POINT_H

#include "config_tl.h"
#include "core/defs.h"

#include "geometry/entities/point.h"
#include "graphic/entities/entity.h"

namespace TL
{

/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

class TL_EXPORT GPoint : public geometry::Point<double>, public GraphicEntity
{
public:

  GPoint();
  GPoint(double x, double y);
  GPoint(const Point<double> &pt);
  GPoint(const GPoint &pt);
  ~GPoint();

  GPoint &operator = (const GPoint &gPoint);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


class TL_EXPORT GPoint3D : public geometry::Point3<double>, public GraphicEntity
{
public:

  GPoint3D();
  GPoint3D(double x, double y, double z);
  GPoint3D(const Point3<double> &pt);
  GPoint3D(const GPoint3D &pt);
  ~GPoint3D();

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
  ~GMultiPoint();

  GMultiPoint &operator = (const GMultiPoint &gMultiPoint);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};


class TL_EXPORT GMultiPoint3D 
  : public geometry::MultiPoint<geometry::Point3<double>>, 
    public GraphicEntity
{
public:

  GMultiPoint3D();
  GMultiPoint3D(const MultiPoint<geometry::Point3<double>> &multiPoint);
  GMultiPoint3D(const GMultiPoint3D &gMultiPoint);
  ~GMultiPoint3D();

  GMultiPoint3D &operator = (const GMultiPoint3D &gMultiPoint);

  bool isMultiEntity() const override;
  bool isSimpleEntity() const override;
};

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_POINT_H
