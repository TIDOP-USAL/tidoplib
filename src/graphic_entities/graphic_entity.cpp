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
}

StyleBrush::~StyleBrush()
{
}


/* ---------------------------------------------------------------------------------- */

StyleSymbol::StyleSymbol()
{
}

StyleSymbol::~StyleSymbol()
{
}

/* ---------------------------------------------------------------------------------- */

StyleLabel::StyleLabel()
{
}

StyleLabel::~StyleLabel()
{
}

/* ---------------------------------------------------------------------------------- */

GraphicEntity::GraphicEntity()
{
}

GraphicEntity::~GraphicEntity()
{
}

} // End namespace I3D