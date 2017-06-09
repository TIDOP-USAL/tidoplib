#include "graphic_entities/graphic_entity.h"

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_core.h"
#include "ogr_featurestyle.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL


namespace I3D
{

using namespace geometry;

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */

StylePen::StylePen()
{
  // valores por defecto
  mPenColor = Color(0);
  mPenWidth = 1;
  mPattern = "";
  mPenName = PenName::SOLID;
  mPenCap = PenCap::BUTT;
  mPenJoin = PenJoin::BEVEL;
  mPerpendicularOffset = 0;
  mPriorityLevel = 0;
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

/* ---------------------------------------------------------------------------------- */

StyleBrush::StyleBrush()
{
  // valores por defecto
  mForeColor = Color(0);
  mForeColor = Color(0);
  mBrushName = BrushName::SOLID;
  mAngle = 0.;
  mScalingFactor = 1.;
  mPriorityLevel = 0;
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


/* ---------------------------------------------------------------------------------- */

StyleSymbol::StyleSymbol()
{
  mName = SymbolName::SOLID;
  mAngle = 0.;
  mColor = Color(0);
  mOutlineColor = Color(0);
  mScalingFactor = 0.;
  mPriorityLevel = 0;
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

void StyleSymbol::setColor(Color &color) 
{
  mColor = color;
}

Color StyleSymbol::getOutlineColor() 
{
  return mOutlineColor;
}

void StyleSymbol::setOutlineColor(Color &outlinecolor) 
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

/* ---------------------------------------------------------------------------------- */

StyleLabel::StyleLabel()
{
  mFontName = "Arial";
  mFontSize = 12;
  mAngle = 0.;
  mForegroundColor = Color(0);
  mBackgroundColor = Color(0);
  mOutlineColor = Color(0);
  mShadowColor = Color(0);
  mStretch = 100.;
  mLabelPlacement = LabelPlacement::p;
  mAnchorPosition = AnchorPosition::VERTICAL_BASELINE | AnchorPosition::HORIZONTAL_LEFT;
  mPerpendicularOffset = 0;
  bBold = false;
  bItalic = false;
  bUnderline = false;
  bStrikeout = false;
  mPriorityLevel = 0;
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


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL
bool GraphicStyle::readFromOGR(OGRStyleMgr *ogrStyle)
{
  OGRStyleTool *ogrStyleTool = NULL;
  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
    if (ogrStyleTool = ogrStyle->GetPart(i)) {
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

void GraphicStyle::setStylePen(std::shared_ptr<StylePen> stylePen)
{
  mStylePen = stylePen;
}

void GraphicStyle::setStyleBrush(std::shared_ptr<StyleBrush> styleBrush)
{
  mStyleBrush = styleBrush;
}

void GraphicStyle::setStyleSymbol(std::shared_ptr<StyleSymbol> styleSymbol)
{
  mStyleSymbol = styleSymbol;
}

void GraphicStyle::setStyleLabel(std::shared_ptr<StyleLabel> styleLabel)
{
  mStyleLabel = styleLabel;
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
    if (strcmp(cap, "") == 0) penJoin = StylePen::PenJoin::MITER;
    else if (strcmp(cap, "") == 0) penJoin = StylePen::PenJoin::ROUNDED;
    else if (strcmp(cap, "") == 0) penJoin = StylePen::PenJoin::BEVEL;
    stylePen->setPenJoin(penJoin);
  }

  double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
  if ( !bDefault ) {
    stylePen->setPerpendicularOffset(I3D_ROUND_TO_INT(perpendicularOffset));
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

GraphicEntity::~GraphicEntity()
{
}


/* ---------------------------------------------------------------------------------- */

GPoint::GPoint() 
  : Point<float>(), 
    GraphicEntity()
{
}

GPoint::GPoint(float x, float y) 
  : Point<float>(x, y), 
    GraphicEntity()
{
}

GPoint::GPoint(const Point<float> &pt) 
  : Point<float>(pt), 
    GraphicEntity()
{
}  

GPoint::~GPoint()
{}

#ifdef HAVE_OPENCV
void GPoint::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GPoint3D::GPoint3D() 
  : Point3<float>(), 
  GraphicEntity()
{
}

GPoint3D::GPoint3D(float x, float y, float z) 
  : Point3<float>(x, y, z), 
  GraphicEntity()
{
}

GPoint3D::GPoint3D(const Point3<float> &pt) 
  : Point3<float>(pt), 
  GraphicEntity()
{
}

GPoint3D::~GPoint3D()
{
}

#ifdef HAVE_OPENCV
void GPoint3D::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GLineString::GLineString() 
  : LineString<geometry::Point<float>>(), 
    GraphicEntity()
{
}

GLineString::~GLineString()
{
}

#ifdef HAVE_OPENCV
void GLineString::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GPolygon::GPolygon() 
  : Polygon<geometry::Point<float>>(), 
    GraphicEntity()
{
}

GPolygon::~GPolygon()
{
}

#ifdef HAVE_OPENCV
void GPolygon::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GMultiPoint::GMultiPoint()
  : MultiPoint<geometry::Point<float>>(), 
    GraphicEntity()
{
}

GMultiPoint::~GMultiPoint()
{
}

#ifdef HAVE_OPENCV
void GMultiPoint::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GMultiLineString::GMultiLineString()
  : MultiLineString<geometry::Point<float>>(), 
    GraphicEntity()
{
}

GMultiLineString::~GMultiLineString()
{
}

#ifdef HAVE_OPENCV
void GMultiLineString::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

GMultiPolygon::GMultiPolygon()
  : MultiPolygon<geometry::Point<float>>(), 
    GraphicEntity()
{
}

GMultiPolygon::~GMultiPolygon()
{
}

#ifdef HAVE_OPENCV
void GMultiPolygon::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */

const char *GLayer::getName() const
{
  return mName.c_str();
}


void GLayer::setName(const char *name)
{
  mName = name;
}

void GLayer::add(std::shared_ptr<GraphicEntity> entity)
{

}

void GLayer::remove()
{

}

} // Fin namespace graph

} // End namespace I3D
