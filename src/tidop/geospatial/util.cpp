#include "crs.h"
#include "util.h"

namespace tl
{

namespace geospatial
{

int utmZoneFromLongitude(double longitude)
{
  int zone = static_cast<int>(ceil((longitude + TL_PI) / (6.0 * TL_DEG_TO_RAD)));
  return zone;
}

Point3D projectPhotoToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix, 
                              const Point3D &principal_point, 
                              const PointD &coordinates_image, 
                              double focal,
                              double z)
{
  Point3D point;

  point.z = z;

  double x = coordinates_image.x;
  double y = coordinates_image.y;
  double z_dif = point.z - principal_point.z;

  TL_TODO("revisar el orden de la matriz de rotación")

  double div = rotation_matrix.at(0, 2) * x + 
               rotation_matrix.at(1, 2) * y - 
               rotation_matrix.at(2, 2) * -focal;

  point.x = principal_point.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
                                         rotation_matrix.at(1, 0) * coordinates_image.y - 
                                         rotation_matrix.at(2, 0) * -focal) / div;

  point.y = principal_point.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
                                         rotation_matrix.at(1, 1) * coordinates_image.y - 
                                         rotation_matrix.at(2, 1) * -focal) / div;

  return point;
}

PointD projectTerrainToPhoto(const tl::math::RotationMatrix<double> &rotation_matrix,
                             const Point3D &principal_point, 
                             const Point3D &coordinates_terrain, 
                             double focal)
{
  double dx = coordinates_terrain.x - principal_point.x;
  double dy = coordinates_terrain.y - principal_point.y;
  double dz = coordinates_terrain.z - principal_point.z;
  double div = rotation_matrix.at(2, 0) * dx + 
               rotation_matrix.at(2, 1) * dy + 
               rotation_matrix.at(2, 2) * (coordinates_terrain.z - principal_point.z);

  PointD photocoord;
  photocoord.x = -focal * (rotation_matrix.at(0, 0) * dx + 
                           rotation_matrix.at(0, 1) * dy + 
                           rotation_matrix.at(0, 2) * dz) / div;
  photocoord.y = -focal * (rotation_matrix.at(1, 0) * dx + 
                           rotation_matrix.at(1, 1) * dy + 
                           rotation_matrix.at(1, 2) * dz) / div;
  return photocoord;
}


#if defined HAVE_GDAL && defined HAVE_PROJ4



#endif // HAVE_GDAL


} // End namespace  geospatial

} // End namespace tl
