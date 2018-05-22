#include "graphic/entities/polygon.h"

namespace TL
{

using namespace geometry;

namespace graph
{ 

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

#ifdef HAVE_OPENCV
void GPolygon::draw(cv::Mat &canvas) const
{

}
#endif

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
#ifdef HAVE_OPENCV
void GMultiPolygon::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */



} // Fin namespace graph

} // End namespace TL
