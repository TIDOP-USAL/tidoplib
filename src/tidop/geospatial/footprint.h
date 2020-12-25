#ifndef TL_GEOSPATIAL_FOOTPRINT_H
#define TL_GEOSPATIAL_FOOTPRINT_H

#include <string>
#include <memory>

#include "tidop/experimental/camera.h"
#include "tidop/experimental/photo.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"

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

	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits,
																				const tl::math::RotationMatrix<double> &rotation_matrix,
																				const Point3D &position,
																				double focal);

private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<VectorWriter> mVectorWriter;
	experimental::Camera mCamera;
};


} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_FOOTPRINT_H