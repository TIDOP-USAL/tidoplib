#ifndef TL_GRAPHIC_PAINTER_H
#define TL_GRAPHIC_PAINTER_H

#include "config_tl.h"
#include "core/defs.h"

#include "geometry/entities/point.h"
#include "graphic/entities/point.h"
#include "graphic/entities/polygon.h"
#include "graphic/entities/linestring.h"

namespace TL
{

template<typename T> class Transform;


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

class Canvas;

class TL_EXPORT Painter
{
protected:

  Transform<geometry::PointF> *mTrf;
  Canvas *mCanvas;

public:

  Painter();
  Painter(Canvas *canvas);
  Painter(const Painter &painter);
  ~Painter();

  void drawPoint(const GPoint &point);
  void drawLineString(const GLineString &lineString);
  void drawPolygon(const GPolygon &polygon);
  void drawMultiPoint(const GMultiPoint &point);
  void drawMultiLineString(const GMultiLineString &multiLineString);
  void drawMultiPolygon(const GMultiPolygon &multiPolygon);

  void setCanvas(Canvas *canvas);
  //void setPen(const StylePen &pen);
  //void setBrush(const StyleBrush &brush);
  //void setSymbol(const StyleSymbol &symbol);
  //void setStyleLabel(const StyleLabel &styleLabel);

  void setTransform(Transform<geometry::PointF> *trf);

  //void drawImage(const RasterGraphics &image, const geometry::WindowI &w);
  //void drawImage(const RasterGraphics &image, Helmert2D<geometry::PointI> *trf);

private:

};


} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_PAINTER_H
