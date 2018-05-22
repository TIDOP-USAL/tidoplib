#include "graphic/canvas.h"


#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

namespace TL
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */

Canvas::Canvas()
  : mWidth(0),
    mHeight(0)
{
}

Canvas::~Canvas()
{
}

int Canvas::getWidth()
{
  return mWidth;
}

int Canvas::getHeight()
{
  return mHeight;
}
 
void Canvas::setSize(int width, int height)
{
  mWidth = width;
  mHeight = height;
}

void Canvas::setBackgroundColor(const Color &color)
{

}

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

CanvasCV::CanvasCV()
  : Canvas()
{
}

CanvasCV::~CanvasCV()
{
}

void CanvasCV::drawPoint(const GPoint &point)
{
  cv::Point pt = point;
  cv::Scalar color = point.getStylePen()->getPenColor().get<cv::Scalar>();

  cv::MarkerTypes markerType;

  switch (point.getStyleSymbol()->getName()) {
  case TL::graph::StyleSymbol::SymbolName::CROSS:
    break;
  case TL::graph::StyleSymbol::SymbolName::DIAGONAL_CROSS:
    markerType = cv::MARKER_TILTED_CROSS;
    break;
  case TL::graph::StyleSymbol::SymbolName::CIRCLE:
    break;
  case TL::graph::StyleSymbol::SymbolName::CIRCLE_FILLED:
    break;
  case TL::graph::StyleSymbol::SymbolName::SQUARE:
    break;
  case TL::graph::StyleSymbol::SymbolName::SQUARE_FILLED:
    break;
  case TL::graph::StyleSymbol::SymbolName::TRIANGLE:
    break;
  case TL::graph::StyleSymbol::SymbolName::TRIANGLE_FILLED:
    break;
  case TL::graph::StyleSymbol::SymbolName::STAR:
    markerType = cv::MARKER_STAR;
    break;
  case TL::graph::StyleSymbol::SymbolName::STAR_FILLED:
    break;
  case TL::graph::StyleSymbol::SymbolName::VERTICAL_BAR:
    break;
  default:
    break;
  }

  
  //TODO: Un poco limitado para todos los casos...
  cv::drawMarker(mCanvas, pt, color, markerType);


}

void CanvasCV::drawLineString(const GLineString &lineString)
{

}

void CanvasCV::drawPolygon(const GPolygon &polygon)
{

}

#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace TL
