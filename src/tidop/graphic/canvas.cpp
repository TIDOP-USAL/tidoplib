#include "tidop/graphic/canvas.h"

#include "tidop/geometry/entities/point.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif


namespace tl
{

using namespace geometry;

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

CanvasCV::CanvasCV()
  : Canvas(),
    mWidth(100),
    mHeight(100),
    mBgColor(tl::Color::NAME::White)
{
  update();
}

CanvasCV::CanvasCV(const CanvasCV &canvas)
  : Canvas(),
    mWidth(canvas.mWidth),
    mHeight(canvas.mHeight),
    mBgColor(canvas.mBgColor)
{
  update();
}

CanvasCV::~CanvasCV()
{
}

int CanvasCV::getWidth() const
{
  return mCanvas.cols;
}

int CanvasCV::getHeight() const
{
  return mCanvas.rows;
}


void CanvasCV::setWidth(int width)
{
  mWidth = width;
  update();
}

void CanvasCV::setHeight(int height)
{
  mHeight = height;
  update();
}

void CanvasCV::setSize(int width, int height)
{
  mWidth = width;
  mHeight = height;
  update();
}

void CanvasCV::setBackgroundColor(const tl::Color &color)
{
  mBgColor = color;
  update();
}

void CanvasCV::drawPoint(const GPoint &point)
{
  //cv::Scalar color = point.getStylePen()->getPenColor().get<cv::Scalar>();
  cv::Scalar color = point.getStylePen()->getPenColor().toCvScalar();
  PointD pt_offset(point.getStyleSymbol()->getOffsetX(), point.getStyleSymbol()->getOffsetY());
  cv::Point pt = point + pt_offset;

  switch (point.getStyleSymbol()->getName()) {
  case tl::graph::StyleSymbol::SymbolName::CROSS:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_CROSS, 10, 1);
    break;
  case tl::graph::StyleSymbol::SymbolName::DIAGONAL_CROSS:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TILTED_CROSS, 10, 1);
    break;
  case tl::graph::StyleSymbol::SymbolName::CIRCLE:
    cv::circle(mCanvas, pt, 10, color, 1);
    break;
  case tl::graph::StyleSymbol::SymbolName::CIRCLE_FILLED:
    cv::circle(mCanvas, pt, 10, color, -1);
    break;
  case tl::graph::StyleSymbol::SymbolName::SQUARE:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_SQUARE, 10, 1);
    break;
  case tl::graph::StyleSymbol::SymbolName::SQUARE_FILLED:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_SQUARE, 10, -1);
    break;
  case tl::graph::StyleSymbol::SymbolName::TRIANGLE:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TRIANGLE_UP, 10, 1);
    break;
  case tl::graph::StyleSymbol::SymbolName::TRIANGLE_FILLED:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TRIANGLE_UP, 10, -1);
    break;
  case tl::graph::StyleSymbol::SymbolName::STAR:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_STAR, 10, 1);
    break;
  case tl::graph::StyleSymbol::SymbolName::STAR_FILLED:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_STAR, 10, -1);
    break;
  case tl::graph::StyleSymbol::SymbolName::VERTICAL_BAR:
    break;
  default:
    cv::line(mCanvas, pt, pt, color, point.getStylePen()->getPenWidth());
    break;
  }

  StyleLabel *style_label = point.getStyleLabel();
  Font font = style_label->getFont();
  cv::QtFont qt_font = cv::fontQt(font.getName(),font.getSize(),
                                  style_label->getForegroundColor().toCvScalar(),
                                  font.isBold() ? CV_FONT_BOLD : CV_FONT_NORMAL,
                                  font.isItalic() ? CV_STYLE_ITALIC : CV_STYLE_NORMAL);
  cv::addText(mCanvas, style_label->getText(), pt, qt_font);
}

void CanvasCV::drawLineString(const GLineString &lineString)
{
  size_t n = lineString.size();
  std::vector<std::vector<cv::Point>> pts(1,std::vector<cv::Point>(n));
  for (size_t i = 0; i < n; i++){
    pts[0][i] = lineString[i];
  }

//  const cv::Point *cpts = (const cv::Point*) cv::Mat(pts).data;
//  int npts = cv::Mat(pts).rows;

  StylePen *stylePen = lineString.getStylePen();
  if (stylePen->getPattern().empty()){
    ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
  } else {
    //cv::polylines(mCanvas, &cpts, &npts, 1, false, stylePen->getPenColor().toCvScalar(), stylePen->getPenWidth());
    cv::polylines(mCanvas, pts, false, stylePen->getPenColor().toCvScalar(), stylePen->getPenWidth());
  }
}

void CanvasCV::drawPolygon(const GPolygon &polygon)
{

}

CanvasCV &CanvasCV::operator =(const CanvasCV &canvas)
{
  if (this != &canvas){
    mWidth = canvas.mWidth;
    mHeight = canvas.mHeight;
    mBgColor = canvas.mBgColor;
  }
  return *this;
}

void CanvasCV::update()
{
  mCanvas = cv::Mat(mHeight, mWidth, CV_MAKETYPE(CV_8U, 3), mBgColor.toCvScalar());
}


#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace TL
