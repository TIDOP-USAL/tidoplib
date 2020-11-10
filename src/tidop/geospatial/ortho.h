#ifndef TL_GEOSPATIAL_ORTHO_H
#define TL_GEOSPATIAL_ORTHO_H

#include <string>
#include <memory>

#include "tidop/experimental/camera.h"
#include "tidop/experimental/photo.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace tl
{

class ImageReader;
class ImageWriter;



namespace geospatial
{

class TL_EXPORT Orthorectification
{

public:

	Orthorectification(const std::string &dtm, 
										 const experimental::Camera &camera);
	~Orthorectification();

	void run(const std::string &file, 
					 const experimental::Photo::Orientation &orientation,
					 const std::string &rectifiedFile);
private:

	std::vector<PointI> imageLimits(int rows, int cols);
	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits,
														 	          const tl::math::RotationMatrix<double> &rotation_matrix,
															          const Point3D &principal_point,
															          double focal);
	Point3D projectImageCoordToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix,
                                     const Point3D &principal_point,
                                     const PointD &coordinates_image,
                                     double focal);
	PointD photocoordinates(const tl::math::RotationMatrix<double> &rotation_matrix,
													const Point3D &principal_point,
													const Point3D &coordinates_terrain,
													double focal);
private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	experimental::Camera mCamera;
};

} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H