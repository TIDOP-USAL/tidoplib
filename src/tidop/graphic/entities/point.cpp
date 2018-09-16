#include "tidop/graphic/entities/point.h"

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

bool GPoint::isMultiEntity() const
{
  return false;
}

bool GPoint::isSimpleEntity() const
{
  return true;
}

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

bool GPoint3D::isMultiEntity() const
{
  return false;
}

bool GPoint3D::isSimpleEntity() const
{
  return true;
}

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

bool GMultiPoint::isMultiEntity() const
{
  return true;
}

bool GMultiPoint::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */


GMultiPoint3D::GMultiPoint3D()
  : MultiPoint3D<Point3<double>>(), 
    GraphicEntity(GraphicEntity::Type::MULTIPOINT_3D)
{
}

GMultiPoint3D::GMultiPoint3D(const MultiPoint3D<Point3<double>> &multiPoint)
  : MultiPoint3D<Point3<double>>(multiPoint), 
    GraphicEntity(GraphicEntity::Type::MULTIPOINT_3D)
{
}

GMultiPoint3D::GMultiPoint3D(const GMultiPoint3D &gMultiPoint3D)
  : MultiPoint3D<Point3<double>>(gMultiPoint3D), 
    GraphicEntity(gMultiPoint3D)
{
}

GMultiPoint3D::~GMultiPoint3D()
{
}

GMultiPoint3D &GMultiPoint3D::operator = (const GMultiPoint3D &gMultiPoint3D)
{
  if (this != &gMultiPoint3D) {
	MultiPoint3D<Point3<double>>::operator=(gMultiPoint3D);
    GraphicEntity::operator=(gMultiPoint3D);
  }
  return *this;
}

bool GMultiPoint3D::isMultiEntity() const
{
  return true;
}

bool GMultiPoint3D::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace TL
