/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/graphic/styles.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_core.h"
#include "ogr_featurestyle.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL


namespace tl
{

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */

StylePen::StylePen()
  : mPenColor(0),
    mPenWidth(1),
    mPattern(""),
    mPenName(Name::solid),
    mPenCap(Cap::butt),
    mPenJoin(Join::bevel),
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

Color StylePen::color() const
{
  return mPenColor;
}

void StylePen::setColor(const Color &pencolor)
{
  mPenColor = pencolor;
}

uint8_t StylePen::width() const
{
  return mPenWidth;
}

void StylePen::setWidth(uint8_t penwidth) 
{
  mPenWidth = penwidth;
}

std::string StylePen::pattern() const
{
  return mPattern;
}

void StylePen::setPattern(std::string pattern) 
{
  mPattern = pattern;
}

StylePen::Name StylePen::name() const
{
  return mPenName;
}

void StylePen::setName(Name name) 
{
  mPenName = name;
}

StylePen::Cap StylePen::cap() const
{
  return mPenCap;
}

void StylePen::setCap(Cap cap) 
{
  mPenCap = cap;
}

StylePen::Join StylePen::join() const
{
  return mPenJoin;
}

void StylePen::setJoin(Join join) 
{
  mPenJoin = join;
}

int32_t StylePen::perpendicularOffset() const
{
  return mPerpendicularOffset;
}

void StylePen::setPerpendicularOffset(int32_t perpendicularoffset)
{
  mPerpendicularOffset = perpendicularoffset;
}

uint32_t StylePen::priorityLevel() const
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
    mBrushName(Name::solid),
    mAngle(0.),
    mScalingFactor(1.),
    mPriorityLevel(0)
{
  mSpacing[0] = mSpacing[1] = 0.;
}

StyleBrush::StyleBrush(const StyleBrush &styleBrush)
  : mForeColor(styleBrush.mForeColor),
    mBackColor(styleBrush.mBackColor),
    mBrushName(styleBrush.mBrushName),
    mAngle(styleBrush.mAngle),
    mScalingFactor(styleBrush.mScalingFactor),
    mSpacing(styleBrush.mSpacing),
    mPriorityLevel(styleBrush.mPriorityLevel)
{
}

StyleBrush::~StyleBrush()
{
}

Color StyleBrush::foreColor() const
{
  return mForeColor;
}

void StyleBrush::setForeColor(Color forecolor)
{
  mForeColor = forecolor;
}

Color StyleBrush::backColor() const
{
  return mBackColor;
}

void StyleBrush::setBackColor(Color backcolor)
{ 
  mBackColor = backcolor;
}

StyleBrush::Name StyleBrush::brushName() const
{
  return mBrushName;
}

void StyleBrush::setBrushName(Name brushname)
{
  mBrushName = brushname;
}

double StyleBrush::angle() const
{
  return mAngle;
}

void StyleBrush::setAngle(double angle)
{
  mAngle = angle;
}

double StyleBrush::scalingFactor() const
{
  return mScalingFactor;
}

void StyleBrush::setScalingFactor(double scalingFactor) 
{
  mScalingFactor = scalingFactor;
}

double StyleBrush::spacingX() const
{
  return mSpacing[0];
}

double StyleBrush::spacingY() const
{
  return mSpacing[1];
}

void StyleBrush::setSpacing(double spacingX, double spacingY ) 
{
  mSpacing[0] = spacingX;
  mSpacing[1] = spacingY;
}

uint32_t StyleBrush::priorityLevel() const
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
    mSpacing = styleBrush.mSpacing;
    mPriorityLevel = styleBrush.mPriorityLevel;
  }
  return *this;
}


/* ---------------------------------------------------------------------------------- */


StyleSymbol::StyleSymbol()
  : mName(Name::cross),
    mAngle(0.),
    mColor(0),
    mOutlineColor(0),
    mScalingFactor(1.),
    mPriorityLevel(0)
{
  mOffset[0] = mOffset[1] = 0.;
}

StyleSymbol::StyleSymbol(const StyleSymbol &styleSymbol)
  : mName(styleSymbol.mName),
    mAngle(styleSymbol.mAngle),
    mColor(styleSymbol.mColor),
    mOutlineColor(styleSymbol.mOutlineColor),
    mScalingFactor(styleSymbol.mScalingFactor),
    mOffset(styleSymbol.mOffset),
    mPriorityLevel(styleSymbol.mPriorityLevel)
{
}

StyleSymbol::~StyleSymbol()
{
}

double StyleSymbol::angle() const
{
  return mAngle;
}

void StyleSymbol::setAngle(double angle) 
{
  mAngle = angle;
}

StyleSymbol::Name StyleSymbol::name() const
{
  return mName;
}

void StyleSymbol::setName(Name name)
{
  mName = name;
}

Color StyleSymbol::color() const
{
  return mColor;
}

void StyleSymbol::setColor(const Color &color)
{
  mColor = color;
}

Color StyleSymbol::outlineColor() const
{
  return mOutlineColor;
}

void StyleSymbol::setOutlineColor(const Color &outlinecolor)
{
  mOutlineColor = outlinecolor;
}

double StyleSymbol::scalingFactor() const
{
  return mScalingFactor;
}

void StyleSymbol::setScalingFactor(double scalingFactor)
{
  mScalingFactor = scalingFactor;
}

double StyleSymbol::offsetX() const
{
  return mOffset[0];
}

double StyleSymbol::offsetY() const
{
  return mOffset[1];
}

void StyleSymbol::setOffset(double dx, double dy) 
{
  mOffset[0] = dx;
  mOffset[1] = dy;
}

uint32_t StyleSymbol::priorityLevel() const
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
    mOffset = styleSymbol.mOffset;
    mPriorityLevel = styleSymbol.mPriorityLevel;
  }
  return *this;
}


/* ---------------------------------------------------------------------------------- */


StyleLabel::StyleLabel()
  : mFont(),
    mText(""),
    mAngle(0.),
    mForegroundColor(0),
    mBackgroundColor(0),
    mOutlineColor(0),
    mShadowColor(0),
    mStretch(100.),
    mLabelPlacement(LabelPlacement::p),
    mAnchorPosition(AnchorPosition::vertical_baseline | AnchorPosition::horizontal_left),
    mPerpendicularOffset(0),
    bStrikeout(false),
    mPriorityLevel(0)
{
  mOffset[0] = mOffset[1] = 0.;
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
    mOffset(styleLabel.mOffset),
    mPerpendicularOffset(styleLabel.mPerpendicularOffset),
    bStrikeout(styleLabel.bStrikeout),
    mPriorityLevel(styleLabel.mPriorityLevel)
{
}

StyleLabel::~StyleLabel()
{
}

std::string StyleLabel::text() const
{
  return mText;
}

void StyleLabel::setText(const std::string &text)
{
  mText = text;
}

double StyleLabel::angle() const
{
  return mAngle;
}

void StyleLabel::setAngle(double angle) 
{
  mAngle = angle;
}

Color StyleLabel::foregroundColor() const
{
  return mForegroundColor;
}

void StyleLabel::setForegroundColor(const Color &color)
{
  mForegroundColor = color;
}

Color StyleLabel::backgroundColor() const
{
  return mBackgroundColor;
}

void StyleLabel::setBackgroundColor(const Color &color)
{
  mBackgroundColor = color;
}

Color StyleLabel::outlineColor() const
{
  return mOutlineColor;
}

void StyleLabel::setOutlineColor(const Color &color)
{
  mOutlineColor = color;
}

Color StyleLabel::shadowColor() const
{
  return mShadowColor;
}

void StyleLabel::setShadowColor(const Color &color)
{
  mShadowColor = color;
}

double StyleLabel::stretch() const
{
  return mStretch;
}

void StyleLabel::setStretch(double stretch)
{
  mStretch = stretch;
}

StyleLabel::LabelPlacement StyleLabel::labelPlacement() const
{
  return mLabelPlacement;
}

void StyleLabel::setLabelPlacement(LabelPlacement labelPlacement)
{
  mLabelPlacement = labelPlacement;
}

StyleLabel::AnchorPosition StyleLabel::anchorPosition() const
{
  return mAnchorPosition;
}

void StyleLabel::setAnchorPosition(AnchorPosition anchorPosition)
{
  mAnchorPosition = anchorPosition;
}

double StyleLabel::offsetX() const
{
  return mOffset[0];
}

double StyleLabel::offsetY() const
{
  return mOffset[1];
}

void StyleLabel::setOffset(double dx, double dy) 
{
  mOffset[0] = dx; 
  mOffset[1] = dy;
}

int StyleLabel::perpendicularOffset() const
{
  return mPerpendicularOffset;
}

void StyleLabel::setPerpendicularOffset(int perpendicularOffset)
{
  mPerpendicularOffset = perpendicularOffset;
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

Font StyleLabel::font() const
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

//#ifdef HAVE_GDAL
//bool GraphicStyle::readFromOGR(OGRStyleMgr *ogrStyle)
//{
//  OGRStyleTool *ogrStyleTool = nullptr;
//  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
//    if ((ogrStyleTool = ogrStyle->GetPart(i)) != nullptr) {
//      OGRSTClassId oci = ogrStyleTool->GetType();
//      switch (oci) {
//      case OGRSTCPen:
//        readStylePen(dynamic_cast<OGRStylePen *>(ogrStyleTool));
//        break;
//      case OGRSTCBrush:
//        readStyleBrush(dynamic_cast<OGRStyleBrush *>(ogrStyleTool));
//        break;
//      case OGRSTCSymbol:
//        readStyleSymbol(dynamic_cast<OGRStyleSymbol *>(ogrStyleTool));
//        break;
//      case OGRSTCLabel:
//        readStyleLabel(dynamic_cast<OGRStyleLabel *>(ogrStyleTool));
//        break;
//      case OGRSTCVector:
//
//        break;
//      default:
//        break;
//      }
//    }
//  }
//  return false;
//}
//#endif 
//
//bool GraphicStyle::write()
//{
//  return false;
//}

StylePen *GraphicStyle::stylePen() const
{
  return mStylePen.get();
}

void GraphicStyle::setStylePen(const std::shared_ptr<StylePen> &stylePen)
{
  mStylePen = stylePen;
}

StyleBrush *GraphicStyle::styleBrush() const
{
  return mStyleBrush.get();
}

void GraphicStyle::setStyleBrush(const std::shared_ptr<StyleBrush> &styleBrush)
{
  mStyleBrush = styleBrush;
}

StyleSymbol *GraphicStyle::styleSymbol() const
{
  return mStyleSymbol.get();
}

void GraphicStyle::setStyleSymbol(const std::shared_ptr<StyleSymbol> &styleSymbol)
{
  mStyleSymbol = styleSymbol;
}

StyleLabel *GraphicStyle::styleLabel() const
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

//#ifdef HAVE_GDAL
//
//void GraphicStyle::readStylePen(OGRStylePen *ogrStylePen)
//{
//  GBool bDefault = false;
//  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();
//  const char *hexColor = ogrStylePen->Color(bDefault);
//  if (!bDefault) stylePen->setPenColor(Color(hexColor));
//
//  double width = ogrStylePen->Width(bDefault);
//  if ( !bDefault ) { 
//    //unsigned __int8 w = (unsigned __int8)GetWidthPx( wd, poStylePen->GetUnit() );
//    //gs->AddStyle( gstPenWidth, w );
//    //stylePen->setPenWidth();
//  }
//
//  const char *pattern = ogrStylePen->Pattern(bDefault);
//  if (!bDefault) stylePen->setPattern(pattern);
//
//  //const char *name = ogrStylePen->Id(bDefault);
//  //if ( !bDefault ) {
//  //  stylePen->setPenName()
//  //}
//
//  const char *cap = ogrStylePen->Cap(bDefault);
//  if ( !bDefault ) {
//    StylePen::PenCap penCap;
//    if (strcmp(cap, "") == 0) penCap = StylePen::PenCap::BUTT;
//    else if (strcmp(cap, "") == 0) penCap = StylePen::PenCap::PROJECTING;
//    else if (strcmp(cap, "") == 0) penCap = StylePen::PenCap::ROUND;
//    stylePen->setPenCap(penCap);
//  }
//
//  const char *join = ogrStylePen->Join(bDefault);
//  if ( !bDefault ) {
//    StylePen::PenJoin penJoin;
//    if (strcmp(join, "") == 0) penJoin = StylePen::PenJoin::MITER;
//    else if (strcmp(join, "") == 0) penJoin = StylePen::PenJoin::ROUNDED;
//    else if (strcmp(join, "") == 0) penJoin = StylePen::PenJoin::BEVEL;
//    stylePen->setPenJoin(penJoin);
//  }
//
//  double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
//  if ( !bDefault ) {
//    stylePen->setPerpendicularOffset(TL_ROUND_TO_INT(perpendicularOffset));
//  }
//
//  setStylePen(stylePen);
//}
//
//void GraphicStyle::readStyleBrush(OGRStyleBrush *ogrStyleBrush)
//{
//
//}
//
//void GraphicStyle::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol)
//{
//
//}
//
//void GraphicStyle::readStyleLabel(OGRStyleLabel *ogrStyleLabel)
//{
//
//}
//
//#endif


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace TL
