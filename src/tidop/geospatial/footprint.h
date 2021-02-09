#ifndef TL_GEOSPATIAL_FOOTPRINT_H
#define TL_GEOSPATIAL_FOOTPRINT_H

#include <string>
#include <memory>

#include "tidop/experimental/camera.h"
#include "tidop/experimental/photo.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geospatial/diffrect.h"

namespace tl
{

class ImageReader;
class VectorWriter;

namespace geospatial
{


class TL_EXPORT Footprint
{

public:

	Footprint(const std::string &dtm);
	~Footprint();

	void run(const std::vector<experimental::Photo> &photos,
					 const std::string &footprint);

private:

	void init();
	Affine<PointI> affineImageToPhotocoordinates();
	std::vector<tl::PointI> imageLimitsInPhotocoordinates();
	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits);
	//std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits,
	//																			const tl::math::RotationMatrix<double> &rotation_matrix,
	//																			const Point3D &position,
	//																			double focal);
	TL_TODO("Mover a calibration")
	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<VectorWriter> mVectorWriter;
	experimental::Camera mCamera;
	Affine<PointI> mAffineImageCoordinatesToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	Window<PointD> mWindowDtmTerrainExtension;
	std::unique_ptr<DifferentialRectification<double>> mDifferentialRectification;
};


} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_FOOTPRINT_H