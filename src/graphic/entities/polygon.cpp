#include "graphic/entities/polygon.h"

namespace TL
{

using namespace geometry;

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */


GPolygon::GPolygon() 
  : Polygon<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::POLYGON_2D)
{
}

GPolygon::GPolygon(const Polygon<Point<double>> &polygon) 
  : Polygon<Point<double>>(polygon), 
    GraphicEntity(GraphicEntity::Type::POLYGON_2D)
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

//#ifdef HAVE_OPENCV
//void GPolygon::draw(cv::Mat &canvas) const
//{

//}
//#endif


/* ---------------------------------------------------------------------------------- */


GPolygon3D::GPolygon3D() 
  : Polygon3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::POLYGON_3D)
{
}

GPolygon3D::GPolygon3D(const Polygon3D<Point3<double>> &polygon)
  : Polygon3D<Point3<double>>(polygon),
    GraphicEntity(GraphicEntity::Type::POLYGON_3D)
{
}

GPolygon3D::GPolygon3D(const GPolygon3D &gPolygon3D) 
  : Polygon3D<Point3<double>>(gPolygon3D),
    GraphicEntity(gPolygon3D)
{
}

GPolygon3D::~GPolygon3D()
{
}

GPolygon3D &GPolygon3D::operator = (const GPolygon3D &gPolygon3D)
{
  if (this != &gPolygon3D) {
    Polygon3D<Point3<double>>::operator=(gPolygon3D);
    GraphicEntity::operator=(gPolygon3D);
  }
  return *this;
}

//#ifdef HAVE_OPENCV
//void GPolygon3D::draw(cv::Mat &canvas) const
//{

//}
//#endif


/* ---------------------------------------------------------------------------------- */

GMultiPolygon::GMultiPolygon()
  : MultiPolygon<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::MULTIPOLYGON_2D)
{
}

GMultiPolygon::GMultiPolygon(const MultiPolygon<Point<double>> &multiPolygon)
  : MultiPolygon<Point<double>>(multiPolygon), 
    GraphicEntity(GraphicEntity::Type::MULTIPOLYGON_2D)
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
//#ifdef HAVE_OPENCV
//void GMultiPolygon::draw(cv::Mat &canvas) const
//{

//}
//#endif


/* ---------------------------------------------------------------------------------- */


GMultiPolygon3D::GMultiPolygon3D()
  : MultiPolygon3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::MULTIPOLYGON_3D)
{
}

GMultiPolygon3D::GMultiPolygon3D(const MultiPolygon3D<Point3<double>> &multiPolygon)
  : MultiPolygon3D<Point3<double>>(multiPolygon),
    GraphicEntity(GraphicEntity::Type::MULTIPOLYGON_3D)
{
}

GMultiPolygon3D::GMultiPolygon3D(const GMultiPolygon3D &gMultiPolygon3D)
  : MultiPolygon3D<Point3<double>>(gMultiPolygon3D),
    GraphicEntity(gMultiPolygon3D)
{
}

GMultiPolygon3D::~GMultiPolygon3D()
{
}

GMultiPolygon3D &GMultiPolygon3D::operator = (const GMultiPolygon3D &gMultiPolygon3D)
{
  if (this != &gMultiPolygon3D) {
    MultiPolygon3D<Point3<double>>::operator=(gMultiPolygon3D);
    GraphicEntity::operator=(gMultiPolygon3D);
  }
  return *this;
}
//#ifdef HAVE_OPENCV
//void GMultiPolygon3D::draw(cv::Mat &canvas) const
//{

//}
//#endif


/* ---------------------------------------------------------------------------------- */



} // Fin namespace graph

} // End namespace TL
