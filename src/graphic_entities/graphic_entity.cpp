#include "graphic_entities/graphic_entity.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

StylePen::StylePen()
{
  // valores por defecto
  mPenColor = Color(0);
  mPenWidth = 1;
  //mPattern;
  mPenName = PenName::SOLID;
  mPenCap = PenCap::BUTT;
  mPenJoin = PenJoin::BEVEL;
  mPerpendicularOffset = 0;
  mPriorityLevel = 0;
}

StylePen::~StylePen()
{
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