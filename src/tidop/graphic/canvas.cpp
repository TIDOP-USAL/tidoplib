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
    mBgColor(Color::Name::white)
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

int CanvasCV::width() const
{
  return mCanvas.cols;
}

int CanvasCV::height() const
{
  return mCanvas.rows;
}

Color CanvasCV::backgroundColor() const
{
  return mBgColor;
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

void CanvasCV::setBackgroundColor(const Color &color)
{
  mBgColor = color;
  update();
}

void CanvasCV::drawPoint(const GPoint &point)
{
  StyleSymbol *style_symbol = point.styleSymbol();
  StylePen *style_pen = point.stylePen();
  Color c = style_pen->color();
  cv::Scalar color = colorToCvScalar(c);
  PointD pt_offset(style_symbol->offsetX(), style_symbol->offsetY());
  cv::Point pt = point + pt_offset;

  switch (style_symbol->name()) {
  case StyleSymbol::Name::cross:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::diagonal_cross:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TILTED_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::circle:
    cv::circle(mCanvas, pt, 10, color, 1);
    break;
  case StyleSymbol::Name::circle_filled:
    cv::circle(mCanvas, pt, 10, color, -1);
    break;
  case StyleSymbol::Name::square:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_SQUARE, 10, 1);
    break;
  case StyleSymbol::Name::square_filled:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_SQUARE, 10, -1);
    break;
  case StyleSymbol::Name::triangle:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TRIANGLE_UP, 10, 1);
    break;
  case StyleSymbol::Name::triangle_filled:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TRIANGLE_UP, 10, -1);
    break;
  case StyleSymbol::Name::star:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_STAR, 10, 1);
    break;
  case StyleSymbol::Name::star_filled:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_STAR, 10, -1);
    break;
  case StyleSymbol::Name::vertical_bar:
    break;
  default:
    cv::line(mCanvas, pt, pt, color, style_pen->width());
    break;
  }

  StyleLabel *style_label = point.styleLabel();
  Font font = style_label->font();
  Color foreColor = style_label->foregroundColor();
  cv::QtFont qt_font = cv::fontQt(font.name(),font.size(), colorToCvScalar(foreColor),
                                  font.isBold() ? CV_FONT_BOLD : CV_FONT_NORMAL,
                                  font.isItalic() ? CV_STYLE_ITALIC : CV_STYLE_NORMAL);
  cv::addText(mCanvas, style_label->text(), pt, qt_font);
}

void CanvasCV::drawPoint(const geometry::PointD &point, const GraphicStyle &style)
{
  StyleSymbol *style_symbol = style.styleSymbol();
  StylePen *style_pen = style.stylePen();
  Color c = style_pen->color();
  cv::Scalar color = colorToCvScalar(c);
  PointD pt_offset(style_symbol->offsetX(), style_symbol->offsetY());
  cv::Point pt = point + pt_offset;

  switch (style_symbol->name()) {
  case StyleSymbol::Name::cross:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::diagonal_cross:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TILTED_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::circle:
    cv::circle(mCanvas, pt, 10, color, 1);
    break;
  case StyleSymbol::Name::circle_filled:
    cv::circle(mCanvas, pt, 10, color, -1);
    break;
  case StyleSymbol::Name::square:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_SQUARE, 10, 1);
    break;
  case StyleSymbol::Name::square_filled:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_SQUARE, 10, -1);
    break;
  case StyleSymbol::Name::triangle:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TRIANGLE_UP, 10, 1);
    break;
  case StyleSymbol::Name::triangle_filled:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_TRIANGLE_UP, 10, -1);
    break;
  case StyleSymbol::Name::star:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_STAR, 10, 1);
    break;
  case StyleSymbol::Name::star_filled:
    cv::drawMarker(mCanvas, pt, color, cv::MARKER_STAR, 10, -1);
    break;
  case StyleSymbol::Name::vertical_bar:
    break;
  default:
    cv::line(mCanvas, pt, pt, color, style_pen->width());
    break;
  }

  StyleLabel *style_label = style.styleLabel();
  Font font = style_label->font();
  Color foreColor = style_label->foregroundColor();
  cv::QtFont qt_font = cv::fontQt(font.name(),font.size(),
                                  colorToCvScalar(foreColor),
                                  font.isBold() ? CV_FONT_BOLD : CV_FONT_NORMAL,
                                  font.isItalic() ? CV_STYLE_ITALIC : CV_STYLE_NORMAL);
  cv::addText(mCanvas, style_label->text(), pt, qt_font);
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

  StylePen *style_pen = lineString.stylePen();
  if (style_pen->pattern().empty()){
    ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
  } else {
    Color color = style_pen->color();
    cv::polylines(mCanvas, pts, false, colorToCvScalar(color), style_pen->width());
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
  mCanvas = cv::Mat(mHeight, mWidth, CV_MAKETYPE(CV_8U, 3), colorToCvScalar(mBgColor));
}

cv::Scalar CanvasCV::colorToCvScalar(const Color &color)
{
  return cv::Scalar(static_cast<double>(color.blue()),
                    static_cast<double>(color.green()),
                    static_cast<double>(color.red()));
}


#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace TL
