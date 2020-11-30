#include "crs.h"
#include "util.h"

namespace tl
{

namespace geospatial
{


int utmZoneFromLongitude(double longitude)
{
  TL_TODO("Esto es el calculo del huso correspondiente a la longitud aunque no tiene porque coincidir con el huso real ya que hay zonas que usan husos extendidos")
  int zone = static_cast<int>(ceil((longitude + 180.) / 6.0));
  if (zone == 0) zone = 1;
  return zone;
}

Point3D projectPhotoToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix, 
                              const Point3D &camera_position, 
                              const PointD &coordinates_image, 
                              double focal,
                              double z)
{
  Point3D point;

  point.z = z;

  double x = coordinates_image.x;
  double y = coordinates_image.y;
  double z_dif = point.z - camera_position.z;

  double div = rotation_matrix.at(0, 2) * x + 
               rotation_matrix.at(1, 2) * y - 
               rotation_matrix.at(2, 2) * focal;

  point.x = camera_position.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
                                         rotation_matrix.at(1, 0) * coordinates_image.y - 
                                         rotation_matrix.at(2, 0) * focal) / div;

  point.y = camera_position.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
                                         rotation_matrix.at(1, 1) * coordinates_image.y - 
                                         rotation_matrix.at(2, 1) * focal) / div;

  return point;
}

PointD projectTerrainToPhoto(const tl::math::RotationMatrix<double> &rotation_matrix,
                             const Point3D &camera_position, 
                             const Point3D &coordinates_terrain, 
                             double focal)
{
  double dx = coordinates_terrain.x - camera_position.x;
  double dy = coordinates_terrain.y - camera_position.y;
  double dz = coordinates_terrain.z - camera_position.z;
  double div = rotation_matrix.at(2, 0) * dx + 
               rotation_matrix.at(2, 1) * dy + 
               rotation_matrix.at(2, 2) * (coordinates_terrain.z - camera_position.z);

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
