#ifndef TL_GEOSPATIAL_ORTHO_H
#define TL_GEOSPATIAL_ORTHO_H

#include <string>
#include <memory>

#include "tidop/experimental/camera.h"
#include "tidop/experimental/photo.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/geometry/transform/affine.h"

namespace tl
{

class ImageReader;
class ImageWriter;



namespace geospatial
{

class TL_EXPORT Orthorectification
{

public:

	Orthorectification(const std::string &dtm);
	~Orthorectification();

  void run2(const std::vector<experimental::Photo> &photos,
					  const std::string &orthoPath);

	void run(const std::vector<experimental::Photo> &photos,
					 const std::string &orthoPath);

private:

	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits,
														 	          const tl::math::RotationMatrix<double> &rotation_matrix,
															          const Point3D &position,
															          double focal);
	Window<PointD> windowOrthoTerrain(const std::vector<Point3D> &footprint);

	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	experimental::Camera mCamera;
	Affine<PointI> mAffinePhotocoordinatesToImage;
};

} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H