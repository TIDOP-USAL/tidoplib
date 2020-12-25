#include "tidop/graphic/entities/polygon.h"

namespace tl
{

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */


GPolygon::GPolygon() 
  : Polygon<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(const Polygon<Point<double>> &polygon) 
  : Polygon<Point<double>>(polygon), 
    GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(const GPolygon &gPolygon) 
  : Polygon<Point<double>>(gPolygon), 
    GraphicEntity(gPolygon)
{
}

GPolygon::GPolygon(GPolygon &&gPolygon) TL_NOEXCEPT
  : Polygon<Point<double>>(std::forward<Polygon<Point<double>>>(gPolygon)),
    GraphicEntity(std::forward<GraphicEntity>(gPolygon))
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

GPolygon &GPolygon::operator = (GPolygon &&gPolygon) TL_NOEXCEPT
{
  if (this != &gPolygon) {
    Polygon<Point<double>>::operator=(std::forward<Polygon<Point<double>>>(gPolygon));
    GraphicEntity::operator=(std::forward<GraphicEntity>(gPolygon));
  }
  return *this;
}

bool GPolygon::isMultiEntity() const
{
  return false;
}

bool GPolygon::isSimpleEntity() const
{
  return true;
}


/* ---------------------------------------------------------------------------------- */


GPolygon3D::GPolygon3D() 
  : Polygon3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const Polygon3D<Point3<double>> &polygon)
  : Polygon3D<Point3<double>>(polygon),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const GPolygon3D &gPolygon3D) 
  : Polygon3D<Point3<double>>(gPolygon3D),
    GraphicEntity(gPolygon3D)
{
}

GPolygon3D::GPolygon3D(GPolygon3D &&gPolygon3D) TL_NOEXCEPT
  : Polygon3D<Point3<double>>(std::forward<Polygon3D<Point3<double>>>(gPolygon3D)),
    GraphicEntity(std::forward<GraphicEntity>(gPolygon3D))
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

GPolygon3D &GPolygon3D::operator = (GPolygon3D &&gPolygon3D) TL_NOEXCEPT
{
  if (this != &gPolygon3D) {
    Polygon3D<Point3<double>>::operator=(std::forward<Polygon3D<Point3<double>>>(gPolygon3D));
    GraphicEntity::operator=(std::forward<GraphicEntity>(gPolygon3D));
  }
  return *this;
}

bool GPolygon3D::isMultiEntity() const
{
  return false;
}

bool GPolygon3D::isSimpleEntity() const
{
  return true;
}


/* ---------------------------------------------------------------------------------- */

GMultiPolygon::GMultiPolygon()
  : MultiPolygon<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(const MultiPolygon<Point<double>> &multiPolygon)
  : MultiPolygon<Point<double>>(multiPolygon), 
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
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

bool GMultiPolygon::isMultiEntity() const
{
  return true;
}

bool GMultiPolygon::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */


GMultiPolygon3D::GMultiPolygon3D()
  : MultiPolygon3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(const MultiPolygon3D<Point3<double>> &multiPolygon)
  : MultiPolygon3D<Point3<double>>(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
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

bool GMultiPolygon3D::isMultiEntity() const
{
  return true;
}

bool GMultiPolygon3D::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */



} // Fin namespace graph

} // End namespace TL
