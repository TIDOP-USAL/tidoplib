#include "graphic/painter.h"

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

#include "graphic/canvas.h"

namespace TL
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */

Painter::Painter()
  : mTrf(nullptr),
    mCanvas(nullptr)
{
}

Painter::Painter(Canvas *canvas)
  : mTrf(nullptr),
    mCanvas(canvas)
{
}

Painter::Painter(const Painter &painter)
  : mTrf(painter.mTrf),
    mCanvas(painter.mCanvas)
{
}

Painter::~Painter()
{
}

void Painter::drawPoint(const GPoint &point) 
{
  mCanvas->drawPoint(point);
}

void Painter::drawLineString(const GLineString &lineString)
{

}

void Painter::drawPolygon(const GPolygon &polygon)
{

}
  
void Painter::drawMultiPoint(const GMultiPoint &point)
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

//void Painter::setPen(const StylePen &pen)
//{
//
//}
//
//void Painter::setBrush(const StyleBrush &brush)
//{
//
//}
//
//void Painter::setSymbol(const StyleSymbol &symbol)
//{
//
//}
//
//void Painter::setStyleLabel(const StyleLabel &styleLabel)
//{
//
//}

//void Painter::setTransform(Transform<geometry::PointF> *trf)
//{
//  mTrf = trf;
//}



} // Fin namespace graph

} // End namespace TL
