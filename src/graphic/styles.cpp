#include "graphic/styles.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_core.h"
#include "ogr_featurestyle.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL


namespace TL
{

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
    bStrikeout(false),
    mPriorityLevel(0)
{
}

StyleLabel::StyleLabel(const StyleLabel &styleLabel)
  : mFont(styleLabel.mFont),
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

StylePen *GraphicStyle::getStylePen() const
{
  return mStylePen.get();
}

void GraphicStyle::setStylePen(const std::shared_ptr<StylePen> &stylePen)
{
  mStylePen = stylePen;
}

StyleBrush *GraphicStyle::getStyleBrush() const
{
  return mStyleBrush.get();
}

void GraphicStyle::setStyleBrush(const std::shared_ptr<StyleBrush> &styleBrush)
{
  mStyleBrush = styleBrush;
}

StyleSymbol *GraphicStyle::getStyleSymbol() const
{
  return mStyleSymbol.get();
}

void GraphicStyle::setStyleSymbol(const std::shared_ptr<StyleSymbol> &styleSymbol)
{
  mStyleSymbol = styleSymbol;
}

StyleLabel *GraphicStyle::getStyleLabel() const
{
  return mStyleLabel.get();
}

void GraphicStyle::setStyleLabel(const std::shared_ptr<StyleLabel> &styleLabel)
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


} // Fin namespace graph

} // End namespace TL
