#include "tidop/graphic/painter.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_core.h"
#include "ogr_featurestyle.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include "tidop/graphic/canvas.h"

namespace tl
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */

Painter::Painter()
  : mTrf(nullptr),
    mCanvas(nullptr),
    mGraphicStyle(new GraphicStyle)
{
}

Painter::Painter(Canvas *canvas)
  : mTrf(nullptr),
    mCanvas(canvas),
    mGraphicStyle(new GraphicStyle)
{
}

Painter::Painter(const Painter &painter)
  : mTrf(painter.mTrf),
    mCanvas(painter.mCanvas),
    mGraphicStyle(painter.mGraphicStyle)
{
}

Painter::~Painter()
{
  if (mGraphicStyle) delete mGraphicStyle;
}

void Painter::drawPoint(const GPoint &point) 
{
  if (mCanvas){
    mCanvas->drawPoint(point);
  } else {
     msgError("Canvas not defined");
  }
}

void Painter::drawPoint(const geometry::PointD &point)
{
  if (mCanvas){
    mCanvas->drawPoint(point, *mGraphicStyle);
  } else {
     msgError("Canvas not defined");
  }
}

void Painter::drawLineString(const GLineString &lineString)
{
  if (mCanvas){
    mCanvas->drawLineString(lineString);
  } else {
    msgError("Canvas not defined");
  }
}

void Painter::drawPolygon(const GPolygon &polygon)
{
  if (mCanvas){
    mCanvas->drawPolygon(polygon);
  } else {
    msgError("Canvas not defined");
  }
}
  
void Painter::drawMultiPoint(const GMultiPoint &multipoint)
{

}

void Painter::drawMultiLineString(const GMultiLineString &multiLineString)
{

}

void Painter::drawMultiPolygon(const GMultiPolygon &multiPolygon)
{

}

void Painter::setCanvas(Canvas *canvas)
{
  mCanvas = canvas;
}

void Painter::setPen(const std::shared_ptr<StylePen> &pen)
{
  mGraphicStyle->setStylePen(pen);
}

void Painter::setBrush(const std::shared_ptr<StyleBrush> &brush)
{
  mGraphicStyle->setStyleBrush(brush);
}

void Painter::setSymbol(const std::shared_ptr<StyleSymbol> &symbol)
{
  mGraphicStyle->setStyleSymbol(symbol);
}

void Painter::setStyleLabel(const std::shared_ptr<StyleLabel> &styleLabel)
{
  mGraphicStyle->setStyleLabel(styleLabel);
}

void Painter::setTransform(Transform<geometry::PointF> *trf)
{
  mTrf = trf;
}



} // Fin namespace graph

} // End namespace TL
