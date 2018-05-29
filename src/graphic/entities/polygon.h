#ifndef TL_GRAPHIC_ENTITIES_POLYGON_H
#define TL_GRAPHIC_ENTITIES_POLYGON_H

#include "config_tl.h"
#include "core/defs.h"

#include "geometry/entities/point.h"
#include "geometry/entities/polygon.h"
#include "graphic/entities/entity.h"

namespace TL
{

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

class TL_EXPORT GPolygon 
  : public geometry::Polygon<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  GPolygon();
  GPolygon(const Polygon<geometry::Point<double>> &polygon);
  GPolygon(const GPolygon &gPolygon);
  ~GPolygon();

  GPolygon &operator = (const GPolygon &gPolygon);
#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


class TL_EXPORT GPolygon3D 
  : public geometry::Polygon<geometry::Point3<double>>, 
    public GraphicEntity
{
public:

  GPolygon3D();
  GPolygon3D(const Polygon<geometry::Point3<double>> &polygon);
  GPolygon3D(const GPolygon3D &gPolygon3D);
  ~GPolygon3D();

  GPolygon3D &operator = (const GPolygon3D &gPolygon3D);
#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


class TL_EXPORT GMultiPolygon 
  : public geometry::MultiPolygon<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  GMultiPolygon();
  GMultiPolygon(const MultiPolygon<geometry::Point<double>> &multiPolygon);
  GMultiPolygon(const GMultiPolygon &gMultiPolygon);
  ~GMultiPolygon();

  GMultiPolygon &operator = (const GMultiPolygon &gMultiPolygon);
#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


class TL_EXPORT GMultiPolygon3D 
  : public geometry::MultiPolygon<geometry::Point3<double>>, 
    public GraphicEntity
{
public:

  GMultiPolygon3D();
  GMultiPolygon3D(const MultiPolygon<geometry::Point3<double>> &multiPolygon);
  GMultiPolygon3D(const GMultiPolygon3D &gMultiPolygon3D);
  ~GMultiPolygon3D();

  GMultiPolygon3D &operator = (const GMultiPolygon3D &gMultiPolygon3D);
#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};



} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_POLYGON_H
