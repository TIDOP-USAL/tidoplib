#include "tidop/vect/formats.h"

#include "tidop/vect/vectio.h"




namespace tl
{

/* --------------------------------------------------------------------------- */


VectorOptions::VectorOptions(Format format) 
  : FileOptions(),
    mFormat(format)
{}

VectorOptions::~VectorOptions()
{}

VectorOptions::Format VectorOptions::getFormat()
{
  return mFormat;
}


/* --------------------------------------------------------------------------- */


ShapeOptions::ShapeOptions() 
  : VectorOptions(VectorOptions::Format::SHP),
    mEncoding(""),
    mDbfDateLastUpdate(""),
    bAdjustType(false),
    mAdjustGeomType(ADJUST_GEOM_TYPE::FIRST_SHAPE),
    bAutoRepack(true),
    bDbfEofChar(true)
{
}

ShapeOptions::~ShapeOptions()
{}

const char *ShapeOptions::getOptions()
{
  return nullptr;
}

void ShapeOptions::enableAdjustType(bool value)
{
#if  GDAL_VERSION_MAJOR >= 2
  bAdjustType = value;
#endif
}

void ShapeOptions::enableAutoRepac(bool value)
{
  bAutoRepack = value;
}

void ShapeOptions::enableDbfEofChar(bool value)
{
  bDbfEofChar = value;
}


std::string ShapeOptions::getEncoding() const
{
  return mEncoding;
}

void ShapeOptions::setEncoding(const std::string &encoding)
{
  mEncoding = encoding;
}

std::string ShapeOptions::getDbfDateLastUpdate() const
{
  return mDbfDateLastUpdate;
}

void ShapeOptions::setDbfDateLastUpdate(const std::string &date)
{
  mDbfDateLastUpdate = date;
}

ShapeOptions::ADJUST_GEOM_TYPE ShapeOptions::getAdjustGeomType() const
{
  return mAdjustGeomType;
}

void ShapeOptions::setAdjustGeomType(ADJUST_GEOM_TYPE type)
{
  mAdjustGeomType = type;
}

}
