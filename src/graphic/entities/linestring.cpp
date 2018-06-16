#include "graphic/entities/linestring.h"

namespace TL
{

using namespace geometry;

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */

GLineString::GLineString() 
  : LineString<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::LINESTRING_2D)
{
}

GLineString::GLineString(const LineString<Point<double>> &lineString) 
  : LineString<Point<double>>(lineString), 
    GraphicEntity(GraphicEntity::Type::LINESTRING_2D)
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

bool GLineString::isMultiEntity() const
{
  return true;
}

bool GLineString::isSimpleEntity() const
{
  return false;
}


//#ifdef HAVE_OPENCV
//void GLineString::draw(cv::Mat &canvas) const
//{

//}
//#endif

/* ---------------------------------------------------------------------------------- */

GLineString3D::GLineString3D() 
  : LineString3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::LINESTRING_3D)
{
}

GLineString3D::GLineString3D(const LineString3D<Point3<double>> &lineString)
  : LineString3D<Point3<double>>(lineString),
    GraphicEntity(GraphicEntity::Type::LINESTRING_3D)
{
}

GLineString3D::GLineString3D(const GLineString3D &lineString) 
  : LineString3D<Point3<double>>(lineString),
    GraphicEntity(lineString)
{
}

GLineString3D::~GLineString3D()
{
}

GLineString3D &GLineString3D::operator = (const GLineString3D &gLineString3D)
{
  if (this != &gLineString3D) {
    LineString3D<Point3<double>>::operator=(gLineString3D);
    GraphicEntity::operator=(gLineString3D);
  }
  return *this;
}

bool GLineString3D::isMultiEntity() const
{
  return false;
}

bool GLineString3D::isSimpleEntity() const
{
  return true;
}

//#ifdef HAVE_OPENCV
//void GLineString3D::draw(cv::Mat &canvas) const
//{

//}
//#endif


/* ---------------------------------------------------------------------------------- */

GMultiLineString::GMultiLineString()
  : MultiLineString<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::MULTILINE_2D)
{
}

GMultiLineString::GMultiLineString(const MultiLineString<geometry::Point<double>> &multiLineString)
  : MultiLineString<Point<double>>(multiLineString), 
    GraphicEntity(GraphicEntity::Type::MULTILINE_2D)
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

bool GMultiLineString::isMultiEntity() const
{
  return true;
}

bool GMultiLineString::isSimpleEntity() const
{
  return false;
}

//#ifdef HAVE_OPENCV
//void GMultiLineString::draw(cv::Mat &canvas) const
//{

//}
//#endif


/* ---------------------------------------------------------------------------------- */

GMultiLineString3D::GMultiLineString3D()
  : MultiLineString3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::MULTILINE_3D)
{
}

GMultiLineString3D::GMultiLineString3D(const MultiLineString3D<geometry::Point3<double>> &multiLineString)
  : MultiLineString3D<Point3<double>>(multiLineString),
    GraphicEntity(GraphicEntity::Type::MULTILINE_3D)
{
}

GMultiLineString3D::GMultiLineString3D(const GMultiLineString3D &gMultiLineString3D)
  : MultiLineString3D<Point3<double>>(gMultiLineString3D),
    GraphicEntity(gMultiLineString3D)
{
}

GMultiLineString3D::~GMultiLineString3D()
{
}

GMultiLineString3D &GMultiLineString3D::operator = (const GMultiLineString3D &gMultiLineString3D)
{
  if (this != &gMultiLineString3D) {
    MultiLineString3D<Point3<double>>::operator=(gMultiLineString3D);
    GraphicEntity::operator=(gMultiLineString3D);
  }
  return *this;
}

bool GMultiLineString3D::isMultiEntity() const
{
  return true;
}

bool GMultiLineString3D::isSimpleEntity() const
{
  return false;
}

//#ifdef HAVE_OPENCV
//void GMultiLineString3D::draw(cv::Mat &canvas) const
//{

//}
//#endif

/* ---------------------------------------------------------------------------------- */

} // Fin namespace graph

} // End namespace TL
