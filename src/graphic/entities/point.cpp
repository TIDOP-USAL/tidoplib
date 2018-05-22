#include "graphic/entities/point.h"

namespace TL
{

using namespace geometry;

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */

GPoint::GPoint() 
  : Point<double>(), 
    GraphicEntity(GraphicEntity::Type::POINT_2D)
{
}

GPoint::GPoint(double x, double y) 
  : Point<double>(x, y), 
    GraphicEntity(GraphicEntity::Type::POINT_2D)
{
}

GPoint::GPoint(const Point<double> &pt) 
  : Point<double>(pt), 
    GraphicEntity(GraphicEntity::Type::POINT_2D)
{
}  

GPoint::GPoint(const GPoint &pt) 
  : Point<double>(pt), 
    GraphicEntity(pt)
{
}  

GPoint::~GPoint()
{}

GPoint &GPoint::operator = (const GPoint &gPoint)
{
  if (this != &gPoint) {
    Point<double>::operator=(gPoint);
    GraphicEntity::operator=(gPoint);
  }
  return *this;
}
#ifdef HAVE_OPENCV
void GPoint::draw(cv::Mat &canvas) const
{

  // TODO: Hay que incluir la transformación terreno-pantalla
  //cv::drawMarker(canvas, ..... );
}
#endif

/* ---------------------------------------------------------------------------------- */

GPoint3D::GPoint3D() 
  : Point3<double>(), 
    GraphicEntity(GraphicEntity::Type::POINT_3D)
{
}

GPoint3D::GPoint3D(double x, double y, double z) 
  : Point3<double>(x, y, z), 
    GraphicEntity(GraphicEntity::Type::POINT_3D)
{
}

GPoint3D::GPoint3D(const Point3<double> &pt) 
  : Point3<double>(pt), 
    GraphicEntity(GraphicEntity::Type::POINT_3D)
{
}

GPoint3D::GPoint3D(const GPoint3D &pt) 
  : Point3<double>(pt), 
    GraphicEntity(pt)
{
}

GPoint3D::~GPoint3D()
{
}

GPoint3D &GPoint3D::operator = (const GPoint3D &gPoint)
{
  if (this != &gPoint) {
    Point3<double>::operator=(gPoint);
    GraphicEntity::operator=(gPoint);
  }
  return *this;
}
#ifdef HAVE_OPENCV
void GPoint3D::draw(cv::Mat &canvas) const
{

}
#endif

/* ---------------------------------------------------------------------------------- */


GMultiPoint::GMultiPoint()
  : MultiPoint<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::MULTIPOINT_2D)
{
}

GMultiPoint::GMultiPoint(const MultiPoint<Point<double>> &multiPoint)
  : MultiPoint<Point<double>>(multiPoint), 
    GraphicEntity(GraphicEntity::Type::MULTIPOINT_2D)
{
}

GMultiPoint::GMultiPoint(const GMultiPoint &gMultiPoint)
  : MultiPoint<Point<double>>(gMultiPoint), 
    GraphicEntity(gMultiPoint)
{
}

GMultiPoint::~GMultiPoint()
{
}

GMultiPoint &GMultiPoint::operator = (const GMultiPoint &gMultiPoint)
{
  if (this != &gMultiPoint) {
    MultiPoint<Point<double>>::operator=(gMultiPoint);
    GraphicEntity::operator=(gMultiPoint);
  }
  return *this;
}

#ifdef HAVE_OPENCV
void GMultiPoint::draw(cv::Mat &canvas) const
{

}
#endif

} // Fin namespace graph

} // End namespace TL
