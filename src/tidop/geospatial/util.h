#ifndef TL_GEOSPATIAL_UTIL_H
#define TL_GEOSPATIAL_UTIL_H

#include "tidop/core/defs.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogr_spatialref.h"
TL_DEFAULT_WARNINGS
#endif

#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace tl
{

namespace geospatial
{

TL_EXPORT int utmZoneFromLongitude(double longitude);

Point3D projectPhotoToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix,
                              const Point3D &camera_position,
                              const PointD &coordinates_image,
                              double focal,
                              double z);

PointD projectTerrainToPhoto(const tl::math::RotationMatrix<double> &rotation_matrix,
                             const Point3D &camera_position,
                             const Point3D &coordinates_terrain,
                             double focal);

} // End namespace geospatial

} // End namespace tl


#endif // TL_GEOSPATIAL_UTIL_H
