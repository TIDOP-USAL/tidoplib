#ifndef TL_GRAPHIC_CANVAS_H
#define TL_GRAPHIC_CANVAS_H

#include "config_tl.h"

#include "core/defs.h"
#include "graphic/color.h"
#include "graphic/styles.h"
#include "graphic/entities/point.h"
#include "graphic/entities/linestring.h"
#include "graphic/entities/polygon.h"

namespace TL
{

template<typename T> class Transform;


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

class TL_EXPORT Canvas
{
protected:

  int mWidth;
  int mHeight;
   
public:

  Canvas();
  ~Canvas();

  int getWidth();
  int getHeight();
   
  void setSize(int width, int height);
  void setBackgroundColor(const Color &color);

  virtual void drawPoint(const GPoint &point) = 0;
  virtual void drawLineString(const GLineString &lineString) = 0;
  virtual void drawPolygon(const GPolygon &polygon) = 0;

};

#ifdef HAVE_OPENCV

class TL_EXPORT CanvasCV : public Canvas
{

private:

  cv::Mat mCanvas;

public:

  CanvasCV();
  ~CanvasCV();

  virtual void drawPoint(const GPoint &point) override;
  virtual void drawLineString(const GLineString &lineString) override;
  virtual void drawPolygon(const GPolygon &polygon) override;

private:

};

#endif // HAVE_OPENCV

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_CANVAS_H
