#include "graphic_entities/graphic_entity.h"

namespace I3D
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

GraphicEntity::GraphicEntity()
{
}

GraphicEntity::~GraphicEntity()
{
}

} // End namespace I3D