#ifndef TL_GRAPHIC_ENTITIES_LINESTRING_H
#define TL_GRAPHIC_ENTITIES_LINESTRING_H

#include "config_tl.h"
#include "core/defs.h"

#include "graphic/entities/entity.h"
#include "geometry/entities/point.h"
#include "geometry/entities/lineString.h"

namespace TL
{

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

class TL_EXPORT GLineString : public geometry::LineString<geometry::Point<double>>, public GraphicEntity
{
public:

  GLineString();
  GLineString(const LineString<geometry::Point<double>> &lineString);
  GLineString(const GLineString &lineString);
  ~GLineString();

  GLineString &operator = (const GLineString &gLineString);
#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


class TL_EXPORT GMultiLineString 
  : public geometry::MultiLineString<geometry::Point<double>>, 
    public GraphicEntity
{
public:

  GMultiLineString();
  GMultiLineString(const MultiLineString<geometry::Point<double>> &multiLineString);
  GMultiLineString(const GMultiLineString &gMultiLineString);
  ~GMultiLineString();

  GMultiLineString &operator = (const GMultiLineString &gMultiLineString);
#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_ENTITIES_LINESTRING_H
