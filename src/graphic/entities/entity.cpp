#include "graphic/entities/entity.h"

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


namespace TL
{

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */


GraphicEntity::GraphicEntity(Type type)
  : GraphicStyle(),
    GData(),
    mEntityType(type)
{
}

GraphicEntity::GraphicEntity(const GraphicEntity &graphicEntity)
  : GraphicStyle(graphicEntity),
    GData(graphicEntity),
    mEntityType(graphicEntity.mEntityType)
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
    mEntityType = graphicEntity.mEntityType;
  }
  return *this;
}





} // Fin namespace graph

} // End namespace TL
