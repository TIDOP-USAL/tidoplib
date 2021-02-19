/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

#ifndef TL_GEOSPATIAL_ORTHO_H
#define TL_GEOSPATIAL_ORTHO_H

#include <string>
#include <memory>

#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geospatial/diffrect.h"
#include "tidop/geospatial/camera.h"
#include "tidop/geospatial/photo.h"

namespace tl
{

class ImageReader;
class ImageWriter;
class VectorWriter;


namespace geospatial
{

class TL_EXPORT Orthorectification
{

public:

	Orthorectification(const std::string &dtm);
	~Orthorectification();

  void run(const std::vector<Photo> &photos,
					 const std::string &orthoPath,
					 const std::string &footprint);



private:

	void init();

	Affine<PointI> affineImageToPhotocoordinates();
	std::vector<tl::PointI> imageLimitsInPhotocoordinates();

	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits);
	Window<PointD> windowOrthoTerrain(const std::vector<Point3D> &footprint);

	TL_TODO("Mover a calibration")
	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	std::unique_ptr<VectorWriter> mVectorWriter;
	Camera mCamera;
	Affine<PointI> mAffineImageCoordinatesToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	Window<PointD> mWindowDtmTerrainExtension;
	std::unique_ptr<DifferentialRectification<double>> mDifferentialRectification;
};

} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H