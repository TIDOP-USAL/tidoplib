#include "graphic/entities.h"

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


//TODO: los métodos draw creo que se deberian quitar y crear una clase Painter virtual
//      y unas clases que hereden de esta (para OpenCv, Qt, ...)

namespace TL
{

using namespace geometry;

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */

StylePen::StylePen()
  : mPenColor(0),
    mPenWidth(1),
    mPattern(""),
    mPenName(PenName::SOLID),
    mPenCap(PenCap::BUTT),
    mPenJoin(PenJoin::BEVEL),
    mPerpendicularOffset(0),
    mPriorityLevel(0)
{
}

StylePen::StylePen(const StylePen &stylePen)
  : mPenColor(stylePen.mPenColor),
    mPenWidth(stylePen.mPenWidth),
    mPattern(stylePen.mPattern),
    mPenName(stylePen.mPenName),
    mPenCap(stylePen.mPenCap),
    mPenJoin(stylePen.mPenJoin),
    mPerpendicularOffset(stylePen.mPerpendicularOffset),
    mPriorityLevel(stylePen.mPriorityLevel)
{
}

StylePen::~StylePen()
{
}

Color StylePen::getPenColor() 
{
  return mPenColor;
}

void StylePen::setPenColor(Color pencolor) 
{
  mPenColor = pencolor;
}

uint8_t StylePen::getPenWidth() 
{
  return mPenWidth;
}

void StylePen::setPenWidth(uint8_t penwidth) 
{
  mPenWidth = penwidth;
}

std::string StylePen::getPattern() 
{
  return mPattern;
}

void StylePen::setPattern(std::string pattern) 
{
  mPattern = pattern;
}

StylePen::PenName StylePen::getPenName() 
{
  return mPenName;
}

void StylePen::setPenName(PenName penname) 
{
  mPenName = penname;
}

StylePen::PenCap StylePen::getPenCap() 
{
  return mPenCap;
}

void StylePen::setPenCap(PenCap pencap) 
{
  mPenCap = pencap;
}

StylePen::PenJoin StylePen::getPenJoin()
{
  return mPenJoin;
}

void StylePen::setPenJoin(PenJoin penjoin) 
{
  mPenJoin = penjoin;
}

int32_t StylePen::getPerpendicularOffset()
{
  return mPerpendicularOffset;
}

void StylePen::setPerpendicularOffset(int32_t perpendicularoffset)
{
  mPerpendicularOffset = perpendicularoffset;
}

uint32_t StylePen::getPriorityLevel() 
{
  return mPriorityLevel;
}

void StylePen::setPriorityLevel(uint32_t priorityLevel) 
{
  mPriorityLevel = priorityLevel;
}

StylePen &StylePen::operator = (const StylePen &stylePen)
{
  if (this != &stylePen) {
    mPenColor = stylePen.mPenColor;
    mPenWidth = stylePen.mPenWidth;
    mPattern = stylePen.mPattern;
    mPenName = stylePen.mPenName;
    mPenCap = stylePen.mPenCap;
    mPenJoin = stylePen.mPenJoin;
    mPerpendicularOffset = stylePen.mPerpendicularOffset;
    mPriorityLevel = stylePen.mPriorityLevel;
  }
  return *this;
}
/* ---------------------------------------------------------------------------------- */

StyleBrush::StyleBrush()
  : mForeColor(0),
    mBackColor(0),
    mBrushName(BrushName::SOLID),
    mAngle(0.),
    mScalingFactor(1.),
    mPriorityLevel(0)
{
}

StyleBrush::StyleBrush(const StyleBrush &styleBrush)
  : mForeColor(styleBrush.mForeColor),
    mBackColor(styleBrush.mBackColor),
    mBrushName(styleBrush.mBrushName),
    mAngle(styleBrush.mAngle),
    mScalingFactor(styleBrush.mScalingFactor),
    mPriorityLevel(styleBrush.mPriorityLevel)
{
}

StyleBrush::~StyleBrush()
{
}

Color StyleBrush::getForeColor()
{
  return mForeColor;
}

void StyleBrush::setForeColor(Color forecolor) 
{
  mForeColor = forecolor;
}

Color StyleBrush::getBackColor() 
{
  return mBackColor;
}

void StyleBrush::setBackColor(Color backcolor) 
{ 
  mBackColor = backcolor;
}

StyleBrush::BrushName StyleBrush::getBrushName() 
{
  return mBrushName;
}

void StyleBrush::setBrushName(BrushName brushname) 
{
  mBrushName = brushname;
}

double StyleBrush::getAngle() 
{
  return mAngle;
}

void StyleBrush::setAngle(double angle)
{
  mAngle = angle;
}

double StyleBrush::getScalingFactor() 
{
  return mScalingFactor;
}

void StyleBrush::setScalingFactor(double scalingFactor) 
{
  mScalingFactor = scalingFactor;
}

double StyleBrush::getSpacingX() 
{
  return mSpacing[0];
}

double StyleBrush::getSpacingY() 
{
  return mSpacing[1];
}

void StyleBrush::setSpacing(double spacingX, double spacingY ) 
{
    mSpacing[0] = spacingX; mSpacing[1] = spacingY;
}

uint32_t StyleBrush::getPriorityLevel() 
{
  return mPriorityLevel;
}

void StyleBrush::setPriorityLevel(uint32_t priorityLevel) 
{
  mPriorityLevel = priorityLevel;
}

StyleBrush &StyleBrush::operator = (const StyleBrush &styleBrush)
{
  if (this != &styleBrush) {
    mForeColor = styleBrush.mForeColor;
    mBackColor = styleBrush.mBackColor;
    mBrushName = styleBrush.mBrushName;
    mAngle = styleBrush.mAngle;
    mScalingFactor = styleBrush.mScalingFactor;
    mPriorityLevel = styleBrush.mPriorityLevel;
  }
  return *this;
}

/* ---------------------------------------------------------------------------------- */

StyleSymbol::StyleSymbol()
  : mName(SymbolName::CROSS),
    mAngle(0.),
    mColor(0),
    mOutlineColor(0),
    mScalingFactor(0.),
    mPriorityLevel(0)
{
}

StyleSymbol::StyleSymbol(const StyleSymbol &styleSymbol)
  : mName(styleSymbol.mName),
    mAngle(styleSymbol.mAngle),
    mColor(styleSymbol.mColor),
    mOutlineColor(styleSymbol.mOutlineColor),
    mScalingFactor(styleSymbol.mScalingFactor),
    mPriorityLevel(styleSymbol.mPriorityLevel)
{
}

StyleSymbol::~StyleSymbol()
{
}

double StyleSymbol::getAngle() 
{
  return mAngle;
}

void StyleSymbol::setAngle(double angle) 
{
  mAngle = angle;
}

StyleSymbol::SymbolName StyleSymbol::getName() 
{
  return mName;
}

void StyleSymbol::setName(SymbolName name) 
{
  mName = name;
}

Color StyleSymbol::getColor()
{
  return mColor;
}

void StyleSymbol::setColor(const Color &color) 
{
  mColor = color;
}

Color StyleSymbol::getOutlineColor() 
{
  return mOutlineColor;
}

void StyleSymbol::setOutlineColor(const Color &outlinecolor) 
{
  mOutlineColor = outlinecolor;
}

double StyleSymbol::getScalingFactor() 
{
  return mScalingFactor;
}

void StyleSymbol::setScalingFactor(double scalingFactor)
{
  mScalingFactor = scalingFactor;
}

double StyleSymbol::getOffsetX()
{
  return mOffset[0];
}

double StyleSymbol::getOffsetY()
{
  return mOffset[1];
}

void StyleSymbol::setOffset(double dx, double dy) 
{
  mOffset[0] = dx; mOffset[1] = dy;
}

uint32_t StyleSymbol::getPriorityLevel() 
{
  return mPriorityLevel;
}

void StyleSymbol::setPriorityLevel(uint32_t priorityLevel) 
{
  mPriorityLevel = priorityLevel;
}

StyleSymbol &StyleSymbol::operator = (const StyleSymbol &styleSymbol)
{
  if (this != &styleSymbol) {
    mName = styleSymbol.mName;
    mAngle = styleSymbol.mAngle;
    mColor = styleSymbol.mColor;
    mOutlineColor = styleSymbol.mOutlineColor;
    mScalingFactor = styleSymbol.mScalingFactor;
    mPriorityLevel = styleSymbol.mPriorityLevel;
  }
  return *this;
}
/* ---------------------------------------------------------------------------------- */

StyleLabel::StyleLabel()
  : /*mFontName("Arial"),
    mFontSize(12),*/
    mFont(),
    mText(""),
    mAngle(0.),
    mForegroundColor(0),
    mBackgroundColor(0),
    mOutlineColor(0),
    mShadowColor(0),
    mStretch(100.),
    mLabelPlacement(LabelPlacement::p),
    mAnchorPosition(AnchorPosition::VERTICAL_BASELINE | AnchorPosition::HORIZONTAL_LEFT),
    mPerpendicularOffset(0),
    /*bBold(false),
    bItalic(false),
    bUnderline(false),*/
    bStrikeout(false),
    mPriorityLevel(0)
{
}

StyleLabel::StyleLabel(const StyleLabel &styleLabel)
  : /*mFontName(styleLabel.mFontName),
    mFontSize(styleLabel.mFontSize),*/
    mFont(styleLabel.mFont),
    mText(styleLabel.mText),
    mAngle(styleLabel.mAngle),
    mForegroundColor(styleLabel.mForegroundColor),
    mBackgroundColor(styleLabel.mBackgroundColor),
    mOutlineColor(styleLabel.mOutlineColor),
    mShadowColor(styleLabel.mShadowColor),
    mStretch(styleLabel.mStretch),
    mLabelPlacement(styleLabel.mLabelPlacement),
    mAnchorPosition(styleLabel.mAnchorPosition),
    mPerpendicularOffset(styleLabel.mPerpendicularOffset),
    /*bBold(styleLabel.bBold),
    bItalic(styleLabel.bItalic),
    bUnderline(styleLabel.bUnderline),*/
    bStrikeout(styleLabel.bStrikeout),
    mPriorityLevel(styleLabel.mPriorityLevel)
{
}

StyleLabel::~StyleLabel()
{
}

double StyleLabel::getAngle() 
{
  return mAngle;
}

void StyleLabel::setAngle(double angle) 
{
  mAngle = angle;
}

Color StyleLabel::getForegroundColor()
{
  return mForegroundColor;
}

void StyleLabel::setForegroundColor(Color &color)
{
  mForegroundColor = color;
}

Color StyleLabel::getBackgroundColor() 
{
  return mBackgroundColor;
}

void StyleLabel::setBackgroundColor(Color &color)
{
  mBackgroundColor = color;
}

Color StyleLabel::getOutlineColor() 
{
  return mOutlineColor;
}

void StyleLabel::setOutlineColor(Color &color)
{
  mOutlineColor = color;
}

Color StyleLabel::getShadowColor() 
{
  return mShadowColor;
}

void StyleLabel::setShadowColor(Color &color)
{
  mShadowColor = color;
}

double StyleLabel::getStretch()
{
  return mStretch;
}

void StyleLabel::setStretch(double stretch)
{
  mStretch = stretch;
}

StyleLabel::LabelPlacement StyleLabel::getLabelPlacement()
{
  return mLabelPlacement;
}

void StyleLabel::setLabelPlacement(LabelPlacement labelPlacement) 
{
  mLabelPlacement = labelPlacement;
}

StyleLabel::AnchorPosition StyleLabel::getAnchorPosition() 
{
  return mAnchorPosition;
}

void StyleLabel::setAnchorPosition(AnchorPosition anchorPosition)
{
  mAnchorPosition = anchorPosition;
}

double StyleLabel::getOffsetX()
{
  return mOffset[0];
}

double StyleLabel::getOffsetY()
{
  return mOffset[1];
}

void StyleLabel::setOffset(double dx, double dy) 
{
  mOffset[0] = dx; 
  mOffset[1] = dy;
}

StyleLabel &StyleLabel::operator = (const StyleLabel &styleLabel)
{
  if (this != &styleLabel) {
    mFontName = styleLabel.mFontName;
    mFontSize = styleLabel.mFontSize;
    mText = styleLabel.mText;
    mAngle = styleLabel.mAngle;
    mForegroundColor = styleLabel.mForegroundColor;
    mBackgroundColor = styleLabel.mBackgroundColor;
    mOutlineColor = styleLabel.mOutlineColor;
    mShadowColor = styleLabel.mShadowColor;
    mStretch = styleLabel.mStretch;
    mLabelPlacement = styleLabel.mLabelPlacement;
    mAnchorPosition = styleLabel.mAnchorPosition;
    mPerpendicularOffset = styleLabel.mPerpendicularOffset;
    bBold = styleLabel.bBold;
    bItalic = styleLabel.bItalic;
    bUnderline = styleLabel.bUnderline;
    bStrikeout = styleLabel.bStrikeout;
    mPriorityLevel = styleLabel.mPriorityLevel;
  }
  return *this;
}

void StyleLabel::setFont(const Font &font)
{
  mFont = font;
}

Font StyleLabel::getFont() const
{
  return mFont;
}

StyleLabel &StyleLabel::operator = (const StyleLabel &styleLabel)
{
  if (this != &styleLabel) {
    /*mFontName = styleLabel.mFontName;
    mFontSize = styleLabel.mFontSize;*/
	  mFont = styleLabel.mFont;
    mText = styleLabel.mText;
    mAngle = styleLabel.mAngle;
    mForegroundColor = styleLabel.mForegroundColor;
    mBackgroundColor = styleLabel.mBackgroundColor;
    mOutlineColor = styleLabel.mOutlineColor;
    mShadowColor = styleLabel.mShadowColor;
    mStretch = styleLabel.mStretch;
    mLabelPlacement = styleLabel.mLabelPlacement;
    mAnchorPosition = styleLabel.mAnchorPosition;
    mPerpendicularOffset = styleLabel.mPerpendicularOffset;
    /*bBold = styleLabel.bBold;
    bItalic = styleLabel.bItalic;
    bUnderline = styleLabel.bUnderline;*/
    bStrikeout = styleLabel.bStrikeout;
    mPriorityLevel = styleLabel.mPriorityLevel;
  }
  return *this;
}

/* ---------------------------------------------------------------------------------- */

GraphicStyle::GraphicStyle()
{
}

GraphicStyle::GraphicStyle(const GraphicStyle &graphicStyle)
  : mStylePen(graphicStyle.mStylePen),
    mStyleBrush(graphicStyle.mStyleBrush),
    mStyleSymbol(graphicStyle.mStyleSymbol),
    mStyleLabel(graphicStyle.mStyleLabel)
{
}

#ifdef HAVE_GDAL
bool GraphicStyle::readFromOGR(OGRStyleMgr *ogrStyle)
{
  OGRStyleTool *ogrStyleTool = nullptr;
  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
    if ((ogrStyleTool = ogrStyle->GetPart(i)) != nullptr) {
      OGRSTClassId oci = ogrStyleTool->GetType();
      switch (oci) {
      case OGRSTCPen:
        readStylePen(dynamic_cast<OGRStylePen *>(ogrStyleTool));
        break;
      case OGRSTCBrush:
        readStyleBrush(dynamic_cast<OGRStyleBrush *>(ogrStyleTool));
        break;
      case OGRSTCSymbol:
        readStyleSymbol(dynamic_cast<OGRStyleSymbol *>(ogrStyleTool));
        break;
      case OGRSTCLabel:
        readStyleLabel(dynamic_cast<OGRStyleLabel *>(ogrStyleTool));
        break;
      case OGRSTCVector:

        break;
      default:
        break;
      }
    }
  }
  return false;
}
#endif 

bool GraphicStyle::write()
{
  return false;
}

std::shared_ptr<StylePen> GraphicStyle::getStylePen() const
{
  return mStylePen;
}

void GraphicStyle::setStylePen(std::shared_ptr<StylePen> stylePen)
{
  mStylePen = stylePen;
}

void GraphicStyle::setStyleBrush(std::shared_ptr<StyleBrush> styleBrush)
{
  mStyleBrush = styleBrush;
}

std::shared_ptr<StyleSymbol> GraphicStyle::getStyleSymbol() const
{
  return mStyleSymbol;
}
void GraphicStyle::setStyleSymbol(std::shared_ptr<StyleSymbol> styleSymbol)
{
  mStyleSymbol = styleSymbol;
}

void GraphicStyle::setStyleLabel(std::shared_ptr<StyleLabel> styleLabel)
{
  mStyleLabel = styleLabel;
}

GraphicStyle &GraphicStyle::operator = (const GraphicStyle &graphicStyle)
{ 
  if (this != &graphicStyle) {
    mStylePen = graphicStyle.mStylePen;
    mStyleBrush = graphicStyle.mStyleBrush;
    mStyleSymbol = graphicStyle.mStyleSymbol;
    mStyleLabel = graphicStyle.mStyleLabel;
  }
  return *this;
}
#ifdef HAVE_GDAL

void GraphicStyle::readStylePen(OGRStylePen *ogrStylePen)
{
  GBool bDefault = false;
  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();
  const char *hexColor = ogrStylePen->Color(bDefault);
  if (!bDefault) stylePen->setPenColor(Color(hexColor));

  double width = ogrStylePen->Width(bDefault);
  if ( !bDefault ) { 
    //unsigned __int8 w = (unsigned __int8)GetWidthPx( wd, poStylePen->GetUnit() );
    //gs->AddStyle( gstPenWidth, w );
    //stylePen->setPenWidth();
  }

  const char *pattern = ogrStylePen->Pattern(bDefault);
  if (!bDefault) stylePen->setPattern(pattern);

  //const char *name = ogrStylePen->Id(bDefault);
  //if ( !bDefault ) {
  //  stylePen->setPenName()
  //}

  const char *cap = ogrStylePen->Cap(bDefault);
  if ( !bDefault ) {
    StylePen::PenCap penCap;
    if (strcmp(cap, "") == 0) penCap = StylePen::PenCap::BUTT;
    else if (strcmp(cap, "") == 0) penCap = StylePen::PenCap::PROJECTING;
    else if (strcmp(cap, "") == 0) penCap = StylePen::PenCap::ROUND;
    stylePen->setPenCap(penCap);
  }

  const char *join = ogrStylePen->Join(bDefault);
  if ( !bDefault ) {
    StylePen::PenJoin penJoin;
    if (strcmp(join, "") == 0) penJoin = StylePen::PenJoin::MITER;
    else if (strcmp(join, "") == 0) penJoin = StylePen::PenJoin::ROUNDED;
    else if (strcmp(join, "") == 0) penJoin = StylePen::PenJoin::BEVEL;
    stylePen->setPenJoin(penJoin);
  }

  double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
  if ( !bDefault ) {
    stylePen->setPerpendicularOffset(TL_ROUND_TO_INT(perpendicularOffset));
  }

  setStylePen(stylePen);
}

void GraphicStyle::readStyleBrush(OGRStyleBrush *ogrStyleBrush)
{

}

void GraphicStyle::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol)
{

}

void GraphicStyle::readStyleLabel(OGRStyleLabel *ogrStyleLabel)
{

}

#endif

/* ---------------------------------------------------------------------------------- */


GraphicEntity::GraphicEntity()
  : GraphicStyle(),
    GData()
{
}

GraphicEntity::GraphicEntity(const GraphicEntity &graphicEntity)
  : GraphicStyle(graphicEntity),
    GData(graphicEntity)
{
}

GraphicEntity::~GraphicEntity()
{
}

GraphicEntity &GraphicEntity::operator = (const GraphicEntity &graphicEntity)
{ 
  if (this != &graphicEntity) {
    GraphicStyle::operator=(graphicEntity);
    GData::operator=(graphicEntity);
  }
  return *this;
}

/* ---------------------------------------------------------------------------------- */

GPoint::GPoint() 
  : Point<double>(), 
    GraphicEntity()
{
}

GPoint::GPoint(double x, double y) 
  : Point<double>(x, y), 
    GraphicEntity()
{
}

GPoint::GPoint(const Point<double> &pt) 
  : Point<double>(pt), 
    GraphicEntity()
{
}  

GPoint::GPoint(const GPoint &pt) 
  : Point<double>(pt), 
    GraphicEntity(pt)
{
}  

GPoint::~GPoint()
{}

GPoint &GPoint::operator = (const GPoint &gPoint)
{
  if (this != &gPoint) {
    Point<double>::operator=(gPoint);
    GraphicEntity::operator=(gPoint);
  }
  return *this;
}
#ifdef HAVE_OPENCV
void GPoint::draw(cv::Mat &canvas) const
{

  // TODO: Hay que incluir la transformación terreno-pantalla
  //cv::drawMarker(canvas, ..... );
}
#endif

/* ---------------------------------------------------------------------------------- */

GPoint3D::GPoint3D() 
  : Point3<double>(), 
    GraphicEntity()
{
}

GPoint3D::GPoint3D(double x, double y, double z) 
  : Point3<double>(x, y, z), 
    GraphicEntity()
{
}

GPoint3D::GPoint3D(const Point3<double> &pt) 
  : Point3<double>(pt), 
    GraphicEntity()
{
}

GPoint3D::GPoint3D(const GPoint3D &pt) 
  : Point3<double>(pt), 
    GraphicEntity(pt)
{
}

GPoint3D::~GPoint3D()
{
}

GPoint3D &GPoint3D::operator = (const GPoint3D &gPoint)
{
  if (this != &gPoint) {
    Point3<double>::operator=(gPoint);
    GraphicEntity::operator=(gPoint);
  }
  return *this;
}
#ifdef HAVE_OPENCV
void GPoint3D::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GLineString::GLineString() 
  : LineString<Point<double>>(), 
    GraphicEntity()
{
}

GLineString::GLineString(const LineString<Point<double>> &lineString) 
  : LineString<Point<double>>(lineString), 
    GraphicEntity()
{
}

GLineString::GLineString(const GLineString &lineString) 
  : LineString<Point<double>>(lineString), 
    GraphicEntity(lineString)
{
}

GLineString::~GLineString()
{
}

GLineString &GLineString::operator = (const GLineString &gLineString)
{
  if (this != &gLineString) {
    LineString<Point<double>>::operator=(gLineString);
    GraphicEntity::operator=(gLineString);
  }
  return *this;
}

#ifdef HAVE_OPENCV
void GLineString::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GPolygon::GPolygon() 
  : Polygon<Point<double>>(), 
    GraphicEntity()
{
}

GPolygon::GPolygon(const Polygon<Point<double>> &polygon) 
  : Polygon<Point<double>>(polygon), 
    GraphicEntity()
{
}

GPolygon::GPolygon(const GPolygon &gPolygon) 
  : Polygon<Point<double>>(gPolygon), 
    GraphicEntity(gPolygon)
{
}

GPolygon::~GPolygon()
{
}

GPolygon &GPolygon::operator = (const GPolygon &gPolygon)
{
  if (this != &gPolygon) {
    Polygon<Point<double>>::operator=(gPolygon);
    GraphicEntity::operator=(gPolygon);
  }
  return *this;
}

#ifdef HAVE_OPENCV
void GPolygon::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GMultiPoint::GMultiPoint()
  : MultiPoint<Point<double>>(), 
    GraphicEntity()
{
}

GMultiPoint::GMultiPoint(const MultiPoint<Point<double>> &multiPoint)
  : MultiPoint<Point<double>>(multiPoint), 
    GraphicEntity()
{
}

GMultiPoint::GMultiPoint(const GMultiPoint &gMultiPoint)
  : MultiPoint<Point<double>>(gMultiPoint), 
    GraphicEntity(gMultiPoint)
{
}

GMultiPoint::~GMultiPoint()
{
}

GMultiPoint &GMultiPoint::operator = (const GMultiPoint &gMultiPoint)
{
  if (this != &gMultiPoint) {
    MultiPoint<Point<double>>::operator=(gMultiPoint);
    GraphicEntity::operator=(gMultiPoint);
  }
  return *this;
}

#ifdef HAVE_OPENCV
void GMultiPoint::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GMultiLineString::GMultiLineString()
  : MultiLineString<Point<double>>(), 
    GraphicEntity()
{
}

GMultiLineString::GMultiLineString(const MultiLineString<geometry::Point<double>> &multiLineString)
  : MultiLineString<Point<double>>(multiLineString), 
    GraphicEntity()
{
}

GMultiLineString::GMultiLineString(const GMultiLineString &gMultiLineString)
  : MultiLineString<Point<double>>(gMultiLineString), 
    GraphicEntity(gMultiLineString)
{
}

GMultiLineString::~GMultiLineString()
{
}

GMultiLineString &GMultiLineString::operator = (const GMultiLineString &gMultiLineString)
{
  if (this != &gMultiLineString) {
    MultiLineString<Point<double>>::operator=(gMultiLineString);
    GraphicEntity::operator=(gMultiLineString);
  }
  return *this;
}
#ifdef HAVE_OPENCV
void GMultiLineString::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GMultiPolygon::GMultiPolygon()
  : MultiPolygon<Point<double>>(), 
    GraphicEntity()
{
}

GMultiPolygon::GMultiPolygon(const MultiPolygon<Point<double>> &multiPolygon)
  : MultiPolygon<Point<double>>(multiPolygon), 
    GraphicEntity()
{
}

GMultiPolygon::GMultiPolygon(const GMultiPolygon &gMultiPolygon)
  : MultiPolygon<Point<double>>(gMultiPolygon), 
    GraphicEntity(gMultiPolygon)
{
}

GMultiPolygon::~GMultiPolygon()
{
}

GMultiPolygon &GMultiPolygon::operator = (const GMultiPolygon &gMultiPolygon)
{
  if (this != &gMultiPolygon) {
    MultiPolygon<Point<double>>::operator=(gMultiPolygon);
    GraphicEntity::operator=(gMultiPolygon);
  }
  return *this;
}
#ifdef HAVE_OPENCV
void GMultiPolygon::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GLayer::GLayer() 
  : mName(""), 
    mEntities(0), 
    mSelectEntity(0)
{
}

GLayer::GLayer(const GLayer &gLayer) 
  : mName(gLayer.mName), 
    mEntities(gLayer.mEntities), 
    mSelectEntity(gLayer.mSelectEntity)
{
}

std::string GLayer::getName() const
{
	return mName;
}

void GLayer::setName(const std::string & name)
{
  mName = name;
}

void GLayer::add(std::shared_ptr<GraphicEntity> entity)
{

}

void GLayer::remove()
{

}


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
